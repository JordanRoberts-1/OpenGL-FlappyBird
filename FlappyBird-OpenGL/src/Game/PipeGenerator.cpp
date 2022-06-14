#include "PipeGenerator.h"

#include "../ECS/Entity.h"
#include "SceneManager.h"
#include "../ECS/PhysicsComponent.h"
#include "../ECS/PipeComponent.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/BoxColliderComponent.h"
#include "../ECS/ScoreTrackingComponent.h"
#include <iostream>

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

	glm::vec2 gapPosition = glm::vec2(600.0f, std::rand() % 360 + 300.0f);
	float gapRadius = 100.0f;

	std::unique_ptr<Entity> topPipe = std::make_unique<Entity>(std::string("top_pipe.png"), std::string("Basic.glsl"), glm::vec3(600.0f, 750.0f, 1.0f), glm::vec2(4.0f));
	glm::vec3 topPosition = glm::vec3(gapPosition.x, gapPosition.y + gapRadius, 1);
	topPipe->GetTransform()->SetPosition(topPosition);

	//Setup components for the top pipes
	PhysicsComponent* topPipePhysics = topPipe->AddComponent<PhysicsComponent>(topPipe.get());
	topPipePhysics->SetBoolGravity(false);
	topPipe->AddComponent<PipeComponent>(topPipe.get());
	topPipe->AddComponent<BoxColliderComponent>(topPipe.get());
	topPipe->AddComponent<ScoreTrackingComponent>(topPipe.get());

	std::unique_ptr<Entity> bottomPipe = std::make_unique<Entity>(std::string("bottom_pipe.png"), std::string("Basic.glsl"), glm::vec3(600.0f, -100.0f, 1.0f), glm::vec2(4.0f));
	glm::vec3 bottomPosition = glm::vec3(gapPosition.x, gapPosition.y - gapRadius - bottomPipe->GetTransform()->GetScaledSize().y, 1);
	bottomPipe->GetTransform()->SetPosition(bottomPosition);


	//Set up the components for the bottom pipes
	PhysicsComponent* bottomPipePhysics = bottomPipe->AddComponent<PhysicsComponent>(bottomPipe.get());
	bottomPipePhysics->SetBoolGravity(false);
	bottomPipe->AddComponent<PipeComponent>(bottomPipe.get());
	bottomPipe->AddComponent<BoxColliderComponent>(bottomPipe.get());


	SceneManager& instance = SceneManager::GetInstance();
	instance.AddObject(std::move(topPipe));
	instance.AddObject(std::move(bottomPipe));
}
