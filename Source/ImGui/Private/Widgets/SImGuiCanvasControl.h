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

#include "ImGuiInputState.h"
#include "ImGuiModuleSettings.h"

#include <Brushes/SlateBorderBrush.h>
#include <Widgets/DeclarativeSyntaxSupport.h>
#include <Widgets/SLeafWidget.h>


// Widget that controls transform of ImGui canvas/space.
// When active, additionally it shows boundaries of ImGui canvas and default visible area.
// TODO: Bind to ImGui context or properties to dynamically read canvas size.
// TODO: Bind to properties to allow configure colors.
class SImGuiCanvasControl : public SLeafWidget
{
	typedef SLeafWidget Super;

public:

	DECLARE_DELEGATE_OneParam(FOnTransformChanged, const FSlateRenderTransform&);

	SLATE_BEGIN_ARGS(SImGuiCanvasControl)
	{}
	SLATE_EVENT(FOnTransformChanged, OnTransformChanged)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	bool IsActive() const { return bActive; }
	void SetActive(bool bInActive);

	const FSlateRenderTransform& GetTransform() const { return Transform; }

	//----------------------------------------------------------------------------------------------------
	// SWidget overrides
	//----------------------------------------------------------------------------------------------------

	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual FReply OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FReply OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent) override;

	virtual FVector2D ComputeDesiredSize(float InScale) const override;

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

private:

	enum class EDragRequest : uint8
	{
		None,
		Content,
		Canvas
	};

	void UpdateVisibility();

	void Zoom(const FGeometry& MyGeometry, const float Delta, const FVector2D& MousePosition);

	void UpdateRenderTransform();

	float GetMinScale(const FGeometry& MyGeometry);

	mutable FSlateBorderBrush CanvasBorderBrush = FSlateBorderBrush("SImGuiCanvasControl-CanvasBorder", FMargin(0.f, 0.f, 1.f, 1.f), FLinearColor::White);
	mutable FSlateBorderBrush FrameBorderBrush = FSlateBorderBrush("SImGuiCanvasControl-FrameBorder", FMargin(0.f, 0.f, 1.f, 1.f), FLinearColor::White);

	FOnTransformChanged OnTransformChanged;

	// Transform from ImGui space.
	FSlateRenderTransform Transform;

	// Offset of the ImGui content in ImGui space.
	FVector2D ContentOffset = FVector2D::ZeroVector;

	// Offset of the ImGui canvas in widget local space.
	FVector2D CanvasOffset = FVector2D::ZeroVector;

	// Scale of the ImGui canvas in widget local space.
	float CanvasScale = 1.f;

	// Opacity scaling visibility of elements during blending.
	float Opacity = 1.f;

	// Whether this widget is active. While active, widget allows to modify transform of ImGui canvas, shows its
	// boundaries and default visible area.
	bool bActive = false;

	// Whether we are blending out after widget was deactivated. While blending out, widget is visible but it doesn't
	// process inputs anymore.
	bool bBlendingOut = false;

	// Request is set on mouse button press before drag operation is started. It remains valid until activating button
	// is released or until drag operation is finished or until it is replaced by alternative request.
	// Highlights are bound to requests, what means that they can also be activated before drag operation is started.
	EDragRequest DragRequest = EDragRequest::None;
};
