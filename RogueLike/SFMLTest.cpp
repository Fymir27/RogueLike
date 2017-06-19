#include <SFML/Graphics.hpp>

int showTestWindow()
{

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	window.setFramerateLimit(60); // call it once, after creating the window
	sf::CircleShape shape(50.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen())
	{
		shape.scale(0.99, 0.99);
		shape.move(1, 1);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type = sf::Event::KeyPressed)
			{
				shape.setScale(1, 1);
				shape.setPosition(0, 0);
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}