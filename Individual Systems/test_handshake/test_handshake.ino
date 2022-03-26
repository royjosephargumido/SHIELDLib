/*
 * Cleartext  :   PID
 * Key        :   Circadian
 * IV/Nonce   :   PUK
 */

#include "AESLib.h"
#include "ESP8266TrueRandom.h"

AESLib aesLib;
byte uuid_pid[16];        //plain text

char ciphertext[512];

// AES Encryption Key
byte CIRCADIAN[16];

// IV/Nonce
byte PUK[16];
byte nonce[16];

String createCircadian() {
  // Generate a new Circadian
  ESP8266TrueRandom.uuid(CIRCADIAN);
  String circadian = ESP8266TrueRandom.uuidToString(CIRCADIAN);
  Serial.println("Circadian (Key): " + circadian);

  return circadian;
}

String createPUK() {
  // Generate a new Profile Unlocking Key (PUK)
  ESP8266TrueRandom.uuid(PUK);
  aesLib.gen_iv(PUK);
  String puk = ESP8266TrueRandom.uuidToString(PUK);

  // Generate a new Circadian
  ESP8266TrueRandom.uuid(CIRCADIAN);
  String circadian = ESP8266TrueRandom.uuidToString(CIRCADIAN);
  Serial.println("Circadian (Key): " + circadian);
  return puk;
}

String createPID() {
  // Generate a new PID
  ESP8266TrueRandom.uuid(uuid_pid);
  String pid = ESP8266TrueRandom.uuidToString(uuid_pid);
  Serial.println("PID (Cleartext): " + pid);

  return pid;
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

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println();

  createCircadian();  //Creates the Circadian Key every 24 Hrs
  
  String puk = createPUK();    //Create the PUK
  Serial.println("PUK (IV/Nonce): " + puk);

  aesLib.set_paddingmode(paddingMode::CMS);
  
  String curr_pid = createPID();
  char cleartext[256];
  sprintf(cleartext, "%s", curr_pid.c_str());

  // Encrypt
  byte encryptionNonce[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  String profile = getProfile(curr_pid, encryptionNonce, CIRCADIAN);
  Serial.println("Profile: " + profile);
  
  // Decrypt
  char _ciphertext[512];
  sprintf(_ciphertext, "%s", profile.c_str());
  
  uint16_t dlen = profile.length();
  byte decryptionNonce[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
  String decrypted = decryptMessage(_ciphertext, dlen, decryptionNonce);
  
  if (decrypted.equals(cleartext))
    Serial.println("Decryption successful.");
  else
    Serial.println("Decryption failed.");
}

void loop() {
}