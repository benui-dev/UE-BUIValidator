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
		// Defaults
		if ( Paths.Num() == 0 )
		{
			Paths.Add( FDirectoryPath() );
			Paths[ 0 ].Path = "/Game/UI";
		}
	}
public:
	// Textures not with this Texture Group will fail
	UPROPERTY( config, EditAnywhere )
	TArray<TEnumAsByte<TextureGroup>> TextureGroups = { TextureGroup::TEXTUREGROUP_UI };

	// Textures must have one of these prefixes
	UPROPERTY( config, EditAnywhere )
	TArray<FString> Prefixes = { "T_UI_" };

	// Textures must pass these size requirements.
	UPROPERTY( config, EditAnywhere )
	TSet<EBUITextureSizeRequirement> TextureSizeRequirements = { EBUITextureSizeRequirement::MultipleOfFour };

	// Textures not in this path within Unreal will fail.
	UPROPERTY( EditAnywhere, meta = ( ContentDir ) )
	TArray<FDirectoryPath> Paths;

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
