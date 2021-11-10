#include "Player.h"
#include "Vector3.h"
#include "Offsets.h"
#include <cstdint>
#include <Windows.h>
#include <iostream>

Player* Player::GetPlayer(int index) {
	static uint32_t moduleBase = (uint32_t)(GetModuleHandle("client.dll"));
	static uint32_t entityList = moduleBase + hazedumper::signatures::dwEntityList;
	//TODO
	
	return nullptr;
}

int* Player::GetMaxPlayer() {
	static uint32_t moduleBase = (uintptr_t)(GetModuleHandle("engine.dll"));
	return (int*)(*(uint32_t*)(moduleBase + hazedumper::signatures::dwClientState) + hazedumper::signatures::dwClientState_MaxPlayer);
}

Vector3* Player::GetOrigin() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecOrigin);
}

Vector3* Player::GetViewOffset() {
	return (Vector3*)(*(uintptr_t*)this + hazedumper::netvars::m_vecViewOffset);
}

Vector3* Player::GetBonePos(int boneID) {
	uint32_t boneMatrix = *(uint32_t*)(*(uint32_t*)this + hazedumper::netvars::m_dwBoneMatrix);
	static Vector3 bonePos;
	//TODO

	return &bonePos;
}

int* Player::GetHealth() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iHealth);
}

int* Player::GetTeam() {
	return (int*)(*(uintptr_t*)this + hazedumper::netvars::m_iTeamNum);
}