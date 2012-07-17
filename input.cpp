#include "input.hpp"

Input::Input()
{
	for(int i=0; i<SDLK_LAST; i++) key[i]=0;
	for(int i=0; i<8; i++) mousebuttons[i]=0;
	mousex=mousey=quit=touche=0;
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
						touche = event.key.keysym.unicode & 0x7F;
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
/*void Input::print_key(char* chaine, Input *in, unsigned max_size)
{
	int size = strlen(chaine);
	if(in->key[SDLK_BACKSPACE] && size > 0) //Pour effacer
	{
		in->key[SDLK_BACKSPACE]=0; //eviter la repetition
		chaine[size-1]='\0'; //efface
	}
	else if(in->key[SDLK_SPACE]) in->key[SDLK_SPACE]=0; //Un espace
	//Si la touche ne fait pas dépasser la taille max de la chaine
	//on concatene
	else if( strlen(in->touche) && (size+strlen(in->touche) < max_size) ) strcat(chaine, in->touche);
	in->touche[0]='\0'; //on vide la touche qui a été appuyée
}*/

