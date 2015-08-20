#pragma once
#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "Vector.h"

//creates one instance of a particle
class Particle
{
public:
	float mass; 
	Vector acceleration;
	Vector nonNormalized; 
	Vector currentPosition; 
	Vector previousPosition;  //particle in previous timeStep, with the current position you calculate velocity for verlet integration

	//custom constructor
	Particle(Vector location)
	{
		mass = 1;
		acceleration = Vector(0, 0, 0);
		nonNormalized = acceleration;
		currentPosition = location;
		previousPosition = location;
	}

	//default constructor
	Particle(){}

	void offsetPosition(const Vector v)
	{
		currentPosition = currentPosition + v;
	}

	//add non normal and normal vectors
	void addNonNormalAndNormal(Vector normal)
	{
		nonNormalized = nonNormalized + normal.normalize();
	}

	//acceleration = force/mass
	void gravity(Vector f)
	{
		//force/mass
		f.v[0] = f.v[0] / mass;
		f.v[1] = f.v[1] / mass;
		f.v[2] = f.v[2] / mass;
		//f.x = f.x / mass;
		//f.y = f.y / mass;
		//f.z = f.z / mass;
		acceleration = acceleration + f;
	}

	//acceleration is translated into the correct position by verlet intergration
	void verlet()
	{
			//like a swap
			Vector temp = currentPosition;
			float timeStep = .5; //time step = incremental change in time for which the verlet integration equation is being solved
			//verlet integration follows, current position = current position + velocity + acceleration*timestep^2
			Vector velocity = currentPosition - previousPosition;
			currentPosition = currentPosition + (velocity) + acceleration*timeStep*timeStep;
			previousPosition = temp;
			acceleration = Vector(0, 0, 0); //need to reset acceleration vector
	}
};

#endif