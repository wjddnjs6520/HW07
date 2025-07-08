// Fill out your copyright notice in the Description page of Project Settings.


#include "DronePawn.h"
#include "DroneController.h"
#include "Components/CapsuleComponent.h"
// 카메라, 스프링 암 함수가 필요한 경우라 include 추가
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// Sets default values
ADronePawn::ADronePawn()
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
	DownSpeed = 0.5f;
	FlySpeed = NormalSpeed * DownSpeed;
	CurrentSpeed = NormalSpeed;
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GroundCheck();
	// 현재 회전값 가져오기
	FRotator CurrentRotation = GetActorRotation();
	if (bIsFly) {
		// 이동 입력값에 비례해 목표 Pitch, Roll 계산
		float TargetPitch = -CurrentMoveValue.X * MaxPitchAngle; // 앞 이동 시 아래로
		float TargetRoll = CurrentMoveValue.Y * MaxRollAngle;  // 오른 이동 시 오른쪽으로

		// 목표 회전값 (Yaw는 현재 값 유지)
		FRotator FlyTargetRotation(TargetPitch, CurrentRotation.Yaw, TargetRoll);
		// 부드럽게 회전값 보간
		FRotator FlyNewRotation = FMath::RInterpTo(CurrentRotation, FlyTargetRotation, DeltaTime, RotationInterpSpeed);
		// 회전 적용
		SetActorRotation(FlyNewRotation);
	}
	else {
		// 목표 회전값 (Yaw는 현재 값 유지)
		FRotator TargetRotation(0.f, CurrentRotation.Yaw, 0.f);
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
		// 회전 적용
		SetActorRotation(NewRotation);
	}

}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInput = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		if (ADroneController* PlayerController = Cast<ADroneController>(GetController())) {
			//W S A D 누르는 동안 Move() 호출
			if (PlayerController->MoveAction) {
				EnhanceInput->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered, this, &ADronePawn::Move);
			}

			//마우스 움직이는 동안 Look() 호출
			if (PlayerController->LookAction) {
				EnhanceInput->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered, this, &ADronePawn::Look);
			}

			//스페이스 입력 동안 올라가기
			if (PlayerController->UpAction) {
				EnhanceInput->BindAction(PlayerController->UpAction, ETriggerEvent::Triggered, this, &ADronePawn::UPDrone);
			}

			//쉬프트 입력동안 내려가기
			if (PlayerController->DownAction) {
				EnhanceInput->BindAction(PlayerController->DownAction, ETriggerEvent::Triggered, this, &ADronePawn::DownDrone);
			}
		}
	}

}

void ADronePawn::Move(const FInputActionValue& value)
{
	CurrentMoveValue = value.Get<FVector2D>();
	FVector MoveDir = FVector::ZeroVector;

	if (!FMath::IsNearlyZero(CurrentMoveValue.X)) {
		MoveDir += FVector(CurrentMoveValue.X, 0.f, 0.f) * CurrentSpeed;
	}
	if (!FMath::IsNearlyZero(CurrentMoveValue.Y)) {
		MoveDir += FVector(0.f, CurrentMoveValue.Y, 0.f) * CurrentSpeed;
	}

	AddActorLocalOffset(MoveDir, true);

	FVector WorldForward = GetActorForwardVector();
	UE_LOG(LogTemp, Warning, TEXT("월드 ForwardVector: X=%.3f, Y=%.3f, Z=%.3f"), WorldForward.X, WorldForward.Y, WorldForward.Z);
	FVector LocalForward = FVector(1.f, 0.f, 0.f);
	UE_LOG(LogTemp, Warning, TEXT("로컬 ForwardVector: X=%.3f, Y=%.3f, Z=%.3f"), LocalForward.X, LocalForward.Y, LocalForward.Z);

}

void ADronePawn::Look(const FInputActionValue& value)
{
	FVector2D LookValue = value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("마우스 입력값: X=%.3f, Y=%.3f"), LookValue.X, LookValue.Y);


	if (!FMath::IsNearlyZero(LookValue.Y)) {
		FRotator ArmRotation = SpringArmComp->GetRelativeRotation();
		ArmRotation.Pitch = FMath::Clamp(ArmRotation.Pitch + (LookValue.Y), -80.f, 80.f);
		SpringArmComp->SetRelativeRotation(ArmRotation);
	}
	if (!FMath::IsNearlyZero(LookValue.X)) {
		FRotator LookDir = FRotator(0.f, LookValue.X, 0.f);
		AddActorLocalRotation(LookDir);
		FRotator ArmRotation = SpringArmComp->GetRelativeRotation();
		ArmRotation.Yaw += LookValue.X;
		SpringArmComp->SetRelativeRotation(ArmRotation);
	}

}

void ADronePawn::UPDrone(const FInputActionValue& value)
{
	bool bUp = value.Get<bool>();

	FVector UpDir = FVector::ZeroVector;;
	if (bUp) {
		UpDir += FVector(0.f, 0.f, 1.f) * CurrentSpeed;
		AddActorLocalOffset(UpDir, true);
	}
	
}

void ADronePawn::DownDrone(const FInputActionValue& value)
{
	bool bDown = value.Get<bool>();
	FVector DownDir = FVector::ZeroVector;;
	if (bDown && bIsFly) {
		DownDir += FVector(0.f, 0.f, -1.f) * CurrentSpeed;
		AddActorLocalOffset(DownDir, true);
	}

}

void ADronePawn::GroundCheck()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f);

	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this); // 자기 자신은 무시

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,       // 결과
		Start,           // 시작 지점
		End,             // 끝 지점
		ECC_Visibility,  // 트레이스 채널
		TraceParams      // 옵션
	);

	// 디버그 라인 그리기
	DrawDebugLine(
		GetWorld(),
		Start,
		End,
		FColor::Green,
		false,
		1.0f,
		0,
		2.0f
	);

	if (bHit)
	{
		bIsFly = false;
		CurrentSpeed = NormalSpeed;
	}
	else {
		bIsFly = true;
		CurrentSpeed = FlySpeed;
	}

}
