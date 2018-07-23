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
//Ultrasonic Us_throwed_bottle(7);

//distance getting from ultrasonic sensor
long distance_cap, distance_label, distance_bottle;
long just_throwed_cap, just_throwed_label, just_throwed_bottle;

//servo motor
#define FULL_OPEN 120
#define LITTLE_OPEN 150
#define CLOSE 180
#define YEAH 140
Servo Motor_cap;
Servo Motor_label;
Servo Motor_bottle;
int motorPin_cap = 14;
int motorPin_label = 15;
int motorPin_bottle = 16;
//キープしておきたいカウント数を格納しておく変数
int n_cap, n_label, n_bottle;
//angle of motor
int motorAngle_cap;
int motorAngle_label;
int motorAngle_bottle;

//speaker(buzzer)
int speakerPin = 11;

//infrared
int infraredPin = 7;
int judge_infrared;
int infra_flag = 0;

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

  distance_cap = 10000;
  just_throwed_cap = 100000;
  distance_label = 10000;
  just_throwed_label = 10000;
  distance_bottle = 10000;
  just_throwed_bottle = 10000;

  judge_infrared = LOW;

  which_term = CAP;

  countNum = 0;
}

//void thankYou(Servo mc, Servo ml, Servo mb){
//  mc.write(CLOSE);
//  ml.write(CLOSE);
//  mb.write(CLOSE);
//  //ぱぱぱ、
//  delay(500);
//  mc.write(YEAH);
//  delay(500);
//  ml.write(YEAH);
//  delay(500);
//  mb.write(YEAH);
//  delay(1000);
//  //ぱぱぱ、
//  mb.write(CLOSE);
//  delay(500);
//  ml.write(CLOSE);
//  delay(500);
//  mc.write(CLOSE);
//  delay(1000);
//  //ぱ、ぱ、ぱ！
//  mc.write(YEAH);
//  ml.write(LITTLE_OPEN);
//  mb.write(YEAH);
//  delay(1000);
//  mc.write(LITTLE_OPEN);
//  ml.write(YEAH);
//  mb.write(LITTLE_OPEN);
//  delay(1000);
//  mc.write(YEAH);
//  ml.write(YEAH);
//  mb.write(YEAH);
//  delay(1000);
//}

/////////----------  loop  ---------------/////////

void loop() {
  //  timer = millis();

  distance_cap = Us_judge_hand_cap.MeasureInCentimeters();
  //  just_throwed_cap = Us_throwed_cap.MeasureInCentimeters();

  //  distance_label = Us_judge_hand_label.MeasureInCentimeters();
  //  just_throwed_label = Us_throwed_label.MeasureInCentimeters();

  //  distance_bottle = Us_judge_hand_bottle.MeasureInCentimeters();
  //  just_throwed_bottle = Us_throwed_bottle.MeasureInCentimeters();

  judge_infrared = digitalRead(infraredPin);


  ////- - - - - - - -infrared sensor- - - - - - //////

  if (judge_infrared == HIGH && infra_flag == false) {
    //パ
    infra_flag = true;
    motorAngle_cap = LITTLE_OPEN;
    motorAngle_label = LITTLE_OPEN;
    motorAngle_bottle = LITTLE_OPEN;

    n_cap = countNum;
    n_label = countNum;
    n_bottle = countNum;
  }

  if (infra_flag == true) {
    //キャップ
    if (motorAngle_cap == LITTLE_OPEN) { //パタパタの「パ」の角度で
      if ((countNum - n_cap) == 50) { //かつ「パ」になったタイミングから100カウント済んでいたら
        motorAngle_cap = CLOSE;
        n_cap = countNum;
      }
    } else if (motorAngle_cap == CLOSE) { //パタパタの「タ」の角度で
      if ((countNum - n_cap) == 50) { //100カウント済んでいたら
        motorAngle_cap = LITTLE_OPEN;
        n_cap = countNum;
      }
    }
    //ラベル
    if (motorAngle_label == LITTLE_OPEN) { //パタパタの「パ」の角度で
      if ((countNum - n_label) == 50) { //かつ「パ」になったタイミングから100カウント済んでいたら
        motorAngle_label = CLOSE;
        n_label = countNum;
      }
    } else if (motorAngle_label == CLOSE) { //パタパタの「タ」の角度で
      if ((countNum - n_label) == 50) { //100カウント済んでいたら
        motorAngle_label = LITTLE_OPEN;
        n_label = countNum;
      }
    }
    //ボトル
    if (motorAngle_bottle == LITTLE_OPEN) { //パタパタの「パ」の角度で
      if ((countNum - n_bottle) == 50) { //かつ「パ」になったタイミングから100カウント済んでいたら
        motorAngle_bottle = CLOSE;
        n_bottle = countNum;
      }
    } else if (motorAngle_bottle == CLOSE) { //パタパタの「タ」の角度で
      if ((countNum - n_bottle) == 50) { //100カウント済んでいたら
        motorAngle_bottle = LITTLE_OPEN;
        n_bottle = countNum;
      }
    }

    ////- - - - - - - -ultrasonic sensor- - - - - - //////
    ////-  .  -  .  -手が近づいたらver.  .  -  .  -  .//////

    //キャップ
    if (distance_cap < 8) {
      if (which_term == CAP) {
        motorAngle_cap = FULL_OPEN;
      } else {//ぴしゃ
        //      Motor_cap.write(175);
        //      delay(300);
        Motor_cap.write(CLOSE);
        delay(300);
        //ここでパタパタの「パ」の状態に戻しておく
        motorAngle_cap = LITTLE_OPEN;
        n_cap = countNum;
      }
    }
    //ラベル
    if (distance_label < 8) {
      if (which_term == LABEL) {
        motorAngle_label = FULL_OPEN;
      } else {
        //ぴしゃ
        //      Motor_label.write(175);
        //      delay(300);
        Motor_label.write(CLOSE);
        delay(300);
        motorAngle_label = LITTLE_OPEN;
        n_label = countNum;
      }
    }
    //ボトル
    if (distance_bottle < 8) {
      if (which_term == BOTTLE) {
        motorAngle_bottle = FULL_OPEN;
      } else {
        //ぴしゃ
        //      Motor_bottle.write(175);
        //      delay(300);
        Motor_bottle.write(CLOSE);
        delay(300);
        motorAngle_bottle = LITTLE_OPEN;
        n_bottle = countNum;
      }
    }

    ////- - - - - - - -ultrasonic sensor- - - - - - //////
    ////-  .  -  .  -ゴミが捨てられたらver.  .  -  .  -  .//////

    if ((just_throwed_cap < 5) && (which_term == CAP)) {
      which_term = LABEL;
    }
    if ((just_throwed_label < 5) && (which_term == LABEL)) {
      which_term = BOTTLE;
    }
    if ((just_throwed_bottle < 5) && (which_term == BOTTLE)) {
      //全員でよろこぶ！
      //    thankYou(Motor_cap, Motor_label, Motor_bottle);

      which_term = CAP;

      //パタパタの「ぱ」状態に戻す
      motorAngle_cap = LITTLE_OPEN;
      motorAngle_label = LITTLE_OPEN;
      motorAngle_bottle = LITTLE_OPEN;

      n_cap = countNum;
      n_label = countNum;
      n_bottle = countNum;
    }

    //毎ループここでモーターの角度をmotorAngleにする
    Motor_cap.write(motorAngle_cap);
    Motor_label.write(motorAngle_label);
    Motor_bottle.write(motorAngle_bottle);

  }



  if ((countNum % 50) == 0) {
    Serial.print(motorAngle_cap);
    Serial.print("::::");
    Serial.print(motorAngle_label);
    Serial.print("::::");
    Serial.print(motorAngle_bottle);
    Serial.print("::::");
    Serial.print(n_cap);
    Serial.print("::::");
    Serial.println(countNum);
  }

  //  Serial.println(judge_infrared);
  countNum++;
  delay(10);
}

//void playTone(int tone, int duration) {
//    for (long i = 0; i < duration * 1000L; i += tone * 2) {
//        digitalWrite(speakerPin, HIGH);
//        delayMicroseconds(tone);
//        digitalWrite(speakerPin, LOW);
//        delayMicroseconds(tone);
//    }
//}
//
//void piyo(){
//  playTone(2000, 10);
//  delay(40);
//  playTone(2000, 10);
//  delay(50);
//  playTone(2000, 10);
//  delay(500);
//}
