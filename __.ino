#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>

static uint8_t recv_cmd[8] = {};
const int u1 = 11;
const int u2 = 12;
const int u3 = 13;
const int b = 10;
const int h = 8;
const int l = A0;

int u1S = 0;
int u2S = 0;
int u3S = 0;
int bS = 0;
int lS = 0;

int j = 0;
int k = 0;
int k1 = 0;
int k2 = 0;
int k3 = 0;
int k4 = 0;
int k5 = 0;
int k6 = 0;
int k7 = 0;

    uint8_t len = 0;
    uint8_t i;

/*
const int Pm1Pin = 2;
const int NPin = 4;
const int pPin = 8;
const int RPin = 9;
int Pm1State = 0;
int NState = 0;
int pState = 0;
int RState = 0;
*/

// Note: You must define a SoftwareSerial class object that the name must be mp3, 
//       but you can change the pin number according to the actual situation.
SoftwareSerial mp3(2, 3);

void Case()
{
    if(len > 0)
    {
        // Print reveiced data    
        // Serial.print("Received cmd: ");   
        // for(i = 0; i < len; i++) {
        //     Serial.print(recv_cmd[i]);
        //     Serial.print(" ");
        // }
        // Serial.println();
            
        switch (recv_cmd[0])
        {
            case 'P':
                if(recv_cmd[1] == 'm') 
                {
                    /** 
                      * Play music in "MP3" folder by index 
                      * example:
                      * "Pm1" -> ./MP3/0001.mp3
                    */
                    PlayMP3folder(recv_cmd[2] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[2]);
                    Serial.println(".mp3 in MP3 folder");
                } 
                else if(recv_cmd[1] == 'f')
                {
                    /** 
                      * Play specify folder and music
                      * example:
                      * "Pf11" -> ./01/001***.mp3
                    */
                    SpecifyfolderPlay(recv_cmd[2] - '0',recv_cmd[3] - '0');
                    Serial.print("Play ");
                    Serial.write(recv_cmd[3]);
                    Serial.print("xxx.mp3");
                    Serial.print(" in folder ");
                    Serial.write(recv_cmd[2]);
                    Serial.println();
                    
                } 
                else
                {
                    /** 
                      * Play music by default index
                      * example:
                      * "P1" -> ./***.mp3
                    */                
                    SpecifyMusicPlay(recv_cmd[1] - '0');
                    Serial.print("Play xxx.MP3 by index ");
                    Serial.write(recv_cmd[1]);
                    Serial.println();
                }            
                // Serial.println("Specify the music index to play");
                break;
            case 'p':
                PlayPause();            
                Serial.println("Pause the MP3 player");
                break;
            case 'R':            
                PlayResume();
                Serial.println("Resume the MP3 player");
                break;
            case 'N':            
                PlayNext();
                Serial.println("Play the next song");
                break;
            case 'L':
                PlayPrevious();
                Serial.println("Play the previous song");
                break;
            case 'l':
                PlayLoop();
                Serial.println("Play loop for all the songs");
                break;
            case 'I':
                IncreaseVolume();
                Serial.println("Increase volume");
                break;
            case 'D':
                DecreaseVolume();
                Serial.println("Decrease volume");
                break;
            default:
                break;
        }

        // clean data buffer
        for(i = 0; i < sizeof(recv_cmd); i++) {
            recv_cmd[i] = '\0';
        }
    }    
  }
  
void setup()
{
  pinMode(u1, INPUT);
  pinMode(u2, INPUT);
  pinMode(u3, INPUT);
  pinMode(b, INPUT);
  pinMode(l,OUTPUT);

  /*
  pinMode(Pm1Pin, INPUT);
  pinMode(NPin, INPUT);
  pinMode(pPin, INPUT);
  pinMode(RPin, INPUT);
  */
    mp3.begin(9600);
    Serial.begin(9600); 
    
    while(!Serial);

    Serial.println("Grove - Serial MP3 Demo");
    Serial.println(
        "Input command:\r\n\r\n"
        "P[ ] play music by default index\r\n"
        "Pm[ ] play music in MP3 folder by index\r\n"
        "Pf[ ][ ] play music by specify folder and index\r\n"        
        "p Pause\r\n"
        "R Resume\r\n"
        "N Next\r\n"
        "L Previous\r\n"
        "l Loop\r\n"
        "I Increase volume\r\n"
        "D Decrease volumern\r\n");
    
    delay(100);
    
    SelectPlayerDevice(0x02);       // Select SD card as the player device.
    SetVolume(0x0E);                // Set the volume, the range is 0x00 to 0x1E.
    analogWrite(l, 50);
    delay(5000);
}

void loop()
{
  u1S =  digitalRead(u1);
  u2S =  digitalRead(u2);
  u3S =  digitalRead(u3);
  bS =  digitalRead(b);
  

  
  /*
  Pm1State = digitalRead(Pm1Pin);
  NState = digitalRead(NPin);
  pState = digitalRead(pPin); 
  RState = digitalRead(RPin);     
  Serial.print(Pm1State);Serial.print('\t');Serial.print(NState);Serial.print('\t');Serial.print(pState);Serial.print('\t');Serial.print(RState);Serial.println('\t');
  */
  delay(1);
    len = 0;
    k = 0;

    Serial.print(u1S);Serial.print('\t');Serial.print(u2S);Serial.print('\t');Serial.print(u3S);Serial.print('\t');Serial.print(bS);Serial.println('\t');
  if(k==0){  
    if(bS==HIGH){
          while(k1==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '1';len++;Serial.println('1');
            k1++;
            }
          Case();
          analogWrite(l, 100);
/*          while(u1S||u2S||u3S){
            recv_cmd[0] = 'p';len++;Serial.print('p');
            recv_cmd[1] = '\0';
            recv_cmd[2] = '\0';
            Case();
            }*/
      }
      if(bS==LOW&& (u1S==HIGH||u2S==HIGH||u3S==HIGH)){
          while(k2==0){
            recv_cmd[0] = 'p';len++;Serial.print('p');
            recv_cmd[1] = '\0';
            recv_cmd[2] = '\0';
            k2++;
            }
          Case();
      }
  k++;
  }
  if(k==1){
      if(bS==LOW&& u1S==LOW&&u2S==HIGH&&u3S==LOW){
          while(k3==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '2';len++;Serial.println('2');
            k3++;
            }
          Case();
          analogWrite(l, 150);
          while(k3==1){
            delay(20000);//20sec
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '3';len++;Serial.println('3');
            k3++;
            }
          Case();
          while(k3==2){
            delay(3000);//3sec
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '1';len++;Serial.println('1');
            k3++;
            //analogWrite(l, 255);delay(500);analogWrite(l, 255);delay(500);
          }
          Case();
      }
      if(bS==LOW&& u1S==HIGH&&u2S==LOW&&u3S==HIGH){
        k++;
      }
  }
  if(k==2){
      if(bS==LOW&& u1S==HIGH&&u3S==HIGH){
          while(k4==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '4';len++;Serial.println('4');
            k4++;
          }
          Case();
          for (int j=0; j<600; j++){
            u2S =  digitalRead(u2);
            if(u2S==HIGH){k5++;}
            delay(100);
            Serial.print(j);Serial.print('\t');Serial.print(u2S);Serial.print('\t');Serial.println(k5);
          }
          while(k4==1){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '3';len++;Serial.println('3');
            k4++;
            }
            Case();
          delay(3000);//3sec
      }
      if(u1S!=HIGH&& u3S!=HIGH){
          while(k6==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '1';len++;Serial.println('1');
            k6++;
          }
          Case();
      }
      if(k4>1&& k5==0){
          while(k5==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '5';len++;Serial.println('5');
            k5++;
          }
          Case();
      }
      if(k5!=0){
          while(k7==0){
           recv_cmd[0] = '\0';
           recv_cmd[1] = '\0';
           recv_cmd[2] = '\0';        
           k7++;
          }
      }
      Case();
      k++;
  }
     /*
    if(NState==HIGH){
          recv_cmd[0] = 'N';len++;Serial.print('N');
          recv_cmd[1] = '\0';
          recv_cmd[2] = '\0';
      }
    if(pState==HIGH){
          recv_cmd[0] = 'p';len++;Serial.print('p');
          recv_cmd[1] = '\0';
          recv_cmd[2] = '\0';
      }
    if(RState==HIGH){
          recv_cmd[0] = 'R';len++;Serial.print('R');
          recv_cmd[1] = '\0';
          recv_cmd[2] = '\0';          
      }   */
/*
    if(Serial.available())
    {
        char chr = '\0';
        while(chr != '\n')  // Blockly read data from serial monitor
        {
            chr = Serial.read();
            // Serial.print(chr);
            recv_cmd[len++] = chr;        
        }
    }*/

    delay(100);
    
//    printReturnedData();
}
