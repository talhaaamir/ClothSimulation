#pragma once
#ifndef _CONSTRAINT_H
#define _CONSTRAINT_H

#include "Vector.h"
#include "Particle.h"

class Constraint
{
public:
	float lengthAtRest; //also known as the constraint length, this is the range that particles must return to in order to create the springiness

	Particle *p1, *p2; //length is between these two particles

	//custom constructor
	Constraint(Particle *P1, Particle *P2)
	{
		p1 = P1;
		p2 = P2;
		Vector distance = p2->currentPosition - p1->currentPosition; 
		lengthAtRest = distance.magnitude();
	}

	//checks to see if the constraint (lengtAtRest) is valid or not, if not then reset with correction vectors
	void checkConstraint()
	{
		Vector distance = p2->currentPosition - p1->currentPosition; 
		float currentLength = distance.magnitude(); //find the distance between both particles
		Vector correction = (distance*(1 -lengthAtRest/currentLength))*.5; //offset to return p1 from current location to length at rest (.5 is half to rest length, negation covers other half)
		Vector negCorrection = correction*-1; //negate for p2
		p1->offsetPosition(correction); 
		p2->offsetPosition(negCorrection); 
	}
};

#endif