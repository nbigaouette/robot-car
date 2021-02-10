//============================UNIROI========================================
//  Infrared Remote contorl Experiment
//=============================================================================
#include <IRremote.h>
 
//Infrared Remote Control
int RECV_PIN = 2;//Set Infrared Remote port
IRrecv irrecv(RECV_PIN);
decode_results results;//Store infrared remote decode data
int on = 0;//Infrared Received flag
unsigned long last = millis();

long run_car = 0x00FD8877;//key UP 
long back_car = 0x00FD9867;// key Down  
long left_car = 0x00FD28D7;//key left
long right_car = 0x00FD6897;//key right
long stop_car = 0x00FDA857;//key OK
long left_turn = 0x00FD30CF;//key *
long right_turn = 0x00FD708F;//key #
long Buzzer_speak = 0x00FDB04F;//key 0 
//==============================
int Left_motor_back=9;       
int Left_motor_go=8;         
int Right_motor_go=6;        
int Right_motor_back=7;    
int Right_motor_en=5;     
int Left_motor_en=10;  
int Left_led=3;//Left led 
int Right_led=4;//Right led

/*Set Button port*/
int key=13;

/*Set BUZZER port*/
int beep=12; 
void setup()
{
  //Initialize motor drive for output mode
  pinMode(Left_motor_go,OUTPUT); 
  pinMode(Left_motor_back,OUTPUT);
  pinMode(Right_motor_go,OUTPUT);
  pinMode(Right_motor_back,OUTPUT);
  pinMode(Right_motor_en,OUTPUT);
  pinMode(Left_motor_en,OUTPUT);
  pinMode(Left_led,OUTPUT);// set Left_led as output 
  pinMode(Right_led,OUTPUT);// set right_led as output 
  pinMode(key,INPUT);// Set button as input
  pinMode(beep,OUTPUT);// Set buzzer as output
  Serial.begin(9600);	
  irrecv.enableIRIn(); // Start the receiver
}


  void run()     // Advance
{
  digitalWrite(Left_motor_en,HIGH);  // Left motor enable
  analogWrite(Left_motor_en,157);
  digitalWrite(Right_motor_en,HIGH);  // Right motor enable
  
  digitalWrite(Right_motor_go,HIGH);  // right motor go ahead
  digitalWrite(Right_motor_back,LOW);   
  analogWrite(Right_motor_go,200);//PWM--Pulse Width Modulation(0~255). right motor go speed is 255.
  analogWrite(Right_motor_back,0); 
  digitalWrite(Left_motor_go,HIGH);  // set left motor go ahead
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,137);//PWM--Pulse Width Modulation(0~255).left motor go speed is 135.
  analogWrite(Left_motor_back,0);
  //delay(time * 100);   //Running time can be adjusted 
  digitalWrite(Right_led,LOW);//rihgt led OFF
  digitalWrite(Left_led,LOW);//left led OFF
 
}

void brake()         //STOP
{
  digitalWrite(Right_motor_go,LOW);//Stop the right motor
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);//Stop the left motor
  digitalWrite(Left_motor_back,LOW);
  //delay(time * 100);  //Running time can be adjusted 
  led();
 
}

void left()        //turn left
{
  digitalWrite(Right_motor_go,HIGH);	// right motor go ahead
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,190); // PWM--Pulse Width Modulation(0~255) control speed，right motor go speed is 255.
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);   // left motor stop
  digitalWrite(Left_motor_back,LOW); 
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);
  //delay(time * 100);	
  digitalWrite(Left_led,HIGH);//left led light
  digitalWrite(Right_led,LOW);//rihgt led OFF
 
}

void spin_left()   //Left rotation
{
  digitalWrite(Right_motor_go,HIGH);// right motor go ahead
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,200);// PWM--Pulse Width Modulation(0~255) control speed ,right motor go speed is 200.
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);   // left motor back off
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,200);// PWM--Pulse Width Modulation(0~255) control speed,left motor back speed is 200.
  //delay(time * 100);
  digitalWrite(Left_led,HIGH);//left led light
  digitalWrite(Right_led,LOW);//rihgt led OFF
   
}


  void right()      //turn right
{
  digitalWrite(Right_motor_go,LOW);   // right motor stop
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);// left motor go ahead
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,230);// PWM--Pulse Width Modulation(0~255) control speed ,left motor go speed is 255.
  analogWrite(Left_motor_back,0);
  //delay(time * 100);
  digitalWrite(Right_led,HIGH);//rihgt led lihgt
  digitalWrite(Left_led,LOW);//left led light
 
}

void spin_right()   //Right rotation
{
  digitalWrite(Right_motor_go,LOW);  // right motor back off
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,200);// PWM--Pulse Width Modulation(0~255) control speed
  digitalWrite(Left_motor_go,HIGH);// left motor go ahead
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,200);// PWM--Pulse Width Modulation(0~255) control speed 
  analogWrite(Left_motor_back,0);
  //delay(time * 100);
  digitalWrite(Right_led,HIGH);//rihgt led lihgt
  digitalWrite(Left_led,LOW);//left led light
  
}

void back()   //back off 
{
  digitalWrite(Right_motor_go,LOW); //right motor back off
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,150);// PWM--Pulse Width Modulation(0~255) control speed
  
   analogWrite(Right_motor_en,165);
   digitalWrite(Left_motor_go,LOW);  //left motor back off
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);
  analogWrite(Left_motor_back,140);// PWM--Pulse Width Modulation(0~255) control speed
 // delay(time * 100);
  
}
void buzzer()
{
  digitalWrite (beep,HIGH);//set beep sround
  delay(1000);//delay 1s
  digitalWrite (beep,LOW); //set beep mute
}

void led()
{
  digitalWrite(Right_led,HIGH);//rihgt led lihgt
  digitalWrite(Left_led,HIGH);//left led light
  delay (100);
  digitalWrite(Right_led,LOW);//rihgt led  off
  digitalWrite(Left_led,LOW);//left led off
  delay (100);
  digitalWrite(Right_led,HIGH);//rihgt led lihgt
  digitalWrite(Left_led,HIGH);//left led light
  delay (100);
  digitalWrite(Right_led,LOW);//rihgt led  off
  digitalWrite(Left_led,LOW);//left led   off
  delay (100);
  digitalWrite(Right_led,HIGH);//rihgt led lihgt
  digitalWrite(Left_led,HIGH);//left led light
 
    
}
  

void dump(decode_results *results)//Decode Infrared Remote Control Received Signal
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) 
  {
    brake();
  } 

//Serial port printing, debugging can be opened, the actual operation will affect the reaction speed, recommended shielding

  /*else 
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
*/
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
    if (results.value == run_car )//key "UP"
      run();//go ahead 
    if (results.value == back_car )//key "DW"
      back();//back off
    if (results.value == left_car )//key "left"
      left();//turn left
    if (results.value == right_car )//key "right"
      right();//turn right
    if (results.value == stop_car )//key "ok"
      brake();//stop
    if (results.value == left_turn )//key "*"
      spin_left();//Left rotation
    if (results.value == right_turn )//key "#"
      spin_right();//Right rotation
    if (results.value == Buzzer_speak )//key  "0"
     buzzer();//speaker
      
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}



