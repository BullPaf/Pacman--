#ifndef H_MENUS
#define H_MENUS
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "input.hpp"
#include "constantes.hpp"
#include "levelmanager.hpp"
//#include "scoremanager.h"
//#include "config.h"

class Menu
{
	public:
		Menu(int);
		~Menu();
		int draw_menu(Graphics &g);
		void set_Title(std::string s) {title = s;}
		void set_Option(int n, std::string s) {options[n] = s;}
		void set_Available(int n, bool t) {available[n] = t;}
		void set_FontSize(int s) {font_size = s;}
		void set_Space(int s) {space = s;}
		int get_OptionsSize() {return options.size();}
	private:
		std::string title;
		std::vector<std::string> options;
		SDL_Surface *img;
		SDL_Rect pos, p1;
		int selection, nb_options, font_size, space;
		std::vector<int> couleur;
		std::vector<bool> available;
};

/*typedef struct
{
	char title[64];
	char **options;
	SDL_Surface *img;
	SDL_Rect pos;
	int selection, nb_options, *couleur, *available, font_size, space;
	POINT p1;
}Menu;


void init_menu(Menu*, int);
void delete_menu(Menu*);
void edit_campagne();
void draw_version();
int main_menu();
int save_menu(int);
void play_menu(int);
void new_file_menu(char*);
void options_menu(config*);
void win_menu();
void lost_menu();
int game_menu();
int edit_menu();
int select_file_menu();
int draw_menu(Menu);*/

#endif
