#ifndef H_LEVEL
#define H_LEVEL
#include "graphics.hpp"
#include "constantes.hpp"
//#include <string.h>

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
		void set_NbGhosts(int nb) {nb_ghosts = nb;}

	private:
		Case map[NB_BLOCKS_HAUTEUR][NB_BLOCKS_LARGEUR]; //Un niveau est un ensemble de Case
		int dotsRemaining; //Le nombre de petit point à manger dans le niveau courant
		int pac_start_x;
		int pac_start_y;
		int pac_start_direction;
		int nb_ghosts;
		int ghost_start_x[NB_MAX_GHOSTS];
		int ghost_start_y[NB_MAX_GHOSTS];
		Couleur ghosts_color[NB_MAX_GHOSTS];
};

class LevelManager
{
	public:
		LevelManager();
		~LevelManager();
		void load_level(int);
		void save_level(int);
		void save_game(int);
		void load_game();
		void has_saved_game();
		void extract_val(string, int);
		void draw_level();
		void affiche_une_case(Case, SDL_Rect*, SDL_Surface*);
		void delete_blocks();
		void remove_bloc(int, int);
		int remove_right_bloc(int);
		int remove_left_bloc(int);
		int remove_down_bloc(int);
		int remove_up_bloc(int);

	private:
		int nb_level;
		SDL_Surface *wall_texture[NB_WALL_BLOCKS];
		SDL_Surface *bonus_texture[NB_BONUS_BLOCKS];
		SDL_Surface *pacman_texture[NB_PACMAN_BLOCK];
		SDL_Surface *ghosts_texture[NB_GHOST_BLOCKS];
		std::string level_filename[256];
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
