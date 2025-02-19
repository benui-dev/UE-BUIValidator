#pragma once

#include "EditorValidatorBase.h"
#include "BUIEditorValidator_Textures.generated.h"

struct FAssetData;

UCLASS(meta = ( DisplayName = "BUI Texture Validator" ))
class BUIVALIDATOR_API UBUIEditorValidator_Textures : public UEditorValidatorBase
{
	GENERATED_BODY()

public:
	UBUIEditorValidator_Textures();

protected:
	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context) override;
};
