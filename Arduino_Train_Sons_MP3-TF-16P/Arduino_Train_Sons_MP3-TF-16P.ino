/***************************************************************************************************
Synopsis:   Commande sons avec carte MP3-TF-16P
Carte:      Arduino Nano
Capteur:    Capteurs ILS  
             
Biblio:     SoftwareSerial.h
            DFPlayer.h
             
Source:               https://github.com/enjoyneering/DFPlayer

                      ESP8266 Core     -https://github.com/esp8266/Arduino
                      EspSoftwareSerial-https://github.com/plerup/espsoftwareserial

Librairie DFPlayer.h  https://github.com/enjoyneering/DFPlayer 

Informations sites:    https://lasonotheque.org/dossier-62-fabriquer-boite-sons-arduino-dfplayer.html

                       https://docs.arduino.cc/learn/built-in-libraries/software-serial/

                       https://idehack.com/dfplayer-mp3-tf-16p-et-arduino/
                        
                       https://lasonotheque.org/fabriquer-boite-sons-arduino-dfplayer_f62.html

                       https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299#Connection_Diagram

                       https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299

                       https://www.wikidebrouillard.org/wiki/Item:DFPlayer_Lecteur_MP3
          
Pssibilté de piloter un son avec la carte Projet_Passage_A_niveau_Stockage           
Auteur:  Christian Mercier  
  Créé le: 11/08/2024 
  62128 Boiry-Becquerelle France           
  https://electroniquepassion.fr/ 
  C.Mercier  62128 Boiry-Becquerelle  

***************************************************************************************************
   This is an Arduino sketch for DFPlayer Mini MP3 module
   written by : enjoyneering
   source code: https://github.com/enjoyneering/DFPlayer

   DFPlayer Mini features:
   - 3.2v..5.0v, typical 4.2v
   - 15mA without flash drive, typical 24mA
   - 24-bit DAC with 90dB output dynamic range and SNR over 85dB
   - micro SD-card, up to 32GB (FAT16, FAT32)
   - USB-Disk up to 32GB (FAT16, FAT32)
   - supports mp3 sampling rate 8KHz, 11.025KHz, 12KHz, 16KHz, 22.05KHz, 24KHz, 32KHz, 44.1KHz, 48KHz
   - supports up to 100 folders, each folder can be assigned to 001..255 songs
   - built-in 3W mono amplifier, NS8002 AB-Class with standby function
   - UART to communicate, 9600bps (parity:none, data bits:8, stop bits:1, flow control:none)

   NOTE:
   - if you hear a loud noise, add a 1K resistor in series with DFPlayer TX pin
   - move the jumper from right to left to automatically switch the amplifier to standby

   Frameworks & Libraries:
   ESP8266 Core      -  https://github.com/esp8266/Arduino
   EspSoftwareSerial -  https://github.com/plerup/espsoftwareserial


   GNU GPL license, all text above must be included in any redistribution,
   see link for details  - https://www.gnu.org/licenses/licenses.html

   Librairie DFPlayer.h  https://github.com/enjoyneering/DFPlayer  
***************************************************************************************************
    NOTE:
    - moduleType:
      - DFPLAYER_MINI:
        - DFPlayer Mini module
        - MP3-TF-16P module
        - FN-M16P module
        - YX5200 chip
        - YX5300 chip
        - JL AAxxxx chip
      - DFPLAYER_FN_X10P:
        - FN-M10P module
        - FN-S10P module
        - FN6100 chip
      - DFPLAYER_HW_247A:
        - HW-247A module
        - GD3200B chip
      - DFPLAYER_NO_CHECKSUM:
        - no checksum calculation (not recomended for MCU without external
          crystal oscillator)
    MOTS CLES:
        https://github.com/enjoyneering/DFPlayer/blob/main/keywords.txt
***************************************************************************************************/
#include <SoftwareSerial.h>
#include <DFPlayer.h>
#define MP3_RX_PIN                    2     //GPIO4/D2 to DFPlayer Mini TX
#define MP3_TX_PIN                    3     //GPIO5/D1 to DFPlayer Mini RX
#define myDFPlayer_SERIAL_SPEED       9600  //DFPlayer Mini suport only 9600-baud
#define myDFPlayer_SERIAL_BUFFER_SIZE 32    /*software serial buffer size in bytes, to send 8-bytes you need
                                            11-bytes buffer (start byte+8-data bytes+parity-byte+stop-byte=11-bytes)*/
#define myDFPlayer_SERIAL_TIMEOUT     350   //average DFPlayer response timeout for GD3200B chip 350msec..500msec
SoftwareSerial myDFPlayerSerial(MP3_RX_PIN,MP3_TX_PIN);//RX, TX
DFPlayer myDFPlayer;

  const int SON_1 = 1;
  const int SON_2 = 2;
  const int SON_3 = 3;
  const int SON_4 = 4;
  const int SON_5 = 5;
  const int SON_6 = 6;
  const int SON_7 = 7;
  const int SON_8 = 8;
  const int SON_9 = 9;
  const int SON_10 = 10;
  const int SON_11 = 11;
  const int SON_12 = 12;
  const int SON_13 = 13;
  const int SON_14 = 14;
  const int SON_15 = 15;
  const int SON_16 = 16;
  const int SON_17 = 17;
  const int SON_18 = 18;
  const int SON_19 = 19;
  const int SON_20 = 20;

  int etatA0 = 0;
  int etatA1 = 0;
  int etatA2 = 0;
  int etatA3 = 0;
  int etatA4 = 0;
  int etatA5 = 0;
  int etatA6 = 0;
  int etatA7= 0;
  int valVolume = 0; 
  const byte BUSY = 4;// Information TF-16P-V3
  byte etatBusy = 0;
  bool memoireEntreA1 = 0;

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

  unsigned long tempsCourant = 0;
//------------------------------------------
void setup()
{
  Serial.begin(115200);  
  myDFPlayerSerial.begin(9600);
  myDFPlayer.begin(myDFPlayerSerial, myDFPlayer_SERIAL_TIMEOUT, DFPLAYER_HW_247A, false);
                                                 //"DFPLAYER_HW_247A" see NOTE, false=no feedback from module after the command
  myDFPlayer.stop();                             //if player was runing during ESP8266 reboot
  myDFPlayer.reset();                            //reset all setting to default
  myDFPlayer.setSource(2);                       //1=USB-Disk, 2=TF-Card, 3=Aux, 4=Sleep, 5=NOR Flash
  myDFPlayer.setEQ(0);                           //0=Off, 1=Pop, 2=Rock, 3=Jazz, 4=Classic, 5=Bass
  myDFPlayer.setVolume(25);                      //0..30, module persists volume on power failure
  myDFPlayer.sleep();                            //inter sleep mode, 24mA
  myDFPlayer.wakeup(2);                          //exit sleep mode & initialize source 1=USB-Disk, 2=TF-Card, 3=Aux, 5=NOR Flash 
  Serial.println(myDFPlayer.getStatus());        //0=stop, 1=playing, 2=pause, 3=sleep or standby, 4=communication error, 5=unknown state
  Serial.println(myDFPlayer.getVolume());        //0..30
  Serial.println(myDFPlayer.getCommandStatus()); //1=module busy, 2=module sleep, 3=request not fully received, 4=checksum not match
                                                 //5=requested folder/track out of range, 6=requested folder/track not found
                                                 //7=advert available while track is playing, 8=SD card not found, 9=???, 10=module sleep
                                                 //11=OK command accepted, 12=OK playback completed, 13=OK module ready after reboot 
  pinMode(LED5, OUTPUT); 
  pinMode(LED6, OUTPUT); 
  pinMode(LED7, OUTPUT);
  pinMode(LED13, OUTPUT);     

  myDFPlayer.playTrack(SON_14);  
}//FIN SETUP  
//------------------------------------------
void loop()
{
  etatA0 = analogRead(A0);
  etatA1 = analogRead(A1);
  etatA2 = analogRead(A2);
  etatA3 = analogRead(A3);
  etatA4 = analogRead(A4);
  etatA5 = analogRead(A5);
  etatA6 = analogRead(A6);
  etatA7= analogRead(A7);// potentiometre volume
  etatBusy = digitalRead(BUSY);    
  valVolume = map(analogRead(A7),0,1023,0,30); 
/******************************
  Commandes sons avec Entrees
*******************************/
  if(etatA0 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);        
        myDFPlayer.playTrack(SON_9);        
    }
  if(etatA1 <=512 && etatBusy == 1)
    {        
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);
        myDFPlayer.playTrack(SON_1);        
    }
  if(etatA2 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);        
        myDFPlayer.playTrack(SON_2);        
    }
  if(etatA3 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);        
        myDFPlayer.playTrack(SON_3);        
    }
  if(etatA4 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);       
        myDFPlayer.playTrack(SON_4);       
    }
  if(etatA5 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);        
        myDFPlayer.playTrack(SON_5);        
    }
  if(etatA6 <=512 && etatBusy == 1)
    {
        myDFPlayer.setTimeout(350);
        delay(40);
        myDFPlayer.setVolume(valVolume);        
        myDFPlayer.playTrack(SON_6);        
    }
/******************************
  Commandes sons avec interval
*******************************/
    if((millis() - tempsPrecedentSon1) >= 480000 && (bitSon1 == 1))//8 minutes (en ms)
      {
        tempsPrecedentSon1 = millis(); 
        tempsPrecedentSon2 = millis();       
        bitSon1 = 0;
        bitSon2 = 1; 
        compteur1 = compteur1 + 1;
        myDFPlayer.setTimeout(500);
        delay(40);
        myDFPlayer.setVolume(valVolume);
        myDFPlayer.playTrack(SON_12);
      }
    if((millis() - tempsPrecedentSon2) >= 120000 && (bitSon2 == 1))//2 minutes (en ms)
      {
        tempsPrecedentSon2 = millis();
        tempsPrecedentSon1 = millis();        
        bitSon1 = 1;
        bitSon2 = 0; 
        compteur2 = compteur2 + 1;
        myDFPlayer.setTimeout(500);
        delay(40);
        myDFPlayer.setVolume(valVolume);
        myDFPlayer.playTrack(SON_13);
      }        
Signalisations();// Appel Signalisation
/******************************
         Reset TF-16P
*******************************/
    if(etatBusy == 1)
      {
        tempsPrecedentReset = millis();
      }
    if(etatBusy == 0)
      {
        if((millis() - tempsPrecedentReset) >= 130000 )// 130 secondes en ms
          {
            tempsPrecedentReset = millis();
            myDFPlayer.reset();
            digitalWrite(LED13, HIGH);
            delay(1000);
            digitalWrite(LED13, LOW);
          }
      }  
    Serial.print("A0:");
    Serial.print(etatA0);
    Serial.print("\t");
    Serial.print("A1:");    
    Serial.print(etatA1);
    Serial.print("\t");
    Serial.print("A2:");
    Serial.print(etatA2);
    Serial.print("\t");
    Serial.print("A3:");
    Serial.print(etatA3);
    Serial.print("\t");
    Serial.print("A4:");
    Serial.print(etatA4);
    Serial.print("\t");
    Serial.print("A5:");
    Serial.print(etatA5);
    Serial.print("\t");
    Serial.print("A6:");
    Serial.print(etatA6);
    Serial.print("\t");
    Serial.print("Potar: ");    
    Serial.print(etatA7);
    Serial.print("\t");
    Serial.print("Busy: ");
    Serial.print(etatBusy);
    Serial.print("\t");
    Serial.print("bitSon1: ");  
    Serial.print(bitSon1);
    Serial.print("\t");
    Serial.print("bitSon2: ");   
    Serial.print(bitSon2);
    Serial.print("\t");
    Serial.print("C1: ");   
    Serial.print(compteur1);    
    Serial.print("\t");
    Serial.print("C2: ");    
    Serial.println(compteur2);
}//FIN LOOP
//------------------------------------------
void Signalisations()
  {
    if (etatA0 < 512 || etatA1 < 512 || etatA2 < 512 || etatA3 < 512 || etatA4 < 512 || etatA5 < 512 || etatA6 < 512)
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
