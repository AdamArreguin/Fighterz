//Author: Matthew Gonzales
//
//
#include "header.h"

Image play[1] = {"fighterZLeft.png"};

class player {
	public:
		int health;
		int xres, yres;
		int playerFrame;
		Vec box[20];
		player() {
			health = 100;
			xres = 609;
			yres = 266;
			playerFrame = 0;
		}

} pl;

void playerHealthRender() {
	Rect r;
	r.bot = 800;
	r.left = 425;
	r.center = 0;
	ggprint16(&r, 32, 0x00ffffff, "Player Health: %i", pl.health);


}

void playerRender() {

}

void takePunch() {
	pl.health -= 5;
}

void takeKick() {
	pl.health -= 10;
}
