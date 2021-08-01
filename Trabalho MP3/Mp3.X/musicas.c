#include "musicas.h"
#include <string.h>
#include <pic18f4520.h>
#include "lcd.h"
#include "bits.h"
#include "keypad.h"
#include "ssd.h"
#include "delay.h"
#include "pwm.h"

typedef struct {
    unsigned int duracao;
    unsigned char nome[17];
} musica;

unsigned char nomes[10][17] = {"Paranoid", "Highway Star", "Born To Be Wild", "Bad To The Bone", "Down The Road", "Wonderwall", "Blue Monday",
    "Back In Black", "Hey Jude", "Kashmir"};
unsigned int duracoes[10] = {16, 15, 170, 45, 8, 10, 80, 130, 11, 12};
musica musicas[10];

unsigned char tecla = 16, indice = 0, flag = 0, tempo, minuto1, minuto2, segundo1, segundo2, cnt = 0, pause = 1, volume = 3;

void iniciaMusica(void) {
    TRISC = 0x00;
    for (unsigned int i = 0; i < 10; i++) {
        musicas[i].duracao = duracoes[i];
        strcpy(musicas[i].nome, (char*) nomes[i]);
    }
    return;
}

void escolheMusica(void) {
    kpDebounce();
    tecla = kpRead();
    ssdUpdate();
    if (bitTst(tecla, 3) || bitTst(tecla, 7)) { //Tecla *
        flag = 1;
        for (;;) {
            ssdUpdate();
            kpDebounce();
            atraso_ms(10);
            if ((kpRead() != tecla) || flag == 1) {
                tecla = kpRead();
                if (bitTst(tecla, 3)) { //1
                    if (indice == 0) {
                        indice = 9;
                    } else {
                        indice -= 1;
                    }
                } else if (bitTst(tecla, 7)) { //2
                    if (indice == 9) {
                        indice = 0;
                    } else {
                        indice += 1;
                    }
                } else if (bitTst(tecla, 0)) { //*
                    flag = 0;
                    break;
                }
                lcdCommand(CLR);
                lcdPosition(1, 0);
                lcdStr("<-(1) (*)  (2)->");
                lcdPosition(0, 0);
                lcdStr(musicas[indice].nome);
                ssdDigit(indice, 3);
                flag = 0;
            }
        }
        tocaMusica();
    }
}

void tocaMusica() {
    pwmInit();
    lcdCommand(CLR);
    lcdPosition(0, 0);
    lcdStr(musicas[indice].nome);
    lcdPosition(1, 0);
    lcdStr("-(1)  (*)  (2)+");

    tempo = musicas[indice].duracao;
    pwmSet(100);
    while (tempo != 0) {
        
        minuto1 = (tempo / 60) % 10;
        minuto2 = (tempo / 60) / 10;
        segundo1 = (tempo % 60) % 10;
        segundo2 = (tempo % 60) / 10;

        ssdDigit(minuto2, 0);
        ssdDigit(minuto1, 1);
        ssdDigit(segundo2, 2);
        ssdDigit(segundo1, 3);

        for (unsigned char j = 0; j < 100; j++) {
            ssdUpdate();
            atraso_ms(10);
            kpDebounce();
            tecla = kpRead();
            if (bitTst(tecla, 3)) {
                while(bitTst(tecla, 3)) {
                    ssdUpdate();
                    kpDebounce();
                    tecla = kpRead();
                }
                alterarVolume(0);
            }
            else if (bitTst(tecla, 7)) {
                while(bitTst(tecla, 7)) {
                    ssdUpdate();
                    kpDebounce();
                    tecla = kpRead();
                }
                alterarVolume(1);
            }
            else if (bitTst(tecla, 0)) {
                while(bitTst(tecla, 0)) {
                    ssdUpdate();
                    kpDebounce();
                    tecla = kpRead();
                }
                if (pause == 0) {pause = 1;} else {pause = 0;}
            } 
            else if (bitTst(tecla, 4)) {
                while(bitTst(tecla, 4)) {
                    ssdUpdate();
                    kpDebounce();
                    tecla = kpRead();
                }
                ssdDigit(0, 0);
                ssdDigit(0, 1);
                ssdDigit(0, 2);
                ssdDigit(0, 3);
                return;
            }
        }
        if (pause == 0) {
            tempo -= 1;
            pwmSet(100);
        } else {
            pwmSet(0);
        }
    }
    TRISA=0x00;
    pwmSet(0);
    bitSet(TRISC, 1);
    atraso_ms(500);
    bitClr(TRISC, 1);
    return;
}

void alterarVolume(char opt) {
    if (opt == 1) {
        if(volume!=8){volume += 1;}
    } else {
        if(volume!=0){volume -= 1;}
    }
    unsigned char old_D, old_A;
    old_D = TRISD;
    
    PORTA=0x00;
    TRISD = 0x00;
    
    if (volume == 0) {
        PORTD = 0b00000000;
    } else if (volume == 1) {
        PORTD = 0b10000000;
    } else if (volume == 2) {
        PORTD = 0b11000000;
    } else if (volume == 3) {
        PORTD = 0b11100000;
    } else if (volume == 4) {
        PORTD = 0b11110000;
    } else if (volume == 5) {
        PORTD = 0b11111000;
    } else if (volume == 6) {
        PORTD = 0b11111100;
    } else if (volume == 7) {
        PORTD = 0b11111110;
    } else if (volume == 8) {
        PORTD = 0b11111111;
    }
    atraso_ms(500);
    TRISD=old_D;
}
