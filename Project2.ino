//#include <IRremote.h>
//Demo Unit
#define R1 5
#define R2 6
//#define R3 7

#define T1 10
#define T2 11
//#define T3 12

#define Green 2
#define Red 3

unsigned long count = 0;
unsigned long wait = 30;
char guess;
typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);

} task;


int delay_gcd;
const unsigned short tasksNum = 3;
task tasks[tasksNum];

//void initRe(){
//  IrReceiver.begin(R1);
//  IrReceiver.begin(R2);
//  IrReceiver.begin(R3);
//}
//
//void initTe(){
//  IrSender.begin(T1);
//  IrSender.begin(T2);
//  IrSender.begin(T3);
//}
/*
 * Currently Running issues with the IR Sensors not all running at the same time. 
 * Need to change the method of implementation.
 */

enum send_states {send_init, send_wait, send_send} send_state; 

void tick_send(){ 
  switch(send_state){
    case send_init:
    send_state = send_wait;
    break;

    case send_wait:
 //  Serial.print("R1: ");
  //  Serial.println(digitalRead(R1));
    if(digitalRead(R1) == HIGH){
//      Serial.println(count);
      count++;
    }
    if(count == 30){
      send_state = send_send;
    }
     send_state = send_wait;
    break;

    case send_send:
    if(digitalRead(R1) == LOW){
      send_state = send_wait;
    }
    send_state = send_send;
    break;

    default:
    send_state = send_init;
    break;
  }
  switch(send_state){
    case send_init:
    break;

    case send_wait:
    break;

    case send_send:
    break;

    default:
    break;
  }
}



enum rec_states {rec_init, rec_wait, rec_rec} rec_state;

void tick_rec(){
  switch(rec_state){
    case rec_init:
    rec_state = rec_wait;
    break;

    case rec_wait:
    digitalWrite(T1,HIGH);
    digitalWrite(T2,HIGH);
  //  digitalWrite(T3,HIGH);
    rec_state = rec_rec;
    break;

    case rec_rec: //might not even need it
    rec_state = rec_rec;
    break;

    default:
    rec_state = rec_init;
    break;
  }
  switch(rec_state){
    case rec_init:
    break;

    case rec_wait:
    break;

    case rec_rec:
    break;

    default:
    break;
  }
}

enum light_states {l_init, l_red, l_green} light_state;

void tick_light(){
  switch(light_state){
    case l_init:
    light_state = l_red;
    break;

    case l_red:
    //Serial.println("Red State");
    //Serial.println(count);
    digitalWrite(Red, HIGH);
    digitalWrite(Green,LOW);
    if(Serial.available() >0){
      guess = Serial.read();
      Serial.print("Guess: ");
      Serial.println(guess);
      delay(100);
Serial.println(guess);
    if(count > 30 && guess != 'n'){
      Serial.print("RBGuess: ");
      Serial.println(guess);
      light_state = l_green;
    }
    else{
    light_state = l_red;}
  }
     if(count > 30 && guess != 'n'){
      light_state = l_green;
    }
    else{
    light_state = l_red;}
    break;

    case l_green:
    Serial.println("Green State");
    digitalWrite(Green, HIGH);
    digitalWrite(Red, LOW);
    if(wait > 0){
      wait--;
      light_state = l_green;
    }
    else if(digitalRead(R1) == HIGH){
      light_state = l_green;
    }
    else{
    light_state = l_red;}
    break;

    default:
    light_state = l_init;
    break;
    }

  switch(light_state){
    case l_init:
    break;

    case l_red:
    wait = 30;
    break;

    case l_green:
    count = 0;
    break;

    default:
    break;
    }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(Green,OUTPUT);
  pinMode(Red,OUTPUT);
  
  pinMode(T1,OUTPUT);
  pinMode(R1,INPUT);

  pinMode(T2,OUTPUT);
  pinMode(R2,INPUT);
  
  //pinMode(T3,OUTPUT);
  //pinMode(R3,INPUT);
  
  Serial.begin(9600);
//  initRe();
//  initTe();
tone(T1, 38000);
tone(T2, 38000);
//tone(T3, 38000);

  unsigned char i = 0;
  tasks[i].state = rec_init;
  tasks[i].period = 20;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &tick_rec;
  i++;

  tasks[i].state = send_init;
  tasks[i].period = 30;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &tick_send;
  i++;
  
  tasks[i].state = l_init;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &tick_light;

}

void loop() {
  // put your main code here, to run repeatedly:
    unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }

}
