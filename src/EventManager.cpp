#include "EventManager.h"

EventManager::EventManager(){
	inSelector = false;
	isDragging = false;	
	mouseSelect = vec2d<int>(-1, -1);

    keys = SDL_GetKeyboardState(NULL);
}

void EventManager::GetInput(Camera& cam, bool& gameIsRunning, EntityManager& entityManager, TileManager& map){
	// Get mouse state

    // SDL event
    SDL_Event event;


    //update for keyboard
    // if(keys[SDL_SCANCODE_A]){
    // }
    // if(keys[SDL_SCANCODE_W]){
    // }
    // if(keys[SDL_SCANCODE_S]){
    // }
    // if(keys[SDL_SCANCODE_D]){
    // }
    // if(keys[SDL_SCANCODE_DOWN]){
    // }
    // if(keys[SDL_SCANCODE_UP]){
    // }
    while(SDL_PollEvent(&event)){
        // Update Keyboard State
        SDL_PumpEvents();




        // Get Mouse State
	    Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y); 
        vec2d<int> mouse = vec2d<int>(mouse_x, mouse_y);
        vec2d<float> toWorld = cam.ScreenToWorld(mouse);




        if(event.type == SDL_QUIT){
            gameIsRunning = false;

        }else if(event.type == SDL_MOUSEWHEEL){
            if(event.wheel.y < 0){
                //Zoom out
                cam.ZoomOut(mouse, -event.wheel.y);
            }else if(event.wheel.y > 0){
                //Zoom in
                cam.ZoomIn(mouse, event.wheel.y);
            }
        }else if(event.type == SDL_KEYDOWN){
            if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                map.SetWallFromWorld(toWorld);
            }
        }
        if(event.type == SDL_MOUSEMOTION && event.button.clicks < 1){
            if(mouseState & SDL_BUTTON(3)){
                //right mouse drag
                cam.Move(vec2d<int>(event.motion.xrel, event.motion.yrel));
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_LEFT){
            	if(!isDragging){
            		isDragging = true;
                    mouseSelect = mouse;
            	}	
            }
        }
        if(event.type == SDL_MOUSEBUTTONUP){
        	//left release
        	if(event.button.button == SDL_BUTTON_LEFT && isDragging){
        		entityManager.ClearSelectedEntity();
        		entityManager.SelectEntityByRect(GetRect().get(), cam);
        		if(entityManager.HasSelectedEntity()){
        			inSelector = true;
        		}else inSelector = false;
	       		isDragging = false;
        	}
            // right clicks
            // if(event.button.button == SDL_BUTTON_RIGHT && event.button.clicks == 1){
            // 	entityManager.ClearSelectedEntity();
            // }
            //right clicks
            if(event.button.button == SDL_BUTTON_RIGHT && event.button.clicks == 1 && inSelector){
                if(keys[SDL_SCANCODE_LSHIFT]){
                    entityManager.SelectedEntityAddPath(toWorld);
                }else{
                    entityManager.SelectedEntityGoTo(toWorld, map);
                }
            }
            if(event.button.button == SDL_BUTTON_LEFT && event.button.clicks >= 1  ){
            	SDL_Point ptr = {mouse_x, mouse_y};
            	entityManager.SelectEntityByPoint(&ptr, cam);
            	if(entityManager.HasSelectedEntity()){
            		inSelector = true;
            	}else inSelector = false;
            }
        }
    }
}

std::unique_ptr<SDL_Rect> EventManager::GetRect(){
	if(!isDragging){
		// return nullptr
		return std::unique_ptr<SDL_Rect>();
	}
	std::unique_ptr<SDL_Rect> rect = std::make_unique<SDL_Rect> ();
	*rect = {mouseSelect.x, mouseSelect.y, (mouse_x - mouseSelect.x), (mouse_y - mouseSelect.y)};
	return rect;
}

EventManager& EventManager::GetInstances(){
	static EventManager* ptr = new EventManager;
	return *ptr;
}
