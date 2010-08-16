#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <unistd.h>

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>

#include "lacube.h"

int main(int argc, char *argv[]) {
	double alpha = 0;
	int deg = 0;
	double rot[2][2];

	SDL_Event event;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		std::cerr << "SDL konnte nicht initialisiert werden: "
				<< SDL_GetError() << std::endl;
		exit(1);
	}
	atexit(SDL_Quit);

	SDL_WM_SetCaption("Basic matrix operations", "Basic matrix operations");

	SDL_Surface *screen;

	screen = SDL_SetVideoMode(screen_width, screen_height, 16, SDL_SWSURFACE);
	if (screen == NULL) {
		std::cerr << "Ich konnte kein Fenster öffnen: " << SDL_GetError()
				<< std::endl;
		exit(1);
	}

	kos kos(screen_width, 10);

	kos.add_point(10, 10);
	kos.add_point(-10, 10);
	kos.add_point(10, -10);
	kos.add_point(-10, -10);

	deg = 1;
	alpha = M_PI / 180 * deg; // rotate with 12 deg

	while (true) {
		alpha = M_PI / 180 * deg; // rotate with deg degrees
		
		rot[0][0] = cos(alpha);
		rot[0][1] = -sin(alpha);
		rot[1][0] = sin(alpha);
		rot[1][1] = cos(alpha);

		kos.execute(rot);
		kos.draw(screen);

		while (SDL_PollEvent(&event)) { /* Loop until there are no events left on the queue */
			switch (event.type) { /* Process the appropiate event type */
			case SDL_QUIT:
				std::cout << "Good bye!" << std::endl;
				return 0;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					float x = (event.button.x - kos.x0) / kos.scale;
					float y = (event.button.y - kos.y0) / kos.scale;
					std::cout << "you clicked -> new point will be added: " << x << " " << y << std::endl;
					kos.add_point(x, y);
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					deg++;
					break;
				case SDLK_DOWN:
					deg--;
					break;
				case SDLK_q:
					return 0;
					break;
				case SDLK_c:
					kos.points.clear();
					break;
				default:
					std::cout << "unhandled key: " << event.key.keysym.sym << std::endl;
				}
				break;
		}
	}

	SDL_Delay(50);
}

return 0;
}

void kos::add_point(double x, double y) {
	std::vector<double> point(2);
	point[0] = x;
	point[1] = y;

	points.push_back(point);
}

void kos::draw(SDL_Surface *screen) {

	// clear screen
	SDL_FillRect(screen, NULL, 0xffffffff);

	// axes
	draw_axes(screen);

	// lines

	// points
	for (std::vector<std::vector<double> >::iterator i = points.begin(); i
			!= points.end(); i++) {
		draw_point(*i, screen);
	}

	SDL_Flip(screen); // refresh screen
}

kos::kos(int _max, int _scale) {
	max = _max;
	scale = _scale;

	x0 = screen_width / 2;
	y0 = screen_height / 2;
}

void kos::draw_point(std::vector<double> point, SDL_Surface *screen) {
	filledEllipseColor(screen, x0 + (point[0] * scale),
			y0 + (point[1] * scale), 4, 4, point_color);
}

void kos::draw_axes(SDL_Surface *screen) {
	lineColor(screen, screen_padding, y0, screen_width - screen_padding, y0,
			axes_color); // x-Achse
	lineColor(screen, x0, screen_padding, x0 - 5, screen_padding + 5,
			axes_color); // Pfeile
	lineColor(screen, x0, screen_padding, x0 + 5, screen_padding + 5,
			axes_color);

	for (int i = 0; i < ((screen_width / 2) - screen_padding) / scale; i++) {
		lineColor(screen, x0 + i * scale, y0 - 2, x0 + i * scale, y0 + 2,
				axes_color); // x-Skalierung
		lineColor(screen, x0 - i * scale, y0 - 2, x0 - i * scale, y0 + 2,
				axes_color);

		lineColor(screen, x0 - 2, y0 + i * scale, x0 + 2, y0 + i * scale,
				axes_color); // y-Skalierung
		lineColor(screen, x0 - 2, y0 - i * scale, x0 + 2, y0 - i * scale,
				axes_color);
	}

	lineColor(screen, x0, screen_padding, x0, screen_height - screen_padding,
			axes_color); // y-Achse
	lineColor(screen, screen_width - screen_padding, x0, screen_width
			- screen_padding - 5, y0 - 5, axes_color); // Pfeile
	lineColor(screen, screen_width - screen_padding, y0, screen_width
			- screen_padding - 5, y0 + 5, axes_color);

	//lineColor(screen, 0, screen_height, screen_width, 0, axes_color); // z-Achse

}

void kos::execute(double mat[2][2]) {
	for (std::vector<std::vector<double> >::iterator i = points.begin(); i
			!= points.end(); i++) {
		std::vector<double> npoint(2, 0);

		for (int p = 0; p < 2; p++) {
			for (int q = 0; q < 2; q++) {
				npoint[p] += i->at(q) * mat[p][q];
			}
		}

		*i = npoint;
	}
}
