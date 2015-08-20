#pragma once
#ifndef _CLOTH_H
#define _CLOTH_H

#include "Vector.h"
#include "Particle.h"
#include "Constraint.h"
#include <iostream>
using namespace std;

class Cloth
{
public:
	//vectors of all the particles and their constraints
	std::vector<Particle> particles;
	std::vector<Constraint> constraints;

	int widthAmount; 
	int heightAmount; 

	//cross product of two sides of the triangle equals the surface normal
	//V = position2-position1
	//W = position3-position1
	//cross product these two vectors
	Vector triangleSurfaceNormal(Particle *p1, Particle *p2, Particle *p3)
	{
		Vector position1 = p1->currentPosition;
		Vector position2 = p2->currentPosition;
		Vector position3 = p3->currentPosition;
		Vector V = position2 - position1;
		Vector W = position3 - position1;
		return V.cross(W);
	}

	Particle* getParticle(int x, int y) 
	{ 
		return &particles[y*widthAmount + x];
	}

	//cloth constructor
	Cloth(float width, float height, int WidthAmount, int HeightAmount) 
	{
		widthAmount = WidthAmount;
		heightAmount = HeightAmount;
		particles.resize(widthAmount*heightAmount); //totality of all particles
		int x, y = 0;
		//place all the particles in a cloth like grid
		for(x = 0; x<widthAmount; x++)
		{
			for(y = 0; y<heightAmount; y++)
			{
				Vector xyzCoordinate = Vector(width*(x/(float)widthAmount), -height*(y/(float)heightAmount), 0);
				particles[y*widthAmount + x] = Particle(xyzCoordinate); // insert particle in column x at y'th row
			}
		}
		
		//add constraints to primary neighbor
		for(x = 0; x<widthAmount; x++)
		{
			for(y = 0; y<heightAmount; y++)
			{
				if(x<widthAmount-1) //particle to right of it
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x + 1, y))));
				}
				if(y<heightAmount-1) //particle one above
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x, y+1))));
				}
				if (x < widthAmount - 1 && y < heightAmount - 1) //particle diagonal up right
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x + 1, y + 1))));
				}
			}
		}
		
		//add constraints to secondary neighbors for more springiness
		for (x = 0; x<widthAmount; x++)
		{
			for (y = 0; y<heightAmount; y++)
			{
				if (x < widthAmount - 2) //particle two rights from
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x + 2, y))));
				}
				if (y < heightAmount - 2) //particle two above
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x, y+2))));
				}
				if (x < widthAmount - 2 && y < heightAmount - 2) //particle 2 diagonal up right
				{
					constraints.push_back(Constraint(getParticle(x, y), (getParticle(x + 2, y+2))));
				}
			}
		}
	}

	//each particle's acceleration (via verlet()()) in grid is calculated along with checking all of the constraints with each other particles
	//most computer intensive of the functions
	void clothCalculations()
	{
		std::vector<Particle>::iterator particle;
		//verlet calculation for each particle
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).verlet();
		}

		std::vector<Constraint>::iterator constraint;
		int i = 0;
		int constraintLoops = 2; //higher means more constricting the cloth becomes (and more computer intensive)
		//#pragma omp parallel for (couldn't figure out OpenMP for multithreaded programming to speed things up
		for (i = 0; i<constraintLoops; i++)
		{	//check all the constraints
			for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
			{
				(*constraint).checkConstraint();
			}
		}
	}

	//add gravity to all particles
	//same setup as above, but this time instead of applying verlet, simply add a directional vector to each particle
	void gravity(const Vector grav)
	{
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).gravity(grav);
		}
	}

	//draw cloth
	void draw()
	{
		std::vector<Particle>::iterator particle; 
		int x, y = 0;
		glBegin(GL_TRIANGLES);
		for(x = 0; x<widthAmount - 1; x++)
		{
			for(y = 0; y<heightAmount - 1; y++)
			{	
				//first add up each triangle normal to each particle
				Vector normal = triangleSurfaceNormal(getParticle(x + 1, y), getParticle(x, y), getParticle(x, y + 1));
				getParticle(x + 1, y)->addNonNormalAndNormal(normal);
				getParticle(x, y)->addNonNormalAndNormal(normal);
				getParticle(x, y + 1)->addNonNormalAndNormal(normal);
				//second triangle
				normal = triangleSurfaceNormal(getParticle(x + 1, y + 1), getParticle(x + 1, y), getParticle(x, y + 1));
				getParticle(x + 1, y + 1)->addNonNormalAndNormal(normal);
				getParticle(x + 1, y)->addNonNormalAndNormal(normal);
				getParticle(x, y + 1)->addNonNormalAndNormal(normal);

				//actually drawing both triangles to get full rectangle
				Vector color = Vector(1.0f, 1.0f, 0.0f);
				glColor3fv((GLfloat*)&color);
				glNormal3fv((GLfloat *)&(getParticle(x+1, y)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x+1, y)->currentPosition));
				glNormal3fv((GLfloat *)&(getParticle(x, y)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x, y)->currentPosition));
				glNormal3fv((GLfloat *)&(getParticle(x, y+1)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x, y+1)->currentPosition));
				//second triangle
				glNormal3fv((GLfloat *)&(getParticle(x + 1, y+1)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x + 1, y+1)->currentPosition));
				glNormal3fv((GLfloat *)&(getParticle(x+1, y)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x+1, y)->currentPosition));
				glNormal3fv((GLfloat *)&(getParticle(x, y + 1)->nonNormalized.normalize()));
				glVertex3fv((GLfloat *)&(getParticle(x, y + 1)->currentPosition));
			}
		}
		glEnd();	
	}

	//if particle is inside sphere (by checking if the particle's magnitude is less than the radius), then reposition that particle outside of the sphere in the correct place
	void ballCollision(const Vector center, const float radius)
	{
		std::vector<Particle>::iterator particle;
		//same for loop set ups as above, check each particle
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			Vector vector = (*particle).currentPosition - center;
			if (vector.magnitude() < radius) 
			{
				(*particle).offsetPosition(vector.normalize()*(radius - vector.magnitude()));
			}
		}
	}
};

#endif