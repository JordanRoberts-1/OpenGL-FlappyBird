#include "PipeGenerator.h"

#include "../ECS/Entity.h"
#include "SceneManager.h"
#include "../ECS/PhysicsComponent.h"
#include "../ECS/Pipes.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/BoxColliderComponent.h"
#include <iostream>

int PipeGenerator::s_UpdateCounter = 0;
const float PipeGenerator::PIPE_SPAWN_X = 600.0f;
const float PipeGenerator::PIPE_SPACING_X = 375.0f;
const float PipeGenerator::PIPE_OFFSET_Y = 300.0f;
const float PipeGenerator::GAP_RADIUS = 100.0f;
const float PipeGenerator::PIPE_SCALE = 4.0f;

void PipeGenerator::SpawnPipes()
{
	std::cout << "Spawning Pipes" << std::endl;

	SpawnPipeSet();
	SpawnPipeSet();
	SpawnPipeSet();
}

void PipeGenerator::SpawnPipeSet()
{
	//Setup Top pipe
	std::unique_ptr<Entity> topPipe = std::make_unique<Entity>(
		std::string("top_pipe.png"), std::string("Basic.glsl"), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(PIPE_SCALE)
		);

	//Setup the components for the Top Pipe
	PhysicsComponent* topPipePhysics = topPipe->AddComponent<PhysicsComponent>(topPipe.get());
	topPipe->AddComponent<BoxColliderComponent>(topPipe.get());
	TopPipeComponent* topPipeComponent = topPipe->AddComponent<TopPipeComponent>(topPipe.get());
	topPipePhysics->SetBoolGravity(false);

	//Setup Bottom Pipe
	std::unique_ptr<Entity> bottomPipe = std::make_unique<Entity>(
		std::string("bottom_pipe.png"), std::string("Basic.glsl"), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(PIPE_SCALE)
		);

	//Set up the components for the Bottom Pipes
	PhysicsComponent* bottomPipePhysics = bottomPipe->AddComponent<PhysicsComponent>(bottomPipe.get());
	bottomPipe->AddComponent<BoxColliderComponent>(bottomPipe.get());
	BottomPipeComponent* bottomPipeComponent = bottomPipe->AddComponent<BottomPipeComponent>(bottomPipe.get());
	bottomPipePhysics->SetBoolGravity(false);

	//Initialize the components
	bottomPipeComponent->Init();
	topPipeComponent->Init();
	topPipeComponent->SetBottomPipe(bottomPipeComponent);
	
	//Add the new objects to the scene
	SceneManager& instance = SceneManager::GetInstance();
	instance.AddObject(std::move(topPipe));
	instance.AddObject(std::move(bottomPipe));
}

void PipeGenerator::Update()
{
	if (s_UpdateCounter % UPDATES_PER_GENERATION == 0)
	{
		MovePipeSet();
	}
	s_UpdateCounter++;
}

void PipeGenerator::MovePipeSet()
{
	SceneManager& scene = SceneManager::GetInstance();

	std::cout << "Moving pipe set" << std::endl;
	
	//Find the furthest left pipe that can be reset and reset it
	for (const auto& object : scene.GetObjects())
	{
		TopPipeComponent* topPipe = object->GetComponent<TopPipeComponent>(TOPPIPECOMPONENT);

		if (topPipe && topPipe->CheckReset())
		{
			topPipe->ResetLocation();
			return;
		}
	}
}


