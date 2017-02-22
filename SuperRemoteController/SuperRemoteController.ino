#include <Wire.h>
#include <avr/pgmspace.h>

#define ir_out 13
//#define led  9
#define SendButton 5
#define UpButton 4
#define DownButton 3
#define Ramen 6
#define intsize 2
#define Speaker 8

int SelNum = 0;
int count = 0;
int last = 0;
unsigned long us = micros();

// テレビON
PROGMEM const unsigned int data1[] = {39,46,36,128,39,46,36,128,39,46,37,128,39,46,36,128,39,47,36,128,39,46,37,127,39,127,40,47,36,128,38,47,36,128,39,127,40,127,39,127,36,48,39,48,35,44,39,128,38,47,36,128,39,46,37,47,36,128,39,46,37,46,37,46,37,46,37,127,39,127,39,46,37,127,38,48,35,48,35,46,37,128,39,47,35,48,35,48,35,129,38,48,35,129,38,129,38,7060,332,170,38,47,36,128,35,51,36,130,36,47,36,130,37,47,36,130,36,47,36,130,36,47,36,130,33,133,34,50,33,134,32,50,33,134,33,134,32,134,33,134,32,51,32,51,32,51,26,139,27,55,28,135,36,50,30,50,33,127,46,41,40,46,40,36,47,43,40,120,46,120,39,48,35,125,48,38,46,40,43,36,46,120,47,37,46,40,42,41,36,131,36,47,35,128,39,128,39
};

//Dボタン
PROGMEM const unsigned int data2[] = {36,51,32,131,35,51,32,131,35,51,33,131,35,51,32,131,35,51,34,130,35,50,34,130,36,132,35,50,33,131,35,51,33,131,35,132,35,132,35,132,35,51,32,51,32,51,32,131,35,50,32,132,35,51,33,50,32,134,32,51,32,50,33,50,32,50,33,134,32,134,33,134,33,134,36,47,36,130,36,47,36,47,37,130,36,47,36,46,37,46,36,130,36,47,36,47,36,7054,332,172,36,47,36,130,36,47,36,130,36,47,36,130,37,46,36,130,37,47,36,130,37,46,36,130,37,130,36,47,36,130,37,47,36,130,36,130,36,130,36,130,37,46,36,47,37,46,36,130,36,47,33,134,33,50,33,51,32,134,33,50,32,50,32,50,33,50,33,134,33,132,35,132,35,131,35,51,33,130,36,50,33,50,32,131,36,51,33,50,32,51,32,132,35,50,33,51,32,
};

//藤堂邸エアコン
PROGMEM const unsigned int data3[] = {48,124,40,131,40,45,40,46,40,45,40,131,40,45,40,45,40,131,40,131,40,45,40,131,47,38,40,46,47,124,40,131,40,45,40,131,40,131,48,38,40,45,39,131,40,46,47,37,40,131,40,45,40,45,40,45,40,45,40,46,47,38,47,38,40,46,40,45,40,45,40,46,40,45,40,45,41,44,40,45,40,45,40,45,40,45,40,45,40,46,39,46,39,46,40,45,40,45,40,45,40,46,40,131,40,131,48,37,40,45,40,45,40,45,40,131,40,45,48,123,40,46,39,46,48,37,40,46,47,38,48,122,40,131,40,46,47,123,48,123,40,46,47,38,48,123,40,45,40,45,40,131,48,38,47,38,40,131,48,38,48,37,40,45,48,38,48,37,40,45,48,37,41,45,40,45,47,38,48,38,40,45,40,45,40,46,39,46,39,46,39,46,40,45,40,45,40,46,40,45,40,46,47,38,39,45,40,46,40,46,39,45,41,44,48,38,40,45,40,45,40,46,40,45,40,46,39,45,40,45,40,45,48,38,40,45,40,45,40,45,40,45,40,46,40,46,40,45,40,45,47,38,40,45,39,46,40,45,40,45,40,46,39,45,40,46,39,45,40,45,40,46,40,45,40,132,39,131,40,46,40,131,40,132,40,46,39,131,40,1333,341,174,40,131,40,132,40,46,39,46,40,45,40,131,40,46,40,45,40,131,40,131,40,45,40,131,40,46,39,45,40,130,40,132,40,46,38,132,40,131,40,46,40,45,40,131,40,45,40,45,40,131,40,46,38,46,40,45,40,45,40,45,40,46,40,45,40,45,40,45,40,45,40,45,40,45,40,45,40,45,40,46,39,45,40,45,40,46,40,45,40,46,39,46,39,46,40,45,40,46,40,45,40,45,40,131,40,132,39,46,40,46,40,45,40,44,40,131,40,45,40,132,39,46,39,46,39,45,40,46,40,45,40,131,40,131,40,45,40,131,40,131,41,44,40,46,39,131,40,45,40,47,38,132,38,46,40,45,40,131,40,45,40,45,40,45,40,45,40,45,40,45,40,45,40,46,40,44,40,45,40,45,40,45,40,45,40,45,40,46,39,46,40,45,40,45,39,46,40,45,40,46,40,45,40,45,40,46,40,45,40,45,40,45,41,44,40,45,40,45,40,45,40,45,40,45,40,46,40,46,39,45,40,46,40,45,40,45,40,45,40,45,40,46,40,45,40,45,40,45,40,46,40,46,40,45,40,45,40,46,39,46,40,45,40,45,40,47,39,45,40,45,40,45,40,46,40,131,39,132,39,45,40,132,40,131,40,45,40,131,40,
};

const unsigned int *data[] {
  data1,
  data2,
  data3,
};

unsigned int buff[1000];

int datanum = 2;

//それぞれのdataのsizeofを格納 dataの番号にインデックスが対応
int sizes[] ={394,394,1162};

char *dataname[] = {"TV_ONOFF", "D_button", "TODO"};

 
// セットアップ
void setup() {
  //size測定部
//  Serial.begin(115200);
//  Serial.print(data[0][0]);
//  Serial.print(sizeof(data3));
//  Serial.print(sizeof(data3[0]));
  
  //ディスプレイ初期化
  pinMode(A2, OUTPUT);
  digitalWrite(A2,HIGH);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  delay(500); //待機時間(必須)
  lcd_init();
  lcd_setCursor(0,0);
  lcd_printStr(dataname[SelNum]);
  delay(1000);
  
  //その他セットアップ
  pinMode(ir_out, OUTPUT);
  //pinMode(led, OUTPUT);
  pinMode(SendButton, INPUT_PULLUP);
  pinMode(UpButton, INPUT_PULLUP);
  pinMode(DownButton, INPUT_PULLUP);
  pinMode(Ramen, INPUT_PULLUP);
  pinMode(Speaker, OUTPUT);
}
 
// dataからリモコン信号を送信
void sendSignal(int sn) {
  int dataSize = sizes[sn] / intsize;
  for (int cnt = 0; cnt < dataSize; cnt++) {
    unsigned long len = pgm_read_word_near(data[sn] + cnt)*10;  // dataは10us単位でON/OFF時間を記録している
    unsigned long us = micros();
    do {
      digitalWrite(ir_out, 1 - (cnt&1)); // cntが偶数なら赤外線ON、奇数ならOFFのまま
      delayMicroseconds(8);  // キャリア周波数38kHzでON/OFFするよう時間調整
      digitalWrite(ir_out, 0);
      delayMicroseconds(7);
    } while (long(us + len - micros()) > 0); // 送信時間に達するまでループ
  }
}
 
void loop() {
  
  //digitalWrite(led, HIGH);
  int pushing = Pushing();
  //送信ボタン
  if(digitalRead(pushing) == LOW && pushing != -1) {
    if(count <= 50) count++;
    if(count == 50) {
      switch(pushing) {
        case SendButton:
          sendSignal(SelNum);
          break;
        case DownButton:
          if(SelNum == datanum) SelNum = 0;
          else SelNum++;
          break;
        case UpButton:
          if(SelNum == 0) SelNum = datanum;
          else SelNum--;
          break;
        case Ramen:
          RamenTimer();
          break;
      }
    }
    lcd_clear();
    lcd_setCursor(0,0);
    lcd_printStr(dataname[SelNum]);
  } else {
    count = 0;
  }

}

int Pushing() {
  if(digitalRead(SendButton) == LOW) return SendButton;
  if(digitalRead(UpButton) == LOW) return UpButton;
  if(digitalRead(DownButton) == LOW) return DownButton;
  if(digitalRead(Ramen) == LOW) return Ramen;

  return -1;
}

void RamenTimer() {
  lcd_clear();
  lcd_setCursor(0,0);
  lcd_printStr("Wait!");
  char str[4];
  boolean doing = true;
  unsigned long Now = millis();
  while(doing) {
    lcd_setCursor(5,1);
    unsigned long timer = 180 - (millis() - Now) / 1000;
    sprintf(str, "%3d", timer);
    lcd_printStr(str);
    if(timer <= 0) {
      doing = false;
      alert();
    }
    delay(1000);
  }
}
