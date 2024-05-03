#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
using namespace std;
using namespace sf;

int ground = 820;
float const rightWall = 1680;
float const leftWall = -180;
vector<RectangleShape> currentTiles;
const int knight_num_of_textures = 15;
const int num_of_sec_enemies = 4;
const int num_of_enemy_textures = 10;
const int num_of_boss_enemies = 2;

struct character {
	Sprite sprite;  // The sprite representing the player character
	float currentFrame;  // The current frame of animation.
	float attack=10;  // characters attack
	float moveX, moveY;  // Movements on x and y direction
	bool onGround;  // indicate if the player is on the ground
	FloatRect rect;  // The bounding rectangle of the player
	int lastKeyPressed;  // The last key pressed by the player
	int noOfAttacks;   // no of attacks dealt by the player to the enemy 
	string state;  // The current state of the player
	int health;  // Health of the player
	Texture stateTexture[knight_num_of_textures];  // Array of textures for different states

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
		sprite.setScale(3.75, 3.75);  //Set sprite scale
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
			if (currentFrame > 10) currentFrame -= 10;
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
	void assign_sec_enemy_info(string enemytype, int posx, int posy, int movementrange, int attackpow, int attpause, int hp) {
		enemy_type = enemytype;
		if (enemytype == "skeleton")
		{
			num_of_cur_textures = 5;
			cur_enemy_idx = 0;
			state = "walk";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(4, 4);
			left_boundary = posx - movementrange; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + movementrange;
			movement_range = movementrange;
			attack = attackpow; //current skeleton power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_sec_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1;
		}
		else if(enemytype == "EvilWizard")
		{
			num_of_cur_textures = 5;
			cur_enemy_idx = 1;
			state = "walk";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(2.6, 2.6);
			left_boundary = posx - movementrange; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + movementrange;
			movement_range = movementrange;
			attack = attackpow; //current wizard power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_sec_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.1696969;
		}
	}

	bool is_player_in_range_x() { // checking if the character is in our boundaries
		return left_boundary <= knight.rect.getPosition().x && knight.rect.getPosition().x <= right_boundary;
	}
	bool is_knight_sword_touching() { // checking if the sword of knight touching the character
		is_attacked = false;
		float diff = rect.left-knight.rect.left;
		
		if (knight.lastKeyPressed == 1) // knight is facing right
		{
			if (-25 <= diff && diff <= 240)
				is_attacked = true;
		}
		else // knight is facing left
		{
			if (-100 <= diff && diff <= 110)
				is_attacked = true;
		}
		return is_attacked&&(knight.state == "Attack" || knight.state == "AttackCombo" || knight.state == "Attack2");
	}


	void load_sec_enemy_textures() {
		stateTexture = new Texture[num_of_cur_textures];
		for (int i = 0; i < num_of_cur_textures; i++)
			stateTexture[i].loadFromFile(enemy_textures[cur_enemy_idx][i]);
	}

	float turn_time = 10, pause_time = 0;
	void update_skeleton_state(float time)
	{
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
		else if (abs(knight.rect.left - rect.left + 60) <= 110)
				state = "attack";
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
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x) // walks towards the player (if the player is left or right)
					dir = 1;
				else
					dir = -1;
				left_boundary = rect.getPosition().x - movement_range;
				right_boundary = left_boundary + 2 * movement_range; // so we don't use the getPosition() twice ;)
			}
			else if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
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
					currentFrame = 0;

				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				left_boundary = rect.getPosition().x - movement_range;
				right_boundary = left_boundary + 2 * movement_range; // so we don't use the getPosition() twice ;)

				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()))
				{
					knight.health -= attack;
				}
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
		if(dir>0)
			sprite.setTextureRect(IntRect(64 * int(currentFrame), 0, 64, 64)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)
		else
			sprite.setTextureRect(IntRect(64 * int(currentFrame)+64, 0, -64, 64)); // update texture rect in the right direction (so we don't update it in every if cond. with the same values)

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
		else if (abs(knight.rect.left - rect.left + 60) <= 110)
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
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x) // walks towards the player (if the player is left or right)
					dir = 1;
				else
					dir = -1;
				left_boundary = rect.getPosition().x - movement_range;
				right_boundary = left_boundary + 2 * movement_range; // so we don't use the getPosition() twice ;)
			}
			else if (turn_time <= 0 && (rect.left >= right_boundary || rect.left <= left_boundary)) // walks left and right and changes directions if reached boundaries
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
				if (currentFrame >= 8)
					currentFrame = 0;

				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				left_boundary = rect.getPosition().x - movement_range;
				right_boundary = left_boundary + 2 * movement_range; // so we don't use the getPosition() twice ;)

				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()))
				{
					knight.health -= attack;
				}
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

}Skeleton_1, Evil_Wizard_1;

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
	double left_tracker;
	double right_tracker;
	// ------------ DYNAMIC ARRAY, DELETED WHEN CLOSING WINDOW ------------
	Texture* stateTexture = new Texture[0];  // Array of textures for different states
	string enemy_type; // to load different enemies and set ther attributes based on thier type (name)
	string enemy_textures[num_of_boss_enemies][num_of_enemy_textures] =
	{
		{"./enemies/Undead_executioner/idle.png",
		"./enemies/Undead_executioner/walk.png",
		"./enemies/Undead_executioner/attacking.png",
		"./enemies/Undead_executioner/attacking1.png",
		"./enemies/Undead_executioner/skill1.png",
		"./enemies/Undead_executioner/death.png"}
		,
	};
	void assign_boss_enemy_info(string enemytype, int posx, int posy, int killzone, int attackpow1,int attackpow2, int attpause, int hp) {
		enemy_type = enemytype;
		if (enemytype == "Boss1")
		{
			num_of_cur_textures = 6;
			cur_enemy_idx = 0;
			state = "idle";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(3.14, 2*3.14); // cuz i love pi and i love people who love pi ;)
			left_boundary = posx - killzone; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + killzone;
			kill_zone = killzone;
			right_tracker = right_boundary + 20;
			left_tracker = left_boundary + 20;
			attack1 = attackpow1; //current boss power
			attack2 = attackpow2;
			attack_pause_time = attpause;// current pause time between every two attacks
			load_boss_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.3;
		}
		else if (enemytype == "Boss2")
		{
			num_of_cur_textures = 5;
			cur_enemy_idx = 1;
			state = "walk";
			health = hp;
			//attacking_factor = 1; not needed 
			sprite.setScale(2.6, 2.6);
			left_boundary = posx - killzone; // |   .   | , assigning boundaries on the left/right of the character "."
			right_boundary = posx + killzone;
			kill_zone = killzone;
			right_tracker = right_boundary + 40;
			left_tracker = left_boundary - 40;
			attack1 = attackpow1; //current boss power
			attack_pause_time = attpause;// current pause time between every two attacks
			load_boss_enemy_textures();
			sprite.setPosition(posx, posy);
			rect.left = posx;
			rect.top = posy;
			speed = 0.3;
		}
	}

	bool is_player_in_range_x() { // checking if the character is in our boundaries
		return left_boundary <= knight.rect.getPosition().x && knight.rect.getPosition().x <= right_boundary;
	}
	bool is_player_in_killzone_x() { // checking if the character is in our boundaries
		return (left_tracker <= knight.rect.getPosition().x || knight.rect.getPosition().x <= right_tracker) &&
			(left_boundary >= knight.rect.getPosition().x || knight.rect.getPosition().x <= right_boundary);
	}
	bool is_knight_sword_touching() { // checking if the sword of knight touching the character
		is_attacked = false;
		float diff = rect.left - knight.rect.left;

		if (knight.lastKeyPressed == 1) // knight is facing right
		{
			if (-25 <= diff && diff <= 240)
				is_attacked = true;
		}
		else // knight is facing left
		{
			if (-100 <= diff && diff <= 110)
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
		currentFrame += 0.0157 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[5]);
			if (currentFrame >= 19)
				dead = true; // player dies after playing the full animation
		}
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (abs(knight.rect.left - rect.left + 60) <= 120)
			state = "attack";
		else if (is_player_in_killzone_x())
			state = "walk";
		else
			state = "idle";


		if (state == "walk")
		{
			sprite.setTexture(stateTexture[1]);
			if (currentFrame >= 4)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x) // walks towards the player (if the player is left or right)
					dir = 1;
				else
					dir = -1;
				left_boundary = rect.getPosition().x - kill_zone;
				right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)
				right_tracker = right_boundary + 40;
				left_tracker = left_boundary - 40;
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
			if (pause_time <= 0) {

				if (skill_shift >= 4)
				{
					//skill_shift = 0;
					sprite.setTexture(stateTexture[3]);
				}
				else
				{
					sprite.setTexture(stateTexture[2]);
				}
				if (currentFrame >= 6)
				{
					currentFrame = 0;
					skill_shift++;
					if (skill_shift > 5)
					{
						skill_shift = 0;
					}
				}

				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				left_boundary = rect.getPosition().x - kill_zone;
				right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)
				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift)
				{
					knight.health -= attack1;
				}
				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && !skill_shift)
				{
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
	//void tracker()
	//{

	//}
	void update_boss2_state(float time)
	{
		currentFrame += 0.3 * time * speed;
		if (health <= 0 || state == "dead")
		{
			sprite.setTexture(stateTexture[5]);
			if (currentFrame >= 19)
				dead = true; // player dies after playing the full animation
		}
		else if (is_knight_sword_touching())
			state = "on hit";
		else if (abs(knight.rect.left - rect.left + 60) <= 120)
			state = "attack";
		else
			state = "idle";


		if (state == "walk")
		{
			sprite.setTexture(stateTexture[1]);
			if (currentFrame >= 4)
				currentFrame = 0;
			rect.left += speed * time * dir;
			sprite.setPosition(rect.left, rect.top); // setting the new position (i change rect positino the set sprite pos the same)
			turn_time -= 0.05 * time; // additional time to wait when turning so the skeleton doesn't turn multiple times in the same place
			if (is_player_in_range_x())
			{
				if (knight.rect.getPosition().x > rect.getPosition().x) // walks towards the player (if the player is left or right)
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
			if (pause_time <= 0) {

				if (skill_shift >= 4)
				{
					//skill_shift = 0;
					sprite.setTexture(stateTexture[3]);
				}
				else
				{
					sprite.setTexture(stateTexture[2]);
				}
				if (currentFrame >= 6)
				{
					currentFrame = 0;
					skill_shift++;
					if (skill_shift > 5)
					{
						skill_shift = 0;
					}
				}

				if (knight.rect.getPosition().x + 45 <= rect.getPosition().x)
					dir = -1;
				else
					dir = 1;
				left_boundary = rect.getPosition().x - kill_zone;
				right_boundary = left_boundary + 2 * kill_zone; // so we don't use the getPosition() twice ;)

				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && skill_shift)
				{
					knight.health -= attack1;
				}
				if (sprite.getGlobalBounds().intersects(knight.sprite.getGlobalBounds()) && !skill_shift)
				{
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
}executioner;

// level 1 map code
struct LevelOne {
	int currentScene = 0;
	int noOFEnemies = 0;
	Sprite backgroundSprite;
	Texture levelTextures[6];


	void loadTextures() {
		string texturesArr[6] = { "external/level1/p1.png", "external/level1/p2.png","external/level1/p3.png", "external/level1/p4.png",
							      "external/level1/p5.png", "external/level1/p6.png" };

		for (int i = 0; i < 6; i++) {
			levelTextures[i].loadFromFile(texturesArr[i]);
		}
	}

	// set character to Ground
	void setOnGround() {
		knight.onGround = true;
		knight.moveY = 0;
	}

	// used in any collision from bottom
	void bottomCollision() {
		knight.onGround = false;
		knight.moveY = 0.1;
	}

	// used to change background sprite and create & place tiles rectangles in the correct place
	void placeScene() {
		backgroundSprite.setTexture(levelTextures[currentScene]);

		// setting common ground values for similar levels 
		if (currentScene == 0 || currentScene == 1 || currentScene == 2 || currentScene == 3 || currentScene == 4) {
			ground = 1300;
		}


		// placing each scene tiles and doors
		if (currentScene == 0) {
			currentTiles.resize(12);

			for (int i = 0; i < 12; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			knight.rect.left = 5;
			knight.rect.top = 500;

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
		if (currentScene == 1) {
			currentTiles.resize(11);

			for (int i = 0; i < 11; i++) {
				currentTiles[i].setFillColor(Color::Transparent);
			}

			knight.rect.left = 3;
			knight.rect.top = 400;

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

			currentTiles[5].setPosition(1215, 555);
			currentTiles[6].setPosition(1345, 445);
			// end of identical tiles 


			currentTiles[7].setSize(Vector2f(450, 440));
			currentTiles[7].setPosition(1470, 285);

			currentTiles[8].setSize(Vector2f(515, 55));
			currentTiles[8].setPosition(1405, 10);

			currentTiles[9].setSize(Vector2f(770, 115));
			currentTiles[9].setPosition(1150, 940);

			currentTiles[10].setSize(Vector2f(55, 190));
			currentTiles[10].setPosition(1845, 760);

		}
		if (currentScene == 2) {
			currentTiles.resize(13);

			knight.rect.left = -75;
			knight.rect.top = 600;

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
		if (currentScene == 3) {
			knight.rect.left = -10;
			knight.rect.top = 300;

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
		if (currentScene == 4) {
			currentTiles.resize(6);

			knight.rect.top = 570;
			knight.rect.left = -10;


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
		if (currentScene == 5) {
			if (!currentTiles.empty()) {
				currentTiles.erase(currentTiles.begin(), currentTiles.end());
			}

			knight.rect.left = -125;

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
				if (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 164 || collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 164.5) {
					knight.rect.left = currentTiles[4].getPosition().x + 22.5;
				}


				//left collision
				else if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[4].getPosition().x)) {
					knight.rect.left = currentTiles[4].getPosition().x - 275;
				}

				// top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[4].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[4].getPosition().y + 25) {
					knight.rect.top = currentTiles[4].getPosition().y - 295.25;
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
				else if (collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 128 || collisionRect.getGlobalBounds().left >= currentTiles[5].getPosition().x + 128.5) {
					knight.rect.left = currentTiles[5].getPosition().x - 15.5;
				}

				// top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[5].getPosition().y && collisionRect.getGlobalBounds().top + 145 < currentTiles[5].getPosition().y + 50) {
					knight.rect.top = currentTiles[5].getPosition().y - 295.25;
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
				if (collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 768 || collisionRect.getGlobalBounds().left >= currentTiles[6].getPosition().x + 769) {
					knight.rect.left = currentTiles[6].getPosition().x + 625;
				}
				// top collision
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[6].getPosition().y) {
					knight.rect.top = currentTiles[6].getPosition().y - 292.5;
					setOnGround();
				}

			}

			// tile 8 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[7].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[7].getPosition().x) {
					knight.rect.left = currentTiles[7].getPosition().x - 275;
				}

				//top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[7].getPosition().y) {
					knight.rect.top = currentTiles[7].getPosition().y - 293;
					setOnGround();
				}
			}

			// tile 9 
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds())) {
				// left collision
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[8].getPosition().x) || collisionRect.getGlobalBounds().left + 121 <= currentTiles[8].getPosition().x) {
					knight.rect.left = currentTiles[8].getPosition().x - 275;
				}

				//top collision 
				else if (collisionRect.getGlobalBounds().top + 145 > currentTiles[8].getPosition().y) {
					knight.rect.top = currentTiles[8].getPosition().y - 293;
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
				placeScene();
			}
		}
		if (currentScene == 1) {
			// screen left and right boundaries collision
			if (collisionRect.getGlobalBounds().left > 1784) {
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
				if ((collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 318) || (collisionRect.getGlobalBounds().left > currentTiles[1].getPosition().x + 319)) {
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
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[3].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[3].getPosition().x)) {
					knight.rect.left = currentTiles[3].getPosition().x - 271.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 124) || (collisionRect.getGlobalBounds().left > currentTiles[3].getPosition().x + 124.5)) {
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
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[4].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[4].getPosition().x)) {
					knight.rect.left = currentTiles[4].getPosition().x - 271.5;
				}

				// right collision
				else if ((collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 188) || (collisionRect.getGlobalBounds().left >= currentTiles[4].getPosition().x + 189.5)) {
					knight.rect.left = currentTiles[4].getPosition().x + 45.5;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[4].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[4].getPosition().y + 15)) {
					setOnGround();
				}
			}

			// tile 6
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds())) {
				// left collision	
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
					knight.rect.left = currentTiles[5].getPosition().x - 271.5;
				}

				// right collision 
				if ((collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 98) || (collisionRect.getGlobalBounds().left > currentTiles[5].getPosition().x + 99.5)) {
					knight.rect.left = currentTiles[5].getPosition().x - 46;
				}

				// top collision
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[5].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[5].getPosition().y + 15)) {
					setOnGround();
				}



				// bottom  collsion 
				else if (collisionRect.getGlobalBounds().top > currentTiles[5].getPosition().y + 49) {
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
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[7].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[7].getPosition().x)) {

					knight.rect.left = currentTiles[7].getPosition().x - 271.5;
				}

				// top collision 
				else if ((collisionRect.getGlobalBounds().top + 148 > currentTiles[7].getPosition().y) && (collisionRect.getGlobalBounds().top + 148 < currentTiles[7].getPosition().y + 15)) {
					setOnGround();
				}

				// bottom  collsion 
				else if (collisionRect.getGlobalBounds().top > currentTiles[7].getPosition().y + 435) {
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
						if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[i].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[i].getPosition().x)) {
							knight.rect.left = currentTiles[i].getPosition().x - 271.5;
						}

						// right collision
						else if (((collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 128) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getPosition().x + 129))) {
							knight.rect.left = currentTiles[i].getPosition().x - 16.5;
						}

						// top collision
						else if ((collisionRect.getGlobalBounds().top + 145 > currentTiles[i].getPosition().y) && (collisionRect.getGlobalBounds().top + 145 < currentTiles[i].getPosition().y + 5)) {
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
			}

			// tile 13 : Door
			if (collisionRect.getGlobalBounds().intersects(currentTiles[12].getGlobalBounds())) {
				knight.rect.left = -10;
				knight.rect.top = 300;
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
				if ((collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 573) || (collisionRect.getGlobalBounds().left >= currentTiles[2].getPosition().x + 574)) {
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
				if ((collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 123) || (collisionRect.getGlobalBounds().left >= currentTiles[3].getPosition().x + 124)) {
					knight.rect.left = currentTiles[3].getPosition().x - 21.5;
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
					if ((collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 68) || (collisionRect.getGlobalBounds().left >= currentTiles[i].getGlobalBounds().getPosition().x + 69)) {
						knight.rect.left = currentTiles[i].getGlobalBounds().getPosition().x - 76.5;
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
				if ((collisionRect.getGlobalBounds().left + 120 <= currentTiles[5].getPosition().x) || (collisionRect.getGlobalBounds().left + 121 <= currentTiles[5].getPosition().x)) {
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
		string texturesArr[6] = { "external/Level 2/Forest P1.png", "external/Level 2/Forest P2.png","external/Level 2/Forest P3.png", "external/Level 2/Forest P4.png",
								  "external/Level 2/Forest P5.png", "external/Level 2/Forest P6.png" };

		for (int i = 0; i < 6; i++) {
			levelTextures[i].loadFromFile(texturesArr[i]);
		}
	}

	void onGround() {
		knight.moveY = 0;
		knight.onGround = true;
	}

	void jump() {
		knight.moveY = -0.9;
		knight.onGround = false;
		knight.state = "Jump";
	}

	// used to change background sprite and create & place tiles rectangles in the correct place
	void placeScene(RenderWindow& window, Sprite& player) {
		backgroundSprite.setTexture(levelTextures[currentScene]);

		if (currentScene == 0) {
			currentTiles.resize(5);

			for (int i = 0; i < 5; i++) {
				currentTiles[i].setFillColor(Color(255, 0, 0, 128));
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

			for (int i = 0; i < 9; i++) {
				currentTiles[i].setFillColor(Color(255, 0, 0, 128));
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
			currentTiles.resize(12);

			for (int i = 0; i < 12; i++) {
				currentTiles[i].setFillColor(Color(255, 0, 0, 128));
			}

			currentTiles[0].setSize(Vector2f(240, 70));
			currentTiles[0].setPosition(0, 580);

			currentTiles[1].setSize(Vector2f(65, 65));
			currentTiles[1].setPosition(420, 475);

			currentTiles[2].setSize(Vector2f(65, 65));
			currentTiles[2].setPosition(615, 700);

			currentTiles[3].setSize(Vector2f(65, 65));
			currentTiles[3].setPosition(870, 750);

			currentTiles[4].setSize(Vector2f(65, 65));
			currentTiles[4].setPosition(1125, 540);

			currentTiles[5].setSize(Vector2f(65, 65));
			currentTiles[5].setPosition(1440, 480);

			currentTiles[6].setSize(Vector2f(175, 75));
			currentTiles[6].setPosition(1725, 640);

			currentTiles[7].setSize(Vector2f(205, 135));
			currentTiles[7].setPosition(955, 970);

			currentTiles[8].setSize(Vector2f(210, 190));
			currentTiles[8].setPosition(1170, 870);

			currentTiles[9].setSize(Vector2f(225, 255));
			currentTiles[9].setPosition(1395, 800);

			currentTiles[10].setSize(Vector2f(195, 155));
			currentTiles[10].setPosition(1620, 900);

			currentTiles[11].setSize(Vector2f(10, 150));
			currentTiles[11].setPosition(1910, 500);
		}

		if (currentScene == 3) {
			knight.rect.top = 0;
			knight.rect.left = 25;
			currentTiles.erase(currentTiles.begin(), currentTiles.end());
			currentTiles.resize(16);

			for (int i = 0; i < 16; i++) {
				currentTiles[i].setFillColor(Color(255, 0, 0, 128));
			}

			currentTiles[0].setSize(Vector2f(255, 70));
			currentTiles[0].setPosition(0, 650);

			currentTiles[1].setSize(Vector2f(65, 450));
			currentTiles[1].setPosition(420, 480);

			currentTiles[2].setSize(Vector2f(65, 450));
			currentTiles[2].setPosition(615, 700);

			currentTiles[3].setSize(Vector2f(65, 450));
			currentTiles[3].setPosition(870, 750);

			currentTiles[4].setSize(Vector2f(65, 450));
			currentTiles[4].setPosition(1125, 510);

			currentTiles[5].setSize(Vector2f(55, 320));
			currentTiles[5].setPosition(1445, 0);

			currentTiles[6].setSize(Vector2f(225, 70));
			currentTiles[6].setPosition(1310, 650);

			currentTiles[7].setSize(Vector2f(260, 65));
			currentTiles[7].setPosition(1660, 650);

			currentTiles[8].setSize(Vector2f(300, 255));
			currentTiles[8].setPosition(0, 800);

			currentTiles[9].setSize(Vector2f(200, 190));
			currentTiles[9].setPosition(300, 870);

			currentTiles[10].setSize(Vector2f(225, 255));
			currentTiles[10].setPosition(550, 800);

			currentTiles[11].setSize(Vector2f(195, 155));
			currentTiles[11].setPosition(775, 900);

			currentTiles[12].setSize(Vector2f(195, 155));
			currentTiles[12].setPosition(970, 900);

			currentTiles[13].setSize(Vector2f(195, 155));
			currentTiles[13].setPosition(1165, 900);

			currentTiles[14].setSize(Vector2f(195, 155));
			currentTiles[14].setPosition(1360, 900);

			currentTiles[15].setSize(Vector2f(10, 150));
			currentTiles[15].setPosition(1930, 500);
		}

		if (currentScene == 4) {
			currentTiles.erase(currentTiles.begin(), currentTiles.end());
			currentTiles.resize(8);

			for (int i = 0; i < 8; i++) {
				currentTiles[i].setFillColor(Color(255, 0, 0, 128));
			}

			currentTiles[0].setSize(Vector2f(425, 80));
			currentTiles[0].setPosition(35, 740);

			currentTiles[1].setSize(Vector2f(425, 80));
			currentTiles[1].setPosition(515, 510);

			currentTiles[2].setSize(Vector2f(200, 75));
			currentTiles[2].setPosition(1050, 350);

			currentTiles[3].setSize(Vector2f(200, 75));
			currentTiles[3].setPosition(1335, 685);

			currentTiles[4].setSize(Vector2f(315, 75));
			currentTiles[4].setPosition(965, 865);

			currentTiles[5].setSize(Vector2f(315, 70));
			currentTiles[5].setPosition(550, 1050);

			currentTiles[6].setSize(Vector2f(345, 85));
			currentTiles[6].setPosition(1575, 870);

			currentTiles[7].setSize(Vector2f(150, 10));
			currentTiles[7].setPosition(1930, 700);
		}

		if (currentScene == 5) {
			currentTiles.erase(currentTiles.begin(), currentTiles.end());
			ground = 555;
			/*if(knight.rect.left){}*/
		}
	}

	// detect collision of main character and tiles rectangels 
	void checkCollision(RenderWindow& window, RectangleShape& collisionRect) {
		if (currentScene == 0) {
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds()))
			{

				if (collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + (423) || collisionRect.getGlobalBounds().left >= currentTiles[0].getPosition().x + (424)) {
					knight.rect.left = currentTiles[0].getPosition().x + 267; // in my case diff is 148 
				}
				// top collision
				else if (collisionRect.getGlobalBounds().top + 170 > currentTiles[0].getPosition().y) {
					onGround();
					knight.rect.top = currentTiles[0].getPosition().y - 298;
				}
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[1].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[2].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds()))
			{
				knight.moveY = -0.9;
				knight.onGround = false;
				knight.state = "Jump";
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds()))
			{
				currentScene++;
				knight.rect.top = 0;
				knight.rect.left = 0;
				placeScene(window, knight.sprite);
			}
		}

		if (currentScene == 1) {
			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[0].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[1].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds()))
			{
				jump();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[3].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds()))
			{
				jump();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[5].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds()))
			{
				jump();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[7].getPosition().y - 300;
				onGround();
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds()))
			{
				currentScene++;
				knight.rect.top = 0;
				knight.rect.left = 0;
				placeScene(window, knight.sprite);
			}
		}

		if (currentScene == 2) {

			if (collisionRect.getGlobalBounds().intersects(currentTiles[0].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[0].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[1].getGlobalBounds()))
			{
				onGround();
				knight.rect.top = currentTiles[1].getPosition().y - 298;
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[2].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[2].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[3].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[3].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[4].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[4].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[5].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[5].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[6].getGlobalBounds()))
			{
				knight.rect.top = currentTiles[6].getPosition().y - 300;
				onGround();
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				knight.moveX = 0;
				knight.moveY = 0.01;
				knight.health -= 10;
				knight.state = "Hit";
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[8].getGlobalBounds()))
			{
				knight.moveX = 0;
				knight.moveY = 0.01;
				knight.health -= 10;
				knight.state = "Hit";
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[9].getGlobalBounds()))
			{
				knight.moveX = 0;
				knight.moveY = 0.01;
				knight.health -= 10;
				knight.state = "Hit";
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[10].getGlobalBounds()))
			{
				knight.moveX = 0;
				knight.moveY = 0.01;
				knight.health -= 10;
				knight.state = "Hit";
			}
			if (collisionRect.getGlobalBounds().intersects(currentTiles[11].getGlobalBounds()))
			{
				currentScene++;
				knight.rect.top = 0;
				knight.rect.left = 50;
				placeScene(window, knight.sprite);
			}
		}

		if (currentScene == 3) {

			for (int i = 0; i < 8; i++)
			{
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds()))
				{
					knight.rect.top = currentTiles[i].getPosition().y - 300;
					onGround();
				}
			}

			for (int i = 8; i < 15; i++)
			{
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds()))
				{
					knight.moveX = 0;
					knight.moveY = 0.01;
					knight.health -= 10;
					knight.state = "Hit";
				}
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[15].getGlobalBounds()))
			{
				knight.rect.top = 0;
				knight.rect.left = 25;
				currentScene++;

				placeScene(window, knight.sprite);
			}
		}

		if (currentScene == 4) {
			for (int i = 0; i < 7; i++)
			{
				if (collisionRect.getGlobalBounds().intersects(currentTiles[i].getGlobalBounds()))
				{
					knight.rect.top = currentTiles[i].getPosition().y - 300;
					onGround();
				}
			}

			if (collisionRect.getGlobalBounds().intersects(currentTiles[7].getGlobalBounds()))
			{
				knight.rect.top = 0;
				knight.rect.left = 600;
				currentScene++;

				placeScene(window, knight.sprite);
			}
		}
	}
} levelTwoMap;

void arcadeMode(RenderWindow& window) {
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("external/background1.png");
	Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	Clock clock;
	knight.assignSprite(); // Initialize player character
	knight.rect.left = 10;
	knight.rect.top = 850;

	Skeleton_1.assign_sec_enemy_info("skeleton", 500, 900, 100, 12, 0.5, 100);
	Evil_Wizard_1.assign_sec_enemy_info("EvilWizard", 1000, 830, 100, 12, 0.5, 120);
	executioner.assign_boss_enemy_info("Boss1", 1600, 515, 130, 20, 30, 0.3, 190);
	while (window.isOpen()) {
		// Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				delete[] Skeleton_1.stateTexture;
				delete[] Evil_Wizard_1.stateTexture;
				window.close();

			}
		}

		// Update game logic
		movements();

		float time = (float)clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 650;
		if (time > 20)
			time = 20;
		knight.update(time);

		if (!Skeleton_1.dead);
		Skeleton_1.update_skeleton_state(time);

		if (!Evil_Wizard_1.dead);
		Evil_Wizard_1.update_evilwiz_state(time);

		if (!executioner.dead);
		executioner.update_boss1_state(time);
		
		// Clear the window
		window.clear();

		// Draw game elements
		window.draw(backgroundSprite);
		window.draw(knight.sprite);
		window.draw(Skeleton_1.sprite);
		window.draw(Evil_Wizard_1.sprite);
		window.draw(executioner.sprite);
		// Display the window
		window.display();
	}
}

void levelOne(RenderWindow& window) {
	Clock clock;
	knight.assignSprite(); // Initialize player character


	levelOneMap.loadTextures();
	levelOneMap.placeScene();
	while (window.isOpen()) {
		// redRect for collision detection
		RectangleShape collisionRect;
		collisionRect.setOutlineThickness(2);
		collisionRect.setOutlineColor(Color::Transparent);
		collisionRect.setFillColor(Color::Transparent);
		collisionRect.setSize(Vector2f(120, 145));
		Vector2f knightPos = knight.sprite.getPosition();
		// adjusting the collision rect to be more accurate 
		collisionRect.setPosition(knightPos.x + 150, knightPos.y + 150);





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
					cout << "Knight Rect Left : " << knight.rect.left << " Knight Rect Top : " << knight.rect.top << endl;
					cout << "Red Rect Left : " << collisionRect.getGlobalBounds().left << " Red Rect Top " << collisionRect.getGlobalBounds().top << endl;
				}
			}
		}




		// check player collision (always should be placed before movement fn to avoid silly animation bugs :) )
		levelOneMap.checkCollision(collisionRect);

		// Update game logic
		movements();



		float time = (float)clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 650;
		if (time > 20)
			time = 20;
		knight.update(time);





		// Clear the window
		window.clear();

		// Draw game elements	
		window.draw(levelOneMap.backgroundSprite);
		window.draw(knight.sprite);

		for (int i = 0; i < currentTiles.size(); i++)
		{
			window.draw(currentTiles[i]);
		}

		window.draw(collisionRect);

		// Display the window
		window.display();
	}
}

void levelTwo(RenderWindow& window) {
	Clock clock;
	knight.assignSprite(); // Initialize player character


	RectangleShape blueRect;
	blueRect.setOutlineColor(Color::Blue);
	blueRect.setOutlineThickness(2);
	blueRect.setFillColor(Color::Transparent);

	levelTwoMap.loadTextures();
	levelTwoMap.placeScene(window, knight.sprite);
	while (window.isOpen()) {
		// redRect for collision detection
		RectangleShape redRect;
		redRect.setOutlineColor(Color::Red);
		redRect.setOutlineThickness(2);
		redRect.setFillColor(Color::Transparent);
		redRect.setSize(Vector2f(90, 170));
		Vector2f knightPos = knight.sprite.getPosition();
		if (knight.lastKeyPressed == 1)  redRect.setPosition(knightPos.x + 160, knightPos.y + 130);
		else  redRect.setPosition(knightPos.x + 190, knightPos.y + 130);
		// Handle events
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
			if (event.mouseButton.button == Mouse::Left) {
				Vector2i mousePos = Mouse::getPosition(window);

				if (event.type == Event::MouseButtonPressed) {
					cout << "MousePos x : " << mousePos.x << " MousePos y :  " << mousePos.y << endl;
					cout << "Knight Rect Left : " << knight.rect.left << " Knight Rect Top : " << knight.rect.top << endl;
					cout << "Red Rect Left : " << redRect.getGlobalBounds().left << " Red Rect Top " << knight.rect.top << endl;
				}
			}
		}



		// blue rectangle around character 
		blueRect.setSize(Vector2f(knight.sprite.getGlobalBounds().width, knight.sprite.getGlobalBounds().height));
		blueRect.setPosition(knight.sprite.getPosition());


		// Update game logic
		levelTwoMap.checkCollision(window, redRect);
		movements();

		float time = (float)clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time /= 650;
		if (time > 20)
			time = 20;
		knight.update(time);


		// Clear the window
		window.clear();

		// Draw game elements

		for (const auto& tile : currentTiles)
		{
			window.draw(tile);
		}
		window.draw(levelTwoMap.backgroundSprite);

		window.draw(knight.sprite);



		window.draw(redRect);
		window.draw(blueRect);

		// Display the window
		window.display();
	}
}
