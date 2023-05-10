#include "DataHandler.h"

namespace AttackDataTransitionFix
{

	bool DelayedAttackDataHandler::ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!FindAttackData(playerRef, a_animationEvent))
			return false;

		const std::string BoolVarName = "ADTF_IsDelayed_" + std::string(a_animationEvent.c_str());
		bool shouldDelayed = false;

		return playerRef->GetGraphVariableBool(BoolVarName, shouldDelayed) && shouldDelayed;
	}

	bool DelayedAttackDataHandler::ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent, const RE::BSFixedString& a_payload)
	{
		constexpr char eventName[] = "ADTF_ReapplyAttackData";

		if (!playerRef)
			return false;

		auto playerHighData = playerRef && playerRef->currentProcess ? playerRef->currentProcess->high : nullptr;
		if (!playerHighData || !playerHighData->attackData)
			return false;

		if (_stricmp(eventName, a_animationEvent.c_str()) == 0) {
			auto attackData = FindAttackData(playerRef, a_payload);
			if (attackData && playerHighData->attackData.get() != attackData) {
				playerHighData->attackData.reset(attackData);
				DEBUG("Reapply Attack Data \"{}\" in event \"{}\"", a_payload, a_animationEvent.c_str());
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
			auto pair = attackDataMap->attackDataMap.find(a_animationEvent);
			if (pair != attackDataMap->attackDataMap.end() && pair->second) {
				return pair->second.get();
			}
		}

		return nullptr;
	}

}