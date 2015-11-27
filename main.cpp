#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include <stdio.h>
#include<iostream>
#include<sstream>
#include<list>
#include<string>
//Music provided by www.soundbible.com
#include "Jugador.h"
#include "Enemigo.h"
#include "Proyectil.h"
#include "Vida.h"

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background,*character;
SDL_Rect rect_background,rect_character;
int con;
int *contador;
TTF_Font *font;
SDL_Color textcolor;
SDL_Surface *surface_message;
SDL_Texture *message;
SDL_Rect message_rect;

using namespace std;

namespace temp
{
    template <typename T> std::string to_string(const T &n)
    {
        std::ostringstream stm;
        stm<< n;
        return stm.str();
    }
 }

int main( int argc, char* args[] )
{

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1020/*WIDTH*/, 380/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }



    TTF_Init();
    font = TTF_OpenFont("fast99.ttf", 40);
    textcolor = {255,255,255};
    surface_message = TTF_RenderText_Solid(font, temp::to_string(con).c_str(), textcolor);
    message = SDL_CreateTextureFromSurface(renderer, surface_message);
    message_rect.y =0;
    message_rect.x =600;
    message_rect.w =60;
    message_rect.h =60;

    //Init textures
    con = 0;
    contador = &con;
    int w=0,h=0;

    background = IMG_LoadTexture(renderer,"back.gif");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;

    list<Entidad*>entidades;
    entidades.push_back(new Jugador(&entidades,renderer, contador));
    entidades.push_back(new Vida(&entidades,renderer));

    double last_frame=0;
    int frame=0;


    //Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return 0;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_d)
                    rect_character.x++;
            }
        }

        int spawn_frequency=60-(frame/100);
        if(spawn_frequency<5)
            spawn_frequency=5;
        if(frame%spawn_frequency==0)
            entidades.push_back(new Enemigo(&entidades,renderer));

        //SDL_Delay(17-(SDL_GetTicks()-last_frame));
        double diferencia = SDL_GetTicks()-last_frame;
        double ajuste = 17 - diferencia;
        if(ajuste>0)
            SDL_Delay(ajuste);
        last_frame=SDL_GetTicks();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
            (*e)->logica();

        for(list<Entidad*>::iterator e = entidades.begin();
            e!=entidades.end();
            e++)
        {
            if((*e)->delete_flag)
            {
                entidades.remove(*e);
                break;
            }
        }
        surface_message = TTF_RenderText_Solid(font, temp::to_string(con).c_str(), textcolor);
        message = SDL_CreateTextureFromSurface(renderer, surface_message);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderCopy(renderer, message, NULL, &message_rect);


        for(int i=0;i<2;i++)
        {
            for(list<Entidad*>::iterator e = entidades.begin();
                e!=entidades.end();
                e++)
            {
                if((*e)->capa==i)
                    (*e)->dibujar();
            }
        }

        SDL_RenderPresent(renderer);
        frame++;
    }

	return 0;
}
