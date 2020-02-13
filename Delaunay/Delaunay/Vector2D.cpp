#pragma once
#include "stdafx.h"
#include "Vector2D.h"
#include <math.h>
#include <iostream>

CVector2D CVector2D::norm() const
{
	double p = _hypot(x, y);
	return CVector2D(x / p, y / p);
}

void CVector2D::move(double dx, double dy) 
{
	x += dx;
	y += dy;
}

double CVector2D::dot(const CVector2D& rhs) const 
{
	return x * rhs.x + y * rhs.y;
}

CVector2D CVector2D::operator * (double rhs) const 
{
	return CVector2D(x * rhs, y * rhs);
}

CVector2D CVector2D::operator - (const CVector2D& rhs) const 
{
	return CVector2D(x - rhs.x, y - rhs.y);
}

CVector2D CVector2D::operator + (const CVector2D& rhs) const 
{
	return CVector2D(x + rhs.x, y + rhs.y);
}

CVector2D CVector2D::operator - () const
{
	return CVector2D(-x, -y);
}

CVector2D CVector2D::project(const CVector2D& rhs) const 
{
	CVector2D r = rhs.norm();
	return r * r.dot(*this);
}

CVector2D CVector2D::footOfPerpendicular(const CVector2D& c, const CVector2D& p, const CVector2D& q) 
{
	return c + (p - c).project(q - c);
}

double CVector2D::Magnitude() 
{
	return _hypot(x, y);
}

double CVector2D::cross(const CVector2D& rhs) 
{
	return x * rhs.y - y * rhs.x;
}

double CVector2D::angle(const CVector2D& rhs)
{
	CVector2D normA = norm();
	CVector2D normB = rhs.norm();

	double radian = acos(normA.dot(normB));
	double ret = (radian / PI) * 180.0;

	return ret;
}

// 반시계 방향인지 검사
double ccw(CVector2D a, CVector2D b)
{
	return a.cross(b);
}

bool ccw(CVector2D p, CVector2D a, CVector2D b) 
{
	return (ccw(a - p, b - p) > 0.0)? true: false;
}