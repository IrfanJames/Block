
/*
	* Colapse all Scopes - it'll be easier
	* Ctrl + M + A in Visual Studio
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <direct.h>
#include <chrono>
#include <future>

//#include <thread>
//#include <sstream>
//#include <string>

#include "imgui.h"
#include "imgui-SFML.h"
#include "SFML/Graphics.hpp"

#include "LOG.hpp"
#include "Timer.hpp"
#include "Windows_Stuff.hpp"
#include "Resource_Manager.hpp"

#include "FireBase_Bot.hpp"

#include "FireBase_GUI.hpp"
#include "FireBase_my_Utils.hpp"
#include "FireBase_App.hpp"

App::App(const std::vector<std::string>& filepaths)
{
	srand(time(NULL));

	GUI::initializeGUI();
	fb::InitializeFireBase();

	ImGui::SFML::Init(GUI::app);
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	Resource box;
	box.SetAll(IDR_BOX, "BMP");
	Block::loadBoxTexture(box.GetResource().ptr, box.GetResource().size_bytes);

	{
		auto ref = fb::storage->GetReference("MP Game");


	}



	// Sound
	{
		Resource buf1, buf2, buf3;
		buf1.SetAll(IDR_MP3_1, "MP3");
		buf2.SetAll(IDR_MP3_2, "MP3");
		buf3.SetAll(IDR_MP3_3, "MP3");

		buffer1.loadFromMemory(buf1.GetResource().ptr, buf1.GetResource().size_bytes);
		buffer2.loadFromMemory(buf2.GetResource().ptr, buf2.GetResource().size_bytes);
		buffer3.loadFromMemory(buf3.GetResource().ptr, buf3.GetResource().size_bytes);

		soundDrag.setBuffer(buffer1);
		soundPickUp.setBuffer(buffer2);
		soundHover.setBuffer(buffer3);

		//soundDrag.setVolume(45);
		soundPickUp.setVolume(45);
		soundHover.setVolume(45);
	}

	//Resource my_pic(IDR_CAP, "BMP");
	//Resource my_font(IDR_FONT2, "FONT");
	//my_font.GetInfo();
	/* Test Circle
	bool DrawCircle = 1; int t_vertices = 34; float t_radius = 50, t_Colors[3] = { (float)204 / 255, (float)77 / 255, (float)5 / 255 }; sf::CircleShape testCircle(t_radius, t_vertices); {
		testCircle.setOrigin(t_radius, t_radius);
		testCircle.setPosition(W / 2, H / 2);
		testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255))); }//*/

#ifdef _DEBUG
	// For Opening Where Left
	GUI::Options::Open("temp_files/last.txt");
#endif

	//////////////// argv //////////////////
	if (filepaths.empty() == false)
	{
		GUI::Options::Open(filepaths.back());
	}
}

App::~App()
{
	//ImGui
	ImGui::SFML::Shutdown();

#ifdef _DEBUG
	GUI::Options::Save("temp_files/last.txt");
#else
	LOG::log_file.close();
#endif
}

void App::Run()
{
	//////////////// Main Loop ////////////////////
	while (GUI::app.isOpen() && End == false)
	{
		/*if (soundHover.getStatus() == sf::Sound::Playing)
			soundHover.setPlayingOffset(sf::milliseconds(off));
		else
			soundHover.play();*/

		Events();

		ImGUI();

		Threads();

		Options();

		Update();

		Render();

		EndFrame();
	}

}



void App::Events()
{
	while (GUI::app.pollEvent(GUI::evnt)) {
		using namespace GUI;

		// ImGui
		ImGui::SFML::ProcessEvent(evnt);

		if (evnt.type == evnt.Closed) {

			std::ofstream window_size("temp_files/win_size.txt");
			window_size << GUI::app.getSize().x << "\t";
			window_size << GUI::app.getSize().y << "\t";
			window_size << GUI::app.getPosition().x << "\t";
			window_size << GUI::app.getPosition().y << "\t";
			window_size.close();

			app.close(); End = 1;
		}

		if (evnt.type == evnt.MouseLeft) { cursorInWin = 0; }

		if (evnt.type == evnt.MouseEntered) { cursorInWin = 1; }

		if (evnt.type == evnt.Resized) {
			view = sf::View(sf::FloatRect(
				(int)view.getCenter().x - (int)(evnt.size.width / 2),
				(int)view.getCenter().y - (int)(evnt.size.height / 2),
				(int)evnt.size.width,
				(int)evnt.size.height));
			//GUI::view.setSize((int)evnt.size.width, (int)evnt.size.height);
			//GUI::view.setCenter((int)GUI::view.getCenter().x, (int)GUI::view.getCenter().y);


			std::ofstream window_size("temp_files/win_size.txt");
			window_size << GUI::app.getSize().x << "\t";
			window_size << GUI::app.getSize().y << "\t";
			window_size << GUI::app.getPosition().x << "\t";
			window_size << GUI::app.getPosition().y << "\t";
			window_size.close();
		}

		if (evnt.type == evnt.MouseButtonPressed)
		{
			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				dragBool = true;
				GUI::iniDrag();
			}

			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				HoverClick = false;

				bool found = false;

				for (int i = 0; i < GreenBlocks.size(); i++)
					if (GreenBlocks[i].getBounds().contains(GUI::cursorInSim()))
					{
						BlockPicked = sf::Vector2i(0, i);
						found = true;
						soundPickUp.play();
					}

				if (found == false)
					for (int i = 0; i < RedBlocks.size(); i++)
						if (RedBlocks[i].getBounds().contains(GUI::cursorInSim()))
						{
							BlockPicked = sf::Vector2i(1, i);
							found = true;
							soundPickUp.play();
						}

				if (found == false)
					for (int i = 0; i < BlueBlocks.size(); i++)
						if (BlueBlocks[i].getBounds().contains(GUI::cursorInSim()))
						{
							BlockPicked = sf::Vector2i(2, i);
							found = true;
							soundPickUp.play();
						}
				if (found == false)
					for (int i = 0; i < YellowBlocks.size(); i++)
						if (YellowBlocks[i].getBounds().contains(GUI::cursorInSim()))
						{
							BlockPicked = sf::Vector2i(3, i);
							found = true;
							soundPickUp.play();
						}
			}
		}

		if (evnt.type == evnt.MouseButtonReleased)
		{
			if (evnt.mouseButton.button == sf::Mouse::Middle) {
				dragBool = false;
			}

			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				HoverClick = true;

				BlockPicked = sf::Vector2i(-1, -1);

				if (soundDrag.getStatus() == sf::Sound::Playing)
					soundDrag.setPlayingOffset(sf::milliseconds(19));
				else
					soundDrag.play();
			}
		}

		if (evnt.type == evnt.KeyPressed)
		{
			if (evnt.key.code == sf::Keyboard::Escape)
			{
				app.close();
				End = 1;
				continue;
			}

			if (evnt.key.code == sf::Keyboard::Enter)
			{
				turn = std::abs((int)((++turn) % 4));
			}

			if (evnt.key.code == sf::Keyboard::R)
			{
				if (0 <= BlockPicked.x && BlockPicked.x < 4)
				{
					int i = BlockPicked.y;
					std::vector<Block>* vec = nullptr;

					if (BlockPicked.x == 0) vec = &GreenBlocks;
					else if (BlockPicked.x == 1) vec = &RedBlocks;
					else if (BlockPicked.x == 2) vec = &BlueBlocks;
					else if (BlockPicked.x == 3) vec = &YellowBlocks;

					if (vec && 0 <= i && i < (*vec).size())
					{
						auto& block = (*vec)[i];

						block.rotate();
						soundDrag.play();
					}
				}
			}

			// Ctrl
			if (evnt.key.control) {
				if (evnt.key.code == sf::Keyboard::N)
				{
					arrangeBlocks();
				}
				if (evnt.key.code == sf::Keyboard::O) {

					std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

					if (!filepath.empty())
						GUI::Options::Open(filepath);

				}
				if (evnt.key.code == sf::Keyboard::S) {
					;
					//_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
					//_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
				}
				if (evnt.key.code == sf::Keyboard::S && evnt.key.shift) {

					_mkdir("Saved-Images");		// Alredy Exists(-1) else Created(0)
					_mkdir("Saved-Projects");	// Alredy Exists(-1) else Created(0)
					std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

					if (!filepath.empty()) {

						if (filepath.back() == 'T')
							GUI::Options::Save(filepath);

						if (filepath.back() == 'G')
							futures.emplace_back(std::async(std::launch::async, GUI::Options::SaveAsImage, filepath));

					}

				}
			}
		}

	}
}

void App::ImGUI()
{
	using namespace GUI;
	ImGuiInFocus = false;

	ImGui::SFML::Update(app, deltaClock.restart());
	/*
	ImGui::Begin("Frist ImGui Win");
	ImGui::Text("My Project will be on Steroids");
	ImGui::Checkbox("Draw Circle", &DrawCircle);
	ImGui::SliderFloat("Radius", &t_radius, 0, 200);
	ImGui::SliderInt("Sides", &t_vertices, 3, 35);
	ImGui::ColorEdit3("Color", t_Colors);
	ImGui::End();

	testCircle.setRadius(t_radius);
	testCircle.setOrigin(testCircle.getRadius(), testCircle.getRadius());
	testCircle.setPointCount(t_vertices);
	testCircle.setFillColor(sf::Color((int)(t_Colors[0] * 255), (int)(t_Colors[1] * 255), (int)(t_Colors[2] * 255)));//*/


	if (ImGui::BeginMainMenuBar())
	{
		if (/*ImGui::IsWindowFocused() || */ ImGui::IsWindowHovered()) {
			//LOG("\nMainBar");
			ImGuiInFocus = true;
		}
		if (ImGui::BeginMenu("File"))
		{
			ImGuiInFocus = true;

			if (ImGui::MenuItem("New", "Ctrl + N")) {
				 //stimuliDisplay = 1;
			}
			if (ImGui::MenuItem("Open...", "Ctrl + O")) {

				std::string filepath = OpenFileDialog("text file (*.txt)\0*.txt\0");

				if (!filepath.empty())
					GUI::Options::Open(filepath);
			}
			if (ImGui::MenuItem("Save", "Ctrl + S")) {
				;
				_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
				_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)
			}
			if (ImGui::MenuItem("Save As...", "Ctrl + Shift + S")) {

				_mkdir("Saved-Images"); // Alredy Exists(-1) else Created(0)
				_mkdir("Saved-Projects"); // Alredy Exists(-1) else Created(0)

				std::string filepath = SaveFileDialog("Project file (*.TXT)\0*.TXT\0PNG (*.PNG)\0*.PNG\0");//JPEG (*.JPG)\0*.JPG\0

				if (!filepath.empty()) {

					if (filepath.back() == 'T')
						GUI::Options::Save(filepath);

					if (filepath.back() == 'G')
						GUI::Options::SaveAsImage(filepath);
						futures.emplace_back(std::async(std::launch::async, GUI::Options::SaveAsImage, filepath));
				}

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Esc")) { GUI::app.close(); End = 1; /*continue;*/ }

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			ImGuiInFocus = true;

			if (ImGui::BeginMenu("Handdrawn Icons"))
			{
				ImGui::Text("Coming Soon...");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			//ImGui::MenuItem("Show QuadTree", "", &visible_QuadTree);
			if (ImGui::BeginMenu("Theme")) {

				if (ImGui::MenuItem("Dark")) {

					darkLightMode = false;

					updateThemeColors();
				}
				if (ImGui::MenuItem("Light")) {

					darkLightMode = true;

					updateThemeColors();
				}

				ImGui::EndMenu();
			}


			//asdf
			/*ImGui::Separator();
			if (ImGui::MenuItem("Game")) {
				stimuliDisplay = 1; stimuliEndNodes = 1; //cout << "17";
				PlayMode = !PlayMode;

				CircuitGUI::comp.clear();
				CircuitGUI::virSerial.clear();
				CircuitGUI::virSprite.clear();
				CircuitGUI::virSerialShift.clear();

				asdfwires.clear();

				if (PlayMode) {
					CircuitGUI::Options::openf("Saved-Projects\\Maze.TXT");
					ShellExecute(0, 0, L"https://www.youtube.com/watch?v=6cRctjPRv6M", 0, 0, SW_SHOW);
				}
				else { wireBool = 0; PlayRot = 0; }
			}*/

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGuiInFocus = true;

			if (ImGui::BeginMenu("Controls"))
			{
				if (ImGui::BeginTable("table_context_menu_2", 2))
				{
					static const std::string opt[5][2] = { //HardCode
						{ "Drag View",        "Press Scroll-Wheel + Drag" },
						{ "Pick Blocks",      "Click + Hold + Drag" },
						{ "Restart",          "Ctrl + N" },
						{ "Rotate",           "R" },
						//{ "Select All",     "Ctrl + A" },
						//{ "Delete",         "Del" },
						//{ "Copy",           "Ctrl + C" },
						//{ "Paste",          "Ctrl + V" },
						//{ "Open",           "Ctrl + O" },
						//{ "Save As",        "Ctrl + Shift + S" },
						//{ "New Components", "Hover Left" }
						{ "Escape",			  "Esc" }
					};

					ImGui::TableSetupColumn("Option");
					ImGui::TableSetupColumn("Keys");

					ImGui::TableHeadersRow();
					ImGui::TableNextRow();

					for (int row = 0; row < 5; row++) //HardCode
					{
						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", opt[row][0].c_str());
						ImGui::SameLine();

						ImGui::TableSetColumnIndex(1);
						ImGui::Text(opt[row][1].c_str());

						bool sameLine = false;
						std::string Text = opt[row][1] + " ", temp = "";
						for (int i = 0; i < Text.length(); i++) {

							if (('0' <= Text[i] && Text[i] <= '9') || ('a' <= Text[i] && Text[i] <= 'z') || ('A' <= Text[i] && Text[i] <= 'Z')) {
								temp += Text[i];
							}
							else if (Text[i] == '+') {
								if (sameLine) ImGui::SameLine();
								ImGui::Text("+");
								sameLine = true;
							}
							else {
								if (temp.size()) {
									if (sameLine) ImGui::SameLine();
									ImGui::SmallButton(temp.c_str());
									temp.clear();
									sameLine = true;
								}
							}

						}

						ImGui::TableNextRow();
					}
					ImGui::EndTable();
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Contacts"))
			{
				static std::string links[2][2] = // HardCode
				{
					{ "Copy Github", "https://github.com/IrfanJames/CircuitSimulator_SFML" },
					{ "Copy Email ", "irfanjamespak@gmail.com" }
				};

				for (int i = 0; i < 2; i++) {
					if (ImGui::Button(links[i][0].c_str()))
						sf::Clipboard::setString(links[i][1]);
					ImGui::SameLine();
					ImGui::Text("%s", links[i][1].c_str());
				}

				ImGui::EndMenu();
			}

			/*ImGui::Separator();

			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), true);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}*/

			ImGui::EndMenu();
		}

		ImGui::Button(std::to_string(turn).c_str());

		/*ImGui::Begin("Right-Click");
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::Selectable("Apple")) LOG("\nApple");
			if (ImGui::Selectable("Banana")) LOG("\nBanana");
			ImGui::EndPopup();
		}
		ImGui::End;

		if (ImGui::BeginPopup("Set Value")) {

			static float value = 0;
			ImGui::DragFloat("Value", &value);

			comp[virSerial.front()].voltage = value;

		}
		ImGui::EndPopup();*/

		ImGui::EndMainMenuBar();
	}

	//Docking
	; {
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
	}
	
	ImGuiStyle& style = ImGui::GetStyle();

	float tmpWindowBorderSize = style.WindowBorderSize;
	ImVec2 tmpWindowPadding = style.WindowPadding;

	//style.WindowBorderSize = 0;
	style.WindowBorderSize = 2;
	style.WindowPadding = ImVec2(0, 0);

	// Green
	{
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(GUI::Green.r, GUI::Green.g, GUI::Green.b, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(18, 65, 43, 255));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(18, 65, 43, 255));
		ImGui::SetNextWindowPos(ImVec2(app.mapCoordsToPixel(PlayerAreas[0].getPosition() + sf::Vector2f(4.5, 4))));
		ImGui::SetNextWindowSize(ImVec2(150, 20));
		if (ImGui::Begin("Window-Green##ID", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
				ImGuiInFocus = true; //LOG("O");

			ImGui::SetNextItemWidth(150);
			ImGui::InputText(" ", GUI::NameGreen.data(), GUI::NameGreen.length(), ImGuiInputTextFlags_AutoSelectAll);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	// Red
	{
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(GUI::Red.r, GUI::Red.g, GUI::Red.b, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(65, 20, 36, 255));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(65, 20, 36, 255));
		ImGui::SetNextWindowPos(ImVec2(app.mapCoordsToPixel(PlayerAreas[1].getPosition() + sf::Vector2f(4.5, 18))));
		ImGui::SetNextWindowSize(ImVec2(150, 20));
		if (ImGui::Begin("Window-Red##ID", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
				ImGuiInFocus = true; //LOG("O");

			ImGui::SetNextItemWidth(150);
			ImGui::InputText(" ", GUI::NameRed.data(), GUI::NameRed.length(), ImGuiInputTextFlags_AutoSelectAll);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	// Blue
	{
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(GUI::Blue.r, GUI::Blue.g, GUI::Blue.b, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(18, 42, 66, 255));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(18, 42, 66, 255));
		ImGui::SetNextWindowPos(ImVec2(app.mapCoordsToPixel(PlayerAreas[2].getPosition() + sf::Vector2f(4, 18))));
		ImGui::SetNextWindowSize(ImVec2(150, 20));
		if (ImGui::Begin("Window-Blue##ID", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
				ImGuiInFocus = true; //LOG("O");

			ImGui::SetNextItemWidth(150);
			ImGui::InputText(" ", GUI::NameBlue.data(), GUI::NameBlue.length(), ImGuiInputTextFlags_AutoSelectAll);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	// Yellow
	{
		ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(GUI::Yellow.r, GUI::Yellow.g, GUI::Yellow.b, 255));
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(65, 65, 36, 255));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(65, 65, 36, 255));
		ImGui::SetNextWindowPos(ImVec2(app.mapCoordsToPixel(PlayerAreas[3].getPosition() + sf::Vector2f(4, 4.5))));
		ImGui::SetNextWindowSize(ImVec2(150, 20));
		if (ImGui::Begin("Window-Yellow##ID", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
				ImGuiInFocus = true; //LOG("O");

			ImGui::SetNextItemWidth(150);
			ImGui::InputText(" ", GUI::NameYellow.data(), GUI::NameYellow.length(), ImGuiInputTextFlags_AutoSelectAll);
		}
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	style.WindowPadding = tmpWindowPadding;
	style.WindowBorderSize = tmpWindowBorderSize;


	/*if (ImGui::Begin("Window-A##ID"))
	{
		if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
			ImGuiInFocus = true; //LOG("O");
	
	
	}
	ImGui::End();*/

	//if (ImGui::Begin("Window-A##ID"))
	//{
	//	if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
	//		ImGuiInFocus = true; //LOG("O");
	//
	//	// NewBot
	//	{
	//		ImGui::Text("New Bots");
	//		static float timePeriod = 10;
	//		ImGui::DragFloat("Time Period (s)", &timePeriod, 0.01);
	//		if (timePeriod < 0) timePeriod = 0.001;
	//		//ImGui::SliderInt("Number", &x, 0, 12);
	//		if (ImGui::Button("Release")) {
	//			Bots.emplace_back(timePeriod * 1000);
	//		}
	//	}
	//
	//	if (ImGui::BeginTable("table_context_menu_3", 3))
	//	{
	//		ImGui::TableSetupColumn("Kill");
	//		ImGui::TableSetupColumn("Count");
	//		ImGui::TableSetupColumn("Time Peroid");
	//
	//		ImGui::TableHeadersRow();
	//		ImGui::TableNextRow();
	//
	//		for (int row = 0; row < Bots.size(); row++)
	//		{
	//			bool kill = false;
	//			ImGui::TableSetColumnIndex(0);
	//			(ImGui::Button(Bots[row].GetName().c_str()));
	//			kill = ImGui::IsItemClicked();
	//			ImGui::SameLine();
	//
	//			ImGui::TableSetColumnIndex(1);
	//			ImGui::Text("%d", Bots[row].GetCount());
	//			ImGui::SameLine();
	//
	//			ImGui::TableSetColumnIndex(2);
	//			ImGui::Text("%f", Bots[row].GetTimePeriod() / 1000);
	//
	//			ImGui::TableNextRow();
	//
	//			if (kill)
	//				Bots.erase(Bots.begin() + row);
	//		}
	//		ImGui::EndTable();
	//	}
	//
	//	/*{
	//		ImGui::Separator();
	//
	//		static int dimensions[2] = { 1,1 };
	//		static std::vector<std::vector<bool>> radio;
	//		if (ImGui::DragInt2("Dimensions", dimensions, 0.03))
	//		{
	//			if (dimensions[0] < 1) dimensions[0] = 1;
	//			if (dimensions[1] < 1) dimensions[1] = 1;
	//
	//			radio.clear();
	//			radio.resize(dimensions[0], std::vector<bool>(dimensions[1], 1));
	//		}
	//
	//		for (int j = 0; j < radio.size(); j++)
	//		{
	//			for (int i = 0; i < radio[j].size(); i++)
	//			{
	//				if (ImGui::RadioButton(("##" + std::to_string(j) + "," + std::to_string(i)).c_str(),
	//					radio[j][i]))
	//					radio[j][i] = !radio[j][i];
	//
	//				if (i < radio[j].size() - 1)
	//					ImGui::SameLine();
	//			}
	//		}
	//
	//		block.arr = radio;
	//		block.updateBounds();
	//	}*/
	//
	//}
	//ImGui::End();

	//if (ImGui::Begin("Window-B##ID"))
	//{
	//	if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered())
	//		ImGuiInFocus = true; //LOG("O");
	//
	//	ImGui::Button("Refresh");
	//	if (ImGui::IsItemClicked()) {
	//		fb::BotsDataSet.clear();
	//
	//		if (!fb::database) return;
	//		std::string node_name = "Bots";
	//
	//		firebase::database::DatabaseReference node_ref = fb::database->GetReference(node_name.c_str());
	//
	//		node_ref.GetValue().OnCompletion([](const firebase::Future<firebase::database::DataSnapshot>& result) {
	//			if (result.status() == firebase::kFutureStatusComplete)
	//			{
	//				//const firebase::Variant& variant = result.result()->value();
	//				//Log(variant);
	//				auto var = result.result()->value();
	//
	//				if (var.is_map())
	//				{
	//					for (auto& [key, val] : var.map())
	//					{
	//						if (val.is_string())
	//						{
	//							fb::BotsDataSet.emplace_back(val.string_value());
	//							fb::LoadNode(fb::BotsDataSet.back());
	//						}
	//					}
	//				}
	//
	//				/*LOG("\n\n----------------------------\n");
	//				Log(glob_variant);
	//				if (glob_variant.is_map()
	//					) {
	//
	//					LOG("\n\nMap: ");
	//					for (auto& [key, val] : glob_variant.map())
	//					{
	//						if (val.is_int64() == false)
	//							break;
	//
	//						double v = val.int64_value();
	//						LOG(v << ", ");
	//					}
	//
	//				}
	//				else LOG("nope");*/
	//			}
	//			});
	//
	//	}
	//
	//	ImGui::Text("Bots");
	//	for (auto& bot : fb::BotsDataSet)
	//	{
	//		if (ImGui::TreeNode(bot.name.c_str()))
	//		{
	//			if (ImGui::Button("Load"))
	//				fb::LoadNode(bot);
	//
	//			for(int i = 0; i < bot.data.size(); i++)
	//				ImGui::Text("%d", bot.data[i]);
	//
	//			ImGui::TreePop();
	//		}
	//	}
	//
	//}
	//ImGui::End();
}

void App::Options() {

	if (ImGuiInFocus) return;

	if ((0 <= GUI::BlockPicked.x && GUI::BlockPicked.x < 4) &&
		(0 <= turn && turn < 4 && turn == GUI::BlockPicked.x) &&
		(sf::Mouse::isButtonPressed(sf::Mouse::Left)))
	{
		using namespace GUI;

		int i = BlockPicked.y;
		std::vector<Block>* vec = nullptr;

		if (BlockPicked.x == 0)      vec = &GreenBlocks;
		else if (BlockPicked.x == 1) vec = &RedBlocks;
		else if (BlockPicked.x == 2) vec = &BlueBlocks;
		else if (BlockPicked.x == 3) vec = &YellowBlocks;

		if (vec && 0 <= i && i < (*vec).size())
		{
			auto& block = (*vec)[i];
			auto& area = PlayerAreas[turn];

			sf::Vector2f pos = sf::Vector2f(
				(int)(GUI::cursorInSim().x - block.getBounds().getSize().x / 2/*.5 */),
				(int)(GUI::cursorInSim().y - block.getBounds().getSize().y / 2/*.5*/));

			block.setPos(pos);

			// Snap on Grid
			{
				if (GridBack.getGlobalBounds().intersects(block.getBounds()))
				{
					static sf::Vector2f prev;

					sf::Vector2f GridSpritePos = GridSprite.getPosition() + sf::Vector2f(2, 2);
					block.setPos(GUI::trim(pos, 40) + (GridSpritePos - GUI::trim(GridSpritePos, 40)));

					if ((sf::Vector2i)prev != (sf::Vector2i)block.getPos())
					{
						if (soundDrag.getStatus() == sf::Sound::Playing)
							soundDrag.setPlayingOffset(sf::milliseconds(19));
						else
							soundDrag.play();
					}

					prev = block.getPos();
				}
			}


			// Don't go in someone else's House
			{
				auto& gArea = GUI::PlayerAreas[0];
				auto& rArea = GUI::PlayerAreas[1];
				auto& bArea = GUI::PlayerAreas[2];
				auto& yArea = GUI::PlayerAreas[3];

				if (BlockPicked.x == 0)
				{
					if (block.getBounds().top < (bArea.getPosition().y + bArea.getSize().y))
						block.setPos(block.getPos() - sf::Vector2f(0, block.getBounds().top - (bArea.getPosition().y + bArea.getSize().y)));

					if (yArea.getPosition().x < block.getBounds().left + block.getBounds().width)
						block.setPos(block.getPos() - sf::Vector2f((block.getBounds().left + block.getBounds().width) - yArea.getPosition().x, 0));

					int x = 0, y = 0;
					if (block.getBounds().top < gArea.getPosition().y)
						if (block.getBounds().left < rArea.getPosition().x + rArea.getSize().x)
							x = block.getBounds().left - (rArea.getPosition().x + rArea.getSize().x);

					if (block.getBounds().left < rArea.getPosition().x + rArea.getSize().x)
						if (block.getBounds().top < rArea.getPosition().y + rArea.getSize().y)
							y = block.getBounds().top - (rArea.getPosition().y + rArea.getSize().y);

					if (std::abs(x) < std::abs(y)) y = 0; else x = 0;
					block.setPos(block.getPos() - sf::Vector2f(x, y));
				}

				if (BlockPicked.x == 1)
				{
					if (gArea.getPosition().y < block.getBounds().top + block.getBounds().height)
						block.setPos(block.getPos() - sf::Vector2f(0, (block.getBounds().top + block.getBounds().height) - (gArea.getPosition().y)));

					if (yArea.getPosition().x < block.getBounds().left + block.getBounds().width)
						block.setPos(block.getPos() - sf::Vector2f((block.getBounds().left + block.getBounds().width) - yArea.getPosition().x, 0));

					int x = 0, y = 0;
					if (block.getBounds().top < bArea.getPosition().y + bArea.getSize().y)
						if (bArea.getPosition().x < block.getBounds().left + block.getBounds().width)
							x = (block.getBounds().left + block.getBounds().width) - bArea.getPosition().x;

					if (bArea.getPosition().x < block.getBounds().left + block.getBounds().width)
						if (block.getBounds().top < bArea.getPosition().y + bArea.getSize().y)
							y = block.getBounds().top - (bArea.getPosition().y + bArea.getSize().y);

					if (std::abs(x) < std::abs(y)) y = 0; else x = 0;
					block.setPos(block.getPos() - sf::Vector2f(x, y));
				}

				if (BlockPicked.x == 2)
				{
					if (gArea.getPosition().y < block.getBounds().top + block.getBounds().height)
						block.setPos(block.getPos() - sf::Vector2f(0, (block.getBounds().top + block.getBounds().height) - (gArea.getPosition().y)));

					if (block.getBounds().left < rArea.getPosition().x + rArea.getSize().x)
						block.setPos(block.getPos() - sf::Vector2f(block.getBounds().left - (rArea.getPosition().x + rArea.getSize().x), 0));

					int x = 0, y = 0;
					if (yArea.getPosition().y < block.getBounds().top + block.getBounds().height)
						if (yArea.getPosition().x < block.getBounds().left + block.getBounds().width)
							x = (block.getBounds().left + block.getBounds().width) - yArea.getPosition().x;

					if (yArea.getPosition().x < block.getBounds().left + block.getBounds().width)
						if (yArea.getPosition().y < block.getBounds().top + block.getBounds().height)
							y = (block.getBounds().top + block.getBounds().height) - yArea.getPosition().y;

					if (std::abs(x) < std::abs(y)) y = 0; else x = 0;
					block.setPos(block.getPos() - sf::Vector2f(x, y));
				}

				if (BlockPicked.x == 3)
				{
					if (block.getBounds().top < bArea.getPosition().y + bArea.getSize().y)
						block.setPos(block.getPos() - sf::Vector2f(0, block.getBounds().top - (bArea.getPosition().y + bArea.getSize().y)));

					if (block.getBounds().left < rArea.getPosition().x + rArea.getSize().x)
						block.setPos(block.getPos() - sf::Vector2f(block.getBounds().left - (rArea.getPosition().x + rArea.getSize().x), 0));

					int x = 0, y = 0;
					if (gArea.getPosition().y < block.getBounds().top + block.getBounds().height)
						if (block.getBounds().left < gArea.getPosition().x + gArea.getSize().x)
							x = block.getBounds().left - (gArea.getPosition().x + gArea.getSize().x);

					if (block.getBounds().left < gArea.getPosition().x + gArea.getSize().x)
						if (gArea.getPosition().y < block.getBounds().top + block.getBounds().height)
							y = (block.getBounds().top + block.getBounds().height) - gArea.getPosition().y;

					if (std::abs(x) < std::abs(y)) y = 0; else x = 0;
					block.setPos(block.getPos() - sf::Vector2f(x, y));
				}
			}


			// Don't go outside the Screen House
			{
				if (block.getBounds().left < GUI::visibleArea().left)
					block.setPos(block.getPos() - sf::Vector2f(block.getBounds().left - GUI::visibleArea().left, 0));
				else
					if (GUI::visibleArea().left + GUI::visibleArea().width < block.getBounds().left + block.getBounds().width)
						block.setPos(block.getPos() - sf::Vector2f(-(GUI::visibleArea().left + GUI::visibleArea().width) + (block.getBounds().left + block.getBounds().width), 0));

				if (block.getBounds().top < GUI::visibleArea().top)
					block.setPos(block.getPos() - sf::Vector2f(0, block.getBounds().top - GUI::visibleArea().top));
				else
					if (GUI::visibleArea().top + GUI::visibleArea().height < block.getBounds().top + block.getBounds().height)
						block.setPos(block.getPos() - sf::Vector2f(0, -(GUI::visibleArea().top + GUI::visibleArea().height) + (block.getBounds().top + block.getBounds().height)));
			}

			static Timer t_writeLatest;
			if (t_writeLatest.repeatEvery(40))
				fb::WriteLastestBotData();
		}
	}

}

void App::Threads()
{
	for (int i = 0; i < futures.size(); i++) {

		std::future_status status = futures[i].wait_for(std::chrono::milliseconds(10));

		if (status == std::future_status::ready) //{
		// The future is ready (thread completed or value available)
		//	LOG("ready");
			futures.erase(futures.begin() + i--);
		//}
		//else if (status == std::future_status::timeout) {
		//	// The future is not ready within the specified duration
		//	LOG("timeout");
		//}
		//else if (status == std::future_status::deferred) {
		//	// The future is deferred (using std::promise and std::async(launch::deferred))
		//	LOG("deferred");
		//}

	}
}

void App::Update()
{
	using namespace GUI;

	// Drag
	{
		if (dragBool) {
			GUI::Drag();
			GUI::colorBrightLineGrid();
		}
	}


	// BlocksData
	{
		fb::UpdateLatestBotResults();

#if 0
		fb::WriteAllBloackSets();

		fb::UpdateWriteResults();
#else

		fb::LoadLastestBotData();

		fb::LoadAllBloackSets();

		fb::UpdateLoadResults();
#endif
	}

	{
		if(0 <= turn && turn < 4)
		{
			using namespace GUI;
			auto& area = PlayerAreas[turn];

			for (int i = 0; i < 4; i++)
			{

				sf::Color col = PlayerAreas[i].getFillColor();
				col.a = 60;
				PlayerAreas[i].setFillColor(col);
			}

			static float phase = 0;
			phase = phase + 0.1f;

			sf::Color col = area.getFillColor();
			col.a = 20 * std::sin(phase) + 60;
			area.setFillColor(col);
		}
	}

	// Hover
	{
		bool found = false;
		static int off = 135;
		
		{
			for (int i = 0; i < GreenBlocks.size(); i++)
				if (GreenBlocks[i].getBounds().contains(GUI::cursorInSim()))
				{
					found = true;

					if (BlockPicked.x == -1 && ItemHoverd != sf::Vector2i(0, i))
					{
						if (soundHover.getStatus() == sf::Sound::Playing)
							soundHover.setPlayingOffset(sf::milliseconds(off));
						else
							soundHover.play();

						ItemHoverd = sf::Vector2i(0, i);
						HoverClick = false;
					}

					if (HoverClick == false && ItemHoverd == sf::Vector2i(0, i))
					{
						GreenBlocks[i].BeingHovered = true;
						break;
					}
				}

			if (found == false)
				for (int i = 0; i < RedBlocks.size(); i++)
					if (RedBlocks[i].getBounds().contains(GUI::cursorInSim()))
					{
						found = true;

						if (BlockPicked.x == -1 && ItemHoverd != sf::Vector2i(0, i))
						{
							if (soundHover.getStatus() == sf::Sound::Playing)
								soundHover.setPlayingOffset(sf::milliseconds(off));
							else
								soundHover.play();

							ItemHoverd = sf::Vector2i(0, i);
							HoverClick = false;
						}

						if (HoverClick == false && ItemHoverd == sf::Vector2i(0, i))
						{
							RedBlocks[i].BeingHovered = true;
							break;
						}
					}

			if (found == false)
				for (int i = 0; i < BlueBlocks.size(); i++)
					if (BlueBlocks[i].getBounds().contains(GUI::cursorInSim()))
					{
						found = true;

						if (BlockPicked.x == -1 && ItemHoverd != sf::Vector2i(0, i))
						{
							if (soundHover.getStatus() == sf::Sound::Playing)
								soundHover.setPlayingOffset(sf::milliseconds(off));
							else
								soundHover.play();

							ItemHoverd = sf::Vector2i(0, i);
							HoverClick = false;
						}

						if (HoverClick == false && ItemHoverd == sf::Vector2i(0, i))
						{
							BlueBlocks[i].BeingHovered = true;
							break;
						}
					}

			if (found == false)
				for (int i = 0; i < YellowBlocks.size(); i++)
					if (YellowBlocks[i].getBounds().contains(GUI::cursorInSim()))
					{
						found = true;

						if (BlockPicked.x == -1 && ItemHoverd != sf::Vector2i(0, i))
						{
							if (soundHover.getStatus() == sf::Sound::Playing)
								soundHover.setPlayingOffset(sf::milliseconds(off));
							else
								soundHover.play();

							ItemHoverd = sf::Vector2i(0, i);
							HoverClick = false;
						}

						if (HoverClick == false && ItemHoverd == sf::Vector2i(0, i))
						{
							YellowBlocks[i].BeingHovered = true;
							break;
						}
					}
		}

		if (found == false)
		{
			HoverClick = false;
			ItemHoverd = sf::Vector2i(-1, -1);
		}
	}

	// PlayerArea
	/*updatePlayerArea();*/
}

void App::Render()
{
	using namespace GUI;

	app.setView(view);

	app.clear(backColor);

	drawGrid();

	drawPlayerArea();

	drawBlocks();


	ImGui::SFML::Render(app); // Last Thing to render

	// GUI::app.display(); // Inside void App::EndFrame()
}

void App::EndFrame()
{
	GUI::app.display();


	// FPS
	{
		static Timer FPS_Timer;
		double duration = FPS_Timer.duration();
		GUI::app.setTitle("Blocks   " + std::to_string(duration) + " | " + std::to_string(1000 / duration));
		FPS_Timer.reset();
	}
}

//void App::arrangeBlocks()
//{
//	// Green
//	{
//		auto& gArea = GUI::PlayerAreas[0];
//		sf::Vector2f offSet = gArea.getPosition() + sf::Vector2f(15, 8);
//
//		GreenBlocks.reserve(Block::no_of_Shapes);
//		GreenBlocks.emplace_back(Block::BlockType(0), Block::green);
//		GreenBlocks.back().setPos(sf::Vector2f(offSet));
//		for (int i = 1; i < 4; i++)
//		{
//			int x = GreenBlocks.back().getBounds().left + GreenBlocks.back().getBounds().width;
//			GreenBlocks.emplace_back(Block::BlockType(i), Block::green);
//			GreenBlocks.back().setPos(sf::Vector2f(offSet.x + x, offSet.y));
//		}
//
//		GreenBlocks[0].setPos(GreenBlocks[0].getPos() + sf::Vector2f( 0, 70));
//		GreenBlocks[1].setPos(GreenBlocks[1].getPos() + sf::Vector2f(22, 70));
//		GreenBlocks[3].setPos(offSet + sf::Vector2f(0, 10));
//		GreenBlocks[2].rotate();
//		GreenBlocks[2].setPos(GreenBlocks[2].getPos() + sf::Vector2f(18, 0));
//
//		int x = GreenBlocks[2].getBounds().left + GreenBlocks[2].getBounds().width;
//		GreenBlocks.emplace_back(Block::BlockType(4), Block::green);
//		GreenBlocks.back().setPos(sf::Vector2f(offSet.x + x, offSet.y + 35));
//
//		for (int i = 5; i < Block::no_of_Shapes; i++)
//		{
//			int x = GreenBlocks.back().getBounds().left + GreenBlocks.back().getBounds().width - 7;
//			GreenBlocks.emplace_back(Block::BlockType(i), Block::green);
//			GreenBlocks.back().setPos(sf::Vector2f(offSet.x + x, offSet.y + 35 * (i != (+Block::no_of_Shapes - 1))));
//		}
//	}
//
//	// Blue
//	{
//		auto& bArea = GUI::PlayerAreas[2];
//		sf::Vector2f offSet = bArea.getPosition() + sf::Vector2f(15, 8);
//
//		BlueBlocks.reserve(Block::no_of_Shapes);
//		BlueBlocks.emplace_back(Block::BlockType(0), Block::blue);
//		BlueBlocks.back().setPos(sf::Vector2f(offSet));
//		for (int i = 1; i < 4; i++)
//		{
//			int x = BlueBlocks.back().getBounds().left + BlueBlocks.back().getBounds().width;
//			BlueBlocks.emplace_back(Block::BlockType(i), Block::blue);
//			BlueBlocks.back().setPos(sf::Vector2f(/*offSet.x*/15 + x, offSet.y));
//		}
//
//		BlueBlocks[0].setPos(BlueBlocks[0].getPos() + sf::Vector2f( 0, 70));
//		BlueBlocks[1].setPos(BlueBlocks[1].getPos() + sf::Vector2f(22, 70));
//		BlueBlocks[3].setPos(offSet + sf::Vector2f(0, 10));
//		BlueBlocks[2].rotate();
//		BlueBlocks[2].setPos(BlueBlocks[2].getPos() + sf::Vector2f(18, 0));
//
//		int x = BlueBlocks[2].getBounds().left + BlueBlocks[2].getBounds().width;
//		BlueBlocks.emplace_back(Block::BlockType(4), Block::blue);
//		BlueBlocks.back().setPos(sf::Vector2f(/*offSet.x*/15 + x, offSet.y + 35));
//
//		for (int i = 5; i < Block::no_of_Shapes; i++)
//		{
//			int x = BlueBlocks.back().getBounds().left + BlueBlocks.back().getBounds().width - 4;
//			BlueBlocks.emplace_back(Block::BlockType(i), Block::blue);
//			BlueBlocks.back().setPos(sf::Vector2f(/*offSet.x*/15 + x, offSet.y + 35 * (i != (+Block::no_of_Shapes - 1))));
//		}
//		BlueBlocks.back().setPos(BlueBlocks.back().getPos() + sf::Vector2f(7, 0));
//	}
//
//	// Red
//	{
//		auto& rArea = GUI::PlayerAreas[1];
//		sf::Vector2f offSet = rArea.getPosition() + sf::Vector2f(40, 40);
//
//		RedBlocks.reserve(Block::no_of_Shapes);
//		RedBlocks.emplace_back(Block::BlockType(0), Block::red);
//		RedBlocks.back().setPos(offSet);
//		for (int i = 1; i < Block::no_of_Shapes; i++)
//		{
//			int y = RedBlocks.back().getBounds().top + RedBlocks.back().getBounds().height;
//			RedBlocks.emplace_back(Block::BlockType(i), Block::red);
//			if (6 < i) { RedBlocks.back().rotate(); RedBlocks.back().rotate(); }
//			RedBlocks.back().setPos(sf::Vector2f(offSet.x + 180 * (6 < i), /*offSet.y*/ 45 + y - 650 * (i == 7)));
//		}
//	}
//
//	// Yellow
//	{
//		auto& yArea = GUI::PlayerAreas[3];
//		sf::Vector2f offSet = yArea.getPosition() + sf::Vector2f(40, 40);
//
//		YellowBlocks.reserve(Block::no_of_Shapes);
//		YellowBlocks.emplace_back(Block::BlockType(0), Block::yellow);
//		YellowBlocks.back().setPos(offSet);
//		for (int i = 1; i < Block::no_of_Shapes; i++)
//		{
//			int y = YellowBlocks.back().getBounds().top + YellowBlocks.back().getBounds().height;
//			YellowBlocks.emplace_back(Block::BlockType(i), Block::yellow);
//			if (6 < i) { YellowBlocks.back().rotate(); YellowBlocks.back().rotate(); }
//			YellowBlocks.back().setPos(sf::Vector2f(offSet.x + 180 * (6 < i), /*offSet.y*/ 45 + y - 650 * (i == 7)));
//		}
//	}
//}