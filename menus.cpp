#include "menus.hpp"

/*Les parametres par defaut des menus*/
Menu::Menu(int nb)
{
	int i;
	font_size=30; //Taille des caractères
	space=60; //espace entre les options
	/*Toutes les options sont selectionnables*/
	for(i=0; i<nb; i++)
	{
		options.push_back("");
		couleur.push_back(blanc);
		available.push_back(true);
	}
	img = IMG_Load("image/menu/menu.png"); //Le pacman pour montrer l'option selectionné
	if(img == NULL )
	{
		fprintf(stderr, "Erreur loading menu.png in >> Menu::Menu(int)<<\n");
		exit(EXIT_FAILURE);
	}
	nb_options = nb;
	selection=0;
}

Menu::~Menu()
{
	free(img);
}

/*Rajouter le centrage des éléments
 * Vérifier que tout entre dans l'ecran
 * sinon réduire la police ou faire
 * des colonnes*/
int Menu::draw_menu(Graphics &g)
{
	int i;
	Input in;
	while(!available[selection]) selection=(selection+1)%(nb_options);
	for(i=0; i<nb_options; i++) if(!available[i]) couleur[i]=gris;
	while(!in.get_Quit())
	{
		couleur[selection]=blanc;
		in.UpdateEvents();
		if(in.get_Key(SDLK_RETURN))
		{
			in.set_Key(SDLK_RETURN, 0);
			return selection;
		}
		else if(in.get_Key(SDLK_DOWN))
		{
			in.set_Key(SDLK_DOWN, 0);
			selection=(selection+1)%(nb_options);
			while(!available[selection]) selection=(selection+1)%(nb_options);
		}
		else if(in.get_Key(SDLK_UP))
		{
			in.set_Key(SDLK_UP, 0);
			if(!selection) selection=nb_options-1;
			else selection=(selection-1)%(nb_options);
			while(!available[selection])
			{
				if(!selection) selection=nb_options-1;
				else selection=(selection-1)%(nb_options);
			}
		}
		SDL_FillRect(g.get_Screen(), NULL, SDL_MapRGB((g.get_Screen())->format, 0, 0, 0));
		couleur[selection]=jaune;
		p1.x=100; p1.y=25;
		g.aff_pol(title, 50, p1, jaune, g.get_Screen());
		p1.x=300; p1.y=100;
		pos.x=p1.x-60;
		pos.y=p1.y+(selection*space)-5;
		SDL_BlitSurface(img, NULL, g.get_Screen(), &pos);
		for(i=0; i<nb_options; i++)
		{
			g.aff_pol(options[i], font_size, p1, couleur[i], g.get_Screen());
			p1.y=p1.y+space;
		}
		SDL_Flip(g.get_Screen());
	}
	return nb_options-1;
}

/*void edit_campagne()
{
	
}*/

/*Quand on appuis echap lors d'une partie*/
/*int game_menu()
{
	int nb = 3;
	Menu menu;
	init_menu(&menu, nb);
	strcpy(menu.title, "PAUSE");
	strcpy(menu.options[0], "CONTINUER");
	strcpy(menu.options[1], "SAUVER");
	strcpy(menu.options[2], "MENU PRINCIPAL");
	if(!SAVE_ENABLE) menu.available[1]=0;
	int selection = draw_menu(menu);
	delete_menu(&menu);
	return selection;
}*/

/*Affiche en début de niveau le niveau actuel
 * et un petit compteur de 3 sec*/
/*void play_menu(int level)
{
	char tmp[16], time[4];
	POINT p1,p2;
	int counter=3;
	sprintf(tmp, "LEVEL  %d", level+1);
	p1.x=320; p1.y=200;
	p2.x=380; p2.y=260;
	while(counter)
	{
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		sprintf(time, "%d", counter);
		aff_pol(tmp, 50, p1, jaune);
		aff_pol(time, 50, p2, jaune);
		SDL_Flip(screen);
		SDL_Delay(1000);
		counter--;
	}
}*/

/*Ceci est un menu special
 * Il ne suit pas les memes
 * regles que les autres menus
 * Accessoirement c'est tres moche*/
/*void options_menu(config *cfg)
{
	config tmp = *cfg;
	int i, j=0, nb=9, couleur[nb], available[nb], selection=0;
	SDL_Surface *pac[2], *ghost[2], *fleche[2];
	SDL_Rect pos;
	POINT p1;
	Input in;
	char title[64];
	char options[nb][64];

	for(i=0; i<nb; i++)
	{
		available[i]=1;
		couleur[i]=blanc;
	}
	pac[0]   = IMG_Load("image/pacman/2.png");
	pac[1]   = IMG_Load("image/pacman/3.png");
	ghost[0] = IMG_Load("image/ghosts/2.png");
	ghost[1] = IMG_Load("image/ghosts/3.png");
	fleche[0] = IMG_Load("image/menu/triangler.png");
	fleche[1] = IMG_Load("image/menu/trianglel.png");
	if(pac == NULL || ghost == NULL)
	{
		fprintf(stderr, "Erreur loading pngs in >>options_menu()<<\n");
		exit(EXIT_FAILURE);
	}
	memset(&in,0,sizeof(in));
	strcpy(title, "OPTIONS");
	strcpy(options[0], "Nb players");
	sprintf(options[1], "%d", cfg->nb_players);
	strcpy(options[2], "Player 1");
	strcpy(options[3], "Player 2");
	strcpy(options[4], "Sound");
	strcpy(options[5], "Reset score");
	strcpy(options[6], "Reset");
	strcpy(options[7], "Cancel");
	strcpy(options[8], "Save");
	available[1]=0;
	available[4]=0;
	if(cfg->nb_players==0)
	{
		available[2]=0;
		available[3]=0;
	}
	else if(cfg->nb_players==1) available[3]=0;
	DELAY=60;
	while(!in.quit)
	{
		SDL_Delay(DELAY);
		for(i=0; i<nb; i++)
		{
			if(!available[i] && i!=1) couleur[i]=gris;
			else couleur[i]=blanc;
		}
		couleur[selection]=blanc;
		UpdateEvents(&in);
		if(in.key[SDLK_ESCAPE]) exit(EXIT_SUCCESS);
		else if(in.key[SDLK_RETURN])
		{
			in.key[SDLK_RETURN]=0;
			if(selection==nb-4) //reset score
			{
				reset_score("data/results.txt");
				reset_score("data/survivor.txt");
			}
			if(selection==nb-3)
			{
				load_default_config(cfg); //default config
				sprintf(options[1], "%d", cfg->nb_players);
				available[2]=1;
				available[3]=0;
			}
			else if(selection==nb-2) //Cancel
			{
				*cfg=tmp;
				return;
			}
			else if(selection==nb-1) //Save
			{
				if(cfg->nb_players==2)
				{
					if(cfg->players[0].character == PACMAN && cfg->players[1].character == PACMAN)
						fprintf(stderr, "Only one pacman is allowed!\n"); //Should not happen
					else return;
				}
				else return;
			}
		}
		else if(in.key[SDLK_DOWN])
		{
			in.key[SDLK_DOWN]=0;
			selection=(selection+1)%nb;
			while(!available[selection]) selection=(selection+1)%nb;
		}
		else if(in.key[SDLK_UP])
		{
			in.key[SDLK_UP]=0;
			if(!(selection)) selection=nb-1;
			else selection=(selection-1)%nb;
			while(!available[selection])
			{
				if(!selection) selection=nb-1;
				else selection=(selection-1)%nb;
			}
		}
		else if(in.key[SDLK_LEFT])
		{
			in.key[SDLK_LEFT]=0;
			if(selection==0) {
				if(cfg->nb_players>0)
				{
					available[cfg->nb_players+1]=0;
					cfg->nb_players=cfg->nb_players-1;
					sprintf(options[1], "%d", cfg->nb_players);
				}
			}
			else if(selection==2 || selection==3) {
				if(cfg->players[selection-2].character == PACMAN) cfg->players[selection-2].character = GHOST;
				else if(cfg->players[selection-2].character == GHOST) cfg->players[selection-2].character = PACMAN;
			}
		}
		else if(in.key[SDLK_RIGHT])
		{
			in.key[SDLK_RIGHT]=0;
			if(selection==0)
			{
				if(cfg->nb_players<2) {
					available[cfg->nb_players+2]=1;
					cfg->nb_players=(cfg->nb_players+1);
					sprintf(options[1], "%d", cfg->nb_players);
				}
			}
			else if(selection==2 || selection==3) {
				if(cfg->players[selection-2].character == PACMAN) cfg->players[selection-2].character = GHOST;
				else if(cfg->players[selection-2].character == GHOST) cfg->players[selection-2].character = PACMAN;
			}
		}
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		couleur[selection]=jaune;

		p1.x=300; p1.y=50;
		aff_pol(title, 50, p1, jaune); //Le titre

		p1.x=310; p1.y=150;
		aff_pol(options[0], 30, p1, couleur[0]); //Nb players
		pos.x=p1.x+120; pos.y=p1.y;
		SDL_BlitSurface(fleche[1], NULL, screen, &pos);
		p1.x+=150;
		aff_pol(options[1], 30, p1, couleur[1]); //Nb players
		pos.x+=50;
		SDL_BlitSurface(fleche[0], NULL, screen, &pos);
		//Afficher des fleches

		j=(j+1)%2;
		p1.x=250; p1.y=225;
		pos.x=p1.x+10; pos.y=p1.y+50;
		aff_pol(options[2], 30, p1, couleur[2]); //Player 1
		SDL_BlitSurface(fleche[1], NULL, screen, &pos);
		pos.x+=25;
		if(cfg->players[0].character == GHOST) SDL_BlitSurface(ghost[j], NULL, screen, &pos);
		else SDL_BlitSurface(pac[j], NULL, screen, &pos);
		pos.x+=25;
		SDL_BlitSurface(fleche[0], NULL, screen, &pos);
		//Afficher des fleches

		p1.x+=200;
		pos.x=p1.x+10;
		aff_pol(options[3], 30, p1, couleur[3]); //Player 2
		SDL_BlitSurface(fleche[1], NULL, screen, &pos);
		pos.x+=25;
		if(cfg->players[1].character == GHOST) SDL_BlitSurface(ghost[j], NULL, screen, &pos);
		else SDL_BlitSurface(pac[j], NULL, screen, &pos);
		pos.x+=25;
		SDL_BlitSurface(fleche[0], NULL, screen, &pos);
		//Afficher des fleches

		p1.x=250; p1.y+=100;
		aff_pol(options[4], 30, p1, couleur[4]); //Sound
		//Afficher des images pour le son

		p1.x=250; p1.y+=75;
		aff_pol(options[5], 30, p1, couleur[5]); //Reset score

		p1.x=150; p1.y+=100;
		for(i=6; i<nb; i++)
		{
			aff_pol(options[i], 30, p1, couleur[i]);
			p1.x+=225;
		}
		SDL_Flip(screen);
	}
}*/

/*Quand tu gagnes*/
/*void win_menu()
{
	POINT p1;
	p1.x=200; p1.y=250;
	aff_pol("YOU WON !", 75, p1, jaune);
	SDL_Flip(screen);
	SDL_Delay(3000);
}*/

/*Quand tu perds*/
/*void lost_menu()
{
	POINT p1;
	p1.x=200; p1.y=250;
	aff_pol("YOU LOST !", 75, p1, jaune);
	SDL_Flip(screen);
	SDL_Delay(3000);
}*/

/*Affiche la version du jeu*/
/*void draw_version()
{
	POINT p1;
	p1.x=EDIT_WIDTH-50; p1.y=EDIT_HEIGHT-30;
	aff_pol(VERSION, 20, p1, jaune);
}*/

/*void delete_menu(Menu *menu)
{
	free(menu->options);
	free(menu->couleur);
	free(menu->img);
	free(menu->available);
}*/
