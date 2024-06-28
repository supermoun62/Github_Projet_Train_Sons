/*
 Description: Programme V1 permettant la diffusion d'un son avec un DFPlayerMini 
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

const byte RXPIN = 2;
const byte TXPIN = 3;
SoftwareSerial mySerial(RXPIN, TXPIN);

DFRobotDFPlayerMini myDFPlayer;

const byte PINBUSY = 4;
const byte LEDBUSY = 5;
byte etatPinBusy= 0;
void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600); 
  
  if (!myDFPlayer.begin(mySerial))
    {
      Serial.println(F("Pb communication : verifier SDcard"));
      while(true);
    }
  
  myDFPlayer.setTimeOut(500);  
  myDFPlayer.volume(5);
  myDFPlayer.play(1);

  pinMode(PINBUSY, INPUT);
  pinMode(LEDBUSY, OUTPUT);
  delay(10000);
}

void loop()
{
  etatPinBusy = digitalRead(PINBUSY);
  
  if(etatPinBusy == 1)
    {
      lecture();
    }

  if(etatPinBusy == 0)// Niveau bas en lecture, niveau haut en veille
    {
      digitalWrite(LEDBUSY,HIGH);
    }
  else
    {
      digitalWrite(LEDBUSY,LOW);
    }
}

void lecture()
{
  myDFPlayer.next();
  delay(500);
}

