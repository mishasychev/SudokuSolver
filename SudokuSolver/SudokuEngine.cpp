#include "SudokuEngine.h"

SudokuEngine::~SudokuEngine()
{
}

void SudokuEngine::OnInitialize()
{
	SetWindowTitle(TEXT("Sudoku solver"));

	field_ = std::make_unique<SudokuField>(this, Vector2D(10.0f), Vector2D(810.0f));
}

void SudokuEngine::OnDraw(ID2D1HwndRenderTarget* renderTarget)
{
	field_->Draw(renderTarget);
}

void SudokuEngine::OnButtonReleased(Button button)
{
	field_->OnButtonReleased(button);
}