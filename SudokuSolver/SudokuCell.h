#pragma once

#include "Object.h"

struct ID2D1SolidColorBrush;
struct IDWriteTextFormat;
struct IDWriteTextLayout;

class SudokuCell : public Object
{
public:
	SudokuCell(Engine* engine, const Vector2D position, const Vector2D dimensions);
	virtual ~SudokuCell();

	void Draw(ID2D1HwndRenderTarget* renderTarget) override;

private:
	uint8_t value_{ 0 };

	ID2D1SolidColorBrush* borderBrush_{ nullptr };
	ID2D1SolidColorBrush* textBrush_{ nullptr };

	IDWriteTextFormat* textFormat_{ nullptr };

public:
	__forceinline uint8_t GetValue() const { return value_; }
	__forceinline void SetValue(const uint8_t newValue) { value_ = newValue; }

	__forceinline void SetBorderBrush(ID2D1SolidColorBrush* newBrush) { borderBrush_ = newBrush; }
	__forceinline void SetTextBrush(ID2D1SolidColorBrush* newBrush) { textBrush_ = newBrush; }

	__forceinline void SetTextFormat(IDWriteTextFormat* newFormat) { textFormat_ = newFormat; }
};

