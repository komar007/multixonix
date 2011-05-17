#include "geometry.h"
#include "shapemanager.h"
#include "collision.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "future_classes.h"

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
	m.add_shape(Path(true, vector<Point>{{1,1}, {9,1}, {9,9}, {1,9}}));
	bool running = true;
	Point pos(5, 0.5);
	Point old_pos(5,0.5);
	double angle = M_PI/2 + 0.01;
	int trace = -1;
	Point balls[] = {{2,3}, {3,4}, {5, 7}};
	double ball_angles[] = {0.2, 0.5, 1.2};
	int beg;
	while (running) {
		app.Clear();
		bool check_trace = false;
		for (auto it = m.begin(); it != m.end(); ++it) {
			if (trace == it->first) {
				check_trace = true;
				continue;
			}
			const Shape& shape = *it->second;
			int where;
			if (shape.get_detector().segment_intersections(old_pos, pos, where) > 0) {
				Point inter = segment_segment_intersection(old_pos, pos, *shape.get_path().nth_point(where), *shape.get_path().nth_point(where+1));
				if (trace == -1) {
					trace = m.start_trace(inter);
					beg = where;
				} else {
					m.extend_trace(trace, inter);
					pair<int, int> ids = m.cut_shape(trace, it->first, beg, where);
					m.destroy_shape(it->first);
					m.destroy_shape(trace);
					check_trace = false;
					trace = -1;
					const Shape& s1 = m.get_shape_const_ref(ids.first);
					const Shape& s2 = m.get_shape_const_ref(ids.second);
					int nballs1=0, nballs2=0;
					for (int i = 0; i < 3; ++i) {
						int where;
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
		for (auto it = m.begin(); it != m.end(); ++it) {
			const Shape& shape = *it->second;
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
				int where;
				if (tr.get_detector().segment_intersections(old, balls[i], where) > 0) {
					m.destroy_shape(trace);
					trace = -1;
					pos = {5, 0.5};
					break;
				}
			}
			for (auto it = m.begin(); it != m.end(); ++it) {
				if (trace == it->first)
					continue;
				const Shape& sh = *it->second;
				int where;
				if (sh.get_detector().segment_intersections(old, balls[i], where) > 0) {
					Point p1 = *sh.get_path().nth_point(where);
					Point p2 = *sh.get_path().nth_point(where+1);
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
