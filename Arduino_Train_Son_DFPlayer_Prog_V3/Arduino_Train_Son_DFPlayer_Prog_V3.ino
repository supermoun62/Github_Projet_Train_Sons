/*
 Description: Programme V2 permettant la diffusion d'un son avec un DFPlayerMini 
  Carte   : Arduino Nano
  DFPlayerMini (Mp3 TF 16P V3.0)

Librairies utilisées:DFRobotDFPlayerMini.h 
                        https://github.com/DFRobot/DFRobotDFPlayerMini 
                        SoftwareSerial.h
                     
 Informations sites:    https://lasonotheque.org/dossier-62-fabriquer-boite-sons-arduino-dfplayer.html

                        https://docs.arduino.cc/learn/built-in-libraries/software-serial/

                        https://idehack.com/dfplayer-mp3-tf-16p-et-arduino/
                        
                        https://lasonotheque.org/fabriquer-boite-sons-arduino-dfplayer_f62.html

                        https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#Connection_Diagram

                        https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299

                        https://www.wikidebrouillard.org/wiki/Item:DFPlayer_Lecteur_MP3 
  
  Auteur:  Christian Mercier  
  Créé le: 03/03/2024 
  62128 Boiry-Becquerelle France                                        
*/
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
//--------------------------------------------
const byte RXPIN = 2;
const byte TXPIN = 3;
SoftwareSerial mySerial(RXPIN, TXPIN);
DFRobotDFPlayerMini myDFPlayer;
//--------------------------------------------
const int DET_0 = A0;
const int DET_1 = A1;

int etatDET_0 = 1023;
int etatDET_1 = 1023;

const byte PIN_BUSY = 4;
const byte LED_BUSY = 5;
const byte LED_DEFAUT = 6;

byte etatPIN_BUSY= 0;
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
//-------------------------------------------- 
if (!myDFPlayer.begin(mySerial))
  {
    Serial.println(F("Pb communication : verifier SDcard"));
    digitalWrite(LED_DEFAUT, HIGH);// Si pas de carte 
    while(true);
  }
//--------------------------------------------  
  myDFPlayer.setTimeOut(500);  
  myDFPlayer.volume(5);
//--------------------------------------------
  pinMode(DET_0, INPUT);
  pinMode(DET_1, INPUT);
  pinMode(PIN_BUSY, INPUT);
  pinMode(LED_BUSY, OUTPUT);  
  pinMode(LED_DEFAUT, OUTPUT);
  delay(2000);
}//Fin SETUP

void loop()
{
  etatDET_0 = analogRead(DET_0);
  etatDET_1 = analogRead(DET_1);
  etatPIN_BUSY = digitalRead(PIN_BUSY);
//--------------------------------------------
  if(etatDET_0 <= 512 )
    {
      myDFPlayer.play(1);      
    }
//-------------------------------------------- 
  if(etatDET_1 <= 512 )
    {
      myDFPlayer.play(2);      
    }
//--------------------------------------------
  if(etatPIN_BUSY == 0)// Niveau bas en lecture, niveau haut en veille
    {
      digitalWrite(LED_BUSY,HIGH);
    }
  else
    {
      digitalWrite(LED_BUSY,LOW);
    }
//--------------------------------------------
Serial.print(etatDET_0);
Serial.print("\t");
Serial.print(etatDET_1);
Serial.print("\t");
Serial.print("\t");
Serial.print(etatPIN_BUSY);
Serial.print("\t");
Serial.print("\t");
Serial.println(LED_DEFAUT);
}//FIN LOOP