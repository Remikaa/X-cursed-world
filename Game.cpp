#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
using namespace std;
using namespace sf;

//void store(int);
//int ground = 600;
//vector<RectangleShape> currentTiles;
//
//struct pauseMenu
//{
//	Font pauseFont;
//	Text Resume;
//	Text Quit;
//	Text Store;
//
//	bool paused = false;
//	int coins = 500;
//
//	Text pauseElements[3];
//	string elements[3] = { "Resume", "Store", "Quit" };
//	int selected = -1;
//	
//	
//		void PauseMenufunc(float width, float height)
//		{
//
//			pauseFont.loadFromFile("menu/Pixelated.ttf");
//
//
//
//			if (!pauseFont.loadFromFile("menu/Pixelated.ttf"))
//			{
//				cerr << "Font did not load" << endl;
//				// Handle error loading font
//			}
//
//			for (int i = 0; i < 3; i++)
//			{
//				FloatRect boundary = pauseElements[i].getLocalBounds();
//				pauseElements[i].setFont(pauseFont);
//				pauseElements[i].setString(elements[i]);
//				pauseElements[i].setCharacterSize(90);
//				pauseElements[i].setFillColor(Color::White);
//				pauseElements[i].setOrigin(boundary.left + boundary.width / 2, boundary.top + boundary.height / 2);
//				pauseElements[i].setPosition(width / 2 - 120 , (height / 4) + (i * 150));
//			}
//
//		}
//
//		void show(RenderWindow & window, int& pageNum)
//		{
//
//
//			while (window.isOpen())
//			{
//				Event event;
//				while (window.pollEvent(event))
//				{
//					if (event.type == Event::Closed)
//					{
//						window.close();
//					}
//
//					if (event.type == Event::KeyPressed)
//					{
//						if (event.key.code == Keyboard::Up)
//						{
//							moveUp();
//						}
//
//						if (event.key.code == Keyboard::Down)
//						{
//							moveDown();
//						}
//
//						if (event.key.code == Keyboard::Enter)
//						{
//							if (selected == 0)
//							{
//								paused = false;
//							}
//
//							if (selected == 1)
//							{
//								store(coins); //needs editing
//							}
//
//							if (selected == 2) //needs editing
//							{
//								pageNum = 1;
//
//							}
//						}
//					}
//
//				}
//
//				window.clear();
//				for (int i = 0; i < 3; i++)
//				{
//					window.draw(pauseElements[i]);
//				}
//				window.display();
//			}
//		}
//
//
//		int pressed() {
//			return selected;
//		}
//
//
//		void moveUp() {
//			if (selected - 1 >= -1) {
//				pauseElements[selected].setFillColor(Color::White);
//				pauseElements[selected].setCharacterSize(90);
//				selected--;
//				sleep(milliseconds(200));
//				if (selected == -1)
//					selected = 2;
//				pauseElements[selected].setFillColor(Color{ 192, 192, 192 });
//				pauseElements[selected].setCharacterSize(80);
//			}
//		}
//
//		void moveDown() {
//			if (selected + 1 <= 3) {
//				pauseElements[selected].setFillColor(Color::White);
//				pauseElements[selected].setCharacterSize(90);
//				selected++;
//				sleep(milliseconds(200));
//				if (selected == 3)
//					selected = 0;
//				pauseElements[selected].setFillColor(Color{ 192, 192, 192 });
//				pauseElements[selected].setCharacterSize(80);
//			}
//		}
//	
//
//};
//
//// main character code (player character)
//
//struct character {
//	Sprite sprite;  // The sprite representing the player character
//	float currentFrame;  // The current frame of animation.
//	float moveX, moveY;  // Movements on x and y direction
//	bool onGround;  // indicate if the player is on the ground
//	FloatRect rect;  // The bounding rectangle of the player
//	int lastKeyPressed;  // The last key pressed by the player
//	int noOfAttacks;   // no of attacks dealt by the player to the enemy
//	string state;  // The current state of the player
//	int health;  // Health of the player
//	Texture stateTexture[15];  // Array of textures for different states
//
//	// Function to load textures for different states
//	void loadTextures() {
//		string stateElementsTX[15] = { "knight/_Idle.png", "knight/_Run.png", "knight/_Dash.png", "knight/_Jump.png", "knight/_Roll.png", "knight/_Hit.png", 
//										"knight/_SlideAll.png", "knight/_Attack.png", "knight/_Attack2.png","knight/_AttackCombo.png", "knight/_CrouchAll.png", 
//										"knight/_CrouchAttack.png", "knight/_CrouchWalk.png", "knight/_Fall.png", "knight/_Death.png" };  //Array of string for 
//
//		for (int i = 0; i < 15; i++)
//			stateTexture[i].loadFromFile(stateElementsTX[i]);
//	}
//
//	// Function to assign sprite and initialize properties
//	void assignSprite() {
//		sprite.setScale(3.7, 3.7);  //Set sprite scale
//		moveX = 0;  // Character starts not moving in the X-axis
//		moveY = 0;  // Character starts not moving in the Y-axis
//		currentFrame = 0;
//		noOfAttacks = 0;
//		lastKeyPressed = 1;
//		state = "Idle";  // Initialize state to idle
//		health = 100;  // Initialize health to 100
//		loadTextures();  // Loading the textures
//		updateTexture();  // Update the texture of the sprite to idle
//	}
//
//	// Function to update player's position and animation based on state
//	void update(float time) {
//		rect.left += moveX * time;  // Movement of the character on x-axis
//
//		if (!onGround)
//			moveY += (0.00085 * time);  // Gravity 
//
//		rect.top += moveY * time;  // Movement of the character on y-axis
//		onGround = false;  // Intialize onGround as false in case the character postion off ground 
//
//		if (rect.top > ground) {  // when character exceeds the ground
//			rect.top = ground;
//			moveY = 0;
//			onGround = true;
//		}
//
//		currentFrame += 0.005 * time;
//
//		// Update animation based on state
//		if (state == "Idle") {
//			if (currentFrame > 10) currentFrame -= 10;  // This condition changes according to number of changes
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));  // To right
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));  // To left
//		}
//		else if (state == "Walk") {
//			if (currentFrame > 10) currentFrame -= 10;
//			if (moveX > 0)  sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));  // movement to right
//			if (moveX < 0)  sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80)); // movement to left
//		}
//		else if (state == "Dash") {
//			if (currentFrame > 2) currentFrame -= 2;
//			if (moveX > 0) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			if (moveX < 0) sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Jump") {
//			if (currentFrame > 3) currentFrame -= 3;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Roll") {
//			if (currentFrame > 12) currentFrame -= 12;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Hit") {
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(0, 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120, 0, -120, 80));
//		}
//		else if (state == "Slide") {
//			if (currentFrame > 4) {
//				if (lastKeyPressed == 1)  sprite.setTextureRect(IntRect(120 * 3, 0, 120, 80));
//				else  sprite.setTextureRect(IntRect(120 * 3 + 120, 0, -120, 80));
//			}
//			else if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80)) ;
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Attack") {
//			if (currentFrame > 4) currentFrame -= 4;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Attack2") {
//			if (currentFrame > 6) currentFrame -= 6;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "AttackCombo") {
//			if (currentFrame > 10) currentFrame -= 10;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Crouch") {
//			if (currentFrame > 3) {
//				if (lastKeyPressed == 1)  sprite.setTextureRect(IntRect(120 * 2, 0, 120, 80));
//				else  sprite.setTextureRect(IntRect(120 * 2 + 120, 0, -120, 80));
//			}
//			else if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "CrouchAttack") {
//			if (currentFrame > 4) currentFrame -= 4;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "CrouchWalk") {
//			if (currentFrame > 8) currentFrame -= 8;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Fall") {
//			if (currentFrame > 3) currentFrame -= 3;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//		else if (state == "Death") {
//			if (currentFrame > 10) currentFrame -= 10;
//			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
//			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
//		}
//
//		sprite.setPosition(rect.left, rect.top);
//		moveX = 0;
//	}
//
//	// Function to update the texture based on current state
//	void updateTexture() {
//		string stateElement[15] = { "Idle", "Walk", "Dash", "Jump", "Roll", "Hit", "Slide", "Attack", "Attack2", "AttackCombo", "Crouch", "CrouchAttack", "CrouchWalk", "Fall", "Death"};
//
//		for (int i = 0; i < 15; i++)
//		{
//			if (state == stateElement[i])
//				sprite.setTexture(stateTexture[i]);
//		}
//	}
//
//	bool isAlive() {
//		return health;
//	}
//} knight;
//
//void movements() {
//	if (Keyboard::isKeyPressed(Keyboard::D)) {
//		knight.state = "Walk";
//		knight.updateTexture();
//		knight.moveX = 0.2;
//		knight.lastKeyPressed = 1;  // Last Key pressed is Right
//	}
//	if (Keyboard::isKeyPressed(Keyboard::A)) {
//		knight.state = "Walk";
//		knight.updateTexture();
//		knight.moveX = -0.2;
//		knight.lastKeyPressed = 2;  // Last Key pressed is Left
//	}
//	if (Keyboard::isKeyPressed(Keyboard::Tab)) {
//		knight.state = "Dash";
//		knight.updateTexture();
//		if (knight.lastKeyPressed == 1) knight.moveX = 0.4;
//		else knight.moveX = -0.4;
//	}
//	if (Keyboard::isKeyPressed(Keyboard::Space)) {
//		if (knight.onGround == true) {
//			knight.moveY = -0.6;
//			knight.state = "Jump";
//			knight.onGround = false;
//		}
//		knight.updateTexture();
//	}
//	if (Keyboard::isKeyPressed(Keyboard::R)) {
//		knight.state = "Roll";
//		knight.updateTexture();
//		if (knight.lastKeyPressed == 1) knight.moveX = 0.2;
//		else knight.moveX = -0.2;
//	}
//	if (Keyboard::isKeyPressed(Keyboard::LControl)) {
//		knight.state = "Slide";
//		knight.updateTexture();
//		if (knight.lastKeyPressed == 1) knight.moveX = 0.2;
//		else knight.moveX = -0.2;
//	}
//	if (Keyboard::isKeyPressed(Keyboard::LShift)) {
//		knight.state = "Crouch";
//		knight.updateTexture();
//	}
//	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::E)) {
//		knight.state = "CrouchAttack";
//		if (knight.lastKeyPressed == 1) knight.moveX = 0.05;
//		else knight.moveX = -0.05;
//		knight.updateTexture();
//	}
//	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::D)) {
//		knight.state = "CrouchWalk";
//		knight.updateTexture();
//		knight.moveX = 0.1;
//		knight.lastKeyPressed = 1;  // Last Key pressed is Right
//	}
//	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::A)) {
//		knight.state = "CrouchWalk";
//		knight.updateTexture();
//		knight.moveX = -0.1;
//		knight.lastKeyPressed = 2;  // Last Key pressed is Right
//	}
//	if (Keyboard::isKeyPressed(Keyboard::E)) { 
//		knight.noOfAttacks++;
//		if (knight.noOfAttacks % 2 == 1) { knight.state = "Attack"; knight.noOfAttacks++;
//		}
//		else if (knight.noOfAttacks % 4 == 0)  knight.state = "AttackCombo";
//		else {
//			knight.state = "Attack2"; 
//			knight.noOfAttacks++;
//		}
//		knight.updateTexture();
//	}
//	if (knight.moveX == 0 && knight.moveY == 0 && !(Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LShift))) {
//		knight.state = "Idle";
//		knight.updateTexture();
//	}
//
//	
//} //wait
//
///* enemy characters code 
//Enemies will be 1 Bosses and 2 small different enemy guards for level 1
// Enemies for  other levels will be determined later */
//struct SecEnemy
//{
//	Sprite sprite;  // The sprite representing the player character
//	float currentFrame;  // The current frame of animation.
//	float moveX, moveY;  // Movements on x and y direction
//	bool onGround;  // indicate if the player is on the ground
//	FloatRect rect;  // The bounding rectangle of the enemy
//	FloatRect killzone; // the range in which the enemy detects
//	bool inkillzone; // true means player in enemy killzone, false means not in enemy kill zone
//	int noOfAttackstaken;   // no of attacks dealt by the player to the enemy
//	string state;  // The current state of the enemy
//	int health;  // Health of the enemy
//	Texture* stateTexture;  // Dynamic Array of textures for different states
//	float speed;
//	float attacking_factor = 0.6;
//	// Function to load textures for different states
//	void loadTextures(int size, string stateElementsTX[]) {
//
//		for (int i = 0; i < size; i++)
//			stateTexture[i].loadFromFile(stateElementsTX[i]);
//	}
//	bool is_in_killzone(FloatRect& killzone, Vector2f& characterPosition) {
//		// Check if the character's position is within the boundaries of the killzone
//		return killzone.contains(characterPosition);
//	}
//	// Function to assign sprite and initialize properties
//	void assignSprite(float scX, float scY, int moveNX, int moveNY, int Health, int size, string stateElementsTX[], string stateElement[]) {
//		sprite.setScale(scX, scY); //Set sprite scale
//		moveX = moveNX;
//		moveY = moveNY;
//		currentFrame = 0;
//		noOfAttackstaken = 0;
//		//lastKeyPressed = 1;
//		state = "Idle";  // Initialize state to idle
//		health = Health;  // Initialize health to 100
//		loadTextures(size, stateElementsTX);  // Loading the textures
//		updateTexture(size, stateElement);  // Update the texture of the sprite to idle
//	}
//	// Function to update the texture based on current state
//	void updateTexture(int size, string stateElement[])
//	{
//		for (int i = 0; i < size; i++)
//		{
//			if (state == stateElement[i])
//				sprite.setTexture(stateTexture[i]);
//		}
//	}
//
//};
//
//// level 1 map code 
//struct LevelOne {
//	int currentScene = 0;
//	int noOFEnemies = 0;
//	Sprite backgroundSprite;
//	Texture levelTextures[6];
//
//
//	void loadTextures() {
//		string texturesArr[6] = { "level-maps/level1/p1.png", "level-maps/level1/p2.png","level-maps/level1/p3.png", "level-maps/level1/p4.png", "level-maps/level1/p5.png", "level-maps/level1/p5.2.png" };
//
//		for (int i = 0; i < 6; i++) {
//			levelTextures[i].loadFromFile(texturesArr[i]);
//		}
//	}
//
//    // set character to Ground
//	void setOnGround() {
//		knight.onGround = true;
//		knight.moveY = 0;
//	}
//
//	// used in any collision from bottom
//	void bottomCollision() {
//		knight.onGround = false;
//		knight.moveY = 0.1;
//	}
//
//	// used to change background sprite and create & place tiles rectangles in the correct place
//	void placeScene() {
//		backgroundSprite.setTexture(levelTextures[currentScene]);
//
//		// setting common ground values for similar levels 
//		if (currentScene == 0 || currentScene == 1 || currentScene == 2 || currentScene == 3 || currentScene == 4) {
//			ground = 1300;
//		}
//
//
//		// placing each scene tiles and doors
//		if (currentScene == 0) {
//			currentTiles.resize(12);
//			
//			for (int i = 0; i < 12; i++) {
//				currentTiles[i].setFillColor(Color::Transparent);
//			}
//
//			knight.rect.left = 5;
//			knight.rect.top = 500;
//
//			currentTiles[0].setSize(Vector2f(545, 55));
//			currentTiles[0].setPosition(0, 995);
//
//			currentTiles[1].setSize(Vector2f(95, 65.5));
//			currentTiles[1].setPosition(545, 885);
//
//			currentTiles[2].setSize(Vector2f(570, 40));
//			currentTiles[2].setPosition(640, 835);
//
//			currentTiles[3].setSize(Vector2f(120, 45));
//			currentTiles[3].setPosition(1280, 670);
//
//			currentTiles[4].setSize(Vector2f(165, 65));
//			currentTiles[4].setPosition(1020, 550);
//
//			currentTiles[5].setSize(Vector2f(130, 70));
//			currentTiles[5].setPosition(830, 495);
//
//			currentTiles[6].setSize(Vector2f(770, 50 ));
//			currentTiles[6].setPosition(0, 390);
//
//			currentTiles[7].setSize(Vector2f(95, 45));
//			currentTiles[7].setPosition(1440, 565);
//
//			currentTiles[8].setSize(Vector2f(125, 50));
//			currentTiles[8].setPosition(1570, 500);
//
//			currentTiles[9].setSize(Vector2f(190, 595));
//			currentTiles[9].setPosition(1730, 460);
//
//			currentTiles[10].setSize(Vector2f(190, 40));
//			currentTiles[10].setPosition(1730, 185);
//
//			currentTiles[11].setSize(Vector2f(55, 195));
//			currentTiles[11].setPosition(1845, 265);
//
//
//		}
//		if (currentScene == 1) {
//			currentTiles.resize(11); 
//			
//			for (int i = 0; i < 11; i++) {
//				currentTiles[i].setFillColor(Color::Transparent);
//			}
//
//			knight.rect.left = 3;
//			knight.rect.top = 400;
//
//			currentTiles[0].setSize(Vector2f(320, 50));
//			currentTiles[0].setPosition(0, 785);
//
//			currentTiles[1].setSize(Vector2f(320, 500));
//			currentTiles[1].setPosition(0, 0);
//
//			currentTiles[2].setSize(Vector2f(95, 50));
//			currentTiles[2].setPosition(415, 770);
//
//			currentTiles[3].setSize(Vector2f(125, 65));
//			currentTiles[3].setPosition(610, 610);
//
//			currentTiles[4].setSize(Vector2f(190, 400));
//			currentTiles[4].setPosition(895, 665);
//
//			// these 3 tiles are identical
//			for (int i = 5; i <= 6; i++) {
//				currentTiles[i].setSize(Vector2f(100, 65));
//			}
//
//			currentTiles[5].setPosition(1215, 555);
//			currentTiles[6].setPosition(1345, 445);
//			// end of identical tiles 
//
//
//			currentTiles[7].setSize(Vector2f(450, 440));
//			currentTiles[7].setPosition(1470, 285);
//
//			currentTiles[8].setSize(Vector2f(515, 55));
//			currentTiles[8].setPosition(1405, 10);
//
//			currentTiles[9].setSize(Vector2f(770, 115));
//			currentTiles[9].setPosition(1150, 940);
//
//			currentTiles[10].setSize(Vector2f(55, 190));
//			currentTiles[10].setPosition(1845, 760);
//			
//		}
//		if (currentScene == 2) {
//			currentTiles.resize(13);
//			
//			knight.rect.left = -75;
//			knight.rect.top = 600;
//
//			for(int i =0;i < 13; i++) {
//				currentTiles[i].setFillColor(Color::Transparent);
//			}
//
//			currentTiles[0].setSize(Vector2f(125, 40));
//			currentTiles[0].setPosition(0, 955);
//
//			currentTiles[1].setSize(Vector2f(125, 60));
//			currentTiles[1].setPosition(0, 620);
//
//
//			// index 2 , 3, 4, 7, 8, 10, 11 are identical blocks in size 
//			for (int i = 2; i < 10; i++) {
//				if (i == 2 || i == 3 || i == 4 || i == 7 || i == 8 || i == 9) {
//					currentTiles[i].setSize(Vector2f(130, 55));
//				}
//			}
//
//			currentTiles[2].setPosition(285, 885);
//			currentTiles[3].setPosition(480, 775);
//			currentTiles[4].setPosition(670, 665);
//
//			currentTiles[5].setSize(Vector2f(325, 425));
//			currentTiles[5].setPosition(925, 625);
//
//			currentTiles[6].setSize(Vector2f(325, 335));
//			currentTiles[6].setPosition(925, 10);
//
//			currentTiles[7].setPosition(1310, 615);
//			currentTiles[8].setPosition(1470, 725);
//			currentTiles[9].setPosition(1595, 900);
//
//	
//			currentTiles[10].setSize(Vector2f(130, 45));
//			currentTiles[10].setPosition(1790, 900);
//
//			currentTiles[11].setSize(Vector2f(130, 60));
//			currentTiles[11].setPosition(1790, 565);
//
//			currentTiles[12].setSize(Vector2f(55, 180));
//			currentTiles[12].setPosition(1875, 705);
//		}
//		if (currentScene == 3) {
//			knight.rect.left = -10;
//			knight.rect.top = 300;
//
//			currentTiles.resize(8);
//
//			
//
//			for (int i = 0; i < 8; i++) {
//				currentTiles[i].setFillColor(Color::Transparent);
//			}
//
//
//			currentTiles[0].setSize(Vector2f(190, 375));
//			currentTiles[0].setPosition(0, 680);
//
//			currentTiles[1].setSize(Vector2f(190, 445));
//			currentTiles[1].setPosition(0, 10);
//
//			currentTiles[2].setSize(Vector2f(575, 330));
//			currentTiles[2].setPosition(190, 725);
//
//			currentTiles[3].setSize(Vector2f(125, 60));
//			currentTiles[3].setPosition(865, 885);
//
//			currentTiles[4].setSize(Vector2f(865, 115));
//			currentTiles[4].setPosition(1055, 940);
//
//			currentTiles[5].setSize(Vector2f(195, 50));
//			currentTiles[5].setPosition(1725, 690);
//
//			currentTiles[6].setSize(Vector2f(320, 50));
//			currentTiles[6].setPosition(1600, 580);
//
//			currentTiles[7].setSize(Vector2f(55, 185));
//			currentTiles[7].setPosition(1885, 760);
//
//		}
//		if (currentScene == 4) {
//			currentTiles.resize(6);
//
//			knight.rect.top = 570;
//			knight.rect.left = -10;
//
//
//			for (int i = 0; i < 6; i++)
//			{
//				currentTiles[i].setFillColor(Color::Transparent);
//			}
//		
//
//			currentTiles[0].setSize(Vector2f(255, 100));
//			currentTiles[0].setPosition(0, 960);
//
//			currentTiles[1].setSize(Vector2f(255, 725));
//			currentTiles[1].setPosition(0, 10);
//
//			currentTiles[2].setSize(Vector2f(355, 55));
//			currentTiles[2].setPosition(285, 1005);
//
//			// index 3, 4 are identical in size 
//			for (int i = 3; i <= 4 ; i++)
//			{
//				currentTiles[i].setSize(Vector2f(70, 260));
//			}
//
//			currentTiles[3].setPosition(765, 890);
//			currentTiles[4].setPosition(990, 730);
//
//			currentTiles[5].setSize(Vector2f(705, 250));
//			currentTiles[5].setPosition(1215, 625);
//		}
//		if (currentScene == 5) {
//			if (!currentTiles.empty()) {
//				currentTiles.erase(currentTiles.begin(), currentTiles.end());
//			}
//			
//			knight.rect.left = -125;
//
//			ground = 400;
//		}
// 	}
//
//	
//	// detect collision of main character and tiles rectangels 
//	void checkCollision(RectangleShape &collisionRect) {
//	
//		if (currentScene == 0) {
//			// screen left and right boundaries collision
//			if (collisionRect.getGlobalBounds().left > 1784) {
//				knight.rect.left = 1636;
//			}
//			else if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			}
//
//			// tile 1
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
//				// top collision
//				setOnGround();
//				knight.rect.top = currentTiles[0].getPosition().y - 295;
//			}
//
//
//			// tile 2
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[1].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[1].getPosition().x) {
//					knight.rect.left = currentTiles[1].getPosition().x - 275;
//				}
//				// top collision
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[1].getPosition().y) {
//					setOnGround();
//					knight.rect.top = currentTiles[1].getPosition().y - 293;
//				}
//
//		    
//			}
//
//			// tile 3
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
//				//right collision
//                if (collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 555 || collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 555.5) {
//					knight.moveY = 0.75;
//				}
//				// left collision
//				else if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[2].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[2].getPosition().x)) {
//					knight.rect.left = currentTiles[2].getPosition().x - 275;
//				} 
//				// top collision
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) {
//					setOnGround();
//					knight.rect.top = currentTiles[2].getPosition().y - 293;
//				}
//			}
//			
//			// tile 4
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
//				// top collision with considering the correct position of character 
//				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[3].getPosition().y + 15) && (collisionRect.getGlobalBounds().left > 1200)) {
//					setOnGround();
//					knight.rect.top = currentTiles[3].getPosition().y - 293;
//				}
//			}
//
//			// tile 5 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
//				// right collision
//				if (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 164 || collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 164.5) {
//					knight.rect.left = currentTiles[4].getPosition().x + 22.5;
//				}
//
//
//				//left collision
//				else if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[4].getPosition().x)) {
//					knight.rect.left = currentTiles[4].getPosition().x - 275;
//				}
//
//				// top collision 
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[4].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[4].getPosition().y + 25) {
//					knight.rect.top = currentTiles[4].getPosition().y - 295.25;
//					setOnGround();
//
//				}
//
//				// bottom
//				else if ((collisionRect.getGlobalBounds().top <= currentTiles[4].getPosition().y + 65) && (collisionRect.getGlobalBounds().left >= 910) && (collisionRect.getGlobalBounds().left < 1125)) {
//					bottomCollision();
//				}
//			}
//
//			// tile 6
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
//				//left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
//					knight.rect.left = currentTiles[5].getPosition().x - 275;
//				}
//				// right collision
//				else if (collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 128 || collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 128.5) {
//					knight.rect.left = currentTiles[5].getPosition().x - 15.5;
//				}
//
//				// top collision 
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 50) {
//					knight.rect.top = currentTiles[5].getPosition().y - 295.25;
//					setOnGround();
//
//				}
//
//				// bottom collision
//				else if ((collisionRect.getGlobalBounds().top <= currentTiles[5].getPosition().y + 70)) {
//					bottomCollision();
//				};
//			}
//
//			// tile 7 (long one on top)
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
//
//			  // right collision	
//			  if (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 768 || collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 769) {
//				  knight.rect.left = currentTiles[6].getPosition().x + 625;
//			  }
//			  // top collision
//			  else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[6].getPosition().y) {
//				  knight.rect.top = currentTiles[6].getPosition().y - 292.5;
//				  setOnGround();
//			  }
//
//			}
//
//			// tile 8 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
//      			// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[7].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[7].getPosition().x) {
//					knight.rect.left = currentTiles[7].getPosition().x - 275;
//				}
//
//				//top collision 
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[7].getPosition().y) {
//					knight.rect.top = currentTiles[7].getPosition().y - 293;
//					setOnGround();
//				}
//			}
//
//			// tile 9 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[8].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[8].getPosition().x) {
//					knight.rect.left = currentTiles[8].getPosition().x - 275;
//				}
//
//				//top collision 
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[8].getPosition().y) {
//					knight.rect.top = currentTiles[8].getPosition().y - 293;
//					setOnGround();
//				}
//			}
//
//			// tile 10
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[9].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[9].getPosition().x) {
//					knight.rect.left = currentTiles[9].getPosition().x - 273.5;
//				}
//				
//				// top collision 
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[9].getPosition().y) {
//					knight.rect.top = currentTiles[9].getPosition().y - 293;
//					setOnGround();
//				}
//			}
//
//			// tile 11 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[10].getPosition().x) ||		collisionRect.getGlobalBounds().left + 121 <= currentTiles[10].getPosition().x) {
//						knight.rect.left = currentTiles[9].getPosition().x - 273.5;
//			    }
//
//				// bottom collision
//				else if ((collisionRect.getGlobalBounds().top <= currentTiles[10].getPosition().y + 30)) {
//					bottomCollision();
//			    }
//			}
//
//			// door : tile 12
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[11].getGlobalBounds()) && noOFEnemies == 0) {
//				currentScene++;
//				placeScene();
//			}
//		}
//		if (currentScene == 1) {
//			// screen left and right boundaries collision
//			if (collisionRect.getGlobalBounds().left > 1784) {
//				knight.rect.left = 1636;
//			}
//			else if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			}
//			// tile 1
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
//				// right collision
//				if ((collisionRect.getGlobalBounds().left  >= currentTiles[0].getPosition().x + 319) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 319.5)) {
//					knight.rect.left = currentTiles[0].getPosition().x + 172;
//				}
//
//				// top collision
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10) {
//					setOnGround();
//				}
//
//			}
//
//			// tile 2
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))   {
//				// right collision
//				if ((collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 318) || (collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 319)) {
//				   knight.rect.left = currentTiles[1].getPosition().x + 175;
//				}
//
//				// bottom collision
//				else if (collisionRect.getGlobalBounds().top <= currentTiles[1].getPosition().y + 490) {
//					bottomCollision();
//				}
//			   
//
//			}
//
//			// tile 3 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
//				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y ) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 15) &&(collisionRect.getGlobalBounds().left > 335) && (collisionRect.getGlobalBounds().left < 497)) {
//					setOnGround();
//				}
//			    
//				else if (collisionRect.getGlobalBounds().left >= 497) {
//					knight.moveY = 0.3;
//				}
//			}
//
//			// tile 4 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
//				// left collision 
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[3].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[3].getPosition().x)) {
//					knight.rect.left = currentTiles[3].getPosition().x - 271.5;
//				}
//
//				// right collision
//				else if ((collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 124) || (collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 124.5)) {
//					knight.rect.left = currentTiles[3].getPosition().x  - 23;
//				}
//
//				// top collision
//				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[3].getPosition().y + 15)) {
//					setOnGround();
//				}
//			}
//
//			// tile 5
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[4].getPosition().x)) {
//					knight.rect.left = currentTiles[4].getPosition().x - 271.5;
//   			    }
//
//				// right collision
//				else if ((collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 189.5)) {
//					knight.rect.left = currentTiles[4].getPosition().x + 45.5;
//				}
//
//				// top collision
//				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[4].getPosition().y + 15)) {
//					setOnGround();
//				}
//			}
//
//			// tile 6
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
//			   // left collision	
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
//					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
//				}
//
//				// right collision 
//				if ((collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 98) || (collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 99.5)) {
//					knight.rect.left = currentTiles[5].getPosition().x - 46;
//				}
//
//				// top collision
//				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[5].getPosition().y + 15)) {
//					setOnGround();
//				}
//
//				
//
//				 // bottom  collsion 
//				 else if (collisionRect.getGlobalBounds().top > currentTiles[5].getPosition().y + 49) {
//					bottomCollision();
//				  }
//			}
//
//			// tile 7
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
//				// left collision	
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x)) {
//					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
//				}
//
//				// top collision
//				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[6].getPosition().y + 15)) {
//					setOnGround();
//				}
//
//			}
//
//			// tile 8
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
//			    //  left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[7].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[7].getPosition().x)) {
//
//					knight.rect.left = currentTiles[7].getPosition().x - 271.5;
//				}
//
//				// top collision 
//				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[7].getPosition().y + 15)) {
//					setOnGround();
//				}
//
//				// bottom  collsion 
//				else if (collisionRect.getGlobalBounds().top > currentTiles[7].getPosition().y + 435) {
//					bottomCollision();
//				}
//
//			}
//
//			// tile 9
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
//				// left collision
//				 if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[8].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[8].getPosition().x)) {
//					knight.rect.left = currentTiles[8].getPosition().x - 271.5;
//				 }
//
//                  // bottom  collsion 
//				 else if (collisionRect.getGlobalBounds().top > currentTiles[8].getPosition().y + 50) {
//					bottomCollision();
//			     }
//				
//			}
//
//			// tile 10
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds())) {
//				if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[9].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[9].getPosition().y + 15)) {
//					setOnGround();
//				}
//			}
//
//			// door : tile 11
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds()) && noOFEnemies == 0) {
//				
//				currentScene++;
//				placeScene();
//			}
//		}
//		if (currentScene == 2) {
//		   
//			// screen left and right boundaries collision
//			if (collisionRect.getGlobalBounds().left > 1784) {
//				knight.rect.left = 1636;
//			}
//			else if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			}
//
//			// tile 1
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
//			  // top collison
//			  if (collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10) {
//				setOnGround();
//			  }
//			}
//
//			// tile 2
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
//
//				//right collision 
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 123) ||(collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 124)) {
//					knight.rect.left = currentTiles[1].getPosition().x - 21.5;
//				}
//
//
//				// top collison
//				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[1].getPosition().y) && collisionRect.getGlobalBounds().top + 145 < currentTiles[1].getPosition().y + 10) {
//					setOnGround();
//				}
//
//				// bottom collison
//				else if ((collisionRect.getGlobalBounds().top > currentTiles[1].getPosition().y + 50)) {
//					bottomCollision();
//				}
//			}
//
//			// tiles : 3, 4, 5, 8, 9, 10,
//			// index : 2, 3, 4, 7, 8, 9  
//			// these tiles shares same size so they have same collison (at least base ones : left & top )
//			for (int i = 2; i <= 9; i++)
//			{
//				if (i == 2 || i == 3 || i == 4 || i == 7 || i == 8 || i == 9) {
//					if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
//					    // left collision
//						if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[i].getPosition().x)) {
//							knight.rect.left = currentTiles[i].getPosition().x - 271.5;
//						}
//
//						// right collision
//						else if (((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 128) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 129))) {
//							knight.rect.left = currentTiles[i].getPosition().x - 16.5;
//						}
//
//						// top collision
//						else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[i].getPosition().y + 5)) {
//							setOnGround();
//						}
//					}
//				}
//			}
//
//			// tile 6
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
//				// top collsion
//				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 10)) {
//					setOnGround();
//				}
//			}
//
//			// tile 7
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
//			    // left collison
//				if (((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x))) {
//					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
//				}
//
//				// right collision 
//				else if((collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 323) || (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 324)) {
//					knight.rect.left = currentTiles[6].getPosition().x + 180;
//				}
//
//				// bottom collsion 
//				else if ((collisionRect.getGlobalBounds().top > currentTiles[6].getPosition().y + 325)) {
//					bottomCollision();
//				}
//			}
//
//			// tile 11
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds())) {
//				// top collison
//				if (collisionRect.getGlobalBounds().top + 145 > currentTiles[10].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[10].getPosition().y + 10) {
//					setOnGround();
//				}
//
//			}
//
//			// tile 12
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[11].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[11].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[11].getPosition().x)) {
//					knight.rect.left = currentTiles[11].getPosition().x - 271.5;
//				}
//
//				// top collision
//				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[11].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[11].getPosition().y + 10) {
//					setOnGround();
//				}
//			}
//
//			// tile 13 : Door
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[12].getGlobalBounds())) {
//				knight.rect.left = -10;
//				knight.rect.top = 300;
//				currentScene++;
//				placeScene();
//			}
//		}
//		if (currentScene == 3) {
//			// screen left and right boundaries collision
//			if (collisionRect.getGlobalBounds().left > 1784) {
//				knight.rect.left = 1636;
//			}
//			else if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			}
//
//			// tile 1 
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 189)) {
//					knight.rect.left = currentTiles[0].getPosition().x + 43.5;
//				}
//
//				// top collsion
//				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 5)) {
//					setOnGround();
//				}
//			}
//
//			// tile 2
//			if(collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
//
//				// right collsion
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 189)) {
//					knight.rect.left = currentTiles[1].getPosition().x + 43.5;
//				}
//
//				// bottom collision
//				else if (collisionRect.getGlobalBounds().top > currentTiles[1].getPosition().y + 440) {
//					bottomCollision();
//				}
//			}
//
//			// tile 3
//			if(collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
//				// right collison
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 573) || (collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 574)) {
//					knight.rect.left = currentTiles[2].getPosition().x + 428.5;
//				}
//
//				// top collsion 
//				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 10)) {
//					setOnGround();
//				}
//			}
//
//			// tile 4
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
//			    // right collison
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 123) || (collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 124)) {
//					knight.rect.left = currentTiles[3].getPosition().x - 21.5;
//				}
//
//				// top collsion 
//				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[3].getPosition().y + 10)) {
//					setOnGround();
//				}
//			}
//
//			// tile 5
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
//				// top collision
//			    if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[4].getPosition().y + 10)) {
//					setOnGround();
//			    }
//			}
//
//			// tile 6 : above the door
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
//				//left collison
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
//					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
//				}
//
//				// bottom collision
//				else if (collisionRect.getGlobalBounds().top > currentTiles[5].getPosition().y + 35) {
//					bottomCollision();
//				}
//			}
//
//			// tile 7
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
//				//left collison
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x)) {
//					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
//				}
//
//				// bottom collision
//				else if (collisionRect.getGlobalBounds().top > currentTiles[6].getPosition().y + 35) {
//					bottomCollision();
//				}
//			}
//
//			// tile 8
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
//				currentScene++;
//				placeScene();
//			}
//		}
//		if (currentScene == 4) {
//			// screen left and right boundaries collision
//			 if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			 }
//
//
//			// tile 1
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
//				// right collision 
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getGlobalBounds().getPosition().x + 253) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getGlobalBounds().getPosition().x + 254)) {
//					knight.rect.left = currentTiles[0].getGlobalBounds().getPosition().x + 108.5;
//				}
//
//				// top collision
//				else if((collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y ) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10)) {
//					setOnGround();
//				}
//			
//			}
//
//			// tile 2
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
//				// right collision
//				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getGlobalBounds().getPosition().x + 253) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getGlobalBounds().getPosition().x + 254)) {
//					knight.rect.left = currentTiles[1].getGlobalBounds().getPosition().x + 108.5;
//				}
//
//
//				// bottom collision
//				else {
//					bottomCollision();
//				}
//			}
//
//			// tile 3
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
//				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 15) ) {
//					setOnGround();
//				}
//			}
//
//			// tile 4 , 5 : identical tiles in collision
//			for (int i = 3; i <= 4; i++)
//			{
//
//				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
//					// right collision
//					if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 68) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 69)) {
//						knight.rect.left = currentTiles[i].getGlobalBounds().getPosition().x - 76.5;
//					}
//
//					// top collision
//					else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[i].getPosition().y + 15)) {
//						setOnGround();
//					}
//				}
//			}
//
//			// tile 6
//			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
//				// left collision
//				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
//					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
//				}
//				
//
//				// top collision
//				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 15)) {
//					setOnGround();
//				}
//			}
//
//			// next scene detection
//			if (collisionRect.getGlobalBounds().left >= 1790) {
//				currentScene++;
//				placeScene();
//			}
//		}
//		if (currentScene == 5) {
//			// screen left and right boundaries collision
//			
//			 if (collisionRect.getGlobalBounds().left < 4) {
//				knight.rect.left = -145;
//			} else if (collisionRect.getGlobalBounds().left > 1820) {
//				knight.rect.left = 1668;
//			}
//		}
//	}
//} levelOneMap;
//
//// aracade mode window (logic)
//void arcadeMode(RenderWindow& window, int& pageNum) 
//{
//
//	Texture backgroundTexture;
//	backgroundTexture.loadFromFile("external/background1.png");
//	Sprite backgroundSprite;
//	backgroundSprite.setTexture(backgroundTexture);
//	pauseMenu pauseMenu;
//	pauseMenu.PauseMenufunc(1920, 1080); //struct, variable, and function have the same name im sorry
//	
//	
//
//	Clock clock;
//	knight.assignSprite(); // Initialize player character
//	knight.rect.left = 10;
//	knight.rect.top = 850;
//
//	while (window.isOpen()) 
//	{
//		// Handle events
//		Event event;
//		while (window.pollEvent(event)) {
//
//
//			if (event.type == Event::Closed) {
//				window.close();
//			}
//
//			if (event.type == Event::KeyPressed)
//			{
//				if (event.key.code == Keyboard::Escape)
//				{
//					pauseMenu.paused = true;
//				}
//			}
//		}
//
//		// Update game logic
//		
//		// Clear the window
//		window.clear();
//
//		if (!pauseMenu.paused) 
//		{
//			
//			movements();
//
//			float time = (float) clock.getElapsedTime().asMicroseconds();
//			clock.restart();
//			time /= 650;
//			if (time > 20)
//				time = 20;
//			knight.update(time);
//			
//		}
//		else 
//		{
//			
//			window.draw(backgroundSprite);
//			window.draw(knight.sprite);
//			for (int i = 0; i < 3; i++)
//			{
//				std::cout << "Pause menu element " << i << " position: (" << pauseMenu.pauseElements[i].getPosition().x << ", " << pauseMenu.pauseElements[i].getPosition().y << ")" << std::endl;
//			}
//
//			pauseMenu.show(window, pageNum);
//			
//		}
//		if (pageNum == 1)
//		{
//			
//		}
//		// Draw game elements
//		window.draw(backgroundSprite);
//		window.draw(knight.sprite);
//
//
//
//		// Display the window
//
//		window.display();
//	}
//}
//
//void levelOne(RenderWindow& window) {
//	Clock clock;
//	knight.assignSprite(); // Initialize player character
//
//
//	levelOneMap.loadTextures();
//	levelOneMap.placeScene();
//	while (window.isOpen()) {
//		// redRect for collision detection
//		RectangleShape collisionRect;
//		collisionRect.setOutlineThickness(2);
//		collisionRect.setOutlineColor(Color::Transparent);
//		collisionRect.setFillColor(Color::Transparent);
//		collisionRect.setSize(Vector2f(120, 145));
//		Vector2f knightPos = knight.sprite.getPosition();
//		// adjusting the collision rect to be more accurate 
//		collisionRect.setPosition(knightPos.x + 150, knightPos.y + 150);
//		
//	
//
//		
//
//		// Handle events
//		Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == Event::Closed) {
//				window.close();
//			}
//
//			if (event.type == Event::MouseButtonPressed) {
//				if (event.mouseButton.button == Mouse::Left) {
//					Vector2i mousePos = Mouse::getPosition(window);
//					cout << "MousePos x : " << mousePos.x << " MousePos y :  " << mousePos.y << endl;
//					 cout << "Knight Rect Left : " << knight.rect.left << " Knight Rect Top : " << knight.rect.top << endl;
//					 cout << "Red Rect Left : " << collisionRect.getGlobalBounds().left << " Red Rect Top " << collisionRect.getGlobalBounds().top << endl;
//				}
//			}
//		}
//
//
//		
//	
//		// check player collision (always should be placed before movement fn to avoid silly animation bugs :) )
//	    levelOneMap.checkCollision(collisionRect);
//
//		// Update game logic
//		movements();
//
//		
//
//		float time = (float)clock.getElapsedTime().asMicroseconds();
//		clock.restart();
//		time /= 650;
//		if (time > 20)
//			time = 20;
//		knight.update(time);
//
//	
//
//		
//
//		// Clear the window
//		window.clear();
//
//		// Draw game elements	
//		window.draw(levelOneMap.backgroundSprite);
//		window.draw(knight.sprite);
//
//		for (int i = 0; i < currentTiles.size(); i++)
//		{
//			window.draw(currentTiles[i]);
//		}
//
//		window.draw(collisionRect);
//		
//		// Display the window
//		window.display();
//	}
//}