#ifndef COLOR_H
#define COLOR_H

class Color {

public:
	// constructor/destructor
    Color();
	Color(const double alpha);
	Color(const double red, const double green, const double blue);
	Color(const double red, const double green, const double blue, const double alpha);
    ~Color();

	// getters
    double r() const;
    double g() const;
    double b() const;
    double a() const;

	// setters
    void r(const double red);
    void g(const double green);
    void b(const double blue);
    void a(const double alpha);

private:
	// atributos
    double red;
    double green;
    double blue;
    double alpha;
};

#endif // COLOR_H
