#include "Entity.h"

Entity::Entity(float x, float y, float w, float h){
	_rect = nullptr;
	_textureRect = nullptr;

	_pos = vec2d<float>(x - w/2, y - h/2);
	_width = w;
	_height = h;

}

Entity::~Entity(){
	delete _textureRect;
}

//memory leak!!!!
void Entity::SetTexture(SDL_Texture *texture, int x, int y, int w, int h){
	delete _textureRect;
	_texture = texture;
	_textureRect = new SDL_Rect;
	_textureRect->x = x;
	_textureRect->y = y;
	_textureRect->w = w;
	_textureRect->h = h;
}

void Entity::SetTexture(SDL_Texture *texture){
	delete _textureRect;
	_texture = texture;
	_textureRect = new SDL_Rect;
	_textureRect->x = 0;
	_textureRect->y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &(_textureRect->w), &(_textureRect->h));
}

void Entity::Render(SDL_Renderer* _renderer, Camera &cam){
	if(_dest.size()){
		// SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
		// vec2d<int> world = cam.WorldToScreen(GetPos());
		// vec2d<int> t2 = cam.WorldToScreen(vec2d<float>(_dest.rbegin()->x, _dest.rbegin()->y));
		// SDL_RenderDrawLine(_renderer, world.x, world.y, t2.x, t2.y);
	}
	if(drawDest){
		SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
		auto destSize = _dest.size();
		if(destSize > 0){
			vec2d<int> world = cam.WorldToScreen(GetPos());
			vec2d<int> t2 = cam.WorldToScreen(_dest[0]);
			SDL_RenderDrawLine(_renderer, world.x, world.y, t2.x, t2.y);

		}
		if(destSize > 1){
			for(int i = 1; i < destSize;++i){	
				auto world = cam.WorldToScreen(_dest[i-1]);
				auto t2 = cam.WorldToScreen(_dest[i]);
				SDL_RenderDrawLine(_renderer, world.x, world.y, t2.x, t2.y);
			}
			auto tmp = cam.WorldToScreen(_dest[1]);
			auto world = cam.WorldToScreen(GetPos());
			SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(_renderer, world.x, world.y, tmp.x, tmp.y);
		}	
	}
	if(_textureRect != nullptr){
		//only render if it has intersection with camera rect
		std::unique_ptr<SDL_Rect> ptr = GetCameraRect(cam);
		if(SDL_HasIntersection(ptr.get(), cam.GetRect().get()) == SDL_TRUE){
			SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
			if(drawBox)SDL_RenderDrawRect(_renderer, ptr.get());
			SDL_RenderCopy(_renderer, _texture, _textureRect, ptr.get());
		}
	}
}

void Entity::MoveTo(vec2d<float> v){
	_dest.clear();
	_dest.push_back(v);
}

void Entity::AddPath(vec2d<float> v){
	_dest.push_back(v);
}

void Entity::SetPath(std::deque<vec2d<float>>&& path){
	toupdate = 1;
	_dest = path;
}

void Entity::GoTo(vec2d<float> v,TileManager& tm){
	_tm = &tm;
	query = v;
	// SetPath(_tm->CalculatePath(tm.WorldToTile(GetPos()), tm.WorldToTile(v)));
}

vec2d<float> Entity::GetPos(int flag){
	if(flag == 1){
		return vec2d<float>(_pos.x , _pos.y );
	}else if(flag == 2){
		return vec2d<float>(_pos.x + _width, _pos.y );
	}else if(flag == 3){
		return vec2d<float>(_pos.x, _pos.y + _height);
	}else if(flag == 4){
		return vec2d<float>(_pos.x + _width, _pos.y + _height);
	}else{
		return vec2d<float>(_pos.x + _width / 2, _pos.y + _height / 2);
	}
}

void Entity::SetPos(vec2d<float> x){
	toupdate = 1;
	_pos = x - vec2d<float>(_width/2, _height/2);
}

void Entity::DeltaPos(vec2d<float> delta){
	_pos += delta;
}

std::deque<vec2d<float>>& Entity::GetDest(){
	return _dest;
}

std::unique_ptr<SDL_Rect> Entity::GetCameraRect(Camera& cam){
	//get rect w.r.t to screen coor
	auto temp = cam.WorldToScreen(GetPos(1));
	auto temp2 = cam.WorldToScreen(GetPos(4));
	std::unique_ptr<SDL_Rect> ptr = std::make_unique<SDL_Rect>();
	*ptr = {temp.x, temp.y, temp2.x - temp.x, temp2.y - temp.y};

	return ptr;
}

void Entity::Update(Uint64 timeElapsed){

	if(query != vec2d<float> (-1,-1)){
		toupdate = 1;
		SetPath(_tm->CalculatePath(GetPos(), query));
		query = vec2d<float> (-1,-1);
	}
	toupdate = 1;
	while(
			toupdate &&
			_dest.size() > 1 && 
			_tm->Walkable(GetPos(), _dest[1]) &&
			_tm->Walkable(GetPos(1), _dest[1] - vec2d<float>(_width/2, _height/2)) &&
			_tm->Walkable(GetPos(2), _dest[1] + vec2d<float>(_width/2, -_height/2)) &&
			_tm->Walkable(GetPos(3), _dest[1] + vec2d<float>(-_width/2, _height/2)) &&
			_tm->Walkable(GetPos(4), _dest[1] + vec2d<float>(_width/2, _height/2)) 
		)
	{
		_dest.pop_front();
	}

	if(toupdate)toupdate = 0;

	if(_dest.size() > 0){
		vec2d<float> curr = GetPos();
		float dx = _dest[0].x - curr.x;
		float dy = _dest[0].y - curr.y;
		float dist = sqrt(dx*dx + dy*dy);
		float cos = dx / dist;
		float sin = dy / dist;
		if(dist < 5){
			SetPos(vec2d<float>(_dest[0].x, _dest[0].y));
			toupdate = 1;
			_dest.pop_front();
		}else{
			// std::cout << _speed * sin * timeElapsed << std::endl;
			// std::cout << sin << std::endl;
			DeltaPos(vec2d<float> (_speed * cos * timeElapsed, _speed * sin * timeElapsed));
		}
	}
}
