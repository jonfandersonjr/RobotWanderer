/**********INCLUDES**********/
#include "buffer.c"

/**********CONSTANTS**********/
const int baseSpeed = 30;
const int maxSpeed = 90;

/*********GLOBAL VARIABLES*********/
#define leftMotor motorB
#define rightMotor motorC
int line = 0;

/**********FUNCTION PROTOTYPES**********/
void displayFloat(float value);
float weightedAvg(buffer bf);
float getWeightLinear(int index);
void sort(float * buffer, int size);
int findMedian(float * buffer, int beginIndex, int endIndex);
float max(float a, float b);
float min(float a, float b);
/*Normalizes value from [minV, maxV] to [0, 100]*/
float normalize(int value, int minV, int maxV);
void updateMotorSpeed(int theSpeed);
task lineFollow();
task sonarSensor();

/**********FUNCTION IMPLEMENTATIONS**********/

/* Displays a value to the robot screen */
void displayFloat(float value) {
	//eraseDisplay();
	displayCenteredBigTextLine(line++++,"%f", value);
}

/*Gets the weight value for the index weight.*/
float getWeightLinear(int index) {
	return index;
}

/* Calculates the weighted average. */
float weightedAvg(buffer bf) {
    float weightedAverage = 0.0;
    float weightsSum = 0.0;
    for (int i = bf.size; i > 0; i--) {
    		float currentWeight = getWeightLinear(i);
    		weightsSum += currentWeight;
    		weightedAverage += bf.values[(i + bf.position) % bf.size] * currentWeight;
    }
    return weightedAverage / weightsSum;
}
/* Sorts the data in ascending order using selection sort. */
void sort(float * buffer, int size) {
    int i, j, minIndex;

    for (i = 0; i < size; i++) {
        minIndex = i;
        for (j = i + 1; j < size - i; j++) {
            if (buffer[minIndex] > buffer[j]) {
                minIndex = j;
            }
        }
        float temp = buffer[i];
        buffer[i] = buffer[minIndex];
        buffer[minIndex] = temp;
    }
}
/* Finds the median in a given data set betweenthe following indices. */
int findMedian(float * buffer, int beginIndex, int endIndex) {
    int median;
    int size = endIndex - beginIndex + 1;
    if (size % 2) {
        median = buffer[beginIndex + (size / 2)];
    } else {
        median = (buffer[beginIndex + (size / 2) - 1] + buffer[beginIndex + (size / 2)]) / (float) 2;
    }
    return median;
}

/* Returns the max from two given inputs. */
float max(float a, float b) {
	if(a > b) {
		return a;
	} else {
		return b;
	}
}

/* Returns the min from two given variables. */
float min(float a, float b){
	if(a < b) {
		return a;
	} else {
		return b;
	}
}

/* Normalizes the input value to a range from 0 - 100 */
float normalize(int value, int minV, int maxV){
	float range = maxV - minV;
	value = max(min(value, maxV), minV);
	value -= minV;
	value *= 100.0 / range;
	return value;
}

/* Updates the speed of both motors. */
void updateMotorSpeed(int theSpeed) {
	setMotorSpeed(leftMotor, theSpeed);
	setMotorSpeed(rightMotor, theSpeed);
}
