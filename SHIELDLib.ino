#include "SHIELDLib.h"

void setup() {
    Serial.begin(115200);

    shield.startDevice();

    Serial.println();
    Serial.println();

    Serial.println(shield.generateCircadian());
    Serial.println(shield.getProfile());

    /*

    //Creates the Circadian Key every 24 Hrs
    String circadian = shield.trng(CIRCADIAN, 16);

    // Generates the PUK, TUK, and the PID

    // PUK
    shield.trng(salt_puk, 16);                                        //Generate the salts for PUK using TRNG
    String puk = shield.perfHKDF(CIRCADIAN, salt_puk, INFO_PUK, 16);  //Generate the PUK using HKDF
    shield.stringtobyte(PUK, puk);                                    //Store the generated PUK

    // TUK
    shield.trng(salt_tuk, 16);                                        //Generate the salts for TUK using TRNG
    String tuk = shield.perfHKDF(CIRCADIAN, salt_tuk, INFO_TUK, 16);  //Generate the TUK using HKDF
    shield.stringtobyte(TUK, tuk);                                    //Store the generated TUK

    //PID
    String pid = shield.perfHKDF(CIRCADIAN, PUK, INFO_PID, 16);       //Generate the PID using HKDF
    shield.stringtobyte(PID, pid);                                    //Store the generated PID

    String cipher = shield.encrypt(CIRCADIAN, PUK, TUK);              // Encrypts PUK using CIRCADIAN and TUK in AES128-CTR
    byte cipher_data[16];
    shield.stringtobyte(cipher_data, cipher);                         //Store the generated ciphertext as bytes

    byte testPUk[MAX_BLOCKS];
    shield.stringtobyte(testPUk, cipher);                             //Converts the given ciphertext into a byte array

    // Decrypts the given ciphertxt using the CIRCADIAN and the TUK
    String plain = shield.decrypt(CIRCADIAN, testPUk, TUK);

    Serial.println("=========================================");
    Serial.println("CK: \t" + circadian);
    Serial.println("PUK:\t" + puk);
    Serial.println("TUK:\t" + tuk);
    Serial.println("PID:\t" + pid);
    Serial.println("=========================================");
    Serial.println("Ciphertext:\t" + cipher);
    Serial.println("PUK:\t\t" + puk);
    Serial.println("Decrypted:\t" + plain);
    Serial.println("=========================================");
    Serial.print("Decryption Status: ");
    if (plain == puk)
    Serial.println("Passed");
    else
    Serial.println("Failed");
    Serial.println("=========================================");

    // Encodes the ciphertext to Base64
    unsigned char base64[45];

    // encode_base64() places a null terminator automatically, because the output is a string
    unsigned int base64_length = shield.encode_base64((unsigned char*)cipher.c_str(), strlen(cipher.c_str()) + 1, base64);

    Serial.println(strlen(cipher.c_str()));
    Serial.print("Base64 Size:\t\t");
    Serial.println(base64_length);
    Serial.print("Base64:\t\t");
    Serial.println((char *) base64);

    // Decodes the Base64 string to ciphertext
    unsigned char string[33]; // 14 bytes for output + 1 for null terminator
    // decode_base64() does not place a null terminator, because the output is not always a string
    unsigned int string_length = shield.decode_base64(base64, string);
    string[string_length] = '\0';

    Serial.print("Decoded Base64:\t");
    Serial.println((char *) string);
    Serial.print("Original Cipher:\t");
    Serial.println(cipher);
    Serial.println("=========================================");
    String x = (char *) string;
    Serial.print("Base64 Status: ");
    if (x == cipher)
    Serial.println("Passed");
    else
    Serial.println("Failed");
    Serial.println("=========================================");
  */
}

void loop() {
  shield.displayDateTime();
}
