#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"
//#include <random>

Ultrasonic ultrasonic2(2);
Ultrasonic ultrasonic3(3);
Ultrasonic ultrasonic4(4);
Ultrasonic ultrasonic5(5);

Servo Motor2;
Servo Motor3;
Servo Motor4;
Servo Motor5;

int motorPin = 8;
int speakerPin = 6; 

int shaft = 180;


void setup() {
  Serial.begin(9600);

  Motor3.attach(motorPin);     // サーボモータの接続
  Motor3.write(shaft); 
}

void loop() {
  //距離センサで得た距離（0〜400cm）
  long Range2, Range3, Range4, Range5;
  Range2 = ultrasonic2.MeasureInCentimeters();
  Range3 = ultrasonic3.MeasureInCentimeters();
  Range4 = ultrasonic4.MeasureInCentimeters();
  Range5 = ultrasonic5.MeasureInCentimeters();

//  runMotor(Range3, Motor3);
//  trashThrowed(Range2, Motor3);
}

//距離センサに近づいたらモータがパカパカしだしてもっと近づいたら開く関数
void runMotor(int range, Servo motor) {
  if (range < 12){
    //12cmより近づいたら
    motor.write(120);
    piyo();
    delay(1000);
    motor.write(130);
  } else  if (range < 20 && range >= 12){
    //20cmより近づいたら
    delay(300);
    motor.write(180);
    piyo();
    delay(300);
    motor.write(150);
    piyo();
  } else {
    //ある程度離れてる段階
    motor.write(180);
  }
}

//ものが捨てられたら何かが起こる関数
// range:センサと物体の距離 motor:どのモータを動かすか
void trashThrowed(int range, Servo motor){
  if(range < 10){
    //センサから10cm以内のところに反応！→なにか捨てられたとき
    //めっちゃパタパタする
    for (int i = 0; i < 5; i++){
      delay(random(100, 200));
      motor.write(180);
      delay(random(100, 200));
      motor.write(160);
      delay(random(100, 200));
      motor.write(140);
    }
  }
}


void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

void piyo(){
  playTone(2000, 1);
  delay(40);
  playTone(2000, 1);
  delay(50);
  playTone(2000, 1);
  delay(500);
}

