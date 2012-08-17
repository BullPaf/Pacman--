#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.hpp"
#include "constantes.hpp"
#include <vector>
#include <dirent.h>
#ifndef WIN32
	#include <sys/types.h>
#endif
//#include <string>

//#define LINE_SIZE 2048

typedef enum Block_type {RIEN, MUR, BONUS, PACMAN, GHOST}Block_type;
typedef enum Couleur {ROUGE, VIOLET, BLEU, JAUNE}Couleur;

typedef struct Case
{
	Block_type type; //Si c'est un MUR ou un BONUS
	int elt_type;
}Case;

class Level
{
	public:
		Level();
		~Level();

		//Setters
		void set_NbGhosts(int nb) {nb_ghosts = nb;}
		void set_BlockType(int i, int j, Block_type type) {map[i][j].type = type;}
		void set_EltType(int i, int j, int elt_type) {map[i][j].elt_type = elt_type;}
		void set_Dots(int d) {dotsRemaining = d;}
		void set_PacStartX(int x) {pac_start_x = x;}
		void set_PacStartY(int y) {pac_start_y = y;}
		void set_PacStartDir(DIRECTION dir) {pac_start_direction = dir;}
		void set_GhostStartX(int n, int x) {ghost_start_x[n] = x;}
		void set_GhostStartY(int n, int y) {ghost_start_y[n] = y;}
		void set_GhostColor(int n, Couleur col) {ghosts_color[n] = col;}
		void set_Case(int i, int j, Case c) {map[i][j]=c;}

		//Getters
		int get_NbGhosts() {return nb_ghosts;}
		Block_type get_BlockType(int i, int j) {return map[i][j].type;}
		int get_EltType(int i, int j) {return map[i][j].elt_type;}
		int get_Dots() {return dotsRemaining;}
		int get_PacStartX() {return pac_start_x;}
		int get_PacStartY() {return pac_start_y;}
		DIRECTION get_PacStartDir() {return pac_start_direction;}
		int get_GhostStartX(int n) {return ghost_start_x[n];}
		int get_GhostStartY(int n) {return ghost_start_y[n];}
		Couleur get_GhostColor(int n) {return ghosts_color[n];}
		Case get_Case(int i, int j) {return map[i][j];}

	private:
		Case map[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR]; //Un niveau est un ensemble de Case
		int dotsRemaining; //Le nombre de petit point à manger dans le niveau courant
		int pac_start_x;
		int pac_start_y;
		DIRECTION pac_start_direction;
		int nb_ghosts;
		int ghost_start_x[NB_MAX_GHOSTS]; //Vector?
		int ghost_start_y[NB_MAX_GHOSTS]; //Vector?
		Couleur ghosts_color[NB_MAX_GHOSTS]; //Vector??
};

class LevelManager
{
	public:
		LevelManager();
		~LevelManager();
		void load_level(int, Level&);
		void save_level(int, Level&);
		void save_game(int);
		void add_level(std::string);
		int load_game();
		bool has_saved_game();
		void extract_val(const char*, int, Level&);
		void draw_level(Level&, Graphics&);
		void affiche_une_case(Case, SDL_Rect*, SDL_Surface*);
		void remove_bloc(int, int, Level&);
		void erase_level(Level&);
		int remove_right_bloc(int);
		int remove_left_bloc(int);
		int remove_down_bloc(int);
		int remove_up_bloc(int);
		
		//getters
		SDL_Surface* get_Wall_Texture(int n) {return wall_texture[n];}
		SDL_Surface* get_Bonus_Texture(int n) {return bonus_texture[n];}
		SDL_Surface* get_Pacman_Texture(int n) {return pacman_texture[n];}
		SDL_Surface* get_Ghosts_Texture(int n) {return ghosts_texture[n];}
		std::string get_LevelName(int i) {return level_filename[i];}
		int get_NbLevel() {return nb_level;}

		//setters
		void set_levelName(int n, std::string s) {level_filename[n] = s;}

	private:
		int nb_level; //Nombre de niveaux total
		SDL_Surface *wall_texture[NB_WALL_BLOCKS];
		SDL_Surface *bonus_texture[NB_BONUS_BLOCKS];
		SDL_Surface *pacman_texture[NB_PACMAN_BLOCK];
		SDL_Surface *ghosts_texture[NB_GHOST_BLOCKS];
		std::vector<std::string> level_filename;
};

/*
int DELAY; //Temps de rafraichissement
int CAMPAGNE_LEVEL; //Le nombre de niveaux de la campagne
int SAVE_ENABLE; //A placer ailleurs

char LEVEL_FILE[256][128]; //Stocke tout les niveaux AILLEURS
char CAMPAGNE[256][128]; //Stocke uniquement les niveaux pour la campagne //AILLEURS

int dans_case(SDL_Rect); //Plutot pour les bougeables?
SDL_Rect get_case(SDL_Rect, int); //Plutot pour les bougeables?
*/
#endif
