#pragma once
class PipeGenerator
{
public:
	static void Update();
private:
	static int s_UpdateCounter;
	static const int s_UPDATESPERGENERATION = 60;
private:
	static void SpawnPipes();
};
