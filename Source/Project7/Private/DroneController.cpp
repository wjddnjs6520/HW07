﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"
#include "EnhancedInputSubsystems.h"

ADroneController::ADroneController() 
	: InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	UpAction(nullptr),
    DownAction(nullptr)
{
}

void ADroneController::BeginPlay()
{
    Super::BeginPlay();

    // 현재 PlayerController에 연결된 Local Player 객체를 가져옴    
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        // Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (InputMappingContext)
            {
                // Subsystem을 통해 우리가 할당한 IMC를 활성화
                // 우선순위(Priority)는 0이 가장 높은 우선순위
                Subsystem->AddMappingContext(InputMappingContext, 0);
            }
        }
    }
}
