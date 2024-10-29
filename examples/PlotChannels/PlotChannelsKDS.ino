#include <PPMReader.h>

byte interruptPin = 2;
byte channelAmount = 8;
bool invertedSignal = true; // Set to true for inverted PPM signals

PPMReader ppm(interruptPin, channelAmount, invertedSignal);

void setup() {
    Serial.begin(115200);

    // Set custom values for minChannelValue, maxChannelValue, channelValueMaxError, blankTime, and failsafeTimeout
    ppm.setMinChannelValue(850);           // Adjust as required
    ppm.setMaxChannelValue(2200);           // Adjust as required
    ppm.setChannelValueMaxError(15);        // Adjust as required
    ppm.setBlankTime(3500);                 // Adjust as required
    ppm.setFailsafeTimeout(500000L);        // Adjust as required
}

void loop() {
    // Print latest valid values from all channels
    for (byte channel = 1; channel <= channelAmount; ++channel) {
        unsigned value = ppm.latestValidChannelValue(channel, 0);
        Serial.print(value);
        if(channel < channelAmount) Serial.print('\t');
    }
    Serial.println();
    delay(20);
}
