#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
using namespace std;
using namespace sf;

int ground = 820;
struct character {
	Sprite sprite;  // The sprite representing the player character
	float currentFrame;  // The current frame of animation.
	float moveX, moveY;  // Movements on x and y direction
	bool onGround;  // indicate if the player is on the ground
	FloatRect rect;  // The bounding rectangle of the player
	int lastKeyPressed;  // The last key pressed by the player
	int noOfAttacks;   // no of attacks dealt by the player to the enemy
	string state;  // The current state of the player
	int health;  // Health of the player
	Texture stateTexture[15];  // Array of textures for different states

	// Function to load textures for different states
	void loadTextures() {
		string stateElementsTX[15] = { "knight/_Idle.png", "knight/_Run.png", "knight/_Dash.png", "knight/_Jump.png", "knight/_Roll.png", "knight/_Hit.png", 
										"knight/_SlideAll.png", "knight/_Attack.png", "knight/_Attack2.png","knight/_AttackCombo.png", "knight/_CrouchAll.png", 
										"knight/_CrouchAttack.png", "knight/_CrouchWalk.png", "knight/_Fall.png", "knight/_Death.png" };  //Array of string for 

		for (int i = 0; i < 15; i++)
			stateTexture[i].loadFromFile(stateElementsTX[i]);
	}

	// Function to assign sprite and initialize properties
	void assignSprite() {
		sprite.setScale(3.25, 3.25);  //Set sprite scale
		moveX = 0;  // Character starts not moving in the X-axis
		moveY = 0;  // Character starts not moving in the Y-axis
		currentFrame = 0;
		noOfAttacks = 0;
		lastKeyPressed = 1; 
		state = "Idle";  // Initialize state to idle
		health = 100;  // Initialize health to 100
		loadTextures();  // Loading the textures
		updateTexture();  // Update the texture of the sprite to idle
	}

	// Function to update player's position and animation based on state
	void update(float time) {
		rect.left += moveX * time;  // Movement of the character on x-axis

		if (!onGround)
			moveY += (0.00085 * time);  // Gravity 

		rect.top += moveY * time;  // Movement of the character on y-axis
		onGround = false;  // Intialize onGround as false in case the character postion off ground 

		if (rect.top > ground) {  // when character exceeds the ground
			rect.top = ground;
			moveY = 0;
			onGround = true;
		}

		currentFrame += 0.005 * time;

		// Update animation based on state
		if (state == "Idle") {
			if (currentFrame > 10) currentFrame -= 10;  // This condition changes according to number of changes
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));  // To right
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));  // To left
		}
		else if (state == "Walk") {
			if (currentFrame > 10) currentFrame -= 10;
			if (moveX > 0)  sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));  // movement to right
			if (moveX < 0)  sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80)); // movement to left
		}
		else if (state == "Dash") {
			if (currentFrame > 2) currentFrame -= 2;
			if (moveX > 0) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			if (moveX < 0) sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Jump") {
			if (currentFrame > 3) currentFrame -= 3;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Roll") {
			if (currentFrame > 12) currentFrame -= 12;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Hit") {
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(0, 0, 120, 80));
			else sprite.setTextureRect(IntRect(120, 0, -120, 80));
		}
		else if (state == "Slide") {
			if (currentFrame > 4) {
				if (lastKeyPressed == 1)  sprite.setTextureRect(IntRect(120 * 3, 0, 120, 80));
				else  sprite.setTextureRect(IntRect(120 * 3 + 120, 0, -120, 80));
			}
			else if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80)) ;
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Attack") {
			if (currentFrame > 4) currentFrame -= 4;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Attack2") {
			if (currentFrame > 6) currentFrame -= 6;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "AttackCombo") {
			if (currentFrame > 10) currentFrame -= 10;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Crouch") {
			if (currentFrame > 3) {
				if (lastKeyPressed == 1)  sprite.setTextureRect(IntRect(120 * 2, 0, 120, 80));
				else  sprite.setTextureRect(IntRect(120 * 2 + 120, 0, -120, 80));
			}
			else if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "CrouchAttack") {
			if (currentFrame > 4) currentFrame -= 4;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "CrouchWalk") {
			if (currentFrame > 8) currentFrame -= 8;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Fall") {
			if (currentFrame > 3) currentFrame -= 3;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Death") {
			if (currentFrame > 10) currentFrame -= 10;
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}

		sprite.setPosition(rect.left, rect.top);
		moveX = 0;
	}

	// Function to update the texture based on current state
	void updateTexture() {
		string stateElement[15] = { "Idle", "Walk", "Dash", "Jump", "Roll", "Hit", "Slide", "Attack", "Attack2", "AttackCombo", "Crouch", "CrouchAttack", "CrouchWalk", "Fall", "Death"};

		for (int i = 0; i < 15; i++)
		{
			if (state == stateElement[i])
				sprite.setTexture(stateTexture[i]);
		}
	}

	bool isAlive() {
		return health;
	}
} knight;

void movements() {
	if (Keyboard::isKeyPressed(Keyboard::D)) {
		knight.state = "Walk";
		knight.updateTexture();
		knight.moveX = 0.2;
		knight.lastKeyPressed = 1;  // Last Key pressed is Right
	}
	if (Keyboard::isKeyPressed(Keyboard::A)) {
		knight.state = "Walk";
		knight.updateTexture();
		knight.moveX = -0.2;
		knight.lastKeyPressed = 2;  // Last Key pressed is Left
	}
	if (Keyboard::isKeyPressed(Keyboard::Tab)) {
		knight.state = "Dash";
		knight.updateTexture();
		if (knight.lastKeyPressed == 1) knight.moveX = 0.4;
		else knight.moveX = -0.4;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		if (knight.onGround == true) {
			knight.moveY = -0.4;
			knight.state = "Jump";
			knight.onGround = false;
		}
		knight.updateTexture();
	}
	if (Keyboard::isKeyPressed(Keyboard::R)) {
		knight.state = "Roll";
		knight.updateTexture();
		if (knight.lastKeyPressed == 1) knight.moveX = 0.2;
		else knight.moveX = -0.2;
	}
	if (Keyboard::isKeyPressed(Keyboard::LControl)) {
		knight.state = "Slide";
		knight.updateTexture();
		if (knight.lastKeyPressed == 1) knight.moveX = 0.2;
		else knight.moveX = -0.2;
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift)) {
		knight.state = "Crouch";
		knight.updateTexture();
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::E)) {
		knight.state = "CrouchAttack";
		if (knight.lastKeyPressed == 1) knight.moveX = 0.05;
		else knight.moveX = -0.05;
		knight.updateTexture();
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::D)) {
		knight.state = "CrouchWalk";
		knight.updateTexture();
		knight.moveX = 0.1;
		knight.lastKeyPressed = 1;  // Last Key pressed is Right
	}
	if (Keyboard::isKeyPressed(Keyboard::LShift) && Keyboard::isKeyPressed(Keyboard::A)) {
		knight.state = "CrouchWalk";
		knight.updateTexture();
		knight.moveX = -0.1;
		knight.lastKeyPressed = 2;  // Last Key pressed is Right
	}
	if (Keyboard::isKeyPressed(Keyboard::E)) { 
		knight.noOfAttacks++;
		if (knight.noOfAttacks % 2 == 1) { knight.state = "Attack"; knight.noOfAttacks++;
		}
		else if (knight.noOfAttacks % 4 == 0)  knight.state = "AttackCombo";
		else {
			knight.state = "Attack2"; 
			knight.noOfAttacks++;
		}
		knight.updateTexture();
	}
	if (knight.moveX == 0 && knight.moveY == 0 && !(Keyboard::isKeyPressed(Keyboard::E) || Keyboard::isKeyPressed(Keyboard::LShift))) {
		knight.state = "Idle";
		knight.updateTexture();
	}

	
}

void arcadeMode(RenderWindow& window) {
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("external/background1.png");
	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	Clock clock;
	knight.assignSprite(); // Initialize player character
	knight.rect.left = 10;
	knight.rect.top = 850;

	while (window.isOpen()) {
		// Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}

		// Update game logic
		movements();

		float time = (float) clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 650;
		if (time > 20)
			time = 20;
		knight.update(time);
		
		// Clear the window
		window.clear();

		// Draw game elements
		window.draw(backgroundSprite);
		window.draw(knight.sprite);

		// Display the window
		window.display();
	}
}