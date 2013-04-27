#pragma once
#include <stdlib.h>
#include <GL/glut.h>
#include "vector3d.h"
#include <iostream>
using namespace std;

class Cylinder{
	private:
		GLUquadricObj *obj;
	public:
		Vector3d position;
		float radius;
		float height;
		float reflectivity;
		Cylinder();
		Cylinder(Vector3d pos, float _radius, float _height, float _reflect):position(pos),radius(_radius),height(_height),reflectivity(_reflect){obj = gluNewQuadric();}
		void draw();
};

void Cylinder::draw(){
	glTranslatef(0.f,-(height/2),0.f);
	glRotatef(-90, 1.f, 0.f, 0.f);
	glTranslatef(position.x,position.y,position.z);
	gluCylinder(obj, radius, radius, height, 30, 30);
}