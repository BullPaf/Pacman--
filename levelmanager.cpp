#include "levelmanager.hpp"
#include <sstream>
#include <fstream>

/*Initialize le tableau du niveau
 * et les différentes variables*/
Level::Level()
{
	for(int i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(int j=0; j<NB_BLOCKS_LARGEUR; j++)
			map[i][j].type = RIEN;
	}
	nb_ghosts=0;
}

Level::~Level() {}

LevelManager::LevelManager()
{
	std::string img;
	std::ostringstream num;
	for (int i=0; i<NB_WALL_BLOCKS; i++)
	{
		num<<i;
		img = "image/level/" + num.str() + ".png";
		if( (wall_texture[i] = IMG_Load(img.c_str())) == NULL)
		{
			std::cerr <<"Fatal Error..." << std::endl;
			exit(EXIT_FAILURE);
		}
		num.str("");
	}
	for (int i=0; i<NB_BONUS_BLOCKS; i++)
	{
		num<<i;
		img = "image/bonus/" + num.str() + ".png";
		if( (bonus_texture[i] = IMG_Load(img.c_str())) == NULL)
		{
			std::cerr <<"Fatal Error..." << std::endl;
			exit(EXIT_FAILURE);
		}
		num.str("");
	}
	for (int i=0; i<NB_PACMAN_BLOCK; i++)
	{
		num<<i*2;
		img = "image/pacman/" + num.str() + ".png";
		if( (pacman_texture[i] = IMG_Load(img.c_str())) == NULL)
		{
			std::cerr <<"Fatal Error..." << std::endl;
			exit(EXIT_FAILURE);
		}
		num.str("");
	}
	for (int i=0; i<NB_GHOST_BLOCKS; i++)
	{
		num<<i*8;
		img = "image/ghosts/" + num.str() + ".png";
		if( (ghosts_texture[i] = IMG_Load(img.c_str())) == NULL)
		{
			std::cerr <<"Fatal Error..." << std::endl;
			exit(EXIT_FAILURE);
		}
		num.str("");
	}

	DIR *dp = opendir(LEVEL_PATH);
	if (dp) {
		struct dirent *entry;
		while ( (entry = readdir(dp)) ) {
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
					continue;
			else
			{
				level_filename.push_back(entry->d_name);
				nb_level++;
			}
		}
		closedir(dp);
	}
	/*FILE *cmp=fopen("data/campagne.txt", "r");
	if(cmp != NULL)
		while (fscanf(cmp, "%s", CAMPAGNE[CAMPAGNE_LEVEL]) != EOF) CAMPAGNE_LEVEL++;
	else
		fprintf(stderr, "Cant read campagne file, campagne won't be available\n");
	fprintf(stderr, "%d Level in campagne\n", CAMPAGNE_LEVEL);*/
}

LevelManager::~LevelManager()
{
	for (int i=0; i<NB_WALL_BLOCKS; i++)
		free(wall_texture[i]);
	for (int i=0; i<NB_BONUS_BLOCKS; i++)
		free(bonus_texture[i]);
	for (int i=0; i<NB_PACMAN_BLOCK; i++)
		free(pacman_texture[i]);
	for (int i=0; i<NB_GHOST_BLOCKS; i++)
		free(ghosts_texture[i]);
}

/*
 * Lit un fichier et extrait les valeurs
 * pour affecter le tableau LEVEL
*/
void LevelManager::load_level(int level, Level &mylevel)
{
	mylevel.set_NbGhosts(0);
	int line=0;
	std::string chaine;
	std::string myfile = LEVEL_PATH + level_filename[level];
	std::ifstream level_file(myfile.c_str());

	if (level_file)
	{
		//Lit le fichier ligne par ligne et s'assure qu'on ne lit pas trop de ligne
		while (getline(level_file, chaine) && line < NB_BLOCKS_HAUTEUR)
		{
			extract_val(chaine.c_str(), line, mylevel); //Recupere les valeurs dans la ligne
			line++;
		}
		level_file.close();
	}
	else
	{
		std::cerr << "Error while opening Level file... Bye!" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*
 * Lit le tableau LEVEL est sauvegarde
 * le niveau dans un fichier
*/
void LevelManager::save_level(int level, Level &mylevel)
{
	std::string myfile = LEVEL_PATH+level_filename[level];
	std::ofstream level_file(myfile.c_str());
	for(int i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(int j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			if(mylevel.get_BlockType(i,j)==MUR) level_file << "1:" << mylevel.get_EltType(i,j) << " ";
			else if(mylevel.get_BlockType(i,j)==PACMAN) level_file << "3:" << mylevel.get_EltType(i,j) << " ";
			else if(mylevel.get_BlockType(i,j)==GHOST) level_file << "4:" << mylevel.get_EltType(i,j) << " ";
			else if(mylevel.get_BlockType(i,j)==BONUS) level_file << "2:" << mylevel.get_EltType(i,j) << " ";
			else level_file << "0 ";
		}
		level_file << std::endl;
	}
	level_file.close();
}

/*Sauve juste le numéro du niveau actuel pour le moment*/
void LevelManager::save_game(int cur_level)
{
	std::ofstream save_file("save/save.txt");
	if (save_file)
	{
		save_file << cur_level;
		save_file.close();
	}
	else std::cerr << "Error while saving game..." << std::endl;
}

/*Charge le dernier niveau sauvegardé*/
int LevelManager::load_game()
{
	int level;
	std::ifstream save_file("save/save.txt");
	if (save_file)
	{
		save_file >> level;
		save_file.close();
		return level;
	}
	else std::cerr << "No game saved..." << std::endl;
	return -1;
}

/*Si une sauvegarde existe*/
bool LevelManager::has_saved_game()
{
	std::ofstream save_file("save/save.txt");
	if (save_file)
	{
		save_file.close();
		return true;
	}
	else return false;
}

/* Cette méthode récupere les valeurs
 * séparées par des espaces dans le string s
 * et affecte cette valeur à la case du niveau
 * correspondante
 * Tres moche mais ca marche alors pas touche!
 * Tres sensible au format du fichier!
*/
void LevelManager::extract_val(const char *s, int line, Level &mylevel)
{
	char nb[3], type;
	nb[0]='0'; nb[1]='0'; nb[2]='\0';
	int nb_val=0, i=0, j=0, nghosts=0;
	Couleur ghost=ROUGE;
	while(nb_val < NB_BLOCKS_LARGEUR) //Tant qu'on a pas lu autant de valeur qu'il n'y a de case pour le niveau
	{
		if(s[i] != ' ') //Si ce n'est pas espace on conserve le caractere lu
		{
			type = s[i];
			if(type == '0') //CASE VIDE
			{
				mylevel.set_BlockType(line, nb_val, RIEN);
				i++;
			}
			else if(type=='1') //MUR
			{
				mylevel.set_BlockType(line, nb_val, MUR);
				i+=2; //On saute les ':'
				while(s[i] != ' ') //tant qu'on a pas d'espace on lit le type de bloc
				{
					nb[j]=s[i];
					j++; i++;
				}
				if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
				{
					nb[1]=nb[0];
					nb[0]='0';
				}
				mylevel.set_EltType(line, nb_val, atoi(nb));
				i++; j=0;
			}
			else if(type=='2') //BONUS
			{
				mylevel.set_BlockType(line, nb_val, BONUS);
				i+=2; //On saute les ':'
				while(s[i] != ' ') //Délimiteur type de bonus
				{
					nb[j]=s[i];
					j++; i++;
				}
				if(j==1) //Si la valeur ne fait qu'un caractere==>decalage
				{
					nb[1]=nb[0];
					nb[0]='0';
				}
				mylevel.set_EltType(line, nb_val, atoi(nb));
				if(atoi(nb) == 9)
				{
					int dots = mylevel.get_Dots();
					mylevel.set_Dots(++dots); //Si c'est un point on incremente le nombre de POINTS
				}
				i++; j=0;
			}
			else if(type=='3') //PACMAN
			{
				i+=2;
				mylevel.set_BlockType(line, nb_val, PACMAN);
				mylevel.set_PacStartX(nb_val);
				mylevel.set_PacStartY(line);
				if(s[i]=='0') mylevel.set_PacStartDir(HAUT);
				else if(s[i]=='1') mylevel.set_PacStartDir(DROITE);
				else if(s[i]=='2') mylevel.set_PacStartDir(BAS);
				else if(s[i]=='3') mylevel.set_PacStartDir(GAUCHE);
				mylevel.set_EltType(line, nb_val, mylevel.get_PacStartDir());
				i++;
			}
			else if(type=='4') //GHOSTS
			{
				i+=2;
				nghosts = mylevel.get_NbGhosts();
				if(nghosts < NB_MAX_GHOSTS) mylevel.set_NbGhosts(++nghosts);
				else
				{
					std::cerr << "Fatal Error: Too much ghosts!" << std::endl;
					exit(EXIT_FAILURE);
				}
				mylevel.set_BlockType(line, nb_val, GHOST);
				if(s[i]=='0') ghost=ROUGE;
				else if(s[i]=='1') ghost=VIOLET;
				else if(s[i]=='2') ghost=BLEU;
				else if(s[i]=='3') ghost=JAUNE;
				mylevel.set_GhostStartX(nghosts-1, nb_val);
				mylevel.set_GhostStartX(nghosts-1, line);
				mylevel.set_GhostColor(nghosts-1, ghost);
				mylevel.set_EltType(line, nb_val, ghost);
				i++; 
			}
			else
			{
				std::cerr <<  "Bad data while loading level..." << std::endl;
				exit(EXIT_FAILURE);
			}
			nb_val ++;
		}
		else i++;
	}
	if (nb_val != NB_BLOCKS_LARGEUR)
	{
		std::cerr << "Error, file level not correctly formated: Expected " << NB_BLOCKS_LARGEUR << " values, only " << nb_val << " values were read" << std::endl;
		exit(EXIT_FAILURE);
	}
}

/*Dessine le niveau à l'écran*/
void LevelManager::draw_level(Level &mylevel, Graphics &graph)
{
	SDL_Rect position, p1, p2;
	for(int i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		position.y=i*BLOCK_SIZE;
		for(int j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			position.x=j*BLOCK_SIZE;
			affiche_une_case(mylevel.get_Case(i,j), &position, graph.get_Screen());
		}
	}
	p1.x=p2.x=WIDTH; p1.y=HEIGHT-1; p2.y=0;
	graph.draw_line(p1, p2, blanc, graph.get_Screen());
}

//Affiche une case... ^^
void LevelManager::affiche_une_case(Case c, SDL_Rect *pos, SDL_Surface *s)
{
	if(c.type==MUR) SDL_BlitSurface(wall_texture[c.elt_type], NULL, s, pos);
	else if(c.type==BONUS) SDL_BlitSurface(bonus_texture[c.elt_type], NULL, s, pos);
	else if(c.type==PACMAN) SDL_BlitSurface(pacman_texture[c.elt_type], NULL, s, pos);
	else if(c.type==GHOST) SDL_BlitSurface(ghosts_texture[c.elt_type], NULL, s, pos);
}

//Si on est pile dans une case
/*int dans_case(SDL_Rect pos)
{
	if( (pos.x % BLOCK_SIZE==0) && (pos.y % BLOCK_SIZE==0) ) return 1;
	else return 0;
}*/

/*Renvoie les index de case en fonction de la position
 * et de la direction*/
/*SDL_Rect get_case(SDL_Rect position, int direction)
{
	SDL_Rect pos;
	switch(direction)
	{
		case DROITE:
			pos.x = (position.x+BLOCK_SIZE-1)/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
		case BAS:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = (position.y+BLOCK_SIZE-1)/BLOCK_SIZE;
			break;
		default:
			pos.x = position.x/BLOCK_SIZE;
			pos.y = position.y/BLOCK_SIZE;
			break;
	}
	return pos;
}*/

/*Quand un bloc est supprimé durant la partie
 * modifie les blocs alentours pour conserver
 * une cohérence dans l'agencement des murs*/
void LevelManager::remove_bloc(int y, int x, Level &mylevel)
{
	mylevel.set_BlockType(y, x, RIEN);//LEVEL[y][x].type=RIEN;
	switch(mylevel.get_EltType(y, x))
	{
		case 14:
			if(x && mylevel.get_BlockType(y,x-1)==MUR)
				mylevel.set_EltType(y, x-1, remove_right_bloc(mylevel.get_EltType(y,x-1)));
			if(x < NB_BLOCKS_LARGEUR && mylevel.get_BlockType(y,x+1)==MUR)
				mylevel.set_EltType(y, x+1, remove_left_bloc(mylevel.get_EltType(y,x+1)));
			break;
		case 15:
			if(y && mylevel.get_BlockType(y-1,x)==MUR)
				mylevel.set_EltType(y-1, x, remove_down_bloc(mylevel.get_EltType(y-1,x)));
			if(y < NB_BLOCKS_HAUTEUR && mylevel.get_BlockType(y+1,x)==MUR)
				mylevel.set_EltType(y+1, x, remove_up_bloc(mylevel.get_EltType(y+1,x)));
			break;
		case 16:
			if(y<NB_BLOCKS_HAUTEUR && mylevel.get_BlockType(y+1,x)==MUR)
				mylevel.set_EltType(y+1, x, remove_up_bloc(mylevel.get_EltType(y+1,x)));
			break;
		case 17:
			if(x && mylevel.get_BlockType(y,x-1)==MUR)
				mylevel.set_EltType(y, x-1, remove_right_bloc(mylevel.get_EltType(y,x-1)));
			break;
		case 18:
			if(y && mylevel.get_BlockType(y-1,x)==MUR)
				mylevel.set_EltType(y-1, x, remove_down_bloc(mylevel.get_EltType(y-1,x)));
			break;
		case 19:
			if(x<NB_BLOCKS_LARGEUR && mylevel.get_BlockType(y,x+1)==MUR)
				mylevel.set_EltType(y, x+1, remove_left_bloc(mylevel.get_EltType(y,x+1)));
			break;
	}
}

void LevelManager::erase_level(Level &lvl)
{
	for(int i=0; i<NB_BLOCKS_HAUTEUR; i++)
	{
		for(int j=0; j<NB_BLOCKS_LARGEUR; j++)
		{
			lvl.set_BlockType(i, j ,RIEN);
		}
	}
	lvl.set_NbGhosts(0);
	lvl.set_Dots(0);
}

void LevelManager::add_level(std::string s)
{
	level_filename.push_back(s);
	nb_level++;
}

/*********Fonctions de remplacement des blocs**************/
int LevelManager::remove_right_bloc(int elt)
{
	if(elt==0) return 11;
	else if(elt==3) return 10;
	else if(elt==5) return 12;
	else if(elt==6) return 2;
	else if(elt==7) return 1;
	else if(elt==8) return 4;
	else if(elt==14) return 17;
	else return elt;
}

int LevelManager::remove_left_bloc(int elt)
{
	if(elt==0) return 13;
	else if(elt==2) return 12;
	else if(elt==4) return 10;
	else if(elt==6) return 5;
	else if(elt==8) return 3;
	else if(elt==9) return 1;
	else if(elt==14) return 19;
	else return elt;
}

int LevelManager::remove_up_bloc(int elt)
{
	if(elt==1) return 10;
	else if(elt==2) return 11;
	else if(elt==5) return 13;
	else if(elt==6) return 0;
	else if(elt==7) return 3;
	else if(elt==9) return 4;
	else if(elt==15) return 16;
	else return elt;
}

int LevelManager::remove_down_bloc(int elt)
{
	if(elt==1) return 12;
	else if(elt==3) return 13;
	else if(elt==4) return 11;
	else if(elt==7) return 5;
	else if(elt==8) return 0;
	else if(elt==9) return 2;
	else if(elt==15) return 18;
	else return elt;
}
/*******************FIN Fonctions de remplacement de blocs*********************/
