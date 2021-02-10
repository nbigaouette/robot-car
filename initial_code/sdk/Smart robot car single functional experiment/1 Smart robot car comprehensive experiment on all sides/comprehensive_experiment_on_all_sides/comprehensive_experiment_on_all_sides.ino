//============================UNIROI===========================
//  multidirectional synthesis experiment
//===============================================================
//#include <Servo.h> 
int Left_motor_back=9;       
int Left_motor_go=8;         

int Right_motor_go=6;       
int Right_motor_back=7;   

int Right_motor_en=5;      
int Left_motor_en=10;      
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
  
  pinMode(key,INPUT);// Set button as input
  pinMode(beep,OUTPUT);// Set buzzer as output
  digitalWrite(key,HIGH);//Initialize button
  digitalWrite(beep,LOW);// set buzzer mute
  
}
void run(int time)     // advance
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
  delay(time * 100);   //Running time can be adjusted 
}

void brake(int time)         //STOP
{
  digitalWrite(Right_motor_go,LOW);//Stop the right motor
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);//Stop the left motor
  digitalWrite(Left_motor_back,LOW);
  delay(time * 100);  //Running time can be adjusted  
}

void left(int time)        //turn left
{
  digitalWrite(Right_motor_go,HIGH);	// right motor go ahead
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,190); // PWM--Pulse Width Modulation(0~255) control speed，right motor go speed is 255.
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);   // left motor stop
  digitalWrite(Left_motor_back,LOW); 
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);
  delay(time * 100);	
}
void spin_left(int time)   //Left rotation
{
  digitalWrite(Right_motor_go,HIGH);// right motor go ahead
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,200);// PWM--Pulse Width Modulation(0~255) control speed ,right motor go speed is 200.
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,LOW);   // left motor back off
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,200);// PWM--Pulse Width Modulation(0~255) control speed,left motor back speed is 200.
  delay(time * 100);
}

void right(int time)      //turn right
{
 digitalWrite(Right_motor_go,LOW);   // right motor stop
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);// left motor go ahead
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,230);// PWM--Pulse Width Modulation(0~255) control speed ,left motor go speed is 255.
  analogWrite(Left_motor_back,0);
  delay(time * 100);
}

void spin_right(int time)   //Right rotation
{
  digitalWrite(Right_motor_go,LOW);  // right motor back off
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,200);// PWM--Pulse Width Modulation(0~255) control speed
  digitalWrite(Left_motor_go,HIGH);// left motor go ahead
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,200);// PWM--Pulse Width Modulation(0~255) control speed 
  analogWrite(Left_motor_back,0);
  delay(time * 100);
}

void back(int time)   //back off 
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
  delay(time * 100);
}

 //==========================================================

void keysacn()
{
  int val;   
  val=digitalRead(key);// Reads the button ,the level value assigns to val
  while(digitalRead(key))// When the button is not pressed
  {
    val=digitalRead(key);
  }
  while(!digitalRead(key))// When the button is pressed
  {
    delay(10);	//delay 10ms
    val=digitalRead(key);// Reads the button ,the level value assigns to val
    if(val==LOW)  //Double check the button is pressed
    {
       
      digitalWrite(beep,HIGH);//The buzzer sounds
      delay(50);//delay 50ms
      while(!digitalRead(key))	//Determine if the button is released or not
        digitalWrite(beep,LOW);//mute
    }
    else
      digitalWrite(beep,LOW);//mute
  }
}

/*main loop*/

void loop()
{
  keysacn();//Press the button to start
 
  while(1)


{
      
       back(10); //back 1s
       brake(5);//stop 0.5s
       run(10);//ahead  1s
       brake(5);//stop  0.5s
       left(10);//turn left  1s
       right(10);//turn right 1s
       spin_right(20); //Right rotation  2s
       spin_left(20);//left rotation2s
       brake(5);//stop  0.5s 

}
}


