#pragma config(Sensor, S3,     ,               sensorEV3_Color)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/**********INCLUDES**********/
#include "sonar.c"

/************CONSTANTS**************/
const int READ_WAIT_TIME = 10; /*Wait time between each sensor read in ms.*/
const int BUFFER_SIZE = 12;
#define SONAR S4
#define LIGHT S2
#define COLOR S3


/*******FUNCTION PROTOYPES********************/
task populateSensorValues();
void calibrateSensors();
float getColor(brightnessSensor sensor);
float getDistance();


/* Main entry of the program. Calibrates sensors, populates buffers, and starts tasks. */
task main() {
	setSoundVolume(20);
	calibrateSensors();

	// starts population of sensor buffers.
	startTask(populateSensorValues);
	sleep(500);

	// starts wasnder, lineFollow, and sonar.
	startTask(wander);
	sleep(500);
	startTask(lineFollow);
	sleep(500);
	startTask(sonarSensor);
	eraseDisplay();
	while(true){
		abortTimeslice();
	}

}


/* IMPORTANT: If this tasks is being turned off and on the initBuffer
	calls may need to be removed/though about.*/
task populateSensorValues() {
	initBuffer(lightBuffer, BUFFER_SIZE);
	initBuffer(colorBuffer, BUFFER_SIZE);
	initBuffer(sonarBuffer, BUFFER_SIZE);
	while(true) {
		push(lightBuffer, SensorValue[LIGHT]);
		push(colorBuffer, SensorValue[COLOR]);
		push(sonarBuffer, getUSDistance(SONAR));
		sleep(READ_WAIT_TIME);
	}
}

/* Calibrates sensors on button presses.
	 Calibration order: WHITE, BLACK, MAX DISTANCE, MIN DISTANCE. */
static void calibrateSensors() {

	// Color/light sensor calibration
	waitForButtonPress();
	flushButtonMessages();
	eraseDisplay();
	whiteColorValue = getColor(rightSensor);
	displayFloat(whiteColorValue);
	whiteLightValue = getColor(leftSensor);
	displayFloat(whiteLightValue);
	waitForButtonPress();
	flushButtonMessages();
	eraseDisplay();
	blackColorValue = getColor(rightSensor);
	displayFloat(blackColorValue);
	blackLightValue = getColor(leftSensor);
	displayFloat(blackLightValue);
	waitForButtonPress();
	flushButtonMessages();


	//run sonar sensor calibration
	observableDistance = getDistance();
	displayFloat(observableDistance);
	waitForButtonPress();
	flushButtonMessages();
	stopDistance = getDistance();
	displayFloat(stopDistance);
	waitForButtonPress();
	flushButtonMessages();

}

/**********CALIBRATION FUNCTIONS**********/

/* Retrieves the color value from the specified sensor for calibration. */
int getColor(brightnessSensor sensor) {
	float buffer[5];
	int i;
	// left sensor value
	if (sensor == leftSensor) {
		for (i = 0; i < 5; i++) {
			buffer[i] = SensorValue[LIGHT];
			sleep(10);
		}
		// right sensor value
	} else {
		for (i = 0; i < 5; i++) {
			buffer[i] = SensorValue[COLOR];
			sleep(10);
		}
	}
	// median used for outlier removal.
	sort(buffer, 5);
	return findMedian(buffer, 0, 5);
}

/* Get distance for sonar calibration. */
float getDistance() {
	float buffer[5];
	int i;
	for (i = 0; i < 5; i++) {
		buffer[i] = getUSDistance(SONAR);
		sleep(10);
	}
// median used for outlier removal.
	sort(buffer, 5);
	return findMedian(buffer, 0, 5);
}
