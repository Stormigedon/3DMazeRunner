#ifndef APPLICATION_H
#define APPLICATION_H
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "SDLEventListener.h"
#include <iostream>
#include "shader.h"
#include <vector>
#include <map>
#include <string>
#include <algorithm>

class Application {
	//g
	SDL_Window *window;
	std::map<SDL_EventType, std::vector<SDLEventListener*>> EventListeners;
	static unsigned int count;
	unsigned int WINDOW_HEIGHT;
	unsigned int WINDOW_WIDTH;
	std::string WINDOW_NAME;
	bool running;

	//SDL_GL_CreateContext()
	public:
		Application(unsigned int height, unsigned int width, std::string name) : WINDOW_HEIGHT{height}, WINDOW_WIDTH{width}, WINDOW_NAME{name}{
			if (SDL_Init(SDL_INIT_VIDEO)) {
				printf ("SDL_Init Error: %s", SDL_GetError());
				abort();
			}

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
			window = SDL_CreateWindow(WINDOW_NAME.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, height, width,SDL_WINDOW_OPENGL); // | SDL_WINDOW_INPUT_GRABBED
			
			//SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_GLContext gl_context = nullptr;
			if(NULL == (gl_context = SDL_GL_CreateContext(window))) {
				std::cout<<"Failed to create OpenGL context"<<std::endl;
				abort();
			}
			else SDL_GL_MakeCurrent(window, gl_context);
			
			if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
				std::cout<<"Failed to intialize GLAD"<<std::endl;
				abort();
			}
			glViewport(0,0, WINDOW_HEIGHT, WINDOW_WIDTH);
			running = true;
		}

		void addSDLEventListener(SDLEventListener* Object, SDL_EventType SDL_EVENT_TYPE) {
			if(std::find(EventListeners[SDL_EVENT_TYPE].begin(), EventListeners[SDL_EVENT_TYPE].end(), Object) != EventListeners[SDL_EVENT_TYPE].end()) {
				return;
			}
			EventListeners[SDL_EVENT_TYPE].push_back(Object);
		}
		void getWindowProperties(unsigned int *height, unsigned int *width, std::string* name) {
			*name = WINDOW_NAME;
			*height = WINDOW_HEIGHT;
			*width = WINDOW_WIDTH;
		}

		bool update() {
			if(!running) return false;
			SDL_Event event;
			while(SDL_PollEvent(&event)) {
				switch (event.type){
				case SDL_QUIT:
					std::cout<<"Terminating program"<<std::endl;
					SDL_DestroyWindow(window);
					SDL_Quit();
					running = false;
					return false;
				case SDL_KEYDOWN:
					//std::cout<<"Key Code Pressed"<<event.key.keysym.sym<<std::endl;
					if(event.key.keysym.sym == SDLK_ESCAPE) {
						std::cout<<"Terminating program"<<std::endl;
						SDL_DestroyWindow(window);
						SDL_Quit();
						running = false;
						return false;
					}
				}
				//std::cout<<(EventListeners.find(SDL_EventType(event.type)) != EventListeners.end())<<std::endl;
				if(EventListeners.find(SDL_EventType(event.type)) != EventListeners.end()) {

					for(SDLEventListener* i: EventListeners[SDL_EventType(event.type)]) {
						i->eventUpdate(&event);
					}
				}
			}
			return true;
		}

		SDL_Window* getWindowPtr() {
			return window;
		}

		void quit() {
			std::cout<<"Terminating program"<<std::endl;
			SDL_DestroyWindow(window);
			SDL_Quit();
			running = false;
		}
};
#endif