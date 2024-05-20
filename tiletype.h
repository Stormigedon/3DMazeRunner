#ifndef TILE_TYPE_HEADER
#define TILE_TYPE_HEADER
#include <vector>
#include <random>
#include <algorithm>
/*
	this is the base class for all the different tile types.
*/
struct TileType {
	int ID;
	bool validNeighbors[64];
	int validNeighborsCount;
};

class Tile {
	bool isCollapsed;
	int finalID;
	std::vector<int> validCollapse;
	Tile *neighbors[4];
	static std::vector<TileType> tileList;

	void collapse() {
		int choice = random() % validCollapse.size();
		isCollapsed = true;
		finalID = validCollapse.at(choice);
		if(neighbors[0]!=NULL) {neighbors[0]->cascade();}
		if(neighbors[1]!=NULL) {neighbors[1]->cascade();}
		if(neighbors[2]!=NULL) {neighbors[2]->cascade();}
		if(neighbors[3]!=NULL) {neighbors[3]->cascade();}
	}
	void cascade() {
		if(isCollapsed) return;
		bool *validPointers[4];
		for(int i = 0; i < 4; i++) {
			if(neighbors[i] != NULL) {
				if(neighbors[i]->isCollapsed) {
					validPointers[i] = (tileList[neighbors[i]->finalID].validNeighbors);
				} else {
					validPointers[i] = new bool[64];
					//loop over the id space(there are optimizations that can be made here)
					//for each ID location, check each validCollapse ID's neighbor list
					// logical OR those values with the current valid list
					for(int j = 0; j < 64; j++) {
						for(std::vector<int>::iterator k = neighbors[i]->validCollapse.begin(); k != neighbors[i]->validCollapse.end(); k++) {
							validPointers[i][j] = validPointers[i][j] || tileList[*k].validNeighbors[j];
						}
					}
				}
			} else {
				validPointers[i] = NULL;
			}
		}
		bool *Union12 = NULL, *Union34 = NULL, *finalUnion;
		if(validPointers[0] && validPointers[1]) {
			ArrayUnion(validPointers[0], validPointers[1], Union12);
		}
		if(validPointers[2] && validPointers[3]) {
			ArrayUnion(validPointers[2], validPointers[3], Union34);
		}

		if(Union12) {
			if(Union34) {
				ArrayUnion(Union12, Union34, finalUnion);
			} else {//union34 is NULL
				finalUnion = Union12;
			}
		} else if(Union34) { //union12 MUST BE NULL
			finalUnion = Union34;
		} else {
			printf("THIS TILE HAS NO NEIGHBORS, THE UNIVERSE IS A LIE!");
			abort();
		}

		//finally check if the new validList is different
		bool changes = false;
		for(int j = 0; j < 64; j++) {
			std::vector<int>::iterator searchNode = std::find(validCollapse.begin(), validCollapse.end(), j);
			if(finalUnion[j]) {
				if(searchNode == validCollapse.end()) {
					validCollapse.push_back(j);
					changes = true;
				}
			} else { //this SHOULDN"T be in the valid list
				if(searchNode != validCollapse.end()) {
					validCollapse.erase(searchNode);
					changes = true;
				}
			}
		}
		//maybe add some path backwards tests here? 
		//maybe as an input pointer to the node that called cascade
		if(changes) {
			for(int i = 0; i < 4; i++) {
				if(neighbors[i] != NULL && !(neighbors[i]->isCollapsed)) {
					neighbors[i]->cascade();
				}
			}
		}
	}

	void ArrayUnion(bool *Array1, bool *Array2, bool *out) {
		out = new bool[64];
		for(int i = 0; i < 64; i++) {
			out[i] = Array1[i] & Array2[i];
		}
	}
	Tile() {
		finalID = -1;
		isCollapsed = false;
		validCollapse = std::vector<int>();
		neighbors[0] = NULL;
		neighbors[1] = NULL;
		neighbors[2] = NULL;
		neighbors[3] = NULL;
	}
};
#endif