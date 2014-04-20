#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define maxRadius 16
#define maxCoord 16

// MISC definitions
#define PI 3.14159265

typedef struct CartPnt_ {
	int height;
	int x;
	int y;
} CartPnt;

typedef struct PolPnt_ {
	int height;
	int deg;
	int r;
} PolPnt;

CartPnt * ReadCoordsFromFile(char *filename);
PolPnt CartToPol(CartPnt c);

// List of Cartesian and Polar Coordinates
int *CartPntArr;
int *PolPntArr;

// MAIN FUNCTION
int main() {
	int i;

	CartPnt *cArr = ReadCoordsFromFile("pattern1.txt");
	
	for(i=0;i<64;i++) {
		printf("Printing Cartesian Coordinates:\n");
		printf("h: %d, x: %d, y: %d\n", cArr[i].height, cArr[i].x, cArr[i].y);
	}
	
	return 0;
}

// -----WIP-----
// Opens Patern File and reads Cartesian Coordinates
CartPnt * ReadCoordsFromFile(char *filename) {
	FILE *fp;
	
	if((fp = fopen(filename, "r")) == NULL) printf("FILE NOT OPENED");
	
	// NEEDS TO BE SELF EXPANDING
	CartPnt *cArr = (CartPnt *)malloc(128*sizeof(CartPnt));
	int cArrSize = 0;
	int height, i, j;
	char parser;
	CartPnt tempPnt;
	while(!feof(fp)) {
		fscanf(fp, "%d", &height);
		
		for(j = maxCoord*-1; j <= maxCoord; j++) {
			for(i = maxCoord*-1; i <= maxCoord; i++) {
				fscanf(fp, " %c", &parser);
				if(parser != '.') {
				
					tempPnt.height = height;
					tempPnt.x = i;
					tempPnt.y = j;
				
					cArr[cArrSize++] = tempPnt;
				}
			}
		}
		
	}
	
	return cArr;
}

// Convets Cartesian Coordinates to Polar Coordinates
PolPnt CartToPol(CartPnt c) {
	PolPnt p;
	p.height = c.height;
	p.deg = (int)(atan2(c.y,c.x) * 180 / PI);
	p.r = (int)(sqrt(pow(c.x,2)+pow(c.y,2)));
	return p;
}

