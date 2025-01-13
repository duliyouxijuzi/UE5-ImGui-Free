// Copyright (c) 2017-2021 Sebastian Gross. All Rights Reserved.
// Published by LeoGame in 2025.
// This project is distributed under the MIT License (MIT).

// MIT License

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "SImGuiLayout.h"
#include "SImGuiWidget.h"

#include "ImGuiModuleManager.h"
#include "ImGuiModuleSettings.h"

#include <SlateOptMacros.h>
#include <Widgets/Layout/SConstraintCanvas.h>
#include <Widgets/Layout/SDPIScaler.h>
#include <Widgets/Layout/SScaleBox.h>


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SImGuiLayout::Construct(const FArguments& InArgs)
{
	checkf(InArgs._GameViewport, TEXT("Null Game Viewport argument"));

	ModuleManager = InArgs._ModuleManager;
	GameViewport = InArgs._GameViewport;

	if (ModuleManager)
	{
		auto& Settings = ModuleManager->GetSettings();
		SetDPIScale(Settings.GetDPIScaleInfo());
		if (!Settings.OnDPIScaleChangedDelegate.IsBoundToObject(this))
		{
			Settings.OnDPIScaleChangedDelegate.AddRaw(this, &SImGuiLayout::SetDPIScale);
		}
	}

	ChildSlot
	[
		// Remove accumulated scale to manually control how we draw data. 
		SNew(SScaleBox)
		.IgnoreInheritedScale(true)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Visibility(EVisibility::SelfHitTestInvisible)
		[
			// Apply custom scale if needed.
			SNew(SDPIScaler)
			.DPIScale(TAttribute<float>(this, &SImGuiLayout::GetDPIScale))
			.Visibility(EVisibility::SelfHitTestInvisible)
			[
				SNew(SConstraintCanvas)
				+ SConstraintCanvas::Slot()
				.Anchors(FAnchors(0.f, 0.f, 1.f, 1.f))
				.AutoSize(true)
				.Offset(FMargin(0.f, 0.f, 0.f, 0.f))
				.Alignment(FVector2D::ZeroVector)
				[
					SNew(SImGuiWidget)
					.ModuleManager(InArgs._ModuleManager)
					.GameViewport(InArgs._GameViewport)
					.ContextIndex(InArgs._ContextIndex)
#if !ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API
					// To correctly clip borders. Using SScissorRectBox in older versions seems to be not necessary.
					.Clipping(EWidgetClipping::ClipToBounds)
#endif
				]
			]
		]
	];

	SetVisibility(EVisibility::SelfHitTestInvisible);
}

SImGuiLayout::~SImGuiLayout()
{
	if (ModuleManager)
	{
		ModuleManager->GetSettings().OnDPIScaleChangedDelegate.RemoveAll(this);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SImGuiLayout::SetDPIScale(const FImGuiDPIScaleInfo& ScaleInfo)
{
	DPIScale = ScaleInfo.GetSlateScale();
}
