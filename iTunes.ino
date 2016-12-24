#include "ILI9163.h"

#define TIMEOUT 1000

#define IP      {192, 168, 1, 70}
#define PORT    1337

TCPClient client;
String image = String("iTunes");
ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

int change_image(String new_image) {
    image = new_image;
    return 0;
}

bool get(String req, uint8_t* buf, int len) {
    if (client.connect(IP, PORT)) {
        client.print("GET /");
        client.print(req);
        client.println(" HTTP/1.0");
        client.println("Content-Length: 0");
        client.println();
        char last[4] = {0};
        //char cl = [14] = {0};
        //int len = 0;
        unsigned int t = millis();
        while (strncmp(last, "\r\n\r\n", 4)) {
            if (millis() - t > TIMEOUT) break;
            if (client.available()) {
                char c = client.read();
                memmove(last, last + 1, 3);
                last[3] = c;
            }
        }
        for (int i = 0; i < len; i++) {
            if (millis() - t > TIMEOUT) break;
            if (client.available()) {
                buf[i] = client.read();
            } else {
                i--;
            }
        }
        return true;
    } else {
        return false;
    }
}

void setup() {
    Serial.begin(9600);
    Serial.println(WiFi.localIP());

    pinMode(D3, INPUT_PULLDOWN);

    Particle.function("change_image", change_image);

    tft.fill(rgb(0, 0, 0));
    tft.copy_buffer();
}

void loop() {
    if (!get(image, tft.buffer, sizeof(tft.buffer)/sizeof(uint8_t))) {
        for (size_t i = 0; i < sizeof(tft.buffer)/sizeof(uint8_t); i += 2) {
            uint8_t r = random(256);
            uint16_t c = rgb(r, r, r);
            tft.buffer[i] = (c >> 8) & 0xff;
            tft.buffer[i + 1] = c & 0xff;
        }
    }
    while (analogRead(A1) < 50) {get("volumeUp", NULL, 0); Particle.process();}
    while (analogRead(A1) > 4000) {get("volumeDown", NULL, 0); Particle.process();}
    if (analogRead(A0) < 50) {get("next", NULL, 0); while (analogRead(A0) < 50) Particle.process();}
    if (analogRead(A0) > 4000) {get("previous", NULL, 0); while (analogRead(A0) > 4000) Particle.process();}
    if (digitalRead(D3)) {get("playPause", NULL, 0); while (digitalRead(D3)) Particle.process();}
    tft.copy_buffer();
}
