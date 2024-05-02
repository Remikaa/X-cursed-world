#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
using namespace std;
using namespace sf;

int ground = 820;
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
