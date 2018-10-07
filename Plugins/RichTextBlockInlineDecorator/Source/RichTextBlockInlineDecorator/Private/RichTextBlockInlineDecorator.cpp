// Copyright 2018 Sam Bonifacio. All Rights Reserved.

#include "RichTextBlockInlineDecorator.h"
#include "UObject/SoftObjectPtr.h"
#include "Rendering/DrawElements.h"
#include "Framework/Text/SlateTextRun.h"
#include "Components/RichTextBlock.h"

class FDefaultRichTextDecorator : public ITextDecorator
{
public:
	FDefaultRichTextDecorator(URichTextBlockDecorator* Decorator, const FSlateFontInfo& DefaultFont, const FLinearColor& DefaultColor);

	virtual ~FDefaultRichTextDecorator();

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override;

	virtual TSharedRef<ISlateRun> Create(const TSharedRef<FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style) override;

protected:

	virtual TSharedRef<ISlateRun> CreateRun(const TSharedRef<FTextLayout>& TextLayout, const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& Style, const FTextRange& InRange);

	FTextBlockStyle CreateTextBlockStyle(const FRunInfo& InRunInfo) const;

	void ExplodeRunInfo(const FRunInfo& InRunInfo, FSlateFontInfo& OutFont, FLinearColor& OutFontColor) const;

protected:

	FSlateFontInfo DefaultFont;
	FLinearColor DefaultColor;

private:
	TWeakObjectPtr<URichTextBlockDecorator> Decorator;
};

#define LOCTEXT_NAMESPACE "UMG"

class FDefaultRichTextRun : public FSlateTextRun
{
public:
	FDefaultRichTextRun(URichTextBlockDecorator* InDecorator, const TSharedRef<FTextLayout>& InTextLayout, const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& InStyle, const FTextRange& InRange)
		: FSlateTextRun(InRunInfo, InText, InStyle, InRange)
		, TextLayout(InTextLayout)
		, Decorator(InDecorator)
	{
	}

	virtual int32 OnPaint(const FPaintArgs& Args,
		const FTextLayout::FLineView& Line,
		const TSharedRef< ILayoutBlock >& Block,
		const FTextBlockStyle& DefaultStyle,
		const FGeometry& AllottedGeometry,
		const FSlateRect& ClippingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override
	{
		FSlateTextRun::OnPaint(Args, Line, Block, DefaultStyle, AllottedGeometry, ClippingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

		return LayerId;
	}

private:
	const TSharedRef<FTextLayout> TextLayout;
	TWeakObjectPtr<URichTextBlockDecorator> Decorator;
};

/////////////////////////////////////////////////////
// FDefaultRichTextDecorator

FDefaultRichTextDecorator::FDefaultRichTextDecorator(URichTextBlockDecorator* InDecorator, const FSlateFontInfo& InDefaultFont, const FLinearColor& InDefaultColor)
	: DefaultFont(InDefaultFont)
	, DefaultColor(InDefaultColor)
	, Decorator(InDecorator)
{
}

FDefaultRichTextDecorator::~FDefaultRichTextDecorator()
{
}

bool FDefaultRichTextDecorator::Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const
{
	return (RunParseResult.Name == TEXT("span"));
}

TSharedRef<ISlateRun> FDefaultRichTextDecorator::Create(const TSharedRef<FTextLayout>& TextLayout, const FTextRunParseResults& RunParseResult, const FString& OriginalText, const TSharedRef<FString>& InOutModelText, const ISlateStyle* Style)
{
	FRunInfo RunInfo(RunParseResult.Name);
	for (const TPair<FString, FTextRange>& Pair : RunParseResult.MetaData)
	{
		RunInfo.MetaData.Add(Pair.Key, OriginalText.Mid(Pair.Value.BeginIndex, Pair.Value.EndIndex - Pair.Value.BeginIndex));
	}

	FTextRange ModelRange;
	ModelRange.BeginIndex = InOutModelText->Len();
	*InOutModelText += OriginalText.Mid(RunParseResult.ContentRange.BeginIndex, RunParseResult.ContentRange.EndIndex - RunParseResult.ContentRange.BeginIndex);
	ModelRange.EndIndex = InOutModelText->Len();

	return CreateRun(TextLayout, RunInfo, InOutModelText, CreateTextBlockStyle(RunInfo), ModelRange);
}

TSharedRef<ISlateRun> FDefaultRichTextDecorator::CreateRun(const TSharedRef<FTextLayout>& TextLayout, const FRunInfo& InRunInfo, const TSharedRef< const FString >& InText, const FTextBlockStyle& Style, const FTextRange& InRange)
{
	return MakeShareable(new FDefaultRichTextRun(Decorator.Get(), TextLayout, InRunInfo, InText, Style, InRange));
}

FTextBlockStyle FDefaultRichTextDecorator::CreateTextBlockStyle(const FRunInfo& InRunInfo) const
{
	FSlateFontInfo Font;
	FLinearColor FontColor;
	ExplodeRunInfo(InRunInfo, Font, FontColor);

	FTextBlockStyle TextBlockStyle;
	TextBlockStyle.SetFont(Font);
	TextBlockStyle.SetColorAndOpacity(FontColor);

	return TextBlockStyle;
}

void FDefaultRichTextDecorator::ExplodeRunInfo(const FRunInfo& InRunInfo, FSlateFontInfo& OutFont, FLinearColor& OutFontColor) const
{
	OutFont = DefaultFont;

	const FString* const FontFamilyString = InRunInfo.MetaData.Find(TEXT("font"));
	const FString* const FontSizeString = InRunInfo.MetaData.Find(TEXT("size"));
	const FString* const FontStyleString = InRunInfo.MetaData.Find(TEXT("style"));
	const FString* const FontColorString = InRunInfo.MetaData.Find(TEXT("color"));

	if (FontFamilyString)
	{
		FSoftObjectPath Font(**FontFamilyString);
		if (UObject* FontAsset = Font.TryLoad())
		{
			OutFont.FontObject = FontAsset;
		}
	}

	if (FontSizeString)
	{
		OutFont.Size = static_cast<uint8>(FPlatformString::Atoi(**FontSizeString));
	}

	if (FontStyleString)
	{
		OutFont.TypefaceFontName = FName(**FontStyleString);
	}

	OutFontColor = DefaultColor;
	if (FontColorString)
	{
		const FString& FontColorStringRef = *FontColorString;

		// Is Hex color?
		if (!FontColorStringRef.IsEmpty() && FontColorStringRef[0] == TCHAR('#'))
		{
			OutFontColor = FLinearColor(FColor::FromHex(FontColorStringRef));
		}
		else if (OutFontColor.InitFromString(*FontColorString))
		{
			// Initialized
		}
		else
		{
			OutFontColor = DefaultColor;
		}
	}
}

/////////////////////////////////////////////////////
// UDefaultRichTextDecorator

URichTextBlockInlineDecorator::URichTextBlockInlineDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> URichTextBlockInlineDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	FSlateFontInfo DefaultFont = InOwner->GetDefaultTextStyle().Font;
	FLinearColor DefaultColor = InOwner->GetDefaultTextStyle().ColorAndOpacity.GetSpecifiedColor();
	return MakeShareable(new FDefaultRichTextDecorator(this, DefaultFont, DefaultColor));
}

/////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
