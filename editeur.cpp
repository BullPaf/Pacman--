#include "editeur.hpp"
#include "input.hpp"

Editor::Editor()
{
	int i;
	for(i=0; i<NB_WALL_BLOCKS; i++)
	{
		editor[i].type=MUR;
		editor[i].elt_type=i;
	}
	for(int j=0; j<NB_BONUS_BLOCKS; j++)
	{
		editor[i].type=BONUS;
		editor[i].elt_type=j;
		i++;
	}
	for(int j=0; j<NB_PACMAN_BLOCK; j++)
	{
		editor[i].type=PACMAN;
		editor[i].elt_type=j;
		i++;
	}
	for(int j=0; j<NB_GHOST_BLOCKS; j++)
	{
		editor[i].type=GHOST;
		editor[i].elt_type=j;
		i++;
	}
}

Editor::~Editor() {}

void Editor::edit_menu(Menu &m)
{
	m.set_Title("MENU EDITEUR");
	m.set_Option(0, "CONTINUER");
	m.set_Option(1, "SAUVER");
	m.set_Option(2, "CHARGER");
	m.set_Option(3, "EFFACER");
	m.set_Option(4, "TESTER");
	m.set_Option(5, "QUITTER");
	m.set_Available(4, false);
}

/*Le menu de séléction de fichier*/
void Editor::file_menu(Menu &m, LevelManager &lvlmng)
{
	int i;
	m.set_FontSize(25);
	m.set_Space(30);
	m.set_Title("Choississez un niveau");
	for(i=0; i<m.get_OptionsSize()-1; i++) m.set_Option(i, lvlmng.get_LevelName(i));
	m.set_Option(i, "Annuler");
}

/*
 * Met en place l'environement d'edition
 * il faut s'accrocher*/
void Editor::editer(Graphics &graph)
{
	/****************Initialisation*********************/
	SDL_WM_SetCaption("Editeur Pacman", NULL);
	SDL_ShowCursor(SDL_ENABLE);
	SDL_WarpMouse(WIDTH / 2, EDIT_HEIGHT / 2);
	SDL_Rect position, info;
	info.x=WIDTH+10;
	info.y=HEIGHT-100;
	position.x=position.y=0;
	int type=0, ok=1, message=AUCUN, tempsPrecedent=0, level=-1, selection=0;
	LevelManager lvlmng;
	Menu menu1(6);
	Level mylevel;
	Input in;
	/*****************Fin Initialisation*****************/
	while(ok)
	{
		in.UpdateEvents();
		position.x = (in.get_MouseX()/BLOCK_SIZE)*BLOCK_SIZE;
		position.y = (in.get_MouseY()/BLOCK_SIZE)*BLOCK_SIZE;
		if(in.get_Quit()) exit(EXIT_SUCCESS);
		/**************************SOURIS*************************/
		if (in.get_MouseButton(SDL_BUTTON_LEFT))
		{
			if( (in.get_MouseX() >= WIDTH) && (in.get_MouseY() < EDIT_WIDTH-1) ) //On click dans le menu de droite
			{
				type = get_block_type(in.get_MouseX(), in.get_MouseY(), type); //Recupère l'élément choisi
				position.x = in.get_MouseX()-BLOCK_SIZE/2; //Centre la texture sur le pointeur
				position.y = in.get_MouseY()-BLOCK_SIZE/2;
			}
			else if ( (in.get_MouseX() < WIDTH) && (in.get_MouseX() >= 0) ) //On veut placer l'objet
				plot_object(in.get_MouseX(), in.get_MouseY(), type, mylevel);
		}
		else if (in.get_MouseButton(SDL_BUTTON_RIGHT))
		{
			if( (in.get_MouseX() < WIDTH) && (in.get_MouseY() >= 0) ) { //Si on est sur la carte
				if(mylevel.get_BlockType(in.get_MouseY()/BLOCK_SIZE, in.get_MouseX()/BLOCK_SIZE) == GHOST)
					mylevel.set_NbGhosts(mylevel.get_NbGhosts()-1); //Si on efface un fantome
				mylevel.set_BlockType(in.get_MouseY()/BLOCK_SIZE, in.get_MouseX()/BLOCK_SIZE, RIEN); //On efface la texture
			}
		}
		else if (in.get_MouseButton(SDL_BUTTON_WHEELUP)) //Molette haute souris
		{
			type = (type-1)%(NB_ALL_BLOCKS);
			if (type < 0) type = NB_ALL_BLOCKS-1;
		}
		else if (in.get_MouseButton(SDL_BUTTON_WHEELDOWN)) type = (type+1)%(NB_ALL_BLOCKS); //Molette bas souris
		/**************************FIN SOURIS*************************/

		/**************************CLAVIER*************************/
		if(in.get_Key(SDLK_ESCAPE))
		{
			in.set_Key(SDLK_ESCAPE, 0);
			edit_menu(menu1);
			selection=menu1.draw_menu(graph);
			if(selection==1) //sauver le niveau
			{
				/*int tmp = save_menu(level);
				if(tmp==0) //On ecrase le fichier en cours
				{
					save_level(level);
					message=SAVE;
					tempsPrecedent = SDL_GetTicks();
				}
				if(tmp==1) //On décide de créer un nouveau niveau
				{
					char new_file[64];
					new_file_menu(new_file); //Taper le nom du niveau
					strcpy(LEVEL_FILE[NB_LEVEL], new_file);
					save_level(NB_LEVEL); //On le sauvegarde
					NB_LEVEL++; //Un niveau de plus youpi
					level=NB_LEVEL-1; //Pour se rappeler on édite quel niveau maintenant
					message=SAVE;
					tempsPrecedent = SDL_GetTicks();
				}*/
			}
			else if(selection == 2) //charger un niveau
			{
				std::cerr << lvlmng.get_NbLevel() << " Available" << std::endl;
				Menu menu2(lvlmng.get_NbLevel()+1);
				file_menu(menu2, lvlmng);
				level = menu2.draw_menu(graph);
				if(level<lvlmng.get_NbLevel())
				{
					lvlmng.load_level(level, mylevel);
					//message=LOAD;
					//tempsPrecedent = SDL_GetTicks();
				}
			}
			else if (selection == 3) //supprime tout
			{
				/*init_level();
				message=DELETE;
				tempsPrecedent = SDL_GetTicks();*/
			}
			else if (selection == 5) return;
		}
		/**************************FIN CLAVIER*************************/

		SDL_FillRect(graph.get_Screen(), NULL, SDL_MapRGB((graph.get_Screen())->format, 0, 0, 0));
		load_gui(graph, lvlmng); //Affiche l'interface
		highlight_block(type, graph); //Encadre l'élément actif
		lvlmng.draw_level(mylevel, graph); //Dessine le niveau
		//Dessine les éléments selectionnables
		if(type<NB_WALL_BLOCKS) SDL_BlitSurface(lvlmng.get_Wall_Texture(type), NULL, graph.get_Screen(), &position);
		else if(type<NB_BONUS_BLOCKS+NB_WALL_BLOCKS)
			SDL_BlitSurface(lvlmng.get_Bonus_Texture(type%NB_WALL_BLOCKS), NULL, graph.get_Screen(), &position);
		else if(type<NB_BONUS_BLOCKS+NB_WALL_BLOCKS+NB_PACMAN_BLOCK)
			SDL_BlitSurface(lvlmng.get_Pacman_Texture(type%(NB_WALL_BLOCKS+NB_BONUS_BLOCKS)), NULL, graph.get_Screen(), &position);
		else SDL_BlitSurface(lvlmng.get_Ghosts_Texture(type%(NB_WALL_BLOCKS+NB_BONUS_BLOCKS+NB_PACMAN_BLOCK)), NULL, graph.get_Screen(), &position);
		//print_info(&message, tempsPrecedent, info, level); //Affiche un message d'info s'il y en a
		SDL_Flip(graph.get_Screen());
	}
}

/*Affecte à une case l'élément séléctionné*/
void Editor::plot_object(int x, int y, int type, Level &mylevel)
{
	//Si on veut placer un pacman
	if(editor[type].type==PACMAN)
	{
		//Si un pacman existe déja on doit le supprimer
		for(int i=0; i<NB_BLOCKS_HAUTEUR; i++)
		{
			for(int j=0; j<NB_BLOCKS_LARGEUR; j++)
			{
				if(mylevel.get_BlockType(i,j)==PACMAN)
				{
					mylevel.set_BlockType(i,j,RIEN);
					break;
				}
			}
		}
	}
	//Si on veut placer un fantome
	else if(editor[type].type==GHOST)
	{
		if(mylevel.get_NbGhosts() >= NB_MAX_GHOSTS) return;//Trop de fantome
		else if(mylevel.get_BlockType(y/BLOCK_SIZE,x/BLOCK_SIZE) != GHOST)
			mylevel.set_NbGhosts(mylevel.get_NbGhosts()+1); //Si on plot un fantome sur un autre pas d'incrément
	}
	else if(mylevel.get_BlockType(y/BLOCK_SIZE,x/BLOCK_SIZE) == GHOST) mylevel.set_NbGhosts(mylevel.get_NbGhosts()-1); //Si on efface un fantome
	mylevel.set_Case(y/BLOCK_SIZE, x/BLOCK_SIZE, editor[type]);
}

/*Affiche des messages dans le menu de droite*/
/*void print_info(int *message, int tempsPrecedent, POINT p, int level)
{
	int tempsActuel;
	char tmp[128];
	sprintf(tmp, "%s ", LEVEL_FILE[level]);
	switch(*message)
	{
		case AUCUN:
			break;
		case SAVE:
			tempsActuel = SDL_GetTicks();
			strcat(tmp, "saved !");
			if (tempsActuel - tempsPrecedent < 5000) aff_pol(tmp, FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		case LOAD:
			tempsActuel = SDL_GetTicks();
			strcat(tmp, "loaded !");
			if (tempsActuel - tempsPrecedent < 5000) aff_pol(tmp, FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		case DELETE:
			tempsActuel = SDL_GetTicks();
			if (tempsActuel - tempsPrecedent < 5000) aff_pol("Level destroyed !", FONT_SIZE, p, blanc);
			else *message=AUCUN;
			break;
		default : break;
	}
}*/

/*
 * Charge l'interface graphique de l'editeur
 * en gros trace des carrée et affiche les
 * textures à l'interieur
*/
void Editor::load_gui(Graphics &graph, LevelManager &lvlmng)
{
	SDL_Rect position, p1, p2;
	int nb_col = 5, nb_ligne=(NB_ALL_BLOCKS-1)/nb_col + 1;
	p1.x=p2.x=WIDTH; p1.y=HEIGHT-1; p2.y=0;
	graph.draw_line(p1, p2, blanc, graph.get_Screen());
	p2.x=WIDTH+nb_col*(BLOCK_SIZE+3);
	for(int i=0; i<=nb_ligne; i++)
	{
		p1.y=p2.y=HEIGHT-(1+i*(BLOCK_SIZE+3));
		graph.draw_line(p1, p2, blanc, graph.get_Screen());
	}
	p1.y=HEIGHT-1;
	for(int i=0; i<=nb_col; i++)
	{
		p1.x=p2.x=WIDTH+i*(BLOCK_SIZE+3);
		graph.draw_line(p1, p2, blanc, graph.get_Screen());
	}
	for (int i=0; i<NB_ALL_BLOCKS; i++)
	{
		position.x=2+WIDTH+((i%5)*(BLOCK_SIZE+3));
		position.y=2+(BLOCK_SIZE+3)*(i/5);
		lvlmng.affiche_une_case(editor[i], &position, graph.get_Screen());
	}
}

/*
 * Encadre en rouge l'élément correspondant
 * au type
*/
void Editor::highlight_block(int type, Graphics &graph)
{
	SDL_Rect p1, p2;
	p1.x=WIDTH+1+(type%5)*(BLOCK_SIZE+3);
	p2.x=p1.x+BLOCK_SIZE+1;
	
	p1.y=HEIGHT-2-(type/5)*(BLOCK_SIZE+3);
	p2.y=p1.y-(BLOCK_SIZE+1);
	graph.draw_rectangle(p2, p1, rouge, graph.get_Screen());
}

/*
 * En fonction de la zone clické
 * Renvoie le type de l'élément correspondant.
 * Si la zone cliqué ne correspond à aucun élément
 * l'ancien type est retourné
*/
int Editor::get_block_type(int x, int y, int type)
{
	int col = (x-WIDTH)/(BLOCK_SIZE+3);
	int line = y/(BLOCK_SIZE+3);
	int new_type = line*5 + col;
	if(new_type<NB_ALL_BLOCKS) return new_type;
	else return type;
}
