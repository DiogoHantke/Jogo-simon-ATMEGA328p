#define BUTTONRED A0
#define BUTTONBLUE A1
#define BUTTONYELLOW A2
#define BUTTONGREEN A3

#define LEDGREEN 2
#define LEDYELLOW 3
#define LEDBLUE 4
#define LEDRED 5

#define BUZZER 6

#define MAX 60

#define START 0
#define INGAMING 1
#define GAMEOVER 2

int btnEvent(int timeNumber);
void ledEvent(int numberLed, int timeNumber);
int start();
int gaming(int numberFirst);
void gameOver();

void(* resetFunc) (void) = 0;

void setup()
{
    pinMode(BUTTONRED, INPUT);
    pinMode(BUTTONBLUE, INPUT);
    pinMode(BUTTONYELLOW, INPUT);
    pinMode(BUTTONGREEN, INPUT);

    pinMode(LEDGREEN, OUTPUT);
    pinMode(LEDYELLOW, OUTPUT);
    pinMode(LEDBLUE, OUTPUT);
    pinMode(LEDRED, OUTPUT);

    pinMode(BUZZER, OUTPUT);

    Serial.begin(9600);

    randomSeed(analogRead(A5)); 
}

void loop()
{  
    static int points = 0;
    int numberFirst = start();

    points = gaming(numberFirst);

    Serial.println("");
    Serial.print("points: ");
    Serial.println(points);

    gameOver();
}

int start(){ 
    int numberBtn = -1;
    int numberFirst = -1;
    int timeNumber = 1000;

    int listLeds[]{
        LEDGREEN, 
        LEDYELLOW, 
        LEDBLUE, 
        LEDRED      
    };
    
    int index = 0;
    static unsigned long previousTime = 0;
        
    while(numberBtn < 0){

        numberBtn = btnEvent(timeNumber);
        
        if(numberBtn >= 0){
            numberFirst = numberBtn;
            Serial.println("started");
            return numberFirst;
        }

        if((millis() - previousTime) >= 200){

          previousTime = millis();

          digitalWrite(listLeds[(index+3)%4], LOW);
          digitalWrite(listLeds[index], HIGH);

          index++;

          if(index >= 4) index = 0;
        }
    }

    return 0;
}

int gaming(int numberFirst){

    bool stateGame = true;

    int listNumber[MAX] = {numberFirst};
    int difficulty = numberFirst;

    int timeNumber = 1000 - (numberFirst*200);

    int btnNumber = -1;
    int lenListNumber = 1;
    int points = 0;
    
    while(stateGame){

      for(int i = 0; i < lenListNumber; i++){
        delay(timeNumber);
        ledEvent(listNumber[i], timeNumber);
      }

      for(int i = 0; i < lenListNumber; i++){
        
        btnNumber = -1;

        while(btnNumber < 0){
            btnNumber = btnEvent(timeNumber);
        }

        Serial.println(btnNumber);

        if(btnNumber == listNumber[i]){
          points++;
        }else{
          stateGame = false;
          break;
        }
      }

      if(!stateGame) break;

      if(lenListNumber < MAX){
        listNumber[lenListNumber] = random(0,4);
        lenListNumber++;
      }
    }
    
    return points;
}

void gameOver(){

    int melody[] = {784, 659, 523, 392, 330, 262};
    int duration = 200;

    for(int i = 0; i < 6; i++){
        tone(BUZZER, melody[i], duration);
        delay(duration);
    }

    noTone(BUZZER);

    for(int i = 0; i < 5; i++){

        digitalWrite(LEDGREEN, HIGH);
        digitalWrite(LEDYELLOW, HIGH);
        digitalWrite(LEDBLUE, HIGH);
        digitalWrite(LEDRED, HIGH);

        delay(150);

        digitalWrite(LEDGREEN, LOW);
        digitalWrite(LEDYELLOW, LOW);
        digitalWrite(LEDBLUE, LOW);
        digitalWrite(LEDRED, LOW);

        delay(150);
    }

    delay(100);
    resetFunc();
}

void ledEvent(int numberLed, int timeNumber){

    int listLeds[]{
        LEDGREEN, 
        LEDYELLOW, 
        LEDBLUE, 
        LEDRED      
    };

    for(int i = 0; i < 4; i++)
        digitalWrite(listLeds[i], LOW);

    switch(numberLed){

      case 0:
        digitalWrite(LEDRED, HIGH);
        tone(BUZZER, 440, timeNumber);
        break;  
    
      case 1:
        digitalWrite(LEDBLUE, HIGH);
        tone(BUZZER, 494, timeNumber);
        break;  
    
      case 2:
        digitalWrite(LEDYELLOW, HIGH);
        tone(BUZZER, 523, timeNumber);
        break;  
    
      case 3:
        digitalWrite(LEDGREEN, HIGH);
        tone(BUZZER, 587, timeNumber);
        break;
    }
    
    delay(timeNumber);

    for(int i = 0; i < 4; i++)
        digitalWrite(listLeds[i], LOW);

    noTone(BUZZER);
}

int btnEvent(int timeNumber){

    int listBtn[] = {
        BUTTONRED,
        BUTTONBLUE,
        BUTTONYELLOW,
        BUTTONGREEN
    };

    int numberBtn = -1;

    for(int i = 0; i < 4; i++){

        if(digitalRead(listBtn[i]) == HIGH){
            numberBtn = i;
            break;
        }
    }
    
    if(numberBtn >= 0){

        while(digitalRead(listBtn[numberBtn]) == HIGH){
            delay(10);
        }

        ledEvent(numberBtn, timeNumber);
    }

    return numberBtn;
}
