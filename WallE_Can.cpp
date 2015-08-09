

#include "WallE_Can.h"

cWallE_Can WallE_Can; // Create WallE_Can Object


Metro Metro10 = Metro(10); 
Metro Metro20 = Metro(20); 
Metro Metro120 = Metro(120); 
Metro Metro200 = Metro(2000); 


CAN_Frame Msg008 =  WallE_Can.createStandardCanMessage(0x008,7, (uint8_t[]) {0xB0,0x01,0x25,0x07,0x00,0x0F,0xFF} );
CAN_Frame Msg040_action =  WallE_Can.createStandardCanMessage(0x040,7, (uint8_t[]) {0xB0,0x01,0x25,0x02,0x26,0x03,0x00} );  //B0 01 25 02 XX 03 XX 
CAN_Frame Msg040_noaction =  WallE_Can.createStandardCanMessage(0x040,3, (uint8_t[]) {0xB0,0x01,0x20} );
CAN_Frame Msg0AA =  WallE_Can.createStandardCanMessage(0x0AA,2, (uint8_t[]) {0xAA,0x04} );
CAN_Frame Msg29C =  WallE_Can.createStandardCanMessage(0x29C,3, (uint8_t[]) {0x03,0x00,0x1A} );
CAN_Frame Msg29D =  WallE_Can.createRemoteRequestCanMessage(0x29D);
CAN_Frame Msg2AC =  WallE_Can.createStandardCanMessage(0x2AC,3, (uint8_t[]) {0x03,0x08,0x00} );
CAN_Frame Msg2AD =  WallE_Can.createRemoteRequestCanMessage(0x2AD);
CAN_Frame Msg2B4 =  WallE_Can.createStandardCanMessage(0x2B4,3, (uint8_t[]) {0x03,0x00,0x02} );
CAN_Frame Msg2B5 =  WallE_Can.createRemoteRequestCanMessage(0x2B5);
CAN_Frame Msg2BC =  WallE_Can.createStandardCanMessage(0x2BC,3, (uint8_t[]) {0x03,0x00,0x0F} );
CAN_Frame Msg2BD =  WallE_Can.createRemoteRequestCanMessage(0x2BD);
CAN_Frame Msg3A4 =  WallE_Can.createStandardCanMessage(0x3A4,3, (uint8_t[]) {0x03,0x00,0x2C} );
CAN_Frame Msg3AC =  WallE_Can.createStandardCanMessage(0x3AC,3, (uint8_t[]) {0x03,0x00,0x1D} );




void cWallE_Can::print_CanMessage(CAN_Frame  &msg, bool isOutMessage){
    //isoTpMsg_t outMsg;
    if(isOutMessage) {
        Serial.print(F("=>"));
    }
    else {
        Serial.print(F("<="));
    }
    Serial.print("\t");
    Serial.print(millis());
    Serial.print("\t");
    Serial.print(F("ID: "));
    Serial.print(msg.id, HEX);
    Serial.print("\t");
    Serial.print(F("Len: "));
    Serial.print(msg.length);
    //Serial.println(); 
    Serial.print("\t");
    //~ Serial.print(F("Data: "));
    for ( int i = 0; i<msg.length; i++) {
        if (msg.data[i] < 0x10) // If the data is less than 10 hex it will assign a zero to the front as leading zeros are ignored...
        {
            Serial.print('0');
        }
        Serial.print(msg.data[i], HEX); 
        Serial.print(" ");
    }
    Serial.println();   
}


CAN_Frame cWallE_Can::createStandardCanMessage(uint32_t ID, uint8_t length,uint8_t * data) {
    
    CAN_Frame msg; // Create message object to use CAN message structure
    
    msg.id = ID; 
    msg.valid = true;
    msg.rtr = 0;
    msg.extended = CAN_STANDARD_FRAME;
    msg.length = length;  // Data length
    
    for(int i=0; i<length; i++){
        msg.data[i] = data[i];
    }
    return msg;
}

CAN_Frame cWallE_Can::createRemoteRequestCanMessage(uint32_t ID) {
    
    CAN_Frame msg; // Create message object to use CAN message structure
    
    msg.id = ID; 
    msg.valid = true;
    msg.rtr = 1;
    msg.extended = CAN_STANDARD_FRAME;
    //~ msg.extended = CAN_EXTENDED_FRAME;
    msg.length = 0;  // Data length

    return msg;
}
