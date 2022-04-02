#include <AESLib.h>
#include <ESP8266TrueRandom.h>

AESLib aesLib;

byte PID[16];         // Profile Identifier - AES Plain Text
byte CIRCADIAN[16];   // Circadian - AES Encryption Key
byte PUK[16];         // Profile Unlocking Key - AES Nonce/IV

String createCircadian() {
  // Generate a new Circadian
  ESP8266TrueRandom.uuid(CIRCADIAN);
  
  return ESP8266TrueRandom.uuidToString(CIRCADIAN);
}

String createPUK() {
  // Generate a new Profile Unlocking Key (PUK)
  ESP8266TrueRandom.uuid(PUK);
  
  return ESP8266TrueRandom.uuidToString(PUK);
}

String createPID() {
  // Generate a new PID
  ESP8266TrueRandom.uuid(PID);

  return ESP8266TrueRandom.uuidToString(PID);
}

String getProfile(String pid, byte* nonce, byte* key) {
  char cleartext[256];  
  sprintf(cleartext, "%s", pid.c_str());
  uint16_t clen = String(cleartext).length();

  int cipherlength = aesLib.get_cipher64_length(clen);
  char encrypted[cipherlength];
  aesLib.encrypt64(cleartext, clen, encrypted, CIRCADIAN, sizeof(CIRCADIAN), nonce);

  return String(encrypted);
}

String decryptMessage(char* msg, uint16_t msgLen, byte iv[]) {
  char decrypted[msgLen];
  aesLib.decrypt64(msg, msgLen, decrypted, CIRCADIAN, sizeof(CIRCADIAN), iv);
  return String(decrypted);
}

String decryptProfile(String profile, byte nonce[]) {
  char _ciphertext[512];
  
  sprintf(_ciphertext, "%s", profile.c_str());
  uint16_t dlen = profile.length();
  String decrypted = decryptMessage(_ciphertext, dlen, nonce);

  return decrypted;
}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
  // Go trough the array and add two characters (for each nibble) to the string buffer
  // At the end add the null terminator.
  // See: https://stackoverflow.com/questions/44748740/convert-byte-array-in-hex-to-char-array-or-string-type-arduino
    for (unsigned int i = 0; i < len; i++)
    {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'a' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'a' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println();

  Serial.println("Circadian (Key):\t" + createCircadian());   //Creates the Circadian Key every 24 Hrs

  // Padding is used in a block cipher where we fill up the blocks with padding bytes
  // Since we use AES in CBC Mode (Cipher block chaining).
  // We uses CMS (Cryptographic Message Syntax) padding mode here.
  // This pads with the same value as the number of padding bytes.
  // Defined in RFC 5652, PKCS#5, PKCS#7 (X.509 certificate) and RFC 1423 PEM.
  // See: https://asecuritysite.com/symmetric/padding
  aesLib.set_paddingmode(paddingMode::CMS);

  //Creates the PID
  String curr_pid = createPID();
  Serial.println("PID (Cleartext):\t" + curr_pid);

  //Creates the Profile Unlocking Key (PUK)
  String puk = createPUK();
  Serial.println("PUK (IV/Nonce):\t" + puk);

  //Duplicates original nonce
  byte orig_nonce[16];
  byte ornonce[16];
  for (int i = 0; i < 16; i++) {
      orig_nonce[i] = PUK[i];
      ornonce[i] = PUK[i];
  }  
  String dup = ESP8266TrueRandom.uuidToString(orig_nonce);
  Serial.println("Duplicated Nonce:\t" + dup);

  //Creates the Profile out of the PID, PUK, and Circadian
  String profile = getProfile(curr_pid, PUK, CIRCADIAN);
  Serial.println("Profile: " + profile);
  
  // Decrypt the Profile using the Nonce
  String decrypted = decryptProfile(profile, orig_nonce);

  char cleartext[256];
  sprintf(cleartext, "%s", curr_pid.c_str());

  Serial.println("\nStatus:");
  Serial.println("====================================================");
  Serial.println("CIRRUS PID:\t" + decrypted);
  Serial.println("Local PID:\t" + String(cleartext));
  Serial.println("====================================================");
  
  if (decrypted.equals(cleartext))
    Serial.println("Decryption successful.");
  else
    Serial.println("Decryption failed.");

  char str[32] = "";
  array_to_string(ornonce, 16, str);
  Serial.println(str);
}

void loop() {
}