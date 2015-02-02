#ifndef MCP342X_H
#define MCP342X_H

#include <Arduino.h>

/*
 * MCP3426/7/8 Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 5/2012
 */

class MCP342X {
public:
	static const byte L = 0;
	static const byte H = 1;
	static const byte F = 2;

	static const byte CHANNEL_0 = 0;
	static const byte CHANNEL_1 = 1;
	static const byte CHANNEL_2 = 2;
	static const byte CHANNEL_3 = 3;
	
	static const byte RESOLUTION_12 = 0;
	static const byte RESOLUTION_14 = 1;
	static const byte RESOLUTION_16 = 2;

	static const byte GAIN_1 = 0;
	static const byte GAIN_2 = 1;
	static const byte GAIN_4 = 2;
	static const byte GAIN_8 = 3; 
	double VRef;

	void init(byte A0, byte A1);
	void selectChannel(byte channel, byte gain = GAIN_1);
	void selectResolution(byte resolution);
	double readADC();
	bool isDataNew();
private:
	//communication register
	static const byte BIT_RDY = 7; //data ready
	static const byte BIT_C1 = 6; //channel select
	static const byte BIT_C0 = 5; //channel select
	static const byte BIT_OC = 4; //conversion mode (one shot/continuous)
	static const byte BIT_S1 = 3; //sample rate
	static const byte BIT_S0 = 2; //sample rate
	static const byte BIT_G1 = 1; //gain
	static const byte BIT_G0 = 0; //gain
	
	// ADC codes
	static const long int HIGH_CODE_16 = 65535;
	static const long int HIGH_CODE_14 = 16383;
	static const long int HIGH_CODE_12 = 4095;
	static const long int ZERO_CODE_16 = 32768;
	static const long int ZERO_CODE_14 = 8192;
	static const long int ZERO_CODE_12 = 2048;
	
	// ADC masks, used to clear the high bits from the high byte of the returned ADC value
	static const byte ADC_16_MASK = 0xff;
	static const byte ADC_14_MASK = 0x3f;
	static const byte ADC_12_MASK = 0x0f;
	static const byte ADC_DATA_NEW_MASK = 0x80;
	
	// internal configuration
	long int highCode = HIGH_CODE_16;
	long int zeroCode = ZERO_CODE_16;
	byte chan = CHANNEL_0;
	byte g = GAIN_1;
	byte res = RESOLUTION_16;
	byte confReg = (chan << BIT_C0) | (1 << BIT_OC) | (res << BIT_S0) | g;
	byte I2C_ADDRESS;
	bool dataNew = false;
};
#endif
