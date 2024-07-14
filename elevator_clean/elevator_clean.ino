
#define REF_DISTANCE   15 //cm. 초음파 센서 장애물 인식 기준 거리
#define REF_WEIGHT     500 //압력센서 인식 기준 


#define TRIG 4 //TRIG 
#define ECHO 2 //ECHO


#include <Servo.h>

Servo myservo; 


int sw;
int pos;
int mode;
int cnt;
int cnt2;
double force;
double distance;
double a,fil_out,bef_dis = 25;
unsigned long duration;



void setup() 
{
  Serial.begin(115200); 
  
  pinMode(3,OUTPUT);
  pinMode(A5,INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  for(int i=9; i<=12; i++)  //red
    pinMode(i,OUTPUT);
  for(int i=A1; i<=A4; i++) //blue
    pinMode(i,OUTPUT);

  for(int i=9; i<=12; i++)  //red
    digitalWrite(i,HIGH);
  for(int i=A1; i<=A4; i++) //blue
    digitalWrite(i,LOW);
  
  myservo.attach(6); 
  myservo.write(175);  //열림45~닫힘175
}

void loop() 
{
  delay(10); 

  force = analogRead(A0);
  
  sw = digitalRead(A5);
  
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  duration = pulseIn(ECHO, HIGH); //10us~3m pulsein 3분까지. 단위 us

  distance = duration * 17 / 1000; //거리 계산 수식 간소화 : duration/1000000 *340 *100 /2

  a = 0.02;
  fil_out = a*distance + (1.0-a)*bef_dis;
  bef_dis = fil_out;

  //Serial.println((String)distance + "\t" + (String)fil_out);  //cm 단위
  Serial.println((String)fil_out + "\t" + (String)force); 


  if(mode == 0) //내부에 사람 없는 상태 
  {
    if(fil_out < REF_DISTANCE)
    {
      for(int i=0; i<2; i++)
      {
        digitalWrite(3,HIGH);
        delay(100);
        digitalWrite(3,LOW);
        delay(100);
      }
      delay(1000);
      
      for(int i=9; i<=12; i++)  //red
        digitalWrite(i,LOW);
      for(int i=A1; i<=A4; i++)  //blue
        digitalWrite(i,HIGH);
        
      delay(1000);

      for(int i=175; i>=45; i--)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }      

      mode = 1;
    }
  }

  if(mode == 1) //승차 기다림
  {
    if(fil_out > REF_DISTANCE)
    {
      cnt++;      //10ms 지연시간마다 1씩 증가
      if(cnt > 500) //100이면 약 1초
      {
        mode = 2;
        cnt = 0;
      }
    }
  }
  
  if(mode == 2) //승차 여부 판단 
  {
    if(force > REF_WEIGHT)
    {
      digitalWrite(3,HIGH);
      delay(100);
      digitalWrite(3,LOW);
      
      delay(500);

      for(int i=45; i<=175; i++)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }    
      
      mode = 3;
    }
    else
    {
      digitalWrite(3,HIGH);
      delay(600);
      digitalWrite(3,LOW);
      
      delay(500);
            
      for(int i=45; i<=175; i++)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }   
      
      delay(500);
      
      for(int i=9; i<=12; i++)  //red
        digitalWrite(i,HIGH);
      for(int i=A1; i<=A4; i++)  //blue
        digitalWrite(i,LOW);
      
      mode = 0;
    }
    
  }
  if(mode == 3)   //사람 승차한 상태
  {
    if(sw == LOW)
    {
      digitalWrite(3,HIGH);
      delay(100);
      digitalWrite(3,LOW);
      
      delay(1000);
      
      for(int i=175; i>=45; i--)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }      

      mode = 4;
    }
  }
  
  if(mode == 4) //하차 기다림
  {
    cnt++;      //10ms 지연시간마다 1씩 증가
    if(cnt > 500) //100이면 약 1초
    {
      mode = 5;
      cnt = 0;
    }
  }

  if(mode == 5)  //하차 여부 판단
  {
    if(force < REF_WEIGHT)
    {
      digitalWrite(3,HIGH);
      delay(100);
      digitalWrite(3,LOW);
      
      delay(500);

      for(int i=45; i<=175; i++)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }    
      
      delay(500);
      
      for(int i=9; i<=12; i++)  //red
        digitalWrite(i,HIGH);
      for(int i=A1; i<=A4; i++)  //blue
        digitalWrite(i,LOW);
      
      
      mode = 0;
    }
    else
    {
      digitalWrite(3,HIGH);
      delay(600);
      digitalWrite(3,LOW);
      
      delay(500);
            
      for(int i=45; i<=175; i++)
      {
        myservo.write(i);  //열림45~닫힘175
        delay(20);
      }   
      
      mode = 3;
    }
  }
}
