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

struct Point {
	int row, col;
};

// LED matrix brightness between 0(darkest) and 15(brightest)
const short intensity = 8;


int delka = 3;
int speed = 300;
bool sezrano = true;
bool win = false;
bool gameOver = false;
bool dontShowIntro = false;

// primarni souradnice hada
int x = 0;
int y = 0;

Point food;

int direction = -1;
const short up     = 0;
const short right  = 1;
const short down   = 2;
const short left   = 3;

// pole age - urcuje vek kazde bunky (ledky). podle toho se pak temer automaticky zhasinaji
// bunky za hadem.
int age[8][8] = {};


const PROGMEM bool snejkMessage[8][56] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

const PROGMEM bool gameOverMessage[8][84] = {
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

void(* restart) (void) = 0; // restartovaci funkce TODO: Remove


void setup() {
	Serial.begin(115200);
	initialize();
	showSnakeMessage();
	wait4move();
}


void loop() {
	generateFood();
	handleGameStates();
	scanJoystick();   // sleduje pohyb joystku, obstarava i cekani mezi "snimky"
	calculateSnake(); // vypocet parametru hada
	//updateMap();      // aktualizace matrixu

	// uncomment this if you want the current game board to be printed to the serial
	// dumpGameBoard();
}




void dumpGameBoard() {
	String buff = "\n\n\n";
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (age[row][col] < 10) buff += " ";
			if (age[row][col] != 0) buff += age[row][col];
			else if (col == food.col && row == food.row) buff += "@";
			else buff += "-";
			buff += " ";
		}
		buff += "\n";
	}
	Serial.println(buff);
}




void handleGameStates() {
	if (gameOver) {

		unrollSnake();

		showGameOverMessage();

		while (digitalRead(Pin::joystickKEY)) {}
		restart();

		while (1) {
			Serial.println("never happens");
		}

		//gameOver = false;
		//break;

	}

	if (digitalRead(Pin::joystickKEY) && !gameOver && !dontShowIntro) {
//		for (int d = 0; d < sizeof(snejkMessage[0]) - 7; d++) {
//			for (int col = 0; col < 8; col++) {
//				delay(messageSpeed);
//				for (int row = 0; row < 8; row++) {
//					// this reads the byte from the PROGMEM and displays it on the screen
//					matrix.setLed(0, row, col, pgm_read_byte(&(snejkMessage[row][col + d])));
//				}
//			}
//		}
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
		speed = map(analogRead(Pin::potentiometer), 0, 1023, 20, 1000);

		// zjistovani smeru
		analogRead(Pin::joystickY) < 200 ? direction = up    : 0;
		analogRead(Pin::joystickY) > 800 ? direction = down  : 0;//10
		analogRead(Pin::joystickX) < 200 ? direction = left  : 0;//11
		analogRead(Pin::joystickX) > 800 ? direction = right : 0;

		// znemozneni zmeny smeru o 180 stupnu (celem vzad)
		direction + 2 == previousDirection ? direction = previousDirection : 0;
		direction - 2 == previousDirection ? direction = previousDirection : 0;

		if (millis() % 100 < 50) {
			matrix.setLed(0, food.row, food.col, 1);
		} else {
			matrix.setLed(0, food.row, food.col, 0);
		}

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





}

// pohodlne zanese data do pole Map, zvysi jejich vek.
void set(int x, int y, bool state) {
//	Map[y][x] = state;
	matrix.setLed(0, y, x, state);

	// TODO: check that
	if (age[y][x] > 3) {
		gameOver = true;
		return;
	}

	handleFood();

	updateAges();
	age[y][x]++;
}

void handleFood() {
//	foodMap[food.row][food.col]
	if (x == food.col && y == food.row) {
//		foodMap[y][x] = 0;
		delka++;
		sezrano = true;
	}
}

// postara se o spravne nastaveni "stari" rozsvicenych ledek v poli age.
void updateAges() {
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {

			// vyjimka pro aktualni bod (hlavu hada)
			if (row == y && col == x) continue;


			if (age[row][col] > 0 ) {
				age[row][col]++;
			}

			if (age[row][col] > delka || age[row][col] == 0) {
				age[row][col] = 0;
				matrix.setLed(0, row, col, 0);
			}

		}
	}
}

void generateFood() {
	if (sezrano) {
		if (delka >= 64) {
			win = true;
			return;
		}

		while (age[food.row][food.col] > 0) {
			food.col = random(8);
			food.row = random(8);
		}

//		foodMap[food.row][food.col] = 1;
		sezrano = false;
	}
}


// zpusibi objeveni hada na druhe strane obrazovky v pripade "vyjeti ven"
void fixOverflow() {
	x < 0 ? x += 8 : 0;
	x > 7 ? x -= 8 : 0;
	y < 0 ? y += 8 : 0;
	y > 7 ? y -= 8 : 0;
}


// prekresli obsah pole Map na fyzicky displej, zobrazuje i data z foodMap
//void updateMap() {
//	for (int row = 0; row < 8; row++) {
//		for (int col = 0; col < 8; col++) {
//			if (foodMap[row][col]) {
//				matrix.setLed(0, row, col, foodMap[row][col]);
//			}
//			else {
//				matrix.setLed(0, row, col, Map[row][col]);
//			}
//		}
//	}
//}




void unrollSnake() {

//		delay(250);


//	for (int i = 0; i < delka; i++) {
//		Serial.println(i);
//	}
//	Serial.println();
//	Serial.println();
//	Serial.println();
//	Serial.println("ageDump:");
//
//
//


}


// TODO: fix!!
void wait4move() {
	while (direction == -1) {
		analogRead(Pin::joystickY) < 200 ? direction = up    : 0;
		analogRead(Pin::joystickY) > 800 ? direction = up    : 0; // dozadu se nepocita :)
		analogRead(Pin::joystickX) < 200 ? direction = left  : 0;
		analogRead(Pin::joystickX) > 800 ? direction = right : 0;

		// toto zpusobi opravdu nahodne generovani jidel
		randomSeed(millis());
	}
	
	food.row = random(8);
	food.col = random(8);
}



// jen zkratka, pro prehlednost kodu
void initialize() {
	pinMode(Pin::joystickVCC, OUTPUT);
	digitalWrite(Pin::joystickVCC, HIGH); // umele VCC pro joy

	pinMode(Pin::joystickGND, OUTPUT);
	digitalWrite(Pin::joystickGND, LOW);  // umele GND pro joy

	pinMode(Pin::joystickKEY, INPUT_PULLUP);

	matrix.shutdown(0, false);         // zapnuti matrixu
	matrix.setIntensity(0, intensity); // nastaveni jasu matrixu
	matrix.clearDisplay(0);            // smazani matrixu
}








void showSnakeMessage() {
	for (int d = 0; d < sizeof(snejkMessage[0]) - 7; d++) {
		for (int col = 0; col < 8; col++) {
			delay(messageSpeed);
			for (int row = 0; row < 8; row++) {
				// this reads the byte from the PROGMEM and displays it on the screen
				matrix.setLed(0, row, col, pgm_read_byte(&(snejkMessage[row][col + d])));
			}
		}
	}
}

void showGameOverMessage() {
	for (int d = 0; d < sizeof(gameOverMessage[0]) - 7; d++) {
		for (int col = 0; col < 8; col++) {
			delay(messageSpeed);
			for (int row = 0; row < 8; row++) {
				// this reads the byte from the PROGMEM and displays it on the screen
				matrix.setLed(0, row, col, pgm_read_byte(&(gameOverMessage[row][col + d])));
			}
		}
	}
}


void showWinMessage() {
}









