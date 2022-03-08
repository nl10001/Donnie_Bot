int ir1;
int ir2;
int ir3;
int enb1 = 5;
int in1 = 3;
int in2 = 4;
int enb2 = 6;
int in3 = 7;
int in4 = 8;
int mspeed = 100;
int tspeed = 190;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(A0, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enb1, OUTPUT);

  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enb2, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
ir1 = analogRead(A0); //ir1 connected to pin A0 mid sensor
ir2 = analogRead(A1);//ir2 connected to pin A1 left sensor
ir3 = analogRead(A2);//ir3 connected to pin A2 right sensor

if(ir1 >= 500){ // midle sensor go forward 
  digitalWrite(in1, HIGH);//motors go forward
  digitalWrite(in2, LOW);
  analogWrite(enb1, mspeed);
  digitalWrite(in3, HIGH); //motors go forward
  digitalWrite(in4, LOW);
  analogWrite(enb2, mspeed);
  delay(500);
}
//turn right
else if(ir2<500){
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enb1, mspeed);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enb2, tspeed);
  delay(500);
}
// w
//turn left
else if(ir3<500){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enb1, mspeed);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
  analogWrite(enb2, tspeed);
  delay(500);
}
 
 Serial.println(ir1);
 Serial.println(ir2);
 Serial.println(ir3);
 delay(500);
}
