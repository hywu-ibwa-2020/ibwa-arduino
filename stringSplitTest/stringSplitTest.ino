# include <stdio.h>
# include <string.h>

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
  // 사용자가 지정한 색상 값을 가져와서 저장
//  String str = "255,0,0";
//  int first = str.indexOf(",");// 첫 번째 콤마 위치
//  int second = str.indexOf(",",first+1); // 두 번째 콤마 위치
// 
//  Serial.println(first);
//  Serial.println(second);

  char text[] = "255 0 0";
  char arr[3];
  char *split = strtok(text, " ");

  int i, j;
  while (split != NULL and i < 4) {
    Serial.println(split);
    arr[i] = split;
    split = strtok(NULL, " ");
    i++;
  }  
  Serial.println(arr[1]);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
