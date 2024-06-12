#ifndef CAMERACONTROLLER
#define CAMERACONTROLLER
#include "SDLEventListener.h"
#include <glm/glm.hpp>

class Camera : public SDLEventListener {
	//public:
	// glm::vec3 position;
	// glm::vec3 front;
	// glm::vec3 top;
	// glm::vec3 right;
	// glm::vec3 motion;

	float yawAngle;
	float pitchAngle;
	glm::mat4 projection;
	glm::mat4 view;
	int rotation;
	float FOV;
	float aspectRatio;
	bool planeMode;
	void updateProjection();
	glm::vec3 position;
	glm::vec3 motion;
	glm::vec3 front;
	glm::vec3 top;
	glm::vec3 right;

	public:
		
		float sensitivity;
		float speed;
		bool eventUpdate(SDL_Event *event);

		void move(float deltaTime);
		void rotate(glm::vec3 axisOfRotation, float radians);
		void pitch(float radians);
		void yaw(float radians);
		void roll(float radians);


		void setPosition(glm::vec3 pos);
		void setFOV(float _FOV);
		void setAspectRatio(float _aspectRatio);
		void setXPosition(float T) {position.x = T;}
		void setYPosition(float T) {position.y = T;}
		void setZPosition(float T) {position.z = T;}

		glm::mat4 getProjection();
		glm::mat4 getView();
		glm::vec3 getPosition();
		glm::vec3 getFront();

		Camera(glm::vec3 _position, glm::vec3 _front, glm::vec3 _top, float _sensitivity, float _FOV, float _speed, float _aspectRatio, bool mode);
};
#endif