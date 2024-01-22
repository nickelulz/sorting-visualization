#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <chrono>
#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <array>

using namespace std;

// config
const char *PRESS_START_2P_PATH = "./res/fonts/PressStart2P-Regular.ttf";

// rectangle stuff
const uint16_t WIDTH = sf::VideoMode::getDesktopMode().width,
      HEIGHT = sf::VideoMode::getDesktopMode().height;
const uint16_t RECTANGLE_WIDTH = 5;
const uint16_t RECTANGLE_COUNT = WIDTH / (RECTANGLE_WIDTH + 2);
const uint16_t RECTANGLE_OUTLINE_WIDTH = 1;

sf::VideoMode video_mode(WIDTH, HEIGHT); 
sf::RenderWindow window(video_mode, "Sorting Visualizer", sf::Style::Fullscreen);

// Resources/States
sf::Font PRESS_START_2P;

// Monitors
chrono::high_resolution_clock::time_point frame_start, frame_end;
float fps;
uint64_t current_frame = 0;

int main(void) {
	if (!PRESS_START_2P.loadFromFile(PRESS_START_2P_PATH)) {
		cout << "Could not load font Press Start 2P." << endl;
		exit(EXIT_FAILURE);
	}

	// Window settings
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);

	sf::Text debug_text("", PRESS_START_2P, 20);
	bool show_debug;

	cout << "Launch Fullscreen:" << WIDTH << "x" << HEIGHT;

	vector<sf::RectangleShape> rectangles;
	for (uint32_t n = 0; n < RECTANGLE_COUNT; ++n) {
		sf::RectangleShape r;
		r.setSize(sf::Vector2f(RECTANGLE_WIDTH, n));
		r.setFillColor(sf::Color::White);
		r.setOutlineColor(sf::Color::Black);
		r.setOutlineThickness(RECTANGLE_OUTLINE_WIDTH);
		rectangles.push_back(r);
	}
	random_shuffle(begin(rectangles), end(rectangles));

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Enable/disable debug menu
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
			show_debug = true;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			show_debug = false;

		// Begin timer to calculate FPS
		frame_start = chrono::high_resolution_clock::now();

		window.clear();

		// Update rectangle position
		for (uint32_t i = 0; i < rectangles.size(); i++) {
			rectangles[i].setPosition(
				(RECTANGLE_WIDTH + RECTANGLE_OUTLINE_WIDTH * 2) * i,
				window.getSize().y - rectangles[i].getGlobalBounds().height
			);
		}

		// Draw rectangle graphs
		for_each(begin(rectangles), end(rectangles), [&window](auto rect) {
			window.draw(rect);
		});

		// Do a single sort
		if (true) { // intentional ratelimit
			for (size_t i = 0; i < rectangles.size(); i++) {
				if (rectangles[i].getFillColor() == sf::Color::Red)
					rectangles[i].setFillColor(sf::Color::White);
				if (rectangles[i].getGlobalBounds().height < 
				    rectangles[current_index].getGlobalBounds().height) {
					auto temp = rectangles[current_index];
					rectangles[]
				}
			}
		}

		// End FPS timer calculation
		frame_end = chrono::high_resolution_clock::now();

		// Calculate FPS
		fps = float(1e9) / float(chrono::duration_cast<chrono::nanoseconds>
			(frame_end - frame_start).count());

		// Apply the debug menu
		if (show_debug) {
			ostringstream debug_string;
			debug_string << 
				window.getSize().x << 
				"x" << window.getSize().y << endl;
			debug_string << "fps: " << uint32_t(fps) << endl;
			debug_text.setString(debug_string.str());

			window.draw(debug_text);
		}

		window.display();
		++current_frame;
	}
}
