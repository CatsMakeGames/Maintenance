// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowAIBase.h"


#include "AI/AIInterface.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"

AShadowAIBase::AShadowAIBase()
{
  
}

/*void AShadowAIBase::BeginPlay()
{
    if(GetPawn() != nullptr)
    {
        if (GetPawn()->Implements<UAIInterface>() || (Cast<IAIInterface>(GetPawn()) != nullptr))
        {
           PatrolPoints = IAIInterface::Execute_GetPatrolPoints(GetPawn());
            if(PatrolPoints.Num() > 0 && GetBlackboardComponent() != nullptr) 
            {
                GetBlackboardComponent()->SetValueAsObject(TEXT("PatrolLocation"),PatrolPoints[FMath::RandRange(0,PatrolPoints.Num()-1)]);
            }
        }
    }
}*/

void AShadowAIBase::UpdatePerceivedActors(TArray<AActor*>SeenActors,TArray<AActor*>HeardActors)
{
    
    if (GetBlackboardComponent() != nullptr)
    {
        bool canSeeTarget = false;
        if(Target != nullptr)
        {
            if(SeenActors.Find(Target) != INDEX_NONE)
            {
                //still can see target, so we update last seen location
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastSeenLocation"),Target->GetActorLocation());
                canSeeTarget = true;
            }
            else
            {
                //no longer can see target
                GetBlackboardComponent()->ClearValue(TEXT("Target"));
                Target = nullptr;
            }
        }
        
        if (!canSeeTarget)
        {
            for (int i = 0; i < SeenActors.Num(); i++)
            {
                if (Cast<AMaintenanceCharacter>(SeenActors[i]) != nullptr)
                {
                    GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), SeenActors[i]);
                    Target = Cast<AMaintenanceCharacter>(SeenActors[i]);
                    GetBlackboardComponent()->SetValueAsVector(TEXT("LastSeenLocation"),Target->GetActorLocation());
                    break;
                }
            }
        }

        
        if(HeardActors.Num() > 0)
        {
            //we just react to most recent noise
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastNoiseLocation"),HeardActors[0]->GetActorLocation());
        }
    }
}
