#include <Crypto.h>
#include <RNG.h>

// Change "MyApp 1.0" to some other tag for your application
// so that different applications will generate different results
// even if the input noise or seed data is otherwise identical.
#define RNG_APP_TAG "SHIELD"

byte data[32];
unsigned long startTime;
size_t length = 48; // First block should wait for the pool to fill up.

void setup() {
    Serial.begin(9600);
    Serial.println("start");

    // Initialize the random number generator.
    RNG.begin(RNG_APP_TAG);

    startTime = millis();
}

void printHex(const byte *data, unsigned len)
{
  Serial.println(len);
    static char const hexchars[] = "0123456789ABCDEF";
    while (len > 0) {
        int b = *data++;
        Serial.print(hexchars[(b >> 4) & 0x0F]);
        Serial.print(hexchars[b & 0x0F]);
        --len;
    }
    Serial.println();
}

void loop() {
    // Perform regular housekeeping on the random number generator.
    RNG.loop();

    // Generate output whenever 32 bytes of entropy have been accumulated.
    // The first time through, we wait for 48 bytes for a full entropy pool.
    if (RNG.available(length)) {
        RNG.rand(data, sizeof(data));
        printHex(data, sizeof(data));
        length = 32;
    }
    //delay(100);
}
