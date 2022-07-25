#include "PipeGenerator.h"

#include "../ECS/Entity.h"
#include "SceneManager.h"
#include "../ECS/PhysicsComponent.h"
#include "../ECS/PipeComponent.h"
#include "../ECS/TransformComponent.h"
#include "../ECS/BoxColliderComponent.h"
#include "../ECS/ScoreTrackingComponent.h"
#include <iostream>

const float PipeGenerator::PIPE_SPAWN_X = 600.0f;
const float PipeGenerator::PIPE_SPACING_X = 375.0f;
const float PipeGenerator::PIPE_OFFSET_Y = 300.0f;
const float PipeGenerator::GAP_RADIUS = 100.0f;
const float PipeGenerator::PIPE_SCALE = 4.0f;

void PipeGenerator::SpawnPipes()
{
	std::cout << "Spawning Pipes" << std::endl;

	SpawnPipeSet(PIPE_SPAWN_X);
	SpawnPipeSet(PIPE_SPAWN_X + PIPE_SPACING_X);
}

void PipeGenerator::SpawnPipeSet(float pipeSpawnX)
{
	glm::vec2 gapPosition = glm::vec2(pipeSpawnX, std::rand() % PIPE_RANDOM_SPAWN_Y + PIPE_OFFSET_Y);

	std::unique_ptr<Entity> topPipe = std::make_unique<Entity>(
		std::string("top_pipe.png"), std::string("Basic.glsl"), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(PIPE_SCALE)
		);

	glm::vec3 topPosition = glm::vec3(gapPosition.x, gapPosition.y + GAP_RADIUS, 1);
	topPipe->GetTransform()->SetPosition(topPosition);

	//Setup components for the top pipes
	PhysicsComponent* topPipePhysics = topPipe->AddComponent<PhysicsComponent>(topPipe.get());
	topPipePhysics->SetBoolGravity(false);
	topPipe->AddComponent<BoxColliderComponent>(topPipe.get());
	topPipe->AddComponent<ScoreTrackingComponent>(topPipe.get());

	PipeComponent* topPipeComponent = topPipe->AddComponent<PipeComponent>(topPipe.get());
	topPipeComponent->SetGapPosition(gapPosition);

	std::unique_ptr<Entity> bottomPipe = std::make_unique<Entity>(
		std::string("bottom_pipe.png"), std::string("Basic.glsl"), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(PIPE_SCALE)
		);

	glm::vec3 bottomPosition = glm::vec3(gapPosition.x, gapPosition.y - GAP_RADIUS - bottomPipe->GetTransform()->GetScaledSize().y, 1);
	bottomPipe->GetTransform()->SetPosition(bottomPosition);

	//Set up the components for the bottom pipes
	PhysicsComponent* bottomPipePhysics = bottomPipe->AddComponent<PhysicsComponent>(bottomPipe.get());
	bottomPipePhysics->SetBoolGravity(false);
	bottomPipe->AddComponent<BoxColliderComponent>(bottomPipe.get());

	PipeComponent* bottomPipeComponent = bottomPipe->AddComponent<PipeComponent>(bottomPipe.get());
	bottomPipeComponent->SetGapPosition(gapPosition);

	//Add the new objects to the scene
	SceneManager& instance = SceneManager::GetInstance();
	topPipeComponent->SetChildPipe(bottomPipeComponent);
	bottomPipeComponent->Init();
	topPipeComponent->Init();
	instance.AddObject(std::move(topPipe));
	instance.AddObject(std::move(bottomPipe));
}


