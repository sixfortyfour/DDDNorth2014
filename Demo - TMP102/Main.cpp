// Main.cpp : Defines the entry point for the console application.
//

#include "windows.web.http.headers.h"
#include "stdafx.h"
#include "arduino.h"
#include "Wire.h"
#include <Winhttp.h>
#include <string>
#include "WinHttpClient.h"

#define TMP102_I2C_ADDRESS 72			// This is the I2C address for TMP102 chip

// Helper function for logging to debug output and the console
void CustomLogging(char* str)
{
	OutputDebugStringA(str); // for VS Output
	printf(str); // for commandline output
}

int _tmain(int argc, _TCHAR* argv[])
{
    return RunArduinoSketch();
}


void setup()
{
	Wire.begin();
}

// the loop routine runs over and over again forever:
void loop()
{
	byte firstbyte, secondbyte;			// Bytes read from the TMP102 temperature registers
	int val;							// Two bytes stored together
	float convertedtemp;				// Multiply our two bytes by scaling factor, mentioned in the datasheet
	float correctedtemp;

	// Say hello
	Wire.beginTransmission(TMP102_I2C_ADDRESS);
	Wire.write("0x00");
	Wire.endTransmission();

	Wire.requestFrom(TMP102_I2C_ADDRESS, 2);
	Wire.endTransmission();

	firstbyte = (Wire.read());
	secondbyte = (Wire.read());

	val = ((firstbyte) << 4);			// MSB into first 4 bits
	val |= (secondbyte >> 4);			// LSB ORed into second 4 bits

	convertedtemp = val*0.0625;
	correctedtemp = convertedtemp - 5;

	wstring url = L"http://tmp102webhost:55673/Home/Push/"; // Change to correct host
	wstring val2send = std::to_wstring(correctedtemp);
	url.append(val2send);

	WinHttpClient client(url);
	bool success = client.SendHttpRequest(L"POST");

	if (!success)
		CustomLogging("Cannot post value");

	delay(5000);
}