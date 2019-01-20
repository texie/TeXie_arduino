#ifndef TeXie_h
#define TeXie_h

#include "Arduino.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif

class TeXie
{
	public:
		TeXie();
		TeXie(char* account, char* secret);
		void addAP(char* ssid, char* password);
		bool connect();
		void handle_line(String line);
		bool read(String stream);
		void run();
		void set_read_callback(void (*callback)(String line));
		char* status();
		bool write(String stream, int value);
		void (*read_callback)(String line);
	private:
		String _account;
		String _secret;
		char* _state;
		bool _wifi_state;
		String _line;
		WiFiClient client;
		ESP8266WiFiMulti wifiMulti;
};

#endif