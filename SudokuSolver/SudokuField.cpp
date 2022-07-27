#include "SudokuField.h"

#include <d2d1.h>
#include <Engine.h>

#include <cstdint>
#include <ranges>

#include "SudokuCell.h"

SudokuField::SudokuField(Engine* engine, const Vector2D position, const Vector2D dimensions)
	: Object(engine, position, dimensions),
	solver_(std::bind_front(&SudokuField::SolvingThread_, this))
{
	SetShouldTick(false);

	//Create brushes
	auto* renderTarget = engine->GetD2D1RenderTarget();
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &whiteBrush);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(72.0f / 255.0f, 100.0f / 255.0f, 243.0f / 255.0f), &blueBrush);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &yellowBrush);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &redBrush);
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &greenBrush);

	//Create writeFormat for cells
	auto* writeFactory = GetEngine()->GetDWriteFactory();
	writeFactory->CreateTextFormat(
		TEXT("Segoe UI"),
		nullptr,
		DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		dimensions.x / 15.0f,
		TEXT("en-us"),
		&textFormat);

	//Initialize cells
	const float delta = dimensions.x / 9.0f;
	for (const size_t row : std::views::iota(0ULL, 9ULL))
	{
		for (const size_t column : std::views::iota(0ULL, 9ULL))
		{
			auto& cell = cells_[row][column];

			const float x = delta * static_cast<float>(column);
			const float y = delta * static_cast<float>(row);

			const Vector2D cellPosition{ position + Vector2D(x, y) };

			cell.reset(new SudokuCell(engine, cellPosition, Vector2D(delta - 6.0f)));

			cell->SetTextFormat(textFormat);
			cell->SetBorderBrush(blueBrush);
			cell->SetTextBrush(whiteBrush);
		}
	}
}

SudokuField::~SudokuField()
{
	solver_.request_stop();
	solver_.join();

	safeRelease(whiteBrush);
	safeRelease(blueBrush);
	safeRelease(yellowBrush);
	safeRelease(redBrush);
	safeRelease(greenBrush);
}

void SudokuField::Draw(ID2D1HwndRenderTarget* renderTarget)
{
	for (const auto& row : cells_)
		for (const auto& cell : row)
			cell->Draw(renderTarget);
}

void SudokuField::OnButtonReleased(Button button) const
{
	state_->OnButtonReleased(button);
}

void SudokuField::CleanField() const
{
	for (const auto& row : cells_)
		for (const auto& cell : row)
			cell->SetValue(0);
}

SudokuCell* SudokuField::SelectCell(const size_t row, const size_t column)
{
	if (row > 8 || column > 8)
		return nullptr;

	selectedCell_ = cells_[row][column].get();
	return selectedCell_;
}

void SudokuField::DeselectCell()
{
	if (selectedCell_ != nullptr)
		selectedCell_->SetBorderBrush(blueBrush);

	selectedCell_ = nullptr;
}

std::pair<size_t, size_t> SudokuField::TransformMousePositionToField() const
{
	const Vector2D mousePosition = GetEngine()->GetMousePosition();
	if (!IsMouseWithinField_(mousePosition))
		return { 9, 9 };

	const Vector2D mouseToField{ mousePosition - GetPosition() };
	const float delta{ GetDimensions().x / 9.0f };

	return { static_cast<size_t>(mouseToField.y / delta), static_cast<size_t>(mouseToField.x / delta) };
}

void SudokuField::SetState(State* newState)
{
	state_.reset(newState);
	state_->Initialize();
}

void SudokuField::StartSolving()
{
	if (bIsSolvingRun_)
		return;

	std::unique_lock<std::mutex> lk(mtx_);
	bIsSolvingRun_ = true;
	cv_.notify_one();
}

void SudokuField::SetBorderBrushes(ID2D1SolidColorBrush* brush) const
{
	for (const auto& row : cells_)
		for (const auto& cell : row)
			cell->SetBorderBrush(brush);
}

void SudokuField::SolvingEnded_(const bool result, const Array9_9& field)
{
	SetState(new Result_State(this, result));

	if (!result)
		return;

	for (const size_t row : std::views::iota(0ULL, 9ULL))
		for (const size_t column : std::views::iota(0ULL, 9ULL))
			cells_[row][column]->SetValue(field[row][column]);
}

void SudokuField::SolvingThread_(const std::stop_token& stopToken)
{
	while (!stopToken.stop_requested())
	{
		std::unique_lock lk(mtx_);
		cv_.wait(lk, stopToken, [this] { return bIsSolvingRun_; });

		SudokuSolver sudokuSolver(GetField());
		const bool solvingResult = sudokuSolver.Solve();

		bIsSolvingRun_ = false;
		SolvingEnded_(solvingResult, sudokuSolver.GetField());
	}
}

bool SudokuField::IsMouseWithinField_(const Vector2D mouse) const
{
	const Vector2D topLeft{ GetPosition() };
	const Vector2D bottomRight{ topLeft + GetDimensions() };

	return topLeft.x <= mouse.x && mouse.x <= bottomRight.x &&
		topLeft.y <= mouse.y && mouse.y <= bottomRight.y;
}

Array9_9 SudokuField::GetField() const
{
	Array9_9 result;

	for (const size_t row : std::views::iota(0ULL, 9ULL))
		for (const size_t column : std::views::iota(0ULL, 9ULL))
			result[row][column] = cells_[row][column]->GetValue();

	return result;
}
