#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <string>
using namespace std;
using namespace sf;
const int base = 100;
const int NumOfEnemies = 20;
// Healths are based on 100 as we need 1/100 as a constant that will be divided by later.
/*Also the score system is built on an assumption that if the user is not rewarded then he lost the game,
so score is always ZERO in the base case.
*/
// Score Function is defined in the suffix comment.
/* 
Score=(Total Hits to Enemies×Enemy Health Factor)−(Total Hits from Enemies×Main Character Health Factor)
Or ShortHand
S = (HH*EHF) - (EH*HHF) 
In this equation:
"Total Hits to Enemies" represents the number of times the player's character successfully hits enemies.
"Total Hits from Enemies" represents the number of times the player's character is hit by enemies.
"Enemy Health Factor" represents the relative importance of enemy health in the scoring. 
This factor could be a constant value or dynamically adjusted based on the current health of enemies.
"Main Character Health Factor" represents the relative importance of the main character's health in the scoring.
Similar to the enemy health factor,
this could be a constant value or dynamically adjusted based on the current health of the main character.
*/
// Coins are calculated using a factor that is defined dynamically from the developer every level or game mode
// this means that coins are related to the score of the level and its difficulty level.



struct Score{
	Text GameScore; // to display the score visually
	Text GameCoin; // Display Coins
	long long EnH; // Enemy hits to the hero
	long long HnH; // Hero hits to the enemy
	long long HeroHlth;
	long long EnemyHlth[NumOfEnemies];
	double Hfactor; // Health Factor
	long long tspl; // total score per level
	double f_coin; // Coins = tspl*factor, this is the factor!
	long long TotalCoin;
	long long Price; // the price of Heroes and anything in the store;

	/*
	*  Function name and description:
	*
	-
	* long long ScoreCalc(EnemyHit, HeroHit, EnemyHealthF, HeroHealthF);
	- Calculate score using the formula defined above.
	* long long CoinConverter(tspl , FactorOfCoin);
	- Converts Total Score Per level to Coins
	* long long CoinWithdrawal(TotalCoin, Price);
	- returns Total Coins left after the purchasing process to a Score.TotalCoin
	* double HealthFactor(long long Health);
	- Calculate A health Factor from any health of any character
	* ScoreDisplay();
	- Displaying the score
	* CoinDisplay();
	- Displaying the coins
	* SetToZero();
	- A function to Set Enemy hits and Hero hits to Zero;
	it can be used after a fighting is finished and score is gained/lost to set a new state for the hits of the hero and Enemy.
	*/
	long long ScoreCalc(long long HeroHit, long long EnemyHit,double EnemyHealthF, double HeroHealthF)
	{
		long double opr_score = round((HeroHit * EnemyHealthF) - (EnemyHit * HeroHealthF));
		long long score = opr_score;
		if (score < 0)
		{
			return 0;
		}
		return score;
	}
	long long CoinConverter(long long tspl, double FactorOfCoin)
	{
		long long coin = (double) FactorOfCoin * tspl;
		return coin;
	}
	long long CoinWithdrawal(long long TotalCoin, long long Price)
	{
		return(TotalCoin - Price);
	}
	double HealthFactor(long long Health)
	{
		double Hf = Health / base;
		return Hf;
	}
	void SetToZero()
	{
		EnH = 0; 
		HnH = 0;
	}
	void ScoreDisplay(RenderWindow &Window , long long &tspl)
	{
		// Define the dimensions and position of the coin box
		double boxW = 120.0;
		double boxH = 60.0;
		Vector2f boxPos(0, 15); // Adjust position as needed

		// Create the coin box shape
		RectangleShape CBox(Vector2f(boxW, boxH));
		CBox.setPosition(boxPos);
		CBox.setFillColor(Color::Black); // Brown color

		// Draw the coin box
		Window.draw(CBox);
		Font font;
		font.loadFromFile("Pixelated.ttf");
		if (!font.loadFromFile("Pixelated.ttf")) {
			// Error handling
			return;
		}
		GameScore.setFont(font);
		GameScore.setCharacterSize(18);
		GameScore.setFillColor(Color::White);
		GameScore.setPosition(boxPos.x + 10, boxPos.y + 5);
		string scoreString = "Score: " + to_string(tspl);

		// Set the string to the text object
		GameScore.setString(scoreString);

		// Draw the text object
		Window.draw(GameScore);
	}
	void CoinDisplay(RenderWindow& Window, long long& TotalCoin)
	{
		// Define the dimensions and position of the coin box
		double boxW = 120.0;
		double boxH = 40.0;
		Vector2f boxPos(280, -10); // Adjust position as needed

		// Create the coin box shape
		RectangleShape CBox(Vector2f(boxW, boxH));
		CBox.setPosition(boxPos);
		CBox.setFillColor(Color(139, 69, 19)); // Brown color

		// Draw the coin box
		Window.draw(CBox);



		// Create text to display the number of coins
		Font Cfont;
		Cfont.loadFromFile("Pixelated.ttf");
		if (!Cfont.loadFromFile("Pixelated.ttf")) {
			// Error handling
			return;
		}

		
		GameCoin.setFont(Cfont);
		GameCoin.setCharacterSize(18);
		GameCoin.setFillColor(Color::Yellow);
		GameCoin.setPosition(boxPos.x + 10, boxPos.y + 5); // Adjust position as needed

		// Create a string to display the number of coins
		string CString = "Coins: " + to_string(TotalCoin);

		// Set the string to the text object
		GameCoin.setString(CString);

		// Draw the coin text
		Window.draw(GameCoin);
	}
};
// main is used only for testing purposes.
//int main()
//{
//	Score score;
//	RenderWindow window(VideoMode(800, 600), "Score sys of the Game");
//
//	score.tspl = 100;
//	score.TotalCoin = 500;
//	while (window.isOpen()) {
//		Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == Event::Closed)
//				window.close();
//		}
//
//		window.clear(); // Clear the window
//
//		// Update and display coins and score
//		score.CoinDisplay(window,  score.TotalCoin);
//		score.ScoreDisplay(window, score.tspl);
//
//		window.display(); // Display the contents of the window
//	}
//
//	return 0;
//}
