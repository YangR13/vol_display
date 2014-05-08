#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unistd.h>   // UNIX standard function definitions 

#include "coord-lib.h"
#include "arduino-serial-lib.h"

#define CONFIGFILE "config.txt"

int scanConfigFile();
int scanPatternFile(FILE *fp, CartPnt *(**cArr), PolPnt *(**pArr));
void printCoordinates(int numCoords, CartPnt **cArr, PolPnt **pArr);

unsigned char ** createSerialData(int numCoords, PolPnt **pArr);
void printSerialData(unsigned char ** serialData);
void sendSerialData(unsigned char **serialData);

// TODO LIST

// GLOBAL VARIABLES
char FILENAME[16];
int maxRadius;	// not counting center point
int maxHeight;

char serialport[32] = "/dev/tty.usbmodem1411";
int baudrate;

// MAIN FUNCTION
int main() {
	FILE *fp;
	int size, numCoords;	// size includes both valid and invalid coordinates; numCoords only includes valid coordinates
	CartPnt **cArr;
	PolPnt **pArr;
	unsigned char **serialData;
	
	if(scanConfigFile()) {
		printf("ERROR: Could not read Config file\n\n");
		return 1;
	}
	
	// file selection would occur here
	char *filename = (char *)FILENAME;

	if((fp = fopen(filename, "r")) == NULL) {
		printf("ERROR: File \"%s\" could not be opened\n\n", filename);
		return 1;
	}
	
	numCoords = scanPatternFile(fp, &cArr, &pArr);
	printCoordinates(numCoords, cArr, pArr);
	
	serialData = createSerialData(numCoords, pArr);
	printSerialData(serialData);
	
	sendSerialData(serialData);
	
	return 0;
}

int scanConfigFile() {
	FILE *fp = fopen(CONFIGFILE, "r");
	char parser[32];
	
	int intBuffer;
	char strBuffer[32];

	while(!feof(fp)) {
		fscanf(fp, "%s ", parser);
		if(strcmp(parser, "FILE:") == 0) {
			fscanf(fp, "%s ", FILENAME);
		}
		if(strcmp(parser, "maxHeight:") == 0) {
			if(fscanf(fp, "%d ", &maxHeight) != 1) return 1;
		}
		if(strcmp(parser, "maxRadius:") == 0) {
			if(fscanf(fp, "%d ", &maxRadius) != 1) return 1;
		}
		if(strcmp(parser, "serialport:") == 0) {
			fscanf(fp, "%s ", serialport);
		}
		if(strcmp(parser, "baudrate:") == 0) {
			if(fscanf(fp, "%d ", &baudrate) != 1) return 1;
		}
	}
	return 0;
}

// READS PATTERN FILE AND CONVERTS TO CARTESIAN AND POLAR CORDINATES
int scanPatternFile(FILE *fp, CartPnt *(**cArr), PolPnt *(**pArr)) {
	char type[16];
	int numCoords = 0;
	int height, x, y;
	char parser;
	
	fscanf(fp, "%s", type);
	
	if(strcmp(type, "TOPDOWN") == 0|| strcmp(type, "FRONTBACK") == 0) {
		// initializing cArr to maximum possible number of coordinates
		*cArr = new CartPnt *[maxHeight * (int)pow(2*maxRadius+1, 2)];
		while(!feof(fp)) {
			if(strcmp(type, "TOPDOWN") == 0) {
				fscanf(fp, "%d", &height);
				for(y = maxRadius; y >= maxRadius*-1; y--) {
					for(x = maxRadius*-1; x <= maxRadius; x++) {
						fscanf(fp, " %c", &parser);
						if(parser == 'E') break;
						if(parser == '0' && round(sqrt(pow(x, 2)+pow(y,2))) <= maxRadius)	//IF MARKED AND WITHIN MAXIMUM RADIUS
							(*cArr)[numCoords++] = new CartPnt(height, x, y);
					}
				}
			}
			if(strcmp(type, "FRONTBACK") == 0) {			
				fscanf(fp, "%d", &y);
				for(height = maxHeight-1; height >= 0; height--) {
					for(x = maxRadius*-1; x <= maxRadius; x++) {
						fscanf(fp, " %c", &parser);
						if(parser == 'E') break;
						if(parser == '0' && round(sqrt(pow(x, 2)+pow(y,2))) <= maxRadius)	//IF MARKED AND WITHIN MAXIMUM RADIUS
							(*cArr)[numCoords++] = new CartPnt(height, x, y);
					}
				}
			}
			
		}
		*pArr = new PolPnt *[numCoords];
		for(int i=0; i<numCoords; i++) (*pArr)[i] = (*cArr)[i]->toPolar();
	}
	if(strcmp(type, "OUTSIDEIN") == 0) {
	
	}
	

	
	return numCoords;
}

// PRINTS CARTESIAN AND POLAR COORDINATES
void printCoordinates(int numCoords, CartPnt **cArr, PolPnt **pArr) {
	for(int i=0; i<numCoords; i++) {
		cArr[i]->printCoord();
		printf(" | ");
		pArr[i]->printCoord();
		printf("\n\n");
	}
	
	printf("Number of coordinates: %d\n\n", numCoords);
}


// READS POLAR COORDINATES AND CONVERTS TO DATA TO SEND TO ARDUINO
unsigned char ** createSerialData(int numCoords, PolPnt **pArr) {
	
	// creates 2D array to hold bytes
	unsigned char **serialData = new unsigned char*[360];
	for(int i=0; i<360; i++)
		serialData[i] = new unsigned char[maxHeight*2];

	// fills 2D array with byte values from pArr
	for(int i=0; i<numCoords; i++) {
		int deg = pArr[i]->getDeg();
		
		// assigns byte spaces within each degree, at maxRadius/8 spaces per height space
		int byteIndex = pArr[i]->getHeight()*(maxRadius/8) + (pArr[i]->getRadius()-1)/8;
		
		// shifts down data if data exceeds capacity of unsigned char
		unsigned char data = (pArr[i]->toData()) >> (((pArr[i]->getRadius()-1)/8)*8);

		serialData[deg][byteIndex] |= data;
	}
	return serialData;
}

void printSerialData(unsigned char **serialData) {
	for(int i=0; i<360; i++) {
		for(int j=0; j<maxHeight*2; j++) {
			if(serialData[i][j] != 0) printf("Data at deg %d and byteIndex %d is: %d\n", i, j, serialData[i][j]);
		}
	}
}


void sendSerialData(unsigned char **serialData) {
	int fd = serialport_init(serialport, baudrate);
	for(int i=0; i<360; i++) {
		write(fd, serialData[i], 360 * maxHeight * (maxRadius/8));
	}
}

