// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "RWBY_CodenameColorsCharacter.generated.h"

UENUM() 
	namespace ECameraType{
		enum Type {
			None,
			Side,
			Third,
		};
}

UENUM()
	namespace ETask {
		enum Type {
			None, 
			Shooting,
			Reload,
	};
}

UENUM()
namespace EDustType {
	enum Type {
		None,
		Ice,
		Fire,
		Gravity,
		Electic,
		Water,
	};
}

UENUM()
namespace ECharacterState {
	enum Type {
		Normal		 UMETA(DisplayName = "Normal"),
		Freezing	 UMETA(DisplayName = "Freezing"),
		Frozen		 UMETA(DisplayName = "Frozen"),
		OnFire		 UMETA(DisplayName = "OnFire"),
		Wet			 UMETA(DisplayName = "Wet"),
		Shocked		 UMETA(DisplayName = "Shocked"),
		GravityLow	 UMETA(DisplayName = "GravityLow"),
	};
}

UENUM()
namespace EPoweredUpState {
	enum Type {
		None,
		FiredUp,
		IcedUp,
		GravityUp,
		ShockedUp,
		WateredUp,
	};
}


UCLASS(config=Game)
class ARWBY_CodenameColorsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* CharMesh;


	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ThirdPersonBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* ThirdPersonCameraBoom;



protected:
	
	void StartJump();
	void StopJump();

	void MoveCharacter( );

	void UseDust();

	void ResetDust();
	
	void OnCrouchEnd();

	void SetMaxAmmo(float NewMaxAmmo);

	//method for replication of variables
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	//method used to switch the camera, and switch movement modes
	void SwitchCamera();

	//method used to fire from the center of the camera (screen)
	void OnFire();

	virtual void OnDodge();

	void OnHeal();

	void StartHealing();

	void StopHealing();

	//method used start shooting
	void StartShooting();

	//method used to stop shooting
	void StopShooting();

	//method used to start dodging
	virtual void StartDodging();

	//method used to stop doding
	virtual void StopDodging();

	void PerformHealing(bool Healing);

	virtual void PerformDodge(bool bDodge);

	//method used to preform tasks (Client)
	void PerformTask(ETask::Type NewTask);

	void PerformUseDust( );

	void PerformLedgeTrace(bool CanTrace);

	void PerformWallSlide(bool CanSlide);

	void PerformWallJump(bool CanJump);

	void PerformElementalDamage(float DeltaSeconds);

	void OnWallJump();

	void OnWallSlide();

	void OnLedgeTrace();

	void LedgeGrab();

	//returns true while in the middle of a dodge
	UPROPERTY(BlueprintReadWrite, Replicated)
	bool WhileDodging;

	void OnElementalDamage(float DeltaSeconds);
	void OnElementalDamage(ECharacterState::Type CurrentState, float DeltaSeconds, int FrozenPercent);

	UPROPERTY(EditAnywhere)
	TArray<float> DownwardVelocity;

	UPROPERTY(EditAnywhere)
	int16 FrozenPercent;

	//Enum used to determine the camera/ movement state of the characters
	ECameraType::Type Perspective;

	//Boolean used to determine the dodging
	UPROPERTY(ReplicatedUsing = OnRep_Dodge, EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "RWBYCharacter" )
	bool isDodging;

	/** Called for forwards/backward input */
	void MoveForward(float Value);

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


	/** Called for side to side input */
	void MoveRight(float Val);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Health)
		float Health;

	bool bFalling;


	UPROPERTY(ReplicatedUsing = OnRep_Task)
		TEnumAsByte<ETask::Type> Task;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Dust)
		TEnumAsByte<EDustType::Type> Dust;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		TEnumAsByte<ECharacterState::Type> CharacterState;


	FTimerHandle TimerHandler_Task;

	FTimerHandle DodgeDelay;

	FTimerHandle PoweredUp;

	FTimerHandle TimerHandler_Healing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		float MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing = OnRep_Ammo)
		float CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool Shooting;

	// the state of character true if powered up
	UPROPERTY(BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing=OnRep_Dust)
		bool bIsPoweredUp;

	// the state of character true if powered up
	UPROPERTY(BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bCanPickupDust;

	// the state of character true if powered up
	UPROPERTY(BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing=OnRep_Health)
		bool bCanHeal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing=OnRep_Ledge)
		bool bCanWallTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bCanClimb;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bHanging;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bClimbing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bDoneClimbing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing = OnRep_Trip)
		bool bLedgeTrip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing = OnRep_Trip)
		bool bFallDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bCanWallSlide;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing = OnRep_Slide)
		bool bSliding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bWallJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter")//, Replicated)
	int32 Semblance;

public:
	ARWBY_CodenameColorsCharacter();

	AActor* LastHitActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TEnumAsByte<EPoweredUpState::Type> PoweredUpState;

	TArray<TEnumAsByte<ECharacterState::Type>> GetStatusEffects();//const { return CharacterStatusEffects; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	TArray<TEnumAsByte<ECharacterState::Type>> CharacterStatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		FVector ClimbPosition;

	FTimerHandle FireLength;

	FTimerHandle ShockLength;

	FTimerHandle IceLength;

	FTimerHandle GravityLength;

	FTimerHandle WaterLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", ReplicatedUsing = OnRep_MeleeAttack)
		bool bMeleeAttacking;

	void SetFrozenPercent(int16 NewAmount);

	int16 GetFrozenPercent();

	void RemoveStateWithDelay();

	void DetermineElementalDamage(ARWBY_CodenameColorsCharacter* CurrentPlayer, float DamageAmount);

	//void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	virtual void Tick(float DeltaSeconds) override;

	//Collect the pickups
	void Collect();

	// returns the power up state for this character
	bool IsPoweredUp();


	//method from the base class AActor, used to take damage. includes damage amount, infomation about the event, and the player of who did this
	float TakeDamage(float DamageAmount, const FDamageEvent & DamageEvent, AController* EventInstigator, AActor * DamageCauser) override;

	float GetShot(float DamageAmount, const FDamageEvent & DamageEvent, AController* EventInstigator, AActor * DamageCauser);
	
	//Deal damage
	void DealDamage(float Damage, FHitResult LineTrace);


	/*Replication methods (on_Reps)
	*
	*
	*/

	UFUNCTION()
		void OnRep_Task();
	
	UFUNCTION()
		void OnRep_Health();
	
	UFUNCTION()
		void OnRep_Dodge();
	
	UFUNCTION()
		void OnRep_Ammo();

	UFUNCTION()
		void OnRep_Dust();
	
	UFUNCTION()
		void OnRep_Ledge();

	UFUNCTION()
		void OnRep_Trip();

	UFUNCTION()
		void OnRep_Slide();

	UFUNCTION()
		virtual void OnRep_MeleeAttack();

	/*Other methods
	*
	*
	*/

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;


	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return CharMesh; }

	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetThirdPersonCameraBoom() const { return ThirdPersonCameraBoom; }


	/*
	* THIS SECTION IS FOR REPLICATION METHODS
	*
	*/
protected:

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformTask(ETask::Type NewTask);
		void ServerPerformTask_Implementation(ETask::Type NewTask);
		bool ServerPerformTask_Validate(ETask::Type NewTask);

	UFUNCTION(Server, Reliable, WithValidation)
		 void ServerPerformDodge(bool bDodging);
		 void ServerPerformDodge_Implementation(bool bDodging);
		 bool ServerPerformDodge_Validate(bool bDodging);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformUseDust( );
		void ServerPerformUseDust_Implementation( );
		bool ServerPerformUseDust_Validate( );

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformHealing(bool Healing);
		void ServerPerformHealing_Implementation(bool Healing);
		bool ServerPerformHealing_validate(bool Healing);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformLedgeTrace(bool CanTrace);
		void ServerPerformLedgeTrace_Implementation(bool CanTrace);
		bool ServerPerformLedgeTrace_Validate(bool CanTrace);

	UFUNCTION(Server, Reliable, WithValidation)
		void OnCrouchStart();
		void OnCrouchStart_Implementation();
		bool OnCrouchStart_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
		void SetClimbing(bool NewState);
		void SetClimbing_Implementation(bool NewState);
		bool SetClimbing_Validate(bool NewState);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformWallSlide(bool CanSlide);
		void ServerPerformWallSlide_Implementation(bool CanSlide);
		bool ServerPerformWallSlide_Validate(bool CanSlide);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformWallJump(bool CanJump);
		void ServerPerformWallJump_Implementation(bool CanJump);
		bool ServerPerformWallJump_Validate(bool CanJump);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerAddVelocity();
		void ServerAddVelocity_Implementation();
		bool ServerAddVelocity_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformElementalDamage(float DeltaSeconds);
		void ServerPerformElementalDamage_Implementation(float DeltaSeconds);
		bool ServerPerformElementalDamage_Validate(float DeltaSeconds);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerRemoveCharacterState(ECharacterState::Type Target);
		void ServerRemoveCharacterState_Implementation(ECharacterState::Type Target);
		bool ServerRemoveCharacterState_Validate(ECharacterState::Type Target);
};
