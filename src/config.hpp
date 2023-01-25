#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdint>

using namespace std;

/*
 * Configuration Variables
 */

constexpr float RECTANGLE_WIDTH = 5.0f;
constexpr float RECTANGLE_OUTLINE_WIDTH = 1.0f;
constexpr uint32_t RECTANGLE_COUNT = 200;

constexpr uint16_t DEFAULT_WINDOW_WIDTH = RECTANGLE_WIDTH * RECTANGLE_COUNT;
constexpr uint16_t DEFAULT_WINDOW_HEIGHT = RECTANGLE_COUNT;

constexpr char* PRESS_START_2P_PATH = "./res/fonts/PressStart2P-Regular.ttf";

/*
 * Global Variables (Fonts, Sprites, Image Locations, etc.)
 */

extern sf::VideoMode video_mode;
extern sf::RenderWindow window;
extern sf::Font PRESS_START_2P;

/*
 * ASSETS
 */

void load_resources();

#endif