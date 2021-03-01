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
	UTexture2D* Texture = Cast<UTexture2D>( InAsset );
	if ( Texture )
	{
		return true;
	}
	return false;
}

EDataValidationResult UBUIEditorValidator_Textures::ValidateLoadedAsset_Implementation( UObject* InAsset, TArray<FText>& ValidationErrors )
{
	const FString AssetPathName = InAsset->GetPathName();

	UTexture2D* Texture = Cast<UTexture2D>( InAsset );
	if ( Texture )
	{
		bool bAnyFailed = false;
		if ( Texture->GetSizeX() % 4 != 0
			|| Texture->GetSizeY() % 4 != 0 )
		{
			bAnyFailed = true;
			AssetFails( InAsset, FText::Format(
				LOCTEXT( "BUIValidatorError_MultipleOfFour", "Texture asset must be a multiple of 4, but size is {0} x {1}" ),
				FText::AsNumber( Texture->GetSizeX() ), FText::AsNumber( Texture->GetSizeY() ) ),
				ValidationErrors );
		}

		FString Filename = Texture->AssetImportData->GetFirstFilename();
		TArray<FString> AbsoluteFilenames = Texture->AssetImportData->ExtractFilenames();

		const UEditorPerProjectUserSettings& EditorSettings = *GetDefault<UEditorPerProjectUserSettings>();

		if ( EditorSettings.DataSourceFolder.Path.IsEmpty() )
		{
			bAnyFailed = true;
			AssetFails( InAsset, LOCTEXT( "BUIValidatorError_NoDataSourceFolder",
				"Data Source Folder is not set. Please set it in Editor Preferences." ),
				ValidationErrors );
		}
		else if ( !Filename.StartsWith( EditorSettings.DataSourceFolder.Path ) )
		{
			bAnyFailed = true;
			AssetFails( InAsset, FText::Format( LOCTEXT( "BUIValidatorError_FileImportedOutsideDataSourceFolder", "Importing a file from '{0}', outside of Data Source Folder '{1}'" ),
				FText::FromString( Filename ),
				FText::FromString( EditorSettings.DataSourceFolder.Path ) ), ValidationErrors );
		}
		if ( !bAnyFailed )
		{
			AssetPasses( InAsset );
		}
		return bAnyFailed ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
	}

	return EDataValidationResult::NotValidated;
}

#undef LOCTEXT_NAMESPACE
