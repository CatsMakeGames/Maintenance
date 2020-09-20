// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AI/AIInterface.h"
#include "Door/KeyComponent.h"
#include "GameFramework/Character.h"
#include "Tools/ToolBase.h"

#include "MaintenanceCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AMaintenanceCharacter : public ACharacter,public IAIInterface,public IInteractionInterface
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorHoldingPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorDroppingPosition;

public:
	AMaintenanceCharacter();

protected:
	virtual void BeginPlay();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Timers,SaveGame)
	FTimerHandle ThrowTimerHandle;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Interaction,SaveGame)
	float InteractionLength = 500.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Interaction,SaveGame)
	AActor* CurrentlyHeldActor = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=SafeZone,SaveGame)
	bool bIsInSafeZone = false;

	//if false object will just be dropped with no velocity
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Throwing,SaveGame)
	bool bIsThrowing = false;

	//how long do you need to hold drop button for state to change o throw
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Throwing,SaveGame)
	float ThrowTime = 1.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Throwing,SaveGame)
	float ThrowStrength = 500.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	TArray<AToolBase*> Tools;

	//Id of selected tool. -1 for hand
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	int32 CurrentlySelectedToolId = -1;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	TArray<FKeyInfo> Keys;
	
protected:

	void ChangeToThrow();
	
	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	virtual bool CanBeSeen_Implementation() override;

	virtual void RemoveKey_Implementation(const FString& keyName) override;

	//Selects specific tool. Return false if tool was NOT found
	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual bool SelectTool(FString name);

	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual bool AddTool(AToolBase*tool);

	//Selects hand(physical objects that player can pickup)
	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual void SelectHand();
	
	UFUNCTION(BlueprintPure,Category= Tools)
	virtual bool HasTool(FString name);
	
	UFUNCTION(BlueprintCallable)
	void PlayerInteract();

	UFUNCTION(BlueprintCallable)
    void PlayerUseItem();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void GetCaught();

	void GetCaught_Implementation();

	UFUNCTION(BlueprintCallable)
    void Throw();

	//Used by save system. Attaches this actor to holding position
	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void ForcePickupActor(AActor*actor);

	UFUNCTION(BlueprintCallable)
	void BeginThrow();
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:

	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

typedef AMaintenanceCharacter APlayer;
