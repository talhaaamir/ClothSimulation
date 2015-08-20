#pragma once
#ifndef _VECTOR_H
#define _VECTOR_H

#include <math.h>

//basic vector class
class Vector 
{
public:
	float v[3];
	float x, y, z;  //decided to go with array approach rather than 3 variables

	//default constructor
	Vector() {}

	//custom constructor
	Vector(float X, float Y, float Z)
	{
	//	x = X;
	//	y = Y;
	//	z = Z;
		v[0] = X;
		v[1] = Y;
		v[2] = Z;
	}

	//magnitude in 3 dimensions (also known as length)
	float magnitude()
	{
	//	return sqrt((x*x) + (y*y) + (z*z));
		return sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2])); //square root of x^2 + y^2 + z^2
	}

	//calculate length and divide each of the x, y, and z coordinated by its length
	Vector normalize()
	{
		float Magnitude = magnitude();
	//	return Vector(x / Magnitude, y / Magnitude, z/Magnitude);
		return Vector(v[0]/Magnitude, v[1]/Magnitude, v[2]/Magnitude);
	}

	//subtract two vectors
	Vector operator- (const Vector &vec)
	{
	//	return Vector(x - vec.x, y - vec.y, z - vec.z);
		return Vector(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2]);
	}
	
	//add two vectors
	Vector operator+ (const Vector &vec)
	{
	//	return Vector(x + vec.x, y + vec.y, z + vec.z);
		return Vector(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
	}
	
	//scalar multiplication
	Vector operator* (const float &a)
	{
	//	return Vector(x*a, y*a, z*a);
		return Vector(v[0]*a, v[1]*a, v[2]*a);
	}
	
	//cross product of two vectors
	Vector cross(const Vector &vec)
	{
	//	return Vector((y*vec.z) - (z*vec.y), (z*vec.x) - (x*vec.z), (x*vec.y) - (y*vec.x));
		return Vector(v[1] * vec.v[2] - v[2] * vec.v[1], v[2] * vec.v[0] - v[0] * vec.v[2], v[0] * vec.v[1] - v[1] * vec.v[0]);
	}
};

#endif