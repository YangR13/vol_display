#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include "library.h"

#define maxRadius 16	// not counting center point
#define maxHeight 16

#define FILENAME "pattern1.txt"	// temporary; file selection will be added later

int scanFileAndConvert(FILE *fp, CartPnt *(**cArr), PolPnt *(**pArr));
void printCoordinates(int numCoords, CartPnt **cArr, PolPnt **pArr);

int ** createSerialData(int numCoords, PolPnt **pArr);
void printSerialData(int **serialData);

//TODO LIST
void sendSerialData(int **serialData);


// MAIN FUNCTION
int main() {
	int size, numCoords;	// size includes both valid and invalid coordinates; numCoords only includes valid coordinates
	CartPnt **cArr;
	PolPnt **pArr;
	int **serialData;
	
	// file selection would occur here
	char *filename = (char *)FILENAME;
	FILE *fp = fopen(filename, "r");
	
	numCoords = scanFileAndConvert(fp, &cArr, &pArr);
	printCoordinates(numCoords, cArr, pArr);
	
	serialData = createSerialData(numCoords, pArr);
	printSerialData(serialData);
	
	return 0;
}

// READS PATTERN FILE AND CONVERTS TO CARTESIAN AND POLAR CORDINATES
int scanFileAndConvert(FILE *fp, CartPnt *(**cArr), PolPnt *(**pArr)) {
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
int ** createSerialData(int numCoords, PolPnt **pArr) {
	int **serialData = new int*[360];

	for(int i=0; i<360; i++)
		serialData[i] = new int[maxHeight];

	for(int i=0; i<numCoords; i++) {
		serialData[pArr[i]->getDeg()][pArr[i]->getHeight()]+=pArr[i]->toData();
	}
	return serialData;
}

void printSerialData(int **serialData) {
	for(int i=0; i<360; i++) {
		for(int j=0; j<maxHeight; j++) {
			if(serialData[i][j] != 0) printf("Data at deg %d and height %d is: %d\n", i, j, serialData[i][j]);
		}
	}
	
}
