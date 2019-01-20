#include "TeXie_arduino.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif
#include <Hash.h>

TeXie::TeXie()
{}

TeXie::TeXie(char* account, char* secret)
{
	_account = account;
	_secret = secret;
	_state = "disconnected";
	_wifi_state = false;
}

void TeXie::addAP(char* ssid, char* password)
{
	wifiMulti.addAP(ssid, password);
}

bool TeXie::connect()
{
	if(client.connect("api.texie.io", 23456))
	{
		_state = "connected";
	} else {
		_state = "disconnected";
	}
}

void TeXie::handle_line(String line)
{
	if(_state == "auth1")
	{
		String challenge = line.substring(3);
		String response = "XA"+_account+":"+sha1(challenge+_secret);
		Serial.print("Answer: ");Serial.println(response);
		client.print(response);client.print("\n");
		client.flush();
		_state = "auth2";
		return;
	} else if (_state == "auth2") {
		if(line == "AXAok") {
			_state = "ready";
		} else {
			_state = "login failed";
		}
		return;
	} else {
		(*read_callback)(line);
	}
}

bool TeXie::read(String stream)
{
	if(_state != "ready")
	{
		return false;
	}
	Serial.print("R"+stream);
	client.print("R"+stream);
	client.print("\n");
	Serial.print("\n");
	client.flush();
	return true;
}

void TeXie::run()
{
	if (wifiMulti.run() != WL_CONNECTED && _wifi_state == true) {
		_wifi_state = false;
		_state = "disconnected";
		Serial.println("WiFi disconnected connected!");
	} else if  (wifiMulti.run() == WL_CONNECTED && _wifi_state == false) {
		_wifi_state = true;
		Serial.println("Got connected to wifi...");
		Serial.println("WiFi connected");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());//
		TeXie::connect();
	}

	if (_wifi_state)
	{
		while (client.available() > 0) {
			// read the bytes incoming from the client:
			char thisChar = client.read();
			String nl = "\n";
			_line += thisChar;
			Serial.write(thisChar);
			if (_line.substring(_line.length()-1) == nl)
			{
				_line = _line.substring(0, _line.length()-1);
				Serial.println("\nGot line:");
				Serial.print(">");
				Serial.print(_line);
				Serial.println("<");	
				TeXie::handle_line(_line);
				_line = "";			
			}
		}
	}

	if (_wifi_state && _state == "connected")  // starting login
	{
		_state = "auth1";
		client.print("XH\n");
		client.flush();
	}
}

void TeXie::set_read_callback(void (*callback)(String line))
{
	read_callback = callback;
}

char* TeXie::status()
{
	return _state;
}

bool TeXie::write(String stream, int value)
{
	if(_state != "ready")
	{
		return false;
	}
	Serial.print("WI"+stream+":");
	client.print("WI"+stream+":");
	Serial.print(value);
	client.print(value);
	client.print("\n");
	Serial.print("\n");
	client.flush();
	return true;
}