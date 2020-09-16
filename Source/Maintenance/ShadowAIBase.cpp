// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowAIBase.h"


#include "AI/AIInterface.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Hearing.h"

AShadowAIBase::AShadowAIBase()
{
    Senses = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Senses"));
}

void AShadowAIBase::BeginPlay()
{
    if(BehaviorTree != nullptr)
    {
        RunBehaviorTree(BehaviorTree);
    }
    
    GetWorldTimerManager().SetTimer(UpdateTimerHandle,this,&AShadowAIBase::UpdatePerceivedActors,1.f,true);

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
}

void AShadowAIBase::UpdatePerceivedActors()
{
    TArray<AActor*>actors;
    Senses->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(),actors);
    if (GetBlackboardComponent() != nullptr)
    {
        bool canSeeTarget = false;
        if(Target != nullptr)
        {
            if(actors.Find(Target) != INDEX_NONE)
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
            for (int i = 0; i < actors.Num(); i++)
            {
                if (Cast<AMaintenanceCharacter>(actors[i]) != nullptr)
                {
                    GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), actors[i]);
                    Target = Cast<AMaintenanceCharacter>(actors[i]);
                    GetBlackboardComponent()->SetValueAsVector(TEXT("LastSeenLocation"),Target->GetActorLocation());
                    break;
                }
            }
        }

        //update hearing
        actors.Empty();
        Senses->GetCurrentlyPerceivedActors(UAISense_Hearing::StaticClass(),actors);
        if(actors.Num() > 0)
        {
            //we just react to most recent noise
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastNoiseLocation"),actors[0]->GetActorLocation());
        }
    }
}
