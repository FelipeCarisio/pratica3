    /////////////////////////////////////////////////////////////////
   //         Arduino Receive Phone Call Tutorial          v1.02  //
  //       Get the latest version of the code here:              //
 //      http://educ8s.tv/arduino-receive-phone-call            //
/////////////////////////////////////////////////////////////////

#include "SIM900.h"
#include <SoftwareSerial.h>
#include "call.h"

CallGSM call;
boolean started=false;

void setup()
{
    Serial.begin(9600);
    
    if (gsm.begin(9600)) 
    {
        Serial.println("\nstatus=READY");
        started=true;
    } 
    else 
        Serial.println("\nstatus=IDLE");
}

void loop()
{


  switch (call.CallStatus())
  {
    case CALL_NONE: // Nothing is happening

      break;

    case CALL_INCOM_VOICE : // Yes! Someone is calling us

      Serial.println("RECEIVING CALL");
       call.PickUp();
       
      break;

    case CALL_COMM_LINE_BUSY:  // In this case the call would be established

      Serial.println("TALKING. Line busy.");
      
      break;
  }
  delay(1000);
}
