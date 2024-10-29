
/*
  PPMReader.h
*/

#ifndef PPM_READER
#define PPM_READER

#include <Arduino.h>

class PPMReader {

public:
    // Public parameters to set range and error margins
    unsigned minChannelValue = 1000;
    unsigned maxChannelValue = 2000;
    unsigned channelValueMaxError = 10;
    unsigned blankTime = 2100;
    unsigned long failsafeTimeout = 500000L;

    // Setters for configuring parameters
    void setMinChannelValue(unsigned value) { minChannelValue = value; }
    void setMaxChannelValue(unsigned value) { maxChannelValue = value; }
    void setChannelValueMaxError(unsigned value) { channelValueMaxError = value; }
    void setBlankTime(unsigned value) { blankTime = value; }
    void setFailsafeTimeout(unsigned long value) { failsafeTimeout = value; }

    PPMReader(byte interruptPin, byte channelAmount, bool inverted = false);
    ~PPMReader(void);

    unsigned rawChannelValue(byte channel);
    unsigned latestValidChannelValue(byte channel, unsigned defaultValue);

private:
    byte interruptPin;
    byte channelAmount;
    volatile unsigned *rawValues;
    volatile byte pulseCounter = 0;
    volatile unsigned long microsAtLastPulse = 0;

    static PPMReader *ppm;
    static void PPM_ISR(void);

    void handleInterrupt(void);
};

#endif
