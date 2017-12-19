#include "PerlinNoise.h"
#include "ILI9163.h"

#include <cmath>

#define TIMEOUT 1000

TCPClient client;
PerlinNoise pn;
ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

void setup() {
    Serial.begin(9600);
    Serial.println(WiFi.localIP());

    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
    for (int x = 0; x < ILI9163_WIDTH; x++) {
        for (int y = 0; y < ILI9163_HEIGHT; y++) {
            double r = pn.noise(x/50.0, y/50.0, 0.8);
            //r *= 25;
            //r = r - floor(r);
            r = r*255;
            tft.draw_pixel(x, y, rgb(r, r, r));
        }
        tft.copy_buffer();
    }
}

int x = 128;

void loop() {
    //get(image, buffer, sizeof(tft.buffer)/sizeof(uint8_t));
    //memmove(buffer, buffer + 128*2, sizeof(tft.buffer)/sizeof(uint8_t) - 128*2);
    for (int y = 0; y < ILI9163_HEIGHT; y++) memmove(tft.buffer + y*128*2, tft.buffer + y*128*2 + 2, 127*2);
    for (int y = 0; y < ILI9163_HEIGHT; y++) {
        double r = pn.noise(x/50.0, y/50.0, 0.8);
        //r *= 25;
        //r = r - floor(r);
        r = r*255;
        tft.draw_pixel(127, y, rgb(r, r, r));
    }
    x++;
    tft.copy_buffer();
}
