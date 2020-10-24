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
  Serial1.begin(9600);               // 블루투스 모듈 통신
  DFPlayerSerial.begin(9600);        // 소프트웨어 시리얼 통신 시작
  mp3_set_serial(DFPlayerSerial);    // DFPlayer 모듈에 소프트웨어 시리얼 설정
  delay(1);                           // 볼륨이 설정될 동안 1ms 대기
  mp3_set_volume (20);                // 볼륨 설정 (0~30)
//  Serial.println("Enter the key!");       // 키 입력하기
//  Serial.println("1 = 1st song");         // 1를 누른다면 첫 번째 노래 재생
//  Serial.println("2 = 2nd song");         // 2를 누른다면 두 번째 노래 재생
//  Serial.println("3 = 3th song");         // 3를 누른다면 세 번째 노래 재생
//  Serial.println("4 = 4th song");         // 4를 누른다면 네 번째 노래 재생
//  Serial.println("5 = Stop");             // 5를 누른다면 노래 중단
//  Serial.println("6 = Continue\n");       // 6을 누른다면 이어서 재생Serial.begin (9600);
}
 
void loop()
{
//   if (Serial1.available()) {
//     char data;
//     data = Serial1.read();
//     if (data > 0) {
//     Serial1.println(data);
//     colorByMood(data);
////     playMusicByMood(data);
//     if (data == '5') {
////      mp3_pause(); 
////      colorWipe(strip.Color(0, 0, 0), 0);
//     } else if (data == '6') {
////      mp3_play();
////      colorWipe(strip.Color(255, 255, 255), 50);
//     }
//   }
//   }
//  printCustomColor("255,0,0");
//  colorByMood('1');
//  colorWipe(strip.Color(255, 0, 0), 50);
  
  String input;
  if(Serial1.available()) {
    input = Serial1.readStringUntil('\n'+'\n');
    Serial.println(input);
    printCustomColor(input);
    
    if (input.substring(0, 2) == "m") {
      colorByMood(input); 
    }
  }
}
void printCustomColor(String RGBValue) {
  String n = ""+'\n';
  if (RGBValue != n) {
    r = splitRValue(RGBValue);
    Serial.println(r);
    g = splitGValue(RGBValue);
    Serial.println(g);
    b = splitBValue(RGBValue);
    Serial.println(b);

    colorWipe(strip.Color(g, r, b), 50);
  }
}
int stringToInt(String str) {
  char exampleChar[1];
  str.toCharArray(exampleChar, 3);
  int intStr = atoi(exampleChar);
  return intStr;
}
int splitRValue(String str) {
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
  
  String str1 = str.substring(0, first); // 첫 번째 토큰 (0, 3)

  char char1[1];
  str1.toCharArray(char1, 4);
  int r = atoi(char1);
  
  return r;
}
int splitGValue(String str) {
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
  
  String str1 = str.substring(first+1, second); // 두 번째 토큰 (4, 7)
  
  char char1[1];
  str1.toCharArray(char1, 4);
  int g = atoi(char1);

  return g;
}
int splitBValue(String str) {
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
  
  String str1 = str.substring(second+1,length); // 세 번째 토큰(8, 10)  

  char char1[1];
  str1.toCharArray(char1, 4);
  int b = atoi(char1);

  return b;
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
      Serial.print("3rd song\n");  // '3rd song'를 시리얼 모니터에 출력
      mp3_play(3); // 세 번째 노래 재생
      break;
    case '4' : 
      Serial.print("4th song\n");  // '4th song'를 시리얼 모니터에 출력
      mp3_play(4); // 네 번째 노래 재생
      break;
  }
}

// 분위기별 맞춤 조명
void colorByMood(String color) {
    if (color == "m01") 
      colorWipe(strip.Color(255, 0, 0), 50);  // 초록
    else if (color == "m02") 
      colorWipe(strip.Color(0, 0, 255), 50);  // 빨강
    else if (color == "m03") 
      colorWipe(strip.Color(0, 255, 0), 50);  // 파랑
    else if (color == "m04")   
      colorWipe(strip.Color(0, 255, 255), 50);  // 보라
    
}
