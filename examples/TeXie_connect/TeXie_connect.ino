#include "TeXie_arduino.h"
#include "Arduino.h"

TeXie texie;
int rounds = 0;

void print_line(String line)
{
	if (line.substring(0, 2) == "AR")
	{
		int sep;
		for (int i = 2; i < line.length(); i++)
		{
			if(line.substring(i, i+1) == ":")
			{
				sep = i;
				break;
			}
		}
		Serial.println("Got read value: "+line.substring(2, sep)+" ("+line.substring(sep+1, sep+2)+") "+line.substring(sep+2));
	} else {
		Serial.print("Callback with line: >");
		Serial.print(line);
		Serial.println("<");
	}
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
	if (texie.status() == "ready" && rounds >= 100)
	{
		rounds = 0;
		Serial.print("Texie-Status: ");
		Serial.println(texie.status());
		texie.write("demo-up", 1);
		delay(1000);
		texie.read("TIME");
	}
	delay(100);
}