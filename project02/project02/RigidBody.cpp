#include "RigidBody.hpp"

glm::mat3 RigidBody::calculateMomentOfInertia()
{
	float I0=0, I1=0, I2=0, I3=0, I4=0, I5=0;
	for(int i=0; i < bodyVertices.size(); i++)
	{
		I0 += glm::pow(bodyVertices[i].y,2.f) + glm::pow(bodyVertices[i].z,2.f);
		I1 += bodyVertices[i].x * bodyVertices[i].y;
		I2 += bodyVertices[i].x * bodyVertices[i].z;
		I3 += glm::pow(bodyVertices[i].x,2.f) + glm::pow(bodyVertices[i].z,2.f);
		I4 += bodyVertices[i].y * bodyVertices[i].z;
		I5 += glm::pow(bodyVertices[i].x,2.f) + glm::pow(bodyVertices[i].y,2.f);
	}
	I1 = -I1;
	I2 = -I2;
	I4 = -I4;

	return glm::mat3(I0,I1,I2,I1,I3,I4,I2,I4,I5);
}

RigidBody::RigidBody(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alpha, float radius, float _height, float _a, float _b) : Figures(file_name, texture_name, _type, _alpha) 
{
	positionNext = glm::vec3(0.f);
	positionPrev = positionNext;
	velocityNext = glm::vec3(1.f,0.f,0.f); //prêdkoœæ
	velocityPrev = velocityNext;
	mass = bodyVertices.size();
	force = glm::vec3(0.f);

	height = _height;
	a = _a;
	b = _b;
	sphereRadius = radius;
	center = new BSphere("sphere2.obj","yellow.tga", GL_TRIANGLES, 1.f);
	box = new Figures("cube3.obj","uvCube.tga", GL_TRIANGLES, .2f);

	centerOfMass = calculateCenterOfMass();
	rotationMatrixNext = glm::mat3(1.0f);
	rotationQuatNext = glm::quat(rotationMatrixNext);
	rotationMatrixPrev = rotationMatrixNext;
	angularVelocity = glm::vec3(0.6f,0.5f,0.7f); //prêdkoœæ k¹towa

	momentOfInertia = calculateMomentOfInertia();
	inverseMomentOfInertia = glm::inverse(momentOfInertia);
	if(_isnan(inverseMomentOfInertia[0].x)) { inverseMomentOfInertia = glm::mat3(1.0f); }
}

RigidBody::~RigidBody(void)
{
}

void RigidBody::setTexture(GLuint texture2)
{
	texture = texture2;
}

void RigidBody::rotate(GLfloat angle, glm::vec3 axes)
{
	Model = glm::rotate(Model, angle, axes);
}


void RigidBody::eulerAlgorithm(float timeStep)
{
	velocityNext += (force/mass)*timeStep;
	positionNext += velocityNext*timeStep;
}

glm::mat3 toMatrix(glm::quat q)
{
	float xx = q.x*q.x, yy = q.y*q.y, zz = q.z*q.z,
		xy = q.x*q.y, xz = q.x*q.z, yz = q.y*q.z,
		xw = q.x*q.w, yw = q.y*q.w, zw = q.z*q.w;

	return glm::mat3(1-2*(yy + zz), 2*(xy - zw), 2*(xz + yw),
					2*(xy + zw), 1-2*(xx + zz), 2*(yz - xw),
					2*(xz - yw), 2*(yz + xw), 1-2*(xx + yy));
}

void normalizeQuat(glm::quat &q)
{
	float n = 1.0f / sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
	q*= n;
}

void RigidBody::eulerRotation(float timeStep)
{
	glm::vec3 resultantForce = glm::vec3(0.f,0.f,0.f);
	glm::vec3 derivativeAngular = inverseMomentOfInertia*(resultantForce-(glm::cross(angularVelocity,momentOfInertia*angularVelocity)));
	angularVelocity += derivativeAngular*timeStep;
	/*
	//obliczenia na macierzach
	glm::mat3 angularVelocityMatrix = glm::mat3(0,-angularVelocity.z,angularVelocity.y,
		angularVelocity.z,0,-angularVelocity.x,
		-angularVelocity.y,angularVelocity.x,0);
	glm::mat3 derivativeRotation = angularVelocityMatrix * rotationMatrixNext;
	rotationMatrixNext += derivativeRotation * timeStep;
	*/
	//obliczenia na kwaternionach
	glm::quat derivativeQuaternion = glm::quat(angularVelocity)*rotationQuatNext*0.5;
	rotationQuatNext = rotationQuatNext + derivativeQuaternion*timeStep;
	normalizeQuat(rotationQuatNext);

	rotationMatrixNext = toMatrix(rotationQuatNext);
}


void RigidBody::beforeMove(float timeStep)
{
	eulerAlgorithm(timeStep);
	eulerRotation(timeStep);
}

float len(glm::vec3 pos)
{
	return glm::sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
}

void RigidBody::drawCenter(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID)
{
	center->setPosition(positionNext);
	center->move();
	center->draw(Projection,View,MatrixID,AlphaID,TextureID);
}

void RigidBody::drawBox(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID)
{
	box->setCenterOfMass(centerOfMass);
	box->draw(Projection,View,MatrixID,AlphaID,TextureID);
	box->setRotMatrix(glm::mat3(glm::scale(glm::mat4(1.f),glm::vec3(height/4))) * getRotMatrix());
	box->setPosition(positionNext);
	box->move();
}

bool RigidBody::sphereCollision(float secondRadius, glm::vec3 pos)
{
	spherePosition = positionNext + rotationMatrixNext * glm::vec3(0,0.2f,0);
	if(sphereRadius + secondRadius >= std::abs(len(spherePosition) - len(pos)))
		return true;
	return false;
}

bool RigidBody::boxCollision(RigidBody body2)
{
	glm::mat3 rmT, rmB;
	glm::vec3 posB, p2;

	boxPosition = box->getEndPosition();

	rmT = glm::transpose(rotationMatrixNext);
	rmB = body2.getRotMatrix() * rmT;
	posB = rmT * (body2.getPosition() - boxPosition);

	glm::vec3 aX = glm::vec3(rmB[0].x,rmB[1].x,rmB[2].x);
	glm::vec3 aY = glm::vec3(rmB[0].y,rmB[1].y,rmB[2].y);
	glm::vec3 aZ = glm::vec3(rmB[0].z,rmB[1].z,rmB[2].z);

	const double epsilon = 1E-7;
	glm::vec3 absAX = glm::vec3(fabs(aX.x)+epsilon, fabs(aX.y)+epsilon, fabs(aX.z)+epsilon);
	glm::vec3 absAY = glm::vec3(fabs(aY.x)+epsilon, fabs(aY.y)+epsilon, fabs(aY.z)+epsilon);
	glm::vec3 absAZ = glm::vec3(fabs(aZ.x)+epsilon, fabs(aZ.y)+epsilon, fabs(aZ.z)+epsilon);

	glm::vec3 a = glm::vec3(getSize().x/2,getSize().y/2,getSize().z/2);
	glm::vec3 b = glm::vec3(body2.getSize().x/2,body2.getSize().y/2,body2.getSize().z/2);

	//15 testow
	//1 Ax
	if(fabs(posB.x) > a.x + b.x * absAX.x + b.y * absAY.x + b.z * absAZ.x)
		return false;
	//2 Ay
	if(fabs(posB.y) > a.y + b.x * absAX.y + b.y * absAY.y + b.z * absAZ.y)
	    return false;
	//3 Az
	if(fabs(posB.z) > a.z + b.x * absAX.z + b.y * absAY.z + b.z * absAZ.z)
	    return false;
 
	//4 Bx
	if(fabs(posB.x*aX.x+posB.y*aX.y+posB.z*aX.z) >
	    (b.x+a.x*absAX.x + a.y * absAX.y + a.z*absAX.z))
		return false;
	//5 By
	if(fabs(posB.x*aY.x+posB.y*aY.y+posB.z*aY.z) >
	    (b.y+a.x*absAY.x + a.y * absAY.y + a.z*absAY.z))
		return false;
	//6 Bz
	if(fabs(posB.x*aZ.x+posB.y*aZ.y+posB.z*aZ.z) >
	    (b.z+a.x*absAZ.x + a.y * absAZ.y + a.z*absAZ.z))
		return false;
 
	//7 Ax x Bx
	if(fabs(posB.z*aX.y-posB.y*aX.z) > 
		a.y*absAX.z + a.z*absAX.y + b.y*absAZ.x + b.z*absAY.x)
		return false;
	//8 Ax x By
	if(fabs(posB.z*aY.y-posB.y*aY.z) > 
		a.y*absAY.z + a.z*absAY.y + b.x*absAZ.x + b.z*absAX.x)
		return false;
	//9 aX x Bz
	if(fabs(posB.z*aZ.y-posB.y*aZ.z) > 
		a.y*absAZ.z + a.z*absAZ.y + b.x*absAY.x + b.y*absAX.x)
		return false;
	 
	//10 Ay x Bx
	if(fabs(posB.x*aX.z-posB.z*aX.x) > 
		a.x*absAX.z + a.z*absAX.x + b.y*absAZ.y + b.z*absAY.y)
		return false;
	//11 Ay x By
	if(fabs(posB.x*aY.z-posB.z*aY.x) > 
		a.x*absAY.z + a.z*absAY.x + b.x*absAZ.y + b.z*absAX.y)
		return false;
	//12 Ay x Bz
	if(fabs(posB.x*aZ.z-posB.z*aZ.x) > a.x*absAZ.z + 
	    a.z*absAZ.x + b.x*absAY.y + b.y*absAX.y)
		return false;
	 
	//13 Az x Bx
	if(fabs(posB.y*aX.x-posB.x*aX.y) > a.x*absAX.y + 
	    a.y*absAX.x + b.y*absAZ.z + b.z*absAY.z)
		return false;
	//14 Az x By
	if(fabs(posB.y*aY.x-posB.x*aY.y) > a.x*absAY.y + 
	    a.y*absAY.x + b.x*absAZ.z + b.z*absAX.z)
		return false;
	//15 Az x Bz
	if(fabs(posB.y*aZ.x-posB.x*aZ.y) > a.x*absAZ.y + 
	    a.y*absAZ.x + b.x*absAY.z + b.y*absAX.z)
		return false;

	return true;
}