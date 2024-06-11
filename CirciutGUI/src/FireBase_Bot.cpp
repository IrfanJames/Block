
#include <string>

#include "LOG.hpp"
#include "FireBase_my_Utils.hpp"

#include "FireBase_Bot.hpp"

Bot::Bot(int timePeriod)
{
	mTimePeriod = timePeriod;
	mTimer.reset();

	mSerial = rand() % 100;
	mName = "Bot-" + std::to_string(mSerial);

	auto root_ref = fb::database->GetReference("Bots");
	root_ref.PushChild().SetValue(mName);
}

Bot::~Bot()
{
}

void Bot::update()
{
	if (mTimer.repeatEvery(mTimePeriod))
	{
		auto root_ref = fb::database->GetReference("BotData").Child(mName);

		int64_t val = mSerial + 100 * mCount++;
		root_ref.PushChild().SetValue(firebase::Variant(val));

		LOG("\nMinion-" << mName << "\t: " << val);
	}
}
