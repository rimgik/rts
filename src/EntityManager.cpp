#include "EntityManager.h"

EntityManager::EntityManager(SDL_Renderer* r){

    SDL_Texture *player_texture = ResourceManager::GetInstance().LoadTextureFromFile(r, "./data/player.png");

    for(int i = 0; i < 1; ++i){
		entityVec.emplace_back(std::shared_ptr<Entity> (new Entity(50,5*i + 50,100,100)));

		entityVec[i]->SetTexture(player_texture);
    }
	// entityVec.push_back(std::shared_ptr<Entity> (new Entity(300,300,100,100)));
	// entityVec[1]->SetTexture(r, player_texture);

}

EntityManager::~EntityManager(){
	ClearAllEntity();										
	std::cout << "All Entity has been freed" << std::endl;
}

void EntityManager::ClearAllEntity(){
	selectedEntity.clear();
	entityVec.clear();
}

void EntityManager::ClearSelectedEntity(){
	for(auto &i : selectedEntity){
		i->drawBox = false;
	}
	selectedEntity.clear();
}

void EntityManager::UpdateEntity(const Uint64& timeElapsed){
	// std::cout << "Updating: " << timeElapsed << std::endl;
	for(auto &i : entityVec){
		i->Update(timeElapsed);
	}
}

void EntityManager::RenderEntity(SDL_Renderer* renderer, Camera& cam){
	for(auto &i : entityVec){
		i->Render(renderer, cam);
	}
}

void EntityManager::MoveSelectedEntity(const vec2d<float>& x){
	for(auto &i : selectedEntity){
		i->MoveTo(x);
	}	
}

void EntityManager::SelectedEntityGoTo(const vec2d<float>& v, TileManager& tm){
	auto temp = tm.WorldToTile(v);
	for(auto &i : selectedEntity){
		i->GoTo(v, tm);
		// i->SetPath(Pathfinding::GetInstance().CalculatePath(tm.WorldToTile(i->GetPos()), temp));
	}	
}

void EntityManager::SelectedEntityAddPath(const vec2d<float>& x){
	for(auto &i : selectedEntity){
		i->AddPath(x);
	}
}

void EntityManager::SetShowDestination(const bool& x){
	for(auto &i : entityVec){
		i->drawDest = x;
	}
}

void EntityManager::SelectEntityByRect(SDL_Rect* rect, Camera& cam){
	for(auto &i : entityVec){
		if(SDL_TRUE == SDL_HasIntersection(rect, i->GetCameraRect(cam).get())){
			selectedEntity.push_back(i);
			i->drawBox = true;
		}
	}
}

void EntityManager::SelectEntityByPoint(SDL_Point* point, Camera& cam){
	std::shared_ptr<Entity> ptr;
	float distance = std::numeric_limits<float>::infinity();
	for(auto &i : entityVec){
		std::unique_ptr<SDL_Rect> temp = std::move(i->GetCameraRect(cam));
		if(SDL_TRUE == SDL_PointInRect(point, temp.get())){
			float x1 = temp->x + temp->w / (float)2;
			float y1 = temp->y + temp->h / (float)2;
			float currDist = sqrt((x1 - point->x)*(x1 - point->x) + (y1-point->y)*(y1-point->y));
			if(currDist < distance){
				distance = currDist;
				ptr = i;
			}
		}
	}
	if(ptr != nullptr){
		ClearSelectedEntity();
		ptr->drawBox = true;
		selectedEntity.emplace_back(ptr);
	}
}

bool EntityManager::HasSelectedEntity(){
	return !selectedEntity.empty();
}