#pragma once
#include "DKUtil/Config.hpp"

namespace AttackDataTransitionFix
{
	using namespace DKUtil::Alias;

	class DelayedAttackDataHandler : public dku::model::Singleton<DelayedAttackDataHandler>
	{
	public:
		friend dku::model::Singleton<DelayedAttackDataHandler>;

		DelayedAttackDataHandler();

		bool ShouldDelayAttack(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
		bool ReapplyAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);
		static RE::BGSAttackData* FindAttackData(RE::Actor* playerRef, const RE::BSFixedString& a_animationEvent);

	private:
		std::map<std::string, const std::vector<std::string>> delayedAttackDataMap;
		std::map<std::string, const std::string> reapplyAttackDataMap;
	};
}