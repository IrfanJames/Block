#pragma once

#include "Timer.hpp"

class Bot {

public:

	Bot(int timePeriod = 100);
	~Bot();
	int64_t GetCount() { return mCount; }
	double GetTimePeriod() { return mTimePeriod; }
	const std::string& GetName() { return mName; }


	void update();

private:
	int mSerial;
	int64_t mCount = 1;
	std::string mName;

	Timer mTimer;
	double mTimePeriod;
};