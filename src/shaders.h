#pragma once
char* vertexCode =
" #version 330 core \n "
" layout(location = 0) in vec3 position; \n "
" layout(location = 1) in vec3 color; \n "
"  \n "
" out vec3 vertexColor; //Output to the fragment shader \n "
"  \n "
" uniform mat4 model; \n "
" uniform mat4 view; \n "
" uniform mat4 projection; \n "
"  \n "
" void main() \n "
" { \n "
"       gl_Position = projection * view * model * vec4(position, 1.0f); \n "
"       vertexColor = color; \n "
" } \n ";

char* fragmentCode =
" #version 330 core \n "
" in vec3 vertexColor; \n "
" out vec4 color; \n "
"  \n "
" void main() \n "
" { \n "
"       color = vec4(vertexColor, 1.0f); \n "
" } \n ";

/*const GLchar* vertexCode = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";
const GLchar* fragmentCode = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";*/