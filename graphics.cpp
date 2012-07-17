#include "graphics.hpp"

/* initialise le graphique */
Graphics::Graphics(int width, int height, std::string const title)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_EnableUNICODE(1);
	m_policeName = "angelina.ttf";
	m_title      = title;
	m_width      = width;
	m_height     = height;
	m_bits       = 16;
	m_screen     = SDL_SetVideoMode(m_width, m_height, m_bits, SDL_SWSURFACE|SDL_DOUBLEBUF);
	if ( m_screen == NULL )
	{
		std::cerr << "Impossible de passer en "<< m_width << "x" << m_height << " en " << m_bits << " bpp: " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	SDL_ShowCursor(SDL_DISABLE);
	SDL_WM_SetCaption(m_title.c_str(), NULL);
}

/* dessine un segment */
void Graphics::draw_line(SDL_Rect const p1, SDL_Rect const p2, Uint16 const color, SDL_Surface* s)
{
	int xmin, xmax;
	int ymin, ymax;
	int i,j;
	float a,b,ii,jj;

	if (p1.x < p2.x) {xmin=p1.x; xmax=p2.x;} else{xmin=p2.x; xmax=p1.x;}
	if (p1.y < p2.y) {ymin=p1.y; ymax=p2.y;} else{ymin=p2.y; ymax=p1.y;}

	if (xmin==xmax) for (j=ymin;j<=ymax;j++) if (dans_ecran(xmin,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + xmin) = color;
	if (ymin==ymax) for (i=xmin;i<=xmax;i++) if (dans_ecran(i,ymin,s)) *((Uint16 *)s->pixels + (s->h-ymin-1) * s->w + i) = color;


	/* On affiche la droite en faisant varier la coordonnée de plus grande pente */
	if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (i=xmin;i<=xmax;i++)
		{
			jj = a*i+b;
			j = jj;
			if (((jj-j) > 0.5) && (j < s->h-1)) j++;
			if (dans_ecran(i,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + i) = color;
		}
	}

	if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0))
	{
		a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
		b = p1.y - a*p1.x;
		for (j=ymin;j<=ymax;j++)
		{
			/*dessine un segment */
			ii = (j-b)/a;
			i = ii;
			if (((ii-i) > 0.5) && (i < s->w-1)) i++;
			if (dans_ecran(i,j,s)) *((Uint16 *)s->pixels + (s->h-j-1) * s->w + i) = color;
		}
	}
}

/* dessine un rectangle non rempli */
void Graphics::draw_rectangle(SDL_Rect const p1, SDL_Rect const p2, Uint16 const color, SDL_Surface* s)
{
	SDL_Rect tmp;
	tmp.x=p1.x;
	tmp.y=p2.y;
	draw_line(p1, tmp, color, s);
	draw_line(tmp, p2, color, s);
	tmp.x=p2.x;
	tmp.y=p1.y;
	draw_line(p1, tmp, color, s);
	draw_line(tmp, p2, color, s);
}

//dessine une croix
void Graphics::draw_cross(SDL_Rect const p1, int const T, Uint16 const color, SDL_Surface* s)
{
	SDL_Rect p2, p3, p4, p5;

	p2.x = p1.x + T/2; p2.y = p1.y - T/2;
	p3.x = p1.x - T/2; p3.y = p1.y - T/2;
	p4.x = p1.x - T/2; p4.y = p1.y + T/2;
	p5.x = p1.x + T/2; p5.y = p1.y + T/2;

	draw_line(p1, p2, color, s);
	draw_line(p1, p3, color, s);
	draw_line(p1, p4, color, s);
	draw_line(p1, p5, color, s);
}

// Affiche le texte passé en argument
// necessite l'installation  de SDL_ttf.h
void Graphics::aff_pol(std::string const a_ecrire, int const taille, SDL_Rect const p, Uint16 const color, SDL_Surface *s)
{
	SDL_Surface *texte = NULL;
	/*Uint16 mytexte[64];
	int i;
	for(i=0; i<strlen(a_ecrire); i++)
	{
		mytexte[i]=a_ecrire[i];
	}
	mytexte[i]='\0';*/
	SDL_Rect position;
	TTF_Font *police = NULL;
	SDL_Color Noire   = {  0,   0,   0};
	SDL_Color Gris   =  {100, 100, 100};
	SDL_Color Blanche = {255, 255, 255};

	SDL_Color Rouge   = {255,   0,   0};
	SDL_Color Verte   = {  0, 255,   0};
	SDL_Color Bleue   = {  0,   0, 255};

	SDL_Color Jaune   = {255, 255,   0};
	SDL_Color Cyan    = {  0, 255, 255};
	SDL_Color Magenta = {255,   0, 255};
	SDL_Color sdl_C;

	sdl_C = Noire;
	if (color == blanc) sdl_C = Blanche;
	if (color == gris)  sdl_C = Gris;
	if (color == rouge) sdl_C = Rouge;
	if (color == vert)  sdl_C = Verte;
	if (color == bleu)  sdl_C = Bleue;

	if (color == jaune)   sdl_C = Jaune;
	if (color == cyan)    sdl_C = Cyan;
	if (color == magenta) sdl_C = Magenta;

	TTF_Init();
	//SDL_EnableUNICODE(1);
	/* Chargement de la police */
	police = TTF_OpenFont(m_policeName.c_str(), taille);
	/* Ecriture du texte dans la SDL_Surface "texte" en mode shaded (optimal) */
	if (police) texte = TTF_RenderText_Blended(police, a_ecrire.c_str(), sdl_C);
	else
	{
		std::cerr << "Impossible d'ouvrir la police: "<< m_policeName << " >> " << SDL_GetError() << std::endl;
		exit(EXIT_FAILURE);
	}
	//if (police) texte = TTF_RenderUNICODE_Blended(police, mytexte, sdl_C);

	position.x = p.x;
	position.y = p.y;
	if (texte) SDL_BlitSurface(texte, NULL, s, &position); /* Blit du texte par-dessus */

	if (police) TTF_CloseFont(police);
	TTF_Quit();
	if (texte) SDL_FreeSurface(texte);
	//SDL_EnableUNICODE(0);
}

/* on vérifie que ça ne déborde pas */
int Graphics::dans_ecran(int const x, int const y, SDL_Surface* const s) const
{
	if (x<0) return 0;
	if (x>=s->w) return 0;
	if (y<0) return 0;
	if (y>=s->h) return 0;
	return 1;
}

Graphics::~Graphics()
{
	SDL_Quit();
}
