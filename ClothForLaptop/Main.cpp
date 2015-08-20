#include <windows.h> 
#include <glut.h>
#include <math.h>
#include <vector>
#include "Vector.h"
#include "Particle.h"
#include "Constraint.h"
#include "Cloth.h"
using namespace std;

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	GLfloat lightPosition[4] = { 1.0, 1.0, 1.0, 0.0 }; //light coming from upper right
	glLightfv(GL_LIGHT0, GL_POSITION, (GLfloat *)&lightPosition);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

Cloth cloth(15, 15, 30, 30);

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glClearColor(1, 1, 1, 0); //white background
	glEnable(GL_LIGHTING);

	Vector ballLocation(10, -5, 5); 
	float timeStep = .5;
	float radius = 3;
	//cloth
	cloth.clothCalculations(); 
	cloth.gravity(Vector(0, 0, 0.2)*timeStep);
	cloth.ballCollision(ballLocation, radius); 

	glTranslatef(-5, 5, -10);
	glRotatef(120, 1, 0, 0); 
	cloth.draw(); 
	//ball
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);  //blue
		glTranslatef(ballLocation.v[0], ballLocation.v[1], ballLocation.v[2]); 
		glutSolidSphere(radius-.1, 20, 20); 
	glPopMatrix();

	glutSwapBuffers();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, (float)w / (float)h, 1.0, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Cloth Simulation");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
