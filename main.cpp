#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using namespace sf;
struct SecEnemy;
// 3 is the number of Perks
#define NUMBER_OF_PERKS 3

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

int pageNum = 1;
int score = 0;
int ground = 1300;
float const rightWall = 1680;
float const leftWall = -180;
vector<RectangleShape> currentTiles;
const int knight_num_of_textures = 15;
const int num_of_sec_enemies = 4;
const int num_of_enemy_textures = 10;
const int num_of_boss_enemies = 2;
int pausedtimes = 0;
bool boss1restarted = false;

// Array to check how many times i click on ubgrade
int upgradeCheck[3] = {};
string totalCoins, tempCheck, tempPowerUP;
int check;
int storeCoins;
fstream coinFile, checkCoinsFile, powerUp1File, powerUp2File, powerUp3File;

SoundBuffer clickbuffer;
Sound clicksound;

void store(RenderWindow& window);

RectangleShape RectCreator(float x, float y, float posx = 0, float posy = 0)
{
	RectangleShape Rect;
	Rect.setOutlineThickness(2);
	Rect.setOutlineColor(Color::Transparent);
	Rect.setFillColor(Color::Transparent);
	Rect.setSize(Vector2f(x, y));
	Rect.setPosition(posx, posy);
	return Rect;
}

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

struct character {
	Sprite sprite;  // The sprite representing the player character
	float currentFrame;  // The current frame of animation.
	float attack = 10;  // characters attack
	float moveX, moveY;  // Movements on x and y direction
	bool onGround;  // indicate if the player is on the ground
	FloatRect rect;  // The bounding rectangle of the player
	int lastKeyPressed;  // The last key pressed by the player
	int noOfAttacks;   // no of attacks dealt by the player to the enemy 
	string state;  // The current state of the player
	double health;  // Health of the player
	Texture stateTexture[knight_num_of_textures];  // Array of textures for different states
	RectangleShape collisionRect;
	bool dead = false;
	bool is_attacked;

	void knightattck()
	{
		
		for (int i = 1; i < 5; i++)
		{
			if (upgradeCheck[0] == i)
			{
				attack = 10 * (i*2);
			}
		}
	}

	// Function to load textures for different states
	void loadTextures() {
		string stateElementsTX[knight_num_of_textures] = { "knight/_Idle.png", "knight/_Run.png", "knight/_Dash.png", "knight/_Jump.png", "knight/_Roll.png", "knight/_Hit.png",
										"knight/_SlideAll.png", "knight/_Attack.png", "knight/_Attack2.png","knight/_AttackCombo.png", "knight/_CrouchAll.png",
										"knight/_CrouchAttack.png", "knight/_CrouchWalk.png", "knight/_Fall.png", "knight/_Death.png" };  //Array of string for 

		for (int i = 0; i < knight_num_of_textures; i++)
			stateTexture[i].loadFromFile(stateElementsTX[i]);
	}

	// Function to assign sprite and initialize properties
	void assignSprite() {
		sprite.setScale(3.7, 3.7);  //Set sprite scale
		moveX = 0;  // Character starts not moving in the X-axis
		moveY = 0;  // Character starts not moving in the Y-axis
		currentFrame = 0;
		noOfAttacks = 0;
		lastKeyPressed = 1;
		state = "Idle";  // Initialize state to idle
		health = 100;  // Initialize health to 100

		knight.rect.left = 10;
		knight.rect.top = 850;
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

		if (rect.left > rightWall)
			rect.left = rightWall;

		if (rect.left < leftWall)
			rect.left = leftWall;

		if (rect.top > ground) {  // when character exceeds the ground
			rect.top = ground;
			moveY = 0;
			onGround = true;
		}

		currentFrame += 0.005 * time;
		if (health <= 0)
		{
			state = "Death";
		}

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
			if (currentFrame > 20) {
				currentFrame = 0;
				state = "";
			}
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Slide") {
			if (currentFrame > 4) {
				if (lastKeyPressed == 1)  sprite.setTextureRect(IntRect(120 * 3, 0, 120, 80));
				else  sprite.setTextureRect(IntRect(120 * 3 + 120, 0, -120, 80));
			}
			else if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}
		else if (state == "Attack") { // -----------------------------------------------------------------------------
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
			if (currentFrame > 10) {
				currentFrame -= 10;
				dead = true;
			}
			if (lastKeyPressed == 1) sprite.setTextureRect(IntRect(120 * int(currentFrame), 0, 120, 80));
			else sprite.setTextureRect(IntRect(120 * int(currentFrame) + 120, 0, -120, 80));
		}

		sprite.setPosition(rect.left, rect.top);
		moveX = 0;
	}

	// Function to update the texture based on current state
	void updateTexture() {
		string stateElement[knight_num_of_textures] = { "Idle", "Walk", "Dash", "Jump", "Roll", "Hit", "Slide", "Attack", "Attack2", "AttackCombo", "Crouch", "CrouchAttack", "CrouchWalk", "Fall", "Death" };

		for (int i = 0; i < knight_num_of_textures; i++)
		{
			if (state == stateElement[i])
				sprite.setTexture(stateTexture[i]);
		}
	}

	bool isAlive() {
		return health > 0;
	}
} knight;

struct perk
{
	Sprite action;
	Sprite upgradeButton;
	Sprite priceTexture;
	Text upgradeText;
	Text price;
	Text info;
	FloatRect bounds;
	FloatRect upgradeBounds;
}perks[NUMBER_OF_PERKS];

//Enemies will be 1 Bosses and 2 small different enemy guards for level 1
// Enemies for other levels will be determined later
struct SecEnemy
{
	Sprite sprite;
	string state;  // The current state of the enemy
	FloatRect rect;  // The bounding rectangle of the enemy
	float currentFrame;  // The current frame of animation.
	float movement_range;
	//not needed	//float attacking_factor;
	//not needed	//float attacking_range = attacking_factor * movement_range; // the boundaries of which the enemy will start attacking the character if it exists in it
	//not needed int curpos; // current x position
	int num_of_cur_textures = 0; // number of textures for the current type
	int cur_enemy_idx; // current enemy index (used for loading the textures)
	int right_boundary; // the boundaries values of x position
	int left_boundary;
	RectangleShape zone;
	int attack;
	int health;
	int attack_pause_time;
	bool dead = false;
	int dir = 1; // character direction
	float speed;
	bool is_attacked;
	// ------------ DYNAMIC ARRAY, DELETED WHEN CLOSING WINDOW ------------
	Texture* stateTexture = new Texture[0];  // Array of textures for different states
	string enemy_type; // to load different enemies and set ther attributes based on thier type (name)
	string enemy_textures[num_of_sec_enemies][num_of_enemy_textures] =
	{
		{"./enemies/Skeleton_enemy/Skeleton idle.png",
		"./enemies/Skeleton_enemy/Skeleton moving.png",
		"./enemies/Skeleton_enemy/Skeleton attack.png",
		"./enemies/Skeleton_enemy/Skeleton on hit.png",
		"./enemies/Skeleton_enemy/Skeleton dead.png"}
		,
		{"./enemies/Evil_Wizard/Idle.png",
		"./enemies/Evil_Wizard/Move.png",
		"./enemies/Evil_Wizard/Attack.png",
		"./enemies/Evil_Wizard/Take Hit.png",
		"./enemies/Evil_Wizard/Death.png"}
	};
	void assign_sec_enemy_info(string enemytype, int posx, int posy, int movementrange, int attackpow, int attpause, int hp, float scalefacx = 0, float scalefacy = 0, float speedfac = 0) {
		enemy_type = enemytype;
		if (enemytype == "skeleton")
		{
			num_of_cur_textures = 5;
			cur_enemy_idx = 0;
			state = "walk";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(4 + scalefacx, 4 + scalefacy);
			left_boundary = posx - movementrange; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + movementrange;
			//zone = RectCreator(190, 50, posx + 150, posy + 150);
			movement_range = movementrange;
			attack = attackpow; //current skeleton power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_sec_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1 + speedfac;
			//rect.left = 10;
			//rect.top = 850;
		}
		else if (enemytype == "EvilWizard")
		{
			num_of_cur_textures = 5;
			cur_enemy_idx = 1;
			state = "walk";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(2.6 + scalefacx, 2.6 + scalefacy);
			left_boundary = posx - movementrange; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + movementrange;
			//zone = RectCreator(300, 100, posx+150,posy+120);
			movement_range = movementrange;
			attack = attackpow; //current wizard power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_sec_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1696969 + speedfac;
		}
	}

	bool is_player_in_range_x() { // checking if the character is in our boundaries
		return zone.getGlobalBounds().intersects(knight.collisionRect.getGlobalBounds());
	}
	bool is_knight_sword_touching() { // checking if the sword of knight touching the character
		is_attacked = false;
		float diff = rect.left - knight.rect.left;
		float diff2 = rect.top - knight.rect.top;

		if (knight.lastKeyPressed == 1) // knight is facing right
		{
			if (-25 <= diff && diff <= 240 && (-200 <= diff2 && diff2 <= 200))
				is_attacked = true;
		}
		else // knight is facing left
		{
			if (-100 <= diff && diff <= 110 && (-200 <= diff2 && diff2 <= 200))
				is_attacked = true;
		}
		return is_attacked && (knight.state == "Attack" || knight.state == "AttackCombo" || knight.state == "Attack2");
	}


	void load_sec_enemy_textures() {
		stateTexture = new Texture[num_of_cur_textures];
		for (int i = 0; i < num_of_cur_textures; i++)
			stateTexture[i].loadFromFile(enemy_textures[cur_enemy_idx][i]);
	}

	float turn_time = 10, pause_time = 0;
	void update_skeleton_state(float time)
	{
		bool attacked = true;
		currentFrame += 0.05 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[4]);
			if (currentFrame >= 13)
				dead = true; // player dies after playing the full animation
		}
		//else if (state != "on hit" && abs(knight.rect.left - rect.left + 60) <= 110) // trying to debug/fix on hit animation
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (is_player_in_range_x()) {
			state = "attack";
		}
		else
			state = "walk";

		if (state == "walk")
		{
			sprite.setTexture(stateTexture[1]);
			if (currentFrame >= 12)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place

			if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
			{
				dir *= -1;
				turn_time = 10;
			}
		}
		else if (state == "attack")
		{
			pause_time -= time; // pause time between every two hits, first hit's pause time = 0
			if (pause_time <= 0) {
				sprite.setTexture(stateTexture[2]);
				if (currentFrame >= 13)
				{
					currentFrame = 0;
				}

				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				//left_boundary = rect.getPosition().x - movement_range;
				//right_boundary = left_boundary + 2 * movement_range; // so we don't use the getPosition() twice ;)

				pause_time = attack_pause_time;
			}

		}
		else if (state == "on hit") {
			sprite.setTexture(stateTexture[3]);
			if (currentFrame >= 3)
			{
				currentFrame = 0;
				state = "";
				health -= knight.attack;
			}

		}
		if (dir > 0)
			sprite.setTextureRect(IntRect(64 * int(currentFrame), 0, 64, 64)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
		else
			sprite.setTextureRect(IntRect(64 * int(currentFrame) + 64, 0, -64, 64)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)

	}

	void update_evilwiz_state(float time)
	{
		currentFrame += 0.05 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[4]);
			if (currentFrame >= 5)
				dead = true; // player dies after playing the full animation
		}
		//else if (state != "on hit" && abs(knight.rect.left - rect.left + 60) <= 110) // trying to debug/fix on hit animation
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (is_player_in_range_x())
			state = "attack";
		else
			state = "walk";

		if (state == "walk")
		{
			sprite.setTexture(stateTexture[1]);
			if (currentFrame >= 8)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place

			if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
			{
				dir *= -1;
				turn_time = 10;
			}
		}
		else if (state == "attack")
		{
			pause_time -= time; // pause time between every two hits, first hit's pause time = 0
			if (pause_time <= 0) {
				sprite.setTexture(stateTexture[2]);
				if (currentFrame >= 8) {
					currentFrame = 0;
					knight.health -= attack;
				}
				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				/*left_boundary = rect.getPosition().x - movement_range;
				right_boundary = left_boundary + 2 * movement_range; */// so we don't use the getPosition() twice ;)

				pause_time = attack_pause_time;
			}

		}
		else if (state == "on hit") {
			sprite.setTexture(stateTexture[3]);
			if (currentFrame >= 4)
			{
				currentFrame = 0;
				state = "";
				health -= knight.attack;
			}

		}
		if (dir > 0)
			sprite.setTextureRect(IntRect(150 * int(currentFrame), 0, 150, 150)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
		else
			sprite.setTextureRect(IntRect(150 * int(currentFrame) + 150, 0, -150, 150)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)

	}

}
Skeleton_1, Skeleton_2, Skeleton_3, Skeleton_4, Skeleton_5, Skeleton_6, Skeleton_7, Skeleton_8, Evil_Wizard_1, Evil_Wizard_2, Evil_Wizard_3, Evil_Wizard_4, Evil_Wizard_5, Evil_Wizard_6, Evil_Wizard_7, Evil_Wizard_8;

struct BossEnemy
{
	Sprite sprite;
	string state;  // The current state of the enemy
	FloatRect rect;  // The bounding rectangle of the enemy
	float currentFrame;  // The current frame of animation.
	float kill_zone;
	//not needed	//float attacking_factor;
	//not needed	//float attacking_range = attacking_factor * movement_range; // the boundaries of which the enemy will start attacking the character if it exists in it
	//not needed int curpos; // current x position
	int num_of_cur_textures = 0; // number of textures for the current type
	int cur_enemy_idx; // current enemy index (used for loading the textures)
	int right_boundary; // the boundaries values of x position
	int left_boundary;
	int attack1;
	int attack2;
	int health;
	int attack_pause_time;
	bool dead = false;
	int dir = 1; // character direction
	float speed;
	bool is_attacked;
	int skill_shift = 0;
	//not needed
	/*
	double left_tracker;
	double right_tracker;
	*/
	RectangleShape zone1;
	RectangleShape zone2;
	// ------------ DYNAMIC ARRAY, DELETED WHEN CLOSING WINDOW ------------
	Texture* stateTexture = new Texture[0];  // Array of textures for different states
	string enemy_type; // to load different enemies and set ther attributes based on thier type (name)
	string enemy_textures[num_of_boss_enemies][num_of_enemy_textures] =
	{
		{"./enemies/Undead_executioner/idle.png",      // 0
		"./enemies/Undead_executioner/walk.png",	   // 1
		"./enemies/Undead_executioner/attacking.png",  // 2
		"./enemies/Undead_executioner/attacking1.png", // 3
		"./enemies/Undead_executioner/skill1.png",     // 4
		"./enemies/Undead_executioner/death.png"}      // 5
		,
		{
		"./enemies/Evil_Wizard_2/Idle.png",			 // 0
		"./enemies/Evil_Wizard_2/Jump.png",          // 1
		"./enemies/Evil_Wizard_2/Attack1.png",       // 2
		"./enemies/Evil_Wizard_2/Attack2.png",       // 3
		"./enemies/Evil_Wizard_2/Death.png",		 // 4
		"./enemies/Evil_Wizard_2/Fall.png",			 // 5
		"./enemies/Evil_Wizard_2/Run.png",			 // 6
		"./enemies/Evil_Wizard_2/Take hit.png",      // 7

		}
	};
	void assign_boss_enemy_info(string enemytype, int posx, int posy, int killzone, int attackpow1, int attackpow2, int attpause, int hp) {
		enemy_type = enemytype;
		if (enemytype == "Boss1")
		{
			num_of_cur_textures = 6;
			cur_enemy_idx = 0;
			state = "idle";
			health = hp;
			sprite.setScale(3.14, 3.14); // cuz i love pi and i love people who love pi ;) , PS. nobody loves you
			left_boundary = posx - killzone; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + killzone;
			kill_zone = killzone;
			attack1 = attackpow1; //current boss power
			attack2 = attackpow2;
			attack_pause_time = attpause;// current pause time between every two attacks
			load_boss_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1;
		}
		else if (enemytype == "Boss2")
		{
			num_of_cur_textures = 8;
			cur_enemy_idx = 1;
			state = "idle";
			health = hp;
			sprite.setScale(4, 4);
			left_boundary = posx - killzone; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + killzone + 100;
			kill_zone = killzone;
			attack1 = attackpow1; //current boss power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_boss_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1;
		}
	}

	bool is_player_in_range_x()
	{ // checking if the character is in our boundaries
		return zone2.getGlobalBounds().intersects(knight.collisionRect.getGlobalBounds());
	}
	bool is_player_in_attack_zone() { // checking if the character is in our boundaries  

		return zone1.getGlobalBounds().intersects(knight.collisionRect.getGlobalBounds());
	}
	bool is_knight_sword_touching() { // checking if the sword of knight touching the character
		is_attacked = false;
		float diff = rect.left - knight.rect.left;
		float width = 0, diff2 = rect.top - knight.rect.top;
		if (enemy_type == "Boss2")
			diff += 400, width = 150;
		if (knight.lastKeyPressed == 1) // knight is facing right
		{
			if (-25 <= diff && diff <= 240 + width /*&& (-25 <= diff2 && diff2 <= 240)*/)
				is_attacked = true;
		}
		else // knight is facing left
		{
			if (enemy_type == "Boss2")
				width = 240, diff -= 100;
			if (-100 - width <= diff && diff <= 110  /* && (-100 <= diff2 && diff2 <= 110)*/)
				is_attacked = true;
		}
		return is_attacked && (knight.state == "Attack" || knight.state == "AttackCombo" || knight.state == "Attack2");

	}



	void load_boss_enemy_textures() {
		stateTexture = new Texture[num_of_cur_textures];
		for (int i = 0; i < num_of_cur_textures; i++)
			stateTexture[i].loadFromFile(enemy_textures[cur_enemy_idx][i]);
	}

	float turn_time = 7, pause_time = 0;
	void update_boss1_state(float time)
	{
		currentFrame += 0.045 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[5]);
			if (currentFrame >= 19)
				dead = true; // player dies after playing the full animation
		}
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (is_player_in_attack_zone())
			state = "attack";
		else if (is_player_in_range_x())
			state = "walk";
		else
			state = "idle";


		if (state == "walk")
		{
			kill_zone = 1500;
			sprite.setTexture(stateTexture[1]);
			if (currentFrame >= 4)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x + 50) // walks towards the player (if the player is left or right)
					dir = 1;
				else
					dir = -1;
				left_boundary = rect.getPosition().x - kill_zone;
				right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)
			}
			else if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
			{
				dir *= -1;
				turn_time = 7;
			}
		}
		else if (state == "attack")
		{
			pause_time -= time; // pause time between every two hits, first hit's pause time = 0
			if (pause_time <= 0)
			{
				if (currentFrame >= 6)
				{
					currentFrame = 0;
					skill_shift %= 5;
					skill_shift++;   // increment attacking times 
				}
				if (skill_shift >= 4)
					sprite.setTexture(stateTexture[3]);
				else
					sprite.setTexture(stateTexture[2]);

				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift <= 4)
				{
					if (currentFrame == 0)
						knight.health -= attack1;

				}
				else if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift > 4)
				{
					if (currentFrame == 0)
						knight.health -= attack2;

				}
				pause_time = attack_pause_time;
			}

		}
		else if (state == "on hit") {
			sprite.setTexture(stateTexture[4]);
			if (currentFrame >= 4)
			{
				currentFrame = 0;
				state = "";
				health -= knight.attack;
			}

		}
		else if (state == "idle")
		{
			sprite.setTexture(stateTexture[0]);
			if (knight.rect.getPosition().x + 45 >= rect.getPosition().x)
				dir = -1;
			else
				dir = 1;
			if (currentFrame >= 4)
			{
				currentFrame = 0;
			}
		}
		if (dir > 0)
			sprite.setTextureRect(IntRect(100 * int(currentFrame), 0, 100, 100)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
		else
			sprite.setTextureRect(IntRect(100 * int(currentFrame) + 100, 0, -100, 100)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)

	}

	void update_boss2_state(float time)
	{
		currentFrame += 0.045 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[4]);
			if (currentFrame >= 7)
				dead = true; // player dies after playing the full animation
		}
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (is_player_in_attack_zone())
			state = "attack";
		else if (is_player_in_range_x())
			state = "walk";
		else
			state = "idle";


		if (state == "walk")
		{
			sprite.setTexture(stateTexture[6]);
			if (currentFrame >= 8)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x + 200) // walks towards the player (if the player is left or right)
					dir = 1;
				else
					dir = -1;
				left_boundary = rect.getPosition().x - kill_zone;
				right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)
			}
			else if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
			{
				dir *= -1;
				turn_time = 7;
			}
		}
		else if (state == "attack")
		{
			pause_time -= time; // pause time between every two hits, first hit's pause time = 0
			if (pause_time <= 0)
			{
				if (currentFrame >= 8)
				{
					currentFrame = 0;
					skill_shift %= 5;
					skill_shift++;   // increment attacking times 
				}
				if (skill_shift >= 4)
					sprite.setTexture(stateTexture[3]);
				else
					sprite.setTexture(stateTexture[2]);

				//if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
				//	dir = -1;
				//else
				//	dir = 1;
				//left_boundary = rect.getPosition().x - kill_zone;
				//right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)
				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift <= 4)
				{
					if (currentFrame == 0)
					{
						//knight.health -= attack1;
					}
				}
				else if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift > 4)
				{
					if (currentFrame == 0)
					{
						//knight.health -= attack2;
					}
				}
				pause_time = attack_pause_time;
			}
		}
		else if (state == "on hit") {
			sprite.setTexture(stateTexture[7]);
			if (currentFrame >= 3)
			{
				currentFrame = 0;
				state = "";
				health -= knight.attack;
			}
		}
		else if (state == "idle")
		{
			sprite.setTexture(stateTexture[0]);
			if (knight.rect.getPosition().x + 45 >= rect.getPosition().x)
				dir = -1;
			else
				dir = 1;
			if (currentFrame >= 8)
			{
				currentFrame = 0;
			}
		}
		if (dir > 0)
			sprite.setTextureRect(IntRect(250 * int(currentFrame), 0, 250, 250)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
		else
			sprite.setTextureRect(IntRect(250 * int(currentFrame) + 250, 0, -250, 250)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
	}
}executioner, EvilBoss;

void setBools()
{
	Skeleton_1.dead = false;
	Skeleton_2.dead = false;
	Skeleton_3.dead = false;
	Skeleton_4.dead = false;
	Skeleton_5.dead = false;
	Evil_Wizard_1.dead = false;
	Evil_Wizard_2.dead = false;
	Evil_Wizard_3.dead = false;
	Evil_Wizard_4.dead = false;

	Skeleton_6.dead = false;
	Skeleton_7.dead = false;
	Skeleton_8.dead = false;
	Evil_Wizard_5.dead = false;
	Evil_Wizard_6.dead = false;
	Evil_Wizard_7.dead = false;
	Evil_Wizard_8.dead = false;
}

struct pauseMenu
{
	Font pauseFont;

	bool paused = false;
	Texture pauseMenuTexture;
	Sprite pauseMenuBg;



	Text pauseElements[3];
	string elements[3] = { "Resume", "Store", "Quit" };
	int selected = -1;


	void PauseMenufunc(float width, float height)
	{

		pauseFont.loadFromFile("menu/Pixelated.ttf");
		pauseMenuTexture.loadFromFile("menu/menuPicdark.jpg");
		pauseMenuBg.setTexture(pauseMenuTexture);
		clickbuffer.loadFromFile("menu/ButtonClick.wav");
		clicksound.setBuffer(clickbuffer);


		if (!pauseFont.loadFromFile("menu/Pixelated.ttf"))
		{
			cerr << "Font did not load" << endl;
			// Handle error loading font
		}

		for (int i = 0; i < 3; i++)
		{
			FloatRect boundary = pauseElements[i].getLocalBounds();
			pauseElements[i].setFont(pauseFont);
			pauseElements[i].setString(elements[i]);
			pauseElements[i].setCharacterSize(90);
			pauseElements[i].setFillColor(Color::White);
			//ignore the following ,but dont delete plz
			//pauseElements[i].setOrigin(boundary.left + boundary.width / 2, boundary.top + boundary.height / 2);
			//pauseElements[i].setPosition(Vector2f((width / 2), (height / 4) + (i * 150)));
			pauseElements[i].setOrigin(pauseElements[i].getLocalBounds().width / 2, pauseElements[i].getLocalBounds().height / 2);
			pauseElements[i].setPosition(Vector2f(width / 2, (height / 4) + (i * 150)));
		}
	}



	void show(RenderWindow& window)
	{
		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}

				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Up)
					{
						moveUp();
					}

					if (event.key.code == Keyboard::Down)
					{
						moveDown();
					}

					if (event.key.code == Keyboard::Enter)
					{
						clicksound.play();

						if (selected == 0)
						{
							clicksound.play();
							paused = false; //resume
							return;
						}

						if (selected == 1)
						{
							store(window);
						}

						if (selected == 2)
						{
							pageNum = 1;
							paused = false; //resume
							knight.assignSprite();
							pausedtimes = 0;
							setBools();
							//currentScene = 0;   //if we want to restart progress after quitting.
							return;
						}
					}
				}

			}

			window.clear();
			window.draw(pauseMenuBg);
			for (int i = 0; i < 3; i++)
			{
				window.draw(pauseElements[i]);
			}
			window.display();
		}
	}


	int pressed()
	{
		return selected;
	}


	void moveUp()
	{
		if (selected - 1 >= -1)
		{
			pauseElements[selected].setFillColor(Color::White);
			pauseElements[selected].setCharacterSize(90);
			selected--;
			sleep(milliseconds(200));
			if (selected == -1)
				selected = 2;
			pauseElements[selected].setFillColor(Color{ 192, 192, 192 });
			pauseElements[selected].setCharacterSize(80);
		}
	}

	void moveDown()
	{
		if (selected + 1 <= 3)
		{
			pauseElements[selected].setFillColor(Color::White);
			pauseElements[selected].setCharacterSize(90);
			selected++;
			sleep(milliseconds(200));
			if (selected == 3)
				selected = 0;
			pauseElements[selected].setFillColor(Color{ 192, 192, 192 });
			pauseElements[selected].setCharacterSize(80);
		}
	}
}pauseMenu;

// level 1 map code
struct LevelOne 
{
	int currentScene = 0;
	int noOFEnemies = 0;
	Sprite backgroundSprite;
	Texture levelTextures[6];


	void loadTextures() {
		string texturesArr[6] = { "external/level1/p1.png", "external/level1/p2.png","external/level1/p3.png", "external/level1/p4.png", "external/level1/p5.png", "external/level1/p6.png" };

		for (int i = 0; i < 6; i++) {
			levelTextures[i].loadFromFile(texturesArr[i]);
		}
	}

	// set character to Ground
	void setOnGround()
	{
		knight.onGround = true;
		knight.moveY = 0;
	}

	// used in any collision from bottom
	void bottomCollision()
	{
		knight.onGround = false;
		knight.moveY = 0.1;
	}

	// used to change background sprite and create & place tiles rectangles in the correct place
	void placeScene() {
		backgroundSprite.setTexture(levelTextures[currentScene]);


		if (currentScene == 0 || currentScene == 1 || currentScene == 2 || currentScene == 3 || currentScene == 4)
		{
			ground = 1300;
		}

		// placing each scene tiles and doors
		if (currentScene == 0) {
			currentTiles.resize(12);

			for (int i = 0; i < 12; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			if (pausedtimes == 0)
			{
				knight.rect.left = 5;
				knight.rect.top = 500;
				//put the mobs initializations here
				//Skeleton_1.rect.left = 670;
				//Skeleton_1.rect.top = 700;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}




			currentTiles[0].setSize(Vector2f(545, 55));
			currentTiles[0].setPosition(0, 995);

			currentTiles[1].setSize(Vector2f(95, 65.5));
			currentTiles[1].setPosition(545, 885);

			currentTiles[2].setSize(Vector2f(570, 40));
			currentTiles[2].setPosition(640, 835);

			currentTiles[3].setSize(Vector2f(120, 45));
			currentTiles[3].setPosition(1280, 670);

			currentTiles[4].setSize(Vector2f(165, 65));
			currentTiles[4].setPosition(1020, 550);

			currentTiles[5].setSize(Vector2f(130, 70));
			currentTiles[5].setPosition(830, 495);

			currentTiles[6].setSize(Vector2f(770, 50));
			currentTiles[6].setPosition(0, 390);

			currentTiles[7].setSize(Vector2f(95, 45));
			currentTiles[7].setPosition(1440, 565);

			currentTiles[8].setSize(Vector2f(125, 50));
			currentTiles[8].setPosition(1570, 500);

			currentTiles[9].setSize(Vector2f(190, 595));
			currentTiles[9].setPosition(1730, 460);

			currentTiles[10].setSize(Vector2f(190, 40));
			currentTiles[10].setPosition(1730, 185);

			currentTiles[11].setSize(Vector2f(55, 195));
			currentTiles[11].setPosition(1845, 265);


		}
		if (currentScene == 1)
		{


			currentTiles.resize(11);

			for (int i = 0; i < 11; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}



			if (pausedtimes == 0)
			{
				knight.rect.left = 3;
				knight.rect.top = 400;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			currentTiles[0].setSize(Vector2f(320, 50));
			currentTiles[0].setPosition(0, 785);

			currentTiles[1].setSize(Vector2f(320, 500));
			currentTiles[1].setPosition(0, 0);

			currentTiles[2].setSize(Vector2f(95, 50));
			currentTiles[2].setPosition(415, 770);

			currentTiles[3].setSize(Vector2f(125, 65));
			currentTiles[3].setPosition(610, 610);

			currentTiles[4].setSize(Vector2f(190, 400));
			currentTiles[4].setPosition(895, 665);

			// these 3 tiles are identical
			for (int i = 5; i <= 6; i++) {
				currentTiles[i].setSize(Vector2f(100, 65));
			}
			// end of identical tiles 

			currentTiles[5].setPosition(1215, 555);
			currentTiles[6].setPosition(1345, 445);



			currentTiles[7].setSize(Vector2f(450, 440));
			currentTiles[7].setPosition(1470, 285);

			currentTiles[8].setSize(Vector2f(515, 55));
			currentTiles[8].setPosition(1405, 10);

			currentTiles[9].setSize(Vector2f(770, 115));
			currentTiles[9].setPosition(1150, 940);

			currentTiles[10].setSize(Vector2f(55, 190));
			currentTiles[10].setPosition(1865, 760);

		}
		if (currentScene == 2)
		{
			currentTiles.resize(13);

			if (pausedtimes == 0)
			{
				knight.rect.left = -75;
				knight.rect.top = 600;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			for (int i = 0; i < 13; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(125, 40));
			currentTiles[0].setPosition(0, 955);

			currentTiles[1].setSize(Vector2f(125, 60));
			currentTiles[1].setPosition(0, 620);


			// index 2 , 3, 4, 7, 8, 10, 11 are identical blocks in size 
			for (int i = 2; i < 10; i++) {
				if (i == 2 || i == 3 || i == 4 || i == 7 || i == 8 || i == 9) {
					currentTiles[i].setSize(Vector2f(130, 55));
				}
			}

			currentTiles[2].setPosition(285, 885);
			currentTiles[3].setPosition(480, 775);
			currentTiles[4].setPosition(670, 665);

			currentTiles[5].setSize(Vector2f(325, 425));
			currentTiles[5].setPosition(925, 625);

			currentTiles[6].setSize(Vector2f(325, 335));
			currentTiles[6].setPosition(925, 10);

			currentTiles[7].setPosition(1310, 615);
			currentTiles[8].setPosition(1470, 725);
			currentTiles[9].setPosition(1595, 900);


			currentTiles[10].setSize(Vector2f(130, 45));
			currentTiles[10].setPosition(1790, 900);

			currentTiles[11].setSize(Vector2f(130, 60));
			currentTiles[11].setPosition(1790, 565);

			currentTiles[12].setSize(Vector2f(55, 180));
			currentTiles[12].setPosition(1875, 705);
		}
		if (currentScene == 3)
		{

			if (pausedtimes == 0)
			{
				knight.rect.left = -10;
				knight.rect.top = 300;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			currentTiles.resize(8);



			for (int i = 0; i < 8; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}


			currentTiles[0].setSize(Vector2f(190, 375));
			currentTiles[0].setPosition(0, 680);

			currentTiles[1].setSize(Vector2f(190, 445));
			currentTiles[1].setPosition(0, 10);

			currentTiles[2].setSize(Vector2f(575, 330));
			currentTiles[2].setPosition(190, 725);

			currentTiles[3].setSize(Vector2f(125, 60));
			currentTiles[3].setPosition(865, 885);

			currentTiles[4].setSize(Vector2f(865, 115));
			currentTiles[4].setPosition(1055, 940);

			currentTiles[5].setSize(Vector2f(195, 50));
			currentTiles[5].setPosition(1725, 690);

			currentTiles[6].setSize(Vector2f(320, 50));
			currentTiles[6].setPosition(1600, 580);

			currentTiles[7].setSize(Vector2f(55, 185));
			currentTiles[7].setPosition(1885, 760);

		}
		if (currentScene == 4)
		{
			currentTiles.resize(6);



			if (pausedtimes == 0)
			{
				knight.rect.top = 570;
				knight.rect.left = -10;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}


			for (int i = 0; i < 6; i++)
			{
				currentTiles[i].setFillColor(Color::Transparent);
			}


			currentTiles[0].setSize(Vector2f(255, 100));
			currentTiles[0].setPosition(0, 960);

			currentTiles[1].setSize(Vector2f(255, 725));
			currentTiles[1].setPosition(0, 10);

			currentTiles[2].setSize(Vector2f(355, 55));
			currentTiles[2].setPosition(285, 1005);

			// index 3, 4 are identical in size 
			for (int i = 3; i <= 4; i++)
			{
				currentTiles[i].setSize(Vector2f(70, 260));
			}

			currentTiles[3].setPosition(765, 890);
			currentTiles[4].setPosition(990, 730);

			currentTiles[5].setSize(Vector2f(705, 250));
			currentTiles[5].setPosition(1215, 625);
		}
		if (currentScene == 5)
		{

			if (!currentTiles.empty()) {
				currentTiles.erase(currentTiles.begin(), currentTiles.end());
			}


			if (pausedtimes == 0)
			{
				knight.rect.left = -125;
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			ground = 400;
		}
	}


	// detect collision of main character and tiles rectangels 
	void checkCollision(RectangleShape& collisionRect) {

		if (currentScene == 0) {
			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left > 1784) {
				knight.rect.left = 1636;
			}
			else if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}

			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// top collision
				setOnGround();
				knight.rect.top = currentTiles[0].getPosition().y - 295;
			}


			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[1].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[1].getPosition().x) {
					knight.rect.left = currentTiles[1].getPosition().x - 275;
				}
				// top collision
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[1].getPosition().y) {
					setOnGround();
					knight.rect.top = currentTiles[1].getPosition().y - 293;
				}


			}

			// tile 3
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
				//right collision
				if (collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 555 || collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 555.5) {
					knight.moveY = 0.75;
				}
				// left collision
				else if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[2].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[2].getPosition().x)) {
					knight.rect.left = currentTiles[2].getPosition().x - 275;
				}
				// top collision
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) {
					setOnGround();
					knight.rect.top = currentTiles[2].getPosition().y - 293;
				}
			}

			// tile 4
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
				// top collision with considering the correct position of character 
				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[3].getPosition().y + 15) && (collisionRect.getGlobalBounds().left > 1200)) {
					setOnGround();
					knight.rect.top = currentTiles[3].getPosition().y - 293;
				}
			}

			// tile 5 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
				// right collision
				if (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 155 || collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 156) {
					knight.rect.left = currentTiles[4].getPosition().x + 20.5;
				}


				//left collision
				else if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[4].getPosition().x)) {
					knight.rect.left = currentTiles[4].getPosition().x - 275;
				}

				// top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[4].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[4].getPosition().y + 10) {
					setOnGround();

				}

				// bottom
				else if ((collisionRect.getGlobalBounds().top <= currentTiles[4].getPosition().y + 65) && (collisionRect.getGlobalBounds().left >= 910) && (collisionRect.getGlobalBounds().left < 1125)) {
					bottomCollision();
				}
			}

			// tile 6
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				//left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 275;
				}

				// right collision
				else if (collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 117 || collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 118) {
					knight.rect.left = currentTiles[5].getPosition().x - 16.5;
				}

				// top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 10) {
					setOnGround();

				}

				// bottom collision
				else if ((collisionRect.getGlobalBounds().top <= currentTiles[5].getPosition().y + 70)) {
					bottomCollision();
				};
			}

			// tile 7 (long one on top)
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {

				// right collision	
				if (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 750 || collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 751) {
					knight.rect.left = currentTiles[6].getPosition().x + 620;
				}
				// top collision
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[6].getPosition().y + 10)) {
					setOnGround();
				}

			}

			// tile 8 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 115 <= currentTiles[7].getPosition().x) || collisionRect.getGlobalBounds().left + 116 <= currentTiles[7].getPosition().x) {
					knight.rect.left = currentTiles[7].getPosition().x - 271.5;
				}

				//top collision 
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[7].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 9 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 115 <= currentTiles[8].getPosition().x) || collisionRect.getGlobalBounds().left + 116 <= currentTiles[8].getPosition().x) {
					knight.rect.left = currentTiles[8].getPosition().x - 271.5;
				}

				//top collision 
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[8].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[8].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 10
			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[9].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[9].getPosition().x) {
					knight.rect.left = currentTiles[9].getPosition().x - 273.5;
				}

				// top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[9].getPosition().y) {
					knight.rect.top = currentTiles[9].getPosition().y - 293;
					setOnGround();
				}
			}

			// tile 11 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[10].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[10].getPosition().x) {
					knight.rect.left = currentTiles[9].getPosition().x - 273.5;
				}

				// bottom collision
				else if ((collisionRect.getGlobalBounds().top <= currentTiles[10].getPosition().y + 30)) {
					bottomCollision();
				}
			}

			// door : tile 12
			if (collisionRect.getGlobalBounds().intersects(currentTiles[11].getGlobalBounds()) && noOFEnemies == 0) {
				currentScene++;
				pausedtimes = 0;
				placeScene();
			}
		}
		if (currentScene == 1) {
			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left > 1800) {
				knight.rect.left = 1636;
			}
			else if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}
			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// right collision
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 319) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 319.5)) {
					knight.rect.left = currentTiles[0].getPosition().x + 172;
				}

				// top collision
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10) {
					setOnGround();
				}

			}

			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
				// right collision
				if ((collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 308) || (collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 309)) {
					knight.rect.left = currentTiles[1].getPosition().x + 175;
				}

				// bottom collision
				else if (collisionRect.getGlobalBounds().top <= currentTiles[1].getPosition().y + 490) {
					bottomCollision();
				}


			}

			// tile 3 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 15) && (collisionRect.getGlobalBounds().left > 335) && (collisionRect.getGlobalBounds().left < 497)) {
					setOnGround();
				}

				else if (collisionRect.getGlobalBounds().left >= 497) {
					knight.moveY = 0.3;
				}
			}

			// tile 4 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
				// left collision 
				if ((collisionRect.getGlobalBounds().left + 118 <= currentTiles[3].getPosition().x) || (collisionRect.getGlobalBounds().left + 119 <= currentTiles[3].getPosition().x)) {
					knight.rect.left = currentTiles[3].getPosition().x - 271.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 114) || (collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 115)) {
					knight.rect.left = currentTiles[3].getPosition().x - 23;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[3].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 5
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 105 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 106 <= currentTiles[4].getPosition().x)) {
					knight.rect.left = currentTiles[4].getPosition().x - 271.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 177) || (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 178)) {
					knight.rect.left = currentTiles[4].getPosition().x + 43.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[4].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 6
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				// left collision	
				if ((collisionRect.getGlobalBounds().left + 118 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 119 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
				}

				// right collision 
				else if ((collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 97) || (collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 98)) {
					knight.rect.left = currentTiles[5].getPosition().x - 46;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[5].getPosition().y + 15)) {
					setOnGround();
				}


				// bottom  collsion 
				else if (collisionRect.getGlobalBounds().top > currentTiles[5].getPosition().y + 50) {
					bottomCollision();
				}
			}

			// tile 7
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				// left collision	
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x)) {
					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[6].getPosition().y + 15)) {
					setOnGround();
				}

			}

			// tile 8
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
				//  left collision
				if ((collisionRect.getGlobalBounds().left + 105 <= currentTiles[7].getPosition().x) || (collisionRect.getGlobalBounds().left + 106 <= currentTiles[7].getPosition().x)) {

					knight.rect.left = currentTiles[7].getPosition().x - 271.5;
				}

				// top collision 
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[7].getPosition().y + 15)) {
					setOnGround();
				}

				// bottom  collsion 
				else if (collisionRect.getGlobalBounds().top > currentTiles[7].getPosition().y + 427.5) {
					bottomCollision();
				}

			}

			// tile 9
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[8].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[8].getPosition().x)) {
					knight.rect.left = currentTiles[8].getPosition().x - 271.5;
				}

				// bottom  collsion 
				else if (collisionRect.getGlobalBounds().top > currentTiles[8].getPosition().y + 50) {
					bottomCollision();
				}

			}

			// tile 10
			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds())) {
				if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[9].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[9].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// door : tile 11
			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds()) && noOFEnemies == 0) {


				currentScene++;
				knight.rect.left = -10;
				knight.rect.top = 600;
				placeScene();
			}
		}
		if (currentScene == 2) {

			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left > 1784) {
				knight.rect.left = 1636;
			}
			else if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}

			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// top collison
				if (collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10) {
					setOnGround();
				}
			}

			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {

				//right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 123) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 124)) {
					knight.rect.left = currentTiles[1].getPosition().x - 21.5;
				}


				// top collison
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[1].getPosition().y) && collisionRect.getGlobalBounds().top + 145 < currentTiles[1].getPosition().y + 10) {
					setOnGround();
				}

				// bottom collison
				else if ((collisionRect.getGlobalBounds().top > currentTiles[1].getPosition().y + 50)) {
					bottomCollision();
				}
			}

			// tiles : 3, 4, 5, 8, 9, 10,
			// index : 2, 3, 4, 7, 8, 9  
			// these tiles shares same size so they have same collison (at least base ones : left & top )
			for (int i = 2; i <= 9; i++)
			{
				if (i == 2 || i == 3 || i == 4 || i == 7 || i == 8 || i == 9) {
					if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
						// left collision
						if ((collisionRect.getGlobalBounds().left + 118 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 119 <= currentTiles[i].getPosition().x)) {
							knight.rect.left = currentTiles[i].getPosition().x - 271.5;
						}

						// right collision
						else if (((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 125) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 126))) {
							knight.rect.left = currentTiles[i].getPosition().x - 16.5;
						}

						// top collision
						else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[i].getPosition().y + 20)) {
							setOnGround();
						}
					}
				}
			}

			// tile 6
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				// top collsion
				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 10)) {
					setOnGround();
				}

				// left collision 
				else if ((collisionRect.getGlobalBounds().left + 118.5 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 119 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
				}

			}

			// tile 7
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				// left collison
				if (((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x))) {
					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
				}

				// right collision 
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 323) || (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 324)) {
					knight.rect.left = currentTiles[6].getPosition().x + 180;
				}

				// bottom collsion 
				else if ((collisionRect.getGlobalBounds().top > currentTiles[6].getPosition().y + 325)) {
					bottomCollision();
				}
			}

			// tile 11
			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds())) {
				// top collison
				if (collisionRect.getGlobalBounds().top + 145 > currentTiles[10].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[10].getPosition().y + 10) {
					setOnGround();
				}

			}

			// tile 12
			if (collisionRect.getGlobalBounds().intersects(currentTiles[11].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[11].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[11].getPosition().x)) {
					knight.rect.left = currentTiles[11].getPosition().x - 271.5;
				}

				// top collision
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[11].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[11].getPosition().y + 10) {
					setOnGround();
				}

				// bottom collision
				else if (collisionRect.getGlobalBounds().top > currentTiles[11].getPosition().y + 55) {
					bottomCollision();
				}
			}

			// tile 13 : Door
			if (collisionRect.getGlobalBounds().intersects(currentTiles[12].getGlobalBounds())) {

				currentScene++;
				placeScene();
			}
		}
		if (currentScene == 3) {
			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left > 1784) {
				knight.rect.left = 1636;
			}
			else if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}

			// tile 1 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 189)) {
					knight.rect.left = currentTiles[0].getPosition().x + 43.5;
				}

				// top collsion
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 5)) {
					setOnGround();
				}
			}

			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {

				// right collsion
				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 189)) {
					knight.rect.left = currentTiles[1].getPosition().x + 43.5;
				}

				// bottom collision
				else if (collisionRect.getGlobalBounds().top > currentTiles[1].getPosition().y + 440) {
					bottomCollision();
				}
			}

			// tile 3
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
				// right collison
				if ((collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 565) || (collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 566)) {
					knight.rect.left = currentTiles[2].getPosition().x + 428.5;
				}

				// top collsion 
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 10)) {
					setOnGround();
				}
			}

			// tile 4
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds())) {
				// right collison
				if ((collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 119) || (collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 120)) {
					knight.rect.left = currentTiles[3].getPosition().x - 20.5;
				}

				// top collsion 
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[3].getPosition().y + 10)) {
					setOnGround();
				}
			}

			// tile 5
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
				// top collision
				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[4].getPosition().y + 10)) {
					setOnGround();
				}
			}

			// tile 6 : above the door
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				//left collison
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
				}

				// bottom collision
				else if (collisionRect.getGlobalBounds().top > currentTiles[5].getPosition().y + 35) {
					bottomCollision();
				}
			}

			// tile 7
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				//left collison
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[6].getPosition().x)) {
					knight.rect.left = currentTiles[6].getPosition().x - 271.5;
				}

				// bottom collision
				else if (collisionRect.getGlobalBounds().top > currentTiles[6].getPosition().y + 35) {
					bottomCollision();
				}
			}

			// tile 8
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
				currentScene++;
				placeScene();
			}
		}
		if (currentScene == 4) {
			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}


			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getGlobalBounds().getPosition().x + 253) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getGlobalBounds().getPosition().x + 254)) {
					knight.rect.left = currentTiles[0].getGlobalBounds().getPosition().x + 108.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[0].getPosition().y + 10)) {
					setOnGround();
				}

			}

			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
				// right collision
				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getGlobalBounds().getPosition().x + 253) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getGlobalBounds().getPosition().x + 254)) {
					knight.rect.left = currentTiles[1].getGlobalBounds().getPosition().x + 108.5;
				}


				// bottom collision
				else {
					bottomCollision();
				}
			}

			// tile 3
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds())) {
				if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[2].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 4 , 5 : identical tiles in collision
			for (int i = 3; i <= 4; i++)
			{

				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
					// right collision
					if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 67) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 68)) {
						knight.rect.left = currentTiles[i].getPosition().x - 76.5;
					}

					// left collision
					else if ((collisionRect.getGlobalBounds().left + 115 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 116 <= currentTiles[i].getPosition().x)) {
						knight.rect.left = currentTiles[i].getPosition().x - 271.5;
					}

					// top collision
					else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[i].getPosition().y + 15)) {
						setOnGround();
					}
				}
			}

			// tile 6
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 115 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 116 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// next scene detection
			if (collisionRect.getGlobalBounds().left >= 1790) {
				currentScene++;
				placeScene();
			}
		}
		if (currentScene == 5) {
			// screen left and right boundaries collision

			if (collisionRect.getGlobalBounds().left < 4) {
				knight.rect.left = -145;
			}
			else if (collisionRect.getGlobalBounds().left > 1820) {
				knight.rect.left = 1668;
			}
		}
	}
} levelOneMap;

// level 2 map code
struct levelTwo {
	int currentScene = 0;
	int noOFEnemies = 0;
	Sprite backgroundSprite;
	Texture levelTextures[6];


	void loadTextures() {
		string texturesArr[6] = { "./external/level2/Forest P1.png", "./external/level2/Forest P2.png","./external/level2/Forest P3.png", "./external/level2/Forest P4.png",
								  "./external/level2/Forest P5.png", "./external/level2/Forest P6.png" };

		for (int i = 0; i < 6; i++) {
			levelTextures[i].loadFromFile(texturesArr[i]);
		}
	}

	void onGround() {
		knight.moveY = 0;
		knight.onGround = true;
	}

	void bottomCollision() {
		knight.onGround = false;
		knight.moveY = 0.1;
	}

	void jump() {
		knight.moveY = -0.9;
		knight.onGround = false;
		knight.state = "Jump";
	}

	// used to change background sprite and create & place tiles rectangles in the correct place
	void placeScene() {
		backgroundSprite.setTexture(levelTextures[currentScene]);

		if (currentScene == 0 || currentScene == 1 || currentScene == 2 || currentScene == 3 || currentScene == 4) {
			ground = 1300;
		}

		if (currentScene == 0) {
			currentTiles.resize(5);

			if (pausedtimes == 0)
			{
				knight.rect.left = 5;
				knight.rect.top = 510;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}


			for (int i = 0; i < 5; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(425, 75));
			currentTiles[0].setPosition(10, 810);

			currentTiles[1].setSize(Vector2f(420, 75));
			currentTiles[1].setPosition(590, 970);

			currentTiles[2].setSize(Vector2f(425, 70));
			currentTiles[2].setPosition(1480, 970);

			currentTiles[3].setSize(Vector2f(85, 75));
			currentTiles[3].setPosition(950, 855);

			currentTiles[4].setSize(Vector2f(10, 150));
			currentTiles[4].setPosition(1910, 830);
		}

		if (currentScene == 1) {
			currentTiles.resize(9);

			if (pausedtimes == 0)
			{
				knight.rect.left = 5;
				knight.rect.top = 510;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			for (int i = 0; i < 9; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(310, 70));
			currentTiles[0].setPosition(15, 820);

			currentTiles[1].setSize(Vector2f(180, 80));
			currentTiles[1].setPosition(450, 915);

			currentTiles[2].setSize(Vector2f(85, 75));
			currentTiles[2].setPosition(500, 790);

			currentTiles[3].setSize(Vector2f(180, 80));
			currentTiles[3].setPosition(865, 685);

			currentTiles[4].setSize(Vector2f(85, 75));
			currentTiles[4].setPosition(915, 565);

			currentTiles[5].setSize(Vector2f(180, 80));
			currentTiles[5].setPosition(1380, 960);

			currentTiles[6].setSize(Vector2f(85, 75));
			currentTiles[6].setPosition(1430, 845);

			currentTiles[7].setSize(Vector2f(170, 75));
			currentTiles[7].setPosition(1735, 470);

			currentTiles[8].setSize(Vector2f(10, 150));
			currentTiles[8].setPosition(1910, 270);
		}

		if (currentScene == 2) {
			currentTiles.resize(8);

			if (pausedtimes == 0)
			{
				knight.rect.left = 10;
				knight.rect.top = 250;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			for (int i = 0; i < 8; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(250, 470));
			currentTiles[0].setPosition(0, 580);

			currentTiles[1].setSize(Vector2f(65, 65));
			currentTiles[1].setPosition(420, 475);

			currentTiles[2].setSize(Vector2f(65, 65));
			currentTiles[2].setPosition(615, 700);

			currentTiles[3].setSize(Vector2f(65, 65));
			currentTiles[3].setPosition(870, 730);

			currentTiles[4].setSize(Vector2f(65, 65));
			currentTiles[4].setPosition(1125, 540);

			currentTiles[5].setSize(Vector2f(65, 65));
			currentTiles[5].setPosition(1440, 480);

			currentTiles[6].setSize(Vector2f(175, 75));
			currentTiles[6].setPosition(1725, 640);

			currentTiles[7].setSize(Vector2f(10, 150));
			currentTiles[7].setPosition(1910, 500);
		}

		if (currentScene == 3) {
			currentTiles.resize(10);

			if (pausedtimes == 0)
			{
				knight.rect.top = 345;
				knight.rect.left = 25;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			for (int i = 0; i < 10; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(255, 70));
			currentTiles[0].setPosition(0, 650);

			currentTiles[1].setSize(Vector2f(65, 450));
			currentTiles[1].setPosition(420, 480);

			currentTiles[2].setSize(Vector2f(65, 450));
			currentTiles[2].setPosition(615, 700);

			currentTiles[3].setSize(Vector2f(65, 450));
			currentTiles[3].setPosition(870, 700);

			currentTiles[4].setSize(Vector2f(65, 450));
			currentTiles[4].setPosition(1125, 525);

			currentTiles[5].setSize(Vector2f(65, 350));
			currentTiles[5].setPosition(1445, 0);

			currentTiles[6].setSize(Vector2f(225, 70));
			currentTiles[6].setPosition(1310, 650);

			currentTiles[7].setSize(Vector2f(260, 65));
			currentTiles[7].setPosition(1660, 650);

			currentTiles[8].setSize(Vector2f(60, 110));
			currentTiles[8].setPosition(1440, 960);

			currentTiles[9].setSize(Vector2f(10, 150));
			currentTiles[9].setPosition(1930, 500);
		}

		if (currentScene == 4) {
			currentTiles.erase(currentTiles.begin(), currentTiles.end());
			currentTiles.resize(8);

			if (pausedtimes == 0)
			{
				knight.rect.left = -30;
				knight.rect.top = 300;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}

			for (int i = 0; i < 8; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			currentTiles[0].setSize(Vector2f(425, 80));
			currentTiles[0].setPosition(35, 700);

			currentTiles[1].setSize(Vector2f(425, 80));
			currentTiles[1].setPosition(515, 510);

			currentTiles[2].setSize(Vector2f(200, 75));
			currentTiles[2].setPosition(1050, 330);

			currentTiles[3].setSize(Vector2f(200, 75));
			currentTiles[3].setPosition(1335, 685);

			currentTiles[4].setSize(Vector2f(315, 75));
			currentTiles[4].setPosition(965, 865);

			currentTiles[5].setSize(Vector2f(315, 70));
			currentTiles[5].setPosition(550, 1020);

			currentTiles[6].setSize(Vector2f(345, 85));
			currentTiles[6].setPosition(1575, 870);

			currentTiles[7].setSize(Vector2f(150, 30));
			currentTiles[7].setPosition(1920, 700);
		}

		if (currentScene == 5) {
			currentTiles.erase(currentTiles.begin(), currentTiles.end());

			if (pausedtimes == 0)
			{
				knight.rect.left = -40;
				knight.rect.top = 553;
				//put the mobs initializations here
			}
			else
			{
				knight.rect.left = knight.rect.getPosition().x;
				knight.rect.top = knight.rect.getPosition().y;
			}
			ground = 555;
		}
	}

	// detect collision of main character and tiles rectangels 
	void checkCollision(RectangleShape& collisionRect) {
		if (currentScene == 0) {
			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds()))
			{
				// right collision
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 423) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 424)) {
					knight.rect.left = currentTiles[0].getPosition().x + 270;
				}

				// top colliison
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[0].getPosition().y + 20)) {
					onGround();
				}
			}


			// tile 2
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))
			{
				// left colision 
				if ((collisionRect.getGlobalBounds().left + 92 <= currentTiles[1].getPosition().x) || (collisionRect.getGlobalBounds().left + 93 <= currentTiles[1].getPosition().x)) {
					knight.rect.left = currentTiles[1].getPosition().x - 255.5;
				}

				// right collision 
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 410) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 411)) {
					knight.rect.left = currentTiles[1].getPosition().x + 261;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[1].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[1].getPosition().y + 20)) {
					onGround();
				}


			}

			// tile 3
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds()))
			{
				// left collision
				if ((collisionRect.getGlobalBounds().left + 92 <= currentTiles[2].getPosition().x) || (collisionRect.getGlobalBounds().left + 93 <= currentTiles[2].getPosition().x)) {
					knight.rect.left = currentTiles[2].getPosition().x - 255.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[2].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[2].getPosition().y + 20)) {
					onGround();
				}
			}


			// tile 4 : jumper 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds()))
			{
				jump();
			}

			// tile 5 : next scene move
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds()))
			{
				currentScene++;
				pausedtimes = 0;
				placeScene();
			}
		}

		if (currentScene == 1) {

			// tile 0
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds()))
			{
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 300) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 301)) {
					knight.rect.left = currentTiles[0].getPosition().x + 151.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[0].getPosition().y + 20)) {
					onGround();
				}
			}


			// tile 1
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))
			{
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 170) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 171)) {
					knight.rect.left = currentTiles[1].getPosition().x + 21.5;
				}

				// left colision 
				else if ((collisionRect.getGlobalBounds().left + 92 <= currentTiles[1].getPosition().x) || (collisionRect.getGlobalBounds().left + 93 <= currentTiles[1].getPosition().x)) {
					knight.rect.left = currentTiles[1].getPosition().x - 255.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[1].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[1].getPosition().y + 20)) {
					onGround();
				}

			}

			// tile 2 : jumper
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds()))
			{
				jump();
			}

			// tile 3
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds()))
			{
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 170) || (collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 171)) {
					knight.rect.left = currentTiles[3].getPosition().x + 21.5;
				}

				// left colision 
				else if ((collisionRect.getGlobalBounds().left + 90 <= currentTiles[3].getPosition().x) || (collisionRect.getGlobalBounds().left + 91 <= currentTiles[3].getPosition().x)) {
					knight.rect.left = currentTiles[3].getPosition().x - 255.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[3].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[3].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 4 : jumper
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds()))
			{
				jump();
			}

			// tile 5
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds()))
			{
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 170) || (collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 171)) {
					knight.rect.left = currentTiles[5].getPosition().x + 21.5;
				}

				// left collision
				else if ((collisionRect.getGlobalBounds().left + 90 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 91 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 255.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[5].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 6 : jumper
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds()))
			{
				jump();
			}

			// tile 7 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				// left collision
				if ((collisionRect.getGlobalBounds().left + 90 <= currentTiles[7].getPosition().x) || (collisionRect.getGlobalBounds().left + 91 <= currentTiles[7].getPosition().x)) {
					knight.rect.left = currentTiles[7].getPosition().x - 255.5;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[7].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 8 : move to next scene
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds()))
			{
				currentScene++;
				pausedtimes = 0;
				placeScene();
			}
		}

		if (currentScene == 2)
		{

			// tile 1 :
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 240) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 241)) {
					knight.rect.left = currentTiles[0].getPosition().x + 91.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[0].getPosition().y + 20)) {
					onGround();
				}
			}

			// tiles : 2 , 3, 4 ,5 ,6
			// index : 1, 2, 3, 4, 5

			for (int i = 1; i <= 5; i++)
			{
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
					// left collision
					if ((collisionRect.getGlobalBounds().left + 89 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 90 <= currentTiles[i].getPosition().x)) {
						knight.rect.left = currentTiles[i].getPosition().x - 255.5;
					}

					// right collision 
					else if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 53) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 54)) {
						knight.rect.left = currentTiles[i].getPosition().x - 93.5;
					}

					// top collision
					else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[i].getPosition().y + 20)) {
						onGround();
					}
				}
			}

			// tile 7
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 89 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 90 <= currentTiles[6].getPosition().x)) {
					knight.rect.left = currentTiles[6].getPosition().x - 255.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[6].getPosition().y + 20)) {
					onGround();
				}
			}


			// tile 8 : move to next scene
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				currentScene++;
				pausedtimes = 0;
				knight.rect.top = 0;
				knight.rect.left = 50;
				placeScene();
			}
		}

		if (currentScene == 3) {

			// tile 1 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// right collision 
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 242) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 243)) {
					knight.rect.left = currentTiles[0].getPosition().x + 93.5;
				}

				// top collision
				if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[0].getPosition().y + 20)) {
					onGround();
				}
			}

			// tiles : 2, 3, 4, 5, 6
			// index : 1, 2, 3, 4, 5
			for (int i = 1; i <= 5; i++) {
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
					// left collision
					if ((collisionRect.getGlobalBounds().left + 87 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 88 <= currentTiles[i].getPosition().x)) {
						knight.rect.left = currentTiles[i].getPosition().x - 255.5;
					}

					// right collision
					else if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 54) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 55)) {
						knight.rect.left = currentTiles[i].getPosition().x - 93.5;
					}

					if (i != 5) {
						// top collision
						if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[i].getPosition().y + 20)) {
							onGround();
						}
					}
					else { // top tile
						// bottom collision
						if (collisionRect.getGlobalBounds().top > currentTiles[i].getPosition().y + 342) {
							bottomCollision();
						}
					}

				}
			}

			// tile 7
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[6].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[6].getPosition().x)) {
					knight.rect.left = currentTiles[6].getPosition().x - 255.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 215) || (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 216)) {
					knight.rect.left = currentTiles[6].getPosition().x + 66.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[6].getPosition().y + 20)) {
					onGround();
				}
				// bottom collision
				else if (collisionRect.getGlobalBounds().top > currentTiles[6].getPosition().y + 69) {
					bottomCollision();
				}
			}


			// tile 8
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[7].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[7].getPosition().x)) {
					knight.rect.left = currentTiles[7].getPosition().x - 255.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[7].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 9 : bottom
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
				// right collision :
				if ((collisionRect.getGlobalBounds().left >= currentTiles[8].getPosition().x + 50) || (collisionRect.getGlobalBounds().left >= currentTiles[8].getPosition().x + 51)) {
					knight.rect.left = currentTiles[8].getPosition().x - 98.5;;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[8].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[8].getPosition().y + 20)) {
					onGround();
				}
			}


			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds()))
			{
				knight.rect.top = 0;
				knight.rect.left = 25;
				pausedtimes = 0;
				currentScene++;

				placeScene();
			}
		}

		if (currentScene == 4) {
			// tile 1 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds())) {
				// right collision
				if ((collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 414) || (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + 415)) {
					knight.rect.left = currentTiles[0].getPosition().x + 266.6;
				}


				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[0].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[0].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 2 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds())) {
				// right collision
				if ((collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 414) || (collisionRect.getGlobalBounds().left >= currentTiles[1].getPosition().x + 415)) {
					knight.rect.left = currentTiles[1].getPosition().x + 266.6;
				}

				// left collision
				else if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[1].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[1].getPosition().x)) {
					knight.rect.left = currentTiles[1].getPosition().x - 255.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[1].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[1].getPosition().y + 20)) {
					onGround();
				}
			}

			// tiles : 3, 4
			// index : 2, 3
			for (int i = 2; i <= 3; i++)
			{
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds())) {
					// right collision
					if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 190) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 191)) {
						knight.rect.left = currentTiles[i].getPosition().x + 41.5;
					}

					// left collision
					else if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[i].getPosition().x)) {
						knight.rect.left = currentTiles[i].getPosition().x - 255.5;
					}

					// top collision
					else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[i].getPosition().y + 20)) {
						onGround();
					}

					else if (i == 2) {
						// bottom collision
						if (collisionRect.getGlobalBounds().top > currentTiles[i].getPosition().y + 65) {
							bottomCollision();
						}
					}
				}
			}

			// tile 5
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[4].getPosition().x)) {
					knight.rect.left = currentTiles[4].getPosition().x - 255.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[4].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 6 : bottom 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 88 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 89 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 255.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 305) || (collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 306)) {
					knight.rect.left = currentTiles[5].getPosition().x + 156.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[5].getPosition().y + 20)) {
					onGround();
				}
			}

			// tile 7 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds())) {
				// top collision
				if ((collisionRect.getGlobalBounds().top + 154 > currentTiles[6].getPosition().y) && (collisionRect.getGlobalBounds().top + 154 < currentTiles[6].getPosition().y + 20)) {
					onGround();
				}
			}

			// move to next scene
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				pausedtimes = 0;
				currentScene++;
				placeScene();
			}
		}
	}
} levelTwoMap;

void healthBox(RenderWindow& window) {

	Texture healthIcon;
	healthIcon.loadFromFile("Store/Textures/heart2.png");

	Sprite healthSprite;
	healthSprite.setTexture(healthIcon);
	healthSprite.setPosition(32.5, 30);

	RectangleShape healthOutline;
	healthOutline.setFillColor(Color::Transparent);
	healthOutline.setOutlineThickness(3);
	healthOutline.setPosition(80, 50);
	healthOutline.setSize(Vector2f(350, 30));

	RectangleShape healthBar;
	healthBar.setFillColor(Color(229, 57, 53));
	healthBar.setPosition(healthOutline.getPosition().x, healthOutline.getPosition().y);
	healthBar.setSize(Vector2f(knight.health * 3.5, healthOutline.getSize().y));



	window.draw(healthOutline);
	window.draw(healthBar);
	window.draw(healthSprite);
}

void exechealthBox(RenderWindow& window) 
{

	Texture healthIcon;
	healthIcon.loadFromFile("Store/Textures/darkheart.png");

	Sprite healthSprite;
	healthSprite.setTexture(healthIcon);
	healthSprite.setPosition(571, 249);

	RectangleShape healthOutline;
	healthOutline.setFillColor(Color::Transparent);
	healthOutline.setOutlineThickness(5);
	healthOutline.setOutlineColor(Color::Black);
	healthOutline.setPosition(616, 269);
	healthOutline.setSize(Vector2f(700, 30));

	RectangleShape healthinline;
	healthOutline.setFillColor(Color::Black);
	/*healthOutline.setOutlineThickness(5);
	healthOutline.setOutlineColor(Color::Black);*/
	healthOutline.setPosition(616, 269);
	healthOutline.setSize(Vector2f(700, 30));

	RectangleShape healthBar;
	healthBar.setFillColor(Color(136, 8, 8));
	healthBar.setPosition(healthOutline.getPosition().x, healthOutline.getPosition().y);
	healthBar.setSize(Vector2f(executioner.health * 3.5, healthOutline.getSize().y));



	window.draw(healthOutline);
	window.draw(healthBar);
	window.draw(healthSprite);
}

void coinBox(RenderWindow& window) {

	Texture coinIcon;
	coinIcon.loadFromFile("Store/Textures/coin.png");

	Sprite coinSprite;
	coinSprite.setTexture(coinIcon);
	coinSprite.setPosition(1620, 30);

	// font
	Font coinFont;
	coinFont.loadFromFile("menu/Pixelated.ttf");

	coinFile.open("coinFile.txt", ios::in);
	if (coinFile.is_open())
	{
		string temp;
		while (getline(coinFile, temp))
		{
			storeCoins = stoi(temp);
		}
		coinFile.close();
	}

	// text
	Text coinValue;
	coinValue.setFont(coinFont);
	coinValue.setString(to_string(storeCoins));
	coinValue.setFillColor(Color::White);
	coinValue.setCharacterSize(60);
	coinValue.setPosition(1710, 30);


	window.draw(coinValue);
	window.draw(coinSprite);
}

void movements();

void levelOne(RenderWindow& window);

void levelTwo(RenderWindow& window);


struct deathMenu
{
	Font deathFont;

	bool booldead = false;
	Texture deathMenuTexture;
	Sprite deathMenuBg;



	Text deathElements[2], youDied;
	string elements[2] = { "Restart", "Quit" };
	int selected = -1;


	void deathMenufunc(float width, float height)
	{

		deathFont.loadFromFile("menu/Pixelated.ttf");
		deathMenuTexture.loadFromFile("menu/deathMenuPic.jpg");
		deathMenuBg.setTexture(deathMenuTexture);
		clickbuffer.loadFromFile("menu/ButtonClick.wav");
		clicksound.setBuffer(clickbuffer);


		youDied.setFont(deathFont);
		youDied.setFillColor(Color::White);
		youDied.setString("You Died");
		youDied.setCharacterSize(150);
		youDied.setOrigin(youDied.getLocalBounds().width / 2, youDied.getLocalBounds().height / 2);
		youDied.setPosition(Vector2f(width / 2, 200));


		if (!deathFont.loadFromFile("menu/Pixelated.ttf"))
		{
			cerr << "Font did not load" << endl;
			// Handle error loading font
		}

		for (int i = 0; i < 2; i++)
		{
			FloatRect boundary = deathElements[i].getLocalBounds();
			deathElements[i].setFont(deathFont);
			deathElements[i].setString(elements[i]);
			deathElements[i].setCharacterSize(85);
			deathElements[i].setFillColor(Color::White);
			deathElements[i].setOrigin(deathElements[i].getLocalBounds().width / 2, deathElements[i].getLocalBounds().height / 2);
			deathElements[i].setPosition(Vector2f(width / 2, (height / 4) + (i * 150) + 250));
		}
	}



	void show(RenderWindow& window, int& currentScene)
	{
		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}

				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Up)
					{
						moveUp();
					}

					if (event.key.code == Keyboard::Down)
					{
						moveDown();
					}

					if (event.key.code == Keyboard::Enter)
					{
						clicksound.play();

						if (selected == 0) //restart
						{
							clicksound.play();
							booldead = false; //resume
							knight.assignSprite();
							pausedtimes = 0;
							currentScene = 0;
							knight.dead = false;
							setBools();
							if (pageNum == 5)
							{
								return;
							}
							else if (pageNum == 6)
							{
								if (executioner.dead)
								{
									
									levelTwo(window);
								}
								else
								{
									levelOne(window);
								}
							}

							return;
						}

						if (selected == 1) //quit
						{
							knight.dead = false;
							clicksound.play();
							pageNum = 1;
							booldead = false; //resume
							knight.assignSprite();
							pausedtimes = 0;
							currentScene = 0;
							setBools();
							return;
						}
					}
				}

			}

			window.clear();
			window.draw(deathMenuBg);
			window.draw(youDied);
			for (int i = 0; i < 3; i++)
			{
				window.draw(deathElements[i]);
			}
			window.display();
		}
	}


	int pressed()
	{
		return selected;
	}


	void moveUp()
	{
		if (selected - 1 >= -1)
		{
			deathElements[selected].setFillColor(Color::White);
			deathElements[selected].setCharacterSize(90);
			selected--;
			sleep(milliseconds(200));
			if (selected == -1)
				selected = 1;
			deathElements[selected].setFillColor(Color{ 192, 192, 192 });
			deathElements[selected].setCharacterSize(80);
		}
	}

	void moveDown()
	{
		if (selected + 1 <= 2)
		{
			deathElements[selected].setFillColor(Color::White);
			deathElements[selected].setCharacterSize(90);
			selected++;
			sleep(milliseconds(200));
			if (selected == 2)
				selected = 0;
			deathElements[selected].setFillColor(Color{ 192, 192, 192 });
			deathElements[selected].setCharacterSize(80);
		}
	}
}deathMenu;

struct completedMenu
{
	Font completedFont;

	Texture completedMenuTexture;
	Sprite completedMenuBg;



	Text completedElements[2], TheForestOfDreams;
	string elements[2] = { "Continue", "Quit" };
	int selected = -1;


	void completedMenufunc(float width, float height)
	{

		completedFont.loadFromFile("menu/Pixelated.ttf");
		completedMenuTexture.loadFromFile("menu/deathMenuPic.jpg");
		completedMenuBg.setTexture(completedMenuTexture);
		clickbuffer.loadFromFile("menu/ButtonClick.wav");
		clicksound.setBuffer(clickbuffer);


		TheForestOfDreams.setFont(completedFont);
		TheForestOfDreams.setFillColor(Color::White);
		TheForestOfDreams.setString("Welcome To The Forest Of Dreams");
		TheForestOfDreams.setCharacterSize(100);
		TheForestOfDreams.setOrigin(TheForestOfDreams.getLocalBounds().width / 2, TheForestOfDreams.getLocalBounds().height / 2);
		TheForestOfDreams.setPosition(Vector2f(width / 2, 200));


		if (!completedFont.loadFromFile("menu/Pixelated.ttf"))
		{
			cerr << "Font did not load" << endl;
			// Handle error loading font
		}

		for (int i = 0; i < 2; i++)
		{
			FloatRect boundary = completedElements[i].getLocalBounds();
			completedElements[i].setFont(completedFont);
			completedElements[i].setString(elements[i]);
			completedElements[i].setCharacterSize(85);
			completedElements[i].setFillColor(Color::White);
			completedElements[i].setOrigin(completedElements[i].getLocalBounds().width / 2, completedElements[i].getLocalBounds().height / 2);
			completedElements[i].setPosition(Vector2f(width / 2, (height / 4) + (i * 150) + 250));
		}
	}



	void show(RenderWindow& window, int& currentScene)
	{
		while (window.isOpen())
		{
			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}

				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Up)
					{
						moveUp();
					}

					if (event.key.code == Keyboard::Down)
					{
						moveDown();
					}

					if (event.key.code == Keyboard::Enter)
					{
						clicksound.play();

						if (selected == 0) //Continue
						{
							clicksound.play();
							knight.assignSprite();
							pausedtimes = 0;
							currentScene = 0;
							knight.dead = false;
							setBools();
							return;
						}

						if (selected == 1) //quit
						{
							knight.dead = false;
							clicksound.play();
							pageNum = 1;
							knight.assignSprite();
							pausedtimes = 0;
							currentScene = 0;
							setBools();
							return;
						}
					}
				}

			}

			window.clear();
			window.draw(completedMenuBg);
			window.draw(TheForestOfDreams);
			for (int i = 0; i < 3; i++)
			{
				window.draw(completedElements[i]);
			}
			window.display();
		}
	}


	int pressed()
	{
		return selected;
	}


	void moveUp()
	{
		if (selected - 1 >= -1)
		{
			completedElements[selected].setFillColor(Color::White);
			completedElements[selected].setCharacterSize(90);
			selected--;
			sleep(milliseconds(200));
			if (selected == -1)
				selected = 1;
			completedElements[selected].setFillColor(Color{ 192, 192, 192 });
			completedElements[selected].setCharacterSize(80);
		}
	}

	void moveDown()
	{
		if (selected + 1 <= 2)
		{
			completedElements[selected].setFillColor(Color::White);
			completedElements[selected].setCharacterSize(90);
			selected++;
			sleep(milliseconds(200));
			if (selected == 2)
				selected = 0;
			completedElements[selected].setFillColor(Color{ 192, 192, 192 });
			completedElements[selected].setCharacterSize(80);
		}
	}
}completedMenu;

int main()
{
	knight.assignSprite(); // Initialize player character

	RenderWindow window(VideoMode(1920, 1080), "X: Cursed World!");
	window.setFramerateLimit(144);
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
						if (event.key.code == Keyboard::Up)
							menu.moveUp();

						if (event.key.code == Keyboard::Down)
							menu.moveDown();

						if (event.key.code == Keyboard::Enter) {
							clickSound.play();
							if (menu.pressed() == 0)
								pageNum = 6;
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
		if (pageNum == 2)
		{
			menu.modeMenu.modeMenuFunc(1920, 1080);

			Texture modeMenuPic;
			modeMenuPic.loadFromFile("menu/menuPic.jpg");
			Sprite modeMenuSprite;
			modeMenuSprite.setTexture(modeMenuPic);



			Event event;
			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
					break;
				}
				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Right)
						menu.modeMenu.moveRight();
					if (event.key.code == Keyboard::Left)
						menu.modeMenu.moveLeft();
					if (event.key.code == Keyboard::Enter)
					{
						clickSound.play();
						if (menu.modeMenu.pressed() == 0)
						{
							pageNum = 5;
							break;
						}
						if (menu.modeMenu.pressed() == 1)
						{
							pageNum = 6;
							break;
						}

					}
					if (event.key.code == Keyboard::Escape)
					{  // return back to the main menu page
						clickSound.play();
						pageNum = 1;
						menu.menuElement[0].setFillColor(Color::White);
						menu.menuElement[0].setCharacterSize(90);
						menu.selected = -1;
						break;
					}
				}

				// Update
				window.clear();
				window.draw(modeMenuSprite);
				menu.modeMenu.draw(window);
				window.display();
			}

		}
		if (pageNum == 3) {
			store(window);
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				clickSound.play();
				pageNum = 1;
				menu.menuElement[1].setFillColor(Color::White);
				menu.menuElement[1].setCharacterSize(90);
			}
		}
		if (pageNum == 4) {
			window.close();
			delete[] Skeleton_1.stateTexture;
			delete[] Skeleton_2.stateTexture;
			delete[] Skeleton_3.stateTexture;
			delete[] Skeleton_4.stateTexture;
			delete[] Skeleton_5.stateTexture;
			delete[] Skeleton_6.stateTexture;
			delete[] Skeleton_7.stateTexture;
			delete[] Skeleton_8.stateTexture;
			delete[] Evil_Wizard_1.stateTexture;
			delete[] Evil_Wizard_2.stateTexture;
			delete[] Evil_Wizard_3.stateTexture;
			delete[] Evil_Wizard_4.stateTexture;
			delete[] Evil_Wizard_5.stateTexture;
			delete[] Evil_Wizard_6.stateTexture;
			delete[] Evil_Wizard_7.stateTexture;
			delete[] Evil_Wizard_8.stateTexture;
			delete[] executioner.stateTexture;
			delete[] EvilBoss.stateTexture;
			
			break;
		}
		if (pageNum == 5)
		{
			Clock clock;

			Texture arcadebackgroundTexture;
			Sprite backgroundSprite;
			arcadebackgroundTexture.loadFromFile("external/background1.png");
			backgroundSprite.setTexture(arcadebackgroundTexture);
			pauseMenu.PauseMenufunc(1920, 1080);

			int pageNum = 0; // Initialize pageNum to control the game flow
			bool paused = false;
			ground = 600;

			while (window.isOpen())
			{
				// Handle events
				Event event;
				while (window.pollEvent(event))
				{


					if (event.type == Event::Closed)
					{
						window.close();
					}

					if (event.type == Event::KeyPressed)
					{
						if (event.key.code == Keyboard::Escape)
						{
							clickSound.play();
							knight.rect.left = knight.rect.getPosition().x;
							knight.rect.top = knight.rect.getPosition().y;
							pauseMenu.paused = true;
						}
					}
				}

				// Update game logic

				// Clear the window
				window.clear();

				if (!pauseMenu.paused)
				{
					movements();
					float time = (float)clock.getElapsedTime().asMicroseconds();
					clock.restart();
					time /= 650;
					if (time > 20)
						time = 20;
					knight.update(time);
				}
				else
				{

					pauseMenu.show(window);
					break;
				}

				// Draw game elements
				window.draw(backgroundSprite);
				window.draw(knight.sprite);



				// Display the window

				window.display();
			}

		}
		if (pageNum == 6)
		{
			if (executioner.dead) /////////////////
			{
				levelTwo(window);
			}
			else
			{
				levelOne(window);
			}
		}
	}

	return 0;
}

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
			knight.moveY = -0.6;
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
		if (knight.noOfAttacks % 2 == 1) {
			knight.state = "Attack"; knight.noOfAttacks++;
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

void store(RenderWindow& window)
{
	//RenderWindow window(VideoMode(1920, 1080), "Store", Style::Fullscreen);

	// Load font of store
	Font storeFont;
	if (!storeFont.loadFromFile("menu/Pixelated.ttf"))
	{
		cerr << "Error loading font file" << "/n";
	}

	Text displayCoinText("", storeFont, 50);

	// Text of storeBanner
	Text textBanner;
	textBanner.setFont(storeFont);
	textBanner.setString("Store");
	textBanner.setFillColor(Color::White);
	textBanner.setCharacterSize(50);
	textBanner.setPosition(885, 45);

	// Load primary textures
	Texture background, storeBadge, powerBoard, infoBoard, infoChain, powerChain, button, coinTexture;
	if (!background.loadFromFile("Store/Textures/background.png") ||
		!storeBadge.loadFromFile("Store/Textures/storebanner.png") ||
		!powerBoard.loadFromFile("Store/Textures/boardofpowerups.png") ||
		!infoBoard.loadFromFile("Store/Textures/boardofinfo.png") ||
		!infoChain.loadFromFile("Store/Textures/infochain.png") ||
		!powerChain.loadFromFile("Store/Textures/chain.png") ||
		!button.loadFromFile("Store/Textures/button.png") ||
		!coinTexture.loadFromFile("Store/Textures/coin.png"))
	{
		cerr << "Error loading primary textures files" << "/n";
	}

	SoundBuffer clickBuffer;
	clickBuffer.loadFromFile("menu/ButtonClick.wav");
	Sound clickSound;
	clickSound.setBuffer(clickBuffer);

	// Create primary sprites
	Sprite storeBackground(background), storeBanner(storeBadge), boardOfPowers(powerBoard),
		boardOfInfo(infoBoard), chainOfInfo(infoChain), chainOne(powerChain), chainTwo(powerChain), coinsText(coinTexture);

	// Set positions for primary sprites
	storeBanner.setPosition(760, 25);
	boardOfPowers.setPosition(1500, 220);
	boardOfInfo.setPosition(40, 365);
	chainOfInfo.setPosition(0, 280);
	chainOne.setPosition(1540, 0);
	chainTwo.setPosition(1830, 0);
	coinsText.setPosition(240, 10);
	coinsText.setScale(0.7, 0.7);

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

		perks[i].priceTexture.setTexture(coinTexture);
		perks[i].priceTexture.setPosition(-1000, -1000);
		perks[i].priceTexture.setScale(0.5, 0.5);

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

		perks[i].info.setFont(storeFont);
		perks[i].info.setFillColor(Color::White);
		perks[i].info.setCharacterSize(40);
		perks[i].info.setPosition(-1000, -1000);

		// Create perks sprites
		// position of Perks
		switch (i)
		{
		case 0:
			perks[i].action.setTexture(sword);
			perks[i].action.setPosition(1570, 250);
			perks[i].price.setString("100");
			perks[i].info.setString("Increase\nAttack\nDamage\nby 25%");
			break;
		case 1:
			perks[i].action.setTexture(resis);
			perks[i].action.setPosition(1710, 250);
			perks[i].price.setString("80");
			perks[i].info.setString("Increase\nResistence\nby 25%");
			break;
		case 2:
			perks[i].action.setTexture(heart);
			perks[i].action.setPosition(1570, 350);
			perks[i].price.setString("120");
			perks[i].info.setString("Increase\nHealth\nby 25%");
			break;
		}
	}

	powerUp1File.open("powerUp1File.txt", ios::in);
	if (powerUp1File.is_open())
	{
		string temp;
		while (getline(powerUp1File, temp))
		{
			upgradeCheck[0] = stoi(temp);
		}
		powerUp1File.close();
	}

	powerUp2File.open("powerUp2File.txt", ios::in);
	if (powerUp2File.is_open())
	{
		string temp;
		while (getline(powerUp2File, temp))
		{
			upgradeCheck[1] = stoi(temp);
		}
		powerUp2File.close();
	}

	powerUp3File.open("powerUp3File.txt", ios::in);
	if (powerUp3File.is_open())
	{
		string temp;
		while (getline(powerUp3File, temp))
		{
			upgradeCheck[2] = stoi(temp);
		}
		powerUp3File.close();
	}

	if (upgradeCheck[0] == 0) sword.loadFromFile("Store/Textures/sword1.png");
	if (upgradeCheck[0] == 1) sword.loadFromFile("Store/Textures/sword2.png");
	if (upgradeCheck[0] == 2) sword.loadFromFile("Store/Textures/sword3.png");
	if (upgradeCheck[0] == 3) sword.loadFromFile("Store/Textures/sword4.png");
	if (upgradeCheck[0] == 4) sword.loadFromFile("Store/Textures/sword5.png");

	if (upgradeCheck[1] == 0) resis.loadFromFile("Store/Textures/resis1.png");
	if (upgradeCheck[1] == 1) resis.loadFromFile("Store/Textures/resis2.png");
	if (upgradeCheck[1] == 2) resis.loadFromFile("Store/Textures/resis3.png");
	if (upgradeCheck[1] == 3) resis.loadFromFile("Store/Textures/resis4.png");
	if (upgradeCheck[1] == 4) resis.loadFromFile("Store/Textures/resis5.png");

	if (upgradeCheck[2] == 0) heart.loadFromFile("Store/Textures/heart1.png");
	if (upgradeCheck[2] == 1) heart.loadFromFile("Store/Textures/heart2.png");
	if (upgradeCheck[2] == 2) heart.loadFromFile("Store/Textures/heart3.png");
	if (upgradeCheck[2] == 3) heart.loadFromFile("Store/Textures/heart4.png");
	if (upgradeCheck[2] == 4) heart.loadFromFile("Store/Textures/heart5.png");

	checkCoinsFile.open("checkCoinsFile.txt", ios::in);
	if (checkCoinsFile.is_open())
	{
		string temp;
		while (getline(checkCoinsFile, temp))
		{
			check = stoi(temp);
		}
		checkCoinsFile.close();
	}

	if (check == 0)
	{
		totalCoins = "300";
		coinFile.open("coinFile.txt", ios::out);
		if (coinFile.is_open())
		{
			coinFile << totalCoins;
			coinFile.close();
		}
		coinFile.open("coinFile.txt", ios::in);
		if (coinFile.is_open())
		{
			string temp;
			while (getline(coinFile, temp))
			{
				storeCoins = stoi(temp);
			}
			coinFile.close();
		}
	}
	else
	{
		coinFile.open("coinFile.txt", ios::in);
		if (coinFile.is_open())
		{
			string temp;
			while (getline(coinFile, temp))
			{
				storeCoins = stoi(temp);
			}
			coinFile.close();
		}
	}

	while (window.isOpen())
	{
		// the mouse position on window
		Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

		Event storeEvent;
		while (window.pollEvent(storeEvent))
		{
			if (storeEvent.type == Event::Closed)
				window.close();

			if (storeEvent.type == Event::KeyPressed)
			{
				if (storeEvent.key.code == Keyboard::Escape)
				{
					if (pageNum == 5) {
						pageNum = 5;
						return;
					}
					else if (pageNum == 6) {
						pageNum = 6;
						return;
					}
					else {
						pageNum = 1;
						return;

					}
				}
			}

			displayCoinText.setString("coins: " + to_string(storeCoins));

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
					tempCheck = "1";
					checkCoinsFile.open("checkCoinsFile.txt", ios::out);
					if (checkCoinsFile.is_open())
					{
						checkCoinsFile << tempCheck;
						checkCoinsFile.close();
					}
					if (upgradeCheck[0] < 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
						perks[0].upgradeButton.setPosition(1550, 800);
						clickSound.play();
						perks[0].upgradeText.setPosition(1607, 827);
						perks[0].price.setPosition(160, 640);
						perks[0].priceTexture.setPosition(235, 650);
						perks[0].info.setPosition(100, 400);

						if (upgradeCheck[0] == 1) perks[0].price.setString("150");
						if (upgradeCheck[0] == 2) perks[0].price.setString("200");
						if (upgradeCheck[0] == 3) perks[0].price.setString("250");
					}
					else if (upgradeCheck[0] == 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of sword hit test
				if (perks[0].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[0] == 0)
					{
						if (storeCoins >= 100)
						{
							tempPowerUP = "1";
							powerUp1File.open("powerUp1File.txt", ios::out);
							if (powerUp1File.is_open())
							{
								powerUp1File << tempPowerUP;
								powerUp1File.close();
							}
							sword.loadFromFile("Store/Textures/sword2.png");
							perks[0].price.setString("150");
							clickSound.play();
							storeCoins -= 100;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[0]++;
						}
					}
					else if (upgradeCheck[0] == 1)
					{
						if (storeCoins >= 150)
						{
							tempPowerUP = "2";
							powerUp1File.open("powerUp1File.txt", ios::out);
							if (powerUp1File.is_open())
							{
								powerUp1File << tempPowerUP;
								powerUp1File.close();
							}
							sword.loadFromFile("Store/Textures/sword3.png");
							perks[0].price.setString("200");
							clickSound.play();
							storeCoins -= 150;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[0]++;
						}
					}
					else if (upgradeCheck[0] == 2)
					{
						if (storeCoins >= 200)
						{
							tempPowerUP = "3";
							powerUp1File.open("powerUp1File.txt", ios::out);
							if (powerUp1File.is_open())
							{
								powerUp1File << tempPowerUP;
								powerUp1File.close();
							}
							sword.loadFromFile("Store/Textures/sword4.png");
							perks[0].price.setString("250");
							clickSound.play();
							storeCoins -= 200;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[0]++;
						}
					}
					else if (upgradeCheck[0] == 3)
					{
						if (storeCoins >= 250)
						{
							tempPowerUP = "4";
							powerUp1File.open("powerUp1File.txt", ios::out);
							if (powerUp1File.is_open())
							{
								powerUp1File << tempPowerUP;
								powerUp1File.close();
							}
							sword.loadFromFile("Store/Textures/sword5.png");
							clickSound.play();
							storeCoins -= 250;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[0]++;
						}
					}
				}

				// the resis hit test
				if (perks[1].bounds.contains(mouse))
				{
					tempCheck = "1";
					checkCoinsFile.open("checkCoinsFile.txt", ios::out);
					if (checkCoinsFile.is_open())
					{
						checkCoinsFile << tempCheck;
						checkCoinsFile.close();
					}
					if (upgradeCheck[1] < 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
						perks[1].upgradeButton.setPosition(1550, 800);
						clickSound.play();
						perks[1].upgradeText.setPosition(1607, 827);
						perks[1].price.setPosition(160, 640);
						perks[1].priceTexture.setPosition(235, 650);
						perks[1].info.setPosition(100, 400);

						if (upgradeCheck[1] == 1) perks[1].price.setString("120");
						if (upgradeCheck[1] == 2) perks[1].price.setString("160");
						if (upgradeCheck[1] == 3) perks[1].price.setString("200");
					}
					else if (upgradeCheck[1] == 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of resis hit test
				if (perks[1].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[1] == 0)
					{
						if (storeCoins >= 80)
						{
							tempPowerUP = "1";
							powerUp2File.open("powerUp2File.txt", ios::out);
							if (powerUp2File.is_open())
							{
								powerUp2File << tempPowerUP;
								powerUp2File.close();
							}
							resis.loadFromFile("Store/Textures/resis2.png");
							perks[1].price.setString("120");
							clickSound.play();
							storeCoins -= 80;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[1]++;
						}
					}
					else if (upgradeCheck[1] == 1)
					{
						if (storeCoins >= 120)
						{
							tempPowerUP = "2";
							powerUp2File.open("powerUp2File.txt", ios::out);
							if (powerUp2File.is_open())
							{
								powerUp2File << tempPowerUP;
								powerUp2File.close();
							}
							resis.loadFromFile("Store/Textures/resis3.png");
							perks[1].price.setString("160");
							clickSound.play();
							storeCoins -= 120;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[1]++;
						}
					}
					else if (upgradeCheck[1] == 2)
					{
						if (storeCoins >= 160)
						{
							tempPowerUP = "3";
							powerUp2File.open("powerUp2File.txt", ios::out);
							if (powerUp2File.is_open())
							{
								powerUp2File << tempPowerUP;
								powerUp2File.close();
							}
							resis.loadFromFile("Store/Textures/resis4.png");
							perks[1].price.setString("200");
							clickSound.play();
							storeCoins -= 160;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[1]++;
						}
					}
					else if (upgradeCheck[1] == 3)
					{
						if (storeCoins >= 200)
						{
							tempPowerUP = "4";
							powerUp2File.open("powerUp2File.txt", ios::out);
							if (powerUp2File.is_open())
							{
								powerUp2File << tempPowerUP;
								powerUp2File.close();
							}
							resis.loadFromFile("Store/Textures/resis5.png");
							clickSound.play();
							storeCoins -= 200;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[1]++;
						}
					}
				}

				// the heart hit test
				if (perks[2].bounds.contains(mouse))
				{
					tempCheck = "1";
					checkCoinsFile.open("checkCoinsFile.txt", ios::out);
					if (checkCoinsFile.is_open())
					{
						checkCoinsFile << tempCheck;
						checkCoinsFile.close();
					}
					if (upgradeCheck[2] < 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
						perks[2].upgradeButton.setPosition(1550, 800);
						clickSound.play();
						perks[2].upgradeText.setPosition(1607, 827);
						perks[2].price.setPosition(160, 640);
						perks[2].priceTexture.setPosition(235, 650);
						perks[2].info.setPosition(100, 400);

						if (upgradeCheck[2] == 1) perks[2].price.setString("170");
						if (upgradeCheck[2] == 2) perks[2].price.setString("220");
						if (upgradeCheck[2] == 3) perks[2].price.setString("270");
					}
					else if (upgradeCheck[2] == 4)
					{
						for (int i = 0; i < NUMBER_OF_PERKS; i++)
						{
							perks[i].upgradeButton.setPosition(-1000, -1000);
							perks[i].upgradeText.setPosition(-1000, -1000);
							perks[i].price.setPosition(-1000, -1000);
							perks[i].priceTexture.setPosition(-1000, -1000);
							perks[i].info.setPosition(-1000, -1000);
						}
					}
				}
				// the upgrade button of heart hit test
				if (perks[2].upgradeBounds.contains(mouse))
				{
					if (upgradeCheck[2] == 0)
					{
						if (storeCoins >= 120)
						{
							tempPowerUP = "1";
							powerUp3File.open("powerUp3File.txt", ios::out);
							if (powerUp3File.is_open())
							{
								powerUp3File << tempPowerUP;
								powerUp3File.close();
							}
							heart.loadFromFile("Store/Textures/heart2.png");
							perks[2].price.setString("170");
							clickSound.play();
							storeCoins -= 120;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[2]++;
						}
					}
					else if (upgradeCheck[2] == 1)
					{
						if (storeCoins >= 170)
						{
							tempPowerUP = "2";
							powerUp3File.open("powerUp3File.txt", ios::out);
							if (powerUp3File.is_open())
							{
								powerUp3File << tempPowerUP;
								powerUp3File.close();
							}
							heart.loadFromFile("Store/Textures/heart3.png");
							perks[2].price.setString("220");
							clickSound.play();
							storeCoins -= 170;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[2]++;
						}
					}
					else if (upgradeCheck[2] == 2)
					{
						if (storeCoins >= 220)
						{
							tempPowerUP = "3";
							powerUp3File.open("powerUp3File.txt", ios::out);
							if (powerUp3File.is_open())
							{
								powerUp3File << tempPowerUP;
								powerUp3File.close();
							}
							heart.loadFromFile("Store/Textures/heart4.png");
							perks[2].price.setString("270");
							clickSound.play();
							storeCoins -= 220;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[2]++;
						}
					}
					else if (upgradeCheck[2] == 3)
					{
						if (storeCoins >= 270)
						{
							tempPowerUP = "4";
							powerUp3File.open("powerUp3File.txt", ios::out);
							if (powerUp3File.is_open())
							{
								powerUp3File << tempPowerUP;
								powerUp3File.close();
							}
							heart.loadFromFile("Store/Textures/heart5.png");
							clickSound.play();
							storeCoins -= 270;
							totalCoins = to_string(storeCoins);
							coinFile.open("coinFile.txt", ios::out);
							if (coinFile.is_open())
							{
								coinFile << totalCoins;
								coinFile.close();
							}
							upgradeCheck[2]++;
						}
					}
				}
			}
		}
		// Rendering
		window.clear();
		window.draw(storeBackground);
		window.draw(storeBanner);
		window.draw(textBanner);
		window.draw(chainOfInfo);
		window.draw(boardOfInfo);
		window.draw(chainOne);
		window.draw(chainTwo);
		window.draw(boardOfPowers);
		for (int i = 0; i < NUMBER_OF_PERKS; i++)
		{
			window.draw(perks[i].action);
			window.draw(perks[i].price);
			window.draw(perks[i].upgradeButton);
			window.draw(perks[i].priceTexture);
			window.draw(perks[i].upgradeText);
			window.draw(perks[i].upgradeText);
			window.draw(perks[i].info);
		}
		window.draw(displayCoinText);
		window.draw(coinsText);
		window.display();
	}
}

void levelOne(RenderWindow& window)
{
	bool monsterCoins[8] = { 0 };
	

	Clock clock;

	pauseMenu.PauseMenufunc(1920, 1080);
	deathMenu.deathMenufunc(1920, 1080);
	completedMenu.completedMenufunc(1920, 1080);

	SoundBuffer clickbuffer;
	Sound clicksound;

	clickbuffer.loadFromFile("menu/ButtonClick.wav");
	clicksound.setBuffer(clickbuffer);

	levelOneMap.loadTextures();
	levelOneMap.placeScene();

	
	if (!executioner.dead)
		executioner.assign_boss_enemy_info("Boss1", 1475, 400, 1200, 10, 20, 2, 200);
	if (!Skeleton_1.dead)
		Skeleton_1.assign_sec_enemy_info("skeleton", 801, 645, 275, 10, 1, 100);
	if (!Skeleton_2.dead)
		Skeleton_2.assign_sec_enemy_info("skeleton", 200, 200, 314, 12, 1, 80);
	if (!Skeleton_3.dead)
		Skeleton_3.assign_sec_enemy_info("skeleton", 1475, 790, 200, 11, 1, 90);
	if (!Skeleton_4.dead)
		Skeleton_4.assign_sec_enemy_info("skeleton", 1587, 440, 200, 11, 1, 90);
	if (!Evil_Wizard_1.dead)
		Evil_Wizard_1.assign_sec_enemy_info("EvilWizard", 1475, 10, 150, 11, 2, 90);
	if (!Evil_Wizard_2.dead)
		Evil_Wizard_2.assign_sec_enemy_info("EvilWizard", 400, 475, 150, 11, 2, 120);
	if (!Evil_Wizard_3.dead)
		Evil_Wizard_3.assign_sec_enemy_info("EvilWizard", 1300, 695, 150, 11, 2, 130);
	while (window.isOpen())
	{

		knight.knightattck();

		Vector2f knightPos = knight.sprite.getPosition();
		// adjusting the collision rect to be more accurate 
		// redRect for collision detection
		knight.collisionRect = RectCreator(100, 145, knightPos.x + 150, knightPos.y + 150);
		Vector2f SPos1 = Skeleton_1.sprite.getPosition();
		Vector2f SPos2 = Skeleton_2.sprite.getPosition();
		Vector2f SPos3 = Skeleton_3.sprite.getPosition();
		Vector2f SPos4 = Skeleton_4.sprite.getPosition();
		Vector2f EPos1 = Evil_Wizard_1.sprite.getPosition();
		Vector2f EPos2 = Evil_Wizard_2.sprite.getPosition();
		Vector2f EPos3 = Evil_Wizard_3.sprite.getPosition();
		Vector2f ExecPos = executioner.sprite.getPosition();
		Skeleton_1.zone = RectCreator(170, 150, SPos1.x + 35, SPos1.y + 35);
		Skeleton_2.zone = RectCreator(170, 150, SPos2.x + 24, SPos2.y);
		Skeleton_3.zone = RectCreator(170, 150, SPos3.x + 24, SPos3.y);
		Skeleton_4.zone = RectCreator(170, 150, SPos4.x + 24, SPos4.y);
		Evil_Wizard_1.zone = RectCreator(200, 200, EPos1.x + 80, EPos1.y + 30);
		Evil_Wizard_2.zone = RectCreator(200, 200, EPos2.x + 80, EPos2.y + 30);
		Evil_Wizard_3.zone = RectCreator(200, 200, EPos3.x + 80, EPos3.y + 30);
		executioner.zone1 = RectCreator(200, 200, ExecPos.x + 80, ExecPos.y + 30);
		executioner.zone2 = RectCreator(2 * executioner.kill_zone, 300, executioner.left_boundary, ExecPos.y);


		if (knight.rect.getPosition().y > 1090)
		{

			knight.dead = true;
		}

		if ((Skeleton_2.state == "attack" || Skeleton_1.state == "attack") && levelOneMap.currentScene == 0)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Skeleton_1.attack) * 0.00711;
		}
		if ((Evil_Wizard_1.state == "attack" || Skeleton_3.state == "attack") && levelOneMap.currentScene == 1)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Skeleton_1.attack) * 0.00754;
		}
		if ((Evil_Wizard_2.state == "attack" || Evil_Wizard_3.state == "attack") && levelOneMap.currentScene == 3)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Skeleton_1.attack) * 0.00754;
		}
		if ((Skeleton_4.state == "attack") && levelOneMap.currentScene == 4)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Skeleton_1.attack) * 0.00711;
		}
		if ((executioner.state == "attack") && levelOneMap.currentScene == 5)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(executioner.attack1) * 0.011;
		}

		if (Skeleton_1.dead)
		{
			if (monsterCoins[0] == false)
			{
				monsterCoins[0] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_2.dead)
		{
			if (monsterCoins[1] == false)
			{
				monsterCoins[1] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_3.dead)
		{
			if (monsterCoins[2] == false)
			{
				monsterCoins[2] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_4.dead)
		{
			if (monsterCoins[3] == false)
			{
				monsterCoins[3] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_1.dead)
		{
			if (monsterCoins[4] == false)
			{
				monsterCoins[4] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_2.dead)
		{
			if (monsterCoins[5] == false)
			{
				monsterCoins[5] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_3.dead)
		{
			if (monsterCoins[6] == false)
			{
				monsterCoins[6] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (executioner.dead)
		{
			if (monsterCoins[7] == false)
			{
				monsterCoins[7] = true;
				storeCoins += 100;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}

		if (!knight.isAlive())
		{
			knight.state = "Death";
			knight.updateTexture();
		}
		// Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(window);
					cout << "MousePos x : " << mousePos.x << " MousePos y :  " << mousePos.y << endl;
					cout << "Red Rect Left : " << knight.collisionRect.getGlobalBounds().left << " Red Rect Top " << knight.collisionRect.getGlobalBounds().top << endl;
					cout << "blue rect left : " << knight.rect.left << "blue rect top : " << knight.rect.top << endl;
				}
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					knight.rect.left = knight.rect.getPosition().x;
					knight.rect.top = knight.rect.getPosition().y;
					pauseMenu.paused = true;
					pausedtimes++;
					clicksound.play();

				}
			}
		}



		// check player collision (always should be placed before movement fn to avoid silly animation bugs :)
		levelOneMap.checkCollision(knight.collisionRect);

		// Clear the window
		window.clear();


		if (!knight.dead)
		{
			if (executioner.dead)
			{
				completedMenu.show(window, levelTwoMap.currentScene);
				break;
			}
			if (!pauseMenu.paused)
			{
				movements();
				float time = (float)clock.getElapsedTime().asMicroseconds();
				clock.restart();
				time /= 650;
				if (time > 20)
					time = 20;
				window.draw(levelOneMap.backgroundSprite);
				healthBox(window);
				coinBox(window);
				if (!Skeleton_1.dead && levelOneMap.currentScene == 0)
				{
					Skeleton_1.update_skeleton_state(time);
					window.draw(Skeleton_1.sprite);
					window.draw(Skeleton_1.zone);
				}
				if (!Skeleton_2.dead && levelOneMap.currentScene == 0)
				{
					Skeleton_2.update_skeleton_state(time);
					window.draw(Skeleton_2.sprite);
					window.draw(Skeleton_2.zone);
				}
				if (!Skeleton_3.dead && levelOneMap.currentScene == 1)
				{
					Skeleton_3.update_skeleton_state(time);
					window.draw(Skeleton_3.sprite);
					window.draw(Skeleton_3.zone);
				}
				if (!Skeleton_4.dead && levelOneMap.currentScene == 4)
				{
					Skeleton_4.update_skeleton_state(time);
					window.draw(Skeleton_4.sprite);
					window.draw(Skeleton_4.zone);
				}
				if (!Evil_Wizard_1.dead && levelOneMap.currentScene == 1)
				{
					Evil_Wizard_1.update_evilwiz_state(time);
					window.draw(Evil_Wizard_1.sprite);
					window.draw(Evil_Wizard_1.zone);
				}
				if (!Evil_Wizard_2.dead && levelOneMap.currentScene == 3)
				{
					Evil_Wizard_2.update_evilwiz_state(time);
					window.draw(Evil_Wizard_2.sprite);
					window.draw(Evil_Wizard_2.zone);
				}
				if (!Evil_Wizard_3.dead && levelOneMap.currentScene == 3)
				{
					Evil_Wizard_3.update_evilwiz_state(time);
					window.draw(Evil_Wizard_3.sprite);
					window.draw(Evil_Wizard_3.zone);
				}
				if (!executioner.dead && levelOneMap.currentScene == 5)
				{
					executioner.update_boss1_state(time);
					window.draw(executioner.sprite);
					window.draw(executioner.zone1);
					window.draw(executioner.zone2);
					exechealthBox(window);
				}

				if (!knight.dead)
				{
					knight.update(time);
					window.draw(knight.collisionRect);
					window.draw(knight.sprite);
				}
			}
			else if (pauseMenu.paused)
			{
				pauseMenu.show(window);
				break;
			}
		}
		else if (knight.dead)
		{
			deathMenu.show(window, levelOneMap.currentScene);
			break;
		}

		for (int i = 0; i < currentTiles.size(); i++)
		{
			window.draw(currentTiles[i]);
		}



		window.display();
	}
}

void levelTwo(RenderWindow& window) 
{
	Clock clock;


	bool monsterCoins[10] = { 0 };

	pauseMenu.PauseMenufunc(1920, 1080);
	deathMenu.deathMenufunc(1920, 1080);

	SoundBuffer clickbuffer;
	Sound clicksound;

	clickbuffer.loadFromFile("menu/ButtonClick.wav");
	clicksound.setBuffer(clickbuffer);

	levelTwoMap.loadTextures();
	levelTwoMap.placeScene();

	if (!EvilBoss.dead)
		EvilBoss.assign_boss_enemy_info("Boss2", 1200, 175, 1200, 10, 20, 2, 300);

	//if (!Evil_Wizard_4.dead)
		//Evil_Wizard_4.assign_sec_enemy_info("EvilWizard", 580, 600, 150, 14, 2, 120, 1, 1, -0.05);

	if (!Skeleton_2.dead)
		Skeleton_2.assign_sec_enemy_info("skeleton", 1500, 770, 150, 17, 1, 100);

	if (!Evil_Wizard_1.dead)
		Evil_Wizard_1.assign_sec_enemy_info("EvilWizard", 1600, 100, 100, 20, 2, 150, 0.5, 0.8, -0.05);

	if (!Skeleton_4.dead)
		Skeleton_4.assign_sec_enemy_info("skeleton", 1550, 570, 150, 18, 12, 200, 2, 2, -0.05);

	if (!Evil_Wizard_5.dead)
		Evil_Wizard_5.assign_sec_enemy_info("EvilWizard", 380, 600, 145, 14, 2, 250, 2, 2, -0.08);

	if (!Evil_Wizard_6.dead)
		Evil_Wizard_6.assign_sec_enemy_info("EvilWizard", 370, 50, 180, 13, 2, 250, 2, 2, -0.08);

	if (!Skeleton_5.dead)
		Skeleton_5.assign_sec_enemy_info("skeleton", 1250, 340, 90, 23, 12, 140, 2, 2, -0.05);

	if (!Skeleton_6.dead)
		Skeleton_6.assign_sec_enemy_info("skeleton", 1600, 340, 110, 23, 12, 140, 2, 2, -0.05);


	while (window.isOpen())
	{
		Vector2f knightPos = knight.sprite.getPosition();
		// adjusting the collision rect to be more accurate 
		// redRect for collision detection
		knight.collisionRect = RectCreator(100, 145, knightPos.x + 150, knightPos.y + 150);

		Vector2f SPos1 = Skeleton_1.sprite.getPosition();
		Vector2f SPos2 = Skeleton_2.sprite.getPosition();
		Vector2f SPos4 = Skeleton_4.sprite.getPosition();
		Vector2f SPos5 = Skeleton_5.sprite.getPosition();
		Vector2f SPos6 = Skeleton_6.sprite.getPosition();
		Vector2f EPos1 = Evil_Wizard_1.sprite.getPosition();
		Vector2f EPos2 = Evil_Wizard_2.sprite.getPosition();
		Vector2f EPos3 = Evil_Wizard_3.sprite.getPosition();
		Vector2f EPos4 = Evil_Wizard_4.sprite.getPosition();
		Vector2f EPos5 = Evil_Wizard_5.sprite.getPosition();
		Vector2f EPos6 = Evil_Wizard_6.sprite.getPosition();
		Vector2f ExecPos = EvilBoss.sprite.getPosition();

		Skeleton_1.zone = RectCreator(170, 150, SPos1.x + 24, SPos1.y);
		Skeleton_2.zone = RectCreator(170, 150, SPos2.x + 24, SPos2.y);
		Skeleton_4.zone = RectCreator(200, 250, SPos4.x + 80, SPos4.y + 50);
		Skeleton_4.zone = RectCreator(200, 250, SPos4.x + 80, SPos4.y + 50);
		Skeleton_5.zone = RectCreator(280, 300, SPos5.x + 80, SPos5.y + 50);
		Skeleton_6.zone = RectCreator(280, 300, SPos6.x + 80, SPos6.y + 50);
		Evil_Wizard_1.zone = RectCreator(200, 200, EPos1.x + 120, EPos1.y + 100);
		Evil_Wizard_4.zone = RectCreator(200, 200, EPos4.x + 130, EPos4.y + 110);
		Evil_Wizard_5.zone = RectCreator(250, 200, EPos5.x + 230, EPos5.y + 220);
		Evil_Wizard_6.zone = RectCreator(250, 200, EPos6.x + 230, EPos6.y + 220);
		EvilBoss.zone1 = RectCreator(300, 350, ExecPos.x + 380, ExecPos.y + 350);
		EvilBoss.zone2 = RectCreator(2 * EvilBoss.kill_zone, 300, EvilBoss.left_boundary + 300, ExecPos.y + 300);

		if ((Skeleton_2.state == "attack" || Evil_Wizard_4.state == "attack") && levelTwoMap.currentScene == 0)
		{
			knight.state = "Hit";
			knight.updateTexture();
			if (Skeleton_2.state == "attack")
				knight.health -= (double)(Skeleton_2.attack) * 0.00711;
			else
				knight.health -= (double)(Evil_Wizard_4.attack) * 0.00711;
		}
		if (Evil_Wizard_1.state == "attack" && levelTwoMap.currentScene == 1)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Evil_Wizard_1.attack) * 0.00711;
		}
		if ((Skeleton_5.state == "attack" || Skeleton_6.state == "attack") && levelTwoMap.currentScene == 3)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(Skeleton_5.attack) * 0.00711;
		}

		if ((Skeleton_4.state == "attack" || Evil_Wizard_5.state == "attack" || Evil_Wizard_6.state == "attack") && levelTwoMap.currentScene == 4)
		{
			knight.state = "Hit";
			knight.updateTexture();
			if (Skeleton_4.state == "attack")
				knight.health -= (double)(Skeleton_5.attack) * 0.00711;
			else
				knight.health -= (double)(Evil_Wizard_5.attack) * 0.00711;

		}
		if ((EvilBoss.state == "attack") && levelTwoMap.currentScene == 5)
		{
			knight.state = "Hit";
			knight.updateTexture();
			knight.health -= (double)(EvilBoss.attack1) * 0.00711;
		}

		if (!knight.isAlive())
		{
			knight.state = "Death";
			knight.updateTexture();
		}

		if (Skeleton_5.dead)
		{
			if (monsterCoins[0] == false)
			{
				monsterCoins[0] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_6.dead)
		{
			if (monsterCoins[1] == false)
			{
				monsterCoins[1] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_7.dead)
		{
			if (monsterCoins[2] == false)
			{
				monsterCoins[2] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Skeleton_8.dead)
		{
			if (monsterCoins[3] == false)
			{
				monsterCoins[3] = true;
				storeCoins += 20;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_4.dead)
		{
			if (monsterCoins[4] == false)
			{
				monsterCoins[4] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_5.dead)
		{
			if (monsterCoins[5] == false)
			{
				monsterCoins[5] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_6.dead)
		{
			if (monsterCoins[6] == false)
			{
				monsterCoins[6] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_7.dead)
		{
			if (monsterCoins[7] == false)
			{
				monsterCoins[7] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (Evil_Wizard_8.dead)
		{
			if (monsterCoins[8] == false)
			{
				monsterCoins[8] = true;
				storeCoins += 30;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}
		if (EvilBoss.dead)
		{
			if (monsterCoins[9] == false)
			{
				monsterCoins[9] = true;
				storeCoins += 100;
				totalCoins = to_string(storeCoins);
				coinFile.open("coinFile.txt", ios::out);
				if (coinFile.is_open())
				{
					coinFile << totalCoins;
					coinFile.close();
				}
			}
		}

		// Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(window);
					cout << "MousePos x : " << mousePos.x << " MousePos y :  " << mousePos.y << endl;
					cout << "Red Rect Left : " << knight.collisionRect.getGlobalBounds().left << " Red Rect Top " << knight.collisionRect.getGlobalBounds().top << endl;
					cout << "blue rect left : " << knight.rect.left << "blue rect top : " << knight.rect.top << endl;
				}
			}

			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Escape)
				{
					knight.rect.left = knight.rect.getPosition().x;
					knight.rect.top = knight.rect.getPosition().y;
					pauseMenu.paused = true;
					pausedtimes++;
					clicksound.play();

				}
			}
		}

		if (knight.rect.getPosition().y > 1090)
		{
			knight.dead = true;
		}



		// check player collision (always should be placed before movement fn to avoid silly animation bugs :)
		levelTwoMap.checkCollision(knight.collisionRect);

		// Clear the window
		window.clear();

		if (!knight.dead)
		{
			if (EvilBoss.dead)
			{
				completedMenu.show(window, levelTwoMap.currentScene);
				break;
			}
			if (!pauseMenu.paused)
			{
				window.draw(levelTwoMap.backgroundSprite);
				healthBox(window);
				coinBox(window);

				movements();
				float time = (float)clock.getElapsedTime().asMicroseconds();
				clock.restart();
				time /= 650;
				if (time > 20)
					time = 20;
				//if (!Evil_Wizard_4.dead && levelTwoMap.currentScene == 0)
				//{
				//	Evil_Wizard_4.update_skeleton_state(time);
				//	window.draw(Evil_Wizard_4.sprite);
				//	//window.draw(Evil_Wizard_4.zone);
				//}
				if (!Skeleton_2.dead && levelTwoMap.currentScene == 0)
				{
					Skeleton_2.update_skeleton_state(time);
					window.draw(Skeleton_2.sprite);
					window.draw(Skeleton_2.zone);
				}
				if (!Skeleton_4.dead && levelTwoMap.currentScene == 4)
				{
					Skeleton_4.update_skeleton_state(time);
					Evil_Wizard_4.update_evilwiz_state(time);
					window.draw(Skeleton_4.sprite);
					window.draw(Skeleton_4.zone);
				}
				if (!Evil_Wizard_1.dead && levelTwoMap.currentScene == 1)
				{
					Evil_Wizard_1.update_evilwiz_state(time);
					window.draw(Evil_Wizard_1.sprite);
					window.draw(Evil_Wizard_1.zone);
				}
				if (!Evil_Wizard_5.dead && levelTwoMap.currentScene == 4)
				{
					Evil_Wizard_5.update_evilwiz_state(time);
					window.draw(Evil_Wizard_5.sprite);
					window.draw(Evil_Wizard_5.zone);
				}
				if (!Evil_Wizard_6.dead && levelTwoMap.currentScene == 4)
				{
					Evil_Wizard_6.update_evilwiz_state(time);
					window.draw(Evil_Wizard_6.sprite);
					window.draw(Evil_Wizard_6.zone);
				}
				if (!Skeleton_5.dead && levelTwoMap.currentScene == 3)
				{
					Skeleton_5.update_skeleton_state(time);
					window.draw(Skeleton_5.sprite);
					window.draw(Skeleton_5.zone);
				}
				if (!Skeleton_6.dead && levelTwoMap.currentScene == 3)
				{
					Skeleton_6.update_skeleton_state(time);
					window.draw(Skeleton_6.sprite);
					window.draw(Skeleton_6.zone);
				}
				if (!EvilBoss.dead && levelTwoMap.currentScene == 5)
				{
					EvilBoss.update_boss2_state(time);
					window.draw(EvilBoss.sprite);
					window.draw(EvilBoss.zone1);
					window.draw(EvilBoss.zone2);
				}

				if (!knight.dead)
				{
					window.draw(knight.collisionRect);
					window.draw(knight.sprite);
					knight.update(time);
				}

			}
			else if (pauseMenu.paused)
			{
				pauseMenu.show(window);
				break;
			}
		}
		else if (knight.dead)
		{
			deathMenu.show(window, levelTwoMap.currentScene);
			break;
		}

		for (int i = 0; i < currentTiles.size(); i++)
		{
			window.draw(currentTiles[i]);
		}

		window.display();
	}
}