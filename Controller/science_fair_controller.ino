#include <MPU6050.h>
#include <Wire.h>
#include <Keyboard.h>

#define PIN_ANALOG_X 0
#define PIN_ANALOG_Y 1

#define BUTTON_A 10
#define BUTTON_B 9
#define BUTTON_C 4
#define BUTTON_D 5
#define BUTTON_E 6
#define BUTTON_F 7
#define BUTTON_G 8 // analog stick button

MPU6050 mpu;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int data;

void setup()
{
  Wire.begin();
  
  Serial.begin(9600);

  mpu.initialize();
  
  pinMode(BUTTON_A, INPUT_PULLUP);
  digitalWrite(BUTTON_A, HIGH);

  pinMode(BUTTON_B, INPUT_PULLUP);
  digitalWrite(BUTTON_B, HIGH);

  pinMode(BUTTON_C, INPUT_PULLUP);
  digitalWrite(BUTTON_C, HIGH);

  pinMode(BUTTON_D, INPUT_PULLUP);
  digitalWrite(BUTTON_D, HIGH);

  pinMode(BUTTON_E, INPUT_PULLUP);
  digitalWrite(BUTTON_E, HIGH);

  pinMode(BUTTON_F, INPUT_PULLUP);
  digitalWrite(BUTTON_F, HIGH);

  pinMode(BUTTON_G, INPUT_PULLUP);
  digitalWrite(BUTTON_G, HIGH);

  Keyboard.begin();
}

void loop()
{
  
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (Serial.available()) {
      data = Serial.read();
      if (data == 'a') Serial.println(ax);
      if (data == 'b') Serial.println(ay);
      if (data == 'c') Serial.println(az);
      if (data == 'd') Serial.println(gx);
      if (data == 'e') Serial.println(gy);
      if (data == 'f') Serial.println(gz);  
  }
  
  if (digitalRead(BUTTON_A) == LOW) Keyboard.press('t');
  else Keyboard.release('t');

  if (digitalRead(BUTTON_B) == LOW) Keyboard.press('h');
  else Keyboard.release('h');

  if (digitalRead(BUTTON_C) == LOW) Keyboard.press('g');
  else Keyboard.release('g');

  if (digitalRead(BUTTON_D) == LOW) Keyboard.press('f');
  else Keyboard.release('f');

  if (digitalRead(BUTTON_E) == LOW) Keyboard.press('1');
  else Keyboard.release('1');

  if (digitalRead(BUTTON_F) == LOW) Keyboard.press('2');
  else Keyboard.release('2');

  if (digitalRead(BUTTON_G) == LOW) Keyboard.press('3');
  else Keyboard.release('3');

  if (analogRead(PIN_ANALOG_Y) > 600)Keyboard.press('w');
  else Keyboard.release('w');
  
  if (analogRead(PIN_ANALOG_Y) < 400) Keyboard.press('s');
  else Keyboard.release('s');

  if (analogRead(PIN_ANALOG_X) > 600) Keyboard.press('d');
  else Keyboard.release('d');

  if (analogRead(PIN_ANALOG_X) < 400) Keyboard.press('a');
  else Keyboard.release('a');
}