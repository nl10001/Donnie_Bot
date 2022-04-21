
#include <ros.h>
#include <math.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32MultiArray.h>


#define TRIGPIN 11
#define ECHOPIN 12

#define IRL A0
#define IRM A1
#define IRR A2


#define MOTOR_RIGHT 5 // 
#define MOTOR_LEFT 6 //
#define MOTOR_IN1 7 //in 1 motor connection
#define MOTOR_IN2 8 //in 2 motor connection
#define MOTOR_IN3 3 //in 3 motor connection
#define MOTOR_IN4 4 //in 4 motor connection

float wheelR=0;
float wheelL=0;



int sensePin=A0;
int tempInput=0;

float irArray[3]={0, 0, 0};

std_msgs::Float32 sonar_msg;
std_msgs::Float32 temp_msg;
std_msgs::Float32MultiArray ir_msg;

ros::Publisher pub_sonar("sonar",&sonar_msg);
ros::Publisher pub_temp("temp",&temp_msg);
ros::Publisher pub_ir("ir",&ir_msg);
ros::NodeHandle nh;

void motorcb(const geometry_msgs::Twist& Twist){
  float x_lin=0;
  float z_ang=0;
  float lin=0;
  float ang=0;
  x_lin=Twist.linear.x;
  z_ang=Twist.angular.z;
  if (x_lin > 1){
    lin =1;
  }
  else if (x_lin < -1){
    lin =-1;
  }
  else{
    lin=x_lin;
  }
  if (z_ang > 1){
    ang=1;
  }
  else if (z_ang < -1){
    ang=-1;
  }
  else {
    ang=z_ang;
  }
  wheelL=lin+ang;
  wheelR=lin-ang;
  
  if (wheelR >0 && wheelL > 0){
    digitalWrite(MOTOR_IN1,HIGH);
    digitalWrite(MOTOR_IN2,LOW);
    digitalWrite(MOTOR_IN3,HIGH);
    digitalWrite(MOTOR_IN4,LOW);
  }
  else if(wheelR < 0 && wheelL < 0){
    digitalWrite(MOTOR_IN1,LOW);
    digitalWrite(MOTOR_IN2,HIGH);
    digitalWrite(MOTOR_IN3,LOW);
    digitalWrite(MOTOR_IN4,HIGH);
  }
  else if(wheelR < 0 && wheelL > 0){
    digitalWrite(MOTOR_IN1,HIGH);
    digitalWrite(MOTOR_IN2,LOW);
    digitalWrite(MOTOR_IN3,LOW);
    digitalWrite(MOTOR_IN4,HIGH);
  }
  else if(wheelR > 0 && wheelL < 0){
    digitalWrite(MOTOR_IN1,LOW);
    digitalWrite(MOTOR_IN2,HIGH);
    digitalWrite(MOTOR_IN3,HIGH);
    digitalWrite(MOTOR_IN4,LOW);
  }
  else{
    digitalWrite(MOTOR_IN1,LOW);
    digitalWrite(MOTOR_IN2,LOW);
    digitalWrite(MOTOR_IN3,LOW);
    digitalWrite(MOTOR_IN4,LOW);
  }
  
  wheelL=abs(wheelL)*127;
  wheelR=abs(wheelR)*127;
  
  analogWrite(MOTOR_LEFT,wheelL);
  analogWrite(MOTOR_RIGHT,wheelR);
  
  
}


ros::Subscriber<geometry_msgs::Twist> motorSub("cmd_vel",motorcb);

float duration;
int distance;

void setup() {
  nh.initNode();
  nh.advertise(pub_sonar);
  nh.advertise(pub_temp);
  nh.advertise(pub_ir);
  nh.subscribe(motorSub);
  pinMode(TRIGPIN,OUTPUT);
  pinMode(ECHOPIN,INPUT);
  pinMode(sensePin,INPUT);
  pinMode(IRL, INPUT);
  pinMode(IRM, INPUT);
  pinMode(IRR, INPUT);
  pinMode(MOTOR_RIGHT,OUTPUT);
  pinMode(MOTOR_LEFT,OUTPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);
  pinMode(MOTOR_IN3,OUTPUT);
  pinMode(MOTOR_IN4,OUTPUT);
  
  // put your setup code here, to run once:

}

long publisher_timer;


void loop() {
  if(millis()>publisher_timer){
    irArray[0]=analogRead(IRL);
    irArray[1]=analogRead(IRM);
    irArray[2]=analogRead(IRR);
    ir_msg.data=irArray;
    ir_msg.data_length=3;
    pub_ir.publish(&ir_msg);
  
  
    digitalWrite(TRIGPIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGPIN,HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGPIN,LOW);
    duration =pulseIn(ECHOPIN,HIGH);
    distance =duration*0.034/2;
    sonar_msg.data=distance;
    pub_sonar.publish(&sonar_msg);

    publisher_timer=millis()+50;
  }


  //float temp;
  //tempInput= analogRead(sensePin);
  //temp=(float)tempInput;
  //temp=(((temp*5)-0.5)*100);
  //temp=(temp-32)/1.8;
  //temp_msg.data=temp;
  //pub_temp.publish(&temp_msg);

  
  
  // put your main code here, to run repeatedly:

nh.spinOnce();
}
