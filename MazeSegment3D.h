#ifndef MAZE_SEGMENT_3D_HEADER
#define MAZE_SEGMENT_3D_HEADER

#include "shader.h"
class MazeSegment3D {
	private:
		static bool initialized;
		static unsigned int textureID;

		static const float DeadEndVertexData[];
		static const float StraightVertexData[];
		static const float CornerVertexData[];
		static const float TSectionVertexData[];
		static const float FourWayVertexData[];

		static unsigned int DeadEndVAO;
		static unsigned int StraightVAO;
		static unsigned int CornerVAO;
		static unsigned int TSectionVAO;
		static unsigned int FourWayVAO;

		static unsigned int DeadEndVBO;
		static unsigned int StraightVBO;
		static unsigned int CornerVBO;
		static unsigned int TSectionVBO;
		static unsigned int FourWayVBO;

		static unsigned int loadTexture(char const * path);
		
	public:
		enum SegmentType {FOURWAYINTERSECTION,TSECTIONEAST,TSECTIONNORTH,CORNERNORTHEAST,TSECTIONWEST,VERTICALSECTION,CORNERNORTHWEST,
						  DEADENDNORTH,TSECTIONSOUTH,CORNERSOUTHEAST,HORIZONTALSECTION,DEADENDWEST,CORNERSOUTHWEST,DEADENDSOUTH,DEADENDEAST,BUGGERED};

		int X, Y;
		SegmentType type;
		
		void draw(Shader *shader);
		bool intialize();

		MazeSegment3D() {
			if(!initialized) {
				intialize();
			}
			X = 0;
			Y = 0;
			type = BUGGERED;
		}

		MazeSegment3D(int _X, int _Y) : X{_X}, Y{_Y} {
			if(!initialized) {
				intialize();
			}
			type = static_cast<SegmentType>(15);
		}
};
#endif