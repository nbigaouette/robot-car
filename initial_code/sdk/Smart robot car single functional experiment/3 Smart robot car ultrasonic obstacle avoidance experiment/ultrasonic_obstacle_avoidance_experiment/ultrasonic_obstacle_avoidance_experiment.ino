//============================UNIROI==========================
// Ultrasonic check obstacle and avoid Experiment
//===============================================================
int Echo = A1;  // Set Echo port
int Trig =A0;  //  Set Trig port 

int Distance = 0;
//==============================
//==============================
int Left_motor_go=8;     
int Left_motor_back=9;    

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
  Serial.begin(9600);	
 
  pinMode(Left_motor_go, OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 6 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 7 (PWM)
  pinMode(Right_motor_en,OUTPUT);// PIN 5 (PWM)
  pinMode(Left_motor_en,OUTPUT);// PIN 10 (PWM)
  pinMode(key,INPUT);// Set button as input
  pinMode(beep,OUTPUT);// Set buzzer as output
  
  pinMode(Echo, INPUT);    // Set Echo port mode
  pinMode(Trig, OUTPUT);   // Set Trig port mode
  
  digitalWrite(key,HIGH);//Initialize button
  digitalWrite(beep,LOW);// set buzzer mute
  
}
void run()     // ahead
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
 // delay(time * 100);   //Running time can be adjusted 
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




void Distance_test()   // Measuring front distance
{
  digitalWrite(Trig, LOW);    // set trig port low level for 2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // set trig port high level for 10μs(at least 10μs)
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // set trig port low level
  float Fdistance = pulseIn(Echo, HIGH);  // Read echo port high level time(unit:μs)
  Fdistance= Fdistance/58;       // Distance(m) =(time(s) * 344(m/s)) / 2     /****** The speed of sound is 344m/s.*******/
                                 //  ==> 2*Distance(cm) = time(μs) * 0.0344(cm/μs)
                                 // ==> Distance(cm) = time(μs) * 0.0172 = time(μs) / 58
  Serial.print("Distance:");      //Output Distance(cm)
  Serial.println(Fdistance);         //display distance
  Distance = Fdistance;
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
  
    Distance_test();//Measuring front distance
   
    if(Distance <38)//The value is the distance that meets the obstacle, and can be set according to the actual situation   
    { 
      delay(10); 
      Distance_test();//Measuring front distance
      while(Distance<38)//Determine whether there is an obstruction again.If there is obstacle , turn the direction and determine again.
      {
       
       spin_right(4);//Right rotation for 300ms
        brake(1);//stop
        delay(100);
        Distance_test();//Measuring front distance
      }
    }
    else 
         run();//There is nothing obstacled. Go ahead.
       
  }
}









