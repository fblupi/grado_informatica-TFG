#include "transferFunction.h"

TransferFunction::TransferFunction() {
	colorFun = vtkSmartPointer<vtkColorTransferFunction>::New();
	scalarFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
	gradientFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
	tf.name = "tf";
	tf.description = "";
}

void TransferFunction::read(std::string &filename) {
	using boost::property_tree::ptree;
	ptree pt;

	read_xml(filename, pt);

	TF tf;
	tf.name = pt.get<std::string>("tf.<xmlattr>.name");
	tf.description = pt.get<std::string>("tf.<xmlattr>.description");

	BOOST_FOREACH(ptree::value_type const &v, pt.get_child("tf.color")) {
		if (v.first == "point") {
			ColorPoint cp;
			cp.x = v.second.get<double>("x");
			cp.r = v.second.get<double>("r");
			cp.g = v.second.get<double>("g");
			cp.b = v.second.get<double>("b");
			cp.m = v.second.get<double>("m", 0.5);
			cp.s = v.second.get<double>("s", 0.0);
			tf.colorTF.push_back(cp);
		}
	}

	BOOST_FOREACH(ptree::value_type const &v, pt.get_child("tf.scalar")) {
		if (v.first == "point") {
			OpacityPoint op;
			op.x = v.second.get<double>("x");
			op.y = v.second.get<double>("y");
			op.m = v.second.get<double>("m", 0.5);
			op.s = v.second.get<double>("s", 0.0);
			tf.scalarTF.push_back(op);
		}
	}

	BOOST_FOREACH(ptree::value_type const &v, pt.get_child("tf.gradient")) {
		if (v.first == "point") {
			OpacityPoint op;
			op.x = v.second.get<double>("x");
			op.y = v.second.get<double>("y");
			op.m = v.second.get<double>("m", 0.5);
			op.s = v.second.get<double>("s", 0.0);
			tf.gradientTF.push_back(op);
		}
	}

	this->tf = tf;

	update();
}

void TransferFunction::write(std::string &filename) {
	using boost::property_tree::ptree;
	ptree pt;

	pt.put("tf.<xmlattr>.name", tf.name);
	pt.put("tf.<xmlattr>.description", tf.description);

	ptree &color = pt.add("tf.color", "");
	BOOST_FOREACH(ColorPoint cp, tf.colorTF) {
		ptree &node = color.add("tf.color.point", "");
		node.put("x", cp.x);
		node.put("r", cp.r);
		node.put("g", cp.g);
		node.put("b", cp.b);
		node.put("m", cp.m);
		node.put("s", cp.s);
	}

	ptree &scalar = pt.add("tf.scalar", "");
	BOOST_FOREACH(OpacityPoint op, tf.scalarTF) {
		ptree &node = scalar.add("tf.scalar.point", "");
		node.put("x", op.x);
		node.put("y", op.y);
		node.put("m", op.m);
		node.put("s", op.s);
	}

	ptree &gradient = pt.add("tf.gradient", "");
	BOOST_FOREACH(OpacityPoint op, tf.gradientTF) {
		ptree &node = gradient.add("tf.gradient.point", "");
		node.put("x", op.x);
		node.put("y", op.y);
		node.put("m", op.m);
		node.put("s", op.s);
	}

	write_xml(filename, pt);
}

void TransferFunction::update() {
	clear();

	BOOST_FOREACH(ColorPoint p, tf.colorTF) {
		colorFun->AddRGBPoint(p.x, p.r, p.g, p.b, p.m, p.s);
	}

	BOOST_FOREACH(OpacityPoint p, tf.scalarTF) {
		scalarFun->AddPoint(p.x, p.y, p.m, p.s);
	}

	BOOST_FOREACH(OpacityPoint p, tf.gradientTF) {
		gradientFun->AddPoint(p.x, p.y, p.m, p.s);
	}
}

vtkSmartPointer<vtkColorTransferFunction> TransferFunction::getColorFun() const {
	return colorFun;
}

vtkSmartPointer<vtkPiecewiseFunction> TransferFunction::getScalarFun() const {
	return scalarFun;
}

vtkSmartPointer<vtkPiecewiseFunction> TransferFunction::getGradientFun() const {
	return gradientFun;
}

void TransferFunction::clear() {
	colorFun->RemoveAllPoints();
	scalarFun->RemoveAllPoints();
	gradientFun->RemoveAllPoints();
}

void TransferFunction::addColorPoint(const double x, const double r, const double g, const double b) {
	ColorPoint p;
	p.x = x;
	p.r = r;
	p.g = g;
	p.b = b;
	p.m = 0.5;
	p.s = 0.0;
	tf.colorTF.push_back(p);
	colorFun->AddRGBPoint(p.x, p.r, p.g, p.b, p.m, p.s);
}

void TransferFunction::addColorPoint(const double x, const double r, const double g, const double b, const double m, const double s) {
	ColorPoint p;
	p.x = x;
	p.r = r;
	p.g = g;
	p.b = b;
	p.m = m;
	p.s = s;
	tf.colorTF.push_back(p);
	colorFun->AddRGBPoint(p.x, p.r, p.g, p.b, p.m, p.s);
}

void TransferFunction::addScalarPoint(const double x, const double y) {
	OpacityPoint p;
	p.x = x;
	p.y = y;
	p.m = 0.5;
	p.s = 0.0;
	tf.scalarTF.push_back(p);
	scalarFun->AddPoint(p.x, p.y, p.m, p.s);
}

void TransferFunction::addScalarPoint(const double x, const double y, const double m, const double s) {
	OpacityPoint p;
	p.x = x;
	p.y = y;
	p.m = m;
	p.s = s;
	tf.scalarTF.push_back(p);
	scalarFun->AddPoint(p.x, p.y, p.m, p.s);
}

void TransferFunction::addGradientPoint(const double x, const double y) {
	OpacityPoint p;
	p.x = x;
	p.y = y;
	p.m = 0.5;
	p.s = 0.0;
	tf.gradientTF.push_back(p);
	gradientFun->AddPoint(p.x, p.y, p.m, p.s);
}

void TransferFunction::addGradientPoint(const double x, const double y, const double m, const double s) {
	OpacityPoint p;
	p.x = x;
	p.y = y;
	p.m = m;
	p.s = s;
	tf.gradientTF.push_back(p);
	gradientFun->AddPoint(p.x, p.y, p.m, p.s);
}