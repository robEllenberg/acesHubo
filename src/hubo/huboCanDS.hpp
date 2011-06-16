#ifndef ACES_HUBOCANi_HPP
#define ACES_HUBOCAN_HPP

#include <iostream>
#include <sys/time.h>
#include "can4linux.h"  //Include the can4linux data structures

//For reference, the canmsg_t data structure from can4linux
/*
typedef struct {
    // flags, indicating or controlling special message properties 
    int             flags;
    int             cob;	     // < CAN object number, used in Full CAN  
    unsigned   long id;		     // < CAN message ID, 4 bytes  
    struct timeval  timestamp;	 // < time stamp for received messages 
    short      int  length;	     // < number of bytes in the CAN message 
    unsigned   char data[CAN_MSG_LENGTH]; // < data, 0...8 bytes 
} canmsg_t;
*/

//CAN Notes
/*
    High ID -> Low Priority
    Low ID -> High Priority
*/

namespace Hubo{
    class canMsg {
        public:
            canMsg();
            canMsg(unsigned long id, short int length, unsigned char* data);
            canMsg(unsigned long id, short int len, unsigned long d1,
                   unsigned long d2);
            void printMe();
            canmsg_t toDriverType();
        private:                       //   [bits] Meaning
            int flags;                  
            int cob;	               //!  CAN object number, used in Full CAN  
            unsigned long id;		   //!  [11/29]CAN message ID, 4 bytes  
            struct timeval  timestamp; //!  time stamp for received messages 
            short int  length;	       //!  [4]number of bytes in the CAN message 
            unsigned char data[8];     //!  [0-64]data, 8 bytes 

    };
}
#endif
