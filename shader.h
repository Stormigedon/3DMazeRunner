#ifndef SHADER_H
#define SHADER_H

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>


class Shader {
    public:
        unsigned int ID;

        //loading constructor
        Shader(const char* vertexPath, const char* fragmentPath) {
			if(SDL_GL_GetCurrentContext() == NULL) {
				std::cerr << "OPEN GL CONTEXT NOT DEFINED! ABORTING" << std::endl;
				abort();
			}
            std::ifstream vertexFile;
            std::ifstream fragmentFile;
            std::string vertexCode, fragCode;
            vertexFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
            fragmentFile.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
            std::cerr << "Loading Vertex Shader" << std::endl;
            //this feels like loading the same data 3 times but it's from the book so what ever *shrug*
            try {
                //this code from the book could be a bit more streamlined;
                //open the vertex file as a file stream object (why not directly open it as a stringstream?)
                vertexFile.open (vertexPath, std::ifstream::in);
                //take the data buffer and put it into a string stream so you have access to the .str() function
                std::stringstream vertexStream;
                vertexStream << vertexFile.rdbuf();
                vertexFile.close();

                vertexCode = vertexStream.str();
            }
            catch (std::ifstream::failure e) {
                std::cerr << "FAILED TO LOAD VERTEX SHADER FILE: " << vertexPath;
                abort();
            }

			std::cerr << "Loading Fragment Shader" << std::endl;
            try {
                //this code from the book could be a bit more streamlined;
                //open the vertex file as a file stream object (why not directly open it as a stringstream?)
                fragmentFile.open (fragmentPath, std::ifstream::in);
                //take the data buffer and put it into a string stream so you have access to the .str() function
                std::stringstream fragStream;
                fragStream << fragmentFile.rdbuf();
                fragmentFile.close();

                fragCode = fragStream.str();
            }
            catch (std::ifstream::failure e) {
                std::cerr << "FAILED TO LOAD FRAGMENT SHADER FILE: " << fragmentPath;
                abort();
            }

			std::cerr<<"poopy Boopy"<<__LINE__<<std::endl;
            const char *vertexShaderSource = vertexCode.c_str();
            const char *fragmentShaderSource = fragCode.c_str();
            //why doesnt this work?
            //const char *vertexShaderSource = vertexStream.str().c_str();
            //const char *fragmentShaderSource = fragStream.str().c_str();

            int success;
            char infoLog[512];

            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
            glCompileShader(vertexShader);
            // check for shader compile errors
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                std::cerr << vertexShaderSource << std::endl;
                abort();
            }
            // fragment shader
            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
            glCompileShader(fragmentShader);
            // check for shader compile errors
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                abort();
            }

            // link shaders
            ID = glCreateProgram();
            glAttachShader(ID, vertexShader);
            glAttachShader(ID, fragmentShader);
            glLinkProgram(ID);
            // check for linking errors
            glGetProgramiv(ID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(ID, 512, NULL, infoLog);
                std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                abort();
            }
            //clean up, clean up, everybody clean up
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

			std::cerr << "Finished Loading Shaders" << std::endl;
        }
        //enable shader and make active
        void use() {
            glUseProgram(ID);
        }
        //update uniform values
        void setBool(const char* name, bool value) const {
            glUniform1i(glGetUniformLocation(ID, name), value);
        }

        void setInt(const char* name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name), value);
        }

        void setFloat(const char* name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name), value);
        }

		void setVec3(const char* name, glm::vec3 value) const {
			glUniform3fv(glGetUniformLocation(ID, name),1,glm::value_ptr(value));
		}

		void setMat4(const char* name, glm::mat4 value) const {
			glUniformMatrix4fv(glGetUniformLocation(ID, name),1, GL_FALSE, glm::value_ptr(value));
		}
};

#endif