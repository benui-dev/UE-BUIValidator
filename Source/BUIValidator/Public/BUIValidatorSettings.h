#pragma once

#include "BUIValidatorSettings.generated.h"

UENUM()
enum class EBUITextureSizeRequirement
{
	MultipleOfFour,
	PowerOfTwo,
};

USTRUCT()
struct FBUIMatchConditions
{
	GENERATED_BODY()
public:
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUseTextureGroup = false;
	// Match UTexture2D assets with this texture group
	UPROPERTY( config, EditAnywhere, Category = Custom, DisplayName = "Texture Group", meta = ( EditCondition = "bUseTextureGroup" ) )
	TEnumAsByte<TextureGroup> TextureGroupName = TextureGroup::TEXTUREGROUP_UI;

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePrefix = false;
	// Match UTexture2D assets with the prefix
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( EditCondition = "bUsePrefix" ) )
	FString Prefix = "T_UI_";

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePath = false;
	// Match UTexture2D assets under this directory
	UPROPERTY( EditAnywhere, meta = ( ContentDir, EditCondition = "bUsePath" ) )
	FDirectoryPath Path;
};

USTRUCT()
struct FBUIValidationRule
{
	GENERATED_BODY()
public:
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUseTextureGroup = false;
	// Textures not with this Texture Group
	UPROPERTY( config, EditAnywhere, Category = Custom, DisplayName = "Texture Group",meta = ( EditCondition = "bUseTextureGroup" ) )
	TEnumAsByte<TextureGroup> TextureGroupName = TextureGroup::TEXTUREGROUP_UI;

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePrefix = false;
	// Files without this prefix will fail
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( EditCondition = "bUsePrefix" ) )
	FString Prefix = "T_UI_";

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUseTextureSizeRequirement = false;
	// Textures without this prefix will fail
	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( EditCondition = "bUseTextureSizeRequirement" ) )
	EBUITextureSizeRequirement TextureSizeRequirement = EBUITextureSizeRequirement::MultipleOfFour;

	UPROPERTY( config, EditAnywhere, Category = Custom, meta = ( InlineEditConditionToggle ) )
	bool bUsePath = false;
	// Textures not in this path will fail 
	UPROPERTY( EditAnywhere, meta = ( ContentDir, EditCondition = "bUsePath" ) )
	FDirectoryPath Path;

	UPROPERTY( EditAnywhere )
	bool bRequireDataFolder = true;
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
