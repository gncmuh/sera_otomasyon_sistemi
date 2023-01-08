#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht11.h>
#include <Smoothed.h>
#include <avr/pgmspace.h>

const uint8_t pinler[10] PROGMEM={2,3,4,5,6,7,A0,8,9,10};  // 8,9,10 pinleri tuş takımı olarak ayarlandı
Smoothed <float> toprak_nemi;

union veriler {

  byte nem,sicaklik;
  
};
union veriler veri;
 
LiquidCrystal_I2C lcd(0x27,16,2);
dht11 dht;

bool kapi1,kapi2,kapi3=false;
char ekran_karakterleri[7]={'<','>','E','H','+','-','K'};
unsigned long zaman=0;unsigned long zaman2=0;unsigned long zaman3=0;
byte sayac=7;
float sera_sicaklik=16;int sera_nem=30;int toprak_nem=10;



int say=0;
void setup() {
  Serial.begin(9600);Wire.begin();
  lcd.init();lcd.backlight();
  toprak_nemi.begin(SMOOTHED_AVERAGE,10);
  if(!Serial)
  {
    while(!bool(0));
  }
  else
  {
    for(int i=0; i<10; i++)
    {
      unsigned int deger=pgm_read_word_near(pinler+i);
      deger=(byte)deger;
      if(deger<7) {pinMode(deger,OUTPUT);}
      else if(deger==7 or deger==14) {pinMode(deger,INPUT);}
      else if(deger>=8) {pinMode(deger,INPUT_PULLUP);}  
      delay(1);
    }
    toprak_nemi.begin(SMOOTHED_AVERAGE,10);
    gorsel_ekran();ana_sayfa();
    digitalWrite(pinler[0],HIGH);digitalWrite(pinler[2],HIGH); // sera içi havalandırma ve su motorları aktifleştirildi. 3 ve 5 motor pinleridir.
    
  }
}

void loop() {

  sistem_yonetimi();
  delay(1);
  
}

void gorsel_ekran(void) {

  if(!bool(0))
  {
    lcd.setCursor(1,0);lcd.print(F("SERA OTOMASYON"));
    lcd.setCursor(4,1);lcd.print(F("SISTEMI"));
    delay(1000);lcd.clear();lcd.blink();
  }
}

void ana_sayfa() {

  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(4,0);lcd.print(F("ANA SAYFA"));
    lcd.setCursor(6,1);
    for(int i=0; i<2; i++)
    {
      lcd.print(ekran_karakterleri[i]);lcd.print(F(" "));
      delay(2);
    }
  }
}

void ayarlar(void) {

  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(4,0);lcd.print(F("AYARLAR"));
    for(int i=0; i<3; i++)
    {
      if(i==0){lcd.setCursor(0,1);lcd.print(ekran_karakterleri[i]);}
      else if(i>=1 and i<=2) {if(i==1){lcd.setCursor(6,1);lcd.print(ekran_karakterleri[i+1]);lcd.print(F(" "));}else if(i==2){lcd.setCursor(8,1);lcd.print(ekran_karakterleri[i+1]);}}
    }
  }
  
}

void sicaklik_ayar_ekrani() {

  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(0,0);lcd.print(F("SERA IC SICAKLIK"));
    lcd.setCursor(0,1);lcd.print(ekran_karakterleri[0]);lcd.setCursor(15,1);lcd.print(ekran_karakterleri[1]);
    lcd.setCursor(4,1);lcd.print(ekran_karakterleri[4]);
    lcd.setCursor(5,1);lcd.print(sera_sicaklik);
    lcd.setCursor(10,1);lcd.print(ekran_karakterleri[5]);
    lcd.setCursor(12,1);lcd.print(ekran_karakterleri[6]);
  }
  
}
void nem_ayar_ekrani() {

  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(1,0);lcd.print(F("SERA NEM AYARI"));
    lcd.setCursor(0,1);lcd.print(ekran_karakterleri[0]);lcd.setCursor(15,1);lcd.print(ekran_karakterleri[1]);
    lcd.setCursor(4,1);lcd.print(ekran_karakterleri[4]);
    lcd.setCursor(5,1);lcd.print(F("%"));lcd.setCursor(6,1);lcd.print(sera_nem);
    lcd.setCursor(9,1);lcd.print(ekran_karakterleri[5]);
    lcd.setCursor(11,1);lcd.print(ekran_karakterleri[6]);
  }
  
}
void toprak_nem_ayar_ekrani(void) {

  if(!bool(0))
  {
    lcd.clear();
    lcd.setCursor(0,0);lcd.print(F("SERA NEM(TK) AYARI"));
    lcd.setCursor(0,1);lcd.print(ekran_karakterleri[0]);
    lcd.setCursor(4,1);lcd.print(ekran_karakterleri[4]);
    lcd.setCursor(5,1);lcd.print(F("%"));lcd.setCursor(6,1);lcd.print(toprak_nem);
    lcd.setCursor(9,1);lcd.print(ekran_karakterleri[5]);
    lcd.setCursor(11,1);lcd.print(ekran_karakterleri[6]);
  }
  
}
void guncelleniyor_ekrani() {

  if(!bool(0))
  {
    lcd.clear();lcd.noBlink();
    lcd.setCursor(2,0);lcd.print(F("GUNCELLENIYOR"));
    lcd.setCursor(3,1);
    for(uint8_t x=0; x<2; x++) {digitalWrite(pinler[4],HIGH);delay(80);digitalWrite(pinler[4],LOW);delay(80);}
    for(uint8_t i=0; i<10; i++)
    {
      lcd.print(F("*"));
      delay(100);
    }
    delay(100);
  }
  lcd.blink();
  
}
void sensor_degerleri_ekrani(void) {

  if(!bool(0))
  {
    lcd.clear();lcd.setCursor(0,0);lcd.print(F("SERA DURUMU KONTROL"));
    lcd.setCursor(0,1),lcd.print(F("<"));lcd.setCursor(15,1),lcd.print(F(">"));
    lcd.setCursor(6,1);lcd.print(F("A"));lcd.setCursor(8,1);lcd.print(F("K"));
  }
}


void otomasyon_baslatma_ekrani(void) {

  if(!bool(0))
  {
    lcd.clear();lcd.setCursor(1,0);lcd.print(F("SERA OTOMASYON"));
    for(uint8_t i=1; i<4; i++)
    {
      if(ekran_karakterleri[i]=='>') {lcd.setCursor(i+14,1);lcd.print(ekran_karakterleri[i]);}
      else if(ekran_karakterleri[i]=='E') {lcd.setCursor(i+4,1);lcd.print(ekran_karakterleri[i]);}
      else if(ekran_karakterleri[i]=='H') {lcd.setCursor(i+5,1);lcd.print(ekran_karakterleri[i]);}
    }
  }
}

int sensor_verileri(void) {

  if(!bool(0))
  {
    lcd.noBlink();
    dht.read(pinler[5]);
    float sicaklik=((float)dht.temperature);
    float nem=((float)dht.humidity);
    float t_nem=analogRead(pinler[6]);
    t_nem=map(t_nem,1024,0,0,100);
    if(!bool(0)){
      
      if(millis()-zaman>2000)
      {
        zaman=millis();lcd.clear();
        lcd.setCursor(0,0);lcd.print(F("TOPRAK NEM="));lcd.setCursor(11,0);lcd.print(t_nem);
      }
      else if(millis()-zaman2>4000)
      {
        zaman2=millis();lcd.clear();
        lcd.setCursor(0,0);lcd.print(F("SICAKLIK="));lcd.setCursor(9,0);lcd.print(sicaklik);
        lcd.setCursor(0,1);lcd.print(F("NEM(%)="));lcd.setCursor(7,1);lcd.print(nem);
      }
      else if(millis()-zaman3>30000)
      {
        zaman3=millis();
        if(zaman3>10000){lcd.noBacklight();}
      }
    }
  }
}

void otomasyon_baslatma(void) {

  if(!bool(0))
  {
    lcd.noBlink();
    dht.read(pinler[5]);
    float sicaklik=((float)dht.temperature);
    float nem=((float)dht.humidity);
    float t_nem=analogRead(pinler[6]);
    t_nem=map(t_nem,1024,0,0,100);

    if(sicaklik<=sera_sicaklik)
    {
      bitWrite(PORTD,3,HIGH);
    }
    else if(sicaklik>=sera_sicaklik)
    {
      bitWrite(PORTD,3,LOW);
    }
    if(nem<=sera_nem)
    {
      bitWrite(PORTD,5,HIGH);
    }
    else if(nem>=sera_nem)
    {
      bitWrite(PORTD,5,LOW);
    }
//    if(t_nem<=toprak_nem)
//    {
//      bitWrite(PORTD,5,HIGH);
//    }
//    else if(t_nem>=toprak_nem)
//    {
//      bitWrite(PORTD,5,LOW);
//    }
  }
}

void sistem_yonetimi() {

  if(!bool(0))
  {
    buradan2:
    byte gelen=tus_takimi();
    if(gelen)
    {
      if(gelen==1)
      {
        sayac++;
        if(sayac>8) {sayac=7;}
      }
      else if(gelen==3)
      {
        sayac--;
        if(sayac<6) {sayac=7;}  
      }
      else if(gelen==2 and sayac==8)
      {
        buradan:
        sayac=7;
        ayarlar();
        do
        {
          gelen=tus_takimi();
          if(gelen)
          {
             if(gelen==1)
             {
                sayac++;
                if(sayac>8) {sayac=8;}
                else if(sayac==1) {sayac=7;}
             }
             else if(gelen==3)
             {
                sayac--;
                if(sayac==5) {sayac=0;}
             }
             else if(gelen==2 and sayac==0)
             {
                sayac=7;ana_sayfa();
                goto buradan2;
             }
             else if(gelen==2 and sayac==6)
             {
                buradan3:
                sayac=4;
                sicaklik_ayar_ekrani();             
                while(!bool(0)){
                  gelen=tus_takimi();
                  if(gelen)
                  {
                    if(gelen==1){
                      sayac++;
                      if(sayac==5) {sayac=10;}
                      else if(sayac==1) {sayac=4;}
                      else if(sayac==11) {sayac=12;}
                      else if(sayac==13) {sayac=15;}
                    }
                    else if(gelen==3)
                    {
                      sayac--;
                      if(sayac==14) {sayac=12;}
                      else if(sayac==11) {sayac=10;}
                      else if(sayac==9) {sayac=4;}
                      else if(sayac==3) {sayac=0;}
                    }
                    else if(gelen==2 and sayac==4)
                    {
                      sera_sicaklik+=.5;    
                    }
                    else if(gelen==2 and sayac==10)
                    {
                      sera_sicaklik-=.5;
                      if(sera_sicaklik<0) {sera_sicaklik=0;}
                    }
                    else if(gelen==2 and sayac==12)
                    {
                      guncelleniyor_ekrani();
                      goto buradan;
                    }
                    else if(gelen==2 and sayac==0)
                    {
                      goto buradan;
                    }
                    else if(gelen==2 and sayac==15)
                    {
                      buradan4:
                      sayac=4;
                      delay(10);
                      nem_ayar_ekrani(); 
                      do{
                        gelen=tus_takimi();
                        if(gelen)
                        {
                          if(gelen==1)
                          {
                            sayac++;
                            if(sayac==5){sayac=9;}
                            else if(sayac==1){sayac=4;}
                            else if(sayac==8) {sayac=9;}
                            else if(sayac==10) {sayac=11;}
                            else if(sayac==12) {sayac=15;}
                          }
                          else if(gelen==2 and sayac==4)
                          {
                            sera_nem+=5;
                          }
                          else if(gelen==2 and sayac==9)
                          {
                            sera_nem-=5;
                            if(sera_nem<0) {sera_nem=0;}
                          }
                          else if(gelen==2 and sayac==0)
                          {
                            goto buradan3;
                          }
                          else if(gelen==3)
                          {
                            sayac--;
                            if(sayac==14) {sayac=11;}
                            else if(sayac==10) {sayac=9;}
                            else if(sayac==8) {sayac=4;}
                            else if(sayac==3) {sayac=0;}
                          }
                          else if(gelen==2 and sayac==11)
                          {
                            guncelleniyor_ekrani();
                            goto buradan;
                          }
                          else if(gelen==2 and sayac==15)
                          {
                            sayac=4;
                            delay(10);
                            toprak_nem_ayar_ekrani();
                            while(!bool(0))
                            {
                              gelen=tus_takimi();
                              if(gelen)
                              {
                                if(gelen==1)
                                {
                                  sayac++;
                                  if(sayac==5) {sayac=9;}
                                  else if(sayac==10) {sayac=11;}
                                  else if(sayac==1) {sayac=4;}
                                  else if(sayac==12) {sayac=11;}
                                }
                                else if(gelen==3)
                                {
                                  sayac--;
                                  if(sayac==3) {sayac=0;}
                                  else if(sayac==8) {sayac=4;}
                                  else if(sayac==10) {sayac=9;}
                                  else if(sayac<0) {sayac=0;}
                                }
                                else if(gelen==2 and sayac==4)
                                {
                                  toprak_nem+=5;                          
                                }
                                else if(gelen==2 and sayac==9)
                                {
                                  toprak_nem-=5; 
                                  if(toprak_nem<0) {toprak_nem=0;}                         
                                } 
                                else if(gelen==2 and sayac==11)
                                {
                                  guncelleniyor_ekrani();goto buradan;
                                }
                                else if(gelen==2 and sayac==0)
                                {
                                  goto buradan4;
                                }
                                if(toprak_nem<100 or toprak_nem<10)
                                {
                                  lcd.setCursor(8,1);lcd.print(F(" "));
                                  if(toprak_nem<10)
                                  {
                                    lcd.setCursor(7,1);lcd.print(F(" "));
                                  }
                                }  
                                lcd.setCursor(6,1);lcd.print(toprak_nem);                
                              }                   
                              lcd.setCursor(sayac,1); 
                              delay(1);                                                                      
                            }                                                       
                          }
                          if(sera_nem<100 or sera_nem<10)
                          {
                            lcd.setCursor(8,1);lcd.print(F(" "));
                            if(sera_nem<10)
                            {
                              lcd.setCursor(7,1);lcd.print(F(" "));
                            }
                          }
                          lcd.setCursor(6,1);lcd.print(sera_nem);
                        }
                        lcd.setCursor(sayac,1);
                        delay(1);         
                      }while(!bool(0));  
                    }
                    lcd.setCursor(5,1);lcd.print(sera_sicaklik);
                  }
                  delay(1);
                  lcd.setCursor(sayac,1);
                }             
             }
             else if(gelen==2 and sayac==8)
             {
               sayac=7;
               ana_sayfa();
               goto buradan2;
             }             
          }
          lcd.setCursor(sayac,1);
          delay(1);
        }while(true);
      }
      else if(gelen==2 and sayac==6)
      {
        buradan5:
        sayac=7;
        sensor_degerleri_ekrani();
        do
        {
          gelen=tus_takimi();
          if(gelen)
          {
            if(gelen==1)
            {
              sayac++;
              if(sayac==9) {sayac=15;}
              else if(sayac==1) {sayac=6;}
            }
            else if(gelen==3)
            {
              sayac--;
              if(sayac==5) {sayac=0;}
              else if(sayac==14) {sayac=8;}
            }
            else if(gelen==2  and sayac==8)
            {
              sayac=7;
              ana_sayfa();goto buradan2;
            }
            else if(gelen==2 and sayac==6)
            {
              while(!bool(0))
              {
                gelen=tus_takimi();
                if(gelen==2){say++;lcd.backlight();if(say>1){lcd.blink();say=0;goto buradan5;}}
                sensor_verileri();
                delay(1);
              }
            }
            else if(gelen==2 and sayac==15)
            {
              sayac=7;
              ana_sayfa();goto buradan2;
            }
            else if(gelen==2 and sayac==0)
            {
              buradan6:
              sayac=7;
              otomasyon_baslatma_ekrani();
              do
              {
                gelen=tus_takimi();
                if(gelen)
                {
                  if(gelen==1)
                  {
                    sayac++;
                    if(sayac==9) {sayac=15;}
                  }
                  else if(gelen==3)
                  {
                    sayac--;
                    if(sayac==5) {sayac=6;}
                    else if(sayac==14) {sayac=8;}
                  }
                  else if(gelen==2 and sayac==6)
                  {
                    lcd.noBlink();
                    while(!bool(0))
                    {
                      otomasyon_baslatma();sensor_verileri();
                      gelen=tus_takimi();
                      if(gelen)
                      {
                        if(gelen==2) {lcd.blink();goto buradan6;}
                      }
                    }
                  }
                  else if(gelen==2 and sayac==15)
                  {
                    goto buradan5;
                  }
                  else if(gelen==2 and sayac==8)
                  {
                    sayac=7;
                    ana_sayfa();goto buradan2;
                  }
                }
                lcd.setCursor(sayac,1);
              }while(!bool(0));
            }
          }
          lcd.setCursor(sayac,1);   
        }while(!bool(0));     
       }      
    }
    lcd.setCursor(sayac,1);
  }
}
      
int tus_takimi() {

  byte giden=0;
  if(!bool(0))
  {
    if(!digitalRead(pinler[7])!=0 and kapi1==false)
    {
      giden=1;
      kapi1=!kapi1;
    }
    else if(!digitalRead(pinler[7])==0 and kapi1==true)
    {
      kapi1=!kapi1;
    }
    if(!digitalRead(pinler[8])!=0 and kapi2==false)
    {
      giden=2;kapi2=!kapi2;
    }
    else if(!digitalRead(pinler[8])==0 and kapi2==true)
    {
      kapi2=!kapi2;
    }
    if(!digitalRead(pinler[9])!=0 and kapi3==false)
    {
      giden=3;kapi3=!kapi3;
    }
    else if(!digitalRead(pinler[9])==0 and kapi3==true)
    {
      kapi3=!kapi3;
    }
    delay(10);
  }
  return giden;
}
