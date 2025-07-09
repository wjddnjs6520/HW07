// Fill out your copyright notice in the Description page of Project Settings.


#include "HW07Pawn.h"
#include "HW07PlayerController.h"
#include "Components/CapsuleComponent.h"
// 카메라, 스프링 암 함수가 필요한 경우라 include 추가
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
AHW07Pawn::AHW07Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//캡슐 컴포넌트 생성
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);

	//스켈레탈 메쉬 컴포넌트 생성
	SkeletalComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	SkeletalComp->SetupAttachment(CapsuleComp);

	//스프링 암 생성
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	// 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	SpringArmComp->TargetArmLength = 300.0f;
	// 컨트롤러 회전(화면 회전)에 따라 스프링 암도 회전하도록 설정
	SpringArmComp->bUsePawnControlRotation = true;

	//카메라 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 스프링 암의 소켓 위치에 카메라를 부착
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// 스프링암을 따라 회전하도록 회전 설정 끄기
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 5.f;
}

// Called when the game starts or when spawned
void AHW07Pawn::BeginPlay()
{
	Super::BeginPlay();

}



// Called every frame
void AHW07Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHW07Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (AHW07PlayerController* PlayerController = Cast<AHW07PlayerController>(GetController())) {
			//W S A D 누르는 동안 Move() 호출
			if (PlayerController->MoveAction) {
				EnhanceInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &AHW07Pawn::Move);
			}


			//마우스 움직이는 동안 Look() 호출
			if (PlayerController->LookAction) {
				EnhanceInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &AHW07Pawn::Look);
			}
		}
	}
}


void AHW07Pawn::Move(const FInputActionValue& value)
{
	FVector2D MoveValue = value.Get<FVector2D>();
	FVector MoveDir = FVector::ZeroVector;

	if(!FMath::IsNearlyZero(MoveValue.X)){
		MoveDir += FVector(MoveValue.X, 0.f, 0.f) * NormalSpeed;
		
	}
	if (!FMath::IsNearlyZero(MoveValue.Y)) {
		MoveDir += FVector(0.f, MoveValue.Y, 0.f) * NormalSpeed;
	}
	AddActorLocalOffset(MoveDir, true);


	FVector WorldForward = GetActorForwardVector();
	UE_LOG(LogTemp, Warning, TEXT("월드 ForwardVector: X=%.3f, Y=%.3f, Z=%.3f"), WorldForward.X, WorldForward.Y, WorldForward.Z);
	FVector LocalForward = FVector(1.f, 0.f, 0.f);
	UE_LOG(LogTemp, Warning, TEXT("로컬 ForwardVector: X=%.3f, Y=%.3f, Z=%.3f"), LocalForward.X, LocalForward.Y, LocalForward.Z);
}

void AHW07Pawn::Look(const FInputActionValue& value)
{
	FVector2D LookValue = value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("마우스 입력값: X=%.3f, Y=%.3f"), LookValue.X, LookValue.Y);

	FRotator LookDir = FRotator::ZeroRotator;

	if (!FMath::IsNearlyZero(LookValue.Y)) {
		FRotator ArmRotation = SpringArmComp->GetRelativeRotation();
		ArmRotation.Pitch = FMath::Clamp(ArmRotation.Pitch + (LookValue.Y), -80.f, 80.f);
		SpringArmComp->SetRelativeRotation(ArmRotation);

	}
	if (!FMath::IsNearlyZero(LookValue.X)) {
		LookDir += FRotator(0.f, LookValue.X, 0.f);
		AddActorLocalRotation(LookDir, true);
		FRotator ArmRotation = SpringArmComp->GetRelativeRotation();
		ArmRotation.Yaw += LookValue.X;
		SpringArmComp->SetRelativeRotation(ArmRotation);
	}
}


