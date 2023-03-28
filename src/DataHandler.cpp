#include "DataHandler.h"

namespace AttackDataTransitionFix
{
	const char powerAttackEvent[] = "attackPowerStartInPlace";
	const char MCO_PowerAttackEvent[] = "MCO_PowerAttackInitiate";

	DelayedAttackDataHandler::DelayedAttackDataHandler()
	{
		delayedAttackDataMap.emplace(powerAttackEvent, std::vector<std::string>{ "IsAttacking" });
		reapplyAttackDataMap.emplace(MCO_PowerAttackEvent, powerAttackEvent);
	}

	bool DelayedAttackDataHandler::ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		return delayedAttackDataMap.find(a_animationEvent.c_str()) != delayedAttackDataMap.end() &&
		       FindAttackData(playerRef, a_animationEvent);
	}

	bool DelayedAttackDataHandler::ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!playerRef)
			return false;

		auto playerHighData = playerRef && playerRef->currentProcess ? playerRef->currentProcess->high : nullptr;
		if (!playerHighData || !playerHighData->attackData)
			return false;

		auto item = reapplyAttackDataMap.find(a_animationEvent.c_str());
		if (item != reapplyAttackDataMap.end()) {
			auto attackData = FindAttackData(playerRef, item->second);
			if (attackData && playerHighData->attackData.get() != attackData) {
				playerHighData->attackData.reset(attackData);
				DEBUG("Reapply Attack Data \"{}\" in event \"{}\"", item->second, item->first);
				return true;
			}
		}

		return false;
	}

	RE::BGSAttackData* DelayedAttackDataHandler::FindAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!playerRef) {
			return nullptr;
		}

		auto race = playerRef->GetRace();
		auto attackDataMap = race ? race->attackDataMap.get() : nullptr;
		if (attackDataMap) {
			auto item = attackDataMap->attackDataMap.find(a_animationEvent);
			if (item != attackDataMap->attackDataMap.end() && item->second) {
				return item->second.get();
			}
		}

		return nullptr;
	}
}