#include <stdint.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <TEA5767.h>
#include <Wire.h>

TEA5767 Radio =TEA5767(); //Pinout SLC and SDA

MCUFRIEND_kbv tft;

const int TS_LEFT = 128, TS_RT = 916, TS_TOP = 921, TS_BOT = 88; // screen calibration ID=0x7789
#define YP A3  
#define XM A2 
#define YM 9   
#define XP 8   

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

Adafruit_GFX_Button prevBtn, nextBtn, pauseBtn, playBtn;

int pixelX, pixelY;     //x and y when touch the screen

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define Maroon  0x89A6
#define Sea     0x2371

double currentFreq=105.8;
boolean RadioOff=true ;

//void TouchGetXY(){
//    TSPoint p = ts.getPoint();
//    if (p.z > 200 && p.z< 2000) { //if Touched
//        pixelX = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //to get the right coordinate 
//        pixelY =320- map(p.y, TS_TOP, TS_BOT, 0, tft.height());
//    Serial.print("\tPressure = "); Serial.println(p.z);
//    Serial.print("\tpixelx = "); Serial.print(pixelX);
//    Serial.print("\tpixely = "); Serial.println(pixelY);
//    }
//    delay(100);
//}

void radioNextChannel(){
  if(currentFreq == 90.9){
    currentFreq = 105.8;
    Radio.setFrequency(currentFreq);
  }
  else{
     if(currentFreq == 105.8){
    currentFreq = 88.2;
    Radio.setFrequency(currentFreq);
  }
  else{
    currentFreq = 90.9;
    Radio.setFrequency(currentFreq);
  }
  }
}

void radioPrevChannel(){
  if(currentFreq == 105.8){
    currentFreq = 90.9;
    Radio.setFrequency(currentFreq);
  }
  else{
     if(currentFreq == 88.2){
    currentFreq = 105.8;
    Radio.setFrequency(currentFreq);
  }
  else{
    currentFreq = 88.2;
    Radio.setFrequency(currentFreq);
  }
  }
}

void radioSetup(){
  Wire.begin();
  Radio.setFrequency(currentFreq);
}

void screenSetup(){
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);           
  tft.fillScreen(Maroon);
  prevBtn.initButton(&tft, 35, 220, 75, 40, WHITE, WHITE, Sea, "<<", 2);//x,y,w,h,outline,fill,textcolor,label,text size
  pauseBtn.initButton(&tft,120, 220, 75, 40, WHITE, WHITE, Sea, "||", 2); 
  playBtn.initButton(&tft,120, 220, 75, 40, WHITE, WHITE, Sea, "|>", 2); 
  nextBtn.initButton(&tft, 205, 220, 75, 40, WHITE, WHITE, Sea, ">>", 2);
  nextBtn.drawButton(true);
  prevBtn.drawButton(true);
  pauseBtn.drawButton(true);
  //playBtn.drawButton(false);
  tft.fillRect(10, 30, 220, 140, Sea); // area to display the frequency
  tft.setCursor(20,65); //printing sentences
  tft.setTextSize(3);
  tft.print("station:");
  tft.setCursor(55,100);
  tft.setTextSize(2);
  tft.print(currentFreq);
  tft.setCursor(115,100);
  tft.print(" FM");
}
void handleButtonPress(){
  digitalWrite(13,HIGH);
  TSPoint p = ts.getPoint();
  digitalWrite(13,LOW);       //change the pins dirciction each time we touch to use the screen to display and touch
  pinMode(XM,OUTPUT);
  pinMode(YP,OUTPUT);
    if (p.z > 300 && p.z< 2000) { //if Touched
        pixelX = map(p.x, TS_LEFT, TS_RT, 0, tft.width()); //to get the right coordinate 
        pixelY =320- map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  if(pixelX>=0 && pixelX<=75 &&pixelY<=240 &&pixelY>=200){//prevButton
    radioPrevChannel();
    tft.fillRect(55,100, 70, 20, Sea);
    if(currentFreq==105.8)
      tft.setCursor(55,100);
    else
      tft.setCursor(65,100);
    tft.setTextSize(2);
    tft.print(currentFreq);
    //radioNextChannel();
    delay(300);
  }
  
  if(pixelX>=85 && pixelX<=160 &&pixelY<=240 &&pixelY>=200){//pauseButton
    
    if(RadioOff){
      Radio.setMuted(true);
      pauseBtn.drawButton(false);
      playBtn.drawButton(true);
      tft.fillRect(10, 30, 220, 140, Sea); // area to display the frequency
      tft.setCursor(20,65); //printing sentences
      tft.setTextSize(3);
      tft.print("Radio Off");
    }
    else{
      Radio.setMuted(false);
      playBtn.drawButton(false);
      pauseBtn.drawButton(true);
      tft.fillRect(10, 30, 220, 140, Sea); // area to display the frequency
      tft.setCursor(20,65); //printing sentences
      tft.setTextSize(3);
      tft.print("station:");
      if(currentFreq==105.8)
        tft.setCursor(55,100);
      else
        tft.setCursor(65,100);
      tft.setTextSize(2);
      tft.print(currentFreq);
      tft.setCursor(115,100);
      tft.print(" FM");
    }
    RadioOff =!RadioOff;
    delay(300);
  }
  
  if(pixelX>=170 && pixelX<=245 &&pixelY<=240 &&pixelY>=200){//nextButton
    radioNextChannel();
    tft.fillRect(55,100, 70, 20, Sea);
    if(currentFreq==105.8)
      tft.setCursor(55,100);
    else
      tft.setCursor(65,100);
    tft.setTextSize(2);
    tft.print(currentFreq);
    //radioNextChannel();
    delay(300);
  }
    }
}

void setup(void){
    Serial.begin(9600);
    radioSetup();
    screenSetup();
}

void loop(void){
  handleButtonPress();
 

//    delay(3000);
//    tft.setTextSize(2);
//    tft.fillRect(55,100, 70, 20, Sea);
//    if(currentFreq==100.9){
//      currentFreq=90.9;
//      tft.setCursor(65,100);
//    }
//    else{
//      currentFreq = 100.9 ;
//      tft.setCursor(55,100);
//    }
//    tft.print(currentFreq);
//    

}
