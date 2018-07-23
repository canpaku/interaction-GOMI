#include <Servo.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "Ultrasonic.h"

//ultrasonic
Ultrasonic Us_judge_hand_cap(2);
Ultrasonic Us_throwed_cap(3);
Ultrasonic Us_judge_hand_label(4);
Ultrasonic Us_throwed_label(5);
Ultrasonic Us_judge_hand_bottle(6);
Ultrasonic Us_throwed_bottle(7);
//distance getting from ultrasonic sensor
long distance_cap, distance_label, distance_bottle;
long just_throwed_cap, just_throwed_label, just_throwed_bottle;

//servo motor
#define FULL_OPEN 120
#define LITTLE_OPEN 150
#define CLOSE 180
Servo Motor_cap;
Servo Motor_label;
Servo Motor_bottle;
int motorPin_cap = 8;
int motorPin_label = 9;
int motorPin_bottle = 10;
//キープしておきたいカウント数を格納しておく変数
int n_cap, n_label, n_bottle;

//angle of motor
int motorAngle_cap;
int motorAngle_label;
int motorAngle_bottle;

//speaker(buzzer)
int speakerPin = 11; 

//infrared
int infraredPin = 12;
int judge_infrared;

//which_term : cap/label/bottle
int which_term;

int countNum;

#define CAP 0
#define LABEL 1
#define BOTTLE 2

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));

  pinMode(infraredPin, INPUT);

  Motor_cap.attach(motorPin_cap);
  Motor_label.attach(motorPin_label);
  Motor_bottle.attach(motorPin_bottle);

  motorAngle_cap = CLOSE;
  motorAngle_label = CLOSE;
  motorAngle_bottle = CLOSE;
  
  Motor_cap.write(motorAngle_cap);
  Motor_label.write(motorAngle_label);
  Motor_bottle.write(motorAngle_bottle);

  n_cap = 0;
  n_label = 0;
  n_bottle = 0;

  distance_cap = 0;
  just_throwed_cap = 0;
  distance_label = 0;
  just_throwed_label = 0;
  distance_bottle = 0;
  just_throwed_bottle = 0;

  judge_infrared = LOW;

  which_term = CAP;

  countNum = 0;
}

/////////----------  loop  ---------------/////////

void loop() {
//  timer = millis();
  
  distance_cap = Us_judge_hand_cap.MeasureInCentimeters();
  just_throwed_cap = Us_throwed_cap.MeasureInCentimeters();
  
  distance_label = Us_judge_hand_label.MeasureInCentimeters();
  just_throwed_label = Us_throwed_label.MeasureInCentimeters();
  
  distance_bottle = Us_judge_hand_bottle.MeasureInCentimeters();
  just_throwed_bottle = Us_throwed_bottle.MeasureInCentimeters();

  judge_infrared = digitalRead(infraredPin);


  ////- - - - - - - -infrared sensor- - - - - - //////
  
  if (judge_infrared == TRUE){
    //パ
    motorAngle_cap = LITTLE_OPEN;
    motorAngle_label = LITTLE_OPEN;
    motorAngle_bottle = LITTLE_OPEN;
    
    n_cap = countNum;
    n_label = countNum;
    n_bottle = countNum;
  }
  
  //キャップ
  if (motorAngle_cap == LITTLE_OPEN){//パタパタの「パ」の角度で
    if((countNum - n_cap) > 100){//かつ「パ」になったタイミングから100カウント済んでいたら
      motorAngle_cap = CLOSE;
      n_cap = 0;
    }
  }
  //ラベル
  if (motorAngle_label == LITTLE_OPEN){//パタパタの「パ」の角度で
    if((countNum - n_label) > 100){//かつ「パ」になったタイミングから100カウント済んでいたら
      motorAngle_label = CLOSE;
      n_label = 0;
    }
  }
  //ボトル
  if (motorAngle_bottle == LITTLE_OPEN){//パタパタの「パ」の角度で
    if((countNum - n_bottle) > 100){//かつ「パ」になったタイミングから100カウント済んでいたら
      motorAngle_bottle = CLOSE;
      n_bottle = 0;
    }
  }

  ////- - - - - - - -ultrasonic sensor- - - - - - //////

  //キャップ
  if(distance_cap < 8){
    motorAngle_cap = FULL_OPEN;
  }
  //ラベル
  if(distance_label < 8){
    motorAngle_label = FULL_OPEN;
  }
  //ボトル
  if(distance_bottle < 8){
    motorAngle_bottle = FULL_OPEN;
  }

  //毎ループここでモーターの角度をmotorAngleにする
  Motor_cap.write(motorAngle_cap);
  Motor_label.write(motorAngle_label);
  Motor_bottle.write(motorAngle_bottle);

  trashSystem(which_term, judge_infrared, distance_cap, just_throwed_cap, distance_label, just_throwed_label,
  distance_bottle, just_throwed_bottle);
  
  countNum++;
  delay(10);
}

//距離センサに近づいたらモータがパカパカしだしてもっと近づいたら開く関数
void runMotor(int range, Servo motor) {
  if (range < 12){
    //12cmより近づいたら
    motor.write(120);
    piyo();
    delay(1000);
    motor.write(130);
    piyo();
  } else  if (range < 20 && range >= 12){
    //20cmより近づいたら
    delay(300);
    motor.write(175);
    delay(300);
    motor.write(150);
  } else {
    //ある程度離れてる段階
    motor.write(175);
  }
}

//ものが捨てられたら何かが起こる関数
// range:センサと物体の距離 motor:どのモータを動かすか
void trashThrowed(int range, Servo motor){
  if(range < 3){
    //センサから10cm以内のところに反応！→なにか捨てられたとき
    //喜んでパタパタってする
    for (int i = 0; i < 2; i++){
      delay(random(100, 200));
      motor.write(180);
      delay(random(100, 200));
      motor.write(160);
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
  playTone(2000, 10);
  delay(40);
  playTone(2000, 10);
  delay(50);
  playTone(2000, 10);
  delay(500);
}
