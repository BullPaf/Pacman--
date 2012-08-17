#include "input.hpp"

Input::Input()
{
	for(int i=0; i<SDLK_LAST; i++) key[i]=0;
	for(int i=0; i<8; i++) mousebuttons[i]=0;
	mousex=mousey=quit=touche[0]=0;
	touche[1]='\0';
}

void Input::UpdateEvents()
{
	SDL_Event event;
	mousebuttons[SDL_BUTTON_WHEELUP] = 0;
	mousebuttons[SDL_BUTTON_WHEELDOWN] = 0;
	while(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
				key[event.key.keysym.sym]=1;
				if( (event.key.keysym.sym>=32 && event.key.keysym.sym<=126) || \
					(event.key.keysym.sym>=SDLK_KP0 && event.key.keysym.sym<=SDLK_KP9) )
						touche[0] = event.key.keysym.unicode & 0x7F;
				break;
			case SDL_KEYUP:
				key[event.key.keysym.sym]=0;
				break;
			case SDL_MOUSEMOTION:
				mousex=event.motion.x;
				mousey=event.motion.y;
				//mousexrel=event.motion.xrel;
				//mouseyrel=event.motion.yrel;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mousebuttons[event.button.button]=1;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button!=SDL_BUTTON_WHEELUP && event.button.button!=SDL_BUTTON_WHEELDOWN)
				mousebuttons[event.button.button]=0;
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			default:
				break;
		}
	}
}

/*Met à jour chaine en fonction de ce qui est tapé
 * Manque plein de chose pour eviter des bugs*/
void Input::print_key(std::string &chaine, unsigned max_size)
{
	int size = chaine.size();
	if(key[SDLK_BACKSPACE] && size > 0) //Pour effacer
	{
		key[SDLK_BACKSPACE]=0; //eviter la repetition
		chaine.erase(size-1); //On enleve le dernier caractere
	}
	else if(key[SDLK_SPACE]) key[SDLK_SPACE]=0; //Un espace
	//Si la touche ne fait pas dépasser la taille max de la chaine
	//on concatene
	else if( strlen(touche) && (size+strlen(touche)) < max_size) chaine+=touche;
	touche[0]='\0'; //on vide la touche qui a été appuyée
}

