#include "PipeGenerator.h"

#include "ECS/Entity.h"
#include "SceneManager.h"
#include "ECS/PhysicsComponent.h"
#include "ECS/PipeComponent.h"

int PipeGenerator::s_UpdateCounter = 0;

void PipeGenerator::Update()
{
	s_UpdateCounter++;
	if (s_UpdateCounter > s_UPDATESPERGENERATION)
	{
		SpawnPipes();
		s_UpdateCounter = 0;
	}
}

void PipeGenerator::SpawnPipes()
{
	std::cout << "Spawning Pipes" << std::endl;

	std::unique_ptr<Entity> topPipe = std::make_unique<Entity>(std::string("flappybirdpipe.png"), std::string("Basic.glsl"), glm::vec2(400.0f, 300.0f), glm::vec2(5.0f));

	//Setup components for the top pipes
	PhysicsComponent* topPipePhysics = topPipe->AddComponent<PhysicsComponent>(topPipe.get());
	topPipePhysics->SetBoolGravity(false);
	topPipe->AddComponent<PipeComponent>(topPipe.get());

	std::unique_ptr<Entity> bottomPipe = std::make_unique<Entity>(std::string("flappybirdpipe.png"), std::string("Basic.glsl"), glm::vec2(400.0f, 0.0f), glm::vec2(5.0f));

	//Set up the components for the bottom pipes
	PhysicsComponent* bottomPipePhysics = bottomPipe->AddComponent<PhysicsComponent>(bottomPipe.get());
	bottomPipePhysics->SetBoolGravity(false);
	bottomPipe->AddComponent<PipeComponent>(bottomPipe.get());

	SceneManager& instance = SceneManager::GetInstance();
	instance.AddObject(std::move(topPipe));
	instance.AddObject(std::move(bottomPipe));
}
