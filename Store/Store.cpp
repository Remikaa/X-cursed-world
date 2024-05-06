#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <time.h>

// 3 is the number of Perks
#define NUMBER_OF_PERKS 3

using namespace std;
using namespace sf;

// All will be related tp Perks
// Structs to hold button texts and Perks
struct perk
{
	Sprite action;
	Sprite upgradeButton;
	Text upgradeText;
	Text price;
	FloatRect bounds;
	FloatRect upgradeBounds;
}perks[NUMBER_OF_PERKS];

// Array to check how many times i click on ubgrade
int upgradeCheck[3] = {};

// Function of store
void store(int, RenderWindow& window);

