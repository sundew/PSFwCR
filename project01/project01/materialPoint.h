#pragma once
#include <GL/glut.h>
#include "vector3d.h"
#include "random.h"
#include "cylinder.h"
#include <time.h>
#include <iostream>
#include <sstream>
using namespace std;

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

void draw_text(float x, float y, string text){
	glPushMatrix();
	glRasterPos2f(x,y);
	glColor4f(1.f,1.f,1.f,1.f);
	for (int i=0; i<text.size(); i++)
    {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
	glPopMatrix();
}

class MaterialPoint{
	private:
		Vector3d position;
		Vector3d velocity;
		Vector3d newPosition;
		Vector3d newVelocity;
		Vector3d force;
		float mass;
		bool cylinderCheck(Vector3d &p,Cylinder cylinder);
		void bounce(Vector3d normal,Cylinder cylinder);
		float radius;
		Vector3d color;

	public:
		MaterialPoint(){
			position = Vector3d(0.0,1.0,0.0);
			newPosition = position;
			velocity = Vector3d(1.0,0.5,0.6);
			newVelocity = velocity;
			force = Vector3d(0.,0.,0.);
			mass = 1;
			radius = 0.3;
			srand (time(NULL));
			color = Vector3d(randFloat(0,1),randFloat(0,1),randFloat(0,1));
			click = false;
		}
		MaterialPoint(Vector3d pos,Vector3d vel, Vector3d _color, float _mass):position(pos),velocity(vel),color(_color),mass(_mass){
			newPosition = position;
			newVelocity = velocity;
			force = Vector3d(0.,0.,0.);
			radius = mass * 0.3;
			click = false;
		}

		bool click;

		void setPosition(Vector3d pos){position = pos;}
		Vector3d getPosition(){return position;}

		void setNewPosition(Vector3d pos){newPosition = pos;}
		Vector3d getNewPosition(){return newPosition;}

		void setVelocity(Vector3d vel){velocity = vel;}
		Vector3d getVelocity(){return velocity;}

		void setNewVelocity(Vector3d vel){newVelocity = vel;}
		Vector3d getNewVelocity(){return newVelocity;}

		Vector3d getForce(){return force;}
		float getMass(){return mass;}

		float getRadius(){return radius;}
		void draw();
		void new_move(float step);
		void move(float step);
		void cylinderCollision(Cylinder cylinder);
};

void eulerAlgorithm(MaterialPoint* p,float time_step){
	p->setNewVelocity(p->getNewVelocity()+(p->getForce()/p->getMass())*time_step);
	p->setNewPosition(p->getNewPosition()+p->getNewVelocity()*time_step);
}

void MaterialPoint::new_move(float step){
	eulerAlgorithm(this,step);
}

void MaterialPoint::move(float step){
	eulerAlgorithm(this,step);
	setPosition(newPosition);
	setVelocity(newVelocity);
}

bool MaterialPoint::cylinderCheck(Vector3d &p,Cylinder cylinder){
	Vector3d vector = cylinder.position - newPosition;
	p = Vector3d(vector.x,0.,vector.z).normalize();

	if(abs(vector.y) + radius > cylinder.height/2){
		p = Vector3d(0.,vector.y,0.).normalize();
		return true;
	}

	float xy_length = sqrt(pow(vector.x,2) + pow(vector.z,2)) + radius;
	if(xy_length > cylinder.radius)
		return true;

	return false;
}

void MaterialPoint::bounce(Vector3d normal,Cylinder cylinder){
	double velocityComponent = normal * newVelocity;

	if (velocityComponent < 0){
		setNewVelocity(newVelocity - normal*((cylinder.reflectivity+1)*velocityComponent));
	}
}

void MaterialPoint::cylinderCollision(Cylinder cylinder){
	Vector3d normal = Vector3d(0.,0.,0.);
	if(cylinderCheck(normal,cylinder))
		bounce(normal,cylinder);
}

void MaterialPoint::draw(){
	glTranslatef(position.x,position.y,position.z);
	glColor4f(color.x,color.y,color.z, 0.9f );
	glutSolidSphere( radius, 20, 20 );
	glColor4f( 0.3f, 0.3f, 0.3f, 0.5f );
	glutWireSphere( radius, 20, 20 );
	if(click){
		draw_text(-3,0.5,"velocity = ["+SSTR(velocity.x)+","+SSTR(velocity.y)+","+SSTR(velocity.z)+"] "
	+"position = ["+SSTR(position.x)+","+SSTR(position.y)+","+SSTR(position.z)+"]");
	}
}

