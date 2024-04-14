#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <time.h>

// 5 is the number of power ups
#define NUMBER_OF_POWER_UPS 5

using namespace std;
using namespace sf;

// All will be related tp power ups
// Structs to hold button texts and power-ups
struct ButtonText
{
	Text buyText;
	Text selectText;
	Text selectedText;
	FloatRect buyBounds;
	FloatRect selectBounds;
	FloatRect selectedBounds;
};
struct PowerUp
{
	Sprite action;
	Sprite cancelButton;
	FloatRect powerBounds;
	FloatRect cancelBounds;
	ButtonText powerText;
}
powerUps[NUMBER_OF_POWER_UPS];

// Array to check if i click select
int selectPowerUps[NUMBER_OF_POWER_UPS] = {};
// Array to check if i click buy
int buyPowerUps[NUMBER_OF_POWER_UPS] = {};

// Function of store
void store(int);
// Function to check how many power-ups are selected (at maximum 2)
int checkSelectPowerUps(int[NUMBER_OF_POWER_UPS]);

int main()
{
	int coins = 500; // it will be got from score function (this value for practice)
	store(coins);
	return 0;
}

void store(int coins)
{
	RenderWindow storeWindow(VideoMode(1920, 1080), "Store", Style::Fullscreen);

	// Load font of store
	Font storeFont;
	if (!storeFont.loadFromFile("./Fonts/Pixelated.ttf"))
	{
		cerr << "Error loading font file" << endl;
	}

	// Text of storeBanner
	Text textBanner;
	textBanner.setFont(storeFont);
	textBanner.setString("Store");
	textBanner.setFillColor(Color::White);
	textBanner.setCharacterSize(50);
	textBanner.setPosition(903, 45);

	// Load primary textures
	Texture background, storeBadge, powerBoard, charBoard, charChain, powerChain;
	if (!background.loadFromFile("./Textures/background.png") ||
		!storeBadge.loadFromFile("./Textures/storebanner.png") ||
		!powerBoard.loadFromFile("./Textures/boardofpowerups.png") ||
		!charBoard.loadFromFile("./Textures/boardofcharacter.png") ||
		!charChain.loadFromFile("./Textures/charchain.png") ||
		!powerChain.loadFromFile("./Textures/chain.png"))
	{
		cerr << "Error loading primary textures files" << endl;
	}

	// Create primary sprites
	Sprite storeBackground(background), storeBanner(storeBadge), boardOfPowers(powerBoard),
		boardOfCharacter(charBoard), chainOfCharacter(charChain), chainOne(powerChain), chainTwo(powerChain);

	// Set positions for primary sprites
	storeBanner.setPosition(780, 25);
	boardOfPowers.setPosition(1500, 220);
	boardOfCharacter.setPosition(50, 385);
	chainOfCharacter.setPosition(0, 300);
	chainOne.setPosition(1540, 0);
	chainTwo.setPosition(1830, 0);

	// Load button textures
	Texture buy, select, selected, cancel;
	if (!buy.loadFromFile("./textures/buy.png") ||
		!select.loadFromFile("./textures/select.png") ||
		!selected.loadFromFile("./textures/selected.png") ||
		!cancel.loadFromFile("./textures/cancel.png"))
	{
		cerr << "Error loading button texture files" << endl;
	}

	// Create button sprites
	Sprite buyButton(buy), selectButton(select), selectedButton(selected);

	// Set positions for button sprites
	buyButton.setPosition(-1000, -1000);
	selectButton.setPosition(-1000, -1000);
	selectedButton.setPosition(-1000, -1000);

	// textures of power ups
	String power_up_textures[NUMBER_OF_POWER_UPS] = { "./Textures/power1.png","./Textures/power2.png","./Textures/power3.png",
		"./Textures/power4.png","./Textures/power5.png" };
	Texture perk[NUMBER_OF_POWER_UPS];

	for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
	{
		perk[i].loadFromFile(power_up_textures[i]);
		powerUps[i].action.setTexture(perk[i]);
		powerUps[i].cancelButton.setTexture(cancel);
		powerUps[i].cancelButton.setPosition(-1000, -1000);

		// Initialize button texts
		powerUps[i].powerText.buyText.setFont(storeFont);
		powerUps[i].powerText.buyText.setString("Buy");
		powerUps[i].powerText.buyText.setFillColor(Color::White);
		powerUps[i].powerText.buyText.setPosition(-1000, -1000);
		powerUps[i].powerText.buyText.setCharacterSize(50);

		powerUps[i].powerText.selectText.setFont(storeFont);
		powerUps[i].powerText.selectText.setString("Select");
		powerUps[i].powerText.selectText.setFillColor(Color::White);
		powerUps[i].powerText.selectText.setPosition(-1000, -1000);
		powerUps[i].powerText.selectText.setCharacterSize(50);

		powerUps[i].powerText.selectedText.setFont(storeFont);
		powerUps[i].powerText.selectedText.setString("Selected");
		powerUps[i].powerText.selectedText.setFillColor(Color::White);
		powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
		powerUps[i].powerText.selectedText.setCharacterSize(45);

		// position of powerups
		switch (i)
		{
		case 0:
			powerUps[0].action.setPosition(1570, 250);
			break;
		case 1:
			powerUps[1].action.setPosition(1570, 343);
			break;
		case 2:
			powerUps[2].action.setPosition(1570, 433);
			break;
		case 3:
			powerUps[3].action.setPosition(1680, 260);
			break;
		case 4:
			powerUps[4].action.setPosition(1680, 355);
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
			for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
			{
				powerUps[i].powerBounds = powerUps[i].action.getGlobalBounds();
				powerUps[i].cancelBounds = powerUps[i].cancelButton.getGlobalBounds();
				powerUps[i].powerText.buyBounds = powerUps[i].powerText.buyText.getGlobalBounds();
				powerUps[i].powerText.selectBounds = powerUps[i].powerText.selectText.getGlobalBounds();
				powerUps[i].powerText.selectedBounds = powerUps[i].powerText.selectedText.getGlobalBounds();
			}

			if (Mouse::isButtonPressed(Mouse::Left))
			{
				// hit test
				// power one
				if (powerUps[0].powerBounds.contains(mouse))
				{
					// if i selected this power up
					if (selectPowerUps[0] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 0)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectedButton.setPosition(1515, 800);
						powerUps[0].powerText.selectedText.setPosition(1568, 830);
						powerUps[0].cancelButton.setPosition(1820, 830);
					}
					// if i buy this power up
					else if (buyPowerUps[0] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 0)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectButton.setPosition(1515, 800);
						powerUps[0].powerText.selectText.setPosition(1585, 825);
					}
					// if i click on it for frist time
					else if (selectPowerUps[0] == 0 && buyPowerUps[0] == 0)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 0)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						buyButton.setPosition(1550, 800);
						powerUps[0].powerText.buyText.setPosition(1575, 825);
					}
				}
				// if i click on buy button 
				if (powerUps[0].powerText.buyBounds.contains(mouse))
				{
					if (coins >= 100)
					{
						coins -= 100;
						buyPowerUps[0] = 1;
						buyButton.setPosition(-1000, -1000);
						powerUps[0].powerText.buyText.setPosition(-1000, -1000);
						selectButton.setPosition(1515, 800);
						powerUps[0].powerText.selectText.setPosition(1585, 825);
					}
				}
				// if i click on select button 
				if (powerUps[0].powerText.selectBounds.contains(mouse))
				{
					int flag = checkSelectPowerUps(selectPowerUps);
					if (flag <= 1)
					{
						selectPowerUps[0] = 1;
						selectButton.setPosition(-1000, -1000);
						powerUps[0].powerText.selectText.setPosition(-1000, -1000);
						selectedButton.setPosition(1515, 800);
						powerUps[0].powerText.selectedText.setPosition(1568, 830);
						powerUps[0].cancelButton.setPosition(1820, 830);
					}
				}
				// if i click on canecl button
				if (powerUps[0].cancelBounds.contains(mouse))
				{
					selectPowerUps[0] = 0;
					selectedButton.setPosition(-1000, -1000);
					powerUps[0].powerText.selectedText.setPosition(-1000, -1000);
					powerUps[0].cancelButton.setPosition(-1000, -1000);
					selectButton.setPosition(1515, 800);
					powerUps[0].powerText.selectText.setPosition(1585, 825);
				}

				// power two
				if (powerUps[1].powerBounds.contains(mouse))
				{
					// if i selected this power up
					if (selectPowerUps[1] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 1)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectedButton.setPosition(1515, 800);
						powerUps[1].powerText.selectedText.setPosition(1568, 830);
						powerUps[1].cancelButton.setPosition(1820, 830);
					}
					// if i buy this power up
					else if (buyPowerUps[1] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 1)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectButton.setPosition(1515, 800);
						powerUps[1].powerText.selectText.setPosition(1585, 825);
					}
					// if i click on it for frist time
					else if (selectPowerUps[1] == 0 && buyPowerUps[1] == 0)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 1)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						buyButton.setPosition(1550, 800);
						powerUps[1].powerText.buyText.setPosition(1575, 825);
					}
				}
				// if i click on buy button 
				if (powerUps[1].powerText.buyBounds.contains(mouse))
				{
					if (coins >= 100)
					{
						coins -= 100;
						buyPowerUps[1] = 1;
						buyButton.setPosition(-1000, -1000);
						powerUps[1].powerText.buyText.setPosition(-1000, -1000);
						selectButton.setPosition(1515, 800);
						powerUps[1].powerText.selectText.setPosition(1585, 825);
					}
				}
				// if i click on select button
				if (powerUps[1].powerText.selectBounds.contains(mouse))
				{
					int flag = checkSelectPowerUps(selectPowerUps);
					if (flag <= 1)
					{
						selectPowerUps[1] = 1;
						selectButton.setPosition(-1000, -1000);
						powerUps[1].powerText.selectText.setPosition(-1000, -1000);
						selectedButton.setPosition(1515, 800);
						powerUps[1].powerText.selectedText.setPosition(1568, 830);
						powerUps[1].cancelButton.setPosition(1820, 830);
					}
				}
				// if i click on canecl button
				if (powerUps[1].cancelBounds.contains(mouse))
				{
					selectPowerUps[1] = 0;
					selectedButton.setPosition(-1000, -1000);
					powerUps[1].powerText.selectedText.setPosition(-1000, -1000);
					powerUps[1].cancelButton.setPosition(-1000, -1000);
					selectButton.setPosition(1515, 800);
					powerUps[1].powerText.selectText.setPosition(1585, 825);
				}

				// power three
				if (powerUps[2].powerBounds.contains(mouse))
				{
					// if i selected this power up
					if (selectPowerUps[2] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 2)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectedButton.setPosition(1515, 800);
						powerUps[2].powerText.selectedText.setPosition(1568, 830);
						powerUps[2].cancelButton.setPosition(1820, 830);
					}
					// if i buy this power up
					else if (buyPowerUps[2] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 2)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectButton.setPosition(1515, 800);
						powerUps[2].powerText.selectText.setPosition(1585, 825);
					}
					// if i click on it for frist time
					else if (selectPowerUps[2] == 0 && buyPowerUps[2] == 0)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 2)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						buyButton.setPosition(1550, 800);
						powerUps[2].powerText.buyText.setPosition(1575, 825);
					}
				}
				// if i click on buy button 
				if (powerUps[2].powerText.buyBounds.contains(mouse))
				{
					if (coins >= 100)
					{
						coins -= 100;
						buyPowerUps[2] = 1;
						buyButton.setPosition(-1000, -1000);
						powerUps[2].powerText.buyText.setPosition(-1000, -1000);
						selectButton.setPosition(1515, 800);
						powerUps[2].powerText.selectText.setPosition(1585, 825);
					}
				}
				// if i click on select button
				if (powerUps[2].powerText.selectBounds.contains(mouse))
				{
					int flag = checkSelectPowerUps(selectPowerUps);
					if (flag <= 1)
					{
						selectPowerUps[2] = 1;
						selectButton.setPosition(-1000, -1000);
						powerUps[2].powerText.selectText.setPosition(-1000, -1000);
						selectedButton.setPosition(1515, 800);
						powerUps[2].powerText.selectedText.setPosition(1568, 830);
						powerUps[2].cancelButton.setPosition(1820, 830);
					}
				}
				// if i click on canecl button
				if (powerUps[2].cancelBounds.contains(mouse))
				{
					selectPowerUps[2] = 0;
					selectedButton.setPosition(-1000, -1000);
					powerUps[2].powerText.selectedText.setPosition(-1000, -1000);
					powerUps[2].cancelButton.setPosition(-1000, -1000);
					selectButton.setPosition(1515, 800);
					powerUps[2].powerText.selectText.setPosition(1585, 825);
				}

				// power four
				if (powerUps[3].powerBounds.contains(mouse))
				{
					// if i selected this power up
					if (selectPowerUps[3] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 3)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectedButton.setPosition(1515, 800);
						powerUps[3].powerText.selectedText.setPosition(1568, 830);
						powerUps[3].cancelButton.setPosition(1820, 830);
					}
					// if i buy this power up
					else if (buyPowerUps[3] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 3)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectButton.setPosition(1515, 800);
						powerUps[3].powerText.selectText.setPosition(1585, 825);
					}
					// if i click on it for frist time
					else if (selectPowerUps[3] == 0 && buyPowerUps[3] == 0)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 3)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						buyButton.setPosition(1550, 800);
						powerUps[3].powerText.buyText.setPosition(1575, 825);
					}
				}
				// if i click on buy button 
				if (powerUps[3].powerText.buyBounds.contains(mouse))
				{
					if (coins >= 300)
					{
						coins -= 300;
						buyPowerUps[3] = 1;
						buyButton.setPosition(-1000, -1000);
						powerUps[3].powerText.buyText.setPosition(-1000, -1000);
						selectButton.setPosition(1515, 800);
						powerUps[3].powerText.selectText.setPosition(1585, 825);
					}
				}
				// if i click on select button
				if (powerUps[3].powerText.selectBounds.contains(mouse))
				{
					int flag = checkSelectPowerUps(selectPowerUps);
					if (flag <= 1)
					{
						selectPowerUps[3] = 1;
						selectButton.setPosition(-1000, -1000);
						powerUps[3].powerText.selectText.setPosition(-1000, -1000);
						selectedButton.setPosition(1515, 800);
						powerUps[3].powerText.selectedText.setPosition(1568, 830);
						powerUps[3].cancelButton.setPosition(1820, 830);
					}
				}
				// if i click on canecl button
				if (powerUps[3].cancelBounds.contains(mouse))
				{
					selectPowerUps[3] = 0;
					selectedButton.setPosition(-1000, -1000);
					powerUps[3].powerText.selectedText.setPosition(-1000, -1000);
					powerUps[3].cancelButton.setPosition(-1000, -1000);
					selectButton.setPosition(1515, 800);
					powerUps[3].powerText.selectText.setPosition(1585, 825);
				}

				// power five
				if (powerUps[4].powerBounds.contains(mouse))
				{
					// if i selected this power up
					if (selectPowerUps[4] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 4)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectedButton.setPosition(1515, 800);
						powerUps[4].powerText.selectedText.setPosition(1568, 830);
						powerUps[4].cancelButton.setPosition(1820, 830);
					}
					// if i buy this power up
					else if (buyPowerUps[4] == 1)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 4)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						selectButton.setPosition(1515, 800);
						powerUps[4].powerText.selectText.setPosition(1585, 825);
					}
					// if i click on it for frist time
					else if (selectPowerUps[4] == 0 && buyPowerUps[4] == 0)
					{
						for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
						{
							if (i == 4)
								continue;
							buyButton.setPosition(-1000, -1000);
							selectButton.setPosition(-1000, -1000);
							selectedButton.setPosition(-1000, -1000);
							powerUps[i].cancelButton.setPosition(-1000, -1000);
							powerUps[i].powerText.buyText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectText.setPosition(-1000, -1000);
							powerUps[i].powerText.selectedText.setPosition(-1000, -1000);
						}
						buyButton.setPosition(1550, 800);
						powerUps[4].powerText.buyText.setPosition(1575, 825);
					}
				}
				// if i click on buy button 
				if (powerUps[4].powerText.buyBounds.contains(mouse))
				{
					if (coins >= 300)
					{
						coins -= 300;
						buyPowerUps[4] = 1;
						buyButton.setPosition(-1000, -1000);
						powerUps[4].powerText.buyText.setPosition(-1000, -1000);
						selectButton.setPosition(1515, 800);
						powerUps[4].powerText.selectText.setPosition(1585, 825);
					}
				}
				// if i click on select button
				if (powerUps[4].powerText.selectBounds.contains(mouse))
				{
					int flag = checkSelectPowerUps(selectPowerUps);
					if (flag <= 1)
					{
						selectPowerUps[4] = 1;
						selectButton.setPosition(-1000, -1000);
						powerUps[4].powerText.selectText.setPosition(-1000, -1000);
						selectedButton.setPosition(1515, 800);
						powerUps[4].powerText.selectedText.setPosition(1568, 830);
						powerUps[4].cancelButton.setPosition(1820, 830);
					}
				}
				// if i click on canecl button
				if (powerUps[4].cancelBounds.contains(mouse))
				{
					selectPowerUps[4] = 0;
					selectedButton.setPosition(-1000, -1000);
					powerUps[4].powerText.selectedText.setPosition(-1000, -1000);
					powerUps[4].cancelButton.setPosition(-1000, -1000);
					selectButton.setPosition(1515, 800);
					powerUps[4].powerText.selectText.setPosition(1585, 825);
				}
			}
		}
		// Rendering
		storeWindow.clear();
		storeWindow.draw(storeBackground);
		storeWindow.draw(storeBanner);
		storeWindow.draw(textBanner);
		storeWindow.draw(chainOfCharacter);
		storeWindow.draw(boardOfCharacter);
		storeWindow.draw(chainOne);
		storeWindow.draw(chainTwo);
		storeWindow.draw(boardOfPowers);
		storeWindow.draw(buyButton);
		storeWindow.draw(selectButton);
		storeWindow.draw(selectedButton);
		for (int i = 0; i < NUMBER_OF_POWER_UPS; i++)
		{
			storeWindow.draw(powerUps[i].action);
			storeWindow.draw(powerUps[i].cancelButton);
			storeWindow.draw(powerUps[i].powerText.buyText);
			storeWindow.draw(powerUps[i].powerText.selectText);
			storeWindow.draw(powerUps[i].powerText.selectedText);
		}
		storeWindow.display();
	}
}

// Function to check how many power-ups are selected (at maximum 2)
int checkSelectPowerUps(int checkArray[NUMBER_OF_POWER_UPS])
{
	int sum = 0;
	for (int i = 0; i < NUMBER_OF_POWER_UPS; i++) {
		sum += checkArray[i];
	}
	return sum;
}