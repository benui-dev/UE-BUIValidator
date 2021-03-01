// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EditorValidatorBase.h"
#include "BUIValidatorSettings.h"
#include "BUIEditorValidator_Textures.generated.h"

/*
* Validates that localized assets (within the L10N folder) conform to a corresponding source asset of the correct type.
* Localized assets that fail this validation will never be loaded as localized variants at runtime.
*/
UCLASS()
class BUIVALIDATOR_API UBUIEditorValidator_Textures : public UEditorValidatorBase
{
	GENERATED_BODY()

public:
	UBUIEditorValidator_Textures();

protected:
	virtual bool CanValidateAsset_Implementation(UObject* InAsset) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(UObject* InAsset, TArray<FText>& ValidationErrors) override;

	bool ShouldGroupValidateAsset( const FBUIValidatorGroup& Group, UObject* InAsset ) const;
	bool ShouldValidateAsset( UObject* InAsset ) const;
};
