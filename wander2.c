/**********INCLUDES**********/
#include "utils.c"

/**********DATA TYPES**********/
typedef struct{
	int left;     //Probability of moving left
	int straight; //Probability of moving straight
	int right;    //Probability of moving right
} movesProbabilities;
movesProbabilities wanderProbabilities;

/*********GLOBAL VARIABLES*********/
const int turnSpeedIncrementAmount = 4;
const int turnSpeedIncrementCount = 6;
const int turnIncrementCountVariability = 3;
const int turnIncrementDuration = 45;

//temporary
const int communicationLag = 30;

const int straightMoveDuration = 350;
const int moveLeftOrRightStraightDuration = 60;

/**********FUNCTION PROTOTYPES**********/
task wander();
void resetWanderProbabilities();
void moveLeft();
void moveStraight();
void moveRight();


/**********FUNCTION IMPLEMENTATIONS**********/
task wander(){
	resetWanderProbabilities();
	while(true){
		int roll = abs(rand() % 184) + 1;
		if(roll < wanderProbabilities.left){ //Move LEFT
			wanderProbabilities.left -= 25;
			wanderProbabilities.right += 25;
			moveLeft();
		}else if(roll < (wanderProbabilities.left + wanderProbabilities.straight)){  //Move STRAIGHT
			//No adjustment of moving straight's probability
			moveStraight();
		}else{ //Move RIGHT
			wanderProbabilities.right -= 25;
			wanderProbabilities.left += 25;
			moveRight();
		}

		sleep(20);
	}
	//Wanders constantly
}

/* Resets wander probabilities to their default values. */
void resetWanderProbabilities(){
	wanderProbabilities.left = 75;
	wanderProbabilities.straight = 35;
	wanderProbabilities.right = 75;
	setMotorSpeed(leftMotor,0);
	setMotorSpeed(rightMotor,0);
}

/* Moves the robot in the left direction */
void moveLeft(){
	int roll = rand() % turnIncrementCountVariability;
	int incrementCount = turnSpeedIncrementCount + roll;
	for(int i = 0; i < incrementCount; ++i){
		setMotorSpeed(rightMotor, baseSpeed + turnSpeedIncrementAmount*i);
		sleep(communicationLag);
		setMotorSpeed(leftMotor, baseSpeed);
		sleep(turnIncrementDuration);
}

	for(int i = 0; i < turnSpeedIncrementCount + roll; ++i){
		setMotorSpeed(rightMotor, baseSpeed + incrementCount * turnSpeedIncrementAmount - turnSpeedIncrementAmount*i);
		sleep(communicationLag);
		setMotorSpeed(leftMotor, baseSpeed);
		sleep(turnIncrementDuration);
	}

	sleep(moveLeftOrRightStraightDuration);
}

/* Moves the robot in  straight direction */
void moveStraight(){
	setMotorSpeed(rightMotor, baseSpeed);
	sleep(communicationLag);
	setMotorSpeed(leftMotor, baseSpeed);
	sleep(straightMoveDuration);
}

/* Moves the robot in the right direction */
void moveRight(){
	int roll = rand() % turnIncrementCountVariability;
	int incrementCount = turnSpeedIncrementCount + roll;
	for(int i = 0; i < incrementCount; ++i){
		setMotorSpeed(rightMotor, baseSpeed);
		sleep(communicationLag);
		setMotorSpeed(leftMotor, baseSpeed + turnSpeedIncrementAmount*i);
		sleep(turnIncrementDuration);
	}

	for(int i = 0; i < turnSpeedIncrementCount; ++i){
		setMotorSpeed(rightMotor, baseSpeed);
		sleep(communicationLag);
		setMotorSpeed(leftMotor, baseSpeed + incrementCount * turnSpeedIncrementAmount - turnSpeedIncrementAmount * i);
		sleep(turnIncrementDuration);
	}

	sleep(moveLeftOrRightStraightDuration);
}