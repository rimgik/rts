//C++ standard library
#include <iostream>
#include <memory>
#include <vector>

//SDL library
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_thread.h"

//include
#include "ResourceManager.h"
#include "Camera.h"
#include "Entity.h"
#include "vec2d.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "TileManager.h"

// Uint32 currTick;
// Uint32 secondTick = 0;
Uint32 deltaTick = SDL_GetTicks64();
Uint32 prevTick = SDL_GetTicks64();
int frameCount = 0;
Uint32 secondTick = 0;
Uint32 currTick = SDL_GetTicks64();
SDL_Renderer* renderer = nullptr;
TileManager* tm = nullptr;
Camera* cam = nullptr;
EntityManager* em = nullptr;
EventManager* event = nullptr;

bool gameIsRunning = true;

int r(void* data){
    Uint64 t = SDL_GetTicks64();
    while(gameIsRunning){
        em->UpdateEntity(1);
        SDL_Delay(5);
        Uint64 ans = SDL_GetTicks64() - t;
        t = SDL_GetTicks64();
    }
    return 1;
}

int main(int argc, char* argv[]){

    const int windowwidth = 1280;
    const int windowheight = 720;
    const int fps = 100;

    //create window using opengl
    SDL_Window *window = nullptr;
    window = SDL_CreateWindow("c++ SDL2 window", 50, 50, windowwidth, windowheight, SDL_WINDOW_SHOWN );

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cout << "SDL could not be initialized" << SDL_GetError();
    }else{
        std::cout << "SDL video ready to go" << std::endl;
    }

    //init SDL_image
    int flags = IMG_INIT_PNG;
    int inited = IMG_Init(flags); 
    if((inited & flags) != flags){
        std::cout << SDL_GetError() << std::endl;
    }
    
    SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, "opengl", SDL_HINT_OVERRIDE);

    //create renderer
    // SDL_renderer *renderer = nullptr;
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );

    //init for game loop
    // bool gameIsRunning = true;
    // int frameCount = 0;

    //game tick
    const int gameTick = 60;

    // Uint32 currTick;
    // Uint32 secondTick = 0;
    Uint32 deltaTick = SDL_GetTicks64();
    Uint32 prevTick = SDL_GetTicks64();

    // custom settings
    SDL_Texture *player_texture = ResourceManager::GetInstance().LoadTextureFromFile(renderer, "./data/player.png");
    SDL_Texture *map_t = ResourceManager::GetInstance().LoadTextureFromFile(renderer, "./data/map.png");
    SDL_Texture *black = ResourceManager::GetInstance().LoadTextureFromFile(renderer, "./data/black.png");
    // Camera cam(500, 500, windowwidth, windowheight, 0, 0, windowwidth, windowheight);
    cam = new Camera(500, 500, windowwidth, windowheight, 0, 0, windowwidth, windowheight);
    // EntityManager em(renderer);
    em = new EntityManager(renderer);
    // TileManager tm;
    tm = new TileManager();
    em->SetShowDestination(true); 
    // EventManager event;
    event = new EventManager();


    SDL_Thread* test = SDL_CreateThread(r, "r", (void*)(10));
    // SDL_Thread* test2 = SDL_CreateThread(r2, "r2", (void*)(10));

    Uint64 lastUpdate = SDL_GetTicks64();



    while(gameIsRunning){

        // currTick = SDL_GetTicks64();

        event->GetInput(*cam, gameIsRunning, *em, *tm);
        // lastUpdate = SDL_GetTicks64();
        // prevTick = SDL_GetTicks64();

        currTick = SDL_GetTicks64();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        //Draw cam rect
        // SDL_RenderDrawRect(renderer, cam.GetRect().get());
        // SDL_RenderDrawRect(renderer, cam2.GetRect().get());

        tm->Render(renderer, *cam);
        // Draw the dragging box
        if(event->GetRect().get() != nullptr){
            SDL_RenderDrawRect(renderer, event->GetRect().get());
        }

        em->RenderEntity(renderer, *cam);

        SDL_RenderPresent(renderer);
        // SDL_Delay(0);
        frameCount++;
        secondTick += SDL_GetTicks64() - currTick;
        if(secondTick > 1000){
            // std::cout << frameCount << std::endl;
            std::cout << "FPS: " << (float)frameCount * 1000 / secondTick << '\n';
            secondTick = 0;
            frameCount = 0;
        }
        SDL_Delay(13);

        //original
        // if(frameCount < (secondTick*FPS + 1000 - FPS)/1000){
        //     //Give us a clear "canvas"
        //     SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        //     SDL_RenderClear(renderer);
        //     SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        //     //Draw cam rect
        //     // SDL_RenderDrawRect(renderer, cam.GetRect().get());
        //     // SDL_RenderDrawRect(renderer, cam2.GetRect().get());

        //     tm.Render(renderer, cam);
        //     // Draw the dragging box
        //     if(event.GetRect().get() != nullptr){
        //         SDL_RenderDrawRect(renderer, event.GetRect().get());
        //     }

        //     em.RenderEntity(cam);

        //     SDL_RenderPresent(renderer);
        //     // SDL_Delay(0);
        //     frameCount++;
        // }

        // secondTick += SDL_GetTicks64() - currTick;
        // if(secondTick > 1000){
        //     // std::cout << secondTick << std::endl;
        //     std::cout << "FPS: " << (float)frameCount * 1000 / secondTick << std::endl;
        //     secondTick = 0;
        //     frameCount = 0;
        // }
    }
    // SDL_FreeSurface(image);
    // SDL_DestroyTexture(texture);
    SDL_DetachThread(test);
    // SDL_DetachThread(test2);
    SDL_DestroyWindow(window);
    ResourceManager::GetInstance().FreeTexture();
    IMG_Quit();
    SDL_Quit();
}