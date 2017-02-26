#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <fstream>
#include <string>

class Shader {
public:
	GLuint program;

	Shader(char* vFilename, char* fFilename) {
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);

		char* vertexCode = loadShader(vFilename);
		char* fragmentCode = loadShader(fFilename);

		if (!vertexCode || !fragmentCode) exit(-3);

		glShaderSource(vertex, 1, &vertexCode, NULL);
		glShaderSource(frag, 1, &fragmentCode, NULL);
		glCompileShader(vertex);
		glCompileShader(frag);

		program = glCreateProgram();
		glAttachShader(program, vertex);
		glAttachShader(program, frag);
		glLinkProgram(program);

		glDeleteShader(vertex);
		glDeleteShader(frag);
	}

private:
	char* loadShader(char* filename) {
		using namespace std;

		ifstream file(filename);
		if (file) {
			string str;
			char* buffer;
			int len;

			file.seekg(0, ios::end);
			len = file.tellg();
			str.reserve(len);
			file.seekg(0, ios::beg);
			str.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

			buffer = new char[len + 1];
			strcpy_s(buffer, len, str.c_str());
			buffer[len + 1] = 0;

			return buffer;

		}
		else {
			return 0;
		}
	}
};