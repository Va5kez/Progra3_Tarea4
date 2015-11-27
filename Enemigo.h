#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "Personaje.h"
#include "Jugador.h"
#include <stdlib.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include <stdio.h>
#include <list>

class Jugador;

class Enemigo : public Personaje
{
    public:
        Jugador* jugador;
        Enemigo(list<Entidad*>* entidades,SDL_Renderer* renderer);
        virtual ~Enemigo();
        void logica();
    protected:
    private:
};

#endif // ENEMIGO_H
