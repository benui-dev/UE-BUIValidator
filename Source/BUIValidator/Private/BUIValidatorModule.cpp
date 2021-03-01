#include "BUIValidatorModule.h"

#define LOCTEXT_NAMESPACE "FBUIValidatorModule"

void FBUIValidatorModule::StartupModule()
{
	//UBUIValidator::Startup();
}

void FBUIValidatorModule::ShutdownModule()
{
	//UBUIValidator::Shutdown();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBUIValidatorModule, BUIValidator)

