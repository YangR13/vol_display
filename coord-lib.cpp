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
	int deg = (int)(atan2(y,x) * 180 / M_PI) * (vol_slices/360.0);
	if(deg < 0) deg+=vol_slices;
	int radius = (int)round((sqrt(pow(x,2)+pow(y,2))));
	
	PolPnt *p = new PolPnt(height, deg, radius);
	return p;
}

/* -------------------------POLAR COORDINATE POINT------------------------- */
PolPnt::PolPnt(int height_, int deg_, int radius_) {
	height = height_;
	deg = deg_;
	radius = radius_;
}

int PolPnt::getHeight() const {return height;}
int PolPnt::getDeg() const {return deg;}
int PolPnt::getRadius() const {return radius;}

void PolPnt::setHeight(int height_) {height = height_;}
void PolPnt::setDeg(int deg_) {deg = deg_;}
void PolPnt::setRadius(int radius_) {radius = radius_;}

void PolPnt::printCoord() const {
	printf("(Polar) height: %d, deg: %d, slice: %d, radius: %d", height, (int)(deg * 360.0 /vol_slices), deg, radius);
}

int PolPnt::toData() const {
	return (int) pow(2, vol_radius - (radius-1));
}