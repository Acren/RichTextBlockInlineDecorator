// Copyright 2018 Sam Bonifacio. All Rights Reserved.

#include "RichTextBlockInlineDecoratorModule.h"

#define LOCTEXT_NAMESPACE "FRichTextBlockInlineDecoratorModule"

void FRichTextBlockInlineDecoratorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FRichTextBlockInlineDecoratorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRichTextBlockInlineDecoratorModule, RichTextBlockInlineDecorator)