// Copyright ben ui. All Rights Reserved.

#include "BUIEditorValidator_Textures.h"
#include "Engine/Texture2D.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "EditorFramework/AssetImportData.h"
#include "BUIValidatorSettings.h"

#define LOCTEXT_NAMESPACE "BUIEditorValidator"

UBUIEditorValidator_Textures::UBUIEditorValidator_Textures()
	: Super()
{
	bIsEnabled = true;
}

bool UBUIEditorValidator_Textures::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const
{
	const UBUIValidatorSettings& ValidatorSettings = *GetDefault<UBUIValidatorSettings>();
	return ValidatorSettings.ShouldValidateAsset(InObject);
}

bool GetIsPowerOfTwo(int32 Num)
{
	return (Num & (Num - 1)) == 0;
}

EDataValidationResult UBUIEditorValidator_Textures::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context)
{
	bool bAnyFailed = false;
	bool bAnyChecked = false;

	UTexture2D* Texture = Cast<UTexture2D>(InAsset);
	if (Texture)
	{
		const FString ImportAssetPath = Texture->AssetImportData->GetFirstFilename();
		const UBUIValidatorSettings& ValidatorSettings = *GetDefault<UBUIValidatorSettings>();

		// First see if this matches
		for (const auto& Group : ValidatorSettings.ValidationGroups)
		{
			if (Group.ShouldGroupValidateAsset(InAsset))
			{
				if (Group.ValidationRule.TextureGroups.Num())
				{
					bAnyChecked = true;

					if (!Group.ValidationRule.TextureGroups.Contains(Texture->LODGroup))
					{
						bAnyFailed = true;
						TArray<FString> TextureGroupNames;
						UEnum* TextureGroupEnum = StaticEnum<TextureGroup>();
						for (const auto& TextureGroup : Group.ValidationRule.TextureGroups)
						{
							TextureGroupNames.Add(TextureGroupEnum->GetMetaData(TEXT("DisplayName"), TextureGroup));
						}
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_TextureGroup", "Texture asset texture group must be '{0}', but is '{1}'"),
							FText::FromString(FString::Join(TextureGroupNames, TEXT(", "))),
							FText::FromString(TextureGroupEnum->GetMetaData(TEXT("DisplayName"), Texture->LODGroup))));
					}
				}

				if (Group.ValidationRule.PixelFormats.Num())
				{
					bAnyChecked = true;
					if (!Group.ValidationRule.PixelFormats.Contains(Texture->GetPixelFormat()))
					{
						bAnyFailed = true;
						TArray<FString> PixelFormatNames;
						for (const auto& PixelFormat : Group.ValidationRule.PixelFormats)
						{
							PixelFormatNames.Add(UEnum::GetValueAsString(PixelFormat));
						}
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_PixelFormat","Texture asset pixel format must be '{0}', but is '{1}'"),
							FText::FromString(FString::Join(PixelFormatNames, TEXT(", "))),
							FText::FromString(UEnum::GetValueAsString(Texture->GetPixelFormat()))));
					}
				}

				if (Group.ValidationRule.CompressionSettings.Num())
				{
					bAnyChecked = true;
					if (!Group.ValidationRule.CompressionSettings.Contains(Texture->CompressionSettings))
					{
						bAnyFailed = true;
						TArray<FString> CompressionSettingNames;
						UEnum* CompressionSettingsEnum = StaticEnum<TextureCompressionSettings>();
						for (const auto& CompressionSetting : Group.ValidationRule.CompressionSettings)
						{
							CompressionSettingNames.Add(
								CompressionSettingsEnum->GetMetaData(TEXT("DisplayName"), CompressionSetting));
						}
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_PixelFormat","Texture asset pixel format must be '{0}', but is '{1}'"),
							FText::FromString(FString::Join(CompressionSettingNames, TEXT(", "))),
							FText::FromString(CompressionSettingsEnum->GetMetaData(TEXT("DisplayName"), Texture->CompressionSettings))));
					}
				}

				if (Group.ValidationRule.MipGenSettings.Num())
				{
					bAnyChecked = true;
					if (!Group.ValidationRule.MipGenSettings.Contains(Texture->MipGenSettings))
					{
						bAnyFailed = true;
						TArray<FString> MipGenSettingNames;
						UEnum* MipGenSettingsEnum = StaticEnum<TextureMipGenSettings>();
						for (const auto& MipGenSetting : Group.ValidationRule.MipGenSettings)
						{
							MipGenSettingNames.Add(MipGenSettingsEnum->GetMetaData(TEXT("DisplayName"), MipGenSetting));
						}
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_PixelFormat", "Texture asset mip gen settings must be '{0}', but is '{1}'"),
							FText::FromString(FString::Join(MipGenSettingNames, TEXT(", "))),
							FText::FromString(MipGenSettingsEnum->GetMetaData(TEXT("DisplayName"), Texture->MipGenSettings))));
					}
				}

				if (Group.ValidationRule.TextureSizeRequirements.Num() > 0)
				{
					bAnyChecked = true;
					if (Group.ValidationRule.TextureSizeRequirements.Contains(EBUITextureSizeRequirement::MultipleOfFour)
						&& (Texture->GetSizeX() % 4 != 0 || Texture->GetSizeY() % 4 != 0))
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_MultipleOfFour",
							"Texture asset size must be a multiple of 4, but is {0}x{1}"),
							FText::AsNumber(Texture->GetSizeX(), &FNumberFormattingOptions::DefaultNoGrouping()),
							FText::AsNumber(Texture->GetSizeY(), &FNumberFormattingOptions::DefaultNoGrouping())));
					}
					else if (Group.ValidationRule.TextureSizeRequirements.Contains(EBUITextureSizeRequirement::PowerOfTwo)
						&& (!GetIsPowerOfTwo(Texture->GetSizeX()) || !GetIsPowerOfTwo(Texture->GetSizeY())))
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_PowerOfTwo", "Texture asset size must be a power of two, but is {0}x{1}"),
							FText::AsNumber(Texture->GetSizeX(), &FNumberFormattingOptions::DefaultNoGrouping()),
							FText::AsNumber(Texture->GetSizeY(), &FNumberFormattingOptions::DefaultNoGrouping())));
					}
				}

				if (Group.ValidationRule.Prefixes.Num() > 0)
				{
					bAnyChecked = true;
					const FString Filename = FPaths::GetCleanFilename(ImportAssetPath);
					bool bAnyMatched = false;
					for (const auto& Prefix : Group.ValidationRule.Prefixes)
					{
						if (Filename.StartsWith(Prefix))
						{
							bAnyMatched = true;
						}
					}
					if (!bAnyMatched)
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_Prefix", "Texture asset name must be prefixed with '{0}', but is '{1}'"),
							FText::FromString(FString::Join(Group.ValidationRule.Prefixes, TEXT(", "))),
							FText::FromString(Filename)));
					}
				}

				if (Group.ValidationRule.Suffixes.Num() > 0)
				{
					bAnyChecked = true;
					const FString Filename = FPaths::GetCleanFilename(ImportAssetPath);
					bool bAnyMatched = false;
					for (const auto& Suffix : Group.ValidationRule.Suffixes)
					{
						if (Filename.EndsWith(Suffix))
						{
							bAnyMatched = true;
						}
					}
					if (!bAnyMatched)
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_Suffix", "Texture asset name must be suffixed with '{0}', but is '{1}'"),
							FText::FromString(FString::Join(Group.ValidationRule.Suffixes, TEXT(", "))),
							FText::FromString(Filename)));
					}
				}

				if (Group.ValidationRule.bRequireDataSourceFolder)
				{
					bAnyChecked = true;
					TArray<FString> AbsoluteFilenames = Texture->AssetImportData->ExtractFilenames();

					const UEditorPerProjectUserSettings& EditorSettings = *GetDefault<UEditorPerProjectUserSettings>();

					if (EditorSettings.DataSourceFolder.Path.IsEmpty())
					{
						bAnyFailed = true;
						AssetFails(InAsset, LOCTEXT("BUIValidatorError_NoDataSourceFolder", "Data Source Folder must be set. Please set it in Editor Preferences"));
					}
					else if (!ImportAssetPath.StartsWith(EditorSettings.DataSourceFolder.Path))
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
						LOCTEXT("BUIValidatorError_FileImportedOutsideDataSourceFolder", "Importing a file from '{0}', outside of Data Source Folder '{1}'"),
							FText::FromString(ImportAssetPath),
							FText::FromString(EditorSettings.DataSourceFolder.Path)));
					}
				}

				if (Group.ValidationRule.bSpecifySRGB)
				{
					bAnyChecked = true;

					if (Texture->SRGB != Group.ValidationRule.bSRGB)
					{
						bAnyFailed = true;
						AssetFails(InAsset, FText::Format(
							LOCTEXT("BUIValidatorError_SRGBMismatch", "Texture asset must have sRGB set to '{0}', was set to '{1}'"),
							Group.ValidationRule.bSRGB ? LOCTEXT("True", "True") : LOCTEXT("False", "False"),
							Texture->SRGB ? LOCTEXT("True", "True") : LOCTEXT("False", "False")));
					}
				}
			}
		}
	}

	if (!bAnyChecked)
		return EDataValidationResult::NotValidated;

	if (!bAnyFailed)
	{
		AssetPasses(InAsset);
	}
	return bAnyFailed ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}

#undef LOCTEXT_NAMESPACE
