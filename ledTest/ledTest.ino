int sunny = 22;
int cloudy = 23;
int snowy = 24;
int rainy = 25;
int lightning = 26;
int sunset = 27;
int sunrise = 28;

void setup() {
  pinMode(sunny, OUTPUT);
  pinMode(cloudy, OUTPUT);
  pinMode(snowy, OUTPUT);
  pinMode(rainy, OUTPUT);
  pinMode(lightning, OUTPUT);
  pinMode(night, OUTPUT);
}

void loop() {
  String input;
  if(Serial.available()) {
    input = Serial.readStringUntil('\n');
    String data = input.substring(0, 1);
    if (data == "w") {
      printWhether(input);
      printIsSunset(input);
    }
  }
}

void printIsSunset(String input) {
  if (input == "sunset") {
    digitalWrite(sunset, HIGH);
  } else if (input == "sunrise") {
    digitalWrite(sunset, LOW);
  }
}
// 날씨 업데이트가 1시간에 한번씩은 필요할 듯?
void printWhether(String whether) {
  if (whether == "wSunny") {
    digitalWrite(sunny, HIGH);
    digitalWrite(cloudy, LOW);
    digitalWrite(snowy, LOW);
    digitalWrite(rainy, LOW);
    digitalWrite(lightning, LOW);
  } else if (whether == "wSunnyCloudy") {
    digitalWrite(sunny, HIGH);
    digitalWrite(cloudy, HIGH);
    digitalWrite(snowy, LOW);
    digitalWrite(rainy, LOW);
    digitalWrite(lightning, LOW);
  } else if (whether == "wCloudy") {
    digitalWrite(sunny, LOW);
    digitalWrite(cloudy, HIGH);
    digitalWrite(snowy, LOW);
    digitalWrite(rainy, LOW);
    digitalWrite(lightning, LOW);
  } else if (whether == "wSnowy") {
    digitalWrite(sunny, LOW);
    digitalWrite(cloudy, LOW);
    digitalWrite(snowy, HIGH);
    digitalWrite(rainy, LOW);
    digitalWrite(lightning, LOW);
  } else if (whether == "wRainy") {
    digitalWrite(sunny, LOW);
    digitalWrite(cloudy, LOW);
    digitalWrite(snowy, LOW);
    digitalWrite(rainy, HIGH);
    digitalWrite(lightning, LOW);
  } else if (whether == "wLightning") {
    digitalWrite(sunny, LOW);
    digitalWrite(cloudy, LOW);
    digitalWrite(snowy, LOW);
    digitalWrite(rainy, LOW);
    digitalWrite(lightning, HIGH);
  }
}
