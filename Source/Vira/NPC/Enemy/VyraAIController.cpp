// Fill out your copyright notice in the Description page of Project Settings.


#include "VyraAIController.h"

#include "BrainComponent.h"
#include "IAIHelpers.h"
#include "VyraEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/GSCCoreComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Vira/Character/VyraPlayerStateCharacter.h"


// Sets default values
AVyraAIController::AVyraAIController() : SeekAfterSightLost_Timer(4.f),
	AttackTargetKeyName(FName("AttackTarget")), StatesKeyName(FName("States")),
	PointOfInterestKeyName(FName("PointOfInterest")), AttackRadiusKeyName(FName("AttackRadius")),
	DefendRadiusKeyName(FName("DefendRadius"))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AVyraAIController::AIPerception_OnPerceptionUpdated);

}

void AVyraAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(AVyraEnemyCharacter* AICharacterBase = CastChecked<AVyraEnemyCharacter>(InPawn))
	{
		AICharacterBase->GetGSCCoreComponent()->OnDeath.AddDynamic(this, &AVyraAIController::Pawn_OnDeath);
		
		if(AICharacterBase->BehaviorTree)
		{
			RunBehaviorTree(AICharacterBase->BehaviorTree);
		}

		SetStateAs_Passive();
		
		float AttackRadius;
		float DefendRadius;
		AICharacterBase->Execute_GetAIIdealRange(AICharacterBase, AttackRadius, DefendRadius);

		if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsFloat(AttackRadiusKeyName, AttackRadius);
			BlackboardComponent->SetValueAsFloat(DefendRadiusKeyName, DefendRadius);
		}
		
		GetWorld()->GetTimerManager().SetTimer(
			CheckForgottenActors_TimerHandle,
			this,
			&AVyraAIController::CheckIfForgottenSeenActor,
			1.f,
			true);
		
	}
}

void AVyraAIController::OnUnPossess()
{
	GetWorld()->GetTimerManager().ClearTimer(CheckForgottenActors_TimerHandle);
	
	Super::OnUnPossess();
	Destroy();
}

void AVyraAIController::CheckIfForgottenSeenActor()
{
	TArray<AActor*> OutActors;
	AIPerceptionComponent->GetKnownPerceivedActors(UAISense_Sight::StaticClass(),OutActors);
	if(KnownSeenActors.Num() != OutActors.Num())
	{
		for (auto Actor : KnownSeenActors)
		{
			if(!OutActors.Contains(Actor))
			{
				KnownSeenActors.Remove(Actor);
				if(Actor == AttackTarget)
				{
					SetStateAs_Passive();
				}
			}
		}
	}
}

void AVyraAIController::SeekAttackTarget()
{
	SetStateAs_Seeking(AttackTarget->GetActorLocation());
	GetWorld()->GetTimerManager().ClearTimer(SeekAttackTarget_TimerHandle);
}

bool AVyraAIController::CanSenseActor(AActor* Actor, EAISense AISense, FAIStimulus& Stimulus)
{
	if(CastChecked<AVyraPlayerStateCharacter>(Actor)->IsDead())
		return false;

	TSubclassOf<UAISense> ClassToCheck;
	switch (AISense) {
	case None:
		break;
	case Sight: ClassToCheck = UAISense_Sight::StaticClass();
		break;
	case Hearing: ClassToCheck = UAISense_Hearing::StaticClass();
		break;
	case Damage: ClassToCheck = UAISense_Damage::StaticClass();
		break;
	default: ;
	}
	
	FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
	if(AIPerceptionComponent->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo))
	{
		for (auto Element : ActorPerceptionBlueprintInfo.LastSensedStimuli)
		{
			TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, Element);

			if(ClassToCheck == SenseClass)
			{
				Stimulus = Element;
				return true;
			}
		}
	}

	return false;
}

void AVyraAIController::Handle_SensedSight(AActor* SensedActor)
{
	KnownSeenActors.AddUnique(SensedActor);
	
	switch (GetCurrentState())
	{
	case Passive:
		{
			SetStateAs_Attacking(SensedActor, false);
			break;
		}
	case Attacking:
		{
			if(SensedActor == AttackTarget)
			{
				GetWorld()->GetTimerManager().ClearTimer(SeekAttackTarget_TimerHandle);
			}
			break;
		}
	case Frozen:
		break;
	case Investigating:
		{
			SetStateAs_Attacking(SensedActor, false);
			break;
		}
	case Dead:
		break;
	case Seeking:
		{
			SetStateAs_Attacking(SensedActor, false);
			break;
		}
	default: ;
	}
}

void AVyraAIController::Handle_LostSight(const AActor* Actor)
{
	if(Actor == AttackTarget)
	{
		switch (GetCurrentState()) {
		case Passive: 
			break;
		case Attacking:
			{
				GetWorld()->GetTimerManager().ClearTimer(SeekAttackTarget_TimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					SeekAttackTarget_TimerHandle,
					this,
					&AVyraAIController::SeekAttackTarget,
					SeekAfterSightLost_Timer,
					false);
				
				break;
			}
		case Frozen:
			{
				GetWorld()->GetTimerManager().ClearTimer(SeekAttackTarget_TimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					SeekAttackTarget_TimerHandle,
					this,
					&AVyraAIController::SeekAttackTarget,
					SeekAfterSightLost_Timer,
					false);
				break;
			}
		case Investigating:
			{
				GetWorld()->GetTimerManager().ClearTimer(SeekAttackTarget_TimerHandle);
				GetWorld()->GetTimerManager().SetTimer(
					SeekAttackTarget_TimerHandle,
					this,
					&AVyraAIController::SeekAttackTarget,
					SeekAfterSightLost_Timer,
					false);
				break;
			}
		case Dead:
			break;
		case Seeking:
			break;
		default: ;
		}
	}
}

void AVyraAIController::Handle_SensedHearing(const FVector& Location)
{
	switch (GetCurrentState())
	{
	case Passive: SetStateAs_Investigating(Location);
		break;
	case Attacking:
		break;
	case Frozen:
		break;
	case Investigating: SetStateAs_Investigating(Location);
		break;
	case Dead:
		break;
	case Seeking: SetStateAs_Investigating(Location);
		break;
	default: ;
	}

}

void AVyraAIController::Handle_SensedDamage(AActor* SensedActor)
{
	switch (GetCurrentState())
	{
	case Passive: SetStateAs_Attacking(SensedActor, false);
		break;
	case Attacking:
		break;
	case Frozen: AttackTarget = SensedActor;
		break;
	case Investigating: SetStateAs_Attacking(SensedActor, false);
		break;
	case Dead:
		break;
	case Seeking: SetStateAs_Attacking(SensedActor, false);
		break;
	default: ;
	}
}

void AVyraAIController::Handle_ForgotActor(AActor* ForgottenActor)
{
	KnownSeenActors.Remove(ForgottenActor);

	if(ForgottenActor == AttackTarget)
	{
		SetStateAs_Passive();
	}
}

void AVyraAIController::Pawn_OnDeath()
{
	BrainComponent->StopLogic(FString("Death"));
	SetStateAs_Dead();
}

void AVyraAIController::AIPerception_OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Element : UpdatedActors)
	{
		if(Element->IsA(AVyraPlayerStateCharacter::StaticClass()))
		{
			FAIStimulus Stimulus;
			//Sight Sense
			if(CanSenseActor(Element, EAISense::Sight, Stimulus))
			{
				Handle_SensedSight(Element);
			}
			else
			{
				Handle_LostSight(Element);
			}

			//Hearing Sense
			if(CanSenseActor(Element, EAISense::Hearing, Stimulus))
			{
				Handle_SensedHearing(Stimulus.StimulusLocation);
			}
			else
			{
				
			}

			//Damage Sense
			if(CanSenseActor(Element, EAISense::Damage, Stimulus))
			{
				Handle_SensedDamage(Element);
			}
			else
			{
				
			}
		}
	}
}

void AVyraAIController::SetStateAs_Passive()
{
	if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Passive);
	}
}

void AVyraAIController::SetStateAs_Attacking(AActor* SensedActor, bool bUseLastKnownAttackTarget)
{
	AActor* Local_AttackTarget;
	if(bUseLastKnownAttackTarget && AttackTarget)
	{
		Local_AttackTarget = AttackTarget;
	}
	else
	{
		Local_AttackTarget = SensedActor;
	}

	if(!Local_AttackTarget)
	{
		SetStateAs_Passive();
		return;
	}

	if(const AVyraPlayerStateCharacter* MyPlayerStateCharacter = CastChecked<AVyraPlayerStateCharacter>(Local_AttackTarget))
	{
		if(MyPlayerStateCharacter->IsDead())
		{
			SetStateAs_Passive();
		}

		if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
		{
			BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Attacking);
			BlackboardComponent->SetValueAsObject(AttackTargetKeyName, Local_AttackTarget);
			AttackTarget = Local_AttackTarget;
		}
	}
}

void AVyraAIController::SetStateAs_Seeking(const FVector& Location)
{
	if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Seeking);
		BlackboardComponent->SetValueAsVector(PointOfInterestKeyName, Location);
	}
}

void AVyraAIController::SetStateAs_Investigating(const FVector& Location)
{
	if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Investigating);
		BlackboardComponent->SetValueAsVector(PointOfInterestKeyName, Location);
	}
}

void AVyraAIController::SetStateAs_Frozen()
{
	if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Frozen);
	}
}

void AVyraAIController::SetStateAs_Dead()
{
	if(UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		BlackboardComponent->SetValueAsEnum(StatesKeyName, EAIStates::Dead);
	}

	ClearFocus(EAIFocusPriority::Gameplay);
	BrainComponent->StopLogic(FString("Dead"));
}

EAIStates AVyraAIController::GetCurrentState()
{
	if(const UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		return static_cast<EAIStates>(BlackboardComponent->GetValueAsEnum(StatesKeyName));
	}
	return EAIStates::Passive;
}

