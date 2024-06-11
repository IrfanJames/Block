#pragma once

#include <string>
#include <vector>
#include <future>

#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include "Timer.hpp"
#include "SFML/Audio.hpp"

#include "Blocks.hpp"

class App {

public:

	App(const std::vector<std::string>& filepaths);

	~App();

	void Run();


private:

	void Events();

	void ImGUI();

	void Options();

	void Threads();

	void Update();

	void Render();

	void EndFrame();

private:

	bool End = 0;
	bool cursorInWin = 0;
	bool ImGuiInFocus = 0;

	bool HoverClick = 0;
	unsigned int turn = 0;
	sf::Vector2i ItemHoverd { -1,-1 };

	// ImGui
	sf::Clock deltaClock;

	sf::Sound soundPickUp, soundDrag, soundHover;
	sf::SoundBuffer buffer1, buffer2, buffer3;

	std::vector<std::future<void>> futures;
};