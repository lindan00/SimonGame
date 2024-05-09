/* Name: Linda Nguyen
 *
 * Date: April 12th, 2023
 * Purpose: Create a program that imitates the 'Simon' Game
 */

//define constants etc.
#define LED_LOSE 13
#define LEDR 12
#define LEDY 11
#define LEDB 10
#define LEDG 9
#define LED_WIN 8

#define SWITCHR 7
#define SWITCHY 6
#define SWITCHB 5
#define SWITCHG 4

#define PERIOD 1000
#define ROUNDS 5
#define ENDLEVEL 6

#define TRUE 1
#define FALSE 0

void ledSwitch(bool STATE){
  digitalWrite(LEDR,STATE);
  digitalWrite(LEDY,STATE);
  digitalWrite(LEDB,STATE);
  digitalWrite(LEDG,STATE);
}

void gameStart(int level){
  unsigned long timei = millis();
  if(level == 1){
    while(millis()-timei < 0.5*PERIOD){
      ledSwitch(HIGH);
    }
    timei = millis();

    while(millis()-timei < 0.5*PERIOD){
      ledSwitch(LOW);
    }
  }
}

void randNumGen(int randomNums[], int level){
  if(level == 1){
    for(int rounds = 0; rounds < ROUNDS; rounds++){
      randomNums[rounds] = random(LEDG,LED_LOSE);
    }  
  }
}

int push(void){
  int buttonPushed = 0;
  unsigned long outerTimer;
  while(millis()-outerTimer < 2.5*PERIOD && buttonPushed == 0){
    unsigned long innerTimer = millis();
    while(digitalRead(SWITCHR) == HIGH){
      digitalWrite(LEDR,HIGH);
      if(millis()-innerTimer > 0.05*PERIOD){
        buttonPushed = LEDR;
      }
    }
    while(digitalRead(SWITCHY) == HIGH){
      digitalWrite(LEDY,HIGH);
      if(millis()-innerTimer > 0.05*PERIOD){
        buttonPushed = LEDY;
      }
    }
    while(digitalRead(SWITCHB) == HIGH){
      digitalWrite(LEDB,HIGH);
      if(millis()-innerTimer > 0.05*PERIOD){
        buttonPushed = LEDB;
      }
    } 
    while(digitalRead(SWITCHG) == HIGH){
      digitalWrite(LEDG,HIGH);
      if(millis()-innerTimer > 0.05*PERIOD){
        buttonPushed = LEDG;
      }
    }    
  }
  ledSwitch(LOW);
  return buttonPushed; 
}

int badPush(int lost){
  int badPush = push();
  if(lost == FALSE && badPush != FALSE){
    lost = TRUE;
  }
  return lost;
}

int simon(int level, int randomNums[], int lost){
  unsigned long timei = millis();
  for(int sequence = 0; sequence < level && lost == FALSE; sequence++){
      lost = badPush(lost);
      while(millis()-timei < 0.5*PERIOD){
        digitalWrite(randomNums[sequence], HIGH);
      lost = badPush(lost);
      }
      lost = badPush(lost);
      timei = millis();
      while(millis()-timei < 0.25*PERIOD){
        digitalWrite(randomNums[sequence],LOW);
      }
  }
  return lost;
}

void endLeds(int led){
    digitalWrite(led,HIGH); 
    delay(1000);   
    digitalWrite(led,LOW);
    delay(1000);    
    digitalWrite(led,HIGH);
    delay(1000);    
    digitalWrite(led,LOW);
    delay(1000);    
    digitalWrite(led,HIGH);  
    delay(1000); 
    digitalWrite(led, LOW);
}

int simonSays(int level, int randomNums[], int lost){
  for(int copySequence = 0;(copySequence < level && lost == FALSE); copySequence++){
      unsigned long timei = millis();
      int buttonPushed = 0;
    
    while(buttonPushed == 0){
      buttonPushed = push();
    }
    if(buttonPushed != randomNums[copySequence] || millis()-timei > 4*PERIOD){
      lost = TRUE;
    }
  }
  return lost;
}

int gameEnd(int level, int lost){
  unsigned long timei = millis();
  while(millis()-timei < PERIOD){
    if(level == ROUNDS){
      if(lost == FALSE){
        endLeds(LED_WIN);        
      }
      level = ENDLEVEL;
    }
    if(lost == TRUE){
      endLeds(LED_LOSE);
      level = ENDLEVEL;
    }

    if(level < ROUNDS){
      digitalWrite(LED_WIN,HIGH);
      delay(500);
      digitalWrite(LED_WIN,LOW);
    }
    delay(500);
  }
  return level;
}
void setup() {
  pinMode(LED_LOSE, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LED_WIN, OUTPUT);
  //Setting the LEDS as outputs
  
  pinMode(SWITCHR, INPUT);
  pinMode(SWITCHY, INPUT);
  pinMode(SWITCHB, INPUT);
  pinMode(SWITCHG, INPUT);
  //Setting the push buttons as inputs

  randomSeed(analogRead(0));
}
//main program loop
void loop() {
  unsigned long timei;
  int randomNums[ROUNDS];
  int lost = FALSE;
  
  for(int level = 1; level < ENDLEVEL; level++){
    gameStart(level);
    randNumGen(randomNums,level);
    lost = badPush(lost);
    lost = simon(level, randomNums, lost);
    lost = simonSays(level, randomNums, lost);
    level = gameEnd(level, lost);
    }
  }