#include "SudokuCell.h"

#include <d2d1.h>
#include <Engine.h>
#include <string>

SudokuCell::SudokuCell(Engine* engine, const Vector2D position, const Vector2D dimensions)
	: Object(engine, position, dimensions)
{
	SetShouldTick(false);
}

SudokuCell::~SudokuCell()
{
}

void SudokuCell::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	const auto position = GetPosition();
	const auto dimensions = GetDimensions();

	const D2D1_RECT_F rect = {
		.left = position.x,
		.top = position.y,
		.right = position.x + dimensions.x,
		.bottom = position.y + dimensions.y
	};

	renderTarget->DrawRectangle(rect, borderBrush_, 3.0f);

	if (value_ != 0)
	{
		const auto s = std::to_wstring(value_);

		const D2D1_RECT_F textRect = {
		.left = position.x + dimensions.x * 0.3f,
		.top = position.y,
		.right = position.x + dimensions.x,
		.bottom = position.y + dimensions.y
		};

		renderTarget->DrawText(s.data(),
			static_cast<UINT32>(s.size()),
			textFormat_,
			textRect,
			textBrush_,
			D2D1_DRAW_TEXT_OPTIONS_NONE,
			DWRITE_MEASURING_MODE_NATURAL);
	}
}
