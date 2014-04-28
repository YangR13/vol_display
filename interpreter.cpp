#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "library.h"

#define maxRadius 16	// not counting center point
#define maxHeight 16

#define FILENAME "pattern1.txt"	// temporary; file selection will be added later

int getSize(char *filename);
int readFileAndConvert(char *filename, CartPnt **cArr, PolPnt **pArr);
void printCoordinates(int numCoords, CartPnt **cArr, PolPnt **pArr);

//TODO LIST
void sendSerialData();

// MAIN FUNCTION
int main() {
	int size, numCoords;	// size includes both valid and invalid coordinates; numCoords only includes valid coordinates
	CartPnt **cArr;
	PolPnt **pArr;
	
	// file selection would occur here
	char *filename = (char *)FILENAME;
	
	size = getSize(filename);
	cArr = new CartPnt *[size];
	pArr = new PolPnt *[size];
	
	numCoords = readFileAndConvert(filename, cArr, pArr);
	printCoordinates(numCoords, cArr, pArr);
	
	return 0;
}

// GET APPROXIMATE SIZE (ROUNDED UP) OF CART AND POL COORDINATE ARRAYS
int getSize(char *filename) {
	FILE *fp = fopen(filename, "r");
	
	int size = 0;
	char parser;
	while(fscanf(fp, " %c", &parser) == 1)
		if(parser == 'x') size++;
	
	return size;
}

// READS PATTERN FILE AND CONVERTS TO CARTESIAN AND POLAR CORDINATES
int readFileAndConvert(char *filename, CartPnt **cArr, PolPnt **pArr) {
	FILE *fp = fopen(filename, "r");
	
	int numCoords, height, i, j;
	char parser;
		
	while(!feof(fp)) {
		fscanf(fp, "%d", &height);
		for(j = maxRadius; j >= maxRadius*-1; j--) {
			for(i = maxRadius*-1; i <= maxRadius; i++) {
				fscanf(fp, " %c", &parser);
				if(parser != '.' && round(sqrt(pow(i, 2)+pow(j,2))) <= maxRadius)	//IF MARKED AND WITHIN MAXIMUM RADIUS
					cArr[numCoords++] = new CartPnt(height, i, j);
			}
		}
	}
	for(int i=0; i<numCoords; i++) pArr[i] = cArr[i]->toPolar();
	
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
