#include "BUIEditorValidator_RequiredProperty.h"
#include <Engine/Texture2D.h>
#include <Editor/EditorPerProjectUserSettings.h>
#include <EditorFramework/AssetImportData.h>
#include "BUIValidatorSettings.h"
#include <UObject/UnrealType.h>

#define LOCTEXT_NAMESPACE "BUIEditorValidator"

UBUIEditorValidator_RequiredProperty::UBUIEditorValidator_RequiredProperty()
	: Super()
{
	bIsEnabled = true;
}

bool UBUIEditorValidator_RequiredProperty::CanValidateAsset_Implementation( UObject* InAsset ) const
{
	UClass* AssetClass = nullptr;
	UBlueprint* Bp = Cast<UBlueprint>( InAsset );
	if ( Bp && Bp->ParentClass )
	{
		AssetClass = Bp->ParentClass;
	}
	else
	{
		AssetClass = InAsset->GetClass();
	}
	for ( TFieldIterator<FProperty> PropertyIterator( AssetClass ); PropertyIterator; ++PropertyIterator )
	{
		if ( PropertyIterator->HasMetaData( "BUIRequired" ) )
		{
			return true;
		}
	}
	return false;
}

EDataValidationResult UBUIEditorValidator_RequiredProperty::ValidateLoadedAsset_Implementation( UObject* InAsset, TArray<FText>& ValidationErrors )
{
	bool bAnyFailed = false;
	bool bAnyChecked = false;

	UClass* AssetClass = nullptr;
	UBlueprint* Bp = Cast<UBlueprint>( InAsset );
	if ( Bp && Bp->ParentClass )
	{
		AssetClass = Bp->ParentClass;
	}
	else
	{
		AssetClass = InAsset->GetClass();
	}

	UObject* MyCDO = AssetClass->ClassDefaultObject;
	for ( TFieldIterator<FProperty> PropertyIterator( AssetClass ); PropertyIterator; ++PropertyIterator )
	{
		if ( PropertyIterator->HasMetaData( "BUIRequired" ) )
		{
			bAnyChecked = true;
			FObjectProperty* ObjProp = CastField<FObjectProperty>( *PropertyIterator );
			if ( ObjProp )
			{
				// How can I tell if the object property is "set"? Whether it's pointing to an object?
				UObject* ValueA = ObjProp->GetObjectPropertyValue_InContainer( InAsset );
				UObject* ValueB = ObjProp->GetObjectPropertyValue_InContainer( Bp );
				UObject* ValueC = ObjProp->GetObjectPropertyValue_InContainer( Bp->ParentClass );

				UObject* Container1 = ObjProp->GetPropertyValue_InContainer( InAsset );
				UObject* Container2 = ObjProp->GetPropertyValue_InContainer( Bp );
				UObject* Container3 = ObjProp->GetPropertyValue_InContainer( Bp->ParentClass );
				UObject* Container4 = ObjProp->GetPropertyValue_InContainer( MyCDO );

				UObject** ContainerA = ObjProp->GetPropertyValuePtr_InContainer( InAsset );
				UObject** ContainerB = ObjProp->GetPropertyValuePtr_InContainer( Bp );
				UObject** ContainerC = ObjProp->GetPropertyValuePtr_InContainer( Bp->ParentClass );
				UObject** ContainerD = ObjProp->GetPropertyValuePtr_InContainer( MyCDO );


				//const void* ObjectContainer1 = ObjProp->ContainerPtrToValuePtr<const void>( Bp->ClassDefaultObject );
				//const UObject* Object1 = ObjProp->GetObjectPropertyValue( ObjectContainer1 );

				//const void* ObjectContainer2 = ObjProp->ContainerPtrToValuePtr<const void>( InAsset->GetClass()->GetDefaultObject() );
				//const UObject* Object2 = ObjProp->GetObjectPropertyValue( ObjectContainer2 );

				//const void* ObjectContainer3 = ObjProp->ContainerPtrToValuePtr<const void>( AssetClass->GetDefaultObject() );
				//const UObject* Object3 = ObjProp->GetObjectPropertyValue( ObjectContainer3 );

				//UObject* ValueD = ObjProp->GetObjectPropertyValue( PropertyIterator->ContainerPtrToValuePtr<UObject*>( AssetClass ) );
				//UObject* ValueE = ObjProp->GetObjectPropertyValue( PropertyIterator->ContainerPtrToValuePtr<UObject*>( Bp ) );
				//UObject* ValueF = ObjProp->GetObjectPropertyValue( PropertyIterator->ContainerPtrToValuePtr<UObject*>( InAsset ) );
				//UObject** Container2 = ObjProp->GetPropertyValuePtr( MyCDO );
				//UObject* Value1 = ObjProp->GetObjectPropertyValue( Container1 );
				//UObject* Value2 = ObjProp->GetObjectPropertyValue( Container2 );
				//UObject* ValueG = ObjProp->GetObjectPropertyValue( PropertyIterator->ContainerPtrToValuePtr<UObject*>( MyCDO ) );
				//UObject* ValueH = ObjProp->GetObjectPropertyValue( MyCDO );
				//UObject* ValueC = ObjProp->GetObjectPropertyValue_InContainer( InAsset->ClassDefaultObject );
				//UObject* ValueD = ObjProp->GetObjectPropertyValue_InContainer( AssetClass->ClassDefaultObject );
				//UObject* ValueE = ObjProp->GetObjectPropertyValue_InContainer( Bp->ClassDefaultObject );
				//UObject* Container1 = ObjProp->GetPropertyValuePtr()<UObject*>( InAsset );
				//UObject* Value1 = ObjProp->GetObjectPropertyValue( Container1 );
				//UObject* Value2 = ObjProp->GetObjectPropertyValue( ObjProp->ContainerPtrToValuePtr<void>( InAsset ) );
				//UObject* Value3 = ObjProp->GetObjectPropertyValue( ObjProp->ContainerPtrToValuePtr<UObject*>( Bp ) );
				//UObject* Value4 = ObjProp->GetObjectPropertyValue( ObjProp->ContainerPtrToValuePtr<void>( Bp ) );
				//UObject* Value5 = ObjProp->GetObjectPropertyValue( ObjProp->ContainerPtrToValuePtr<UObject*>( AssetClass->ClassDefaultObject ) );
				//UObject* Value6 = ObjProp->GetObjectPropertyValue( ObjProp->ContainerPtrToValuePtr<void>( AssetClass->ClassDefaultObject ) );
				if (ValueA->IsValidLowLevel() )
				{
					bAnyFailed = true;
					AssetFails( InAsset, LOCTEXT( "BUIValidatorError_NotSet", "All class variables marked BUIRequired must be set to non-null" ), ValidationErrors );
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
