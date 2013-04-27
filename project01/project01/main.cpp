/*
Projekt 1 - Paulina Banasiak

(0) kamera:
	W - do przodu
	S - do ty³u
	A - w lewo
	D - w prawo

(1) U³o¿enie kursora myszy definiuje kierunek ruchu, gdy wciœniêty LPM.
(2) Przytrzymanie klawisza z cyfr¹ odpowiadaj¹c¹ numerowi kuli wypisuje jej po³o¿enie i prêdkoœæ.
*/
#include <GL/glut.h>
#include <iostream>
#include "vector3d.h"
#include "materialPoint.h"
#include "pointsCollection.h"
#include "cylinder.h"
#include <sstream>
using namespace std;

float xpos = 0, ypos = 0, zpos = 7, xrot = 0, yrot = 0, angle=0.0;
float lastx, lasty;
float mouse_x = 0, mouse_y = 0;

bool can_move = false;
bool* keyStates = new bool[256];

Cylinder cylinder(Vector3d(0,0,0),4,8,1);
PointsCollection collection(5,0.001,cylinder); //ilosc kul, krok czasowy, walec

void camera()
{
	glRotatef(xrot,1.0,0.0,0.0);
    glRotatef(yrot,0.0,1.0,0.0);
    glTranslated(-xpos,0,-zpos); 
}

void keyoperations()
{
	float x = xpos,y = ypos,z = zpos;
	float xrotrad, yrotrad;
	if(!keyStates['w']){
			yrotrad = (yrot / 180 * 3.141592654f);
			xrotrad = (xrot / 180 * 3.141592654f); 
			xpos += float(sin(yrotrad))/100;
			zpos -= float(cos(yrotrad))/100;
			ypos -= float(sin(xrotrad))/100;
	}
	if(!keyStates['s']){
			yrotrad = (yrot / 180 * 3.141592654f);
			xrotrad = (xrot / 180 * 3.141592654f); 
			xpos -= float(sin(yrotrad))/100;
			zpos += float(cos(yrotrad))/100;
			ypos += float(sin(xrotrad))/100;
	}
	if(!keyStates['d']){
			yrotrad = (yrot / 180 * 3.141592654f);
			xpos += float(cos(yrotrad)) * 0.02;
			zpos += float(sin(yrotrad)) * 0.02;
	}
	if(!keyStates['a']){
			yrotrad = (yrot / 180 * 3.141592654f);
			xpos -= float(cos(yrotrad)) * 0.02;
			zpos -= float(sin(yrotrad)) * 0.02;
	}

	for(int i = 0; i < collection.count; i++){
		collection.collection[i].click = false;
		if(i < 10 && !keyStates[48+i])
			collection.collection[i].click = true;
	}
}

void renderScene()
{
	collection.update(cylinder);

	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.f, 0.f, 5.f, 0.f, 0.f, 0, 0.f, 1.f, 0.f);
	
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	keyoperations();
    camera();

	collection.draw();

	glPushMatrix();
	glColor4f( 0.3f, 0.3f, 0.5f, 0.3f );
		if(can_move)
			glColor4f( 0.f, 0.3f, 0.f, 0.3f );
		cylinder.draw();
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void keyboard( unsigned char key, int x, int y ){
	keyStates[key] = false;

	switch( key ){
		case 27:
			exit( 1 );
			break;
	}
}

void keyboardUp (unsigned char key, int x, int y)
{
	keyStates[key] = true;
}

void mouseClick(int button, int state, int x, int y){
	if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_DOWN))
		{can_move = true;
		lastx = x;
		lasty = y;
}
	if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_UP))
		can_move = false;

}

void mouseMovement(int x, int y) {
	if(can_move){
		int diffx = x-lastx;
		int diffy = y-lasty;
		lastx = x;
		lasty = y;
		xrot += (float) diffy/4;
		yrot += (float) diffx/4;
	}
}

void changeSize( int w, int h ){
	if(h==0)
		h=1;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, w, h );
	gluPerspective( 45, ( float ) w/h, 1, 1000 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

int main(int argc, char *argv[]){

	glutInit( &argc, argv );
	glutInitWindowPosition( -1, -1 );
	glutInitWindowSize (1100,700);
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "First Project" );
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutWarpPointer(550, 350);

	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutMotionFunc(mouseMovement);
	glutMouseFunc(mouseClick);

	glutMainLoop();
	return 0;
}