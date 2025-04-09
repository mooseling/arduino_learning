#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

unsigned long frame[] = {
    0x88053,
    0x83444d44,
    0x9440a201
};


void setup() {
  Serial.begin(115200);
  matrix.begin();
}


void loop() {
  matrix.loadFrame(frame);
  translateFrame();
  delay(300);
}


void translateFrame() {
  // Mostly we just bit shift to do this, but this will give 10 wrong pixels:
  // Each pixel at the beginning of a row (8)
  // And each pixel at the beginning of a long (only 2, since we covered the first such above)
  // These we will extract by hand

  // frame[0] has 3 values we need to find
  // Extract the starts of row 0 and 1, which just need to move within frame[0]
  long handExtracted1 =            (frame[0] << 11) & 2148007936; // Bitmask 1000 0000 0000 1000 0000 0000 0000 0000

  // Add the start of row 2. This value is at index 3 of frame[1]
  handExtracted1 = handExtracted1 | (frame[1] >> 21) & 128;        // Bitmask 0000 0000 0000 0000 0000 0000 1000 0000

  // frame[1] has 4 values we need to find
  // Extract the start of frame[1]. This value is the last bit on frame[0]
  long handExtracted2 =             (frame[0] << 31) & 2147483648; // Bitmask 1000 0000 0000 0000 0000 0000 0000 0000

  // Add the start of rows 3 and 4, which just need to move within frame[1]
  handExtracted2 = handExtracted2 | (frame[1] << 11) & 134250496;  // Bitmask 0000 1000 0000 0000 1000 0000 0000 0000

  // Add the start of row 5, which is in frame[2]
  handExtracted2 = handExtracted2 | (frame[1] << 11) & 134250496;

  // frame[2] has 3 values we need to find
  // Extract the start of frame[2]. The value is the last bit in frame[1]
  long handExtracted3 =             (frame[1] << 31) & 2147483648; // Bitmask 1000 0000 0000 0000 0000 0000 0000 0000

  // Add the starts of rows 6 and 7, which just need to move within frame[2]
  handExtracted3 = handExtracted3 | (frame[2] << 11) & 8390656;    // Bitmask 0000 0000 1000 0000 0000 1000 0000 0000

  // Now we have 3 longs with particular bits correct... How do we apply just those bits to another number?
  // Step 1: The output is the long shifted right one
  // Step 2: The output is combined with a bitmask so that the spots we are going to fill all become 0
  //    This just requires a normal bitmask: 1 where we let through, 0 where we want 0. Use AND (&).
  // Step 3: The output is combined with handExtracted so that:
  //    A 0 in handExtracted lets through the value in output
  //    A 1 in handExtracted goes through as 1
  //    OR satisfies this (|)
  // Bitmasks:
  // frame[0]: 0111 1111 1111 0111 1111 1111 0111 1111 = 2146959231
  // frame[1]: 0111 0111 1111 1111 0111 1111 1111 0111 = 2013233143
  // frame[2]: 0111 1111 0111 1111 1111 0111 1111 1111 = 2139092991
  
  frame[0] = ((frame[0] >> 1) & 2146959231) | handExtracted1;
  frame[1] = ((frame[1] >> 1) & 2013233143) | handExtracted2;
  frame[2] = ((frame[2] >> 1) & 2139092991) | handExtracted3;
}
