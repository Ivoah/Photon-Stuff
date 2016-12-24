#include "ILI9163.h"

#define WHITE   rgb(255, 255, 255)
#define BLACK   rgb(0, 0, 0)

ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

void setup() {
    Serial.begin(9600);
    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
}

int p = 0;
int np = 0;

void loop() {
    for (int y = 0; y < ILI9163_HEIGHT; y++) {
        memmove(tft.buffer + y*128*2, tft.buffer + y*128*2 + 2, 127*2);
        tft.draw_pixel(127, y, BLACK);
    }
    np = map(analogRead(A1), 0, 4095, 0, 127);
    tft.draw_pixel(127, p, BLACK);
    tft.draw_line(126, p, 127, np, WHITE);
    p = np;
    tft.copy_buffer();
}
