/*  WallE  


CAN Nachrichten
    PowerUpSequence :  hohe Spannungspegel 
	=> kann ohne extra Elektronik nicht simuliert werden
    Joystick_Neutral_Position
	=> 3 Byte Nachricht wird alle 20ms verschickt
    Joystick_JustMovedFromNeutralPosition
	=> Eine Sequenz von ca. 10 verschiedenen Nachrichten
	=> diese Sequenz von Nachrichten können wir ggf. noch simulieren
    Joystick_Active_Position 
	=> 7 Byte Nachricht wird alle 20ms verschickt

TODO:

- CAN Msg print out chicker
- filter implement
- strategie fuer CAN Analyse?
- ggf.  Hohlstecker anlöten

Changes: 
    LIib "CAN"  in  1.5.7  
    geaendert fuer DXBus
    basiert auf  "ReceiveCANMessages.ino"

*/

/*   WallE  Testfunktionen

Hinweis:
- CAN-Nachrichten können aus SerialMonitor per CopyPaste in eine Excel-Liste  übertragen werden
- Falls überhaupt keine CAN-Nachrichten angzeigt werden, muss ggf. der Abschlusswiderstand geändert werden ( Jumper 1  auf dem CAN-Shield)


Test1: 
Schalter auf J (= Joystick)
Achtung:  (ich weiß nicht mehr, welche Farben die Bananenstecker am Kabel haben)
Kabel "PowerModul"  an PowerModul anschließen
Kabel "Joystick"  an Joystick anschließen
Can High=> Grüner Bananenstecker  an gelbes Kabel
Can Low => Gelber Bananenstecker  an Lila Kabel

Arduino ( RJ45 Stecker ) NICHT anschließen


Ergebnis? Funktioniert die manuelle Steuerung noch normal?


Test 2:
Arduino anschließen ( 12V Spannungsversorgung über Hohlstecker,  WallE und Arduino brauchen identischen GND)
Schalter auf J (= Joystick)
USB Kabel für Serial Port anschließen  ( KBaud?  => 115200)

Ergebnis:  Werden im Serial-Fenster Nachrichten angezeigt?
beim Aufstarten?
beim Stillstand?
wenn der Joystick nach vorne bewegt wird?


Test3:
Schalter auf J (= Joystick)
WallE einschaltetn
nach einiger Zeit,  Schalter auf A

Ergebnis:  welche Can-Nachrichten werden angezeigt?

 */

#include <Arduino.h>
#include <CAN.h>
#include <SPI.h>
#include <CAN_MCP2515.h>

#include <Metro.h> 
//~ #include <Bounce.h> 

#include "WallE_Can.h"


/*  CAN Shield Pins
__________________

|          T4  T3  T2  T1

|                   A0   A1
__________________

Taster:
T1: Pin3
T2: Pin5
T3: not connected
T4: [A2] => Pin 16

Potis:  an A0 und A1

*/

//~ Bounce taster1 = Bounce(3, 10);  // 10 ms debounce
//~ Bounce taster2 = Bounce(5, 10);  // 10 ms debounce
//~ Bounce taster4 = Bounce(16, 10);  // 10 ms debounce



void setup()
{
  Serial.begin(115200);  
  WallE_Can.begin();
    
  //~ pinMode(3, INPUT_PULLUP);  // fuer taster1
    
  delay(1000);  // Delay added just so we can have time to open up Serial Monitor and CAN bus monitor. It can be removed later...
  Serial.println("Start:  WallE ver 2015_06_29 ... ");
}



void loop()
{
    WallE_Can.update();
    

    if (WallE_Can._state == WALLE_CAN_STATE_AUTOMATIC )
    {
	 
        //~ WallE_Can.move( (direction_t) {FORWARD_FAST, NO_MOVEMENT});
        //~ WallE_Can.move( (direction_t) {NO_MOVEMENT, NO_MOVEMENT});
        
        char inChar = (char)Serial.read(); // nur ein Zeichen
        if(inChar == 'f') {
            //~ WallE_Can.move( (direction_t) {ForwardBackward, RightLeft});
            WallE_Can.move( (direction_t) {FORWARD_FAST, NO_MOVEMENT});
            Serial.println("Forward...");
        }
        if(inChar == 'b') {
            WallE_Can.move( (direction_t) {BACKWARD_FAST, NO_MOVEMENT});
            Serial.println("Backward...");
        }
        if(inChar == 's') {
            WallE_Can.move( (direction_t) {NO_MOVEMENT, NO_MOVEMENT});
            Serial.println("Stop...");
        }
        if(inChar == 'r') {
            WallE_Can.move( (direction_t) {NO_MOVEMENT, RIGHT_FAST});
            Serial.println("Right...");
        }
        if(inChar == 'l') {
            WallE_Can.move( (direction_t) {NO_MOVEMENT, LEFT_FAST});
            Serial.println("Left...");
        }
    } // end if
    
} // end loop







