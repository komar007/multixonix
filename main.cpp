#include "geometry.h"
#include "shapemanager.h"
#include "collision.h"
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace std;

void draw_circle(sf::RenderWindow& app, Point pos, const sf::Color& col)
{
	sf::Shape h;
	h.AddPoint(pos.x/10*app.GetWidth()-4, pos.y/10*app.GetHeight(), col);
	h.AddPoint(pos.x/10*app.GetWidth(), pos.y/10*app.GetHeight()-4, col);
	h.AddPoint(pos.x/10*app.GetWidth()+4, pos.y/10*app.GetHeight(), col);
	h.AddPoint(pos.x/10*app.GetWidth(), pos.y/10*app.GetHeight()+4, col);
	app.Draw(h);
}

int main()
{
	sf::RenderWindow app(sf::VideoMode(800, 600), "Xonix", sf::Style::Close);
	ShapeManager m(true);
	ShapeManager q(false);
	m.attach(q);
	m.add_shape(Path(true, vector<Point>{{1,1}, {9,1}, {9,9}, {1,9}}));
	bool running = true;
	Point pos(5, 9.5);
	Point old_pos(5,9.5);
	double angle = -M_PI/2;
	int trace = -1;
	Point balls[] = {{2,3}, {3,4}, {5, 7}};
	double ball_angles[] = {0.2, 0.5, 1.2};
	int beg;
	while (running) {
		app.Clear();
		bool check_trace = false;
		for (auto it = m.begin(); it != m.end(); ++it) {
			const Shape& shape = *it;
			if (trace == shape.get_id()) {
				check_trace = true;
				continue;
			}
			vector<Detector::Intersection> where;
			if (shape.get_detector().segment_intersections(old_pos, pos, where) > 0) {
				if (trace == -1) {
					trace = m.start_trace(where[0].second);
					beg = where[0].first.get_index();
				} else {
					m.extend_trace(trace, where[0].second);
					pair<int, int> ids = m.cut_shape(trace, shape.get_id(), beg, where[0].first.get_index());
					m.destroy_shape(shape.get_id());
					m.destroy_shape(trace);
					check_trace = false;
					trace = -1;
					const Shape& s1 = m.get_shape_const_ref(ids.first);
					const Shape& s2 = m.get_shape_const_ref(ids.second);
					int nballs1=0, nballs2=0;
					for (int i = 0; i < 3; ++i) {
						vector<Detector::Intersection> where;
						if (s1.get_detector().segment_intersections(balls[i], Point(-1, -1), where) % 2 == 1)
							nballs1++;
						if (s2.get_detector().segment_intersections(balls[i], Point(20, 20), where) % 2 == 1)
							nballs2++;
					}
					if (nballs1 == 0)
						m.destroy_shape(ids.first);
					if (nballs2 == 0)
						m.destroy_shape(ids.second);
				}
				break;
			}
		}
		cerr << "\r";
		if (trace != -1)
			m.extend_trace(trace, pos);
		for (auto it = q.begin(); it != q.end(); ++it) {
			const Shape& shape = *it;
			if (!shape.get_path().closed) {
				int num = 0;
				for (auto i = shape.get_path().begin(), j = i+1; i != shape.get_path().end(); ++i, ++j) {
					sf::Shape s = sf::Shape::Line(i->x/10*app.GetWidth(), i->y/10*app.GetHeight(), j->x/10*app.GetWidth(), j->y/10*app.GetHeight(), 1, sf::Color(0, 255, 0));
					app.Draw(s);
					++num;
				}
				cerr << num;
				continue;
			}
			int num = 0;
			for (auto i = shape.get_path().begin(), j = i+1; i != shape.get_path().end(); ++i, ++j){
				sf::Shape s = sf::Shape::Line(i->x/10*app.GetWidth(), i->y/10*app.GetHeight(), j->x/10*app.GetWidth(), j->y/10*app.GetHeight(), 1, sf::Color(255, 0, 0));
				app.Draw(s);
				++num;
			}
			cerr << num;
			cerr << " ";
		}
		for (int i = 0; i < 3; ++i) {
			Point old = balls[i];
			balls[i].x += 0.02*cos(ball_angles[i]);
			balls[i].y += 0.02*sin(ball_angles[i]);
			if (check_trace) {
				const Shape& tr = m.get_shape_const_ref(trace);
				vector<Detector::Intersection> where;
				if (tr.get_detector().segment_intersections(old, balls[i], where) > 0) {
					m.destroy_shape(trace);
					trace = -1;
					pos = {5, 9.5};
					break;
				}
			}
			for (auto it = m.begin(); it != m.end(); ++it) {
				const Shape& sh = *it;
				if (trace == sh.get_id())
					continue;
				vector<Detector::Intersection> where;
				if (sh.get_detector().segment_intersections(old, balls[i], where) > 0) {
					Point p1 = *where[0].first;
					Point p2 = *(where[0].first+1);
					double b = asin((p2.y - p1.y) / (p2 - p1).length());
					ball_angles[i] = - ball_angles[i] + 2*b;
					if (ball_angles[i] > 2*M_PI)
						ball_angles[i] -= 2*M_PI;
					if (ball_angles[i] < 0)
						ball_angles[i] += 2*M_PI;
					balls[i] = old + Vector(0.03*cos(ball_angles[i]), 0.03*sin(ball_angles[i]));
				}
			}
			draw_circle(app, balls[i], sf::Color(0, 0, 255));
		}
		draw_circle(app, pos, sf::Color(255, 0, 0));
		app.Display();
		sf::Event e;
		while (app.GetEvent(e)) {
			switch (e.Type) {
			case sf::Event::Closed:
				running = false;
				break;
			default: break;
			}
		}
		const sf::Input& input = app.GetInput();
		if (input.IsKeyDown(sf::Key::Up)) {
			old_pos = pos;
			pos.x = pos.x + 0.04*cos(angle) + rand()%100 * 0.01 * 0.0001;
			pos.y = pos.y + 0.04*sin(angle) + rand()%100 * 0.01 * 0.0001;
		}
		if (input.IsKeyDown(sf::Key::Left)) {
			angle -= 0.05;
		}
		if (input.IsKeyDown(sf::Key::Right)) {
			angle += 0.05;
		}
		sf::Sleep(0.02);

	}
	return 0;
}
