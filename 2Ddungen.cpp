#include <iostream>
#include <stack>
#include <map>
#include <random>
#include <chrono>
#include <string>

using namespace std;

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

struct cell{
	int X, Y;
	unsigned int type;
	int hash;
};

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

			// for(int i = 0; i<size; i++) {
			// 	for(int j = 0; j<size; j++) {
			// 		std::cout<<getMazeCharFromMask(MazeGrid[j][i].type);
			// 	}
			// 	std::cout<<std::endl;
			// }


			// std::cout<<std::endl;
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
	std::cout<<"desired Maze size(max 25) q to quit"<<std::endl;
	int size;
	//todo be graceful here :'D
	std::cin>>size;
	if(size > 25) {size = 25;}
	cell** finalMaze;
	finalMaze = generateMaze(size);
	int Xs = std::rand() % size;
	int Ys = std::rand() % size;
	int Xe = std::rand() % size;
	int Ye = std::rand() % size;
	for(int j = 0; j < size; j++) {
		for (int i = 0; i < size; i++) {
			if(i == Xs && j == Ys) {
				std::cout<<"S";
			} else if (i == Xe && j == Ye) {
				std::cout<<"E";
			} else {
				std::cout<<getMazeCharFromMask(finalMaze[i][j].type);
			}
		}
		std::cout<<std::endl;
	}
	
}