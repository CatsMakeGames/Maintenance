// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MaintenanceCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AMaintenanceCharacter : public ACharacter
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

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Interaction)
	float InteractionLenght = 500.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Interaction)
	AActor* CurrentlyHeldActor = nullptr;

protected:

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

	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
    void UseItem();

	UFUNCTION(BlueprintCallable)
    void Throw();
	
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
