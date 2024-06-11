#pragma once

#include <string>
#include <vector>

#include "firebase/app.h"
#include "firebase/database.h"
#include "firebase/auth.h"
#include "firebase/storage.h"

#include "Timer.hpp"

namespace fb {

	extern firebase::App* fireBase_app;
	extern firebase::database::Database* database;
	extern firebase::storage::Storage* storage;

	struct Data
	{
		char rot = 0;
		float posX = 0;
		float posY = 0;

		Data() = default;

		Data(char rot, float posX, float posY) :
			rot(rot), posX(posX), posY(posY)
		{
		}
	};

	struct BotsData
	{
		bool isInputDone = true;
		bool isOutputDone = true;
		Data data[30];
		Timer timer;

		firebase::Future<void> writeResult;
		firebase::Future<firebase::database::DataSnapshot> result;
	};

	extern BotsData LastestBotData;
	void LoadLastestBotData();
	void WriteLastestBotData();
	void UpdateLatestBotResults();

	extern BotsData allBlockData;
	//extern BotsData greenData;
	//extern BotsData redData;
	//extern BotsData blueData;
	//extern BotsData yellowData;
	extern void LoadAllBloackSets();
	extern void WriteAllBloackSets();
	extern void UpdateLoadResults();
	extern void UpdateWriteResults();


	extern void InitializeFireBase();
	extern void Log(const firebase::Variant& variant, unsigned long indentLevel = 0);
	extern void DeleteNode(const std::string& node);
	extern void ReadNode(const std::string& node_name);
	extern void WriteBlockSet(int BlockSet);
	extern void LoadBlockSet(int BlockSet);

	/*extern firebase::auth::Auth* auth_my;
	extern void authh();*/
}