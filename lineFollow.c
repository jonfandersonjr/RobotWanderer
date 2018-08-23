/**********INCLUDES**********/
#include "wander.c"

/**********DATA TYPES**********/
enum brightnessSensor {
  leftSensor,
  rightSensor
};

enum color {
	black,
	white,
	grey
};

enum direction {
	dir_left,
	dir_right
};

/*********CONSTANTS*********/
const int WHITE_MIN = 85;
const int BLACK_MAX = 12;
const int TURN_TIME_MS = 20;
const int FOLLOW_SPEED = 20;
const int CORRECTION_SPEED = 4;
const int SPIN_TIME = 2700;
const int SLEEP_TIME = 10;

/*********GLOBAL VARIABLES*********/
//IMPORTANT: Theis buffer MUST be initialized with initBuffer() before usage.
buffer lightBuffer; // Already initialized in main.
buffer colorBuffer; // Already initialized in main.
#define IS_WHITE(value) (value >= WHITE_MIN)
#define IS_GREY(value) ((value) > BLACK_MAX && (value) < WHITE_MIN)
#define IS_BLACK(value) ((value) <= BLACK_MAX)
float whiteColorValue;
float blackColorValue;
float whiteLightValue;
float blackLightValue;
float leftValue;
float rightValue;

/**********FUNCTION PROTOTYPES**********/
float getBrightnessValue(brightnessSensor sensor);
color getSensorColor(brightnessSensor sensor);
void correct();
void traverseLine();
void turn(direction dir, int time_ms, int speed);
int spinAndCountLines();
void spin(int spinTime);
bool isEndOfLine();


/**********FUNCTION IMPLEMENTATIONS**********/

/* Task that starts line following */
task lineFollow() {
	while(true){
		leftValue = getBrightnessValue(leftSensor);
		rightValue = getBrightnessValue(rightSensor);
		if (IS_BLACK(leftValue) || IS_BLACK(rightValue)) {
      stopTask(wander);
      traverseLine();
    }
    sleep(5);
	}
}

/* Follows the line using the left sensor. */
void traverseLine() {
	bool endCheck = false;
	updateMotorSpeed(FOLLOW_SPEED);

	leftValue = getBrightnessValue(leftSensor);
	rightValue = getBrightnessValue(rightSensor);
	while (!endCheck) { //While not at end of line, keep correcting
		correct();
		updateMotorSpeed(FOLLOW_SPEED);
		sleep(2);
		endCheck = isEndOfLine();
	}
	playSound(soundBeepBeep);
	startTask(wander);
}

/* Corrects the robot back onto the line: turns right when white and left when black is encountered.
	The corrections by default need to be small to prevent over correction issues. */
void correct() {
	float value = getBrightnessValue(leftSensor);
	// follows the line with the left sensor using the edge (grey values).
	while(!IS_GREY(value)) {
		// correct right if white.
		if (IS_WHITE(value)) {
			turn(dir_right, TURN_TIME_MS, CORRECTION_SPEED);
			// correct left if black.
		} else if (IS_BLACK(value)) {
			turn(dir_left, TURN_TIME_MS, CORRECTION_SPEED);
		}
		value = getBrightnessValue(leftSensor);
	}
}

/* Check to see if the robot is at the end of a line by performing a full-circle spin.
   Left sensor values are taken while spinning. A value grater than 1 denotes the robot
   is not at the end of a line. */
bool isEndOfLine() {
	float leftValue = getBrightnessValue(leftSensor);
	float rightValue = getBrightnessValue(rightSensor);
	// end of line check when both sensors hit grey, then white.
	if (IS_GREY(leftValue) && IS_GREY(rightValue)) {
		repeatUntil(IS_WHITE(leftValue) && IS_WHITE(rightValue)) {
			updateMotorSpeed(FOLLOW_SPEED);
			leftValue = getBrightnessValue(leftSensor);
			rightValue = getBrightnessValue(rightSensor);
		}
	} else {
		return false;
	}
	sleep(15);
	return spinAndCountLines() < 2;
}


/*
* Returns a standardized brightness values for the specified sensor.
* Returned value will be in range [0.0; 100.0].
* Values closer to 0.0 represent black, and values closer to 100.0 represent white.
*/
float getBrightnessValue(brightnessSensor sensor) {
	switch(sensor){
		case leftSensor:
			return normalize(weightedAvg(lightBuffer), blackLightValue, whiteLightValue);
		case rightSensor:
			return normalize(weightedAvg(colorBuffer), blackColorValue, whiteColorValue);
	}
	return -100.0;
}

/* A function to return the color found from a sensor. */
color getSensorColor(brightnessSensor sensor) {
	float value = getBrightnessValue(sensor);
	if(IS_WHITE(value)){
		return white;
	}else if(IS_GREY(value)){
		return grey;
	}else{
		return black;
	}
}

/* Performs a turn given the direction, speed, and time.
   Mostly used for corrections back onto the line. */
void turn(direction dir, int time_ms, int speed) {
	if (dir == dir_left) {
		setMotorSpeed(leftMotor, -speed);
		setMotorSpeed(rightMotor, speed);
	} else if (dir == dir_right) {
		setMotorSpeed(leftMotor, speed);
		setMotorSpeed(rightMotor, -speed);
	}
	sleep(time_ms);
}

/* Performs a spin given the amount of time. The direction of the spin is
   to the right. Used for checking the end of a line. */
void spin(int spinTime) {
	setMotorSpeed(leftMotor, baseSpeed);
	setMotorSpeed(rightMotor, -baseSpeed);
	sleep(spinTime);
}

/* Performs the end of line check by spinning 360 degrees and counting the
   amount of lines encountered. */
int spinAndCountLines(){
	int lineCount = 0;
	bool isOnLine = false;
	// for loop checking in intervals the sensor values.
	for (int i = 0; i < SPIN_TIME / SLEEP_TIME; ++i) {
		color currentReading = getSensorColor(leftSensor);
		if(currentReading == black){
			isOnLine = true;
		}else if(currentReading == white){
			if(isOnLine){
				++lineCount;
			}
			isOnLine = false;
		}
		spin(SLEEP_TIME);
	}
	return lineCount;
}
