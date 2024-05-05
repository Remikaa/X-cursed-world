#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
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
void store(int);

//int main()
//{
//	int coins;
//	store(coins);
//	return 0;
//}

void store(int coins)
{
	RenderWindow storeWindow(VideoMode(1920, 1080), "Store", Style::Fullscreen);

	// Load font of store
	Font storeFont;
	if (!storeFont.loadFromFile("Store/Pixelated.ttf"))
	{
		cerr << "Error loading font file" << "/n";
	}

	// Text of storeBanner
	Text textBanner;
	textBanner.setFont(storeFont);
	textBanner.setString("Store");
	textBanner.setFillColor(Color::White);
	textBanner.setCharacterSize(50);
	textBanner.setPosition(885, 45);

	// Load primary textures
	Texture background, storeBadge, powerBoard, infoBoard, infoChain, powerChain, button;
	if (!background.loadFromFile("Store/Textures/background.png") ||
		!storeBadge.loadFromFile("Store/Textures/storebanner.png") ||
		!powerBoard.loadFromFile("Store/Textures/boardofpowerups.png") ||
		!infoBoard.loadFromFile("Store/Textures/boardofinfo.png") ||
		!infoChain.loadFromFile("Store/Textures/infochain.png") ||
		!powerChain.loadFromFile("Store/Textures/chain.png") || 
		!button.loadFromFile("Store/Textures/button.png"))
	{
		cerr << "Error loading primary textures files" << "/n";
	}

	// Create primary sprites
	Sprite storeBackground(background), storeBanner(storeBadge), boardOfPowers(powerBoard),
		boardOfInfo(infoBoard), chainOfInfo(infoChain), chainOne(powerChain), chainTwo(powerChain);

	// Set positions for primary sprites
	storeBanner.setPosition(760, 25);
	boardOfPowers.setPosition(1500, 220);
	boardOfInfo.setPosition(40, 365);
	chainOfInfo.setPosition(0, 280);
	chainOne.setPosition(1540, 0);
	chainTwo.setPosition(1830, 0);

	// Load Perks textures
	Texture heart, resis, sword;
	if (!heart.loadFromFile("Store/Textures/heart1.png") ||
		!resis.loadFromFile("Store/Textures/resis1.png") ||
		!sword.loadFromFile("Store/Textures/sword1.png"))
	{
		cerr << "Error loading Perks textures files" << "/n";
	}

	for (int i = 0; i < NUMBER_OF_PERKS; i++)
	{
		// Create button
		perks[i].upgradeButton.setTexture(button);
		perks[i].upgradeButton.setPosition(-1000, -1000);

		// Create text on button
		perks[i].upgradeText.setFont(storeFont);
		perks[i].upgradeText.setString("Upgrade");
		perks[i].upgradeText.setFillColor(Color::White);
		perks[i].upgradeText.setCharacterSize(45);
		perks[i].upgradeText.setPosition(-1000, -1000);

		// Create price
		perks[i].price.setFont(storeFont);
		perks[i].price.setFillColor(Color::White);
		perks[i].price.setCharacterSize(40);
		perks[i].price.setPosition(-1000, -1000);

		// Create perks sprites
		// position of Perks
		switch (i)
		{
		case 0: 
			perks[i].action.setTexture(sword);
			perks[i].action.setPosition(1570, 250);
			perks[i].price.setString("100");
			break;
		case 1:
			perks[i].action.setTexture(resis);
			perks[i].action.setPosition(1710, 250);
			perks[i].price.setString("60");
			break;
		case 2:
			perks[i].action.setTexture(heart);
			perks[i].action.setPosition(1570, 350);
			perks[i].price.setString("80");
			break;
		}
	}

	while (storeWindow.isOpen())
	{
		// the mouse position on window
		Vector2f mouse = storeWindow.mapPixelToCoords(Mouse::getPosition(storeWindow));

		Event storeEvent;
		while (storeWindow.pollEvent(storeEvent))
		{
			if (storeEvent.type == Event::Closed)
				storeWindow.close();

			if (storeEvent.type == Event::KeyPressed)
			{
				if (storeEvent.key.code == Keyboard::Escape)
				{
					storeWindow.close();
				}
			}

			// retrieve the bounding box
			for (int i = 0; i < NUMBER_OF_PERKS; i++)
			{
				perks[i].bounds = perks[i].action.getGlobalBounds();
				perks[i].upgradeBounds = perks[i].upgradeText.getGlobalBounds();
			}

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// the sword hit test
				if (perks[0].bounds.contains(mouse))
				{
					if (upgradeCheck[0] < 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
						perks[0].upgradeButton.setPosition(1550, 800);
						perks[0].upgradeText.setPosition(1607, 827);
						perks[0].price.setPosition(180, 640);
					}
					else if (upgradeCheck[0] == 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of sword hit test
				if (perks[0].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[0] == 0)
					{
						sword.loadFromFile("Store/Textures/sword2.png");
						perks[0].price.setString("120");
						upgradeCheck[0]++;
					}
					else if (upgradeCheck[0] == 1)
					{
						sword.loadFromFile("Store/Textures/sword3.png");
						perks[0].price.setString("140");
						upgradeCheck[0]++;
					}
					else if (upgradeCheck[0] == 2)
					{
						sword.loadFromFile("Store/Textures/sword4.png");
						perks[0].price.setString("160");
						upgradeCheck[0]++;
					}
				}

				// the resis hit test
				if (perks[1].bounds.contains(mouse))
				{
					if (upgradeCheck[1] < 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
						perks[1].upgradeButton.setPosition(1550, 800);
						perks[1].upgradeText.setPosition(1607, 827);
						perks[1].price.setPosition(180, 640);
					}
					else if (upgradeCheck[1] == 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of resis hit test
				if (perks[1].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[1] == 0)
					{
						resis.loadFromFile("Store/Textures/resis2.png");
						perks[1].price.setString("100");
						upgradeCheck[1]++;
					}
					else if (upgradeCheck[1] == 1)
					{
						resis.loadFromFile("Store/Textures/resis3.png");
						perks[1].price.setString("140");
						upgradeCheck[1]++;
					}
					else if (upgradeCheck[1] == 2)
					{
						resis.loadFromFile("Store/Textures/resis4.png");
						perks[1].price.setString("180");
						upgradeCheck[1]++;
					}
				}

				// the heart hit test
				if (perks[2].bounds.contains(mouse))
				{
					if (upgradeCheck[2] < 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
						perks[2].upgradeButton.setPosition(1550, 800);
						perks[2].upgradeText.setPosition(1607, 827);
						perks[2].price.setPosition(180, 640);
					}
					else if (upgradeCheck[2] == 3)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of heart hit test
				if (perks[2].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[2] == 0)
					{
						heart.loadFromFile("Store/Textures/heart2.png");
						perks[2].price.setString("110");
						upgradeCheck[2]++;
					}
					else if (upgradeCheck[2] == 1)
					{
						heart.loadFromFile("Store/Textures/heart3.png");
						perks[2].price.setString("140");
						upgradeCheck[2]++;
					}
					else if (upgradeCheck[2] == 2)
					{
						heart.loadFromFile("Store/Textures/heart4.png");
						perks[2].price.setString("170");
						upgradeCheck[2]++;
					}
				}
			}
		}
		// Rendering
		storeWindow.clear();
		storeWindow.draw(storeBackground);
		storeWindow.draw(storeBanner);
		storeWindow.draw(textBanner);
		storeWindow.draw(chainOfInfo);
		storeWindow.draw(boardOfInfo);
		storeWindow.draw(chainOne);
		storeWindow.draw(chainTwo);
		storeWindow.draw(boardOfPowers);
		for (int i = 0; i < NUMBER_OF_PERKS; i++)
		{
			storeWindow.draw(perks[i].action);
			storeWindow.draw(perks[i].price);
			storeWindow.draw(perks[i].upgradeButton);
			storeWindow.draw(perks[i].upgradeText);
		}
		storeWindow.display();
	}
}