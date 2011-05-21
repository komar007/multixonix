#include "xonix.h"
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
	Xonix x(Xonix::MASTER);
	Ball ball(Point(5, 5));
	ball.angle = 0.2;
	ball.speed = 0.04;
	x.add_actor(ball);
	Player player(Point(5,1));
	player.angle = 1.5;
	player.speed = 0.04;
	int player_id = x.add_actor(player);
	Player& rplayer = dynamic_cast<Player&>(x.get_actors().get_actor_ref(player_id));
	bool running = true;
	while (running) {
		x.step();
		app.Clear();
		for (auto it = x.get_shapes().begin(); it != x.get_shapes().end(); ++it) {
			const Shape& shape = *it;
			if (!shape.get_path().closed) {
				for (auto i = shape.get_path().begin(), j = i+1; i != shape.get_path().end(); ++i, ++j) {
					sf::Shape s = sf::Shape::Line(i->x/10*app.GetWidth(), i->y/10*app.GetHeight(), j->x/10*app.GetWidth(), j->y/10*app.GetHeight(), 1, sf::Color(0, 255, 0));
					app.Draw(s);
				}
				continue;
			}
			for (auto i = shape.get_path().begin(), j = i+1; i != shape.get_path().end(); ++i, ++j){
				sf::Shape s = sf::Shape::Line(i->x/10*app.GetWidth(), i->y/10*app.GetHeight(), j->x/10*app.GetWidth(), j->y/10*app.GetHeight(), 1, sf::Color(255, 0, 0));
				app.Draw(s);
			}
		}
		for (auto it = x.get_actors().begin(); it != x.get_actors().end(); ++it)
			draw_circle(app, it->get_pos(), sf::Color(0, 0, 255));
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
		}
		if (input.IsKeyDown(sf::Key::Left)) {
			rplayer.angle -= 0.05;
		}
		if (input.IsKeyDown(sf::Key::Right)) {
			rplayer.angle += 0.05;
		}
		sf::Sleep(0.02);
	}
	return 0;
}
