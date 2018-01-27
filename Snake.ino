
#include "LedControl.h"

// Piny
const short joyXpin = 2;
const short joyYpin = 3;
const short joyKEYpin = 18;
const short joyVCCpin = 15;
const short joyGNDpin = 14;
const short potPin = 7;

const short DINpin = 11;
const short CLKpin = 9;
const short CSpin  = 10;
const short intensity = 8;

int debugTime = 100;
int messageRefreshTime = 4;


int delka = 3;
int speed = 300;
bool sezrano = true;
bool gameOver = false;
bool dontShowIntro = false;

// primarni souradnice hada
int x = 3;
int y = 4;

int foodX;
int foodY;

const short up     = 0;
const short right  = 1;
const short down   = 2;
const short left   = 3;
int direction = 8;

// pole Map - slouzi jako buffer pro matrix. manipuluje se s timto polem,
// pak az se cele najednou posle na matrix.
bool Map[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 0, 0, 0}, 
    {0, 0, 0, 1, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// pole age - urcuje vek kazde bunky (ledky). podle toho se pak temer automaticky zhasinaji
// bunky za hadem.
long age[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}, 
    {0, 0, 0, 2, 0, 0, 0, 0}, 
    {0, 0, 0, 3, 0, 0, 0, 0}, 
    {0, 0, 0, 4, 0, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0}
};

// pole foodMap - obsahuje informaci o tom, kde se nachazi jidlo. 
bool foodMap[8][8];


const short length = 56;
const bool snejkMessage[8][length] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0}
};

const short length2 = 84;
const bool gameOverMessage[8][length2] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0,0,1,1,0,0,0}
};



LedControl MTX = LedControl(DINpin, CLKpin, CSpin, 1);

void(* restart) (void) = 0; // restartovaci funkce

void setup() 
{
    initialize();
    handleGameStates();
    updateMap();
    wait4move();
    generateFood();
    updateMap();
    
//    Serial.println(freeram());
}


void loop() 
{
    generateFood();
    
    handleGameStates();
    
    scanJoistyck();   // sleduje pohyb joystku, obstarava i cekani mezi "snimky"
    
    calculateSnake(); // vypocet parametru hada
    
    updateMap();      // aktualizace matrixu
    
}



void handleGameStates()
{
    
    
    while(gameOver)
    {
        if(digitalRead(joyKEYpin))
        {
            delay(250);
            for(int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) 
            {
                for(int col = 0; col < 8; col++) 
                {
                    delay(messageRefreshTime);
                    for(int row = 0; row < 8; row++) 
                    {
                        int i = col + d;
                        // MTX.setLed(0, row, col, *(arrayy+size*row+i));
                        MTX.setLed(0, row, col, gameOverMessage[row][i]);
                    }
                }
            }
            
            while(digitalRead(joyKEYpin)){}
            restart();
            while(1){Serial.println("never happens");}
            
            //gameOver = false;
            //break;
        }
    }
    
    
    if(digitalRead(joyKEYpin) && !gameOver && !dontShowIntro)
    {
        for(int d = 0; d < sizeof(snejkMessage[0]) - 7; d++) 
        {
            for(int col = 0; col < 8; col++) 
            {
                delay(messageRefreshTime);
                for(int row = 0; row < 8; row++) 
                {
                    int i = col + d;
                    // MTX.setLed(0, row, col, *(arrayy+size*row+i));
                    MTX.setLed(0, row, col, snejkMessage[row][i]);
                }
            }
        }
        dontShowIntro = true;
    }
    
    dontShowIntro = true;
    
}







/// tento soubor patri k souboru snejk.ino, 
/// do ktereho se automaticky vlozi pri kompilaci.




// skenovani vstupu po urcity cas, znemozneni zmeny smeru o 180 stupnu (celem vzad), a jeste par blbosti
void scanJoistyck()
{
    int previousDirection = direction;
    long timestamp = millis() + speed;
    
    while(millis() < timestamp)
    {
        // nastavovani rychlosti hada, 10-1000ms
        speed = map(analogRead(potPin), 0, 1023, 1, 1000); 

        // zjistovani smeru
        analogRead(joyYpin) < 200 ? direction = up    : 0;
        analogRead(joyYpin) > 800 ? direction = down  : 0;//10
        analogRead(joyXpin) < 200 ? direction = left  : 0;//11
        analogRead(joyXpin) > 800 ? direction = right : 0;
        
        // znemozneni zmeny smeru o 180 stupnu (celem vzad)
        direction + 2 == previousDirection ? direction = previousDirection : 0;
        direction - 2 == previousDirection ? direction = previousDirection : 0;
        
        // blikani jidla
        if(timestamp - millis() < 1)
            MTX.setLed(0, foodY, foodX, 1);
            
        else if(timestamp - millis() < speed / 4)
            MTX.setLed(0, foodY, foodX, 0);
            
        else if(timestamp - millis() < speed / 2)
            MTX.setLed(0, foodY, foodX, 1);
            
        else if(timestamp - millis() < 3 * speed / 4)
            MTX.setLed(0, foodY, foodX, 0);
    }
}



// spocita kompletni data o zmene pohybu hada a zanese je do pole Map.
void calculateSnake()
{
    
    
    switch (direction) {
    case up:
        y--;
        fixOverflow();
        set(x, y, 1);
        break;
    
    case right:
        x++;
        fixOverflow();
        set(x, y, 1);
        break;
    
    case down:
        y++;
        fixOverflow();
        set(x, y, 1);
        break;
    
    case left:
        x--;
        fixOverflow();
        set(x, y, 1);
        break;
    }
    detectBody();
    handleFood();
    updateAge();
}



void detectBody()
{
    if(age[y][x] > 3)
    {
        gameOver = true;
    }
}


void handleFood()
{
    if(foodMap[y][x])
    {
        foodMap[y][x] = 0;
        delka++;
        sezrano = true;
    }
}



void generateFood()
{
    if(sezrano)
    {
        while(Map[foodY][foodX] == 1)
        {
            foodX = random(8);
            foodY = random(8);
        }
        
        Map[foodY][foodX] = 1; 
        foodMap[foodY][foodX] = 1;
        sezrano = false;
        
    }
}


// postara se o spravne nastaveni "stari" rozsvicenych ledek v poli age.
void updateAge()
{
    for(int row = 0; row < 8; row++) 
    {
        for(int col = 0; col < 8; col++) 
        {
            if(age[row][col] > delka || age[row][col] == 0)
            {
                age[row][col] = 0;
                Map[row][col] = 0;
            }
            
            if(age[row][col] != 0 && age[row][col] != -1)
            {
                age[row][col]++;
            }
            
        }
    }
}



// zpusibi objeveni hada na druhe strane obrazovky v pripade "vyjeti ven"
void fixOverflow()
{
    x < 0 ? x += 8 : 0;
    x > 7 ? x -= 8 : 0;
    y < 0 ? y += 8 : 0;
    y > 7 ? y -= 8 : 0;
}



// najde konce hada. nedokoncene, zabugovane, funguje spolehlive jen poro zvisleho hada. nepouzite.
void detectEnds()
{
    bool detectTerminate = false;
    for(int row = 0; row < 8; row++) 
    {
        for(int col = 0; col < 8; col++) 
        {
            if(Map[row][col] == 1 && detectTerminate == false)
            {
                y = row;
                x = col;
                detectTerminate = true;
            }
            
            if(Map[row][col] == 1 && detectTerminate == true)
            {
                // assY = row; // zastarale, pred pouzitim zkontrolovat!
                // assX = col; // zastarale, pred pouzitim zkontrolovat!
            }
        }
    }
    detectTerminate = false;
}



// prekresli obsah pole Map na fyzicky displej, zobrazuje i data z foodMap
void updateMap()
{
    for(int row = 0; row < 8; row++) 
    {
        for(int col = 0; col < 8; col++) 
        {
            if(foodMap[row][col])
            {
                MTX.setLed(0,row,col,foodMap[row][col]);
            }
            else
            {
                MTX.setLed(0,row,col,Map[row][col]);
            }
        }
    }
}


// // printMessage(pole, sizeof(pole[0]));
 
// void printMessage(const bool *arrayy, int size)
// {
    
    
// }



void wait4move()
{
    while(direction == 8)
    {
        analogRead(joyYpin) < 200 ? direction = up    : 0;
        analogRead(joyYpin) > 800 ? direction = up    : 0; // dozadu se nepocita :)
        analogRead(joyXpin) < 200 ? direction = left  : 0;
        analogRead(joyXpin) > 800 ? direction = right : 0;
        
        // toto zpusobi opravdu nahodne generovani jidel
        randomSeed(millis());
    }
    foodX = random(8);
    foodY = random(8);
}


// nakresli na obrazovku obrazek z pole. nepouzite.
void draw(byte data[8])
{
    for(int i; i <= 7; i++)
    {
        MTX.setRow(0, i, data[i]);
    }
}




// debug
void msg(String input, int n)
{
    delay(debugTime);
    Serial.print(input);
    Serial.println(n);
    delay(debugTime);
}

// debug
void msg(String input)
{
    delay(debugTime);
    Serial.println(input);
    delay(debugTime);
}







// pohodlne zanese data do pole Map, zvysi jejich vek.
void set(int x, int y, bool state)
{
    Map[y][x] = state;
    age[y][x]++;
}

// jen zkratka, pro prehlednost kodu
void initialize()
{
    pinMode(joyVCCpin, OUTPUT);
    digitalWrite(joyVCCpin, HIGH); // umele VCC pro joy
    
    pinMode(joyGNDpin, OUTPUT);
    digitalWrite(joyGNDpin, LOW);  // umele GND pro joy
    
    pinMode(joyKEYpin, INPUT_PULLUP);
    
    Serial.begin(9600);
    
    MTX.shutdown(0, false);         // zapnuti matrixu
    MTX.setIntensity(0, intensity); // nastaveni jasu matrixu
    MTX.clearDisplay(0);            // smazani matrixu
}


