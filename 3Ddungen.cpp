#include <iostream>
#include <stack>
#include <map>
#include <random>
#include <chrono>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MazeSegment3D.h"
#include "shader.h"
#include "application.h"
#include "camera.h"
#include "sphere.h"

using namespace std;

struct cell{
	int X, Y;
	unsigned int type;
	int hash;
};

std::string getMazeCharFromMask(unsigned int mask) {
	//N E S W
	mask = mask & 15;
	switch(mask) {
		case 0: //0000
			return "╬"; //╬
		case 1: //0001
			return "╠";
		case 2: //0010
			return "╩";
		case 3: //0011
			return "╚";
		case 4: //0100
			return "╣";
		case 5: //0101
			return "║";
		case 6: //0110	
			return "╝";
		case 7: //0111
			return "╨";
		case 8: //1000
			return "╦";
		case 9: //1001
			return "╔";
		case 10: //1010
			return "═";
		case 11: //1011
			return "╞";
		case 12: //1100
			return "╗";
		case 13: //1101
			return "╥";
		case 14: //1110
			return "╡";
		case 15: //1111
			return "╳";
	}
	return "╳";
}

cell** generateMaze(int size){
	std::stack<cell*> stack;
	//std::map<int, cell*> visited;
	//visited doesn't have to be a dynamic size, we already know the end size, just use a bool array
	bool visited[size*size];
	cell MazeGrid[size][size];
	//std::srand(124832256);
	std::srand(std::chrono::duration_cast<std::chrono::seconds>((std::chrono::system_clock::now()).time_since_epoch()).count());
	
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			MazeGrid[i][j].X = i;
			MazeGrid[i][j].Y = j;
			MazeGrid[i][j].hash = i + j*size;
			MazeGrid[i][j].type = 15;
			visited[i + j*size] = false;
		}	
	}

	stack.push(&MazeGrid[rand() % size][rand() % size]);

	while(!stack.empty()) {
		cell* options[4];
		int optionsCount = 0;
		cell* currCell = stack.top();
		stack.pop();

		visited[currCell->hash] = true;

		if(currCell->X > 0) {
			if(!visited[MazeGrid[currCell->X - 1][currCell->Y].hash]) {
				options[optionsCount] = &MazeGrid[currCell->X - 1][currCell->Y];
				optionsCount++;
			}
		}
		if(currCell->X < (size-1)) {
			if(!visited[MazeGrid[currCell->X + 1][currCell->Y].hash]) {
				options[optionsCount] = &MazeGrid[currCell->X + 1][currCell->Y];
				optionsCount++;
			}
		}
		if(currCell->Y > 0) {
			if(!visited[MazeGrid[currCell->X][currCell->Y - 1].hash]) {
				options[optionsCount] = &MazeGrid[currCell->X][currCell->Y - 1];
				optionsCount++;
			}
		}
		if(currCell->Y < (size-1)) {
			if(!visited[MazeGrid[currCell->X][currCell->Y + 1].hash]) {
				options[optionsCount] = &MazeGrid[currCell->X][currCell->Y + 1];
				optionsCount++;
			}
		}

		if(optionsCount > 0) {
			int choice = std::rand() % optionsCount;
			if(options[choice]->X == currCell->X) {
				if(options[choice]->Y < currCell->Y) { //traveling through current North Gate
					options[choice]->type &= 0b1101;
					currCell->type        &= 0b0111;
				} else { //traveling through current South gate
					options[choice]->type &= 0b0111;
					currCell->type        &= 0b1101;
				}
			} else { //if they are different we know to compair X
				if(options[choice]->X > currCell->X) { //traveling through current East Gate
					options[choice]->type &= 0b1110;
					currCell->type        &= 0b1011;
				} else { //traveling through current West gate
					options[choice]->type &= 0b1011;
					currCell->type        &= 0b1110;
				}
			}
			stack.push(currCell);
			stack.push(options[choice]);
		}
	}

	cell** out = new cell *[size];
	for(int i = 0; i < size; i++) {
		out[i] = new cell[size];
        for (int j = 0; j < size; ++j) {
            out[i][j] = MazeGrid[i][j];
        }		
	}

	return out;
}


int main() {
	std::cout<<"desired Maze size(min:3, max:25) q to quit"<<std::endl;
	int size;
	//todo be graceful here :'D
	std::cin>>size;

	//size = 7;
	if(size > 25) {size = 25;}
	if(size < 3) {size = 3;}
	std::cout<<"Loading Maze"<<std::endl;
	std::cout<<"\tgenerating maze of size: "<<size<<std::endl;
	cell** finalMaze;
	finalMaze = generateMaze(size);

	int Xs = std::rand() % size;
	int Ys = std::rand() % size;
	int Xe = std::rand() % size;
	int Ye = std::rand() % size;

	for(int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			if(Xe == i && Ye == j) {
				std::cout<<"E";
			} else {
				std::cout<<getMazeCharFromMask(finalMaze[i][j].type);
			}
		}
		std::cout<<std::endl;
	}

	Application apphoe(800,600,"Maze 3D X-treme");
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_FALSE);
	Camera camera(glm::vec3(0, 0, 0), glm::vec3(0,0,1),glm::vec3(0,1,0), 0.01,75,2.5,800/600,true);
	
	apphoe.addSDLEventListener(&camera, SDL_KEYDOWN);
	apphoe.addSDLEventListener(&camera, SDL_KEYUP);
	apphoe.addSDLEventListener(&camera, SDL_MOUSEMOTION);

	Shader mazeShade("./vertexShader.vs","./fragmentShader.fs");

	Sphere *steve = Sphere::getInstance();

	MazeSegment3D Maze[size][size];
	std::cout<<"\tmapping IDs..."<<std::endl;
	for(int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			//explicit converstion to the enum type for my maze.  wonder if there's a cleaner looking method for doing this
			Maze[i][j].X = i;
			Maze[i][j].Y = j;
			Maze[i][j].type = static_cast<MazeSegment3D::SegmentType>(finalMaze[i][j].type);
		}
		std::cout<<"\tmapping IDs " << (int)(((float)(j+1.0f)/size)*100) << "%"<<std::endl;
	}
	
	//star date 2024-05-08 11.05 I can finally start rendering things, after ALLL of that other work
	//rendering FINALLY funcitons 2024-05-20 ~06.27 I hate textures now. 
	SDL_Window *window = apphoe.getWindowPtr();
	SDL_ShowWindow(window);
	float R=0.2f, G=0.3f, B=0.3f;
	glClearColor(0.0f, 0.01f, 0.06f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	unsigned long then;
	float deltaTime = 1;
	int playerX, playerY;

	while(apphoe.update()) {
		then = SDL_GetTicks();
		camera.move(deltaTime);
		//do some simple collision detection
		playerX = (int)(camera.getPosition().x+0.5);
		playerY = (int)(camera.getPosition().z+0.5);
		MazeSegment3D occupiedSection = Maze[playerX][playerY];
		if((camera.getPosition().x - playerX) < -0.35 && (occupiedSection.type & 0b0001)) {
			camera.setXPosition(playerX - 0.35);
		}	
		if((camera.getPosition().x - playerX) > 0.35 && (occupiedSection.type & 0b0100)) {
			camera.setXPosition(playerX + 0.35);
		}
		if((camera.getPosition().z - playerY) < -0.35 && (occupiedSection.type & 0b1000)) {
			camera.setZPosition(playerY - 0.35);
		}
		if((camera.getPosition().z - playerY) > 0.35 && (occupiedSection.type & 0b0010)) {
			camera.setZPosition(playerY + 0.35);
		}


		
		//debuging collision
		// std::cout<<"Maze Position: "<<playerX<<","<<playerY<<std::endl;
		// std::cout<<"Cube Position: "<<(camera.getPosition().x - playerX)<<","<<(camera.getPosition().z - playerY)<<std::endl;
		// std::cout<<"Camera Position: "<<camera.getPosition().x<<","<<camera.getPosition().y<<","<<camera.getPosition().z<<std::endl;
		// std::cout<<"Cube Walls: \n\tWest"<<(occupiedSection.type & 0b0001)<<"\n\tEast"<<((occupiedSection.type & 0b0100)>>2)<<"\n\tSouth"
		// 	<<((occupiedSection.type & 0b0010)>>1)<<"\n\tNorth"<<((occupiedSection.type & 0b1000)>>3)<<std::endl;
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		mazeShade.use();
		mazeShade.setMat4("projection", camera.getProjection());
		mazeShade.setMat4("view", camera.getView());
		mazeShade.setVec3("viewPos", camera.getPosition());
		for(int j = 0; j < size; j++) {
			for (int i = 0; i < size; i++) {
				//explicit converstion to the enum type for my maze.  wonder if there's a cleaner looking method for doing this
				Maze[i][j].draw(&mazeShade);
			}
		}


		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model,glm::vec3(Xe,0.0,Ye));
		model = glm::scale(model,glm::vec3(0.25));
		mazeShade.setMat4("model",model);
		steve->draw(&mazeShade);


		//bearing spheres for debuging
		// model = glm::mat4(1.0f);
		// model = glm::translate(model,glm::vec3(0.0,0.0,8.0));
		// mazeShade.setMat4("model",model);
		// steve->draw(&mazeShade);

		// model = glm::mat4(1.0f);
		// model = glm::translate(model,glm::vec3(8.0,0.0,8.0));
		// mazeShade.setMat4("model",model);	
		// steve->draw(&mazeShade);

		// model = glm::mat4(1.0f);
		// model = glm::translate(model,glm::vec3(8.0,0.0,0.0));
		// mazeShade.setMat4("model",model);
		// steve->draw(&mazeShade);

		SDL_GL_SwapWindow(window);
		deltaTime = (SDL_GetTicks() - then) / 1000.0f;

		//last thing to do, check if your in the success sphere!
		// do this at the end to avoid a seg fault lol. :'D
		if(playerX == Xe && playerY == Ye) {
			std::cout<<"YOU WIN!!!"<<std::endl;
			apphoe.quit();
			//std::cout<<apphoe.update()<<std::endl;
		}
		//std::cout<<"Delta: "<<deltaTime<<std::endl;
	}
}