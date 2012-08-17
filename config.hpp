#ifndef H_CONFIG
#define H_CONFIG
#include <stdlib.h>
#include "levelmanager.hpp"
#include "graphics.hpp"

class Player
{
	public:
		Player();
		~Player();
		void set_Up(int k) {up = k;}
		void set_Down(int k) {down = k;}
		void set_Left(int k) {left = k;}
		void set_Right(int k) {right = k;}
		void set_Char(int c) {character = c;}
	private:
		int up, down, left, right; //Keys for directions
		int character;
};

class Config
{
	public:
		Config();
		~Config();
	private:
		int nb_players;
		Player players[2];
};

#endif
