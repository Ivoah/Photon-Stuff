#include "ILI9163.h"

#define WHITE  rgb(255, 255, 255)
#define BLACK   rgb(0, 0, 0)

#define IP      {10, 23, 101, 234}
#define PORT    1337

TCPClient client;
ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

void setup() {
    Time.zone(-4);

    Serial.begin(9600);
    Serial.println(WiFi.localIP());

    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
}

void loop() {
    tft.fill(rgb(0, 0, 0));
    tft.draw_line(0, 11, 127, 11, WHITE);
    tft.draw_line(0, 12, 127, 12, WHITE);

    tft.set_cursor(2, 2);
    //tft.print(WiFi.SSID());

    uint8_t strength = map(WiFi.RSSI(), -127, -1, 0, 6);
    for (int i = 0; i < 7; i++) {
        if (strength >= i) {
            tft.draw_line(113 + i*2, 8, 113 + i*2, 8 - i, WHITE);
        } else {
            tft.draw_pixel(113 + i*2, 8, WHITE);
        }
    }

    char time[12];
    sprintf(time, "%02d:%02d:%02d %s", Time.hourFormat12(), Time.minute(), Time.second(), Time.isAM() ? "AM" : "PM");
    //String time = String(Time.hourFormat12()) + ":" + String(Time.minute()) + ":" + String(Time.second()) + (Time.isAM() ? " AM" : " PM");
    tft.print(time);

    tft.copy_buffer();
}
