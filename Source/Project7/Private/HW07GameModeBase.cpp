// Fill out your copyright notice in the Description page of Project Settings.


#include "HW07GameModeBase.h"
#include "HW07Pawn.h"
#include "HW07PlayerController.h"

AHW07GameModeBase::AHW07GameModeBase()
{
	DefaultPawnClass = AHW07Pawn::StaticClass();
	PlayerControllerClass = AHW07PlayerController::StaticClass();
}