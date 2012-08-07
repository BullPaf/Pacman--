#include "graphics.hpp"
#include "editeur.hpp"
#include "menus.hpp"

void create_menu(Menu &m)
{
	if(m.get_OptionsSize() < 9)
	{
		std::cerr << "Error : Menu Error size... expected 9 but size is " << m.get_OptionsSize() << std::endl;
		exit(EXIT_FAILURE);
	}
	m.set_Title("BIENVENUE DANS PACMAN!!");
	m.set_Option(0, "CAMPAGNE");
	m.set_Option(1, "EDIT CAMPAGNE");
	m.set_Option(2, "PARTIE UNIQUE");
	m.set_Option(3, "SURVIVOR");
	m.set_Option(4, "CHARGER");
	m.set_Option(5, "EDITER");
	m.set_Option(6, "OPTIONS");
	m.set_Option(7, "HIGH SCORE");
	m.set_Option(8, "QUITTER");
	//if(!has_saved_game()) menu.available[4]=0;
	//int selection = draw_menu(menu);
	//delete_menu(&menu);
	//return selection;
}

int main(int argc, char **argv)
{
	bool continuer=true;
	Graphics graphics_manager(EDIT_WIDTH, HEIGHT, "Pacman");
	Editor myeditor;
	Menu main_menu(9);
	create_menu(main_menu);
	while (continuer)
	{
		switch(main_menu.draw_menu(graphics_manager))
		{
			case 0: //Campagne
				/*if(CAMPAGNE_LEVEL)
				{
					load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
					campagne(&cfg, 0);
				}*/
				break;
			case 1: //Campagne
				//edit_campagne();
				break;
			case 2: //Jouer un niveau
				//one_level(select_file_menu(), &cfg);
				break;
			case 3: //Mode survivor
				//survivor(select_file_menu(), &cfg);
				break;
			case 4: // Charger une partie
				//load_default_config(&cfg); //Pour eviter de faire des campagnes avec des configs folkloriques
				//campagne(&cfg, load_game());
				break;
			case 5: //Edition
				myeditor.editer(graphics_manager);
				break;
			case 6: //Option
				//options_menu(&cfg);
				break;
			case 7: //Voir les meilleurs score
				//draw_result("data/results.txt", 0);
				break;
			case 8: //Quitter
				continuer=false;
				break;
			default: break;
		}
	}
	SDL_Quit();
	return 0;
}

