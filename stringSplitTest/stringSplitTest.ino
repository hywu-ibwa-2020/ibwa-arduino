# include <stdio.h>
# include <string.h>

void setup() {
  Serial.begin(9600);
  String str = "255,255,255";
  int first = str.indexOf(",");// 첫 번째 콤마 위치
  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
 
//  Serial.println(first);
//  Serial.println(second);  

  String str1 = str.substring(0, first); // 첫 번째 토큰 (0, 3)
  String str2 = str.substring(first+1, second); // 두 번째 토큰 (4, 7)
  String str3 = str.substring(second+1,length); // 세 번째 토큰(8, 10)
 
//  Serial.println(str1);
//  Serial.println(str2);
//  Serial.println(str3);

  char char1[1];
  str1.toCharArray(char1, 4);
  int intStr1 = atoi(char1);
  Serial.println(intStr1);

  char char2[1];
  str2.toCharArray(char2, 4);
  int intStr2 = atoi(char2);
  Serial.println(intStr2);

  char char3[1];
  str3.toCharArray(char3, 4);
  int intStr3 = atoi(char3);
  Serial.println(intStr3);
}

void loop() {
  // put your main code here, to run repeatedly:

}
