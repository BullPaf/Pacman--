#ifndef H_GRAPHICS
#define H_GRAPHICS

#include <iostream>
#include <SDL.h>
//#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <string>
#include <ctime>

// les constantes de couleur
                       //   r     g     b
#define noir    0x0000 // 0000 000000 000000
#define gris    0xf3cf // 1111 001111 001111
#define blanc   0xffff // 1111 111111 111111
#define rouge   0xf000 // 1111 000000 000000
#define vert    0xfc0  // 0000 111111 000000
#define bleu    0x3f   // 0000 000000 111111
#define jaune   0xffc0 // 1111 111111 000000
#define cyan    0xfff  // 0000 111111 111111
#define magenta 0xf03f // 1111 000000 111111

class Graphics
{
	public:
		Graphics(int, int, std::string const);
		~Graphics();
		void draw_line(SDL_Rect const, SDL_Rect const, Uint16 const, SDL_Surface*);
		void draw_rectangle(SDL_Rect const, SDL_Rect const, Uint16 const, SDL_Surface*); // dessine un rectangle non rempli
		void draw_cross(SDL_Rect const, int const, Uint16 const, SDL_Surface*); //dessine une croix
		void aff_pol(std::string const, int const, SDL_Rect const, Uint16 const, SDL_Surface*); //Ecrit du texte à l'écran
		int dans_ecran(int const, int const, SDL_Surface* const) const;

	private:
		SDL_Surface* m_screen; //fenetre sur laquelle on dessine 
		std::string m_policeName, m_title;
		int m_width, m_height, m_bits;
};

#endif
