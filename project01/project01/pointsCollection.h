#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include "materialPoint.h"
#include "random.h"
#include "cylinder.h"
using namespace std;

class PointsCollection{
	public:
		int count;
		float step;
		PointsCollection(int _count,float _step,Cylinder cylinder):count(_count),step(_step){generate(cylinder);}
		vector<MaterialPoint> collection;
		void generate(Cylinder cylinder);
		void update(Cylinder cylinder);
		void draw();
		void collisions(int i);
};

void PointsCollection::generate(Cylinder cylinder){
	srand (time(NULL));
	for(int i = 0; i<count; i++){
		float radius = randFloat(0.5,2.5);
		Vector3d color = Vector3d(randFloat(0,1),randFloat(0,1),randFloat(0,1));
		Vector3d position = Vector3d(randFloat(-(cylinder.radius-1),cylinder.radius-1),randFloat(-(cylinder.height/2 - 0.5),cylinder.height/2 - 0.5),randFloat(-(cylinder.radius-1),cylinder.radius-1));
		Vector3d velocity = Vector3d(randFloat(0,1),randFloat(0,1),randFloat(0,1));
		MaterialPoint p(position,velocity,color,radius);
		collection.push_back(p);
	}
}

void PointsCollection::update(Cylinder cylinder){
	for(int i = 0; i<count; i++){
		collection[i].new_move(step);
		collection[i].cylinderCollision(cylinder);
		collection[i].new_move(step);
		collisions(i);
		collection[i].move(step);
	}

}

void PointsCollection::draw(){
	for(int i = 0; i<count; i++){
		glPushMatrix();
			collection[i].draw();
		glPopMatrix();
	}
	cout<<collection[1].getVelocity().length()<<endl;
}

void PointsCollection::collisions(int i){
	float e = 1; //wspolczynnik sprezystosci
	for (int j = i + 1; j < count; j++){
		float distance = (collection[i].getNewPosition() - collection[j].getNewPosition()).length();
		if(distance <= collection[i].getRadius()+collection[j].getRadius()){
			Vector3d normal = (collection[i].getPosition() - collection[j].getPosition()).normalize();
			float reducedMass = 1/(1/collection[i].getMass() + 1/collection[j].getMass());
			float dvn = (collection[i].getVelocity()-collection[j].getVelocity())*normal;

			float J = -reducedMass*(e+1)*dvn;

			Vector3d uI = collection[i].getVelocity() + normal*J/collection[i].getMass();
			Vector3d uJ = collection[j].getVelocity() - normal*J/collection[j].getMass();

			collection[i].setNewVelocity(uI);
			collection[j].setNewVelocity(uJ);
		}
	}
}