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
		int get_Key(int k) {return key[k];}
		void set_Key(int k, int val) {key[k] = val;}
		void print_key(std::string&, unsigned);

	private:
		int key[SDLK_LAST];
		int mousex,mousey;
		//int mousexrel,mouseyrel;
		int mousebuttons[8];
		char touche[2]; //Stocke le nom de la touche appuyée
		int quit;
};

#endif
