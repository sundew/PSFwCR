#pragma once
#include "Figures.hpp"
class BSphere : public Figures
{
private:
	GLfloat radius;
public:
	BSphere(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alhpa);
	~BSphere(void);
};

