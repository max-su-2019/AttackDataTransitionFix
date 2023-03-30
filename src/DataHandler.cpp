#include "DataHandler.h"

namespace AttackDataTransitionFix
{
	//attackDataName: Name of the attack data to delay transition
	//variableNames: List of the graph variable boolen names that attach to the behavior attack state
	//reapplyEvent: The animation event name that retrigger the attack data trasition
	void DelayedAttackDataHandler::InsertDelayedAttackData(const std::string& attackDataName, const std::vector<std::string> variableNames, const std::string& reapplyEvent)
	{
		delayedAttackDataMap.emplace(attackDataName, variableNames);
		reapplyAttackDataMap.emplace(reapplyEvent, attackDataName);
	}

	DelayedAttackDataHandler::DelayedAttackDataHandler()
	{
		InsertDelayedAttackData("attackPowerStartInPlace", { "MCO_IsAttacking", "MCO_InSprintAttack" }, "MCO_PowerAttackEntry");
		InsertDelayedAttackData("attackPowerStartForwardH2HRightHand", { "MCO_IsAttacking", "MCO_InSprintAttack" }, "MCO_PowerAttackH2HEntry");

		InsertDelayedAttackData("attackStart", { "MCO_IsPowerAttacking", "MCO_InSprintPowerAttack" }, "MCO_AttackEntry");
		InsertDelayedAttackData("AttackStartH2HRight", { "MCO_IsPowerAttacking", "MCO_InSprintPowerAttack" }, "MCO_AttackH2HEntry");
	}

	bool DelayedAttackDataHandler::ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!FindAttackData(playerRef, a_animationEvent))
			return false;

		auto pair = delayedAttackDataMap.find(a_animationEvent.c_str());
		if (pair != delayedAttackDataMap.end()) {
			for (const auto& varName : pair->second) {
				bool result = false;
				if (playerRef->GetGraphVariableBool(varName, result) && result) {
					return true;
				}
			}
		}

		return false;
	}

	bool DelayedAttackDataHandler::ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent)
	{
		if (!playerRef)
			return false;

		auto playerHighData = playerRef && playerRef->currentProcess ? playerRef->currentProcess->high : nullptr;
		if (!playerHighData || !playerHighData->attackData)
			return false;

		auto pair = reapplyAttackDataMap.find(a_animationEvent.c_str());
		if (pair != reapplyAttackDataMap.end()) {
			auto attackData = FindAttackData(playerRef, pair->second);
			if (attackData && playerHighData->attackData.get() != attackData) {
				playerHighData->attackData.reset(attackData);
				DEBUG("Reapply Attack Data \"{}\" in event \"{}\"", pair->second, pair->first);
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