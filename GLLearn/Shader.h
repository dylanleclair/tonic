#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderTwo
{

private:
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource =
		"#version 330 core\n"
		"out vec4 FragColor;\n;"
		"in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"FragColor = vertexColor;\n"
		"}\0";


public:
	unsigned int shader_program; // the shader program


	void compile() {
		// start with the vertexShader
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER); // pass in GL_VERTEX_SHADER since this is the type we are making
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr); // object to compile is 1st arg, second specifies how many strings, 3rd is source, 4th can be left null
		glCompileShader(vertexShader);

		// see if it was successful!

		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
			std::cout << "Vertex shader compilation failed.\n";

		}

		// compile fragment shader
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);

		char info_frag[512];
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, nullptr, info_frag);
			std::cout << "Fragment shader compilation failed.\n";
		}

		shader_program = glCreateProgram();
		glAttachShader(shader_program, vertexShader);
		glAttachShader(shader_program, fragmentShader);

		glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
			std::cout << "Failed to link shaders.";
		}



		// now, we no longer need the shaders as they have been compiled and linked
		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);


	}



};

class Shader {
public: 
	// the program ID
	unsigned int ID;

	// constructor reads and builds the shader!
	Shader(const char* vertexPath, const char* fragmentPath); // take paths as c strings

	// use the shader (wraps glUseProgram)
	void use();

	// utility uniform wrappers
	void setBool(const std::string& name, bool) const;
	void setInt(const std::string& name, int) const;
	void setFloat(const std::string& name, float) const;


	Shader(const char* vertexPath, const char* fragmentPath) {
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// compile and link the shaders!
		unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

		int success;

		char infoLog[512];

		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glShaderSource(fragment, 1, &fShaderCode, NULL);

		glCompileShader(vertex);
		glCompileShader(fragment);

			

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		// check for shader compile errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		// check for linking errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// delete the old programs
		glDeleteShader(vertex); 
		glDeleteShader(fragment);

	} // end constructor



};




#endif