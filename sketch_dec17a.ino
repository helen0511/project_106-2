//重要！！
//如果把 Arduino 的電給切掉，要記得也把 "熱敷墊" 的 "電" 給 "切掉"
// SetVolume 調聲音大小

#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>
// Note: You must define a SoftwareSerial class object that the name must be mp3, 
//       but you can change the pin number according to the actual situation.
SoftwareSerial mp3(2, 3);
SoftwareSerial BTSerial(10, 9); // 接收, 傳送

const int u1 = 11;    //紅
const int u2 = 12;    //黃
const int u3 = 13;    //紫
const int lc = 5;     //進度漸進亮燈
const int l = 4;      //平板撐正確燈
const int h1 = 6;      //接上繼電器板子
const int h2 = 7;      //接上繼電器板子
//目前使用 13, 12, 11, 10, 9, , 7, 6, 5, 4, 3, 2,  

int u1S = 0;          //左1 手
int u2S = 0;          //左2 中
int u3S = 0;          //左3 腳
int BT=0;             //藍芽立刻存值
long int number=0;    //藍芽立刻收值，一個一個疊加
long int numberok=0;  //藍芽儲存收值，直到下一個傳送近來才改變
char val;             // 儲存接收資料的變數
int j = 0;            //平板撐進度
int k = 0;            //表示流程進度走到哪一段
int k1 = 0;
int k2 = 0;
int k3 = 0;
int k4 = 0;           //平板撐進度：1.做完一分鐘 2.響完提示音
int k5 = 0;           //平板撐時，判斷中間感應次數

//音樂 0001.mp3   起床
//音樂 0002.mp3   靜坐
//音樂 0003.mp3   正確
//音樂 0004.mp3   錯誤
//音樂 0005.mp3   獎勵
//音樂 0006.mp3   平板

static uint8_t recv_cmd[8] = {};
       uint8_t len = 0;
       uint8_t i;
  
void setup() {
  pinMode(u1, INPUT);
  pinMode(u2, INPUT);
  pinMode(u3, INPUT);
  pinMode(lc,OUTPUT);
  pinMode(l,OUTPUT);
  pinMode(h1,OUTPUT);
  pinMode(h2,OUTPUT);

  Serial.begin(9600); 
  mp3.begin(9600);  
  BTSerial.begin(9600);
  BTSerial.println("BT is ready!");// 藍牙連線成功後，發佈「準備好了」訊息。看起來很多餘
  
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
  SetVolume(0x15);                // SetVolume(0x0E); Set the volume, the range is 0x00 to 0x1E.
  delay(1000);
}

void loop()
{
  u1S =  digitalRead(u1);
  u2S =  digitalRead(u2);
  u3S =  digitalRead(u3);

  delay(1);
  len = 0;

  Serial.print(k);Serial.print('\t');Serial.print(k1);Serial.print('\t');Serial.print(k2);Serial.print('\t');Serial.print(k3);Serial.print('\t');Serial.print(k4);Serial.print('\t');Serial.print(k5);
  Serial.print("--狀態--");
  Serial.print(u1S);Serial.print('\t');Serial.print(u2S);Serial.print('\t');Serial.print(u3S);
  Serial.println("--------------------感測--------------------");
  if(k==0){                                       //k 表示流程進度走到哪一段
    Serial.print(BTSerial.available());Serial.print('\t');Serial.print('\t');Serial.print(number);Serial.print('\t');Serial.print(numberok);
    Serial.println("--------------------藍芽--------------------");
    analogWrite(l, 0);
    digitalWrite(lc, 0);
    k=0; k1=0; k2=0; k3=0; k4=0; k5=0; j=0;
    if (BTSerial.available()){                    //偵測藍芽有沒有東西，有東西就收來做動作
      val = BTSerial.read();                      
      number=number*10+val%0x30;
      if (BTSerial.available()==1){               //藍芽有東西，收到最後剩一個才動
        BT=1;                                     //開關打開（應該也是可以不用開關啦，有點多餘）
        if (BT==1){                               //藍芽沒有東西不會動，開關開了才會動
          numberok=number*10;                     //原本收的東西，還沒有收最後一個，加上最後一個後轉存為 numberok
          number=0;                               //歸 0 收值暫存
          BT=0;                                   //關掉開關
        }
      }
    }                                             //---------------------------------------k=0-1 藍芽偵測
    if(numberok!=0){                              //numberok 有收到值才動作
      Serial.println(numberok);
      numberok=numberok/60;                     //方便測試加快速度 numberok/60 一分鐘變一秒
      if(numberok>=180000){                       //如果手機設的時間超過 3分00秒 = 180秒 = 180'000
        delay(numberok-180000);                   //我讓她先delay (設定時間-180'000)秒        
        digitalWrite(h1,HIGH);                     //開始發熱 
        digitalWrite(h2,HIGH);                     //開始發熱
        delay(180000);                            //發熱 3分00秒
      } else {
        digitalWrite(h1,HIGH);                     //開始發熱
        digitalWrite(h2,HIGH);                     //開始發熱
        delay(numberok);                          //delay 收值時間（會停下來，藍芽重複傳送的東西可能會被無視或延遲完才動，不知道會怎樣）
      }
      numberok=0;                                 //歸 0 收值
      val = '0';                                  //歸 0 收值
      if(k1==0){                                  //k1 很像按鈕開一下起床音樂，按一次而已。（Delay完一定會動）
        recv_cmd[0] = 'P';len++;Serial.print('P');
        recv_cmd[1] = 'm';len++;Serial.print('m');
        recv_cmd[2] = '1';len++;Serial.println('1'); //起床
        Case();                                     //開始撥放剛剛按的音樂
        k1++;                                       //不會一直按然後重複前面。
      }                                             //---------------------------------------k=0-1, k1+1 起床音樂
      analogWrite(lc, 30);                        //類比燈lc = 6
      k=1;                                        //跳出藍芽感測 k=1 不會再偵測藍芽
    }                                             //---------------------------------------k=0-2 收值delay 
  }
  // 我覺得直接這樣也可（少了暫停動作、少了明確偵測中間有感應旁邊沒感應）4/18
  if(k==1){                                             //跳靜坐，起床音樂還沒停
    u1S =  digitalRead(u1);  u2S =  digitalRead(u2);  u3S =  digitalRead(u3);
    if((u1S==HIGH || u2S==HIGH || u3S==HIGH) && k1!=0){ //踩瑜珈墊
       if(k2==0){                                       //k2 按停音樂，按一次而已。（播了起床音樂才會動）
         recv_cmd[0] = 'p';len++;Serial.print('p');
         recv_cmd[1] = '\0';
         recv_cmd[2] = '\0';
         Case();                                    //按停音樂
         delay(1000);      
         k2=1;
       }                                            //---------------------------------------k=1, k2=1 停止音樂
       if(k2==1){                                   //k3 按開靜坐音樂，按一次而已。（有靜坐姿勢才會動）
         recv_cmd[0] = 'P';len++;Serial.print('P');
         recv_cmd[1] = 'm';len++;Serial.print('m');
         recv_cmd[2] = '2';len++;Serial.println('2'); //靜坐
         Case();
         delay(20000);                                //20sec...延遲前面靜坐音樂
         k2=2;
       }                                             //--------------------------------------k=1, k2=2 靜坐音樂
       analogWrite(lc, 150);                         //類比燈l = A0
       digitalWrite(h1, LOW);                         //停止發熱
       digitalWrite(h2, LOW);                         //停止發熱
       if(k2==2){                                    //按開靜坐音樂後
         recv_cmd[0] = 'P';len++;Serial.print('P');
         recv_cmd[1] = 'm';len++;Serial.print('m');
         recv_cmd[2] = '3';len++;Serial.println('3'); //正確叮咚
         Case();
         delay(5000);//5sec
         k2=3;
         k=2;
       }                                             //---------------------------------------k=1, k2=3 正確提示       
    }                                                //---------------------------------------k=1 做完靜坐
  }
  //------------------------------------------------------------------------------------------------------------------------k=1
  if(k==2){                                             //換動作了嗎？
          analogWrite(lc, 200);                         //燈再亮
          u1S =  digitalRead(u1);  u2S =  digitalRead(u2);  u3S =  digitalRead(u3);
          if((u1S==HIGH || u2S==HIGH) && u3S==LOW){          //跑完靜坐音樂，還在靜坐
            if(k3==0){
              recv_cmd[0] = 'P';len++;Serial.print("-(0,1,0)-");Serial.print('P');
              recv_cmd[1] = 'm';len++;Serial.print('m');
              recv_cmd[2] = '1';len++;Serial.println('1'); //起床
              Case();                                     //開始撥放剛剛按的音樂
              delay(1000);//1sec
              k3=1;                                       //不會一直按然後重複前面。
            }
            analogWrite(lc, 0);delay(200);analogWrite(lc, 200);delay(200);//閃燈
          }                                               //--------------------------------------k=1-2.(X) 起床音樂
          if(u1S==HIGH && u2S == LOW && u3S==HIGH){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '3';len++;Serial.println('3');   //正確叮咚
            Case();
            delay(1000);//1sec
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '6';len++;Serial.println('6');
            k3=1;
            k=3;                                        //去下段
          }  
  }
  //------------------------------------------------------------------------------------------------------------------------k=2
  if(k==3){
    u1S =  digitalRead(u1);  u2S =  digitalRead(u2);  u3S =  digitalRead(u3);
    if(u1S==HIGH && u3S==HIGH){                       //平板動作，停掉音樂
      digitalWrite(l, HIGH);                          //姿勢正確
      digitalWrite(lc, HIGH);
      if(k4==0){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '6';len++;Serial.println('6');
            Case();
            delay(1000);//1sec
            k4=1;
      }                                             //---------------------------------------k=2-1, k4=1 撥音樂
      for (int j=0; j<100; j++){                       //慢慢偵測平板中間
        u1S =  digitalRead(u1);  u2S =  digitalRead(u2);  u3S =  digitalRead(u3);
        if(u2S == HIGH){
          recv_cmd[0] = 'P';len++;Serial.print('P');
          recv_cmd[1] = 'm';len++;Serial.print('m');
          recv_cmd[2] = '4';len++;Serial.println('4');//錯誤叭叭
          Case();
          delay(1000);
          digitalWrite(l, LOW);
          k5++;
        }
        delay(250);                                 // 250 = 25 秒
        Serial.print(u1S);Serial.print('\t');Serial.print(u2S);Serial.print('\t');Serial.print(u3S);Serial.print('\t');Serial.print(k5);Serial.print('\t');Serial.println(j);
      }                                             //---------------------------------------k=2-1 偵測中間
      k4=2;                                         //---------------------------------------k=2-1, k4=2 跳出平板
      if(k4==2){
            recv_cmd[0] = 'P';len++;Serial.print('P');
            recv_cmd[1] = 'm';len++;Serial.print('m');
            recv_cmd[2] = '3';len++;Serial.println('3');//正確叮咚
            Case();
            delay(2500);//2.5sec
            k4=3;
      }                                             //---------------------------------------k=2-1, k4=3
      if(k4==3 && k5==0){
        recv_cmd[0] = 'P';len++;Serial.print('P');
        recv_cmd[1] = 'm';len++;Serial.print('m');
        recv_cmd[2] = '5';len++;Serial.println('5');//獎勵
        Case();
        k=0;
      }                                             //---------------------------------------k=2-1, k=0, k4=3
      if(k4==3 && k5!=0){
        recv_cmd[0] = '\0';
        recv_cmd[1] = '\0';
        recv_cmd[2] = '\0'; 
        Case();
        k=0;
      }                                             //---------------------------------------k=2-1, k=0, k4=3
    }                                               //---------------------------------------k=2-1 做完平板支撐
    /*
    if(u1S==LOW && u3S==LOW){
      recv_cmd[0] = 'P';len++;Serial.print("-(0,X,0)-");Serial.print('P');
      recv_cmd[1] = 'm';len++;Serial.print('m');
      recv_cmd[2] = '1';len++;Serial.println('1');//起床
      Case();
    }                                               //---------------------------------------k=2-2 沒作平板支撐
    */
  }
  //------------------------------------------------------------------------------------------------------------------------k=3
    delay(100);
//    printReturnedData();
}

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
