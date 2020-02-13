#pragma once
#include <math.h>
#include <iostream>
const double PI = 3.1415926535; 
const double EPSILON = 0.0000001;
class CVector2D
{
public:
								CVector2D(){}
								CVector2D(double x_, double y_) :x(x_), y(y_){}
							   ~CVector2D(){}

	CVector2D					norm() const;
	void						move(double dx, double dy);

	double						dot(const CVector2D& rhs) const;

	
	CVector2D					project(const CVector2D& rhs) const;
	CVector2D					footOfPerpendicular(const CVector2D& c, const CVector2D& p, const CVector2D& q);
	double						Magnitude();
	double						cross(const CVector2D& rhs);

	double						angle(const CVector2D& rhs);

	bool						Equal(const CVector2D& rhs) { return x == rhs.x && y == rhs.y; }

	CVector2D					operator * (double rhs) const;
	CVector2D					operator - (const CVector2D& rhs) const;
	CVector2D					operator + (const CVector2D& rhs) const;
	CVector2D					operator - () const;

	double						x;
	double						y;
};

double	ccw(CVector2D a, CVector2D b);
bool	ccw(CVector2D p, CVector2D a, CVector2D b);