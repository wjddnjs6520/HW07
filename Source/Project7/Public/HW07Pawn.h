// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "HW07Pawn.generated.h"

//컴포넌트
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;

// IA 입력 값 구조체
struct FInputActionValue;

UCLASS()
class PROJECT7_API AHW07Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AHW07Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 캡슐 컴포넌트(루트). 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleComp;

	// 캡슐 컴포넌트(루트). 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* SkeletalComp;

	// 스프링 암 컴포넌트. 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;

	// 카메라 컴포넌트. 블루프린트에서 읽기 가능(수정은 코드로). 카테고리 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
