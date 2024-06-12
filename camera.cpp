#include "camera.h"
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

void Camera::rotate(glm::vec3 axisOfRotation, float radians){
	glm::quat quaternion = glm::quat(std::cos(radians/2),std::sin(radians/2) * glm::normalize(axisOfRotation));
	front = glm::normalize(quaternion * front);
	top = glm::normalize(quaternion * top);
	right = glm::normalize(quaternion * right);
}

void Camera::pitch(float radians){
	rotate(right,radians);
}

void Camera::yaw(float radians){
	rotate(top,radians);
}

void Camera::roll(float radians){
	rotate(front,radians);
}

void Camera::move(float deltaTime) {
	// if(rotation != 0) {
	// 	roll(rotation * sensitivity);
	// }
	//this avoids normalizing a zero vector, very bad, much NaN.
	if((motion.x == 0 && motion.y == 0 && motion.z == 0)) return;
	glm::vec3 normalMotion = glm::normalize(motion) * speed;
	//std::cout<<"normalMotion: ("<<normalMotion.x<<","<<normalMotion.y<<","<<normalMotion.z<<")"<<std::endl;
	if(planeMode) {normalMotion.y = 0;}
	position = position + ((normalMotion.x * front) + (normalMotion.y * top) + (normalMotion.z * right)) * deltaTime;
	if(planeMode) {
		position.y = 0;
	}
}

void Camera::setPosition(glm::vec3 pos) {
	position = pos;
}

void Camera::setFOV(float _FOV) {
	FOV = _FOV;
	updateProjection();
}
void Camera::setAspectRatio(float _aspectRatio) {
	aspectRatio = _aspectRatio;
	updateProjection();
}
glm::mat4 Camera::getProjection() {
	return projection;
}
glm::mat4 Camera::getView() {
	return glm::lookAt(position, position + front, top);
}

glm::vec3 Camera::getPosition() {
	return position;
}


Camera::Camera(glm::vec3 _position, glm::vec3 _front, glm::vec3 _top, float _sensitivity, float _FOV, float _speed, float _aspectRatio, bool mode) {
	position = _position;
	front = _front;
	top = _top;
	right = glm::cross(_front,_top);
	sensitivity = _sensitivity;
	speed = _speed;
	aspectRatio = _aspectRatio;
	FOV = _FOV;
	motion = glm::vec3(0.0f);
	rotation = 0;
	planeMode = mode;
	pitchAngle = std::acos(_front.y);
	yawAngle = std::acos((_front.x / std::sin(pitchAngle)));
	updateProjection();
}

void Camera::updateProjection() {
	projection = glm::perspective(glm::radians(FOV), aspectRatio,0.1f, 100.0f);
}

bool Camera::eventUpdate(SDL_Event *event) {
	switch (event->type) {
		case SDL_KEYDOWN:
			if(!event->key.repeat) {
				switch (event->key.keysym.sym) {
					//todo:  Make this a state machine.
					case SDLK_w:
						motion.x += 1;
						break;
					case SDLK_s:
						motion.x -= 1;
						break;
					case SDLK_a:
						motion.z -= 1;
						break;
					case SDLK_d:
						motion.z += 1;
						break;
					case SDLK_SPACE:
						motion.y += 1;
						break;
					case SDLK_c:
						motion.y -= 1;
						break;
					case SDLK_q:
						rotation -= 1;
						break;
					case SDLK_e:
						rotation += 1;
						break;
				}
			}
			break;
		case SDL_KEYUP:
			switch (event->key.keysym.sym) {
				case SDLK_w:
						motion.x += -1;
						break;
					case SDLK_s:
						motion.x -= -1;
						break;
					case SDLK_a:
						motion.z -= -1;
						break;
					case SDLK_d:
						motion.z += -1;
						break;
					case SDLK_SPACE:
						motion.y += -1;
						break;
					case SDLK_c:
						motion.y -= -1;
						break;
					case SDLK_q:
						rotation -= -1;
						break;
					case SDLK_e:
						rotation += -1;
						break;
			}
			break;
		case SDL_MOUSEMOTION:
			pitchAngle += event->motion.yrel * sensitivity;
			//std::cout<<pitchAngle<<std::endl;
			if(pitchAngle > glm::radians(179.0f)) {pitchAngle = glm::radians(179.0f);}
			else if(pitchAngle < 0.01f) {pitchAngle = 0.01f;}

			yawAngle += event->motion.xrel * sensitivity;
			if(yawAngle >= 6.28318) {yawAngle = 0;}
			else if(yawAngle < 0) {yawAngle = 6.28318;}

			front.x = std::sin(pitchAngle) * std::cos(yawAngle);
			front.z = std::sin(pitchAngle) * std::sin(yawAngle);
			front.y = std::cos(pitchAngle);

			right = glm::cross(front, glm::vec3(0.0f,1.0f,0.0f));
			top = glm::cross(right, front);
			// i'll have to add a fix based on some world malarky later
			// glm::vec2 mouseMovement;
			// mouseMovement = glm::vec2(event->motion.xrel,event->motion.yrel);
			// glm::vec3 rotationAxis;
			// rotationAxis = glm::normalize(mouseMovement.x * top + mouseMovement.y * right);
			// rotate(rotationAxis, mouseMovement.length() * -sensitivity);

			// pitch(event->motion.yrel * -sensitivity);
			// yaw(event->motion.xrel * -sensitivity);
			break;
		default:
			break;
	}

	//std::cout<<"Motion: ("<<motion.x<<","<<motion.y<<","<<motion.z<<")"<<std::endl;
	return true;
}

glm::vec3 Camera::getFront() {
	return front;
}