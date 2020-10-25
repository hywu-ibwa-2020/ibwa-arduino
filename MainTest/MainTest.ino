#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
#include <string.h>
#include <Servo.h>

#define LED_PIN 6
#define LED_COUNT 30

Servo sv1_up, sv2_up;
Servo sv1_down, sv2_down;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);
SoftwareSerial DFPlayerSerial(10, 11); // DFPlayer RX, TX
// 서보모터 각도값 변수
int is_checked = 180;    // 버튼 클릭시 서보모터 10도 작동
int is_not_checked = 0; // 버튼 클릭 안했을 떄 서보모터 0도 작동

// 서보모터 delay 시간 값
int wait = 500;
//int sensorValue = 0;

// custom function 선언
void colorWipe(uint32_t c, int colorWait);
void colorByMood(char color);

int r, g, b;
// 1. AI 맞춤 추천 / 분위기별 맞춤 추천 조명 기능
// 2. 사용자가 지정한 색상으로 조명 변경

void setup()
{
  Serial.begin(9600);
//  Serial1.begin(9600);  // 블루투스 모듈 통신
    
  sv1_up.attach(22);    // 서보모터 핀넘버
  sv1_down.attach(23);
  sv2_up.attach(24);
  sv2_down.attach(25);

  sv1_up.write(0);        // 서보모터 각도 초기화
  sv1_down.write(0);
  sv2_up.write(0);
  sv2_down.write(0);
  
  strip.begin(); //네오픽셀을 초기화하기 위해 모든LED를 off시킨다
  strip.show(); 
  strip.setBrightness(50);
  
  DFPlayerSerial.begin(9600);        // 소프트웨어 시리얼 통신 시작
  mp3_set_serial(DFPlayerSerial);    // DFPlayer 모듈에 소프트웨어 시리얼 설정
  delay(1);                           // 볼륨이 설정될 동안 1ms 대기
  mp3_set_volume (20);                // 볼륨 설정 (0~30)
}
 
void loop()
{ 
  String input;
  if(Serial.available()) {
    Serial.println("--------------------------------------------");
    input = Serial.readStringUntil('\n');
    Serial.println("입력된 input: " + input);
    String data = input.substring(0, 1);
    Serial.println("정제된 데이터: " + data);
    Serial.println("--------------------------------------------");
    if (data == "m") {                             // 분위기별 맞춤 추천 기능
      Serial.println("<< 분위기별 맞춤 추천 기능 >>");
      recByMood(input);
    } else if (data == "c") {                       // 모터 제어
      Serial.println("<< 모터 제어 기능 >>");
      operateMotorsCds(input);
    } else {
      Serial.println("<< 사용자 지정 조명 색 변경 기능 >>");
      printCustomColor(input);                      // 사용자 색상 변경 "255,255,255" 형태로 전달됨
    }
  }
}
// 분위기별 맞춤 추천 기능 제어하는 함수
void recByMood(String input) {
  if (input == "mpause") {
    Serial.println("음악을 잠시 멈춥니다.");
    mp3_pause();
  } else if (input == "mstart") {
    Serial.println("음악을 다시 재생합니다.");
    mp3_play();
  } else if (input == "mstop") {
    Serial.println("맞춤 추천 기능을 종료합니다.");
    Serial.println("음악을 멈춥니다.");
    mp3_stop();
    Serial.println("조명을 끕니다.");
    colorWipe(strip.Color(0, 0, 0), 50);
  } else {
    Serial.println("input: " + input);
    Serial.println("조명을 켭니다.");
    colorByMood(input);
    Serial.println("음악을 재생합니다.");
    playMusicByMood(input);
  }
}
// 커튼 모터 제어하는 함수
void operateMotorsCds(String mainBtn){
  // 커일업
  if (mainBtn == "c1up"){
    Serial.println("1번 커튼을 올립니다.");
    sv1_up.write(is_checked);
    delay(wait);
    sv1_up.write(is_not_checked);
  } else if (mainBtn == "c2up"){   // 커이업
    Serial.println("2번 커튼을 올립니다.");
    sv2_up.write(is_checked);
    delay(wait);
    sv2_up.write(is_not_checked);      
  } else if (mainBtn == "c1down"){   // 커일단
    Serial.println("1번 커튼을 내립니다.");
    sv1_down.write(is_checked);
    delay(wait);
    sv1_down.write(is_not_checked);      
  } else if (mainBtn == "c2down"){   // 커이단
    Serial.println("2번 커튼을 내립니다.");
    sv2_down.write(is_checked);
    delay(wait);
    sv2_down.write(is_not_checked);      
  } else if (mainBtn == "c12up"){   // 커일이업
    Serial.println("1, 2번 커튼을 모두 올립니다.");
    sv1_up.write(is_checked);
    sv2_up.write(is_checked);
    delay(wait);
    sv1_up.write(is_not_checked);
    sv2_up.write(is_not_checked);      
  } else if (mainBtn == "c12down"){   // 커일이단
    Serial.println("1, 2번 커튼을 모두 내립니다.");
    sv1_down.write(is_checked);
    sv2_down.write(is_checked);
    delay(wait);
    sv1_down.write(is_not_checked);
    sv2_down.write(is_not_checked);      
  }  
}
// 입력받은 rgb 값으로 네오픽셀 색상 변경하는 함수
void printCustomColor(String RGBValue) {
  Serial.println(RGBValue);
  String n = ""+'\n';
  if (RGBValue != n) {
    r = splitValue(RGBValue, 1);
    Serial.println(r);
    g = splitValue(RGBValue, 2);
    Serial.println(g);
    b = splitValue(RGBValue, 3);
    Serial.println(b);
    colorWipe(strip.Color(g, r, b), 50);
  }
}
// String을 int로 변경하는 함수
int stringToInt(String str) {
  char exampleChar[1];
  str.toCharArray(exampleChar, 3);
  int intStr = atoi(exampleChar);
  return intStr;
}
// 입력받은 값에서 r 값 추출하는 함수
int splitValue(String str, int num) {
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
  
  String str1 = str.substring(0, first); // 첫 번째 토큰 (0, 3)
  String str2 = str.substring(first+1, second); // 두 번째 토큰 (4, 7)
  String str3 = str.substring(second+1,length); // 세 번째 토큰(8, 10) 

  char char1[1];
  
  if (num == 1) {
    str1.toCharArray(char1, 4);
    int r = atoi(char1);
    Serial.println("r값을 추출합니다." + r);
    return r;
  } else if (num == 2) {
    str1.toCharArray(char1, 4);
    int g = atoi(char1);
    Serial.println("g값을 추출합니다." + g);
    return g;
  } else {
    str1.toCharArray(char1, 4);
    int b = atoi(char1);
    Serial.println("b값을 추출합니다." + b);
    return b;
  }
}
// 지정한 컬러로 픽셀 하나씩 출력해주는 함수
void colorWipe(uint32_t c, int colorWait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(colorWait);
  }
}
// 분위기별 맞춤 노래
void playMusicByMood(String input) {
  if (input == "m01") {
    Serial.println("첫번째 노래를 재생합니다.");
    mp3_play(21);
  } else if (input == "m02") {
    Serial.println("두번째 노래를 재생합니다.");
    mp3_play(22);
  } else if (input == "m03") {
    Serial.println("세번째 노래를 재생합니다.");
    mp3_play(23);
  } else if (input == "m04") { 
    Serial.println("네번째 노래를 재생합니다.");
    mp3_play(24);
  } else if (input == "m05") { 
    Serial.println("다섯번째 노래를 재생합니다.");
    mp3_play(25);
  } else if (input == "m06") { 
    Serial.println("여섯번째 노래를 재생합니다.");
    mp3_play(26);
  } else {
    Serial.println("잘못된 접근입니다.");
  }
}

// 분위기별 맞춤 조명
void colorByMood(String input) {
    if (input == "m01") {
      Serial.println("m01: 보라색 조명을 켭니다.");
      colorWipe(strip.Color(104, 123, 238), 50);  // 보라
    } else if (input == "m02") {
      Serial.println("m02: 노랑색 조명을 켭니다.");
      colorWipe(strip.Color(230, 255, 65), 50);  // 노랑
    } else if (input == "m03") {
      Serial.println("m03: 빨강색 조명을 켭니다.");
      colorWipe(strip.Color(0, 255, 0), 50);  // 빨강
    } else if (input == "m04") { 
      Serial.println("m04: 분홍색 조명을 켭니다.");
      colorWipe(strip.Color(158, 255, 155), 50);  // 분홍
    } else if (input == "m05") {
      Serial.println("m05: 파랑색 조명을 켭니다.");
      colorWipe(strip.Color(100, 0, 205), 50);  // 파랑
    } else if (input == "m06") { 
      Serial.println("m06: 초록색 조명을 켭니다.");
      colorWipe(strip.Color(100, 0, 0), 50);  // 초록
    } else {
      Serial.println("잘못된 접근입니다.");
    }
}
