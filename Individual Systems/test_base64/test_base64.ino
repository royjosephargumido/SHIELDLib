#include "base64.hpp"

void setup() {
  Serial.begin(115200);
}

void loop() {
  unsigned char string[] = "String example";
  unsigned char base64[21]; // 20 bytes for output + 1 for null terminator
  
  // encode_base64() places a null terminator automatically, because the output is a string
  unsigned int base64_length = encode_base64(string, strlen((char *) string), base64);
  
  printf("%d\n", base64_length); // Prints "20"
  printf((char *) base64); // Prints "U3RyaW5nIGV4YW1wbGU="
}
