#include "BSphere.hpp"


BSphere::BSphere(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alpha) : Figures(file_name, texture_name, _type, _alpha) 
{
	positionNext = glm::vec3(0.f,0.f,0.f);
	radius = 3.f;
	centerOfMass = glm::vec3(0.f,0.f,0.f);
}

BSphere::~BSphere(void)
{
}