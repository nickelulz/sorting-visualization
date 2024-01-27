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
#include <memory>

using namespace std;

// config
const char *PRESS_START_2P_PATH = "./res/fonts/PressStart2P-Regular.ttf";
const auto RESOLUTION = sf::VideoMode::getDesktopMode();
const uint16_t RECTANGLE_WIDTH = 5, RECTANGLE_OUTLINE_WIDTH = 1;
const uint16_t RECTANGLE_COUNT = WIDTH / (RECTANGLE_WIDTH + 2);

enum sort_type {
  // O(n^2)
  BUBBLE,
  INSERTION,
  SELECTION,

  // O(nlogn)
  MERGE,
  QUICK,

  // O(n+k)
  RADIX,
}

struct timer {
  chrono::high_resolution_clock::time_point frame_start, frame_end;
  float fps;
  uint64_t current_frame = 0;
} timer_t;

struct state_t {
  bool show_debug;
  enum sort_type sort_alg;
}

int main(void) {
  sf::Font PRESS_START_2P;

	if (!PRESS_START_2P.loadFromFile(PRESS_START_2P_PATH)) {
		cout << "Could not load font Press Start 2P." << endl;
		exit(EXIT_FAILURE);
	}

  // Graphics
  sf::VideoMode video_mode(WIDTH, HEIGHT);
  sf::RenderWindow window(video_mode, "Sorting Visualizer", sf::Style::Fullscreen);

	// Window settings
  window.setFramerateLimit(120);
	window.setVerticalSyncEnabled(true);

	sf::Text debug_text("", PRESS_START_2P, 20);
	bool show_debug;

	cout << "Launch Fullscreen:" << RESOLUTION.width << "x" << RESOLUTION.height;

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

  struct timer_t timer;
  struct state_t state;
  while (window.isOpen()) {
    poll_events(&state);
    perform_sort(&rectangles);
    update_screen(&window, &rectangles);
    timer_benchmark(&timer);
  }
}

unique_ptr<vector<sf::RectangleShape>> generate_rectangles

void poll_events(struct state_t *state) {
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
}

void perform_sort(vector<sf::RectangleShape> *rectangles) {
  // if statement for rate-limiting
  if (!(state->rate_limiting && timer->frame % state->rate_limit))
    perform_sort(rectangles);
}

void update_screen(struct state_t *state, vector<sf::RectangleShape> *rectangles) {
  window.clear();

  // Update rectangle position
  for (uint32_t i = 0; i < rectangles.size(); i++) {
    rectangles[i].setPosition(
      (RECTANGLE_WIDTH + RECTANGLE_OUTLINE_WIDTH * 2) * i,
      window.getSize().y - rectangles[i].getGlobalBounds().height
    );
  }

  // Draw rectangle graphs
  for_each(begin(*rectangles), end(*rectangles), [&window](auto rect) {
    window.draw(rect);
  });

  // if statement for rate-limiting
  if (!(state->rate_limiting && timer->frame % state->rate_limit))
    perform_sort(rectangles);
}

void timer_benchmark(struct timer_t *timer) {
  // end current frame
  timer->frame_end = chrono::high_resolution_clock::now();

  // calculate FPS
  fps = float(1e9) / float(
    chrono::duration_cast<
      chrono::nanoseconds(frame_end - frame_start)
        .count());

  // begin next frame
  timer->frame_start = chrono::high_resolution_clock::now();
  timer->frame_end = 0;
}
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

    // if statement for rate-limiting
    if (true) {
      switch
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
				window.getSize().x < "x" << window.getSize().y << endl;
			debug_string << "fps: " << uint32_t(fps) << endl;
			debug_text.setString(debug_string.str());

			window.draw(debug_text);
		}

		window.display();
		++current_frame;
	}
}
