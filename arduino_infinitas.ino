#include <HID-Project.h>
#include <HID-Settings.h>



char keys[7] = {'z','s','x','d','c','f','v'};
int key[7] = {3,4,5,6,7,8,9};

//rotary
int rotaryFlag[2][2] = {{0,0},{0,0}};  //[0] is Previous state [1] is Current state.
int rotaryState[4][2] = {{0,0},{0,1},{1,1},{0,1}};
int rotaryState2[4] = {0,1,3,2};
int countA = 200;


int i=0;


int val[2]; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  NKROKeyboard.begin();
  for(i=0;i<7;i++){
    pinMode(key[i], INPUT);
  }
  pinMode(11, INPUT);
  pinMode(10, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=0;i<7;i++){
    if(digitalRead(key[i]))NKROKeyboard.add(keys[i]);else NKROKeyboard.remove(keys[i]);
  }
  i = rotaryCheck(10,11,1);
    switch(i){
     case 0:
       if(countA>200){
       NKROKeyboard.remove('l');
       NKROKeyboard.remove(';');
       }else{
         countA++;
       }
       break;
     case 1:
       NKROKeyboard.add('l');
       NKROKeyboard.remove(';');
       countA = 0;
       break;
     case 2:
       NKROKeyboard.add(';');
       NKROKeyboard.remove('l');
       countA = 0;
       break;
     default:
       break;
    } 
  NKROKeyboard.send();
}

int rotaryCheck(int a,int b,int LR){
  val[0]=digitalRead(a);
  val[1]=digitalRead(b);
  for(i=0;i<4;i++){
    if(memcmp(val,rotaryState[i] ,sizeof(int) * 2) == 0){
      rotaryFlag[1][LR] = i;
    }
  }
  if(rotaryFlag[0][LR] ==0 && rotaryFlag[1][LR] ==3 || rotaryFlag[0][LR] ==3 && rotaryFlag[1][LR] ==0 ||rotaryFlag[0][LR] ==1 && rotaryFlag[1][LR] ==2 ||rotaryFlag[0][LR] ==2 && rotaryFlag[1][LR] ==1 ){
    rotaryFlag[0][LR] = rotaryFlag[1][LR];
    return 3;
  }
  if(rotaryFlag[0][LR] != rotaryFlag[1][LR]){
    if(rotaryFlag[0][LR] ==0 && rotaryFlag[1][LR] ==2 ){
      rotaryFlag[0][LR] = rotaryFlag[1][LR];
      return 2;
    }
    else if(((rotaryState2[rotaryFlag[0][LR]]<<1)+rotaryState2[rotaryFlag[1][LR]])%4>1){
      rotaryFlag[0][LR] = rotaryFlag[1][LR];  
      
      return 1;
    }else{
      rotaryFlag[0][LR] = rotaryFlag[1][LR];
      return 2;
    }
  }else{
    return 0;
  }
  rotaryFlag[0][LR] = rotaryFlag[1][LR];

}
