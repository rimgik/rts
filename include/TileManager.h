#pragma once

// C++ standard lib
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <cstdlib>

// SDL2
#include "SDL2/SDL.h"

// include
#include "Camera.h"
#include "vec2d.h"
// #include "Pathfinding.h"
class Pathfinding;

class TileManager{
public:
	TileManager ();

	// Render the grid line, only render those are visible
	void Render(SDL_Renderer* renderer, Camera& cam) const;

	// World coor to grid coor
	vec2d<int> WorldToTile(vec2d<float> v) const;

	// gird coor to world coor 
	// 0 for center at the left uppper corner of the grid
	// 1 for center at the center of the grid
	vec2d<float> TileToWorld(vec2d<int> v, bool flag) const;

	// Set wall by wall grid
	void SetWallFromGrid(const vec2d<int>& v);

	// Set wall by world coor	
	void SetWallFromWorld(const vec2d<float>& v);	

	// Toggle wall 
	bool ToggleWallFromGrid(const vec2d<int>& v);
	bool ToggleWallFromWorld(const vec2d<float>& v);

	// check wall
	bool IsOccupiedFromGrid(const vec2d<int>& v) const;
	bool IsOccupiedFromWorld(const vec2d<float>& v) const;

	// Select tile by world coor
	void SelectTileFromWorld(const vec2d<float>& v);

	bool* GetGrid() const;

	// Getter
	int GetLengthOfGrid() const;

	bool Walkable(const vec2d<float>& src, const vec2d<float>& target) const;
	std::deque<vec2d<float>> OptimizePath(std::deque<vec2d<float>>& path) const;
	//pathfinding using real world coor
	std::deque<vec2d<float>> CalculatePath(const vec2d<float>& src, const vec2d<float>& target) const;
private:
	bool grid[100][100];
	// grid length in world
	int length = 50;

	int cnt = 0;
	std::vector<vec2d<int>> labelTile;

};