#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <loguru.cpp>

#include <algorithm>
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "config.hpp"

using namespace std;

sf::VideoMode video_mode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
sf::RenderWindow window(video_mode, "Sorting Visualizer");

// Resources/States
sf::Font PRESS_START_2P;

// Monitors
chrono::high_resolution_clock::time_point frame_start, frame_end;
float fps;
uint64_t current_frame = 0;

int main(void) {
	if (!PRESS_START_2P.loadFromFile(PRESS_START_2P_PATH)) {
		LOG_F(FATAL, "Could not load font Press Start 2P.");
		exit(EXIT_FAILURE);
	}

	// Window settings
	window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);

	sf::Text debug_text("", PRESS_START_2P, 20);
	bool show_debug;

	LOG_F(INFO, "Launch: %d x %d px", 
		DEFAULT_WINDOW_WIDTH, 
		DEFAULT_WINDOW_HEIGHT);

	array<sf::RectangleShape, RECTANGLE_COUNT> rectangles;
	for (uint32_t n = 0; n < rectangles.size(); ++n) {
		rectangles[n].setSize(sf::Vector2f(RECTANGLE_WIDTH, n));
		rectangles[n].setFillColor(sf::Color::White);
		rectangles[n].setOutlineColor(sf::Color::Black);
		rectangles[n].setOutlineThickness(RECTANGLE_OUTLINE_WIDTH);
	}
	random_shuffle(begin(rectangles), end(rectangles));

	uint32_t current_index = 0;

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
			uint32_t smallest_index = current_index;

			for (uint32_t i = current_index+1; i < rectangles.size(); i++)
				if (rectangles[i].getGlobalBounds().height < rectangles[current_index].getGlobalBounds().height)
					smallest_index = i;

			LOG_F(INFO, "smallest: %.2f", rectangles[smallest_index].getGlobalBounds().height);

			auto temp = rectangles[smallest_index];
			rectangles[smallest_index] = rectangles[current_index];
			rectangles[current_index] = temp;

			++current_index;
		}

		// End FPS timer calculation
		frame_end = chrono::high_resolution_clock::now();

		// Calculate FPS
		fps = float(1e9) / float(chrono::duration_cast<chrono::nanoseconds>(frame_end - frame_start).count());

		// Apply the debug menu
		if (show_debug) {
			ostringstream debug_string;
			debug_string << window.getSize().x << "x" << window.getSize().y << endl;
			debug_string << "fps: " << uint32_t(fps) << endl;
			debug_text.setString(debug_string.str());

			window.draw(debug_text);
		}

		window.display();
		++current_frame;
	}
}