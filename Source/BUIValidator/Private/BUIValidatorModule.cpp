#include "BUIValidatorModule.h"
#include "BUIValidatorSettings.h"
#include <ISettingsModule.h>
#include <ISettingsSection.h>
#include <ISettingsContainer.h>

#define LOCTEXT_NAMESPACE "FBUIValidatorModule"

void FBUIValidatorModule::StartupModule()
{
	if ( ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>( "Settings" ) )
	{
		ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer( "Project" );
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings( "Project", "Plugins", "BUI Validator",
			LOCTEXT( "RuntimeGeneralSettingsName", "BUI Validator" ),
			LOCTEXT( "RuntimeGeneralSettingsDescription", "Configure UI data asset validation." ),
			GetMutableDefault<UBUIValidatorSettings>()
		);

		if ( SettingsSection.IsValid() )
		{
			SettingsSection->OnModified().BindRaw( this, &FBUIValidatorModule::HandleSettingsSaved );
		}
	}
}

void FBUIValidatorModule::ShutdownModule()
{
	if ( ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>( "Settings" ) )
	{
		SettingsModule->UnregisterSettings( "Project", "CustomSettings", "General" );
	}
}

bool FBUIValidatorModule::HandleSettingsSaved()
{
	UBUIValidatorSettings* Settings = GetMutableDefault<UBUIValidatorSettings>();
	bool ResaveSettings = false;

	// You can put any validation code in here and resave the settings in case an invalid
	// value has been entered

	if ( ResaveSettings )
	{
		Settings->SaveConfig();
	}

	return true;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBUIValidatorModule, BUIValidator)

