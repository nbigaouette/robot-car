//============================UNIROI========================================
//  Test Infrared Remote button code values
//=============================================================================
#include <IRremote.h>
 
//Infrared Remote Control
int RECV_PIN = 2;//Set Infrared Remote port
IRrecv irrecv(RECV_PIN);
decode_results results;//Store infrared remote decode data
int on = 0;//Infrared Received flag
unsigned long last = millis();

void setup()
{

  Serial.begin(9600);	
  irrecv.enableIRIn(); // Start the receiver
}

void dump(decode_results *results)//Decode Infrared Remote Control Received Signal
{
  int count = results->rawlen;

//Serial port printing, debugging can be opened, the actual operation will affect the reaction speed, recommended shielding
  
  {

    if (results->decode_type == NEC) 
    {
      Serial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) 
    {
      Serial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == RC5) 
    {
      Serial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) 
    {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) 
  {
    if ((i % 2) == 1) 
    {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else  
    {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");

}

void loop()
{
  if (irrecv.decode(&results)) //receive infrared signal
  {
    if (millis() - last > 250) //make sure receive signal
    {
      on = !on;//Sign position reversal
     
      dump(&results);//decode
    }
    
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}



