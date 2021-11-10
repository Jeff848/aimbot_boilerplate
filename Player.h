#pragma once

class Vector3;
class Player
{
private:
	Player();

public:
	static int* GetMaxPlayer();
	static Player* GetPlayer(int index);

	Vector3* GetOrigin();
	Vector3* GetViewOffset();
	Vector3* GetBonePos(int boneID);
	int* GetHealth();
	int* GetTeam();
};