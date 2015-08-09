
#include <Arduino.h>
#include <CAN.h>
#include <SPI.h>
#include <CAN_MCP2515.h>

#include <Metro.h> 


//sh http://en.wikipedia.org/wiki/Struct_(C_programming_language)
typedef struct direction {
   int8_t ForwardBackward;
   int8_t RightLeft;
} direction_t;


// Werte sind geraten und muessen verifiziert werden
enum Speed {
    NO_MOVEMENT = 0,
    //~ RIGHT_SLOW = RIGHT_FAST/2,  // gibt es ggf. hier eine Kommazahl?
    RIGHT_SLOW = 0x01,
    RIGHT_FAST = 0x1A, 
    LEFT_SLOW = 0xFF,
    LEFT_FAST = 0xE6,
    
    RIGHTLEFT_MAX = 0x1A,
    RIGHTLEFT_MIN = 0xE6,
    
    FORWARD_SLOW = 0x01,
    FORWARD_FAST = 0x26, 
    BACKWARD_SLOW = 0xFF,
    BACKWARD_FAST = 0xE6,
    FORWARDBACKWARD_MAX = 0x26,
    FORWARDBACKWARD_MIN = 0xE6
};


enum WALLE_CAN_STATES {
    WALLE_CAN_STATE_INIT = 0,  // classe startet mit 0  
    WALLE_CAN_STATE_LISTEN_ONLY,
    WALLE_CAN_STATE_AUTOMATIC,
    WALLE_CAN_STATE_AUTOMATIC_START_SEQUENCE
    };

extern CAN_Frame Msg040_action;
extern CAN_Frame Msg3AC;
extern Metro Metro20;
    
//It is not possible to declare and use classes declared in one .pde file in another .pde file from within the Arduino IDE.
//One workaround is to make the second file into a C++ source file (.cpp) and then add a #include "<filename>" directive in the beginning of the first file.
class cWallE_Can
{
    public:
        
    const uint8_t _PinInputCanToggleSwitch = 4;  // CAN Umschalter ist auch an Pin4
    const uint8_t _PinLed = 8;
    direction_t  _direction;
    int _state;
    
    int _milliCounter;
    
    unsigned long _SerialMillisecondCounter;
    
    //~ cWallE_Can(){ };  //Konstruktor macht erstmal nichts

    void begin(void) { // variailen initialisieren und Pins setzen
        pinMode(_PinInputCanToggleSwitch, INPUT_PULLUP);
        pinMode(_PinLed, OUTPUT);
        CAN.begin(CAN_BPS_105K);

    } ;

    
    //~ void move(int8_t  directionForwardBackward, int8_t directionRightLeft) {
    void move(direction_t direct) {
        // range checken  und interne variablen setzen
        //~ if( direct.ForwardBackward > 0 && direct.ForwardBackward >  FORWARDBACKWARD_MAX) {  Serial.println(" Forward : Out of range ...");   };  // out of range
        //~ if( direct.ForwardBackward < 0 && direct.ForwardBackward <  FORWARDBACKWARD_MIN) {  Serial.println(" Back: Out of range ...");   };  // out of range
        //~ if( direct.RightLeft > 0 && direct.RightLeft >  RIGHTLEFT_MAX) {  Serial.println(" Right: Out of range ...");   };  // out of range
        //~ if( direct.RightLeft < 0 && direct.RightLeft <  RIGHTLEFT_MIN) {  Serial.println(" Left: Out of range ...");   };  // out of range
        _direction = direct;
    };
    
    void can_write(void) {  
        CAN_Frame message; // Create message object to use CAN message structure
        if (CAN.available() == true)  { // Check to see if a valid message has been received.
            
            message = CAN.read(); 
            print_CanMessage(message, false);
            //~ if(message.id == 0x040)   print_CanMessage(message, false);
            if(message.id == 0x008)   print_CanMessage(message, false);
            //~ if(message.id == 0x2B4)   print_CanMessage(message, false);
        
        print_CanMessage(message, false);
        }

        if (Metro20.check() == 1) {  // alle 20ms 
            Msg040_action.data[4] = _direction.ForwardBackward ; 
            Msg040_action.data[6] = _direction.RightLeft ; 
            CAN.write(Msg040_action);
            //~ print_CanMessage(Msg040_action, true);
        }
    };
    
    void can_listen(void) {  
        CAN_Frame message; // Create message object to use CAN message structure
        if (CAN.available() == true)  { // Check to see if a valid message has been received.
            message = CAN.read(); 
            if(message.id == 0x040)   print_CanMessage(message, false);
            //~ if(message.id == 0x008)   print_CanMessage(message, false);
            //~ if(message.id == 0x2B4)   print_CanMessage(message, false);
        }
    };
    
    void update(void) {  
        // listen to can messages or  simulate joystick messages
        if(digitalRead(_PinInputCanToggleSwitch)==LOW) {  // listen only
            if( _state != WALLE_CAN_STATE_LISTEN_ONLY ) {  // transition
                Serial.println("WALLE_CAN_STATE_LISTEN_ONLY");
                digitalWrite(_PinLed, HIGH);
            }
            _state = WALLE_CAN_STATE_LISTEN_ONLY;
            can_listen();
        }
        else {  //simulate can messages
            if( _state != WALLE_CAN_STATE_AUTOMATIC)  {  // transition
                Serial.println("WALLE_CAN_STATE_AUTOMATIC"); 
                digitalWrite(_PinLed, LOW);
            }
            // assume WallE is moving
            _state = WALLE_CAN_STATE_AUTOMATIC;
            can_write();
        }
    };  // end update()
    
    void print_CanMessage(CAN_Frame  &msg, bool isOutMessage);
    CAN_Frame createRemoteRequestCanMessage(uint32_t ID);
    CAN_Frame createStandardCanMessage(uint32_t ID, uint8_t length,uint8_t * data);

};

extern cWallE_Can WallE_Can;
