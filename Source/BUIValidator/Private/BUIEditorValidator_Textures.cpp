#include "BUIEditorValidator_Textures.h"
#include <Engine/Texture2D.h>
#include <Editor/EditorPerProjectUserSettings.h>
#include <EditorFramework/AssetImportData.h>

#define LOCTEXT_NAMESPACE "BUIEditorValidator"

UBUIEditorValidator_Textures::UBUIEditorValidator_Textures()
	: Super()
{
	bIsEnabled = true;
}

bool UBUIEditorValidator_Textures::CanValidateAsset_Implementation( UObject* InAsset ) const
{
	return ShouldValidateAsset( InAsset );
}

bool GetIsPowerOfTwo( int32 Num )
{
	return ( Num & ( Num - 1 ) ) == 0;
}

bool UBUIEditorValidator_Textures::ShouldValidateAsset( UObject* InAsset ) const
{
	UTexture2D* Texture = Cast<UTexture2D>( InAsset );
	if ( Texture )
	{
		const UBUIValidatorSettings& ValidatorSettings = *GetDefault<UBUIValidatorSettings>();
		for ( const auto& Group : ValidatorSettings.ValidationGroups )
		{
			if ( ShouldGroupValidateAsset( Group, InAsset ) )
				return true;
		}
	}

	return false;
}

bool UBUIEditorValidator_Textures::ShouldGroupValidateAsset( const FBUIValidatorGroup& Group, UObject* InAsset ) const
{
	UTexture2D* Texture = Cast<UTexture2D>( InAsset );
	if ( !Texture )
		return false;

	const FString AssetPathInUnreal = Texture->GetPathName();

	bool bMatchAnyTextureGroup = Group.MatchConditions.TextureGroups.Num() == 0
		|| Group.MatchConditions.TextureGroups.Contains( Texture->LODGroup );

	bool bMatchAnyPath = Group.MatchConditions.Paths.Num() == 0;
	for ( const auto& Path : Group.MatchConditions.Paths )
	{
		if ( AssetPathInUnreal.StartsWith( Path.Path ) )
		{
			bMatchAnyPath = true;
			break;
		}
	}

	bool bMatchAnyPrefix = Group.MatchConditions.Prefixes.Num() == 0;
	for ( const auto& Prefix : Group.MatchConditions.Prefixes )
	{
		if ( FPaths::GetCleanFilename( AssetPathInUnreal ).StartsWith( Prefix ) )
		{
			bMatchAnyPrefix = true;
			break;
		}
	}

	// Let's apply rules to this texture
	return bMatchAnyTextureGroup && bMatchAnyPath && bMatchAnyPrefix;
}

EDataValidationResult UBUIEditorValidator_Textures::ValidateLoadedAsset_Implementation( UObject* InAsset, TArray<FText>& ValidationErrors )
{
	bool bAnyFailed = false;
	bool bAnyChecked = false;

	UTexture2D* Texture = Cast<UTexture2D>( InAsset );
	if ( Texture )
	{
		const FString ImportAssetPath = Texture->AssetImportData->GetFirstFilename();
		const UBUIValidatorSettings& ValidatorSettings = *GetDefault<UBUIValidatorSettings>();

		// First see if this matches
		for ( const auto& Group : ValidatorSettings.ValidationGroups )
		{
			if ( ShouldGroupValidateAsset( Group, InAsset ) )
			{
				if ( Group.ValidationRule.TextureGroups.Num() )
				{
					bAnyChecked = true;

					if ( !Group.ValidationRule.TextureGroups.Contains( Texture->LODGroup ) )
					{
						bAnyFailed = true;
						TArray<FString> TextureGroupNames;
						for ( const auto& TextureGroup : Group.ValidationRule.TextureGroups )
						{
							TextureGroupNames.Add( UTexture::GetTextureGroupString( TextureGroup ) );
						}
						AssetFails( InAsset, FText::Format(
							LOCTEXT( "BUIValidatorError_TextureGroup", "Texture asset size must be set to texture group '{0}', but is set to texture group '{1}'" ),
							FText::FromString( FString::Join( TextureGroupNames, TEXT( ", " ) ) ),
							FText::FromString( UTexture::GetTextureGroupString( Texture->LODGroup ) ) ),
							ValidationErrors );
					}
				}

				if ( Group.ValidationRule.TextureSizeRequirements.Num() > 0 )
				{
					bAnyChecked = true;
					if ( Group.ValidationRule.TextureSizeRequirements.Contains( EBUITextureSizeRequirement::MultipleOfFour )
						&& ( Texture->GetSizeX() % 4 != 0
							|| Texture->GetSizeY() % 4 != 0 ) )
					{
						bAnyFailed = true;
						AssetFails( InAsset, FText::Format(
							LOCTEXT( "BUIValidatorError_MultipleOfFour", "Texture asset size must be a multiple of 4, but size is {0} x {1}" ),
							FText::AsNumber( Texture->GetSizeX() ), FText::AsNumber( Texture->GetSizeY() ) ),
							ValidationErrors );
					}
					else if ( Group.ValidationRule.TextureSizeRequirements.Contains( EBUITextureSizeRequirement::PowerOfTwo )
						&& ( !GetIsPowerOfTwo( Texture->GetSizeX() )
							|| !GetIsPowerOfTwo( Texture->GetSizeY() ) ) )
					{
						bAnyFailed = true;
						AssetFails( InAsset, FText::Format(
							LOCTEXT( "BUIValidatorError_PowerOfTwo", "Texture asset size must be a power of two, but size is {0} x {1}" ),
							FText::AsNumber( Texture->GetSizeX() ), FText::AsNumber( Texture->GetSizeY() ) ),
							ValidationErrors );
					}
				}

				if ( Group.ValidationRule.Prefixes.Num() > 0 )
				{
					bAnyChecked = true;
					const FString Filename = FPaths::GetCleanFilename( ImportAssetPath );
					bool bAnyMatched = false;
					for ( const auto& Prefix : Group.ValidationRule.Prefixes )
					{
						if ( Filename.StartsWith( Prefix ) )
						{
							bAnyMatched = true;
						}
					}
					if ( !bAnyMatched )
					{
						bAnyFailed = true;
						AssetFails( InAsset, FText::Format(
							LOCTEXT( "BUIValidatorError_Prefix", "Texture asset name must start with one of the prefixes '{0}', but name is '{1}'" ),
							FText::FromString( FString::Join( Group.ValidationRule.Prefixes, TEXT(", ") ) ),
							FText::FromString( Filename ) ),
							ValidationErrors );
					}
				}

				if ( Group.ValidationRule.bRequireDataSourceFolder )
				{
					bAnyChecked = true;
					TArray<FString> AbsoluteFilenames = Texture->AssetImportData->ExtractFilenames();

					const UEditorPerProjectUserSettings& EditorSettings = *GetDefault<UEditorPerProjectUserSettings>();

					if ( EditorSettings.DataSourceFolder.Path.IsEmpty() )
					{
						bAnyFailed = true;
						AssetFails( InAsset, LOCTEXT( "BUIValidatorError_NoDataSourceFolder",
							"Data Source Folder is not set. Please set it in Editor Preferences" ),
							ValidationErrors );
					}
					else if ( !ImportAssetPath.StartsWith( EditorSettings.DataSourceFolder.Path ) )
					{
						bAnyFailed = true;
						AssetFails( InAsset, FText::Format( LOCTEXT( "BUIValidatorError_FileImportedOutsideDataSourceFolder", "Importing a file from '{0}', outside of Data Source Folder '{1}'" ),
							FText::FromString( ImportAssetPath ),
							FText::FromString( EditorSettings.DataSourceFolder.Path ) ), ValidationErrors );
					}
				}
			}
		}
	}

	if ( !bAnyChecked )
		return EDataValidationResult::NotValidated;

	if ( !bAnyFailed )
	{
		AssetPasses( InAsset );
	}
	return bAnyFailed ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}

#undef LOCTEXT_NAMESPACE
