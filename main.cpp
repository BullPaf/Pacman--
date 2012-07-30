#include "graphics.hpp"
#include "editeur.hpp"

int main(int argc, char **argv)
{
	Graphics graphics_manager(800, 600, "Pacman");
	Editor myeditor;
	myeditor.editer(graphics_manager);
	return 0;
}

