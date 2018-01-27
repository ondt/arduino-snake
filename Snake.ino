#include "LedControl.h" // LedControl library is used for controlling a LED matrix. Find it using Library Manager or download zip here: https://github.com/wayoda/LedControl

// there are defined all the pins.
struct Pin {
	static const short joystickX = A2;   // joystick X axis pin
	static const short joystickY = A3;   // joystick Y axis pin
	static const short joystickKEY = 18; // joystick KEY pin (Z axis button) (A4)
	static const short joystickVCC = 15; // virtual VCC for the joystick (to make the joystick connectable right next to the arduino nano) (A1)
	static const short joystickGND = 14; // virtual GND for the joystick (to make the joystick connectable right next to the arduino nano) (A0)

	static const short potentiometer = A7; // potentiometer for snake speed control

	static const short CLK = 10; // clock for LED matrix
	static const short CS  = 11; // chip-select for LED matrix
	static const short DIN = 12; // data-in for LED matrix
};

// LED matrix brightness between 0(darkest) and 15(brightest)
const short intensity = 8;


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
const PROGMEM bool snejkMessage[8][length] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const short length2 = 84;
const PROGMEM bool gameOverMessage[8][length2] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0}
};

int messageSpeed = 4; // lower = faster message scrolling


LedControl matrix(Pin::DIN, Pin::CLK, Pin::CS, 1);

void(* restart) (void) = 0; // restartovaci funkce

void setup() {
	initialize();
	handleGameStates();
	updateMap();
	wait4move();
	generateFood();
	updateMap();
}


void loop() {
	generateFood();
	handleGameStates();
	scanJoystick();   // sleduje pohyb joystku, obstarava i cekani mezi "snimky"
	calculateSnake(); // vypocet parametru hada
	updateMap();      // aktualizace matrixu
}



void handleGameStates() {
	while (gameOver) {
		if (digitalRead(Pin::joystickKEY)) {
			delay(250);
			for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {

				for (int col = 0; col < 8; col++) {
					delay(messageSpeed);
					for (int row = 0; row < 8; row++) {
						// this reads the byte from the PROGMEM and displays it on the screen
						matrix.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d])));
					}
				}
			}

			while (digitalRead(Pin::joystickKEY)) {}
			restart();

			while (1) {
				Serial.println("never happens");
			}

			//gameOver = false;
			//break;
		}
	}

	if (digitalRead(Pin::joystickKEY) && !gameOver && !dontShowIntro) {
		for (int d = 0; d < sizeof(snejkMessage[0]) - 7; d++) {
			for (int col = 0; col < 8; col++) {
				delay(messageSpeed);
				for (int row = 0; row < 8; row++) {
					// this reads the byte from the PROGMEM and displays it on the screen
					matrix.setLed(0, row, col, pgm_read_byte(&(snejkMessage[row][col + d])));
				}
			}
		}
		dontShowIntro = true;
	}

	dontShowIntro = true;

}








// skenovani vstupu po urcity cas, znemozneni zmeny smeru o 180 stupnu (celem vzad), a jeste par blbosti
void scanJoystick() {
	int previousDirection = direction;
	long timestamp = millis() + speed;

	while (millis() < timestamp) {
		// nastavovani rychlosti hada, 10-1000ms
		speed = map(analogRead(Pin::potentiometer), 0, 1023, 1, 1000);

		// zjistovani smeru
		analogRead(Pin::joystickY) < 200 ? direction = up    : 0;
		analogRead(Pin::joystickY) > 800 ? direction = down  : 0;//10
		analogRead(Pin::joystickX) < 200 ? direction = left  : 0;//11
		analogRead(Pin::joystickX) > 800 ? direction = right : 0;

		// znemozneni zmeny smeru o 180 stupnu (celem vzad)
		direction + 2 == previousDirection ? direction = previousDirection : 0;
		direction - 2 == previousDirection ? direction = previousDirection : 0;

		// blikani jidla
		if (timestamp - millis() < 1)
			matrix.setLed(0, foodY, foodX, 1);

		else if (timestamp - millis() < speed / 4)
			matrix.setLed(0, foodY, foodX, 0);

		else if (timestamp - millis() < speed / 2)
			matrix.setLed(0, foodY, foodX, 1);

		else if (timestamp - millis() < 3 * speed / 4)
			matrix.setLed(0, foodY, foodX, 0);
	}
}



// spocita kompletni data o zmene pohybu hada a zanese je do pole Map.
void calculateSnake() {
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



void detectBody() {
	if (age[y][x] > 3) {
		gameOver = true;
	}
}


void handleFood() {
	if (foodMap[y][x]) {
		foodMap[y][x] = 0;
		delka++;
		sezrano = true;
	}
}



void generateFood() {
	if (sezrano) {
		while (Map[foodY][foodX] == 1) {
			foodX = random(8);
			foodY = random(8);
		}

		Map[foodY][foodX] = 1;
		foodMap[foodY][foodX] = 1;
		sezrano = false;
	}
}


// postara se o spravne nastaveni "stari" rozsvicenych ledek v poli age.
void updateAge() {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (age[row][col] > delka || age[row][col] == 0) {
				age[row][col] = 0;
				Map[row][col] = 0;
			}

			if (age[row][col] != 0 && age[row][col] != -1) {
				age[row][col]++;
			}
		}
	}
}



// zpusibi objeveni hada na druhe strane obrazovky v pripade "vyjeti ven"
void fixOverflow() {
	x < 0 ? x += 8 : 0;
	x > 7 ? x -= 8 : 0;
	y < 0 ? y += 8 : 0;
	y > 7 ? y -= 8 : 0;
}



// najde konce hada. nedokoncene, zabugovane, funguje spolehlive jen poro zvisleho hada. nepouzite.
void detectEnds() {
	bool detectTerminate = false;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (Map[row][col] == 1 && detectTerminate == false) {
				y = row;
				x = col;
				detectTerminate = true;
			}

			if (Map[row][col] == 1 && detectTerminate == true) {
				// assY = row; // zastarale, pred pouzitim zkontrolovat!
				// assX = col; // zastarale, pred pouzitim zkontrolovat!
			}
		}
	}
	detectTerminate = false;
}



// prekresli obsah pole Map na fyzicky displej, zobrazuje i data z foodMap
void updateMap() {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (foodMap[row][col]) {
				matrix.setLed(0, row, col, foodMap[row][col]);
			}
			else {
				matrix.setLed(0, row, col, Map[row][col]);
			}
		}
	}
}


// // printMessage(pole, sizeof(pole[0]));

// void printMessage(const bool *arrayy, int size)
// {


// }



void wait4move() {
	while (direction == 8) {
		analogRead(Pin::joystickY) < 200 ? direction = up    : 0;
		analogRead(Pin::joystickY) > 800 ? direction = up    : 0; // dozadu se nepocita :)
		analogRead(Pin::joystickX) < 200 ? direction = left  : 0;
		analogRead(Pin::joystickX) > 800 ? direction = right : 0;

		// toto zpusobi opravdu nahodne generovani jidel
		randomSeed(millis());
	}
	foodX = random(8);
	foodY = random(8);
}


// nakresli na obrazovku obrazek z pole. nepouzite.
void draw(byte data[8]) {
	for (int i; i <= 7; i++) {
		matrix.setRow(0, i, data[i]);
	}
}







// pohodlne zanese data do pole Map, zvysi jejich vek.
void set(int x, int y, bool state) {
	Map[y][x] = state;
	age[y][x]++;
}

// jen zkratka, pro prehlednost kodu
void initialize() {
	pinMode(Pin::joystickVCC, OUTPUT);
	digitalWrite(Pin::joystickVCC, HIGH); // umele VCC pro joy

	pinMode(Pin::joystickGND, OUTPUT);
	digitalWrite(Pin::joystickGND, LOW);  // umele GND pro joy

	pinMode(Pin::joystickKEY, INPUT_PULLUP);

	Serial.begin(9600);

	matrix.shutdown(0, false);         // zapnuti matrixu
	matrix.setIntensity(0, intensity); // nastaveni jasu matrixu
	matrix.clearDisplay(0);            // smazani matrixu
}


