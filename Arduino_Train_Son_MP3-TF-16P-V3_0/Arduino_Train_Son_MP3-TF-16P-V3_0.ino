/* Description: Programme V10 permettant la diffusion d'un son avec un DFPlayerMini 
  Carte   : Arduino Nano
  DFPlayerMini (Mp3 TF 16P V3.0)

 Librairies utilisées:  DFRobotDFPlayerMini.h 
                        https://github.com/DFRobot/DFRobotDFPlayerMini 
                        SoftwareSerial.h
                        https://docs.arduino.cc/learn/built-in-libraries/software-serial/
                     
 Informations sites:    https://lasonotheque.org/dossier-62-fabriquer-boite-sons-arduino-dfplayer.html

                        https://docs.arduino.cc/learn/built-in-libraries/software-serial/

                        https://idehack.com/dfplayer-mp3-tf-16p-et-arduino/
                        
                        https://lasonotheque.org/fabriquer-boite-sons-arduino-dfplayer_f62.html

                        https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#Connection_Diagram

                        https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299

                        https://www.wikidebrouillard.org/wiki/Item:DFPlayer_Lecteur_MP3
  
  Auteur:  Christian Mercier  
  Créé le: 11/08/2024 
  62128 Boiry-Becquerelle France                                        
*/
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
// Set up a new SoftwareSerial object
SoftwareSerial mySoftwareSerial (2,3);//rxPin, txPin
//*****************************************************
// Create the Player object
DFRobotDFPlayerMini myDFPlayer;
//*****************************************************
const int E0 = 0;
int etatE0 = 0;
const int E1 = 1;
int etatE1 = 0;
const int E2 = 2;
int etatE2 = 0;
const int E3 = 3;
int etatE3 = 0;
const int E4 = 4;
int etatE4 = 0;
const int E5 = 5;
int etatE5 = 0;
const int E6 = 6;
int etatE6 = 0;
const int E7 = 7;
int etatE7 = 0;
int valVolume = 0;
const byte BUSY = 4;// Information TF-16P-V3
byte etatBusy = 0;

const byte LED5 = 5;//Busy
const byte LED6 = 6;//defaut
const byte LED7 = 7;//Entrees
const byte LED13 = 13;// Info reset TF-16P-V3

unsigned long tempsPrecedentReset = 0;
unsigned long tempsPrecedentSon1 = millis();
unsigned long tempsPrecedentSon2 = 0;

bool bitSon1 = 1;
bool bitSon2 = 0;
unsigned long compteur1 = 0;
unsigned long compteur2 = 0;
//------------------------------------------
void setup()
  {
    Serial.begin(115200);
    delay(500);
//*****************************************************
    // Define pin modes for TX and RX
    //pinMode(rxPin, INPUT);
    //pinMode(txPin, OUTPUT);
    // Set the baud rate for the SoftwareSerial object
    mySoftwareSerial.begin(9600);
    myDFPlayer.begin(mySoftwareSerial) ;
    delay(500);
//*****************************************************
    pinMode(LED5, OUTPUT); 
    pinMode(LED6, OUTPUT); 
    pinMode(LED7, OUTPUT);
    pinMode(LED13, OUTPUT);
//*****************************************************
   if (!myDFPlayer.begin(mySoftwareSerial)) //Utilisation de  softwareSerial pour communiquer
    {  
      Serial.println(F("Pb communication:"));
      Serial.println(F("1.SVP verifier connexion serie!"));
      Serial.println(F("2.SVP verifier SDcard !"));
      while(true)
        {
          digitalWrite(LED6, HIGH);  
        }
    }
    Serial.println(F("DFPlayer Mini En ligne."));

//*****************************************************    
    myDFPlayer.setTimeOut(500) ;
    delay(40);
    myDFPlayer.volume(20);// Volume à la mise sous tension  
    myDFPlayer.play(7);// Son à la mise sous tension       
  }//FIN SETUP  
//------------------------------------------
void loop()
  {
    etatE0 = analogRead(E0);
    etatE1 = analogRead(E1);
    etatE2 = analogRead(E2);
    etatE3 = analogRead(E3);
    etatE4 = analogRead(E4);
    etatE5 = analogRead(E5);
    etatE6 = analogRead(E6);
    etatE7 = analogRead(E7);// potentiometre volume
    etatBusy = digitalRead(BUSY);    
    valVolume = map(analogRead(A7),0,1023,0,30); 
    //myDFPlayer.volume(valVolume);  

    if(etatE0 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(3);        
      }
    if(etatE1 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(1);        
      }
    if(etatE2 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(12);        
      }
    if(etatE3 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(4);        
      }
    if(etatE4 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(5);       
      }
    if(etatE5 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(11);        
      }
    if(etatE6 <=512 && etatBusy == 1)
      {
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(13);        
      }
/******************************
  Commandes sons avec interval
*******************************/
    if((millis() - tempsPrecedentSon1) >= 90000 && (bitSon1 == 1))
      {
        tempsPrecedentSon1 = millis(); 
        tempsPrecedentSon2 = millis();       
        bitSon1 = 0;
        bitSon2 = 1; 
        compteur1 = compteur1 + 1;
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(14);// angelus
      }
    if((millis() - tempsPrecedentSon2) >= 20000 && (bitSon2 == 1))
      {
        tempsPrecedentSon2 = millis();
        tempsPrecedentSon1 = millis();        
        bitSon1 = 1;
        bitSon2 = 0; 
        compteur2 = compteur2 + 1;
        myDFPlayer.setTimeOut(500);
        delay(40);
        myDFPlayer.volume(valVolume);
        myDFPlayer.play(8);// cloche
      }
/******************************
         Reset TF-16P
*******************************/
    if(etatBusy == 1)
      {
        tempsPrecedentReset = millis();
      }
    if(etatBusy == 0)
      {
        if((millis() - tempsPrecedentReset) >= 40000 )// en ms
          {
            tempsPrecedentReset = millis();
            myDFPlayer.reset();
            digitalWrite(LED13, HIGH);
            delay(1000);
            digitalWrite(LED13, LOW);
          }
      }
/******************************
         APPEL FONCTIONS
*******************************/
    Signalisations();
//*****************************************************  
    Serial.print(etatE0);
    Serial.print("\t");
    Serial.print(etatE1);
    Serial.print("\t");
    Serial.print(etatE2);
    Serial.print("\t");
    Serial.print(etatE3);
    Serial.print("\t");
    Serial.print(etatE4);
    Serial.print("\t");
    Serial.print(etatE5);
    Serial.print("\t");
    Serial.print(etatE6);
    Serial.print("\t");    
    Serial.print(etatE7);
    Serial.print("\t");
    Serial.print("Busy");
    Serial.print("\t");
    Serial.print(etatBusy);
    Serial.print("\t");
    Serial.print("bitSon1");
    Serial.print("\t");
    Serial.print(bitSon1);
    Serial.print("\t");
    Serial.print("bitSon2");
    Serial.print("\t");
    Serial.print(bitSon2);
    Serial.print("\t");
    Serial.print("C1");
    Serial.print("\t");
    Serial.print(compteur1);    
    Serial.print("\t");
    Serial.print("C2");
    Serial.print("\t");
    Serial.println(compteur2);
  }//FIN LOOP
//------------------------------------------  
void Signalisations()
  {
    if (etatE0 < 512 || etatE1 < 512 || etatE2 < 512 || etatE3 < 512 || etatE4 < 512 || etatE5 < 512 || etatE6 < 512)
      {
        digitalWrite(LED7, HIGH);
      }      
    else
      {
        digitalWrite(LED7, LOW);
      }

    if(etatBusy == 0) 
      {
        digitalWrite(LED5, HIGH);
      } 
    else
      {
        digitalWrite(LED5, LOW);
      }     
  }// FIN FONCTION SIGNALISATION 

