#ifndef H_INPUT
#define H_INPUT
#include "graphics.hpp"
//#include <string.h>

class Input
{
	public:
		Input();
		void UpdateEvents();
		int get_MouseX() {return mousex;}
		int get_MouseY() {return mousey;}
		int get_Quit() {return quit;}
		int get_MouseButton(int b) {return mousebuttons[b];}
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
