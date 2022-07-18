
#include "PhysicsComponent.h"

TArray<FVector> UPhysicsComponent::GenerateTrajectory(FVector StartPos, FVector Velocity, float MaxTime, float TimeStep, float MinZValue /*= 0.0f*/)
{
	TArray<FVector> Trajectory;
	FVector GravityVector(0.0f, 0.0f, Gravity);

	for (float time = 0; time < MaxTime; time += TimeStep)
	{
		FVector position = StartPos + Velocity * time + GravityVector * time * time / 2;
		if (position.Z <= MinZValue)
			break;

		Trajectory.Add(position);
	}
	return Trajectory;

}

