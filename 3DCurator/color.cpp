#include "color.h"

Color::Color() {
	red = green = blue = alpha = 1.0; // blanco opaco por defecto
}

Color::Color(const double alpha) {
	// color
	red = green = blue = 1.0; // blanco por defecto
	// opacidad
	if (alpha >= .0 && alpha <= 1.0) {
		this->alpha = alpha;
	} else {
		this->alpha = 1.0; // valor máximo si se introduce mal
	}
}

Color::Color(const double red, const double green, const double blue) {
	// rojo
	if (red >= .0 && red <= 1.0) {
		this->red = red;
	} else {
		this->red = 1.0; // valor máximo si se introduce mal
	}
	// verde
	if (green >= .0 && green <= 1.0) {
		this->green = green;
	} else {
		this->green = 1.0; // valor máximo si se introduce mal
	}
	// azul
	if (blue >= .0 && blue <= 1.0) {
		this->blue = blue;
	} else {
		this->blue = 1.0; // valor máximo si se introduce mal
	}
	// opacidad
	alpha = 1.0; // opaco por defecto
}

Color::Color(const double red, const double green, const double blue, const double alpha) {
	// rojo
	if (red >= .0 && red <= 1.0) {
		this->red = red;
	} else {
		this->red = 1.0; // valor máximo si se introduce mal
	}
	// verde
	if (green >= .0 && green <= 1.0) {
		this->green = green;
	} else {
		this->green = 1.0; // valor máximo si se introduce mal
	}
	// azul
	if (blue >= .0 && blue <= 1.0) {
		this->blue = blue;
	} else {
		this->blue = 1.0; // valor máximo si se introduce mal
	}
	// opacidad
	if (alpha >= .0 && alpha <= 1.0) {
		this->alpha = alpha;
	} else {
		this->alpha = 1.0; // valor máximo si se introduce mal
	}
}

Color::~Color() {

}

double Color::r() const {
	return red;
}

double Color::g() const {
	return green;
}

double Color::b() const {
	return blue;
}

double Color::a() const {
	return alpha;
}

void Color::r(const double red) {
	if (red >= .0 && red <= 1.0) {
		this->red = red;
	} else {
		this->red = 1.0; // valor máximo si se introduce mal
	}
}

void Color::g(const double green) {
	if (green >= .0 && green <= 1.0) {
		this->green = green;
	} else {
		this->green = 1.0; // valor máximo si se introduce mal
	}
}

void Color::b(const double blue) {
	if (blue >= .0 && blue <= 1.0) {
		this->blue = blue;
	} else {
		this->blue = 1.0; // valor máximo si se introduce mal
	}
}

void Color::a(const double alpha) {
	if (alpha >= .0 && alpha <= 1.0) {
		this->alpha = alpha;
	} else {
		this->alpha = 1.0; // valor máximo si se introduce mal
	}
}