#include "ILI9163.h"

#define WHITE   rgb(255, 255, 255)
#define BLACK   rgb(0, 0, 0)

ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

void setup() {
    Serial.begin(9600);
    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
}

void loop() {
    tft.fill(rgb(255, 0, 0));
    tft.set_cursor(0, 0);
    tft.println("Testing print");
    tft.println("newline");
    tft.copy_buffer();
}
