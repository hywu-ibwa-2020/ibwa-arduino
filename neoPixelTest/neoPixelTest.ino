#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <string.h>

#define LED_PIN 6
#define LED_COUNT 30

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
SoftwareSerial DFPlayerSerial(10, 11); // DFPlayer RX, TX

// custom function 선언
void colorWipe(uint32_t c, int wait);
void colorByMood(char color);

int r, g, b;
// 1. AI 맞춤 추천 / 분위기별 맞춤 추천 조명 기능
// 2. 사용자가 지정한 색상으로 조명 변경

void setup()
{
  strip.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip.show(); 
  strip.setBrightness(50);

  Serial.begin(9600);
  // 사용자가 지정한 색상 값을 가져와서 저장
  String str = "255,0,0";
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
 
  Serial.println(first);
  Serial.println(second);
  
//  Serial.begin(9600);               // 블루투스 모듈 통신
  DFPlayerSerial.begin(9600);        // 소프트웨어 시리얼 통신 시작
  mp3_set_serial(DFPlayerSerial);    // DFPlayer 모듈에 소프트웨어 시리얼 설정
  delay(1);                           // 볼륨이 설정될 동안 1ms 대기
  mp3_set_volume (20);                // 볼륨 설정 (0~30)
  Serial.println("Enter the key!");       // 키 입력하기
  Serial.println("1 = 1st song");         // 1를 누른다면 첫 번째 노래 재생
  Serial.println("2 = 2nd song");         // 2를 누른다면 두 번째 노래 재생
  Serial.println("3 = 3th song");         // 3를 누른다면 세 번째 노래 재생
  Serial.println("4 = 4th song");         // 4를 누른다면 네 번째 노래 재생
  Serial.println("5 = Stop");             // 5를 누른다면 노래 중단
  Serial.println("6 = Continue\n");       // 6을 누른다면 이어서 재생Serial.begin (9600);
}
 
void loop()
{
//  colorByMood(2);

   if (Serial.available()) {
     char data;
     data = Serial.read();
     if (data > 0) {
     Serial.println(data);
     colorByMood(data);
     playMusicByMood(data);
     if (data == '5') {
      mp3_pause(); 
//      colorWipe(strip.Color(0, 0, 0), 0);
     } else if (data == '6') {
      mp3_play();
//      colorWipe(strip.Color(255, 255, 255), 50);
     }
   }
   }
   r = 231;
   g = 255;
   b = 7;
   colorWipe(strip.Color(g, r, b), 50);
   
//   colorWipe(strip.Color(255,   0,   0), 0); // Green
//   delay(5000);
//   colorWipe(strip.Color(0, 255, 0), 50); // Red
//   colorWipe(strip.Color(  0,   0, 255), 50); // Blue
}

// 지정한 컬러로 픽셀 하나씩 출력해주는 함수
void colorWipe(uint32_t c, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
} 
// 분위기별 맞춤 노래
void playMusicByMood(char input) {
  switch(input) {
    case '1' : 
      Serial.print("1st song\n");  // '1st song'를 시리얼 모니터에 출력
      mp3_play(1); // 첫 번째 노래 재생
      break;
    case '2' : 
      Serial.print("2nd song\n");  // '2nd song'를 시리얼 모니터에 출력
      mp3_play(2); // 두 번째 노래 재생
      break;
    case '3' : 
      Serial.print("3rd song\n");  // '1st song'를 시리얼 모니터에 출력
      mp3_play(3); // 세 번째 노래 재생
      break;
    case '4' : 
      Serial.print("4th song\n");  // '2nd song'를 시리얼 모니터에 출력
      mp3_play(4); // 네 번째 노래 재생
      break;
  }
}

// 분위기별 맞춤 조명
void colorByMood(char color) {
    switch (color)
    {
    case '1':
      colorWipe(strip.Color(255, 0, 0), 50);  // 초록
      break;

    case '2':
      colorWipe(strip.Color(0, 255, 0), 50);  // 빨강
      break;

    case '3':
      colorWipe(strip.Color(0, 0, 255), 50);  // 파랑
      break;
      
    case '4':
      colorWipe(strip.Color(0, 255, 255), 50);  // 보라
      break;
    
//    default:
//      colorWipe(strip.Color(50, 50, 50), 0);
//      break;
    }
}
