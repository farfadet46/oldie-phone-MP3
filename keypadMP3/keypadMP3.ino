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
//Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
#define NUM_SONGS 16
String SONGS[NUM_SONGS] = {"1500","1600","1700","1963","1980","1981","1982","1983","1984","1985","1986","1987","1988","1989","1990","2006"};

char CODE_ENTERED[4];
int CODE_COUNTER = 0;

//0 – waiting
//1 – proximity triggered, ringing
//2 – in use
//3 – hangup, idle for XXXXms
int MODE = 0;
int DEBUG = true;

long MODE_1_TIMER_START;
long MODE_1_DURATION = 5000; //rings for 5s

bool REPLAY_MODE = true; 
bool TIMEOUT_DIGIT = false;
bool PLAYED_A_SONG = false;
long MODE_2_TIMER_DIGIT_START;
long MODE_2_DURATION_DIGIT_TIMEOUT = 5000; //

long MODE_3_TIMER_START;
long MODE_3_DURATION = 5000; //idle for 5s after hangup


int VOL_SPEAKER = 10;
float VOL_SPEAKER_MULTIPLIER = 1.0; //0.0-3.0 (-= 0.5 each time)
int VOL_HEADSET = 28;

bool BROADCAST = false;

void setup() {
  mySoftwareSerial.begin(9600);
  
  //myDFPlayer.volume(5);
  
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
      
      //myDFPlayer.play("1234.mp3");
       myDFPlayer.stop();
       //myDFPlayer.randomAll(); 
       myDFPlayer.playMp3Folder(1983);
      Serial.println("ecoute 1983.mp3 situé dans le dossier MP3 !");
      }
  }

  delay(10);
}

void keypad_to_song(){
  /*
  //if(DEBUG==true){Serial.println("key");}
  
  char key = "1";
  /*
  if(key=='#'){ //increase volume

    if(DEBUG==true){Serial.println("#");}

    VOL_SPEAKER_MULTIPLIER -= 0.5;
    if(VOL_SPEAKER_MULTIPLIER<0){VOL_SPEAKER_MULTIPLIER=3.0;}
    VOL_SPEAKER = int(10.0*VOL_SPEAKER_MULTIPLIER);
    VOL_HEADSET = VOL_SPEAKER + 20;

    if(DEBUG==true){Serial.print(VOL_SPEAKER);Serial.print(",");Serial.print(VOL_HEADSET);Serial.println();}

    if(BROADCAST==true){
      //musicPlayer.setVolume(VOL_HEADSET,VOL_SPEAKER); //handset. speaker. 
      musicPlayer.setVolume(VOL_SPEAKER,VOL_HEADSET); 
    }else{
      //musicPlayer.setVolume(VOL_HEADSET,100); //handset. speaker off. 
      musicPlayer.setVolume(100,VOL_HEADSET);
    }
    
  }
  if(key=='*'){ //toggle broadcast
    
    BROADCAST = !BROADCAST;

    if(BROADCAST==true){
      //musicPlayer.setVolume(VOL_HEADSET,VOL_SPEAKER); //handset. speaker.  
      musicPlayer.setVolume(VOL_SPEAKER,VOL_HEADSET);
    }else{
      //musicPlayer.setVolume(VOL_HEADSET,100); //handset. speaker off. 
      musicPlayer.setVolume(100,VOL_HEADSET); //handset. speaker off. 
    }
    
  }

  if(key=='1'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t1.mp3");
    CODE_ENTERED[CODE_COUNTER] = '1';
  }
  if(key=='2'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t2.mp3");
    CODE_ENTERED[CODE_COUNTER] = '2';
  }
  if(key=='3'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t3.mp3");
    CODE_ENTERED[CODE_COUNTER] = '3';
  }
  if(key=='4'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t4.mp3");
    CODE_ENTERED[CODE_COUNTER] = '4';
  }
  if(key=='5'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t5.mp3");
    CODE_ENTERED[CODE_COUNTER] = '5';
  }
  if(key=='6'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t6.mp3");
    CODE_ENTERED[CODE_COUNTER] = '6';
  }
  if(key=='7'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t7.mp3");
    CODE_ENTERED[CODE_COUNTER] = '7';
  }
  if(key=='8'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t8.mp3");
    CODE_ENTERED[CODE_COUNTER] = '8';
  }
  if(key=='9'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t9.mp3");
    CODE_ENTERED[CODE_COUNTER] = '9';
  }
  if(key=='0'){
    musicPlayer.stopPlaying();
    musicPlayer.playFullFile("t0.mp3");
    CODE_ENTERED[CODE_COUNTER] = '0';
  }
  if(key=='0'||key=='1'||key=='2'||key=='3'||key=='4'||key=='5'||key=='6'||key=='7'||key=='8'||key=='9'){
    MODE_2_TIMER_DIGIT_START = millis(); TIMEOUT_DIGIT = true; REPLAY_MODE = false; PLAYED_A_SONG = false; 
    CODE_COUNTER++; if(CODE_COUNTER==4){code_entered();}
  }
  */
}


void code_entered(){
  /*
  TIMEOUT_DIGIT = false;

  String code_4 = String(CODE_ENTERED[0]) + String(CODE_ENTERED[1]) + String(CODE_ENTERED[2]) + String(CODE_ENTERED[3]);

  bool song_was_found = false;
  for(int i=0;i<NUM_SONGS;i++){
    if(code_4 == SONGS[i]){
      
      song_was_found = true;

      CODE_COUNTER = 0;
      TIMEOUT_DIGIT = false;
      
      if(DEBUG==true){Serial.println("Song Found...");Serial.println(SONGS[i]+".mp3");}
      
      String s = SONGS[i]+".mp3";
      char filename[9]; //learned the hard way about null termination. fuk you C! 
      s.toCharArray(filename, 9);
      musicPlayer.startPlayingFile(filename); 
      
      PLAYED_A_SONG = true;
    }
  }

  if(song_was_found == false){
    CODE_COUNTER = 0;
    TIMEOUT_DIGIT = false;
    musicPlayer.startPlayingFile("sorry2.mp3");  
  }
*/
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
