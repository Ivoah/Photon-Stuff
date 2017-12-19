#include "ILI9163.h"

#define WHITE   rgb(255, 255, 255)
#define BLACK   rgb(0, 0, 0)

#define TIMEOUT 1000*10

#define IP      {10, 24, 102, 77}
#define PORT    1337

ILI9163 tft = ILI9163(D1, D0, D2); // cs, rst, a0

bool get(String req, uint8_t* buf) {
    TCPClient client;
    if (client.connect(IP, PORT)) {
        client.print("GET /");
        client.print(req);
        client.println(" HTTP/1.0");
        //client.println("Content-Length: 0");
        //client.println();
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
    if (get("tft", tft.buffer)) {
        // Hooray!
    } else {
        for (size_t i = 0; i < sizeof(tft.buffer)/sizeof(uint8_t); i += 2) {
            uint8_t r = random(256);
            uint16_t c = rgb(r, r, r);
            tft.buffer[i] = (c >> 8) & 0xff;
            tft.buffer[i + 1] = c & 0xff;
        }
    }
    tft.copy_buffer();
}

void loop() {
    if (System.buttonPushed()) {
        if (get("tft", tft.buffer)) {
            // Hooray!
        } else {
            for (size_t i = 0; i < sizeof(tft.buffer)/sizeof(uint8_t); i += 2) {
                uint8_t r = random(256);
                uint16_t c = rgb(r, r, r);
                tft.buffer[i] = (c >> 8) & 0xff;
                tft.buffer[i + 1] = c & 0xff;
            }
        }
        tft.copy_buffer();
    }
}
