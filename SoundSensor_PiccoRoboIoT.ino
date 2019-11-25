#include <vs-rc202.h>
#include "motion.h"

// モーションを無効化するカウント数
#define CNT_NUM_DISABLE_MOTION 30

// モーションを無効化するカウント
int disableMotionCnt = 0;

void setup() {
    Serial.begin(115200);
    initLib();

    // サーボは頭1、腰1、脚2の合計4自由度になっている
    // 各サーボを有効化
    servoEnable(1, 1);
    servoEnable(2, 1);
    servoEnable(3, 1);
    servoEnable(4, 1);
}

void loop() {
    // アナログピン１に接続されているサウンドセンサーの値
    int soundVal = readSens(1);
    Serial.print("soundVal:");
    Serial.println(soundVal);

    detectMotion(soundVal);
    selectMotion();
    
    delay(100);
}

void detectMotion(int soundVal){
    // 小さすぎる数値は無視するようにする
    if(soundVal < 30) {
        // モーション無効化(ストップ)までのカウントをしておく
        disableMotionCnt++;
        // 無効化のカウント数まで言っていない場合は、モーション番号を変えない
        if(disableMotionCnt < CNT_NUM_DISABLE_MOTION) {
            return;
        }
    }

    // モーション番号を変えるタイミングで無効化カウントをリセットしておく
    disableMotionCnt = 0;
    
    int motionNum = M_NUM0;
    
    // サウンドセンサーの値(音の大きさ)で、モーション番号を決める
    // 1024段階あるが、あまり大きな値は出ないので、低めの値で割り振っている
    if(soundVal < 30) {
        motionNum = M_NUM0;
    } else if(soundVal < 100) {
        motionNum = M_NUM1;
    } else if(soundVal < 200) {
        motionNum = M_NUM2;
    } else if(soundVal < 300) {
        motionNum = M_NUM3;
    } else if(300 <= soundVal) {
        motionNum = M_NUM4;
    }
    
    // モーション番号を設定する
    setMotionNumber(motionNum);
}

void selectMotion(){
    // 設定していたモーション番号を取得し、その番号毎にモーションを実行する
    switch(getMotionNumber()){              
        case M_NUM0:
            // モーションを一度だけ実行する
            // 第二引数には実行するパターン数を指定するので、
            // 基本的にモーション配列の2次元の要素数を指定すればいい
            playMotionOnce(motion0, 1);
            break;
        case M_NUM1:
            playMotionOnce(motion1, 3);
            break;
        case M_NUM2:
            playMotionOnce(motion2, 3);
            break;
        case M_NUM3:
            // モーションを繰り返し実行する
            playMotion(motion3, 5);
            break;
        case M_NUM4:
            playMotion(motion4, 5);
            break;
    }
}
