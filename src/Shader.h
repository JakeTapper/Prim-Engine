#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <fstream>
#include <string>

class Shader {
public:
	GLuint program; //OpenGL shader program loaded from the file

	Shader(char* vFilename, char* fFilename) {
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER); //Vertex program
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER); //Fragment program

		const char* vertexCode = loadShader(vFilename);	//Loads the file for vertex
		const char* fragmentCode = loadShader(fFilename);	//Loads the file for fragment

		if (!vertexCode || !fragmentCode) {
			printf("Error loading shaders! Files could not be opened!");
			exit(-3); //If either were not loaded, exit with code -3
		}
		//Create and compile the shaders
		glShaderSource(vertex, 1, &vertexCode, NULL);
		glShaderSource(frag, 1, &fragmentCode, NULL);
		glCompileShader(vertex);
		glCompileShader(frag);

		//Create the program and attach the compiled shaders to it
		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, frag);
		glLinkProgram(program);

		//Delete the shaders, since they are already in the program, which is what is used for rendering
		glDeleteShader(vertex);
		glDeleteShader(frag);

	}

private:
	//Helper function for loading the text from a file
	char* loadShader(char* filename) {
		using namespace std;

		ifstream file(filename); //Holdes the file data
		if (file) {
			string str;		//Read data
			char* buffer;	//Read data in the return format
			int len;		//Length of the file

			//Find the file length, then read that lenght of data to str
			file.seekg(0, ios::end);
			len = file.tellg();
			str.reserve(len);
			file.seekg(0, ios::beg);
			str.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

			//Copy the string data to the buffer
			buffer = (char*) malloc(len + 1);
			if (!buffer) exit(-4);
			strcpy_s(buffer, len, str.c_str());
			buffer[len + 1] = 0;

			//Return the file data
			return buffer;

		}
		else {
			return 0;
		}
	}
};