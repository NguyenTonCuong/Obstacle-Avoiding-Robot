  
#include <Servo.h>
#define IN1 7  //IN1 on pin 3 conrtols one side of bridge A
#define IN2 6  //IN2 on pin 5 controls other side of A
#define IN3 5  //IN3 on pin 6 conrtols one side of bridge B
#define IN4 4  //IN4 on pin 9 controlj's other side of B
#define MAX_SPEED 255 //từ 0-255
#define MIN_SPEED 0
Servo head;
// trạng thái trước đó của button
int leftscanval, centerscanval, rightscanval, ldiagonalscanval, rdiagonalscanval;
int max=0;
char choice;
// cài đặt chân của cảm biến siêu ẩm uq
const int trigger=2;
const int echo=3;
const int distancelimit = 40; //Distance limit for obstacles in front           
const int sidedistancelimit = 40; //Minimum distance in cm to obstacles at both sides (the robot will allow a shorter distance sideways)
int distance;
int numcycles = 0;
// quyết định chiều đi
char turndirection; //Gets 'l', 'r' or 'f' depending on which direction is obstacle free
const int turntime = 300; //Time the robot spends turning (miliseconds)
int thereis;
//byte led1=10;
unsigned long time1=0;
int v =0;
int loa=0;
int pos = 0;
char command; 
void setup(){
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
 //pinMode(11, OUTPUT);
  pinMode(12,INPUT_PULLUP);
  head.attach(10);
  head.write(80); 
}

  int watch(){
  long howfar;
  digitalWrite(trigger,LOW);
  delayMicroseconds(5);                                                                              
  digitalWrite(trigger,HIGH);
  delayMicroseconds(15);
  digitalWrite(trigger,LOW);
  howfar=pulseIn(echo,HIGH);
  howfar=howfar*0.01657; //how far away is the object in cm
  Serial.println(howfar);
  if(howfar>2){
  return round(howfar);
  if(howfar==0){
  return round(50);
}
  }
  }
void watchsurrounding(){ //Meassures distances to the right, left, front, left diagonal, right diagonal and asign them in cm to the variables rightscanval, 
                         //leftscanval, centerscanval, ldiagonalscanval and rdiagonalscanval (there are 5 points for distance testing)
  //beep(50);
  centerscanval = watch();
  if(centerscanval<distancelimit){stopmove();}
  for(pos = 80; pos <= 120; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);                       // waits 15ms for the servo v    m k m  m n  to reach the position 
  } 
  delay(5);
  ldiagonalscanval = watch();
  if(ldiagonalscanval<distancelimit){stopmove();}
  for(pos = 120; pos <= 160; pos += 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);                       // waits 15ms for the servo to reach the position 
  } 
  delay(5);
  leftscanval = watch();
  if(leftscanval<sidedistancelimit){stopmove();}
  for(pos = 160; pos >=120; pos -= 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);                       // waits 15ms for the servo to reach the position 
  } 
  delay(10);
    ldiagonalscanval = watch();
  if(ldiagonalscanval<distancelimit){stopmove();}
  for(pos = 120; pos >=80; pos -= 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);
   }
   delay(10);         
  centerscanval = watch();
  if(centerscanval<distancelimit){stopmove();}
 for(pos = 80; pos >=40; pos -= 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(1);
   }
  delay(10);
  rdiagonalscanval = watch();
  if(rdiagonalscanval<distancelimit){stopmove();}
 for(pos = 40; pos >=0; pos -= 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);
   }
  delay(10);
  rightscanval = watch();
  if(rightscanval<sidedistancelimit){stopmove();}

 for(pos = 0; pos <= 80; pos += 1) 
   {                                  // in steps of 1 degree 
    head.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(2);
   }
}
int timmax(int a,int b,int c,int d){
  int max1;
 if (a>max1){
  max1=a;}
  if (b>max1){
    max1=b;
  }
  if (c>max1){
    max1=c;
  }
  if (d>max1){
    max1=d;
  }
  return max1;
 }
char decide(){
  watchsurrounding();
  int timmax1=timmax(leftscanval,rightscanval,ldiagonalscanval,rdiagonalscanval);
   Serial.println(timmax1);
  if (timmax1==leftscanval){
    choice = 'l';
       Serial.println("quay trai");
  }
   if (timmax1==ldiagonalscanval){
    choice = 'm';
     Serial.println("quay trai");
  }
  if (timmax1==rightscanval){
    choice = 'r';
     Serial.println("quay phai");
  }
  if (timmax1==rdiagonalscanval){
    choice = 's';
     Serial.println("quay phai");
  }
    return choice;
}
void tuhanh(){
   go();  // if nothing is wrong go forward using go() function above.
  distance = watch(); // use the watch() function to see if anything is ahead (when the robot is just moving forward and not looking around it will test the distance in front)
  if (distance<distancelimit){ // The robot will just stop if it is completely sure there's an obstacle ahead (must test 25 times) (needed to ignore ultrasonic sensor's false signals)
   // beep();
      ++thereis;}
  if (distance>distancelimit){
      thereis=0;} //Count is restarted
  if (thereis > 3){
    backwards1(turntime/5);   
    stopmove(); 
    watchsurrounding();
  
    // Since something is ahead, stop moving.
    turndirection = decide(); //Decide which direction to turn.
    switch (turndirection){
     // case 't':
      // tuhanh();
      case 'l':
        turnleft(turntime);
        break;
        case 'm':
        turnleft(turntime);
        break;
      case 'r':
        turnright(turntime);
        break;
        case 's':
        turnright(turntime);
        break;
       
    }
    thereis=0;
  }
 } 
 
// su dung module L298
//dieu khien moto A
void motorA_Dung() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}
 
void motorB_Dung() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void motorA_Tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, HIGH);// chân này không có PWM
  analogWrite(IN2, 255 - speed);
}
void motorA_Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  digitalWrite(IN1, LOW);// chân này không có PWM
  analogWrite(IN2, speed);
}
void motorB_Tien(int speed) { //speed: từ 0 - MAX_SPEED
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(IN3, speed);
  digitalWrite(IN4, LOW);// chân này không có PWM
}
void motorB_Lui(int speed) {
  speed = constrain(speed, MIN_SPEED, MAX_SPEED);//đảm báo giá trị nằm trong một khoảng từ 0 - MAX_SPEED - http://arduino.vn/reference/constrain
  analogWrite(IN3, 255 - speed);
  digitalWrite(IN4, HIGH);// chân này không có PWM
}
void turnleft(int t){
  motorA_Lui(255);
  motorB_Tien(255);
  //motorB_Dung();
  delay(t);
}
void turnright(int t){
 // motorA_Dung();
  motorA_Tien(255);
  motorB_Lui(255);
 delay(t);
}
void backwards1(int t){
       motorA_Lui(230);
       motorB_Lui(230);
       delay(t);
}
void go(){ 
       motorA_Tien(250);
       motorB_Tien(250);
}
void go1(){ 
  //int duty = map(v, 0, 255, 0, 100);
        motorA_Tien(2.55*v);
        motorB_Tien(2.55*v);
}
void backwards(){
       motorA_Lui(230);
       motorB_Lui(230);
}
void backwards2(){
       //int duty = map(v, 0, 255,0, 100);
        motorA_Lui(2.55*v);
        motorB_Lui(2.55*v);
}
void  stopmove(){
        motorA_Dung();
        motorB_Dung();
}
void  stopmove1(int t){
        motorA_Dung();
        motorB_Dung();
        delay(t/2);
}
void right1(){
  //int duty = map(v, 0, 255, 0, 100);
  motorA_Tien(2.55*v-50);
  motorB_Lui(2.55*v-50);
}
void right2(){
  //int duty = map(v, 0, 255, 0, 100);
  motorA_Tien(2.55*v-60);
  motorB_Lui(2.55*v-60);
}
void left1(){
   //int duty = map(v, 0, 255, 0, 100);
 motorA_Lui(2.55*v-50);
 motorB_Tien(2.55*v-50);
}
 void left2(){
   //int duty = map(v, 0, 255, 0, 100);
 motorA_Lui(2.55*v-60);
 motorB_Tien(2.55*v-60);
}
void controlblt(){
if(Serial.available() > 0){ 
    command = Serial.read(); 
    stopmove(); //initialize with motors stoped
    //Change pin mode only if new command is different from previous.   
    Serial.println(command);
    switch(command){
      break;
    case 'F':  
      go1();
      break;
    case 'B':  
       backwards2();
      break;
    case 'L':  
      left1();
      break;
    case 'R':
      right1();
      break;
    case 'W':
      batdentruoc();
      break;
    case 'w':
      tatdentruoc();
      break;
    case 'U':
      batdensau();
      break;
    case 'u':
      tatdensau();
      break;
       case '0':
     v=0;
     break;
    case '1':
      v=10;
      break;
     case '2':
      v=20;
      break;
     case '3':
      v=30;
      break;
    case '4':
      v=40;
      break;
     case '5':
      v=50;
      break;
    case '6':
      v=60;
      break;
     case '7':
      v=70;
      break;
    case '8':
      v=80;
      break;
    case '9':
      v=90;
      break;
    case 'q':
      v=100;
      break;
    case 'I':
      right2();
      break;
    case 'G':
     left2();
      ;
      break;
     case 'H':
        right2();
      break;
     case 'J':
      left2();
     
     break;
     case 'X':
      nguyhien();
     break;
     case 'x':
      tatnguyhien();
     break;
     case 'V':
     coi();
       break;
     case 'v':
     coi();
     break;
    }
  } 
   //stopmove(); //initialize with motors stoped
}
void batdentruoc(){
  digitalWrite(A0, HIGH);
}
void tatdentruoc(){
  digitalWrite(A0, LOW);
}
void batdensau(){
  digitalWrite(A1, HIGH);
}
void tatdensau(){
   digitalWrite(A1, LOW);
}
void nguyhien(){
    digitalWrite(A2, HIGH );
     digitalWrite(A3, HIGH );
}
void tatnguyhien(){
    digitalWrite(A2, LOW );
    digitalWrite(A3, LOW );
}
void coi(){
analogWrite(11, 20);
delay(150);
analogWrite(11, 0); 
delay(150);
}
void tatcoi(){
 analogWrite(11, 0 );
}

 void loop(){
  int control =digitalRead(12);
  if(control==HIGH){
    controlblt();
  }
  else{
    tuhanh();
  }
 }
  
 
      

