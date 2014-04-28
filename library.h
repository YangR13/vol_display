#ifndef LIBRARY_H
#define LIBRARY_H

class CartPnt;
class PolPnt;

/* -----------------------CARTESIAN COORDINATE POINT----------------------- */
class CartPnt {
	private:
		int height;
		int x;
		int y;
		
	public:
		CartPnt(int height_, int x_, int y_);
		int getHeight() const;
		int getX() const;
		int getY() const;
		
		void setHeight(int height_);
		void setX(int x_);
		void setY(int y_);
		
		void printCoord();
		
		PolPnt * toPolar();	
};

/* -------------------------POLAR COORDINATE POINT------------------------- */
class PolPnt {
	private:
		int height;
		int deg;
		int radius;
		
	public:
		PolPnt(int height_, int deg_, int radius_);
		int getHeight() const;
		int getDeg() const;
		int getRadius() const;
		
		void setHeight(int height_);
		void setDeg(int deg_);
		void setRadius(int radius_);
		
		void printCoord();
};

#endif /* LIBRARY_H */