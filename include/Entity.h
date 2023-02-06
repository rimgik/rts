#pragma once

// C++ standard library
#include <vector>
#include <deque>

//SDL library
#include "SDL2/SDL.h"

// box2d library
#include "box2d/box2d.h"

//include
#include "ResourceManager.h"
#include "vec2d.h"
#include "Camera.h"
#include "TileManager.h"

class Entity{
public:	
	Entity(float x, float y, float w, float h);
	~Entity();

	// Set Texture for entity, x,y,w,h are attribute for texture 
	void SetTexture(SDL_Texture *texture, int x, int y, int w, int h);

	// Set Texture for entity, x,y,w,h are default, i.e whole texture
	void SetTexture(SDL_Texture *texture);

	void Render(SDL_Renderer* , Camera &cam);
	void Update(Uint64 timeElapsed);

	//get destination
	std::deque<vec2d<float>>& GetDest();

	//get pos in real world
	// 0-> center, 1 -> left top, 2-> top right, 3->bottom left, 4->bottom right
	vec2d<float> GetPos(int flag = 0);

	//set pos in real world
	void SetPos(vec2d<float> x);

	//change pos by dx, dy in real world
	void DeltaPos(vec2d<float> delta);

	// Return bounding box w.r.t to camera
	std::unique_ptr<SDL_Rect> GetCameraRect(Camera& cam);

	// clear dest and add path
	void MoveTo(vec2d<float> v);

	// add path without clearing dest	
	void AddPath(vec2d<float> v);

	void GoTo(vec2d<float> v, TileManager& tm);

	// clear dest and add multiple path
	void SetPath(std::deque<vec2d<float>>&& path);

	bool drawDest = false;
	bool drawBox = false;

protected:
	vec2d<float> _pos;
	//destination
	std::deque<vec2d<float>> _dest;
	// vec2d<float> _speed;
	float _speed = 1;
	vec2d<float> _acceleration;
	float _width;
	float _height;

	//temp to delete
	vec2d<float> query = vec2d<float>(-1,-1);
	const TileManager* _tm;

	bool toupdate = 0;


	//rect for texture
	SDL_Rect* _textureRect;
	//rect in real world
	SDL_Rect* _rect;

	SDL_Texture* _texture;

};

class Unit : public Entity{
private:
	int teams;
	double deg;
};