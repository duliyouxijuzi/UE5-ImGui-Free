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

#include <utility>


namespace Utilities
{
	//====================================================================================================
	// Range
	//====================================================================================================

	template<typename T>
	class TRange
	{
	public:

		TRange() {}

		TRange(const T& RangeBegin, const T& RangeEnd) { SetRange(RangeBegin, RangeEnd); }

		const T& GetBegin() const { return Begin; }
		const T& GetEnd() const { return End; }

		bool IsEmpty() const { return Begin == End; }

		void SetEmpty() { Begin = End = T(); }

		void SetRange(const T& RangeBegin, const T& RangeEnd)
		{
			checkf(RangeBegin <= RangeEnd, TEXT("Invalid arguments: RangeBegin > RangeEnd."));
			Begin = RangeBegin;
			End = RangeEnd;
		}

		void AddPosition(const T& Position)
		{
			AddRangeUnchecked(Position, Position + 1);
		}

		void AddRange(const T& RangeBegin, const T& RangeEnd)
		{
			checkf(RangeBegin <= RangeEnd, TEXT("Invalid arguments: RangeBegin > RangeEnd."));
			AddRangeUnchecked(RangeBegin, RangeEnd);
		}

	private:

		void AddRangeUnchecked(const T& RangeBegin, const T& RangeEnd)
		{
			if (IsEmpty())
			{
				Begin = RangeBegin;
				End = RangeEnd;
			}
			else
			{
				if (Begin > RangeBegin)
				{
					Begin = RangeBegin;
				}

				if (End < RangeEnd)
				{
					End = RangeEnd;
				}
			}
		}

		T Begin = T();
		T End = T();
	};


	// Enable range-based loops

	template<typename T>
	const T& begin(const TRange<T>& Range)
	{
		return Range.GetBegin();
	}

	template<typename T>
	const T& end(const TRange<T>& Range)
	{
		return Range.GetEnd();
	}


	//====================================================================================================
	// Bounded Range
	//====================================================================================================

	template<typename T, T BeginBound, T EndBound>
	class TBoundedRange
	{
	public:

		constexpr const T& GetLowerBound() const { return BeginBound; }
		constexpr const T& GetUpperBound() const { return EndBound; }

		const T& GetBegin() const { return Begin; }
		const T& GetEnd() const { return End; }

		bool IsEmpty() const { return Begin == End; }

		void SetEmpty() { Begin = End = BeginBound; }

		void SetFull()
		{
			Begin = BeginBound;
			End = EndBound;
		}

		void AddPosition(const T& Position)
		{
			checkf(Position >= BeginBound && Position < EndBound, TEXT("Position out of range."));

			AddRangeUnchecked(Position, Position + 1);
		}

		void AddRange(const T& RangeBegin, const T& RangeEnd)
		{
			checkf(RangeBegin <= RangeEnd, TEXT("Invalid arguments: RangeBegin > MaxPosition."));
			checkf(RangeBegin >= BeginBound, TEXT("RangeBegin out of range."));
			checkf(RangeBegin <= EndBound, TEXT("RangeEnd out of range."));

			AddRangeUnchecked(RangeBegin, RangeEnd);
		}

	private:

		void AddRangeUnchecked(const T& RangeBegin, const T& RangeEnd)
		{
			if (IsEmpty())
			{
				Begin = RangeBegin;
				End = RangeEnd;
			}
			else
			{
				if (Begin > RangeBegin)
				{
					Begin = RangeBegin;
				}

				if (End < RangeEnd)
				{
					End = RangeEnd;
				}
			}
		}

		T Begin = EndBound;
		T End = BeginBound;
	};


	// Enable range-based loops

	template<typename T, T BeginBound, T EndBound>
	const T& begin(const TBoundedRange<T, BeginBound, EndBound>& Range)
	{
		return Range.GetBegin();
	}

	template<typename T, T BeginBound, T EndBound>
	const T& end(const TBoundedRange<T, BeginBound, EndBound>& Range)
	{
		return Range.GetEnd();
	}
}
