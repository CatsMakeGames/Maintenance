// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "AI/AIInterface.h"
#include "Door/KeyComponent.h"
#include "GameFramework/Character.h"
#include "Notes/StoryNote.h"
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

	//used only for running
	virtual void Tick(float DeltaSeconds) override;

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

	//this is for the ending. Player's tools will be removed and only gun will be available.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	bool bHasGun = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	TArray<FKeyInfo> Keys;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Tools,SaveGame)
	USoundBase*KeyPickupSound = nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Notes,SaveGame)
    TArray<FStoryNote> Notes;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Running,SaveGame)
	float MaxRunTime = 5.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Running,SaveGame)
	float CurrentRunTime = 0.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Running,SaveGame)
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Movement,SaveGame)
	float MaxWalkSpeed = 400.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category= Movement,SaveGame)
	float MaxRunSpeed = 800.f;

	bool bIsRunButtonPressed = false;
	
protected:

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Running)
	void StartRunning();

	void StartRunning_Implementation();

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Running)
	void ShootGun();

	void ShootGun_Implementation(){}

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category=Running)
    void StopRunning();

	void StopRunning_Implementation();
	
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

	virtual bool CanBeSeenByThisShadow_Implementation(AActor* Shadow, float SightRadius) override;

	//returns first tool of this class
	UFUNCTION(BlueprintCallable,Category= Tools)
	AToolBase* GetToolByClass(TSubclassOf<AToolBase> Class);
	
	//Remove specific tool. Return false if tool was NOT found
	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual bool RemoveTool(FString name);
	
	//Selects specific tool. Return false if tool was NOT found
	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual bool SelectTool(FString name);

	UFUNCTION(BlueprintCallable,Category= Tools)
	virtual bool AddTool(AToolBase*tool);

	//Used when save is loaded
	UFUNCTION(BlueprintCallable,Category= Tools)
	void ReattachTools();

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

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
    void PickUpKey(FKeyInfo key);

	void PickUpKey_Implementation(FKeyInfo key);

	UFUNCTION(BlueprintCallable)
    void Throw();

	//Used by save system. Attaches this actor to holding position
	UFUNCTION(BlueprintCallable,Category=SaveSystem)
	virtual void ForcePickupActor(AActor*actor);

	UFUNCTION(BlueprintCallable)
	void BeginThrow();

	//returns false if player has same note
	UFUNCTION(BlueprintCallable)
	virtual bool PickupNote(FStoryNote note);
	
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
