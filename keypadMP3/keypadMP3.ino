#include <Adafruit_Keypad.h>
#include <Adafruit_Keypad_Ringbuffer.h>

// define your specific keypad here via PID
#define KEYPAD_PID1824
// define your pins here
// can ignore ones that don't apply
#define R1    2
#define R2    3
#define R3    4
#define R4    5
#define C1    8
#define C2    9
#define C3    10
//#define C4    11
// leave this import after the above configuration
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


SoftwareSerial mySoftwareSerial(6, 7); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

#include "keypad_config.h"
//initialize an instance of class NewKeypad
Adafruit_Keypad kpd = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char key ="";
char CODE_ENTERED[4];
int CODE_COUNTER = 0;

//0 – waiting
//1 – proximity triggered, ringing
//2 – in use
//3 – hangup, idle for XXXXms
//int MODE = 0;
int DEBUG = true;

//bool REPLAY_MODE = true; 
bool TIMEOUT_DIGIT = false;
bool PLAYED_A_SONG = false;
long MODE_2_TIMER_DIGIT_START;
long MODE_2_DURATION_DIGIT_TIMEOUT = 5000; //

long MODE_3_TIMER_START;
long MODE_3_DURATION = 5000; //idle for 5s after hangup


int VOL_SPEAKER = 50;
float VOL_SPEAKER_MULTIPLIER = 1.0; //0.0-3.0 (-= 0.5 each time)

void setup() {
  mySoftwareSerial.begin(9600);
  
  myDFPlayer.volume(VOL_SPEAKER);
  
  Serial.begin(115200);
  if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  Serial.println("starting...");

  int valeur = myDFPlayer.readFileCounts();
  Serial.println(valeur);
  
  //keyboard start :
  kpd.begin();
  Serial.print("OK, waiting for input.");
}

void loop() {
  // put your main code here, to run repeatedly:
  kpd.tick();

  while(kpd.available()){
    keypadEvent e = kpd.read();
    Serial.print((char)e.bit.KEY);
    if(e.bit.EVENT == KEY_JUST_PRESSED) Serial.println(" pressed");
    else if(e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println(" released");
      
      // myDFPlayer.playMp3Folder(1983);
      //Serial.println("ecoute 1983.mp3 situé dans le dossier MP3 !");
      key = char(e.bit.KEY);
      keypad_to_song();
      }
  }
  delay(10);
}

void keypad_to_song(){
  
  //char key = "1";
  
  
  if(key=='#'){
    //reset le 4 digit !
    
    CODE_COUNTER = 0;
    TIMEOUT_DIGIT = false;
    Serial.println("Restart ! OK");
    //jouer un son explicite de RAZ !
    myDFPlayer.playMp3Folder(1);
    
  }
  if(key=='*'){
    myDFPlayer.playMp3Folder(1);
  }
  if(key=='1'){
    myDFPlayer.playMp3Folder(11);
    CODE_ENTERED[CODE_COUNTER] = '1';
    Serial.println("OH un 1 !");
  }
  if(key=='2'){
   myDFPlayer.playMp3Folder(12);
    CODE_ENTERED[CODE_COUNTER] = '2';
    Serial.println("OH un 2 !");
  }
  if(key=='3'){
    myDFPlayer.playMp3Folder(13);
    CODE_ENTERED[CODE_COUNTER] = '3';
  }
  if(key=='4'){
    myDFPlayer.playMp3Folder(14);
    CODE_ENTERED[CODE_COUNTER] = '4';
  }
  if(key=='5'){
    myDFPlayer.playMp3Folder(15);
    CODE_ENTERED[CODE_COUNTER] = '5';
  }
  if(key=='6'){
    myDFPlayer.playMp3Folder(16);
    CODE_ENTERED[CODE_COUNTER] = '6';
  }
  if(key=='7'){
    myDFPlayer.playMp3Folder(17);
    CODE_ENTERED[CODE_COUNTER] = '7';
  }
  if(key=='8'){
    myDFPlayer.playMp3Folder(18);
    CODE_ENTERED[CODE_COUNTER] = '8';
  }
  if(key=='9'){
    myDFPlayer.playMp3Folder(19);
    CODE_ENTERED[CODE_COUNTER] = '9';
  }
  if(key=='0'){
    myDFPlayer.playMp3Folder(10);
    CODE_ENTERED[CODE_COUNTER] = '0';
  }
  if(key=='0'||key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'){
    MODE_2_TIMER_DIGIT_START = millis();
    TIMEOUT_DIGIT = true;
   // PLAYED_A_SONG = false;
    CODE_COUNTER++;
    if(CODE_COUNTER==4){
      //si on est à 4 chiffre entré, on passe a la suite
      code_entered();
    }
  }
}


void code_entered(){
  
   // LECTURE DE LA MUSIQUE ENTREE
   
  TIMEOUT_DIGIT = false;
  CODE_COUNTER = 0;
  
  int code_4 = (CODE_ENTERED[0]) + (CODE_ENTERED[1]) + (CODE_ENTERED[2]) + (CODE_ENTERED[3]);
  Serial.println(code_4);
  myDFPlayer.playMp3Folder(code_4);
  PLAYED_A_SONG = true;
}

void play_song(String song){
/*
      CODE_COUNTER = 0;
      TIMEOUT_DIGIT = false;
      
      String s = song+".mp3";

      if(DEBUG==true){Serial.println("Jukebox...");Serial.println(song+".mp3");}

      
      char filename[9]; //learned the hard way about null termination. fuk you C! 
      s.toCharArray(filename, 9);
      musicPlayer.startPlayingFile(filename); 
*/
}
