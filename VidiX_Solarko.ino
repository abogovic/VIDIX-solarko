#include <ESP32Servo.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_GFX.h>

#define LDR1 12
#define LDR2 4

#define TFT_CS 5
#define TFT_DC 21

#define error 10
int Spoint =  90;
Servo servo;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int PinTipkalo_L_R = 34;
int brojac=0;

int mVperAmp = 100; 
int RawValue= 0;
int ACSoffset = 2500; 
double Voltage = 0;
double Amps = 0;

int PinTemp = 26;
int temp;
int lightValue;

float tempV;
float tempC;

float tempMax=35.00;
int postotak;
float postotak_2;
int intezitet_br;

void setup() {

  servo.attach(13); 
  servo.write(Spoint);
  delay(10);
  
  pinMode(PinTemp, INPUT);
  pinMode(PinTipkalo_L_R, INPUT_PULLUP);

  Serial.begin(9600);
  
  tft.begin();
  tft.setRotation(3);
}

void loop() {

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);

  for(int i=0; i<=30;i++){
    motor();
  }

  if(brojac==-1){
    brojac=3;
  } else if(brojac==4){
    brojac=0;
  }

  if (brojac==0){

    temper();

  } else if(brojac==1){

    intezitet();

  } else if(brojac==2){

    jakost_struje();

  } else if(brojac==3){

    napon();
  }

}

void temper(){

  tft.setCursor(8, 30);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.println("Temperatura zraka");

  while(brojac==0){

    temp=analogRead(PinTemp);
    tempV=map(temp, 0, 4095, 0, 3300);
    tempC=tempV/10-40;

    for(int i=58; i<62; i++){
      if (tempC<=15){
        tft.drawFastHLine(10, i, 300, ILI9341_RED); 
      } else if (tempC>15 && tempC<=25){ 
        tft.drawFastHLine(10, i, 300, ILI9341_YELLOW); 
      } else {
        tft.drawFastHLine(10, i, 300, ILI9341_GREEN); 
        }

    if (tempC<=15){
      tft.setTextColor(ILI9341_RED);

    } else if (tempC>15 && tempC<=25){
      tft.setTextColor(ILI9341_YELLOW);

    } else{
      tft.setTextColor(ILI9341_GREEN);
    }

    for(int i=2; i<4; i++){
      if (tempC<=15){
      tft.drawCircle(210, 110, i, ILI9341_RED);

      } else if (tempC>15 && tempC<=25){
        tft.drawCircle(210, 110, i, ILI9341_YELLOW);

      } else{
        tft.drawCircle(210, 110, i, ILI9341_GREEN);
      }
    }

    tft.drawRoundRect(6, 190, 307, 48, 10, ILI9341_RED);

    tft.setTextSize(1);
    tft.setCursor(12, 198);
    tft.setTextColor(ILI9341_RED);
    tft.println("PAZNJA");
    
    if (tempC<=15){
      tft.setTextColor(ILI9341_RED);

    } else if (tempC>15 && tempC<=25){
      tft.setTextColor(ILI9341_YELLOW);

    } else{
      tft.setTextColor(ILI9341_GREEN);
    }

    tft.setCursor(17, 213);
    if (tempC<=15){
      tft.println("Niske temperature mogu biti ukazatelj na sporije");

    } else if (tempC>15 && tempC<25){
      tft.println("Sobne temperature mogu biti ukazatelj na normalno");

    }else{
      tft.println("Visoke temperature mogu biti ukazatelj na brze");
    }

    tft.setCursor(17, 223);
    tft.println("punjenje baterije");

    }

    tft.setTextSize(4);
    tft.setCursor(220, 110);
    tft.println("C"); 
    tft.setCursor(60, 110);
    tft.print(tempC);


    if (analogRead(PinTipkalo_L_R) > 4000) {
      brojac+=1;
    } else if (analogRead(PinTipkalo_L_R)>1900 and analogRead(PinTipkalo_L_R)<2000) {
      brojac-=1;
    }

    delay(200);

    tft.fillRect(60, 105, 140, 40, ILI9341_BLACK);
    tft.fillRoundRect (8, 193, 302, 42, 10, ILI9341_BLACK);

  }

  loop();
 
}

void jakost_struje(){
  
  tft.setTextSize(3);
  tft.setCursor(45, 10);    
  tft.println("Jakost struje");

  for(int i=40; i<43; i++){
    tft.drawFastHLine(10, i, 300, ILI9341_WHITE);
  }

  tft.setTextSize(2);

  while(brojac==2){

    postotak_2 = map(analogRead(33), 4095, 0, 0, 100);

    float average = 0;
    for(int i = 0; i < 1000; i++) 
    {
      average = average + (.0264 * analogRead(15) -13.51+14.6+2) / 1000;
    }

    Voltage = (average / 1024.0) * 5000;
    Amps = (((Voltage - ACSoffset) / mVperAmp)-5)*(postotak_2/100);

    if(average <= 0){
      Amps=0;
    }

   


    tft.setTextSize(3);

    tft.setCursor(200, 120);
    tft.print("A"); 

    if(Amps > 20){
      Amps = 20.0;
    }

    tft.setCursor(80, 120);
    tft.println(abs(Amps),2); 

    if (analogRead(PinTipkalo_L_R) > 4000) {
      brojac+=1;
    } else if (analogRead(PinTipkalo_L_R)>1900 and analogRead(PinTipkalo_L_R)<2000) {
      brojac-=1;
    }

    delay(500);

    tft.fillRect(150, 120, 140, 40, ILI9341_BLACK);
    tft.fillRect(80, 120, 140, 40, ILI9341_BLACK);

    

  }

  loop();
}

void napon(){

  tft.setTextSize(3);
  tft.setCursor(105, 10);
  tft.println("Napon");
  
  for(int i=40; i<43; i++){
    tft.drawFastHLine(10, i, 300, ILI9341_WHITE);
  }

  tft.setTextSize(3);

  while(brojac==3){

  
    postotak_2 = map(analogRead(33), 4095, 0, 0, 100);

    float average = 0;
    for(int i = 0; i < 1000; i++) 
    {
      average = average + (.0264 * analogRead(15) -13.51+13.50+9) / 1000;
    }

    Voltage = ((average / 1024.0) * 500000)*(postotak_2/100);


    if (Voltage<0){
      Voltage=0;
    }

    


    tft.setCursor(210, 120);
    tft.print("mV");


    tft.setCursor(70, 120);
    tft.print(Voltage,2); 
    


    if (analogRead(PinTipkalo_L_R) > 4000) {
      brojac+=1;
    } else if (analogRead(PinTipkalo_L_R)>1900 and analogRead(PinTipkalo_L_R)<2000) {
      brojac-=1;
    }

    delay(500);

    tft.fillRect(210, 120, 140, 40, ILI9341_BLACK);
    tft.fillRect(70, 120, 140, 40, ILI9341_BLACK);

    

  }

  loop();
}

void intezitet(){

  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.print("Intezitet svjetlosti:");
  
  while(brojac==1){
    tft.fillRoundRect(10+postotak*2.5, 145, 24, 16, 2, ILI9341_BLACK);
    tft.drawLine(36+postotak*2.5, 190, 36+postotak*2.5, 145, ILI9341_BLACK);

    tft.setTextSize(2);
    
    int j=0;
    for(int i=35; i<=260; i=i+52){

      tft.setCursor(i,198);
      tft.print(j);
      j+=25;
    }

    lightValue = analogRead(33);

    intezitet_br=abs(lightValue-4095);
    
    tft.fillRoundRect(10, 70, 60, 32, 2, ILI9341_BLACK);

    tft.setCursor(10, 70);
    tft.print(intezitet_br);

    //postotak=intezitet_br/4095*100;
    postotak = map(lightValue, 4095, 0, 0, 100);

    tft.setTextSize(2);

    if (postotak<5){

      for(int brojac=40; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }

      tft.fillRoundRect(40, 170, 20, 16, 2, ILI9341_RED);

    } else if(postotak<13){

      for(int brojac=65; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }

      tft.fillRoundRect(40, 170, 20, 16, 2, ILI9341_RED);

      tft.fillRoundRect(65, 170, 20, 16, 2, ILI9341_RED);

    } else if(postotak<23){

      for(int brojac=90; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }

      tft.fillRoundRect(90, 170, 20, 16, 2, ILI9341_YELLOW);

    } else if(postotak<33){

      for(int brojac=115; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }

      tft.fillRoundRect(90, 170, 20, 16, 2, ILI9341_YELLOW);

      tft.fillRoundRect(115, 170, 20, 16, 2, ILI9341_YELLOW);

    } else if(postotak<51){

      for(int brojac=140; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=140; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }

      tft.fillRoundRect(140, 170, 20, 16, 2, ILI9341_YELLOW);

    } else if(postotak<62){

      for(int brojac=165; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=165; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }

      tft.fillRoundRect(165, 170, 20, 16, 2, ILI9341_YELLOW);

    } else if(postotak<73){

      for(int brojac=190; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){
      
        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=190; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }

      tft.fillRoundRect(190, 170, 20, 16, 2, ILI9341_YELLOW);

    } else if(postotak<82){

      for(int brojac=215; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=190; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }

      tft.fillRoundRect(215, 170, 20, 16, 2, ILI9341_GREEN);

    } else if(postotak<93){

      for(int brojac=240; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_BLACK);
      }
      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=190; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }

      tft.fillRoundRect(215, 170, 20, 16, 2, ILI9341_GREEN);

      tft.fillRoundRect(240, 170, 20, 16, 2, ILI9341_GREEN);

    } else {

      for(int brojac=40; brojac<=65; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_RED);
      }
      for(int brojac=90; brojac<=190; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_YELLOW);
      }
      for(int brojac=215; brojac<=265; brojac+=25){

        tft.fillRoundRect(brojac, 170, 20, 16, 2, ILI9341_GREEN);
      }

    }

    tft.fillRect(37+postotak*2.5, 170, 50, 16, ILI9341_BLACK); //CRNI KVADRAT (30)
    tft.drawRoundRect(35, 165, 255, 26, 5, ILI9341_WHITE);

    tft.fillRoundRect(10+postotak*2.5, 145, 24, 16, 2, ILI9341_BLACK);
    tft.drawLine(36+postotak*2.5, 190, 36+postotak*2.5, 145, ILI9341_BLACK);

    tft.drawLine(36+postotak*2.5, 190, 36+postotak*2.5, 145, ILI9341_WHITE);
    tft.drawRoundRect(10+postotak*2.5, 145, 24, 16, 2, ILI9341_WHITE);

    tft.setCursor(13+postotak*2.5, 150);
    tft.setTextSize(1);
    tft.print(postotak);
    tft.print("%");

    delay(200);

    tft.drawRoundRect(10+postotak*2.5, 145, 24, 16, 2, ILI9341_BLACK);
    tft.fillRoundRect(10+postotak*2.5, 145, 24, 16, 2, ILI9341_BLACK);
    tft.drawLine(36+postotak*2.5, 190, 36+postotak*2.5, 145, ILI9341_BLACK);

    if (analogRead(PinTipkalo_L_R) > 4000) {
      brojac+=1;
    } else if (analogRead(PinTipkalo_L_R)>1900 and analogRead(PinTipkalo_L_R)<2000) {
      brojac-=1;
    }

  }

  loop();

}

void motor(){
  
 
  int ldr1 = analogRead(LDR1);
  
  int ldr2 = analogRead(LDR2);

  
  int value1 = abs(ldr1 - ldr2);
  int value2 = abs(ldr2 - ldr1);

  
  if ((value1 <= error) || (value2 <= error)) {
  } else {
    if (ldr1 > ldr2) {
      Spoint = --Spoint;
    }
    if (ldr1 < ldr2) {
      Spoint = ++Spoint;
    }
  }
  
  servo.write(Spoint);
  delay(80);
  
  }
