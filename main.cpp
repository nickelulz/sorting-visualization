#include <SFML/Graphics.hpp>

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
const char *PRESS_START_2P_PATH =
  "./PressStart2P-Regular.ttf";
const auto RESOLUTION =
  sf::VideoMode::getDesktopMode();
const uint16_t
  RECTANGLE_WIDTH = 5,
  RECTANGLE_OUTLINE_WIDTH = 1;
const uint16_t RECTANGLE_COUNT =
  RESOLUTION.width / (RECTANGLE_WIDTH + 2);

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

  // O(n * n!)
  BOGO,

  SORT_ALG_COUNT
};

struct sort_detail_t {
  const char *name, *runtime;
  const sf::Keyboard::Key key_map;
  const bool complete;
};

const sort_detail_t SORT_DETAILS[] = {
  //              Name         Average O(x)   Key Map         Complete
  [BUBBLE]    = { "Bubble",    "O(n^2)",      sf::Keyboard::B, true },
  [INSERTION] = { "Insertion", "O(n^2)",      sf::Keyboard::I, false },
  [SELECTION] = { "Selection", "O(n^2)",      sf::Keyboard::S, false },
  [MERGE]     = { "Merge",     "O(n*log(n))", sf::Keyboard::M, false },
  [QUICK]     = { "Quick",     "O(n*log(n))", sf::Keyboard::Q, false },
  [RADIX]     = { "Radix",     "O(n+k)",      sf::Keyboard::R, false },
  [BOGO]      = { "Bogo",      "O(n*n!)",     sf::Keyboard::G, false },
};

struct frame_timer_t {
  chrono::high_resolution_clock::time_point
    frame_start, frame_end;
  float fps;
  uint64_t current_frame = 0;
};

union sort_trackers_t {
  struct { uint16_t inner, outer, selection; } lin;
};


struct state_t {
  bool show_debug, rate_limiting, sorted;
  enum sort_type sort_alg;
  vector<sf::RectangleShape> rectangles;
  sort_trackers_t sort_vars;

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

void perform_sort(state_t *state) {
  if (is_sorted(begin(state->rectangles),
        end(state->rectangles))) {
    state->sorted = true;
    for (auto& rect: state->rectangles)
      rect.setFillColor(sf::Color::Green);
    return;
  }

  switch (state->sort_alg) {

    /*
    case BUBBLE:
    {
      if (state->sort_vars.lin.outer >= state->rectangles.size())
        break;

      if (state->sort_vars.lin.inner == state->sort_vars.lin.outer - 1) {
        state->sort_vars.lin.inner = 0;
        state->sort_vars.lin.outer++;
      }
        
      if (state->rectangles[state->sort_vars.lin.inner].getLocalBounds().height >
          state->rectangles[state->sort_vars.lin.inner + 1].getLocalBounds().height) {
        auto& temp = state->rectangles[state->sort_vars.lin.inner];
        state->rectangles[state->sort_vars.lin.inner] =
          state->rectangles[state->sort_vars.lin.inner + 1];
        state->rectangles[state->sort_vars.lin.inner + 1] = temp;
      }

      state->sort_vars.lin.inner++;

      break;
    }
  */
  

    case INSERTION:
    {
      break;
    }
    case SELECTION:
    {
      break;
    }
    case MERGE:
    {
      break;
    }
    case QUICK:
    {
      break;
    }
    case RADIX:
    {
      break;
    }
    case BOGO:
    {
      break;
    }
  }

  cout << "sort: " << sort_counter++ << endl;
}

void initialize(graphics_t *graphics, state_t *state) {
  // Graphics
  graphics->video_mode = sf::VideoMode(
     RESOLUTION.width,
     RESOLUTION.height);
  graphics->window.create(
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
  graphics->debug_text =
    sf::Text("",
      graphics->PRESS_START_2P,
      20);
}

vector<sf::RectangleShape> generate_rectangles() {
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

void begin_sort(state_t *state, sort_type sort) {
  state->sort_alg = sort;
  state->sorted = false;
  state->rectangles = generate_rectangles();
}

void poll_events(state_t *state, graphics_t *graphics) {
  sf::Event event;
  while (graphics->window.pollEvent(event)) {
    if (event.type == sf::Event::Closed ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::X))
      graphics->window.close();
  }

  for (uint8_t i = 0; i < SORT_ALG_COUNT; ++i)
    if (sf::Keyboard::isKeyPressed(SORT_DETAILS[i].key_map))
      begin_sort(state, (sort_type) i);

  // Enable/disable debug menu
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
    state->show_debug = true;
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
    state->show_debug = false;
}

void update_screen(graphics_t *graphics,
    state_t *state, frame_timer_t *timer) {
  graphics->window.clear();

  // Update rectangle position
  for (uint32_t i = 0; i < state->rectangles.size(); ++i) {
    state->rectangles[i].setPosition(
      (RECTANGLE_WIDTH + RECTANGLE_OUTLINE_WIDTH * 2) * i,
      graphics->window.getSize().y -
        state->rectangles[i].getGlobalBounds().height
    );

    graphics->window.draw(state->rectangles[i]);
  }

  // Apply the debug menu
  if (state->show_debug) {
    ostringstream debug_string;

    debug_string << graphics->window.getSize().x
      << "x" << graphics->window.getSize().y << endl;
    debug_string << "FPS: " << uint32_t(timer->fps) << endl;
    debug_string << "Sorting Algorithm: " <<
      SORT_DETAILS[state->sort_alg].name << " Sort" << endl;
    debug_string << "Big O Runtime: " <<
      SORT_DETAILS[state->sort_alg].runtime << endl;

    if (state->sorted)
      debug_string << "SORT COMPLETE!" << endl;

    debug_string << endl;

    debug_string << "O -> Show Debug Menu" << endl;
    debug_string << "P -> Hide Debug Menu" << endl;

    debug_string << endl;

    debug_string << "B -> Bubble" << (SORT_DETAILS[BUBBLE].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "I -> Insertion" << (SORT_DETAILS[INSERTION].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "S -> Selection" << (SORT_DETAILS[SELECTION].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "M -> Merge" << (SORT_DETAILS[MERGE].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "Q -> Quick" << (SORT_DETAILS[QUICK].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "R -> Radix" << (SORT_DETAILS[RADIX].complete ? "" : " (INCOMPLETE)") << endl;
    debug_string << "G -> Bogo" << (SORT_DETAILS[BOGO].complete ? "" : " (INCOMPLETE)") << endl;

    debug_string << "X -> Quit" << endl;

    graphics->debug_text.setString(debug_string.str());
    graphics->window.draw(graphics->debug_text);
  }

  graphics->window.display();
}

void timer_benchmark(frame_timer_t *timer) {
  // end current frame
  timer->frame_end = chrono::high_resolution_clock::now();
  timer->current_frame++;

  // calculate FPS
  timer->fps = float(1e9) / float(
    chrono::duration_cast<chrono::nanoseconds>
      (timer->frame_end - timer->frame_start).count());

  // begin next frame
  timer->frame_start = chrono::high_resolution_clock::now();
}

int main(void) {
  cout << "Launch Fullscreen:" <<
    RESOLUTION.width << "x" << RESOLUTION.height;

  frame_timer_t timer;
  state_t state;
  graphics_t graphics;

  state.show_debug = true;
  initialize(&graphics, &state);
  begin_sort(&state, BUBBLE);

  // begins the timer for the current frame
  // timer.frame_start = chrono::high_resolution_clock::now();
  while (graphics.window.isOpen()) {
    poll_events(&state, &graphics);

    // if statement for rate-limiting
    if (!(state.rate_limiting &&
          timer.current_frame % state.rate_limit == 0))
      perform_sort(&state);

    update_screen(&graphics, &state, &timer);
    timer_benchmark(&timer);
  }
}
