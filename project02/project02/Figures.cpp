#include "Figures.hpp"


Figures::Figures(const char *file_name, const char *texture_name, GLenum _type, GLfloat _alpha)
{
	type = _type;
	alpha = _alpha;

	loadOBJ(file_name, vertices, uvs, normals, bodyVertices);

	texture = loadTGA_glfw(texture_name);

	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*uvs.size(), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*normals.size(), &normals[0], GL_STATIC_DRAW);

	Model = glm::mat4(1.0f);
	
	TranslationMatrix = glm::mat4(1.0f);

	size = (vertices.size() * sizeof(glm::vec3))/(sizeof(GLfloat)*3);

}


Figures::~Figures(void)
{
}

void Figures::draw(glm::mat4 Projection, glm::mat4 View, GLuint MatrixID, GLuint AlphaID, GLuint TextureID)
{
	MVP = Projection * View * Model;

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniform1f(AlphaID, alpha);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(TextureID, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(type, 0, size);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void Figures::move()
{
	Model = glm::translate(glm::mat4(1.f),positionNext) * glm::mat4(rotationMatrixNext) * glm::translate(glm::mat4(1.f),-centerOfMass);
}

glm::vec3 Figures::calculateCenterOfMass()
{
	glm::vec3 center = glm::vec3(0.f,0.f,0.f);
	for(int i=0; i < bodyVertices.size(); i++)
	{
		center += bodyVertices[i];
	}
	center /= bodyVertices.size();
	return center;
}

void Figures::setPosition(glm::vec3 pos)
{
	positionNext = pos;
}
void Figures::setRotMatrix(glm::mat3 matrix)
{
	rotationMatrixNext = matrix;
}
void Figures::setCenterOfMass(glm::vec3 center)
{
	centerOfMass = center;
}

void Figures::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	positionNext = glm::vec3(x,y,z);
	Model = glm::translate(glm::mat4(1.0f),positionNext);
}

