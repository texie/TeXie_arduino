#include "TeXie_arduino.h"
#include "Arduino.h"

TeXie texie;
int rounds = 0;

void print_line(dataset d)
{
	Serial.println("Got read value: "+d.stream+" ("+d.type+") "+d.value);
}

void setup()
{
	Serial.begin(115200);
	texie = TeXie("demo", "demo");
	texie.addAP("ssid1", "pass1");
	texie.addAP("ssid2", "pass2");
	texie.set_read_callback(&print_line);
}

void loop()
{
	rounds++;
	texie.run();
	if (rounds >= 100)
	{
		rounds = 0;
		Serial.print("Texie-Status: ");
		Serial.println(texie.status());
		if(texie.status() == "ready")
		{
			texie.write("demo-up", 1);
			texie.write("demo-up/2", 12.12345);
			delay(1000);
			texie.read("TIME");
			texie.read("demo-up/2");
		};
	}
	delay(100);
}