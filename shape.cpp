#include "shape.h"

using namespace std;

Shape::Shape(const Path& _path, bool with_detector)
	: id(-1)
	, path(_path)
	, detector(NULL)
{
	initialize(with_detector);
}

Shape::Shape(Path&& _path, bool with_detector)
	: id(-1)
	, path(move(_path))
	, detector(NULL)
{
	initialize(with_detector);
}

void Shape::initialize(bool with_detector)
{
	if (with_detector)
		detector = new Detector(path, Vector(0, 0), 0.05); //FIXME: choose sensible values
}

int Shape::intersections(const Point& s1, const Point& s2, vector<Detector::Intersection>& out_edges) const
throw (logic_error)
{
	if (!detector)
		throw logic_error("intersections called on shape without detector");
	return detector->segment_intersections(s1, s2, out_edges);
}

bool Shape::point_inside(const Point& p)
{
	if (!detector)
		throw logic_error("point_inside called on shape without detector");
	vector<Detector::Intersection> out;
	int n = detector->segment_intersections(p, detector->get_box().first-Vector(1, 1), out);
	return n % 2 == 1;
}

Shape::~Shape()
{
	delete detector;
}

void Shape::extend(const Point& point) throw (domain_error)
{
	if (path.closed)
		throw domain_error("closed path in Shape::extend");
	path.push_back(point);
	if (detector && path.size() >= 2)
		detector->add_segment(path.nth_point(path.size()-2));
}
