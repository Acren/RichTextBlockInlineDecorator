// Copyright 2018 Sam Bonifacio. All Rights Reserved.

#pragma once

#include "RichTextBlockDecorator.h"
#include "RichTextBlockInlineDecorator.generated.h"

UCLASS()
class URichTextBlockInlineDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	URichTextBlockInlineDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};