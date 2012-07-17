#ifndef H_INPUT
#define H_INPUT
#include "graphics.hpp"
//#include <string.h>

class Input
{
	public:
		Input();
		void UpdateEvents();
		//void print_key(string&, unsigned);

	private:
		int key[SDLK_LAST];
		int mousex,mousey;
		//int mousexrel,mouseyrel;
		int mousebuttons[8];
		char touche; //Stocke le nom de la touche appuyée
		int quit;
};

#endif
