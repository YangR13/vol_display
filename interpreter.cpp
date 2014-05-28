#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unistd.h>   // UNIX standard function definitions 
#include <cstdint>

#include "coord-lib.h"
#include "arduino-serial-lib.h"

#define CONFIGFILE "config.txt"

int scanConfigFile();
int scanPatternFile(FILE *fp, CartPnt *(**cArr), PolPnt *(**pArr));
void printCoordinates(int numCoords, CartPnt **cArr, PolPnt **pArr);

uint16_t **createSerialData(int numCoords, PolPnt **pArr);
void printSerialData(uint16_t **serialData);
void sendSerialData(uint16_t **serialData);

// TODO LIST

// GLOBAL VARIABLES
char FILENAME[16];
int maxRadius;	// not counting center point
int maxHeight;

char serialport[32];
int baudrate;

// MAIN FUNCTION
int main() {
	FILE *fp;
	int numCoords;
	CartPnt **cArr;
	PolPnt **pArr;
	uint16_t **serialData;
	
	if(scanConfigFile()) {
		printf("ERROR: Could not read Config file\n\n");
		return 1;
	}
	
	// file selection would occur here
	char patternfile[32] = "patterns/";
	strcat(patternfile, FILENAME);

	if((fp = fopen(patternfile, "r")) == NULL) {
		printf("ERROR: File \"%s\" could not be opened\n\n", patternfile);
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
uint16_t **createSerialData(int numCoords, PolPnt **pArr) {
	
	// creates 2D array to hold bytes
	uint16_t **serialData = new uint16_t*[360];
	for(int i=0; i<360; i++)
		serialData[i] = new uint16_t[maxHeight];

	// fills 2D array with byte values from pArr
	for(int i=0; i<numCoords; i++) {
		serialData[pArr[i]->getDeg()][pArr[i]->getHeight()] |= pArr[i]->toData();
	}
	return serialData;
}

void printSerialData(uint16_t **serialData) {
	for(int i=0; i<360; i++) {
		for(int j=0; j<maxHeight; j++) {
			if(serialData[i][j] != 0) printf("Data at deg %d and height %d is: %d\n", i, j, serialData[i][j]);
		}
	}
}


void sendSerialData(uint16_t **serialData) {
	int bytecount = 0;
	int fd = serialport_init(serialport, baudrate);
	if(fd == -1) return;
	
	printf("Sending serial data...\n");
	for(int i=0; i<360; i++) {
		for(int j=0; j<maxHeight; j++) {
			bytecount += write(fd, &serialData[i][j], 2);
		}
	}
	printf("Data transfer complete\n");
	printf("Bytes transfered: %d\n", bytecount);
}

