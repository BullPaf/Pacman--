#include "graphics.hpp"
#include "editeur.hpp"

int main(int argc, char **argv)
{
	Graphics graphics_manager(EDIT_WIDTH, HEIGHT, "Pacman");
	Editor myeditor;
	myeditor.editer(graphics_manager);
	return 0;
}

