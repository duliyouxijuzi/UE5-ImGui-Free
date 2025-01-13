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

#pragma once

#include <UObject/WeakObjectPtr.h>
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Widgets/SCompoundWidget.h>


class FImGuiModuleManager;
class UGameViewportClient;
struct FImGuiDPIScaleInfo;

// Layout preset for ImGui Widget.
class SImGuiLayout : public SCompoundWidget
{
	typedef SCompoundWidget Super;

public:

	SLATE_BEGIN_ARGS(SImGuiLayout)
	{}
	SLATE_ARGUMENT(FImGuiModuleManager*, ModuleManager)
	SLATE_ARGUMENT(UGameViewportClient*, GameViewport)
	SLATE_ARGUMENT(int32, ContextIndex)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	~SImGuiLayout();

	const TWeakObjectPtr<UGameViewportClient>& GetGameViewport() const { return GameViewport; }

private:

	float GetDPIScale() const { return DPIScale; }
	void SetDPIScale(const FImGuiDPIScaleInfo& ScaleInfo);

	FImGuiModuleManager* ModuleManager = nullptr;
	TWeakObjectPtr<UGameViewportClient> GameViewport;

	float DPIScale = 1.f;
};
