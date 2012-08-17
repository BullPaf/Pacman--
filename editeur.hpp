#ifndef H_EDITEUR
#define H_EDITEUR
#include "levelmanager.hpp"
#include "menus.hpp"

enum {AUCUN, SAVE, LOAD, DELETE}; //Pour les messages de l'editeur

//CASE editor[NB_ALL_BLOCKS]; //Contient tout les éléments du jeu!

class Editor
{
	public:
		Editor();
		~Editor();
		void edit_menu(Menu&);
		void file_menu(Menu&, LevelManager&);
		void save_menu(Menu&, LevelManager&, int);
		void new_file_menu(Graphics&, std::string&);
		void editer(Graphics&);
		void load_gui(Graphics&, LevelManager&);
		void init_editor();
		void highlight_block(int, Graphics&);
		int get_block_type(int, int, int);
		void print_info(int*, int, SDL_Rect, int);
		void plot_object(int, int, int, Level&);
	private:
		Case editor[NB_ALL_BLOCKS];
};

/*int editer();
void load_gui();
void init_editor();
void highlight_block(int);
int get_block_type(int, int, int);
void print_info(int*, int, POINT, int);
void plot_object(int, int, int);*/

#endif
