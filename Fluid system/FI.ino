#include <Servo.h>

int a=44; 
int b=43; 
int c=47; 
int d=48; 
int e=49; 
int f=45; 
int g=46; 
int waterSensorPin = A0;

Servo servo;

//display number 0
void display0(void) 
{ 
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
    digitalWrite(d,HIGH);  
    digitalWrite(e,HIGH);  
    digitalWrite(f,HIGH);  
}
//display number 1
void display1(void) 
{ 
    digitalWrite(b,HIGH);
    digitalWrite(c,HIGH);
} 
//display number2
void display2(void) 
{
    digitalWrite(a,HIGH);
    digitalWrite(b,HIGH);
    digitalWrite(g,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(d,HIGH);
} 

void clearDisplay(void) 
{ 
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(g,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);  
    digitalWrite(e,LOW);  
    digitalWrite(f,LOW);  
}

void handleFuel(){
  int fuelLevel = 0;
        fuelLevel = analogRead(waterSensorPin);
        Serial.print("Water level: ");
        Serial.println(fuelLevel);
        
          if (fuelLevel<=535){
            clearDisplay();
            display0(); 
          }
          if(fuelLevel<=555 && fuelLevel>535){
            clearDisplay();
            display1(); 
          }
          if(fuelLevel>555){
            clearDisplay();
            display2();
          }
          //delay(100);
}

void windShield(){
  for(int windShieldAngle = 0; windShieldAngle < 180; windShieldAngle++)  
  {                                  
    servo.write(windShieldAngle);              
    delay(10);                   
  } 
  // now scan back from 180 to 0 degrees
  for(int windShieldAngle = 180; windShieldAngle >= 0; windShieldAngle--)    
  {                                
    servo.write(windShieldAngle);           
    delay(10);       
  } 
}

 
void setup() 
{ 
    Serial.begin(9600);
    int i;
    for(i=2;i<=8;i++) // 7seg pins - common anode
        pinMode(i,OUTPUT); 

    servo.attach(12);//pin
    servo.write(0); //start angle
} 
void loop() 
{     
        handleFuel();
        int rainSen = analogRead(A1);
        Serial.print("rain: ");
        Serial.println(rainSen);
        if(rainSen<800)
          windShield();
        else
        servo.write(0);
}
