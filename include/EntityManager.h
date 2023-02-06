#pragma once

// C++ standard library
#include <memory>
#include <vector>
#include <limits>

// SDL library
#include "SDL2/SDL.h"

// include
#include "vec2d.h"
#include "Entity.h"
#include "Camera.h"
#include "TileManager.h"

class EntityManager
{
public:
	EntityManager(SDL_Renderer* r);
	~EntityManager();

	void ClearAllEntity();
	void ClearSelectedEntity();
	void UpdateEntity(const Uint64& timeElapsed);
	void RenderEntity(SDL_Renderer* renderer, Camera &cam);

	// Move selected entity directly go x (don't use this as this doesn't use path finding)
	void MoveSelectedEntity(const vec2d<float>& x);

	// Path finding to destination, and move
	void SelectedEntityGoTo(const vec2d<float>& v, TileManager& tm);

	void SelectedEntityAddPath(const vec2d<float>& x);

	void SetShowDestination(const bool& x);

	void SelectEntityByRect(SDL_Rect* rect, Camera& cam);
	
	void SelectEntityByPoint(SDL_Point* point, Camera& cam);

	bool HasSelectedEntity();

private:

	std::vector<std::shared_ptr<Entity>> entityVec;
	std::vector<std::shared_ptr<Entity>> selectedEntity;
};