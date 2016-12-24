#include "ILI9163.h"

#define TIMEOUT 1000

#define IP      {192, 168, 1, 70}
#define PORT    1337

String image = String("iTunes");
ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

int change_image(String new_image) {
    image = new_image;
    return 0;
}

bool get(String req, uint8_t* buf) {
    TCPClient client;
    if (client.connect(IP, PORT)) {
        client.print("GET /");
        client.print(req);
        client.println(" HTTP/1.0");
        client.println("Content-Length: 0");
        client.println();
        String header;
        String val;
        int len = 0;
        unsigned int t = millis();
        while (client.read() != '\n'); // Consume "HTTP/1.0 200 OK"
        while (true) {
            if (millis() - t > TIMEOUT) {client.stop(); return false;} // Timeout
            if (client.available()) {
                char c = client.read(); // Get next character
                if (c == ':') { // If we find a colon
                    client.read(); // Consume the space character
                    do { // Loop while we haven't hit a newline
                        if (millis() - t > TIMEOUT) {client.stop(); return false;} // Timeout
                        c = client.read(); // Read next character
                        val.concat(c); // Add it to our value string
                    } while (c != '\n');
                    val.remove(val.length() - 2); // Dump the last two characters ("\r\n")
                    if (header.equals("Content-Length")) {
                        len = val.toInt();
                    }
                    header = String(); // New header
                    val = String(); // New value
                } else if (c == '\n') { // If we find a newline before we find a colon
                    break; // Break, we're done with the header
                } else { // If we don't have a colon or a newline
                    header.concat(c); // Add the character to our header string
                }
            }
        }

        for (int i = 0; i < len && client.connected(); i++) {
            if (millis() - t > TIMEOUT) {client.stop(); return false;}
            if (client.available()) {
                buf[i] = client.read();
            } else {
                i--;
            }
        }
        client.stop();
        return true;
    } else {
        client.stop();
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
    if (!get(image, tft.buffer)) {
        for (size_t i = 0; i < sizeof(tft.buffer)/sizeof(uint8_t); i += 2) {
            uint8_t r = random(256);
            uint16_t c = rgb(r, r, r);
            tft.buffer[i] = (c >> 8) & 0xff;
            tft.buffer[i + 1] = c & 0xff;
        }
    }
    while (analogRead(A1) < 50) {get("volumeUp", NULL); Particle.process();}
    while (analogRead(A1) > 4000) {get("volumeDown", NULL); Particle.process();}
    if (analogRead(A0) < 50) {get("next", NULL); while (analogRead(A0) < 50) Particle.process();}
    if (analogRead(A0) > 4000) {get("previous", NULL); while (analogRead(A0) > 4000) Particle.process();}
    if (digitalRead(D3)) {get("playPause", NULL); while (digitalRead(D3)) Particle.process();}
    tft.copy_buffer();
}
