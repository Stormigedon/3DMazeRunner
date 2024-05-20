#ifndef SDLEVENTLISTENER_H
#define SDLEVENTLISTENER_H

#include <SDL2/SDL.h>
class SDLEventListener {
	public: 
		virtual bool eventUpdate(SDL_Event *event) {
			return false;
		}
};
#endif