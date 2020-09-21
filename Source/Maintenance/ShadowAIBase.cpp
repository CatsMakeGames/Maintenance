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

void AShadowAIBase::BeginPlay()
{
    Super::BeginPlay();
    
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
                if(IAIInterface::Execute_CanBeSeenByThisShadow(Target,this,GetSightRadius()))
                {
                    canSeeTarget = true;
                }         
            }
            else
            {
                //no longer can see target
                GetBlackboardComponent()->ClearValue(TEXT("Target"));
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastSeenLocation"),Target->GetActorLocation());
                LastNoiseLocation = Target->GetActorLocation();
                if(GetPawn()!=nullptr)
                {
                    if (GetPawn()->Implements<UAIInterface>() || (Cast<IAIInterface>(GetPawn()) != nullptr))
                    {
                        IAIInterface::Execute_OnLostTarget(GetPawn());
                    }
                }
                Target = nullptr;
            }
        }
        
        if (!canSeeTarget)
        {
            for (int i = 0; i < SeenActors.Num(); i++)
            {
                if (Cast<AMaintenanceCharacter>(SeenActors[i]) != nullptr)
                {
                    if (SeenActors[i]->Implements<UAIInterface>() || (Cast<IAIInterface>(SeenActors[i]) != nullptr))
                    {
                        if(IAIInterface::Execute_CanBeSeen(SeenActors[i]))
                        {
                            GetBlackboardComponent()->SetValueAsObject(TEXT("Target"), SeenActors[i]);
                            Target = Cast<AMaintenanceCharacter>(SeenActors[i]);
                            GetBlackboardComponent()->SetValueAsVector(TEXT("LastSeenLocation"),Target->GetActorLocation());
                            LastNoiseLocation = Target->GetActorLocation();
                            if(GetPawn()!=nullptr)
                            {
                                if (GetPawn()->Implements<UAIInterface>() || (Cast<IAIInterface>(GetPawn()) != nullptr))
                                {
                                    IAIInterface::Execute_OnSawTarget(GetPawn());
                                }
                            }
                            break;
                        }
                    }                              
                }
            }
        }

        
        if(HeardActors.Num() > 0)
        {
            //we just react to most recent noise
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastNoiseLocation"),HeardActors[0]->GetActorLocation());
            LastSeenLocation = HeardActors[0]->GetActorLocation();
        }
    }
}

AActor* AShadowAIBase::AssignNewPatrolPoint()
{
    if (PatrolPoints.Num() > 0 && GetBlackboardComponent() != nullptr)
    {
        int32 id = FMath::RandRange(0, PatrolPoints.Num() - 1);
        GetBlackboardComponent()->SetValueAsObject(TEXT("PatrolLocation"), PatrolPoints[id]);

        return PatrolPoints[id];
    }
    return nullptr;
}

AActor* AShadowAIBase::GetCurrentPatrolPoint()
{
    if (GetBlackboardComponent() != nullptr)
    {
        if (GetBlackboardComponent()->GetValueAsObject(TEXT("PatrolLocation")) != nullptr)
        {
            return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TEXT("PatrolLocation")));
        }
    }
    return nullptr;
}

EAIState AShadowAIBase::GetCurrentAIState_Implementation()
{
    if(Target != nullptr)
    {
        return EAIState::EAS_Chase;
    }
    return EAIState::EAS_Normal;
}
