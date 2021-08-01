#include "config.h"
#include "bits.h"
#include "lcd.h"
#include "keypad.h"
#include "delay.h"
#include "musicas.h"
#include "ssd.h"
#include"config.h"
#include <pic18f4520.h>
#include"io.h"
#include"pwm.h"



void main(void) {
    
    
   
    char text6[8]="  UNIFEI";
    char text7[10]="Joao Pedro   ";
    char text8[16]="      2018004282";
    
    
    
  
    
    
    iniciaMusica();
    lcdInit();
    ssdInit();
    kpInit();
    kpDebounce();
    lcdCommand(ON);
    
    
    
    
  
    
    
    char logo[48] = {
        0x01, 0x03, 0x03, 0x0E, 0x1C, 0x18, 0x08, 0x08,
        0x11, 0x1F, 0x00, 0x01, 0x1F, 0x12, 0x14, 0x1F,
        0x10, 0x18, 0x18, 0x0E, 0x07, 0x03, 0x02, 0x02,
        0x08, 0x18, 0x1C, 0x0E, 0x03, 0x03, 0x01, 0x00,
        0x12, 0x14, 0x1F, 0x08, 0x00, 0x1F, 0x11, 0x00,
        0x02, 0x03, 0x07, 0x0E, 0x18, 0x18, 0x10, 0x00,
    };
    
    
              
    
    
    
 
    lcdCommand(CLR);
    lcdPosition(0,3);
    for(char i=0;i<8;i++){
    lcdData(text6[i]);}
    lcdPosition(1,3);
    for(char i=0;i<10;i++){
    lcdData(text7[i]);}
    lcdPosition(2,4);
    for(char i=0;i<16;i++){
    lcdData(text8[i]);}
    lcdPosition(0,0); 
    lcdData(0);
    lcdData(1);
    lcdData(2);
    lcdPosition(1,0);
    lcdData(3);
    lcdData(4);
    lcdData(5);
    lcdCommand(0x40);
       for(char i=0;i<48;i++){
          lcdData(logo[i]);}
    atraso_ms(4000);
    lcdCommand(CLR);
    
    
    
    
    
    lcdPosition(0, 6);
    lcdStr("MP3");
    lcdPosition(1, 4);
    lcdStr("Player");
    lcdPosition(0,0); 
                lcdData(0);
                lcdData(1);
                lcdData(2);
                lcdPosition(1,0);
                lcdData(3);
                lcdData(4);
                lcdData(5);
                lcdCommand(0x40);
                for(char i=0;i<48;i++){
                lcdData(logo[i]);
                }
    atraso_ms(5000);
    
    lcdCommand(CLR);
    lcdPosition(0, 0);
    lcdStr("Escolha a musica");
    for (;;) {
        lcdPosition(1, 0);
        lcdStr("<-(1) (*)  (2)->");
        escolheMusica();
    }
}