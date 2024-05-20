#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS
#include <queue>


 template <typename T> void floodVisitNodes(T *startingNode) {
	std::queue<T> visitQueue;
	visitQueue.enque(startingNode);

	while(!visitQueue.empty()) {
		T* currNode = visitQueue.pop();
		currNode->visited = true;
		for(T::ittorator i = currNode.neighborsStart(); i != currNode.neighborsEnd(); i++) {
			if(i->visited == false) {
				visitQueue.push(i->node());
			}
		}
	}
}
#endif