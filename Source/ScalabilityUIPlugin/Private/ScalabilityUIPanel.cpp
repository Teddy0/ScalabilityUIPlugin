// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "ScalabilityUIPrivatePCH.h"
#include "ScalabilityUIPanel.h"
#include "Engine.h"

#define LOCTEXT_NAMESPACE "EngineScalabiltySettings"

ESlateCheckBoxState::Type SScalabilityUIPanel::IsGroupQualityLevelSelected(const TCHAR* InGroupName, int32 InQualityLevel) const
{
	int32 QualityLevel = -1;

	if (FCString::Strcmp(InGroupName, TEXT("ResolutionQuality")) == 0) { QualityLevel = CachedQualityLevels.ResolutionQuality; }
	else if (FCString::Strcmp(InGroupName, TEXT("ViewDistanceQuality")) == 0) QualityLevel = CachedQualityLevels.ViewDistanceQuality;
	else if (FCString::Strcmp(InGroupName, TEXT("AntiAliasingQuality")) == 0) QualityLevel = CachedQualityLevels.AntiAliasingQuality;
	else if (FCString::Strcmp(InGroupName, TEXT("PostProcessQuality")) == 0) QualityLevel = CachedQualityLevels.PostProcessQuality;
	else if (FCString::Strcmp(InGroupName, TEXT("ShadowQuality")) == 0) QualityLevel = CachedQualityLevels.ShadowQuality;
	else if (FCString::Strcmp(InGroupName, TEXT("TextureQuality")) == 0) QualityLevel = CachedQualityLevels.TextureQuality;
	else if (FCString::Strcmp(InGroupName, TEXT("EffectsQuality")) == 0) QualityLevel = CachedQualityLevels.EffectsQuality;

	return (QualityLevel == InQualityLevel) ? ESlateCheckBoxState::Checked : ESlateCheckBoxState::Unchecked;
}

void SScalabilityUIPanel::OnGroupQualityLevelChanged(ESlateCheckBoxState::Type NewState, const TCHAR* InGroupName, int32 InQualityLevel)
{
	if (FCString::Strcmp(InGroupName, TEXT("ResolutionQuality")) == 0) CachedQualityLevels.ResolutionQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("ViewDistanceQuality")) == 0) CachedQualityLevels.ViewDistanceQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("AntiAliasingQuality")) == 0) CachedQualityLevels.AntiAliasingQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("PostProcessQuality")) == 0) CachedQualityLevels.PostProcessQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("ShadowQuality")) == 0) CachedQualityLevels.ShadowQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("TextureQuality")) == 0) CachedQualityLevels.TextureQuality = InQualityLevel;
	else if (FCString::Strcmp(InGroupName, TEXT("EffectsQuality")) == 0) CachedQualityLevels.EffectsQuality = InQualityLevel;

	Scalability::SetQualityLevels(CachedQualityLevels);
	Scalability::SaveState(GGameUserSettingsIni);

	//Write the values into the ini
	const TCHAR* OculusSettings = TEXT("Oculus.Settings");
	GConfig->SetBool(OculusSettings, TEXT("bOverrideScreenPercentage"), CachedQualityLevels.ResolutionQuality < Scalability::MaxResolutionScale, GEngineIni);
	GConfig->SetFloat(OculusSettings, TEXT("ScreenPercentage"), (float)CachedQualityLevels.ResolutionQuality * 1.35f, GEngineIni);
}

void SScalabilityUIPanel::OnResolutionScaleChanged(float InValue)
{
	CachedQualityLevels.ResolutionQuality = (int32)FMath::Lerp(Scalability::MinResolutionScale, Scalability::MaxResolutionScale, InValue);
	Scalability::SetQualityLevels(CachedQualityLevels);
	Scalability::SaveState(GGameUserSettingsIni);

	//Write the values into the ini
	const TCHAR* OculusSettings = TEXT("Oculus.Settings");
	GConfig->SetBool(OculusSettings, TEXT("bOverrideScreenPercentage"), CachedQualityLevels.ResolutionQuality < Scalability::MaxResolutionScale, GEngineIni);
	GConfig->SetFloat(OculusSettings, TEXT("ScreenPercentage"), (float)CachedQualityLevels.ResolutionQuality * 1.35f, GEngineIni);
}

float SScalabilityUIPanel::GetResolutionScale() const
{
	return (float)(CachedQualityLevels.ResolutionQuality - Scalability::MinResolutionScale) / (float)(Scalability::MaxResolutionScale - Scalability::MinResolutionScale);
}

FString SScalabilityUIPanel::GetResolutionScaleString() const
{
	return FString::Printf(TEXT("%1.1fx"), ((float)CachedQualityLevels.ResolutionQuality / Scalability::MaxResolutionScale));
}

TSharedRef<SWidget> SScalabilityUIPanel::MakeButtonWidget(const FText& InName, const TCHAR* InGroupName, int32 InQualityLevel, const FText& InToolTip)
{
	return SNew(SCheckBox)
		.Style(FCoreStyle::Get(), "ToggleButtonCheckbox")
		.OnCheckStateChanged(this, &SScalabilityUIPanel::OnGroupQualityLevelChanged, InGroupName, InQualityLevel)
		.IsChecked(this, &SScalabilityUIPanel::IsGroupQualityLevelSelected, InGroupName, InQualityLevel)
		.ToolTipText(InToolTip)
		.Content()
		[
			SNew(STextBlock)
			.Text(InName)
		];
}

TSharedRef<SWidget> SScalabilityUIPanel::MakeHeaderButtonWidget(const FText& InName, int32 InQualityLevel, const FText& InToolTip)
{
	return SNew(SButton)
		.OnClicked(this, &SScalabilityUIPanel::OnHeaderClicked, InQualityLevel)
		.ToolTipText(InToolTip)
		.Content()
		[
			SNew(STextBlock)
			.Text(InName)
		];
}

TSharedRef<SWidget> SScalabilityUIPanel::MakeLaunchButtonWidget(const FText& InName, int32 InReturnCode, const FText& InToolTip)
{
	return SNew(SButton)
		.OnClicked(this, &SScalabilityUIPanel::OnLaunchClicked, InReturnCode)
		.ToolTipText(InToolTip)
		.Content()
		[
			SNew(STextBlock)
			.Text(InName)
		];
}

FReply SScalabilityUIPanel::OnHeaderClicked(int32 InQualityLevel)
{
	CachedQualityLevels.SetFromSingleQualityLevel(InQualityLevel);
	Scalability::SetQualityLevels(CachedQualityLevels);
	Scalability::SaveState(GGameUserSettingsIni);

	//Write the values into the ini
	const TCHAR* OculusSettings = TEXT("Oculus.Settings");
	GConfig->SetBool(OculusSettings, TEXT("bOverrideScreenPercentage"), CachedQualityLevels.ResolutionQuality < Scalability::MaxResolutionScale, GEngineIni);
	GConfig->SetFloat(OculusSettings, TEXT("ScreenPercentage"), (float)CachedQualityLevels.ResolutionQuality * 1.35f, GEngineIni);

	return FReply::Handled();
}

FReply SScalabilityUIPanel::OnLaunchClicked(int32 InReturnCode)
{
	ReturnCode = InReturnCode;
	RequestDestroyWindow();
	return FReply::Handled();
}

SGridPanel::FSlot& SScalabilityUIPanel::MakeGridSlot(int32 InCol, int32 InRow, int32 InColSpan /*= 1*/, int32 InRowSpan /*= 1*/)
{
	float PaddingH = 2.0f;
	float PaddingV = InRow == 0 ? 8.0f : 2.0f;
	return SGridPanel::Slot(InCol, InRow)
		.Padding(PaddingH, PaddingV)
		.RowSpan(InRowSpan)
		.ColumnSpan(InColSpan);
}

void SScalabilityUIPanel::Construct(const FArguments& InArgs)
{
	const FText NamesLow(LOCTEXT("QualityLowLabel", "Low"));
	const FText NamesMedium(LOCTEXT("QualityMediumLabel", "Medium"));
	const FText NamesHigh(LOCTEXT("QualityHighLabel", "High"));
	const FText NamesEpic(LOCTEXT("QualityEpicLabel", "Epic"));
	const FText NamesAuto(LOCTEXT("QualityAutoLabel", "Auto"));

	auto TitleFont = FCoreStyle::Get().GetFontStyle(FName("ExpandableArea.TitleFont"));
	auto GroupFont = FCoreStyle::Get().GetFontStyle(FName("ExpandableArea.TitleFont"));

	const FName BannerName( *(FPaths::GamePluginsDir() / TEXT("ScalabilityUIPlugin/Resources/Banner.png")) );
	const FIntPoint Size = FSlateApplication::Get().GetRenderer()->GenerateDynamicImageResource(BannerName);
	if (Size.X > 0 && Size.Y > 0)
	{
		BannerDynamicImageBrush = MakeShareable(new FSlateDynamicImageBrush(BannerName, FVector2D(Size.X, Size.Y)));
	}

	ReturnCode = 0;
	InitialQualityLevels = CachedQualityLevels = Scalability::GetQualityLevels();
	static float Padding = 1.0f;
	static float QualityColumnCoeff = 1.0f;
	static float QualityLevelColumnCoeff = 0.2f;

	SWindow::Construct(SWindow::FArguments()
		//.Title(InArgs._Title)
		.SupportsMinimize(false)
		.SupportsMaximize(false)
		.CreateTitleBar(false)
		.SizingRule( ESizingRule::FixedSize )
		.ClientSize(FVector2D(470, 410))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			[
				SNew(SImage).Image(BannerDynamicImageBrush.IsValid() ? BannerDynamicImageBrush.Get() : NULL)
			]
			+SVerticalBox::Slot()
			[
				SNew(SGridPanel)
				.FillColumn(0, QualityColumnCoeff)

				+ MakeGridSlot(0, 1, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 2, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 3, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 4, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 5, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 6, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]
				+ MakeGridSlot(0, 7, 5, 1)[SNew(SBorder).BorderImage(FCoreStyle::Get().GetBrush("ExpandableArea.Border"))]

				+MakeGridSlot(0,0).VAlign(VAlign_Center) [ SNew(STextBlock).Text(LOCTEXT("QualityLabel", "Quality")).Font(TitleFont) ]
				+MakeGridSlot(1,0) [ MakeHeaderButtonWidget(NamesLow, 0, LOCTEXT("QualityLow", "Set all groups to low quality")) ]
				+MakeGridSlot(2,0) [ MakeHeaderButtonWidget(NamesMedium, 1, LOCTEXT("QualityMedium", "Set all groups to medium quality")) ]
				+MakeGridSlot(3,0) [ MakeHeaderButtonWidget(NamesHigh, 2, LOCTEXT("QualityHigh", "Set all groups to high quality")) ]
				+MakeGridSlot(4,0) [ MakeHeaderButtonWidget(NamesEpic, 3, LOCTEXT("QualityEpic", "Set all groups to epic quality")) ]

				+ MakeGridSlot(0, 1)[SNew(STextBlock).Text(LOCTEXT("ResScaleLabel1", "Pixels Per DisplayPixel")).Font(GroupFont)]
				+ MakeGridSlot(5, 1)[SNew(STextBlock).Text(this, &SScalabilityUIPanel::GetResolutionScaleString)]
				+ MakeGridSlot(1, 1, 4, 1)[SNew(SSlider).OnValueChanged(this, &SScalabilityUIPanel::OnResolutionScaleChanged).Value(this, &SScalabilityUIPanel::GetResolutionScale)]

				+MakeGridSlot(0,2) [ SNew(STextBlock).Text(LOCTEXT("AntiAliasingQualityLabel1", "Anti-Aliasing")).Font(GroupFont) ]
				+MakeGridSlot(1,2) [ MakeButtonWidget(NamesLow, TEXT("AntiAliasingQuality"), 0, LOCTEXT("AntiAliasingQualityLow", "Set anti-aliasing quality to low")) ]
				+MakeGridSlot(2,2) [ MakeButtonWidget(NamesMedium, TEXT("AntiAliasingQuality"), 1, LOCTEXT("AntiAliasingQualityMed", "Set anti-aliasing quality to medium")) ]
				+MakeGridSlot(3,2) [ MakeButtonWidget(NamesHigh, TEXT("AntiAliasingQuality"), 2, LOCTEXT("AntiAliasingQualityHigh", "Set anti-aliasing quality to high")) ]
				+MakeGridSlot(4,2) [ MakeButtonWidget(NamesEpic, TEXT("AntiAliasingQuality"), 3, LOCTEXT("AntiAliasingQualityEpic", "Set anti-aliasing quality to epic")) ]

				+MakeGridSlot(0,3) [ SNew(STextBlock).Text(LOCTEXT("PostProcessQualityLabel1", "Post Processing")).Font(GroupFont) ]
				+MakeGridSlot(1,3) [ MakeButtonWidget(NamesLow, TEXT("PostProcessQuality"), 0, LOCTEXT("PostProcessQualityLow", "Set post processing quality to low")) ]
				+MakeGridSlot(2,3) [ MakeButtonWidget(NamesMedium, TEXT("PostProcessQuality"), 1, LOCTEXT("PostProcessQualityMed", "Set post processing quality to medium")) ]
				+MakeGridSlot(3,3) [ MakeButtonWidget(NamesHigh, TEXT("PostProcessQuality"), 2, LOCTEXT("PostProcessQualityHigh", "Set post processing quality to high")) ]
				+MakeGridSlot(4,3) [ MakeButtonWidget(NamesEpic, TEXT("PostProcessQuality"), 3, LOCTEXT("PostProcessQualityEpic", "Set post processing quality to epic")) ]

				+MakeGridSlot(0,4) [ SNew(STextBlock).Text(LOCTEXT("ShadowLabel1", "Shadows")).Font(GroupFont) ]
				+MakeGridSlot(1,4) [ MakeButtonWidget(NamesLow, TEXT("ShadowQuality"), 0, LOCTEXT("ShadowQualityLow", "Set shadow quality to low")) ]
				+MakeGridSlot(2,4) [ MakeButtonWidget(NamesMedium, TEXT("ShadowQuality"), 1, LOCTEXT("ShadowQualityMed", "Set shadow quality to medium")) ]
				+MakeGridSlot(3,4) [ MakeButtonWidget(NamesHigh, TEXT("ShadowQuality"), 2, LOCTEXT("ShadowQualityHigh", "Set shadow quality to high")) ]
				+MakeGridSlot(4,4) [ MakeButtonWidget(NamesEpic, TEXT("ShadowQuality"), 3, LOCTEXT("ShadowQualityEpic", "Set shadow quality to epic")) ]

				+MakeGridSlot(0,5) [ SNew(STextBlock).Text(LOCTEXT("TextureQualityLabel1", "Textures")).Font(GroupFont) ]
				+MakeGridSlot(1,5) [ MakeButtonWidget(NamesLow, TEXT("TextureQuality"), 0, LOCTEXT("TextureQualityLow", "Set texture quality to low")) ]
				+MakeGridSlot(2,5) [ MakeButtonWidget(NamesMedium, TEXT("TextureQuality"), 1, LOCTEXT("TextureQualityMed", "Set texture quality to medium")) ]
				+MakeGridSlot(3,5) [ MakeButtonWidget(NamesHigh, TEXT("TextureQuality"), 2, LOCTEXT("TextureQualityHigh", "Set texture quality to high")) ]
				+MakeGridSlot(4,5) [ MakeButtonWidget(NamesEpic, TEXT("TextureQuality"), 3, LOCTEXT("TextureQualityEpic", "Set texture quality to epic")) ]

				+MakeGridSlot(0,6) [ SNew(STextBlock).Text(LOCTEXT("EffectsQualityLabel1", "Effects")).Font(GroupFont) ]
				+MakeGridSlot(1,6) [ MakeButtonWidget(NamesLow, TEXT("EffectsQuality"), 0, LOCTEXT("EffectsQualityLow", "Set effects quality to low")) ]
				+MakeGridSlot(2,6) [ MakeButtonWidget(NamesMedium, TEXT("EffectsQuality"), 1, LOCTEXT("EffectsQualityMed", "Set effects quality to medium")) ]
				+MakeGridSlot(3,6) [ MakeButtonWidget(NamesHigh, TEXT("EffectsQuality"), 2, LOCTEXT("EffectsQualityHigh", "Set effects quality to high")) ]
				+MakeGridSlot(4,6) [ MakeButtonWidget(NamesEpic, TEXT("EffectsQuality"), 3, LOCTEXT("EffectsQualityEpic", "Set effects quality to epic")) ]

				+MakeGridSlot(0,7)[SNew(STextBlock).Text(LOCTEXT("ViewDistanceLabel1", "View Distance")).Font(GroupFont)]
				+MakeGridSlot(1,7)[MakeButtonWidget(LOCTEXT("ViewDistanceLabel2", "Near"), TEXT("ViewDistanceQuality"), 0, LOCTEXT("ViewDistanceQualityNear", "Set view distance to near"))]
				+MakeGridSlot(2,7)[MakeButtonWidget(LOCTEXT("ViewDistanceLabel3", "Medium"), TEXT("ViewDistanceQuality"), 1, LOCTEXT("ViewDistanceQualityMed", "Set view distance to medium"))]
				+MakeGridSlot(3,7)[MakeButtonWidget(LOCTEXT("ViewDistanceLabel4", "Far"), TEXT("ViewDistanceQuality"), 2, LOCTEXT("ViewDistanceQualityFar", "Set view distance to far"))]
				+MakeGridSlot(4,7)[MakeButtonWidget(LOCTEXT("ViewDistanceLabel5", "Epic"), TEXT("ViewDistanceQuality"), 3, LOCTEXT("ViewDistanceQualityEpic", "Set view distance to epic"))]

				+ MakeGridSlot(3, 9)[MakeLaunchButtonWidget(LOCTEXT("PlayLabel", "Play!"), 1, LOCTEXT("Play", ""))]
				+ MakeGridSlot(4, 9)[MakeLaunchButtonWidget(LOCTEXT("QuitLabel", "Quit"), 0, LOCTEXT("Quit", ""))]
			]
		]);
}

#undef LOCTEXT_NAMESPACE
