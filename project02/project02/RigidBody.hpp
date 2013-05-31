#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Figures.hpp"
#include "Bsphere.hpp"

class RigidBody : public Figures
{
private:
	glm::mat3 rotationMatrixPrev;

	glm::quat rotationQuatNext;

	glm::vec3 angularVelocity;
	glm::mat3 momentOfInertia;
	glm::mat3 inverseMomentOfInertia;
	
	glm::vec3 positionPrev;
	glm::vec3 velocityNext;
	glm::vec3 velocityPrev;

	float height;
	float a,b;
	float sphereRadius;
	glm::vec3 spherePosition;
	glm::vec3 boxPosition;
	float density;
	float volume;
	float mass;
	glm::vec3 force;

	BSphere* center;
	Figures* box;

	void eulerAlgorithm(float timeStep);
	void eulerRotation(float timeStep);
	glm::mat3 calculateMomentOfInertia();

public:
	RigidBody(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alpha, float radius, float _height, float _a, float _b);
	~RigidBody(void);
	void beforeMove(float timeStep);

	glm::vec3 getSize(){return glm::vec3(a,b,height);};
	void rotate(GLfloat angle, glm::vec3 axes);
	void setTexture(GLuint texture2);
	bool sphereCollision(float secondRadius, glm::vec3 pos);
	bool boxCollision(RigidBody body2);
	float getRadius(){return sphereRadius;};
	glm::vec3 getBoxPosition(){return boxPosition;};
	void drawCenter(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID);
	void drawBox(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID);
};

#endif