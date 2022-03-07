#include <SoftwareSerial.h>

SoftwareSerial ble_device(D3,D2);

String str_ii = "";
int ii_0 = 0;

void setup() {  
  Serial.begin(115200);
  ble_device.begin(9600);
  delay(100);
 
  ble_help(); // uncomment to print commands
  delay(100);
  Serial.println("Running...");
 
  // Enter AT+ commands of interest here (BLE Address, UUIDs, Power settings)
  //ble_cmd("AT+NAMEBLExAR","Device Name: "); // printout device name
  //ble_cmd("AT+LADDR","Address: "); // printout BLE address
  //ble_cmd("AT+CHAR","Char UUID: "); // printout character UUID
  //ble_cmd("AT+VERSION","Version: "); // module version  
  //ble_cmd("AT+RESET",""); // reset BLE module
}

void loop() {

}

String ble_cmd(String cmd_str,String desc_str){
  str_ii = "";
  unsigned long t1 = millis();
  ble_device.println(cmd_str);
  while (true){
    char in_char = ble_device.read();
    if (int(in_char)==-1 or int(in_char)==42){
      if ((millis()-t1)>2000){ // 2 second timeout
        return "Err";
      }
      continue;
    }
    if (in_char=='\n'){
      Serial.print("Bluetooth "+desc_str);
      Serial.println(str_ii.substring(0,str_ii.length()));
      return str_ii;
    }
    str_ii+=in_char;
  }
}

void ble_help(){
  ble_device.println("AT+HELP"); // list all AT+ commands
  while (true){ // loop to print all AT+ commands
    char in_char = ble_device.read();
    if (int(in_char)==-1 or int(in_char)==42){continue;}
    str_ii+=in_char;
    if (in_char=='\n'){
      if (str_ii==String('\r')+String('\n')){
        if (ii_0 == 0){
          ii_0 = 1;
          continue;
        }
        break; // break after more than 1 empty carriage return and newline
      }
      Serial.print(str_ii);
      str_ii = "";
    }
  }
}
