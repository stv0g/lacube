#ifndef LACUBE_H_
#define LACUBE_H_

#include <vector>

int main(int argc, char *argv[]);

const int screen_height = 500;
const int screen_width = screen_height;
const int screen_padding = 10;

class kos {
	
	static const int point_color = 0x000000ff;
	static const int axes_color = 0x000000ff;
	
public:
	kos(int max, int scale);
	void add_point(double x, double y);
	void draw(SDL_Surface *screen);
	void execute(double mat[2][2]);
	double scale;
	int max;
	int x0;
	int y0;
	std::vector<std::vector<double> > points;

private:
	void draw_axes(SDL_Surface *screen);
	void draw_point(std::vector<double> point, SDL_Surface *screen);
	void draw_line(SDL_Surface *screen);
};

#endif /*LACUBE_H_*/
