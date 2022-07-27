#include "SudokuFieldState.h"

#include <Engine.h>

#include "SudokuCell.h"
#include "SudokuField.h"

void None_State::Initialize()
{
	owner_->DeselectCell();

	owner_->SetBorderBrushes(owner_->blueBrush);
}

void None_State::OnButtonReleased(Button button)
{
	switch (button)
	{
	case Button::MOUSE_LEFT: { owner_->SetState(new Modifying_State(owner_)); return; }
	case Button::DEL: { owner_->CleanField(); return; }
	case Button::ENTER: { owner_->SetState(new Solving_State(owner_)); return; }
	}
}

void Modifying_State::Initialize()
{
	owner_->SetBorderBrushes(owner_->blueBrush);

	owner_->DeselectCell();
	const auto [row, column] = owner_->TransformMousePositionToField();
	SudokuCell* selectedCell = owner_->SelectCell(row, column);

	if (selectedCell != nullptr)
		selectedCell->SetBorderBrush(owner_->yellowBrush);
	else
		owner_->SetState(new None_State(owner_));
}

void Modifying_State::OnButtonReleased(Button button)
{
	switch (button)
	{
	case Button::MOUSE_LEFT: { Initialize(); return; }
	case Button::DEL: { owner_->CleanField(); return; }
	case Button::ENTER: { owner_->SetState(new Solving_State(owner_)); return; }

	case Button::NUM_0:
	case Button::NUM_1:
	case Button::NUM_2:
	case Button::NUM_3:
	case Button::NUM_4:
	case Button::NUM_5:
	case Button::NUM_6:
	case Button::NUM_7:
	case Button::NUM_8:
	case Button::NUM_9:
	{
		owner_->GetSelectedCell()->SetValue(static_cast<uint8_t>(button) - 48);
		return;
	}
	case Button::NUMPAD_0:
	case Button::NUMPAD_1:
	case Button::NUMPAD_2:
	case Button::NUMPAD_3:
	case Button::NUMPAD_4:
	case Button::NUMPAD_5:
	case Button::NUMPAD_6:
	case Button::NUMPAD_7:
	case Button::NUMPAD_8:
	case Button::NUMPAD_9:
	{
		owner_->GetSelectedCell()->SetValue(static_cast<uint8_t>(button) - 96);
		return;
	}
	}
}

void Solving_State::Initialize()
{
	owner_->DeselectCell();
	owner_->SetBorderBrushes(owner_->blueBrush);

	owner_->StartSolving();
}

void Solving_State::OnButtonReleased(Button button)
{
}

void Result_State::Initialize()
{
	owner_->SetBorderBrushes(solvingResult_ ? owner_->greenBrush : owner_->redBrush);
}

void Result_State::OnButtonReleased(Button button)
{
	switch (button)
	{
	case Button::MOUSE_LEFT: { owner_->SetState(new Modifying_State(owner_)); return; }
	case Button::DEL: { owner_->CleanField(); owner_->SetState(new None_State(owner_)); return; }
	}
}
