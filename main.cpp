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
const uint16_t 
  RECTANGLE_WIDTH = 5,
  RECTANGLE_OUTLINE_WIDTH = 1;
const uint16_t RECTANGLE_COUNT = RESOLUTION.width / (RECTANGLE_WIDTH + 2);

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
};

struct frame_timer_t {
  chrono::high_resolution_clock::time_point
    frame_start, frame_end;
  float fps;
  uint64_t current_frame = 0;
};

struct state_t {
  bool show_debug, rate_limiting;
  enum sort_type sort_alg;

  /*
   * The number of frames between
   * each stage of the sort
   */
  uint8_t rate_limit;
};

struct graphics_t {
  // render context
  sf::VideoMode video_mode;
  sf::RenderWindow window;

  // render objects
  sf::Font PRESS_START_2P;
  sf::Text debug_text;
};

static uint32_t sort_counter = 0;
void perform_sort(state_t *state,
    vector<sf::RectangleShape> *rectangles) {
  switch (state->sort_alg) {
    case BUBBLE:
    case INSERTION:
    case SELECTION:
    case MERGE:
    case QUICK:
    case RADIX:
      break;
  }

  cout << "sort: " << sort_counter++ << endl;
}

void initialize(graphics_t *graphics) {
  // Graphics
  graphics->video_mode = sf::VideoMode(
     RESOLUTION.width,
     RESOLUTION.height);
  graphics->window = sf::RenderWindow(
     graphics->video_mode,
     "Sorting Visualizer",
     sf::Style::Fullscreen);

  if (!graphics->PRESS_START_2P.loadFromFile(PRESS_START_2P_PATH)) {
    cout << "Could not load font Press Start 2P." << endl;
    exit(EXIT_FAILURE);
  }

  // Window settings
  graphics->window.setFramerateLimit(120);
  graphics->window.setVerticalSyncEnabled(true);
  graphics->debug_text = sf::Text("", PRESS_START_2P, 20);
}

vector<sf::RectangleShape> generate_rectangles {
  vector<sf::RectangleShape> rectangles;
  for (uint32_t n = 0; n < RECTANGLE_COUNT; ++n) {
    sf::RectangleShape new_rect;
    new_rect.setSize(sf::Vector2f(RECTANGLE_WIDTH, n));
    new_rect.setFillColor(sf::Color::White);
    new_rect.setOutlineColor(sf::Color::Black);
    new_rect.setOutlineThickness(RECTANGLE_OUTLINE_WIDTH);
    rectangles.push_back(new_rect);
  }
  random_shuffle(begin(rectangles), end(rectangles));
  return rectangles;
}

void poll_events(state_t *state) {
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

void update_screen(graphics_t *graphics,
    state_t *state,
    vector<sf::RectangleShape> *rectangles) {
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

  // Apply the debug menu
  if (show_debug) {
    ostringstream debug_string;

    debug_string << window.getSize().x < "x" << window.getSize().y << endl;
    debug_string << "fps: " << uint32_t(fps) << endl;
    debug_text.setString(debug_string.str());

    window.draw(debug_text);
  }

  window.display();
}

void timer_benchmark(frame_timer_t *timer) {
  // end current frame
  timer->frame_end = chrono::high_resolution_clock::now();
  timer->current_frame++;

  // calculate FPS
  fps = float(1e9) / float(
    chrono::duration_cast<
      chrono::nanoseconds(frame_end - frame_start)
        .count());

  // begin next frame
  timer->frame_start = chrono::high_resolution_clock::now();
  timer->frame_end = 0;
}

// test
int main(void) {
  cout << "Launch Fullscreen:" <<
    RESOLUTION.width << "x" << RESOLUTION.height;

  frame_timer_t timer;
  state_t state;
  graphics_t graphics;

  initialize(&graphics);
  auto rectanges = generate_rectangles();

  while (graphics.window.isOpen()) {
    poll_events(&state);

    // if statement for rate-limiting
    if (!(state->rate_limiting && timer->frame % state->rate_limit == 0))
      perform_sort(&rectangles);

    update_screen(&window, &state, &rectangles);
    timer_benchmark(&timer);
  }
}
