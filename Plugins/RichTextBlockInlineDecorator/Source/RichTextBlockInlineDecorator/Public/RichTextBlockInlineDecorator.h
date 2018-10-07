// Copyright 2018 Sam Bonifacio. All Rights Reserved.

#pragma once

#include "Components/RichTextBlockDecorator.h"
#include "RichTextBlockInlineDecorator.generated.h"

/**
 * Allows inline styling of text using <span> tags, supporting functionality that used to exist in the engine pre-4.20
 */
UCLASS()
class URichTextBlockInlineDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	URichTextBlockInlineDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};