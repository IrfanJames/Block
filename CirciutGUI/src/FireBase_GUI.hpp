
// Colapse all Scopes - it'll be easier
// Ctrl + M + A in Visual Studio
#pragma once

#include "SFML/Graphics.hpp"
#include "FireBase_Bot.hpp"
#include "Blocks.hpp"


//extern sf::Texture compTex[8];
extern std::vector<sf::Texture> compTex;


namespace GUI {

	// Constants
	extern const int gap;
	extern const sf::Vector2f zero;
	extern const sf::Color normalCompColor;

	extern bool darkLightMode;
	extern sf::Color tempDimColor;

	extern sf::Event evnt;
	extern sf::RenderWindow app;
	extern sf::View view;
	extern sf::Font font;


	// Cursor
	extern sf::Vector2f mouseHold;
	extern sf::Vector2f mouseOffSet;
	extern time_t click; // Time passed since Click
	 
	extern bool Click(int Sensitivity);
	extern sf::Vector2f cursorInSim();
	extern sf::Vector2f onScreen(float X, float Y);
	extern sf::Vector2f onScreen(const sf::Vector2f& point);
	extern sf::FloatRect visibleArea();
	extern float trim(float num, int wrt = gap);
	extern sf::Vector2f trim(sf::Vector2f vec, int wrt = gap);
	
	// Warning: Clears the vec
	extern void str_to_vecInt(const std::string& str, std::vector<int>& vec);


	// Grid
	extern sf::Color backColor;
	extern void drawGrid();


	// Drag
	extern bool dragBool;
	extern void iniDrag();
	extern void Drag();
	extern void colorBrightLineGrid();

	// PlayerArea
	extern sf::Color Green;
	extern sf::Color Red;
	extern sf::Color Blue;
	extern sf::Color Yellow;
	extern sf::Texture GridTex;
	extern sf::Sprite  GridSprite;
	extern sf::RectangleShape GridBack;
	extern sf::RectangleShape PlayerAreas[4];
	void initializePlayerArea();
	void updatePlayerArea();
	void drawPlayerArea();

	// Vectors
	extern std::vector<Bot> Bots;
	extern void BotsUpdate();

	extern sf::Vector2i BlockPicked;
	extern const std::string NameDefault;
	extern std::string NameGreen;
	extern std::string NameRed;
	extern std::string NameBlue;
	extern std::string NameYellow;
	extern std::vector<Block> GreenBlocks;
	extern std::vector<Block> RedBlocks;
	extern std::vector<Block> BlueBlocks;
	extern std::vector<Block> YellowBlocks;
	void initializeBlocks();
	void arrangeBlocks();
	void drawBlocks();

	// Unique and Sorted
	extern void addToVector(int integer, std::vector<int>& vec);


	extern void updateThemeColors();
	extern void initializeGUI();


	namespace Options
	{
		extern void Open(const std::string& filepath);

		extern void Save(const std::string& file);

		extern void SaveAsImage(const std::string& filepath);
	}

	

	
}
