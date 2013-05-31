/*
Projekt 2 - Paulina Banasiak

(0) kamera:
	(WSAD) lub strza³ki + myszka - poruszanie
	(SHIFT) - przyspiesza poruszanie kamer¹

(1) Fizyka zaimplementowana g³ównie w klasie RigidBody:
	(a) w konstruktorze dla testu mo¿na zmieniaæ wartoœci prêdkoœci
	(b) obliczanie ruchu obs³uguje funkcja beforeMove - wpierw zwyk³y ruch liniowy, nastêpnie obrót w funkcji eulerRotation
	(c) kolizje obs³uguj¹ funkcje sphereCollision oraz boxCollision
(2) Figury s¹ zdefiniowane w plikach obj.
(3) Po³o¿enie wyznaczane jest w programie w celu zachowania przejrzystoœci wyœwietlania.
(4) Tensor momentu bezw³adnoœci wylicza funkcja calculateMomentOfInertia na podstawie wspó³rzêdnych wierzcho³ków z pliku obj.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "controls.hpp"
#include "RigidBody.hpp"
#include "BSphere.hpp"

GLuint loadShaders(const char * vertex_file_path,const char * fragment_file_path){
 
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
 
    GLint Result = GL_FALSE;
    int maxInfoLogLength;
	int infoLogLength = 0;
 
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
 
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &maxInfoLogLength);
    GLchar* VertexShaderErrorMessage = new GLchar[maxInfoLogLength+1];
    glGetShaderInfoLog(VertexShaderID, maxInfoLogLength, &infoLogLength, VertexShaderErrorMessage);
    if(infoLogLength > 0)
		fprintf(stdout, "%s\n", VertexShaderErrorMessage);
	else
		fprintf(stdout, ".......... OK!\n");
 
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
 
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &maxInfoLogLength);
	GLchar* FragmentShaderErrorMessage = new GLchar[maxInfoLogLength+1];
	glGetShaderInfoLog(FragmentShaderID, maxInfoLogLength, &infoLogLength, FragmentShaderErrorMessage);
    if(infoLogLength > 0)
		fprintf(stdout, "%s\n", FragmentShaderErrorMessage);
	else
		fprintf(stdout, ".......... OK!\n");
 
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
 
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &maxInfoLogLength);
    GLchar* ProgramErrorMessage = new GLchar[maxInfoLogLength+1];
	glGetProgramInfoLog(ProgramID, maxInfoLogLength, &infoLogLength, ProgramErrorMessage);
    if(infoLogLength > 0)
		fprintf(stdout, "%s\n", ProgramErrorMessage);
	else
		fprintf(stdout, ".......... OK!\n");
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
 
    return ProgramID;
}

bool init()
{
	if(!glfwInit()){
		fprintf(stderr, "Failed GLFW\n");
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES,4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	if(!glfwOpenWindow(1024,768,0,0,0,0,32,0,GLFW_WINDOW)){
		fprintf(stderr, "Failed to open window\n");
		glfwTerminate();
		return -1;
	}

	glewExperimental = true;
	if(glewInit() != GLEW_OK){
		fprintf(stderr, "Failed GLEW\n");
		return -1;
	}

	glfwSetWindowTitle("Second Project");

	glfwEnable(GLFW_STICKY_KEYS);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE_ARB);
	glDepthFunc(GL_LESS);
	return 1;
}

int main(){
	if (init()==-1) return -1;

	glClearColor(0.f,0.f,0.f,0.f);

	GLuint VertexArrayID;
	glGenVertexArrays(1,&VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint programID = loadShaders( "vertexshader.txt", "fragmentShader.txt" );

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint AlphaID = glGetUniformLocation(programID, "Alpha");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	glm::mat4 Projection;
	glm::mat4 View;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	RigidBody rigidBody = RigidBody("pyramid2.obj","green.tga", GL_TRIANGLES, .8f, 3.f,4.f,4.f,4.f);
	BSphere sphere = BSphere("sphere.obj","blue.tga", GL_LINES, .5f);
	GLuint texture = loadTGA_glfw("green.tga");
	GLuint texture2 = loadTGA_glfw("red.tga");

	RigidBody cube = RigidBody("cube.obj","uvCube.tga", GL_TRIANGLES, .8f, 1.73f,2.f,2.f,2.f);
	cube.setPosition(15.f,0.f,0.f);
	BSphere sphere2 = BSphere("sphere3.obj","blue.tga", GL_LINES, .5f);

	glfwSetMousePos(1024/2, 768/2);
	do{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		computeMatricesFromInputs();
		Projection = getProjectionMatrix();
		View = getViewMatrix();

		glUseProgram(programID);

		
		rigidBody.draw(Projection,View,MatrixID,AlphaID,TextureID);
		rigidBody.beforeMove(0.001);
		rigidBody.move();
		rigidBody.drawCenter(Projection,View,MatrixID,AlphaID,TextureID);
		rigidBody.drawBox(Projection,View,MatrixID,AlphaID,TextureID);  //rysowanie OBB
		
		cube.draw(Projection,View,MatrixID,AlphaID,TextureID);
		cube.drawCenter(Projection,View,MatrixID,AlphaID,TextureID);
		//cube.drawBox(Projection,View,MatrixID,AlphaID,TextureID);

		sphere2.draw(Projection,View,MatrixID,AlphaID,TextureID);
		sphere2.setPosition(cube.getPosition());
		sphere2.move();

		if(!rigidBody.sphereCollision(cube.getRadius(),cube.getPosition())){
			sphere.setCenterOfMass(glm::vec3(0.f,-0.2f,0.f));
			sphere.draw(Projection,View,MatrixID,AlphaID,TextureID);
			sphere.setRotMatrix(rigidBody.getRotMatrix());
			sphere.setPosition(rigidBody.getPosition());
			sphere.move();
		}

		rigidBody.setTexture(texture);
		if(rigidBody.boxCollision(cube)){
			rigidBody.setTexture(texture2);
		}

		glfwSwapBuffers();
	}
	while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
		glfwGetWindowParam(GLFW_OPENED));
}