// This #include statement was automatically added by the Particle IDE.
#include "ILI9163.h"

#define YELLOW  rgb(255, 255, 0)
#define BLACK   rgb(0, 0, 0)

#define IP      {10, 23, 101, 234}
#define PORT    1337

TCPClient client;
ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

void setup() {
    Serial.begin(9600);
    Serial.println(WiFi.localIP());

    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
}

void loop() {
    if (client.connect(IP, PORT)) {
        int t = millis();
        for (int y = 0; y < ILI9163_HEIGHT; y++) memmove(tft.buffer + y*128*2, tft.buffer + y*128*2 + 2, 127*2);
        tft.draw_line(127, 127, 127, 0, BLACK);
        while (!client.available());
        char cpu = client.read();
        tft.draw_line(127, 127, 127, 127 - cpu, YELLOW);
        client.stop();
        tft.copy_buffer();
        delay(min(1000 - (millis() - t), 1000));
    }
}
