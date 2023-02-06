#pragma once

#include <memory>

#include "SDL2/SDL.h"

#include "Camera.h"
#include "EntityManager.h"
#include "TileManager.h"

class EventManager{
public:
	EventManager();

	//process input
	void GetInput(Camera& cam, bool& gameIsRunning, EntityManager& entityManager, TileManager& map);
	// get drag mode square
	std::unique_ptr<SDL_Rect> GetRect();
	//singleton
	static EventManager& GetInstances();

	bool inSelector;
	bool isDragging;
private:
    std::unique_ptr<SDL_Rect> selectorRect = std::make_unique<SDL_Rect>();
    vec2d<int> mouseSelect;
	const Uint8* keys;	
    int mouse_x, mouse_y;
};