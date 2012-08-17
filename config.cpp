#include "config.hpp"

Player::Player()
{
}

Player::~Player()
{
}

Config::Config()
{
	nb_players = 1;
	players[0].set_Char(PACMAN);
	players[0].set_Up(SDLK_UP);
	players[0].set_Down(SDLK_DOWN);
	players[0].set_Left(SDLK_LEFT);
	players[0].set_Right(SDLK_RIGHT);

	players[1].set_Char(GHOST);
	players[1].set_Up(SDLK_z);
	players[1].set_Down(SDLK_s);
	players[1].set_Left(SDLK_q);
	players[1].set_Right(SDLK_d);
}

