#pragma once

#include <Input.h>

class SudokuField;

class State
{
public:
	State(SudokuField* sudokuField) : owner_(sudokuField) {}

	virtual void Initialize() = 0;

	virtual void OnButtonReleased(Button button) = 0;

protected:
	SudokuField* owner_;
};

class None_State : public State
{
public:
	None_State(SudokuField* sudokuField) : State(sudokuField) {}

	void Initialize() override;

	void OnButtonReleased(Button button) override;
};

class Modifying_State : public State
{
public:
	Modifying_State(SudokuField* sudokuField) : State(sudokuField) {}

	void Initialize() override;

	void OnButtonReleased(Button button) override;
};

class Solving_State : public State
{
public:
	Solving_State(SudokuField* sudokuField) : State(sudokuField) {}

	void Initialize() override;

	void OnButtonReleased(Button button) override;
};

class Result_State : public State
{
public:
	Result_State(SudokuField* sudokuField, const bool solvingResult)
	: State(sudokuField),
	solvingResult_(solvingResult)
	{}

	void Initialize() override;

	void OnButtonReleased(Button button) override;

private:
	bool solvingResult_{ false };
};