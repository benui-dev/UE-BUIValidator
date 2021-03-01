#pragma once

#include "BUIValidatorSettings.generated.h"

UENUM()
enum class EBUITextureSizeRequirement
{
	MultipleOfFour,
	PowerOfTwo,
};

USTRUCT( meta = ( ToolTip = "All parts of a rule must pass in order for the rule to be applied" ) )
struct FBUIMatchConditions
{
	GENERATED_BODY()
public:
	// Match UTexture2D assets with any of these texture groups
	UPROPERTY( config, EditAnywhere, Category = Custom )
	TArray<TEnumAsByte<TextureGroup>> TextureGroups = { TextureGroup::TEXTUREGROUP_UI };

	// Match UTexture2D assets with any of these prefixes
	UPROPERTY( config, EditAnywhere, Category = Custom )
	TArray<FString> Prefixes = { "T_UI_" };

	// Match UTexture2D assets under any of these directories
	UPROPERTY( EditAnywhere, meta = ( ContentDir ) )
	TArray<FDirectoryPath> Paths;
};

USTRUCT()
struct FBUIValidationRule
{
	GENERATED_BODY()
	FBUIValidationRule()
	{
		Path.Path = "/Game/UI";
	}
public:
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUseTextureGroup = false;
	// Textures not with this Texture Group will fail
	UPROPERTY( config, EditAnywhere, Category = Custom, DisplayName = "Texture Group",meta = ( EditCondition = "bUseTextureGroup" ) )
	TEnumAsByte<TextureGroup> TextureGroupName = TextureGroup::TEXTUREGROUP_UI;

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePrefix = false;
	// Textures must have one of these prefixes
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( EditCondition = "bUsePrefix" ) )
	TArray<FString> Prefixes = { "T_UI_" };

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUseTextureSizeRequirement = false;
	// Textures must pass these size requirements.
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( EditCondition = "bUseTextureSizeRequirement" ) )
	EBUITextureSizeRequirement TextureSizeRequirement = EBUITextureSizeRequirement::MultipleOfFour;

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePath = false;
	// Textures not in this path within Unreal will fail.
	UPROPERTY( EditAnywhere, meta = ( ContentDir, EditCondition = "bUsePath" ) )
	FDirectoryPath Path;

	// Require that the Data Source Folder be set in Editor Preferences, and that assets are imported from there.
	UPROPERTY( EditAnywhere )
	bool bRequireDataSourceFolder = true;
};

USTRUCT()
struct FBUIValidatorGroup
{
	GENERATED_BODY()
public:
	UPROPERTY( config, EditAnywhere, Category = Conditions )
	FBUIMatchConditions MatchConditions;

	UPROPERTY( config, EditAnywhere, Category = Requirements )
	FBUIValidationRule ValidationRule;
};

UCLASS( config = Game, defaultconfig )
class UBUIValidatorSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY( config, EditAnywhere, Category = Validation )
	TArray<FBUIValidatorGroup> ValidationGroups;
};
