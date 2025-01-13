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

#include "ImGuiKeyInfoCustomization.h"

#if WITH_EDITOR

#include "ImGuiModuleSettings.h"

#include <PropertyCustomizationHelpers.h>
#include <SKeySelector.h>
#include <Widgets/SCompoundWidget.h>
#include <Widgets/Input/SCheckBox.h>


#define LOCTEXT_NAMESPACE "ImGuiEditor"


//----------------------------------------------------------------------------------------------------
// Helper widgets
//----------------------------------------------------------------------------------------------------

namespace
{
	class SPropertyKeySelector : public SCompoundWidget
	{
	public:
		SLATE_BEGIN_ARGS(SPropertyKeySelector)
		{}
		SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, KeyHandle)
		SLATE_ATTRIBUTE(FSlateFontInfo, Font)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			KeyHandle = InArgs._KeyHandle;

			ChildSlot
			[
				SNew(SKeySelector)
				.CurrentKey(this, &SPropertyKeySelector::GetCurrentKey)
				.OnKeyChanged(this, &SPropertyKeySelector::OnKeyChanged)
				.Font(InArgs._Font)
			];
		}

		TOptional<FKey> GetCurrentKey() const
		{
			TArray<void*> RawPtrs;
			KeyHandle->AccessRawData(RawPtrs);

			if (RawPtrs.Num())
			{
				FKey* KeyPtr = static_cast<FKey*>(RawPtrs[0]);

				if (KeyPtr)
				{
					for (int32 Index = 1; Index < RawPtrs.Num(); Index++)
					{
						if (*static_cast<FKey*>(RawPtrs[Index]) != *KeyPtr)
						{
							return TOptional<FKey>();
						}
					}

					return *KeyPtr;
				}
			}

			return FKey();
		}

		void OnKeyChanged(TSharedPtr<FKey> SelectedKey)
		{
			KeyHandle->SetValueFromFormattedString(SelectedKey->ToString());
		}

	private:

		TSharedPtr<IPropertyHandle> KeyHandle;
	};

	class SPropertyTriStateCheckBox : public SCompoundWidget
	{
	public:

		using FCheckBoxStateRaw = std::underlying_type<ECheckBoxState>::type;

		SLATE_BEGIN_ARGS(SPropertyTriStateCheckBox)
		{}
		SLATE_ARGUMENT(TSharedPtr<IPropertyHandle>, PropertyHandle)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs)
		{
			PropertyHandle = InArgs._PropertyHandle;

			// We are abusing SCheckBox a bit but our GetPropertyValue with custom OnCheckBoxStateChanged implementation
			// gives a checkbox that allows to cycle between all three states.
			ChildSlot
			[
				SNew(SCheckBox)
				.IsChecked(this, &SPropertyTriStateCheckBox::GetPropertyValue)
				.OnCheckStateChanged(this, &SPropertyTriStateCheckBox::OnCheckBoxStateChanged)
			];
		}

		FCheckBoxStateRaw GetPropertyRawValue() const
		{
			FCheckBoxStateRaw Value;
			PropertyHandle.Get()->GetValue(Value);
			return Value;
		}

		ECheckBoxState GetPropertyValue() const
		{
			return static_cast<ECheckBoxState>(GetPropertyRawValue());
		}

		void OnCheckBoxStateChanged(ECheckBoxState State)
		{
			const FCheckBoxStateRaw PrevEnumValue = (GetPropertyRawValue() + 2) % 3;
			PropertyHandle.Get()->SetValue(PrevEnumValue);
		}

	private:

		TSharedPtr<IPropertyHandle> PropertyHandle;
	};
}


//----------------------------------------------------------------------------------------------------
// FImGuiKeyInfoCustomization implementation
//----------------------------------------------------------------------------------------------------

namespace InputConstants
{
	static const FMargin PropertyPadding(0.0f, 0.0f, 4.0f, 0.0f);
}

TSharedRef<IPropertyTypeCustomization> FImGuiKeyInfoCustomization::MakeInstance()
{
	return MakeShareable(new FImGuiKeyInfoCustomization);
}

void FImGuiKeyInfoCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InStructPropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	TSharedPtr<IPropertyHandle> KeyHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FImGuiKeyInfo, Key));
	TSharedPtr<IPropertyHandle> ShiftHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FImGuiKeyInfo, Shift));
	TSharedPtr<IPropertyHandle> CtrlHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FImGuiKeyInfo, Ctrl));
	TSharedPtr<IPropertyHandle> AltHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FImGuiKeyInfo, Alt));
	TSharedPtr<IPropertyHandle> CmdHandle = InStructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FImGuiKeyInfo, Cmd));

#define ADD_CHECK_BOX_SLOT(Handle) \
	+ SHorizontalBox::Slot()\
	.Padding(InputConstants::PropertyPadding)\
	.HAlign(HAlign_Left)\
	.VAlign(VAlign_Center)\
	.AutoWidth()\
	[\
		Handle->CreatePropertyNameWidget()\
	]\
	+ SHorizontalBox::Slot()\
	.Padding(InputConstants::PropertyPadding)\
	.HAlign(HAlign_Left)\
	.VAlign(VAlign_Center)\
	.AutoWidth()\
	[\
		SNew(SPropertyTriStateCheckBox).PropertyHandle(Handle)\
	]

	HeaderRow
	.NameContent()
	[
		InStructPropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MaxDesiredWidth(400.f)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(InputConstants::PropertyPadding)
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(200.f)
			[
				SNew(SPropertyKeySelector)
				.KeyHandle(KeyHandle)
				.Font(StructCustomizationUtils.GetRegularFont())
			]
		]
		ADD_CHECK_BOX_SLOT(ShiftHandle)
		ADD_CHECK_BOX_SLOT(CtrlHandle)
		ADD_CHECK_BOX_SLOT(AltHandle)
		ADD_CHECK_BOX_SLOT(CmdHandle)
	];

#undef ADD_CHECK_BOX_SLOT
}

void FImGuiKeyInfoCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InStructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
}


#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR
