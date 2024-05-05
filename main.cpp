#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
using namespace std;
using namespace sf;

int pageNum = 1;
/*************************************
*     page no     *     Function     *
**************************************
*        1        *     Main Menu    *
*        2        *       Play       *
*        3        *       Store      *
*        4        *       Exit       *
*        5        *   Endless Mode   *
*        6        *    Level Mode    *
**************************************/
//test trial 2

int score = 0;

struct mode {
	Text modeElement[2];
	int modeSelected = -1;
	Font font;

	void modeMenuFunc(float width, float height) {
		font.loadFromFile("menu/Pixelated.ttf");  // load the Font file and save it in "font" variable

		string element[2] = { "Arcade Mode", "Levels" };  // array of string 

		for (int i = 0; i < 2; i++)
		{
			modeElement[i].setFont(font);
			modeElement[i].setFillColor(Color::White);
			modeElement[i].setString(element[i]);
			modeElement[i].setCharacterSize(90);
			FloatRect bounds = modeElement[i].getLocalBounds();  // get (x, y) boundraies of the element
			modeElement[i].setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2); // Set origin in the middle of the text
			modeElement[i].setPosition(Vector2f(450 + (i * 1150), (height / 2)));
		}
	}

	int pressed() {
		return modeSelected;
	}


	// draw menu elements
	void draw(RenderWindow& window) {
		for (int i = 0; i < 2; i++)
			window.draw(modeElement[i]);
	}

	void moveLeft() {
		if (modeSelected - 1 >= -1) {
			modeElement[modeSelected].setFillColor(Color::White);
			modeElement[modeSelected].setCharacterSize(90);
			modeSelected--;
			sleep(milliseconds(200));
			if (modeSelected == -1)
				modeSelected = 1;
			modeElement[modeSelected].setFillColor(Color{ 192, 192, 192 });
			modeElement[modeSelected].setCharacterSize(80);
		}
	}

	void moveRight() {
		if (modeSelected + 1 <= 2) {
			modeElement[modeSelected].setFillColor(Color::White);
			modeElement[modeSelected].setCharacterSize(90);
			modeSelected++;
			sleep(milliseconds(200));
			if (modeSelected == 2)
				modeSelected = 0;
			modeElement[modeSelected].setFillColor(Color{ 192, 192, 192 });
			modeElement[modeSelected].setCharacterSize(80);
		}
	}
};

struct mainMenu {
	Text menuElement[5], Title;  // array of text that store the main menu elements' text
	int selected = -1;
	Font font;
	mode modeMenu;


	// Menu function assigns each menu element in the text menuElement array and set features of the text
	void menu(float width, float height) {
		font.loadFromFile("menu/Pixelated.ttf");  // load the Font file and save it in "font" variable

		Title.setFont(font);
		Title.setFillColor(Color::White);
		Title.setCharacterSize(120);
		Title.setPosition(Vector2f(500, 100));
		Title.setString("X: Cursed World");

		string element[3] = { "Play", "Store", "Exit" };  // array of string 

		for (int i = 0; i < 3; i++)
		{
			menuElement[i].setFont(font);
			menuElement[i].setFillColor(Color::White);
			menuElement[i].setString(element[i]);
			menuElement[i].setCharacterSize(90);
			FloatRect bounds = menuElement[i].getLocalBounds();  // get (x, y) boundraies of the element
			menuElement[i].setOrigin(bounds.left + bounds.width / 2, bounds.top + bounds.height / 2); // Set origin 
			menuElement[i].setPosition(Vector2f((width / 2), (height / 4) + (i * 120) + 120));
		}
	}

	int pressed() {
		return selected;
	}


	// draw menu elements
	void draw(RenderWindow& window) {
		window.draw(Title);

		for (int i = 0; i < 3; i++)
			window.draw(menuElement[i]);
	}

	void moveUp() {
		if (selected - 1 >= -1) {
			menuElement[selected].setFillColor(Color::White);
			menuElement[selected].setCharacterSize(90);
			selected--;
			sleep(milliseconds(200));
			if (selected == -1)
				selected = 2;
			menuElement[selected].setFillColor(Color{ 192, 192, 192 });
			menuElement[selected].setCharacterSize(80);
		}
	}

	void moveDown() {
		if (selected + 1 <= 3) {
			menuElement[selected].setFillColor(Color::White);
			menuElement[selected].setCharacterSize(90);
			selected++;
			sleep(milliseconds(200));
			if (selected == 3)
				selected = 0;
			menuElement[selected].setFillColor(Color{ 192, 192, 192 });
			menuElement[selected].setCharacterSize(80);
		}
	}

};

int gameMode(RenderWindow& window, mainMenu& menu, Sound& clickSound);
// Function of store
void store(int);

void movements();

void arcadeMode(RenderWindow& window);

void levelOne(RenderWindow& window);

void levelTwo(RenderWindow& window);

int main() {

	RenderWindow window(VideoMode(1920, 1080), "X: Cursed World!");
	window.setFramerateLimit(60);
	mainMenu menu;
	menu.menu(1920, 1080);

	Texture mainMenuPic;
	mainMenuPic.loadFromFile("menu/menuPic.jpg");
	Sprite menuPic;
	menuPic.setTexture(mainMenuPic);

	SoundBuffer clickBuffer;
	clickBuffer.loadFromFile("menu/ButtonClick.wav");
	Sound clickSound;
	clickSound.setBuffer(clickBuffer);

	while (true) {
		if (pageNum == 1) {
			while (window.isOpen()) {
				Event event;

				while (window.pollEvent(event)) {
					if (event.type == Event::Closed) {
						window.close();
						break;
					}

					if (event.type == Event::KeyPressed) {
						if ((event.key.code == Keyboard::W) || event.key.code == Keyboard::Up)
							menu.moveUp();

						if ((event.key.code == Keyboard::S) || event.key.code == Keyboard::Down)
							menu.moveDown();

						if (event.key.code == Keyboard::Enter) {
							clickSound.play();
							if (menu.pressed() == 0)
								pageNum = 2;
							if (menu.pressed() == 1)
								pageNum = 3;
							if (menu.pressed() == 2)
								pageNum = 4;

						}
					}
				}

				window.clear();

				window.draw(menuPic);
				menu.draw(window);

				window.display();

				if (pageNum != 1)
					break;

			}
		}
		if (pageNum == 2) {
			int modeSelected = gameMode(window, menu, clickSound);
			if (modeSelected == 5) {
				arcadeMode(window);
				cout << "Arcade mode!" << endl;
			}
			else if (modeSelected == 6) {
				levelOne(window);
				cout << "Levels Mode!" << endl;
			}
			else
			{
				cout << "Back" << endl;
			}
		}
		if (pageNum == 3) {
			//store(coins);
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				clickSound.play();
				pageNum = 1;
				menu.menuElement[1].setFillColor(Color::White);
				menu.menuElement[1].setCharacterSize(90);
			}
		}
		if (pageNum == 4) {
			window.close();
			break;
		}
	}

	return 0;
}

int gameMode(RenderWindow& window, mainMenu& menu, Sound& clickSound) {

	menu.modeMenu.modeMenuFunc(1920, 1080);

	Texture modeMenuPic;
	modeMenuPic.loadFromFile("menu/menuPic.jpg");
	Sprite modeMenuSprite;
	modeMenuSprite.setTexture(modeMenuPic);


	while (true) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
				break;
			}
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Right)
					menu.modeMenu.moveRight();
				if (event.key.code == Keyboard::Left)
					menu.modeMenu.moveLeft();
				if (event.key.code == Keyboard::Enter) {
					clickSound.play();
					if (menu.modeMenu.pressed() == 0)
						return 5; // Return 5 for Arcade/Endless Mode
					if (menu.modeMenu.pressed() == 1)
						return 6; // Return 6 for Level Mode
				}
				if (event.key.code == Keyboard::Escape) {  // return back to the main menu page
					clickSound.play();
					pageNum = 1;
					menu.menuElement[0].setFillColor(Color::White);
					menu.menuElement[0].setCharacterSize(90);
					return pageNum;
				}
			}

			// Update
			window.clear();
			window.draw(modeMenuSprite);
			menu.modeMenu.draw(window);
			window.display();
		}
	}
}