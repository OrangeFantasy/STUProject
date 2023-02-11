// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/STUGameHUD.h"

#include <Engine/Canvas.h>
#include "Blueprint/UserWidget.h"

void ASTUGameHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->AddToViewport();
	}
}

