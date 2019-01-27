#ifndef TeXie_h
#define TeXie_h

#include "Arduino.h"

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#endif

struct dataset
{
	String stream;
	char type;
	String value;
};

class TeXie
{
	public:
		TeXie();
		TeXie(char* account, char* secret);
		void addAP(char* ssid, char* password);
		bool connect();
		void handle_line(String line);
		bool read(String stream);
		void (*read_callback)(dataset d);
		void run();
		bool write(String stream, double value);
		bool write(String stream, int value);
		void set_read_callback(void (*callback)(dataset d));
		char* status();
	private:
		String _account;
		WiFiClient client;
		String _line;
		dataset _line_to_dataset(String line);
		ESP8266WiFiMulti wifiMulti;
		bool _wifi_state;
		String _secret;
		char* _state;
		
};

#endif