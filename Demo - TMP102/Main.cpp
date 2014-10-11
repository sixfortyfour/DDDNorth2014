// Main.cpp : Defines the entry point for the console application.
//

#include "windows.web.http.headers.h"
#include "stdafx.h"
#include "arduino.h"
#include "Wire.h"
#include <Winhttp.h>
#include <string>
#include "WinHttpClient.h"

#define TMP102_I2C_ADDRESS 0x48		// This is the I2C address for TMP102 chip

const uint8_t TMP102_START = 0x0;

// Helper function for logging to debug output and the console
void CustomLogging(char* str)
{
	OutputDebugStringA(str);		// For VS Output
	printf(str);					// For commandline output
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
	ULONG firstbyte, secondbyte;			// Bytes read from the TMP102 temperature registers
	int val;							// Two bytes stored together
	float convertedtemp;				// Multiply our two bytes by scaling factor, mentioned in the datasheet
	float correctedtemp;

	// Say hello
	Wire.beginTransmission(TMP102_I2C_ADDRESS);
	Wire.write(TMP102_START);
	Wire.endTransmission(false);

	delay(100);

	if (Wire.requestFrom(TMP102_I2C_ADDRESS, 2) != 1)
	{
		firstbyte = (Wire.read());
		secondbyte = (Wire.read());

		val = ((firstbyte) << 4);			// MSB into first 4 bits
		val |= (secondbyte >> 4);			// LSB ORed into second 4 bits

		convertedtemp = val*0.0625f;
		correctedtemp = convertedtemp - 5;

		char buffer[12];
		sprintf_s(buffer, sizeof(buffer), "%f\n", correctedtemp);
		CustomLogging(buffer);

		try
		{
			wstring url = L"http://iain-pc.home:55673/Home/Push/"; // Change to correct host
			wstring val2send = std::to_wstring((int)correctedtemp);
			url.append(val2send);

			WinHttpClient client(url);
			bool success = client.SendHttpRequest(L"POST");

			if (!success)
				CustomLogging("Cannot post value");
		}
		catch (const std::exception ex)
		{
			CustomLogging("Failed to reach temperature logger\n");
		}
	}

	delay(1000);
}