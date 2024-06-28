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
  Créé le: 14/06/2024 
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
const int DET1J2 = A0;// Cde Exterieure par 4N35
const int DET2J3 = A1;// Cde Exterieure par 4N35
const int DET3J4 = A2;//        J4
const int DET4J4 = A3;//        ''
const int DET5J4 = A4;//        ''
const int DET6J4 = A5;//        ''
const int DET7J4 = A6;//        ''
const int POTARVOLUME = A7;// potentiometre volume par J7

int etatDET1J2 = 1023;
int etatDET2J3 = 1023;
int etatDET3J4 = 1023;
int etatDET4J4 = 1023;
int etatDET5J4 = 1023;
int etatDET6J4 = 1023;
int etatDET7J4 = 1023;

int etatPINBUSY = 0;
int etatPOTARVOLUME = 0;
int valVolume = 0;

const byte PINBUSY = 4;// TF.16P.V3.0
const byte LEDBUSY = 5;
const byte LEDDEFAUT = 6;
const byte LEDDET1J2 = 7;
const byte LEDDET2J3 = 8;

void setup()
{
  mySerial.begin(9600);
  Serial.begin(9600);
//-------------------------------------------- 
if (!myDFPlayer.begin(mySerial))
  {
    Serial.println(F("Pb communication : verifier SDcard"));
    digitalWrite(LEDDEFAUT, HIGH);
    while(true);
  }
//--------------------------------------------  
  myDFPlayer.setTimeOut(500);  
  myDFPlayer.volume(8);
  //myDFPlayer.play(1);
//--------------------------------------------
  pinMode(DET1J2, INPUT);
  pinMode(DET2J3, INPUT);
  pinMode(DET3J4, INPUT);
  pinMode(DET4J4, INPUT);
  pinMode(DET5J4, INPUT);
  pinMode(DET6J4, INPUT);
  pinMode(DET7J4, INPUT);
  pinMode(POTARVOLUME, INPUT);
  pinMode(PINBUSY, INPUT);  
  
  pinMode(LEDBUSY, OUTPUT);
  pinMode(LEDDEFAUT, OUTPUT);  
  pinMode(LEDDET1J2, OUTPUT);
  pinMode(LEDDET2J3, OUTPUT);
  delay(2000);
  myDFPlayer.play(8);//Joue son 8 à l'initialisation 
  delay(2000);
}//Fin SETUP

void loop()
{
  etatDET1J2 = analogRead(DET1J2);
  etatDET2J3 = analogRead(DET2J3);
  etatDET3J4 = analogRead(DET3J4);
  etatDET4J4 = analogRead(DET4J4);
  etatDET5J4 = analogRead(DET5J4);
  etatDET6J4 = analogRead(DET6J4);
  etatDET7J4 = analogRead(DET7J4);
  etatPOTARVOLUME = analogRead(POTARVOLUME);
  etatPINBUSY = digitalRead(PINBUSY);

Volume();// Appel fonction volume
//--------------------------------------------
  if(etatDET1J2 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(1);      
    }
//-------------------------------------------- 
  if(etatDET2J3 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(2);      
    }
//-------------------------------------------- 
  if(etatDET3J4 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(3);      
    }
//-------------------------------------------- 
  if(etatDET4J4 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(4);      
    }
//--------------------------------------------
  if(etatDET5J4 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(5);      
    }
//-------------------------------------------- 
  if(etatDET6J4 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(6);      
    }
//--------------------------------------------
  if(etatDET7J4 <= 512 && etatPINBUSY == 1)
    {
      myDFPlayer.play(7);      
    }
//--------------------------------------------
Signalisation();// Appel fonction Signalisation
//--------------------------------------------
Serial.print(etatDET1J2);
Serial.print("\t");
Serial.print(etatDET2J3);
Serial.print("\t");
Serial.print(etatDET3J4);
Serial.print("\t");
Serial.print(etatDET4J4);
Serial.print("\t");
Serial.print(etatDET5J4);
Serial.print("\t");
Serial.print(etatDET6J4);
Serial.print("\t");
Serial.print(etatDET7J4);
Serial.print("\t");
Serial.print("\t");
Serial.print(etatPOTARVOLUME);
Serial.print("\t");
Serial.print(valVolume); 
Serial.print("\t");
Serial.print(myDFPlayer.readVolume());//  Renvoi le volume du DFPlayer
Serial.print("\t");
Serial.print("\t");
Serial.print(etatPINBUSY);
Serial.println("\t");
}//FIN LOOP

void Signalisation()
{
  if(etatPINBUSY == 0)// Niveau bas en lecture, niveau haut en veille
    {
      digitalWrite(LEDBUSY,HIGH);
    }
  else
    {
      digitalWrite(LEDBUSY,LOW);
    }

  if (etatDET1J2 <512)  
    {
      digitalWrite(LEDDET1J2,HIGH);
    }
  else
    {
      digitalWrite(LEDDET1J2,LOW);  
    }
  if (etatDET2J3 <512)  
    {
      digitalWrite(LEDDET2J3,HIGH);
    }
  else
    {
      digitalWrite(LEDDET2J3,LOW);  
    }

}//FIN SIGNALISATION   

void Volume()
{
  valVolume = map(analogRead(A7),0,1023,0,30);
  myDFPlayer.volume(valVolume);
}//FIN VOLUME




