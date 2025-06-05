/**
  ******************************************************************************
  * @file    SI5351-VFO-PRO-MINI-REV0.ino
  * @author  Stefano Homebrew
  * @version V1.0
  * @date    05-06-2025
  *
  *
  ******************************************************************************
  *
  *                     SI5351-VFO-PRO-MINI-REV0 - PROJECT
  *
  * This project "SI5351-VFO-PRO-MINI-REV0", define an experimental open platform to build
  * a RX Vfo for Radio's Homebrewing 
  * 
  * BUTTONS:
  * bottoneBfo:  BFO ON/OFF
  * bottoneBand: FOR DEFINED RADIO BANDS
  * bottoneMfrq: I.F. FREQUENCY VALUE ----OPTIONAL---
  * bottoneStep: ROTARY ENCODER BUTTON FOR STEP FREQUENCY
  * 
  * Library: 
  * Led_Control https://github.com/wayoda/LedControl
  * Rotary Encoder https://github.com/brianlow/Rotary
  * Si5351 https://github.com/etherkit/Si5351Arduino
  * 
  * My projects videos https://www.youtube.com/@stefanohomebrew
  *
  * NOTE: this is an experimental project and the functions can be changed
  * without any advise.
  * The SI5351-VFO-PRO-MINI-REV0 uses examples and projects freeware and available in the opensource
  * community.
  *
  * The SI5351-VFO-PRO-MINI-REV0 openSource software is released under the license:
  *              Common Creative - Attribution 3.0
  ******************************************************************************
*/

#include "LedControl.h"
#include <Rotary.h>
#include "Wire.h"
#include <si5351.h>

unsigned long freq_old = 0;
unsigned long freq = 9670000; 
unsigned long freq3 = (freq);
unsigned long freqSi = 455000;
unsigned long freqPLL = 0;

long bfoPLL = 455000;
byte bfo= 0;
byte varBfo= 0;
int passo = 10000;
byte pass= 1;
byte band = 0;
byte mdfrq = 0;

int bottoneStep = 4;/// PULSANTE PASSO/STEP
int bottoneBand = A1;/// PULSANTE BANDE
int bottoneMfrq = A2;/// NON USATO SU QUESTA VERSIONE
int bottoneBfo  = A3;/// PULSANTE BFO ON/OFF

/*
 pin 1 is connected to the DataIn 6
 pin 2 is connected to the CLK 8
 pin 3 is connected to CS 7
*/
LedControl lc=LedControl(6,8,7,1);
Rotary r = Rotary(2, 3);
Si5351 si5351;


void setup() {
    Serial.begin (9600);
    pinMode(bottoneStep,INPUT_PULLUP);
    pinMode(bottoneBand,INPUT_PULLUP);
    pinMode(bottoneMfrq,INPUT_PULLUP);///PER FUTURE VERSIONI
    pinMode(bottoneBfo,INPUT_PULLUP);
    pinMode(5,OUTPUT);

    digitalWrite(5, LOW);
    
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
    sei();
//////////////////////////////////////////////SI5351 INIT VALUE
    si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0,0);
    si5351.set_correction(157000, SI5351_PLL_INPUT_XO);//DA CALIBRARE
    si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);
    si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA);
  //  si5351.output_enable(SI5351_CLK1, 1);///////////////TX OUT (OPTIONAL)
    si5351.output_enable(SI5351_CLK0, 0);///////////////RX OUT
    si5351.output_enable(SI5351_CLK1, 0);
    
//////////////////////////////////////////////LED INIT VALUE    
    lc.shutdown(0,false);// turn off power saving, enables display
    lc.setIntensity(0,10);// sets brightness (0~15 possible values)
    lc.clearDisplay(0);// clear screen
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'-',false);
    lc.setChar(0,5,'-',false);
    lc.setChar(0,4,'4',false);
    lc.setChar(0,3,'5',false);
    lc.setChar(0,2,'5',false);
    lc.setChar(0,1,'0',false);
    lc.setChar(0,0,'0',false);
    delay(2000);  
    printNumber(freq3);
    inviapll();  
}
 
/////////////////////////////////////////////////////  
ISR(PCINT2_vect) {
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {  
    freq = (freq + passo);
    freq3 = (freq);
   printNumber(freq3);
}
  else if (result == DIR_CCW) {   
    freq = (freq - passo);
    freq3 = (freq);
   printNumber(freq3);


  } 
}
//////////////////////////////////////////////////////////////////
void loop() {
  
////////////////////////////BOTTONE RTX PORT A3///DIVENTA BOTTONE BFO ON/OFF
  if (digitalRead(bottoneBfo) == LOW){// ATTIVA DISATTIVA BFO
  delay(300); 
  bfo1();
  }
//////////////////////////////END BOTTONE RTX  OPTIONAL
    if (digitalRead(bottoneStep) == LOW) {
    delay(200); 
    Step();
    }
    ////////////////////////////////START  BANDS SWITCH
 
    if (digitalRead(bottoneBand) == LOW) {
    delay(200); 
    Bands();
    }
   
    ////////////////////////////////END  BANDS SWITCH
    if (digitalRead(bottoneMfrq) == LOW) {
    delay(200); 
    MediaFrq();
    }
    /////////////////////////////////////IF ROTARY ENCODER ROTATION         
     if (freq != freq_old)  {   
     inviapll();
     freq_old = freq;
     }
}
////////////////////////////////SEND TO SI5351
void inviapll() {
    freqPLL = (freq + freqSi);
    //freqPLL = (freq );
    si5351.set_freq ((freqPLL * 100), SI5351_CLK0);
  
}

void printNumber(long num)
{
  long terzo;
  long quinto;
  long quarto;
  long sesto;
  long settimo;
  long ottavo;
  long var4=(long)num;
  long secondo,primo; 
  primo=var4%10;
  var4=var4/10;
  secondo=var4%10;
  var4=var4/10;
  terzo=var4%10;
  var4=var4/10;
  quarto=var4%10;
  var4=var4/10;
  quinto=var4%10; 
  var4=var4/10; 
   sesto=var4%10; 
  var4=var4/10; 
  settimo=var4%10; 
  var4=var4/10; 
  ottavo=var4%10; 
  var4=var4/10; 
  
  //Now print the number digit by digit
  lc.setDigit(0,7,(byte)ottavo,false);
  lc.setDigit(0,6,(byte)settimo,true);
  lc.setDigit(0,5,(byte)sesto,false);
  lc.setDigit(0,4,(byte)quinto,false);
  lc.setDigit(0,3,(byte)quarto,true);
  lc.setDigit(0,2,(byte)terzo,false);
  lc.setDigit(0,1,(byte)secondo,false);
  lc.setDigit(0,0,(byte)primo,false);
  delay (300);  
  Serial.println(freq); 
}
////////////////////////////////STEP     
void Step(){   
    pass++;
    if (pass > 4) {
      (pass = 1);
    }
    if (pass < 1) {
      (pass = 1);
    }
   if (pass == 1) {
   printNumber(1000);
    
      passo = 1000;
    }
   if (pass == 2) {
   printNumber(10000);
      
      passo = 10000;
    }
    if (pass == 3) {
   printNumber(20000);
      
      passo = 20000;
    }
   if (pass == 4) {
   printNumber(100);
     
      passo = 100;    
    }        
 Serial.println(passo);   
}

//////////////////////////////// BANDS SWITCH
void Bands(){  
   band++;
   if (band == 1) {
    freq = 15700000;
    inviapll();
    freq3 = (freq);
   printNumber(freq3);
     
      passo = 10000;
    }
    if (band == 2) {
    freq = 7200000;
    inviapll();
    freq3 = (freq);
   printNumber(freq3);
    
      passo = 10000;
    }   
   if (band == 3) {
    freq = 9670000;
    inviapll();
    freq3 = (freq);
   printNumber(freq3);
     
      passo = 10000;
    }   
   if (band == 4) {
    freq = 10100000;
    inviapll();
    freq3 = (freq);
   printNumber(freq3);
      
      passo = 10000;
    }
       if (band == 5) {
    freq = 1017000;
    inviapll();
    freq3 = (freq);
   printNumber(freq3);
     
      passo = 10000;
    }      
       if (band == 5) {
        band = 0;
       }
   //    Serial.println(band);
}
////////////////////////////////END BANDS 
/////////////////////////////////START MEDIA FRQ OPTIONAL
void MediaFrq(){  
   mdfrq++;
   if (mdfrq == 1) {
    freqSi = 0;
    inviapll();
   
   printNumber(0);
     

    }   
   if (mdfrq == 2) {
    freqSi = 10700000;
    inviapll();
   printNumber(freqSi);
      
      passo = 1000;
    }   
   if (mdfrq == 3) {
    freqSi = 10600000;
    inviapll();

   printNumber(freqSi);
    

    }
       if (mdfrq == 3) {
        mdfrq = 0;
       }
      // Serial.println(mdfrq);
}
///////////////////////////////////////////////BFO OUT - ATTIVA USCITA CLOCK 1 SI 5351
void bfo1(){ 
    bfo++;
     switch (bfo) {  
case 1:   
    si5351.output_enable(SI5351_CLK1, 1);
    si5351.set_freq ((bfoPLL * 100), SI5351_CLK1);// ATTIVA USCITA CLOCK 1 SI 5351
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'B',false);
    lc.setChar(0,5,'F',false);
    lc.setChar(0,4,'0',false);
    lc.setChar(0,3,'-',false);
    lc.setChar(0,2,'4',false);
    lc.setChar(0,1,'5',false);
    lc.setChar(0,0,'5',false);
break;
case 2:
    si5351.output_enable(SI5351_CLK1, 0);//DISATTIVA USCITA CLOCK 1 SI 5351
    lc.setChar(0,7,'-',false);
    lc.setChar(0,6,'0',false);
    lc.setChar(0,5,'F',false);
    lc.setChar(0,4,'F',false);
    lc.setChar(0,3,'-',false);
    lc.setChar(0,2,'4',false);
    lc.setChar(0,1,'5',false);
    lc.setChar(0,0,'5',false);   
    bfo=0;
break;     
    } 
}       
