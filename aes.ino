#include <SPI.h>
#include <Ethernet.h>
#include <ArdOSC.h>
#include <IRremote.h>
#include <OneWire.h>
#include <Wire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
//#include <LiquidCrystal_I2C.h>
// LiquidCrystal_I2C lcd(0x38,20,4);  // set the LCD address to 0x20 for a 16 chars and 2 line display
float tempoC,tempiC;
int RECV_PIN = 11;
IRsend irsend;
IRrecv irrecv(RECV_PIN);
decode_results results;
byte myMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte myIp[]  = { 192, 168, 1, 177 };
int  serverPort  = 8000;
int ledPin =  9;
int t=100;
//char test1='+';
OSCServer server;
int destPort=9000;
byte destIp[]  = { 0, 0, 0, 0 };
byte clientIp0[50];
byte clientIp1[50];
byte clientIp2[50];
byte clientIp3[50];
int clientCount=0;
byte clientTtl[50];
OSCClient client;
OSCMessage global_mes;
int i=0;
int n=1;
void setup(){ 
 Serial.begin(19200);
 //  lcd.init();   // initialize the lcd 
 // lcd.backlight();
 irrecv.enableIRIn(); // Start the receiver
 Ethernet.begin(myMac ,myIp); 
 server.begin(serverPort);
 server.addCallback("/TV/BtPower",&TvPower);
 server.addCallback("/TV/BtMute",&TvMute);
 server.addCallback("/TV/BtPrev",&TvPrev);
 server.addCallback("/TV/BtAv",&TvAv);
 server.addCallback("/TV/BtVolUp",&TvVolUp);
 server.addCallback("/TV/BtVolDn",&TvVolDn);
 server.addCallback("/TV/BtChUp",&TvChUp);
 server.addCallback("/TV/BtChDn",&TvChDn);
 server.addCallback("/TV/Bt1",&Tv1);
 server.addCallback("/TV/Bt2",&Tv2);
 server.addCallback("/TV/Bt3",&Tv3);
 server.addCallback("/TV/Bt4",&Tv4);
 server.addCallback("/TV/Bt5",&Tv5);
 server.addCallback("/TV/Bt6",&Tv6);
 server.addCallback("/TV/Bt7",&Tv7);
 server.addCallback("/TV/Bt8",&Tv8);
 server.addCallback("/TV/Bt9",&Tv9);
 server.addCallback("/TV/Bt0",&Tv0);
 server.addCallback("/Apple/Up",&AUp);
 server.addCallback("/Apple/Dn",&ADn);
 server.addCallback("/Apple/Left",&ALeft);
 server.addCallback("/Apple/Right",&ARight);
 server.addCallback("/Apple/Ok",&AOk);
 server.addCallback("/Apple/Menu",&AMenu);
 server.addCallback("/Apple/Play",&APlay);
 server.addCallback("/TV",&TV);
 sensors.begin();
}
void irRecive(){
 if (irrecv.decode(&results)) {
  Serial.println(results.value, HEX);
 
 global_mes.setAddress(destIp,destPort);
 global_mes.beginMessage("/IRCode");
 global_mes.addArgFloat(results.value);
 client.send(&global_mes);
 global_mes.flush(); //object data clear

  dump(&results);
  irrecv.resume(); // Receive the next value
 }
} 

void loop(){
 if(server.aviableCheck()>0){
 }
 irRecive();
//if ((i%5)==0) lcdRis();
 if (i>=100) {
  inTemp();
  i=0;
 }
 i++;
 delay(50);
}
void dump(decode_results *results) {
 int count = results->rawlen;
 if (results->decode_type == UNKNOWN) {
  Serial.println("Could not decode message");
 } 
 else {
  if (results->decode_type == NEC) {
   Serial.print("Decoded NEC: ");
  } 
  else if (results->decode_type == SONY) {
   Serial.print("Decoded SONY: ");
  } 
  else if (results->decode_type == RC5) {
   Serial.print("Decoded RC5: ");
  } 
  else if (results->decode_type == RC6) {
   Serial.print("Decoded RC6: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
 }
 Serial.print("Raw (");
 Serial.print(count, DEC);
 Serial.print("): ");
 for (int i = 0; i < count; i++) {
  if ((i % 2) == 1) {
   Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
  } 
  else {
   Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
  }
  Serial.print(" ");
 }
 Serial.println("");
}
void TvPower(OSCMessage *_mes){
  float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x1000c, 12);}
  if (n==2){n=0;  irsend.sendRC5(0xc, 12);}
  n++;  
  }
}
void TvMute(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
 if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x80d, 12);}
  if (n==2){n=0;  irsend.sendRC5(0xd, 12);}
  n++;  
  }
}
void TvPrev(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1){ 
  if (n==1){irsend.sendRC5(0x822, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x22, 12);}
n++;
}
}
void TvAv(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x838, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x38, 12);}
  n++;  
  }
}
void TvVolUp(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x810, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x10, 12);}
  n++;  
  }
}
void TvVolDn(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x811, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x11, 12);}
  n++;  
  }
}
void TvChUp(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x820, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x20, 12);}
  n++;
  }
}
void TvChDn(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x821, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x21, 12);}
  n++;
  }
}
void Tv1(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x801, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x1, 12);}
  n++;  
  }
}
void Tv2(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
 if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x802, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x2, 12);}
  n++;  
  }
}
void Tv3(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x803, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x3, 12);}
  n++;  
  }
}
void Tv4(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x804, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x4, 12);}
  n++;  
  }
}
void Tv5(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x805, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x5, 12);}
  n++;  
  }
}
void Tv6(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x806, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x6, 12);}
  n++;  
  }
}
void Tv7(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x807, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x7, 12);}
  n++;  
  }
}
void Tv8(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x808, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x8, 12);}
  n++;  
  }
}
void Tv9(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x809, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x9, 12);}
  n++;  
  }
}
void Tv0(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
   if (tmpF==1) {
  if (n==1){irsend.sendRC5(0x800, 12);}
  if (n==2){n=0;  irsend.sendRC5(0x0, 12);}
  n++;  
  }
}

void AUp(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E1D035, 32);
}
void ADn(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E1B035, 32);
}
void ALeft(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E11035, 32);
}
void ARight(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E1E035, 32);
}
void AOk(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E1BA35, 32);
}
void AMenu(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E14035, 32);
}
void APlay(OSCMessage *_mes){
 float tmpF=_mes->getArgFloat(0);
  if (tmpF==1) irsend.sendNEC(0x77E17A35, 32);

 
}
void TV(OSCMessage *_mes){
 Serial.println("TV");

  byte *sourceIp = _mes->getIpAddress();
  Serial.print("source Ip=");
  Serial.print(sourceIp[0]);Serial.print(".");
  Serial.print(sourceIp[1]);Serial.print(".");
  Serial.print(sourceIp[2]);Serial.print(".");
  Serial.println(sourceIp[3]);
 int newClient=1;
 for (int ic=0; ic <= clientCount; ic++){
   if (clientIp0[ic]==sourceIp[0] and clientIp1[ic]==sourceIp[1] and clientIp2[ic]==sourceIp[2] and clientIp3[ic]==sourceIp[3]){
    clientTtl[ic]=128; 
    newClient=0;
   }
 }
 if (newClient>0) {
  clientIp0[clientCount]=sourceIp[0];
  clientIp1[clientCount]=sourceIp[1];
  clientIp2[clientCount]=sourceIp[2];
  clientIp3[clientCount]=sourceIp[3];
  clientTtl[clientCount]=255;
  clientCount++;
 }
  
  

 }
 
void inTemp(){
 sensors.requestTemperatures();
 tempoC = sensors.getTempCByIndex(0);
 tempiC = sensors.getTempCByIndex(1);
 Serial.println(tempoC);
 Serial.println(tempiC);
 
  Serial.print("client count=");
 Serial.println(clientCount);
  for (int ic=0; ic < clientCount; ic++){
  Serial.print(clientIp0[ic]);Serial.print(".");
  Serial.print(clientIp1[ic]);Serial.print(".");
  Serial.print(clientIp2[ic]);Serial.print(".");
  Serial.print(clientIp3[ic]);
  Serial.print(" ttl="); Serial.println(clientTtl[ic]);
  
}
 for (int ic=0; ic < clientCount; ic++){
 if (clientTtl[ic]>0) {
 destIp[0]=clientIp0[ic];
 destIp[1]=clientIp1[ic];
 destIp[2]=clientIp2[ic];
 destIp[3]=clientIp3[ic];
 Serial.print("Send to ");
 Serial.print(destIp[0]);Serial.print(".");
 Serial.print(destIp[1]);Serial.print(".");
 Serial.print(destIp[2]);Serial.print(".");
 Serial.println(destIp[3]);
 global_mes.setAddress(destIp,destPort);
 global_mes.beginMessage("/LbTin");
 global_mes.addArgFloat(tempiC);
 client.send(&global_mes);
 global_mes.flush(); //object data clear
 global_mes.setAddress(destIp,destPort);
 global_mes.beginMessage("/LbTout");
 global_mes.addArgFloat(tempoC);
 client.send(&global_mes);
 global_mes.flush(); //object data clear
 }
 if (clientTtl[ic]>0) {clientTtl[ic]--;}
 
 }

}
/*void lcdRis(){
 // lcd.backlight();
 // sensors.requestTemperatures();
 //t tempoC = sensors.getTempCByIndex(0);
 //float tempiC = sensors.getTempCByIndex(1);
 
 //lcd.setCursor(0,0);  lcd.print(test1);
if (test1=='x') test1='+'; else test1='x';

 //lcd.setCursor(10,0); lcd.print (hour());lcd.print (":");lcd.print (minute());lcd.print (":");lcd.print (second());
 //lcd.setCursor(0,1); 
 lcd.print ("Out ");lcd.print (tempoC);
 lcd.setCursor(10,0); lcd.print ("In ");lcd.print (tempiC);
 //lcd.setCursor(0,2);  lcd.print ("Prognoz ");lcd.print (tempPrognoz);
 //lcd.setCursor(10,2); lcd.print ("");
 lcd.setCursor(0,3); lcd.print ("MENU");
 lcd.setCursor(5,3); lcd.print ("UP");
 lcd.setCursor(10,3); lcd.print ("DN");
 lcd.setCursor(15,3); lcd.print ("ENTER"); 
}*/
