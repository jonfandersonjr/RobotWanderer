/**********INCLUDES**********/
#include "lineFollow.c"

/*********GLOBAL VARIABLES*********/
//IMPORTANT: Theis buffer MUST be initialized with initBuffer() before usage.
buffer sonarBuffer;

float observableDistance;
float stopDistance;

/*******PROTOYPES********************/
task sonarSensor();
void discoverObject();
void turnAwayFromObject();
float getSonarDistance();

/**********FUNCTION IMPLEMENTATIONS**********/

/* Task that starts object detection */
task sonarSensor(){
	while(true){
		// robot finds an object in its range.
		discoverObject();
		startTask(wander);
		startTask(lineFollow);
		sleep(50);
	}
}

/* Backup from object and turn either left or right away from it. */
void turnAwayFromObject() {
	updateMotorSpeed(-baseSpeed); // backup from object.
	sleep(1000);

	//choose left or right turn at random
	if ((abs(rand()) % 2) == 1) { // left turn
		setMotorSpeed(leftMotor, -baseSpeed);
		setMotorSpeed(rightMotor, baseSpeed);
	}
	else { //right turn
		setMotorSpeed(leftMotor, baseSpeed);
		setMotorSpeed(rightMotor, -baseSpeed);
	}
	sleep(650 + (abs(rand()) % 350));
}

/* Move towards object until within specified range (1 in) or object disappears. */
void discoverObject() {
	float distanceRatio = 1;
	float objectDistance = getSonarDistance();
	while (objectDistance > stopDistance
			&& objectDistance < observableDistance) { //should take avg distance away from buffer
		distanceRatio = (objectDistance / observableDistance); //should take avg distance away from buffer
		updateMotorSpeed((int) floor(maxSpeed * distanceRatio));
    objectDistance = getSonarDistance();
	}
	//we successfully got within distance
	if (getSonarDistance() <= stopDistance + 2) {
		updateMotorSpeed(0);
		sleep(2000);
		turnAwayFromObject();
	}
}

/* Gets the distance of the object from the robot. */
float getSonarDistance() {
	return weightedAvg(sonarBuffer);
}
