// Authour: Antonio Morales.

#pragma once

#include "RWBY_CodenameColorsCharacter.h"
#include "RubyRose.generated.h"

/**
 * 
 */
UCLASS()
class RWBY_CODENAMECOLORS_API ARubyRose : public ARWBY_CodenameColorsCharacter
{
	GENERATED_BODY()
	
public:

	ARubyRose();

	void Tick(float DeltaSeconds) override;

protected:

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

	void PerformAttack(bool ShouldAttack);

	void OnAttack();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;


	//Boolean used to determine the dodging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RWBYCharacter", Replicated)
		bool bCanDealAttackDamage;

	//Boolean used to determine the dodging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "RWBYCharacter", Replicated)
		bool bCanDamageSameActor;

private:

	void OnDodge() override;

	void StartDodging() override;

	void StopDodging() override;

	void PerformDodge(bool bDodge) override;

	UPROPERTY(EditAnywhere, Category = "Ruby Rose")
	TSubclassOf<class ARosePetal> RosePetal;
	 
	void Spawn(FVector SpawnLocation);

	FTimerHandle PetalSpawnDelay;

	void SetAttackingBool(bool NewBoolState);

	FTimerHandle Attack;

//***** ON REP FUNCTIONS*****\\

public:

	UFUNCTION()
	void OnRep_MeleeAttack() override;

public:

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformAttack(bool ShouldAttack);
		void ServerPerformAttack_Implementation(bool ShouldAttack);
		bool ServerPerformAttack_Validate(bool ShouldAttack);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerPerformDodge(bool bDodging) ;
		void ServerPerformDodge_Implementation(bool bDodging);
		bool ServerPerformDodge_Validate(bool bDodging);

	UFUNCTION(Server, Reliable, WithValidation)
		void StartAttack();
		void StartAttack_Implementation();
		bool StartAttack_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
		void StopAttack();
		void StopAttack_Implementation();
		bool StopAttack_Validate();


};
