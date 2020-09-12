#include <SoftwareSerial.h>
#include <DFMiniMp3.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "DS3231.h" // RTC 모듈을 사용하기 위한 라이브러리
#include <Servo.h>
#include <TM1637Display.h>

#define CLK 2 // TM1637 클럭 연결 포트(D22)
#define DIO 9 // TM1637 데이터 연결 포트(D9)
#define PIN 5 // 네오픽셀 연결핀 번호 디지털
#define NUMPIXELS 30 // 네오픽셀 갯수
#define BRIGHTNESS 50 // 네오픽셀 밝기 설정 (0~255)
RTClib RTC; // RTC 모듈 객체 생성
Time t;

int delayval = 20; // 일정시간(20mS)동안 지연
unsigned long previousMillis;
bool showdot = false;
int Moon = 27;
int Sun = 26;
int blueTx=10;
int blueRx=11;
int UpPushCounter = 0;
int DownPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

// 네오픽셀 라이브러리를 설정할 때 신호를 보내는데 사용할 픽셀수와 핀수를 알려줍니다.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800);
SoftwareSerial BTSerial(blueTx, blueRx); // 시리얼 통신을 위한 객체선언
TM1637Display display(CLK, DIO);
Servo servo1, servo2, servo3;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //시리얼 모니터
  BTSerial.begin(9600); //블루투스 시리얼 개방
  Wire.begin();
  servo1.attach(7);
  servo2.attach(6);
  servo3.attach(4);
  servo1.write(0); //서보 초기각도 0도 설정
  pinMode(Moon, OUTPUT);  // LED 설정
  pinMode(Sun, OUTPUT);
  pixels.begin(); // 네오픽셀 라이브러리 초기화
  strip.setBrightness(BRIGHTNESS);
//  mp3_set_serial(Serial); // DFPlayer 시리얼 세팅
//  delay(1); // 볼륨값 적용을 위한 delay
//  mp3_set_volume(30); // 볼륨조절 값 0~30
}

void loop() {
  // put your main code here, to run repeatedly:
  timer();

//  BTS();

  day_night();
  cds_motor();

  curtain1_ctrl();
  neo_pixel();
}

// 시계
void timer(){
  delay(1000);

  // RTC로 부터 읽은 시간
  DateTime now = RTC.now();

  // 시리얼 모니터에 날짜, 시간을 표시
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.print(" since midnight 1/1/1970 = ");
  Serial.print(now.unixtime());
  Serial.print("s = ");
  Serial.print(now.unixtime() / 86400L);
  Serial.println("d");

  // TM1637 FND에 시간 표시
  display.write(now.hour() / 10, 0);
  display.write(now.hour(), 1);
  display.write(now.minute() / 10, 2);
  display.write(now.minute() % 10, 3);

}

// Moonlight LED 제어(시간대)
void day_night(){     
  t = rtc.getTime(); //RTC에 저장된 시간을 들고옴

  // am6:00가 되면 해 led 켜기(시간 임시 설정)
  if(t.hour == 6 && t.min == 0 && t.sec == 0){
    SlightOn();
    MlightOff();
}
  if(t.hour == 19 && t.min == 0 && t.sec == 0){ // pm7:00가 되면 달 led 켜기(시간 임시 설정)
    SlightOff();
    MlightOn();
  }
}


void MlightOn() {
  digitalWrite(Moon, HIGH);
}

void MlightOff() {
  digitalWrite(Moon, LOW);  
}

void SlightOn() {
  digitalWrite(Sun, HIGH);
}

void SlightOff() {
  digitalWrite(Sun, LOW);  
}


// 조도센서 + 서보모터 제어
void cds_motor(){     
  int sensor = analogRead(A0);          // 조도센서의 데이터 받아오기
  if (sensor > 1000){                   // 주위가 어둡다면 블라인드 올리기   
    servo1.write(45);                   // 서보모터 각도 45도로 하여 버튼 눌렀다 떼기
    delay(250);
    servo1.write(0);
  }
  
    while(sensor >=1000 && sensor <= 1023){    // 조도센서의 빛 밝기에 변함이 없을 때
      delay(100); 
      sensor = analogRead(A0);                   // 조도센서의 값 재입력
    }
    
  }else {                                      // 주위가 밝다면  
    servo3.write(45);                          // 서보모터 각도 45도로 하여 버튼 눌렀다 떼기
    delay(250);
    servo3.write(0);
  } 
  
    while(sensor >= 0 && sensor < 1000){       // 조도센서의 빛 밝기에 변함이 없을 때     
      delay(100);
      sensor = analogRead(A0);                   // 조도센서의 값 재입력
   }
  }
}


// 블라인드1 제어
void curtain1_ctrl(){         // ??? 다시 생각해보기
  if(BTSerial.available()){
    buttonState = BTSerial.read();
    if(buttonState != lastButtonState){
      if(buttonState == '0'){
        UpPushCounter++;
        Serial.println("up");
        Serial.print("number of up button pushes: ");
        Serial.println(UpPushCounter);  
      } else if (buttonState == '1'){
          DownPushCounter++;
          Serial.println("down");  
          Serial.print("number of down button pushes: ");
          Serial.println(DownPushCounter);
      }
      delay(50);
  }
  lastButtonState = buttonState;

  if(UpPushCounter % 2 == 0) {
    servo1.write(45);
    delay(250);
    servo1.write(0);
  } else {
    servo2.write(45);
    delay(250);
    servo2.write(0);
  }

  if(DownPushCounter % 2 == 0) {
    servo3.write(45);
    delay(250);
    servo3.write(0);  
  } else {
    servo2.write(45);
    delay(250);
    servo2.write(0);
  }
}


// 네오픽셀
byte r = 0; g = 0; b = 0; w = 0;  //RGBW 변수 선언
void neo_pixel(){
  if(BTSerial.available()>2){     //수신받는 블루투스 테이터가 2개 초과일때  
    r = BTSerial.read();          //첫번째 데이터를 r에 저장    
    g = BTSerial.read();          //두번째 데이터를 g에 저장   
    b = BTSerial.read();          //세번째 데이터를 b에 저장 
    w = BTSerial.read();          //네번째 데이터를 w에 저장  
    BTSerial.flush();             //통신내용 초기화(블루투스통신)
    
    for(int i=NUMPIXELS;i>=0;i--){
          pixels.setPixelColor(i, pixels.Color(0,0,0,0)); //지정된 색깔 없음
          pixels.show(); //업데이트 된 픽셀 색상을 하드웨어로 보냄
          delay(delayval); //일정 시간(20mS) 동안 지연됨
        }
        delay(delayval);
    for(int i=0;i<NUMPIXELS;i++){
          pixels.setPixelColor(i, pixels.Color(r,g,b,w)); //지정된 색깔
          pixels.show(); 
          delay(delayval); 
        }
  }
}
