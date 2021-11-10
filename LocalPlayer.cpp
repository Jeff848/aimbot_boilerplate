#include "LocalPlayer.h"
#include "Vector3.h"
#include "Offsets.h"
#include <cstdint>
#include <Windows.h>
#include <math.h>

LocalPlayer* LocalPlayer::Get() {
	static uint32_t clientModule = (uint32_t)(GetModuleHandle("client.dll"));
	static LocalPlayer* localPlayer = (LocalPlayer*)(clientModule + hazedumper::signatures::dwLocalPlayer);
	return localPlayer;
}

Vector3* LocalPlayer::GetOrigin() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}

Vector3* LocalPlayer::GetViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset);
}

int* LocalPlayer::GetHealth() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iHealth);
}

int* LocalPlayer::GetTeam() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum);
}

void LocalPlayer::AimAt(Vector3* target) {
	static uint32_t engineModule = (uint32_t)(GetModuleHandle("engine.dll"));
	static Vector3* viewAngles = (Vector3*)(*(uint32_t*)(engineModule + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_ViewAngles);

	//TODO
}

float LocalPlayer::GetDistance(Vector3* other) {
	//TODO
}