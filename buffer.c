/**********CONSTANTS**********/
static const int MAX_SIZE = 200;

/**********DATA TYPES**********/
/*
* A circular array data struct.
* Position indicates index of the last value added to the buffer.
*/
typedef struct {
	float values[MAX_SIZE];
	int size;
	int position; /* The location of the last read value.*/
} buffer;

/**********FUNCTION PROTOTYPES**********/
/*Initializes the buffer with the desired size.*/
void initBuffer(buffer bf, int size);
void push(buffer bf, const float value);
float getNthValue(buffer bf, int n);
void printBuffer(buffer bf);

/**********FUNCTION IMPLEMENTATIONS**********/

/* Initializes an inputed buffer */
void initBuffer(buffer bf, int size) {
	bf.position = -1;
	bf.size = size;
}

/* Adds a value to the end of the buffer */
void push(buffer bf, const float value) {
	++bf.position;
	bf.position %= bf.size;
	bf.values[bf.position] = value;
}

/* Retrieves a value at position n, starting at the tail.*/
float getNthValue(buffer bf, int n) {
	return bf.values[(bf.position + n) % bf.size];
}

/* Prints the contents of the buffer. */
void printBuffer(buffer bf) {
	for (int i = 0; i < bf.size; ++i) {
		if (i == bf.position) {
			writeDebugStream("[%f], ", bf.values[i]);
		} else {
			writeDebugStream("%f, ", bf.values[i]);
		}
	}
}
