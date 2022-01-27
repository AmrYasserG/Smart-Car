#include <Arduino_FreeRTOS.h>
#include <semphr.h>

int FLWheelEN = 5;
int BLWheelEN = 8;
int BRWheelEN = 7;
int FRWheelEN = 6;
int FLWheel1 = 25;
int FLWheel2 = 24;
int BLWheel1 = 23;
int BLWheel2 = 22;
int BRWheel1 = 29;
int BRWheel2 = 28;
int FRWheel1 = 26;
int FRWheel2 = 27;
int backTrig= 10;
int backEcho= 9;
int frontEcho=4 ;
int frontTrig= 3;
int distanceBack,distanceFront ;
int startPark =1;
int buzzer = 11;
int parkStage = 1 ;

void frontUltrasonic(){
   
   pinMode(frontTrig, OUTPUT);
   digitalWrite(frontTrig, LOW);
   delayMicroseconds(2);
   digitalWrite(frontTrig, HIGH);
   delayMicroseconds(10);
   digitalWrite(frontTrig, LOW);
   pinMode(frontEcho, INPUT);
   long duration = pulseIn(frontEcho, HIGH);
   distanceFront = duration * 0.034 / 2 ;    //d = v*t  over 2 cuz the wave hit the target and come back again
   Serial.print(distanceFront);
   Serial.print("cm front");
   Serial.println();
}

void backUltrasonic(){
   pinMode(backTrig, OUTPUT);
   digitalWrite(backTrig, LOW);
   delayMicroseconds(2);
   digitalWrite(backTrig, HIGH);
   delayMicroseconds(10);
   digitalWrite(backTrig, LOW);
   pinMode(backEcho, INPUT);
   long duration = pulseIn(backEcho, HIGH);
   distanceBack = duration * 0.034 / 2 ;    //d = v*t  over 2 cuz the wave hit the target and come back again
   if(distanceBack<15&&distanceFront>7){
    tone(buzzer, (1000-(distanceBack*5)));
   }
   else
    noTone(buzzer);
   Serial.print(distanceBack);
   Serial.print("cm back");
   Serial.println();
}

void carSetup(){
   pinMode(FLWheelEN, OUTPUT);
  pinMode(BLWheelEN, OUTPUT);
  pinMode(BRWheelEN, OUTPUT);
  pinMode(FRWheelEN, OUTPUT);
  pinMode(FLWheel1, OUTPUT);
  pinMode(FLWheel2, OUTPUT);
  pinMode(BLWheel1, OUTPUT);
  pinMode(BLWheel2, OUTPUT);
  pinMode(BRWheel1, OUTPUT);
  pinMode(BRWheel2, OUTPUT);
  pinMode(FRWheel1, OUTPUT);
  pinMode(FRWheel2, OUTPUT);
  }


void carBack(){
  digitalWrite(FLWheel1,HIGH); 
  digitalWrite(FLWheel2,LOW);
  digitalWrite(BLWheel1,HIGH);
  digitalWrite(BLWheel2,LOW);
  digitalWrite(BRWheel1,HIGH);
  digitalWrite(BRWheel2,LOW);
  digitalWrite(FRWheel1,HIGH);
  digitalWrite(FRWheel2,LOW);
  analogWrite(FLWheelEN, 100);
  analogWrite(BLWheelEN, 100);
  //analogWrite(BRWheelEN, 0);
 // analogWrite(FRWheelEN, 0);
}
void carFront(){
  digitalWrite(FLWheel1,LOW); 
  digitalWrite(FLWheel2,HIGH);
  digitalWrite(BLWheel1,LOW);
  digitalWrite(BLWheel2,HIGH);
  digitalWrite(BRWheel1,LOW);
  digitalWrite(BRWheel2,HIGH);
  digitalWrite(FRWheel1,LOW);
  digitalWrite(FRWheel2,HIGH);
  analogWrite(FLWheelEN, 80);
  analogWrite(BLWheelEN, 80);
  analogWrite(BRWheelEN, 80);
  analogWrite(FRWheelEN, 80);
}

void carBackLeft(){
  digitalWrite(BRWheel1,HIGH);
  digitalWrite(BRWheel2,LOW);
  digitalWrite(FRWheel1,HIGH);
  digitalWrite(FRWheel2,LOW);
  analogWrite(BRWheelEN, 150);
  analogWrite(FRWheelEN, 150);
  
}
void carBackRight(){
  digitalWrite(FLWheel1,HIGH); 
  digitalWrite(FLWheel2,LOW);
  digitalWrite(BLWheel1,HIGH);
  digitalWrite(BLWheel2,LOW);
  analogWrite(FLWheelEN, 150);
  analogWrite(BLWheelEN, 150);
  
  
}
void carStop(){
  analogWrite(FLWheelEN, 0);
  analogWrite(BLWheelEN, 0);
  analogWrite(BRWheelEN, 0);
  analogWrite(FRWheelEN, 0);
}

void ultrasonicSensors(void *pvParameters){
  TickType_t xLastWakeTime;
  const TickType_t xDelays=pdMS_TO_TICKS(500);
  xLastWakeTime = xTaskGetTickCount();
  while(1){
    frontUltrasonic();
    backUltrasonic();
    vTaskDelayUntil(&xLastWakeTime,xDelays);
  }
}

void park(void *pvParameters){
  while(startPark==1){
    if(distanceFront>7
    ){
      if(parkStage==1){
       carBackRight();
      if(distanceBack<=40){
       carStop();
       parkStage=2;
       }
    }
    if(parkStage==2){
       carBack();
       if(distanceBack<=18){
       carStop();
       parkStage=3;
       }
    }
    if(parkStage==3){
       carBackLeft();
       if(distanceBack<=8){
       carStop();
       parkStage=4;
       }
    }
    if(parkStage==4){
      carFront();
      }
      if(distanceFront<=12 ){
        carStop();
        parkStage=5;
      }
    if(parkStage==5){
      carStop();
      startPark=0;
    }
    }else
    carStop();
  }
}


void setup() {
  xTaskCreate(park,"park",128,NULL,1,NULL);
   xTaskCreate(ultrasonicSensors,"park",128,NULL,2,NULL);
  Serial.begin(9600);
 carSetup();
}

void loop() {
}
