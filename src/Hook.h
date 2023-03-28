#pragma once

namespace AttackDataTransitionFix
{
	class AnimEventHook
	{
		using EventResult = RE::BSEventNotifyControl;

	public:
		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> AnimEventSinkVtbl{ RE::VTABLE_PlayerCharacter[2] };
			_ProcessEvent = AnimEventSinkVtbl.write_vfunc(0x1, ProcessEvent);
			INFO("Hook Process Animation Event!");
		}

	private:
		static EventResult ProcessEvent(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink, RE::BSAnimationGraphEvent* a_event, RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource);

		static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
	};

	class AttackDataUpdateHook
	{
	public:
		static void InstallHook()
		{
			REL::Relocation<uintptr_t> hook{ REL::ID(26729) };  //1403E88F0
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(1 << 4);
			_UpdateAttackData = trampoline.write_call<5>(hook.address() + 0x3A, UpdateAttackData);
		};

	private:
		static bool UpdateAttackData(void* a1, void* a2, int a_flag, const RE::BSFixedString& a_animationEvent, RE::NiPointer<RE::BGSAttackData>& a_attackDataPtr);

		static inline REL::Relocation<decltype(UpdateAttackData)> _UpdateAttackData;
	};
}