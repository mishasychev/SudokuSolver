#pragma once

#include "Object.h"

#include <array>
#include <memory>

#include <Input.h>

#include <thread>
#include <condition_variable>
#include <mutex>

#include "SudokuSolver.h"
#include "SudokuFieldState.h"

struct ID2D1SolidColorBrush;
struct IDWriteTextFormat;
class SudokuCell;

class SudokuField : public Object
{
public:
	SudokuField(Engine* engine, const Vector2D position, const Vector2D dimensions);
	virtual ~SudokuField();

	void Draw(ID2D1HwndRenderTarget* renderTarget) override;

	void OnButtonReleased(Button button) const;

	void CleanField() const;

	SudokuCell* SelectCell(const size_t row, const size_t column);
	void DeselectCell();

	std::pair<size_t, size_t> TransformMousePositionToField() const;

	void StartSolving();

	void SetBorderBrushes(ID2D1SolidColorBrush* brush) const;

private:
	bool IsMouseWithinField_(const Vector2D mouse) const;

public:
	//Resources
	ID2D1SolidColorBrush* whiteBrush{ nullptr };
	ID2D1SolidColorBrush* blueBrush{ nullptr };
	ID2D1SolidColorBrush* yellowBrush{ nullptr };
	ID2D1SolidColorBrush* redBrush{ nullptr };
	ID2D1SolidColorBrush* greenBrush{ nullptr };

	IDWriteTextFormat* textFormat{ nullptr };

private:
	//Cells
	std::array<std::array<std::unique_ptr<SudokuCell>, 9>, 9> cells_;

	SudokuCell* selectedCell_{ nullptr };

	//Solving
	std::jthread solver_;
	std::condition_variable_any cv_;
	std::mutex mtx_;

	bool bIsSolvingRun_{ false };

	void SolvingEnded_(const bool result, const Array9_9& field);

	void SolvingThread_(const std::stop_token& stopToken);

	//State
	std::unique_ptr<State> state_{ new None_State(this) };

public:
	void SetState(State* newState);

	__forceinline SudokuCell* GetSelectedCell() const { return selectedCell_; }

	Array9_9 GetField() const;
};

