#include "MazeSegment3D.h"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glad.c"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float MazeSegment3D::DeadEndVertexData[] = {
	// positions          // normals           // texture coords
	//south wall
	-0.4999f, -0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.4999f, -0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  0.0f,
	 0.4999f,  0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	 0.4999f,  0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	-0.4999f,  0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.4999f, -0.5f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

	// north wall
	// -0.4999f, -0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	//  0.4999f, -0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.5f,  0.0f,
	//  0.4999f,  0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.5f,  1.0f,
	//  0.4999f,  0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.5f,  1.0f,
	// -0.4999f,  0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
	// -0.4999f, -0.5,  0.4999f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

	//west wall
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	-0.4999f,  0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  0.0f,
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,

	//east wall
	 0.4999f,  0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	 0.4999f,  0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.4999f, -0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.4999f, -0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.4999f, -0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  0.0f,
	 0.4999f,  0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  1.0f,

	//floor
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f
};

const float MazeSegment3D::StraightVertexData[] = {
	// positions          // normals           // texture coords
	//west wall
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	-0.4999f,  0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  0.0f,
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,

	//east wall
	 0.4999f,  0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	 0.4999f,  0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	 0.4999f, -0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.4999f, -0.5f, -0.4999f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	 0.4999f, -0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  0.0f,
	 0.4999f,  0.5f,  0.4999f, -1.0f,  0.0f,  0.0f,  0.5f,  1.0f,

	//floor
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f
};

const float MazeSegment3D::CornerVertexData[] = {
	// positions          // normals           // texture coords
	//south wall
	-0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  0.0f,
	 0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	 0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	-0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	//west wall
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	-0.4999f,  0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f, -0.4999f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
	-0.4999f, -0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  0.0f,
	-0.4999f,  0.5f,  0.4999f,  1.0f,  0.0f,  0.0f,  0.5f,  1.0f,
	//floor
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f
};

const float MazeSegment3D::TSectionVertexData[] = {
	// positions          // normals           // texture coords
	//south wall
	-0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  0.0f,
	 0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	 0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.5f,  1.0f,
	-0.4999f,  0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.4999f, -0.4999f, -0.4999f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	//floor
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f
};

const float MazeSegment3D::FourWayVertexData[] = {
	// positions          // normals           // texture coords
	//floor
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  1.0f
};

bool MazeSegment3D::initialized = false;

unsigned int MazeSegment3D::DeadEndVAO = 0;
unsigned int MazeSegment3D::StraightVAO = 0;
unsigned int MazeSegment3D::CornerVAO = 0;
unsigned int MazeSegment3D::TSectionVAO = 0;
unsigned int MazeSegment3D::FourWayVAO = 0;

unsigned int MazeSegment3D::DeadEndVBO = 0;
unsigned int MazeSegment3D::StraightVBO = 0;
unsigned int MazeSegment3D::CornerVBO = 0;
unsigned int MazeSegment3D::TSectionVBO = 0;
unsigned int MazeSegment3D::FourWayVBO = 0;
//what if all programs are console programs from now on. 
unsigned int MazeSegment3D::textureID = 0;

void MazeSegment3D::draw(Shader *shader) {
	shader->use();
	shader->setInt("texture1",textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(X,0,Y));
	if(type == FOURWAYINTERSECTION) {
		glBindVertexArray(FourWayVAO);
		shader->setMat4("model",model);
		//std::cout<<glGetError()<<std::endl;
		glDrawArrays(GL_TRIANGLES, 0, 12); //6 * number of faces
	} else if(type == TSECTIONEAST || type == TSECTIONNORTH || type == TSECTIONSOUTH || type == TSECTIONWEST) {
		glBindVertexArray(TSectionVAO);
		switch(type) {
			case TSECTIONEAST:
				model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case TSECTIONNORTH:
				model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case TSECTIONWEST:
				model = glm::rotate(model,glm::radians(270.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			default:
				break;
		}
		shader->setMat4("model",model);
		glDrawArrays(GL_TRIANGLES, 0, 18); //6 * number of faces
	} else if (type == CORNERNORTHEAST || type == CORNERNORTHWEST || type == CORNERSOUTHEAST || type == CORNERSOUTHWEST) {
		glBindVertexArray(CornerVAO);
		switch(type) {
			case CORNERNORTHEAST:
				model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case CORNERSOUTHWEST:
				model = glm::rotate(model,glm::radians(270.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case CORNERNORTHWEST:
				model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			default:
				break;
		}
		shader->setMat4("model",model);
		glDrawArrays(GL_TRIANGLES, 0, 24); //6 * number of faces
	} else if (type == DEADENDEAST || type == DEADENDWEST || type == DEADENDNORTH || type == DEADENDSOUTH) {
		glBindVertexArray(DeadEndVAO);
		switch(type) {
			case DEADENDEAST:
				model = glm::rotate(model,glm::radians(270.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case DEADENDNORTH:
				model = glm::rotate(model,glm::radians(180.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			case DEADENDWEST:
				model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
				break;
			default:
				break;
		}
		shader->setMat4("model",model);
		glDrawArrays(GL_TRIANGLES, 0, 30);
	} else if (type == VERTICALSECTION || type == HORIZONTALSECTION) {
		glBindVertexArray(StraightVAO);
		if(type == HORIZONTALSECTION) {
				model = glm::rotate(model,glm::radians(90.0f),glm::vec3(0.0f,1.0f,0.0f));
		}
		shader->setMat4("model",model);
		glDrawArrays(GL_TRIANGLES, 0, 24);
	}
	glBindVertexArray(0);
}

bool MazeSegment3D::intialize() {
	initialized = true;
	//because opengl context is needed and initialize should check for that shit probably, or atleast not happend until one exists
	textureID = loadTexture("./MazeTexture.png");
	
	glGenBuffers(1, &DeadEndVBO);
	glGenVertexArrays(1, &DeadEndVAO);

	glBindVertexArray(DeadEndVAO);
	glBindBuffer(GL_ARRAY_BUFFER, DeadEndVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DeadEndVertexData), DeadEndVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
	glBindVertexArray(0);
	
	glGenBuffers(1, &StraightVBO);
	glGenVertexArrays(1, &StraightVAO);

	glBindVertexArray(StraightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, StraightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(StraightVertexData), StraightVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
	glBindVertexArray(0);

	glGenBuffers(1, &CornerVBO);
	glGenVertexArrays(1, &CornerVAO);

	glBindVertexArray(CornerVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CornerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CornerVertexData), CornerVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
	glBindVertexArray(0);

	glGenBuffers(1, &TSectionVBO);
	glGenVertexArrays(1, &TSectionVAO);

	glBindVertexArray(TSectionVAO);
	glBindBuffer(GL_ARRAY_BUFFER, TSectionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TSectionVertexData), TSectionVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
	glBindVertexArray(0);

	glGenBuffers(1, &FourWayVBO);
	glGenVertexArrays(1, &FourWayVAO);

	glBindVertexArray(FourWayVAO);
	glBindBuffer(GL_ARRAY_BUFFER, FourWayVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(FourWayVertexData), FourWayVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)0);
	glEnableVertexAttribArray(1);	
	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*3));
	glEnableVertexAttribArray(2);	
	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, sizeof(float)*8, (void*)(sizeof(float)*6));
	glBindVertexArray(0);

	return true;
}

unsigned int MazeSegment3D::loadTexture(char const * path) {
	stbi_set_flip_vertically_on_load(true); 
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
		//not sure whye I have to explicitly activate texture 1, texture 0 is all black for some reason *shrug*
		glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		std::cout<<"Potential Error code from texture " <<glGetError()<<std::endl;
		std::cout<<"texture Width and Hight " <<width<<","<<height<<std::endl;
		std::cout<<"texture format " <<format<<std::endl;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}