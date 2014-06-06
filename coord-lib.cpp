#include <cstdio>
#include <cmath>

#include "coord-lib.h"
extern int vol_slices;
extern int vol_radius;

/* -----------------------CARTESIAN COORDINATE POINT----------------------- */
CartPnt::CartPnt(int height_, int x_, int y_) {
	height = height_;
	x = x_;
	y = y_;
}

int CartPnt::getHeight() const {return height;}
int CartPnt::getX() const {return x;}
int CartPnt::getY() const {return y;}

void CartPnt::setHeight(int height_) {height = height_;}
void CartPnt::setX(int x_) {x = x_;}
void CartPnt::setY(int y_) {y = y_;}

void CartPnt::printCoord() const {
	printf("(Cartesian) height: %d, x: %d, y: %d", height, x, y);
}

// Convets Cartesian Coordinates to Polar Coordinates
PolPnt * CartPnt::toPolar() const {
	int slice = (int)(atan2(y,x) * 180 / M_PI) * (vol_slices/360.0);
	if(slice < 0) slice += vol_slices;
	int radius = (int)round((sqrt(pow(x,2)+pow(y,2))));
	
	PolPnt *p = new PolPnt(height, slice, radius);
	return p;
}

/* -------------------------POLAR COORDINATE POINT------------------------- */
PolPnt::PolPnt(int height_, int slice_, int radius_) {
	height = height_;
	slice = slice_;
	radius = radius_;
	deg = (int)slice * (360.0/vol_slices);
}

int PolPnt::getHeight() const {return height;}
int PolPnt::getSlice() const {return slice;}
int PolPnt::getDeg() const {return deg;}
int PolPnt::getRadius() const {return radius;}

void PolPnt::setHeight(int height_) {height = height_;}
void PolPnt::setSlice(int slice_) {slice = slice_;}
void PolPnt::setRadius(int radius_) {radius = radius_;}

void PolPnt::printCoord() const {
	printf("(Polar) height: %d, slice: %d, deg: %d, radius: %d", height, slice, deg, radius);
}

int PolPnt::toData() const {
	return (int) pow(2, vol_radius - (radius-1));
}