#include "math.h"

#include "ILI9163.h"

#define WHITE   rgb(255, 255, 255)
#define BLACK   rgb(0, 0, 0)

#define TIMEOUT 1000*10

#define IP      {10, 24, 102, 77}
#define PORT    1337

ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

#define xy_to_z(x, y) sqrt(64*64 - x*x - y*y)

void setup() {

}

void loop() {
    for (int y = -64; y < 64; y++) {
        for (int x = -64; x < 64; x++) {
            if (x*x + y*y < 64*64) {
                tft.draw_pixel(64 + x, 64 + y, rgb(0, xy_to_z(x, y)*256/64, 0));
            }
        }
    }
    tft.copy_buffer();
}
