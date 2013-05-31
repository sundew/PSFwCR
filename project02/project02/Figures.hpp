#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objloader.hpp"
class Figures
{
protected:
	GLfloat alpha;
	GLfloat *g_vertex_buffer_data;
	GLuint texture;
	glm::mat4 TranslationMatrix;
	glm::mat4 Model;
	glm::mat4 MVP;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint size;
	std::vector<glm::vec3> bodyVertices;
	GLenum type;

	glm::vec3 positionNext;
	glm::vec3 centerOfMass;
	glm::mat3 rotationMatrixNext;

	glm::vec3 calculateCenterOfMass();
public:
	Figures(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alpha);
	~Figures(void);
	void draw(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID);
	virtual void move();
	glm::vec3 getPosition(){return positionNext;}
	glm::mat3 getRotMatrix(){return rotationMatrixNext;}
	glm::vec3 getCenterOfMass(){return centerOfMass;}
	
	void setPosition(GLfloat x, GLfloat y, GLfloat z);
	void setPosition(glm::vec3 pos);
	void setRotMatrix(glm::mat3 matrix);
	void setCenterOfMass(glm::vec3 center);
	glm::vec3 getEndPosition(){return glm::vec3(Model[3].x,Model[3].y,Model[3].z); };
};

