// -------------------------------------------------------------------------------------------
// This module sets up the Teensy 3.2 as:
// - an I2C slave in communication to the Beaglebone Black (BBB)
// - handles the BBB's Debian built-in i2c commands
//   (1) /usr/sbin/i2cget i2cBus i2cAddr slaveReg               (i2cget 2 0x44 0x00)
//   (2) /usr/sbin/i2cset i2cBus i2cAddr slaveReg slaveRegData  (i2cget 2 0x44 0x00 0xAA)
//
//                             3.3 V          
//                             ---
//        BBB                   |              Teensy 3.2
//   ----------------           X 4.7 kOhm  ----------------
//   P9_19 : I2C2_SCL ----------|---------- pin19 : I2C0_SCL
//
//                             3.3 V          
//                             ---
//                              |
//                              X 4.7 kOhm
//                              |
//   P9_20 : I2C2_SDA ----------|---------- pin18 : I2C0_SDA
//
// - Register Map: stored in EEPROM (2kb=256kB)
//
//   @00   : ID1
//   @01   : ID2
//   @05-02: YYYYMMDD
//   @07-06: BPF leftF
//   @09-08: BPF RightF
// -------------------------------------------------------------------------------------------

#include <EEPROM.h>
#include <i2c_t3.h>

//** Constants **
const char         dev_i2cAddr   = 0x44; //
const unsigned int dev_ID        = 0x0001;  //2 bytes? 
const unsigned int dev_Date      = 0x20200331; //4bytes 
const unsigned int dev_BpfLeftF  = 0x044C;     //1100MHz 2byts
const unsigned int dev_BpfRightF = 0x0578;     //1400MHz
//
const          int ledPin13      = 13;

//** Rx/Tx I2C functions **
void receiveEvent(size_t count);
void requestEvent(void);

//** Memory **
#define      MEM_LEN  256
char memBuff[MEM_LEN];

//** Global variables **
volatile uint8_t rxEvent; // i2cget/i2cset
volatile uint8_t rxArgs;
volatile uint8_t rxAddr;
volatile uint8_t rxData;
//
volatile uint8_t txEvent; // i2cget

//* test live i2c connection talking from pin Teensy to BBB
//int analogPin = 20;
//int anlog_val =0;
void setup() {
  char         c=0x00; //Variable to store data read from EEPROM.
  unsigned int i=0;
  
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  pinMode(ledPin13,OUTPUT);
//  pinMode(analogPin,INPUT);
  //** initialize EEPROM **
  EEPROM.write(0, char( dev_ID            &0xFF));
  EEPROM.write(1, char((dev_ID       >> 8)&0xFF));
  EEPROM.write(2, char( dev_Date          &0xFF));
  EEPROM.write(3, char((dev_Date     >> 8)&0xFF));
  EEPROM.write(4, char((dev_Date     >>16)&0xFF));
  EEPROM.write(5, char((dev_Date     >>24)&0xFF));
  EEPROM.write(6, char( dev_BpfLeftF      &0xFF));
  EEPROM.write(7, char((dev_BpfLeftF >> 8)&0xFF));
  EEPROM.write(8, char( dev_BpfRightF     &0xFF));
  EEPROM.write(9, char((dev_BpfRightF>> 8)&0xFF));
  //
//for (i=10; i<EEPROM.length(); i++) {
//  EEPROM.write(i, 0);
//}
  Serial.printf("EEPROM Initialization completed!\n", rxAddr,rxData);

  /*
   * Set up output 
   */

  for (byte i = 14; i <=17; i++){
    pinMode(i,OUTPUT); //similar to set direction in linux
  }
  
  digitalWrite(ledPin13,HIGH);

  //** setup Teensy 3.2 I2C **
  //I2C Device : I2C Slave
  //I2C Address: dev_i2cAddr
  //I2C SDA    : pin18 (I2C0-SDA Teensy 3.2) - external pullup
  //I2C SCL    : pin19 (I2C0-SCL Teensy 3.2) - external pullup
  //baud rate  : 400kHz
  Wire.begin(I2C_SLAVE, dev_i2cAddr, I2C_PINS_18_19, I2C_PULLUP_EXT, 400000);

  //test
    /*Hard code memory??        */
   //device ID = 0x00001, will fix if doesn't need 2 bytes
   EEPROM.write(0,0);
   EEPROM.write(1,1);
  //date = 0x20200408, will write function to get date from PC or board
   EEPROM.write(2,20);
   EEPROM.write(3,20);  
   EEPROM.write(4,4);
   EEPROM.write(5,8);

//   EEPROM.write(10,0);
   
  //** Memory **
//memset(memBuff, 0, sizeof(memBuff));
  for (i=0; i<MEM_LEN; i++) {
    memBuff[i]=EEPROM.get(i,c);
  }
  //** Events **
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  rxEvent=0; rxArgs=0; rxAddr=0; rxData=0;
  txEvent=0;
}

void loop() {
  char         c=0x00; //Variable to store data read from EEPROM.
  unsigned int i;
  
//  anlog_val = analogRead(analogPin); //read from pin
//  EEPROM.write(1,anlog_val);
//  delay(200);
  
//  Serial.println(anlog_val);
  if (rxEvent) {
    switch (rxArgs) {
    case 1: //i2cget
      Serial.printf("i2cget @%02X : ", rxAddr);
      for (i=0;i<10;i++) {
        digitalWrite(ledPin13,HIGH); delay(25);
        digitalWrite(ledPin13,LOW);  delay(25);
      }
      rxEvent=0; break;
 /*     
    case 2: //i2cset
      memBuff[rxAddr]=rxData;
      //we know how many byte we want to send?
     for (int i = 1;i<6;i++){
      Wire.read(&memBuff[rxAddr+i],1);
      c = rxData+i;
      EEPROM.write(rxAddr+i,c);
    }
      Serial.printf("i2cset @%02X = %02X\n", rxAddr,rxData);
      for (i=0;i<10;i++) {
        digitalWrite(ledPin13,HIGH); delay(50);
        digitalWrite(ledPin13,LOW);  delay(50);
      }
      rxEvent=0; break;
*/      
    case 2:
      Serial.println(rxAddr);
      Serial.printf("i2cset @%02X = %02X\n", rxAddr,rxData);
      rxEvent=0;
     
 //   printf("Store value to EEPROM directly");
      break;
    default:
      Serial.printf("ERROR: invalid command\n");
      for (i=0;i<3;i++) {
        digitalWrite(ledPin13,HIGH); delay(500);
        digitalWrite(ledPin13,LOW);  delay(500);
      }
      rxEvent=0; break;
    
    }
  }
  if (txEvent) {
    txEvent=0;
    Serial.printf("tx @%02X = %02X\n", rxAddr,memBuff[rxAddr]);
  }
  digitalWrite(ledPin13,HIGH);
}

//** RxEvent (incoming I2C data) **
void receiveEvent(size_t count) {
  rxEvent = 1;
  rxArgs  = count;

  //might not need this because we know how many byte we need. Might be change to read() and do bitwise to segregate the byte
  rxAddr  = Wire.readByte();
  rxData  = Wire.readByte();
/* 
  //check the received byte by toggling the requested LED
    if (digitalRead (rxData) == LOW){
      digitalWrite(rxData,HIGH);
    }
    else{
      digitalWrite(rxData,LOW);
    }
    Serial.println(rxAddr);
  }
*/
//**TxEvent (outgoing I2C data) **
void requestEvent(void) {
  txEvent = 1;
  for (int i = 0;i<10;i++){
  Wire.write(&memBuff[rxAddr+i],1); //we know how many byte we need at which specific location
  }
}
