/*
 * MCP3426/7/8 Libary
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 5/2012
 */

#include "MCP342X.h"
#include <Wire.h>

void MCP342X::init(byte A0, byte A1)
{
	if ((A0 == L && A1 == L) || (A0 == F && A1 == F)) I2C_ADDRESS = B1101000;
	else if (A0 == L && A1 == F) I2C_ADDRESS = B1101001;
	else if (A0 == L && A1 == H) I2C_ADDRESS = B1101010;
	else if (A0 == H && A1 == L) I2C_ADDRESS = B1101100;
	else if (A0 == H && A1 == F) I2C_ADDRESS = B1101101;
	else if (A0 == H && A1 == H) I2C_ADDRESS = B1101110;
	else if (A0 == F && A1 == L) I2C_ADDRESS = B1101011;
	else if (A0 == F && A1 == H) I2C_ADDRESS = B1101111;

	VRef = 2.048;
}

void MCP342X::selectResolution(byte resolution) 
{
	res = resolution & 3; 					// mask out everything but the low two bits
	// build configuration register
	confReg = (chan << BIT_C0) | (1 << BIT_OC) | (res << BIT_S0) | g;
	
	Serial.println(confReg);
	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(confReg);
	Wire.endTransmission();
}

void MCP342X::selectChannel(byte channel, byte gain)
{  
	chan = channel & 3;
	g = gain & 3;

    //configuration register, assuming 16 bit resolution
	confReg = (chan << BIT_C0) | (1 << BIT_OC) | (res << BIT_S0) | g;

	Serial.println(confReg);
	Wire.beginTransmission(I2C_ADDRESS);
	Wire.write(confReg);
	Wire.endTransmission();			   
}

double MCP342X::readADC()
{
	
	long int highCode, zeroCode;
	byte highByteMask;
	
	if (res == RESOLUTION_16) 
	{
		highCode = HIGH_CODE_16;
		zeroCode = ZERO_CODE_16;
		highByteMask = ADC_16_MASK;
	} 
	else if (res == RESOLUTION_14) 
	{
		highCode = HIGH_CODE_14;
		zeroCode = ZERO_CODE_14;
		highByteMask = ADC_14_MASK;
	} 
	else 
	{
		highCode = HIGH_CODE_12;
		zeroCode = ZERO_CODE_12;
		highByteMask = ADC_12_MASK;
	}
	
  	Wire.requestFrom(I2C_ADDRESS, (byte) 3);
  
	int h = Wire.read();
  	int l = Wire.read();
  	int r = Wire.read();
  
	h &= highByteMask;
  	long t = h << 8 |  l;
  	
	if (t >= zeroCode) t = highCode - t;
  	
	double v = (double) t * VRef/((double)(zeroCode));
	
	if (r & ADC_DATA_NEW_MASK) {
		dataNew = false;
	} else {
		dataNew = true;
	}

	return v;
}

bool MCP342X::isDataNew() 
{
	return dataNew;
}
