#pragma once
class PipeGenerator
{
public:
	static void SpawnPipes();
	static void SpawnPipeSet(float pipeSpawnX);
	static const int UPDATES_PER_GENERATION = 75;
	static const int PIPE_RANDOM_SPAWN_Y = 360;
	static const float PIPE_OFFSET_Y;
	static const float PIPE_SPAWN_X;
	static const float PIPE_SPACING_X;
	static const float GAP_RADIUS;
	static const float PIPE_SCALE;
};

