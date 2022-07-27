#include "SudokuSolver.h"

#include <ranges>
#include <algorithm>

SudokuSolver::SudokuSolver(const Array9_9& field_)
	: field_(field_)
{
}

bool SudokuSolver::Solve()
{
	if (!CheckField_())
		return false;

	return Solve_Impl_(0, 0);
}

bool SudokuSolver::Solve_Impl_(size_t row, size_t column)
{
	if (row == 8 && column == 9)
		return true;

	if (column == 9)
	{
		row++;
		column = 0;
	}

	if (field_[row][column] != 0)
		return Solve_Impl_(row, column + 1);

	for (uint8_t testValue = 1; testValue <= 9; testValue++)
	{
		if (CheckValue_(testValue, row, column))
		{
			field_[row][column] = testValue;

			if (Solve_Impl_(row, column + 1))
				return true;
		}

		field_[row][column] = 0;
	}

	return false;
}

bool SudokuSolver::CheckField_()
{
	for (const size_t row : std::views::iota(0ULL, 9ULL))
		for (const size_t column : std::views::iota(0ULL, 9ULL))
		{
			const uint8_t currentValue = field_[row][column];
			if (currentValue == 0)
				continue;

			field_[row][column] = 0;
			if (!CheckValue_(currentValue, row, column))
				return false;

			field_[row][column] = currentValue;
		}

	return true;
}

bool SudokuSolver::CheckRow_(const uint8_t value, const size_t row, const size_t column) const
{
	return std::ranges::all_of(field_[row], [=](const uint8_t cell)
		{
			return cell != value;
		});
}

bool SudokuSolver::CheckColumn_(const uint8_t value, const size_t row, const size_t column) const
{
	return std::ranges::all_of(field_, [=](const std::array<uint8_t, 9>& rowCells)
		{
			return rowCells[column] != value;
		});
}

bool SudokuSolver::CheckBlock_(const uint8_t value, const size_t row, const size_t column) const
{
	const size_t startRow = (row / 3) * 3;
	const size_t startColumn = (column / 3) * 3;

	for (const size_t i : std::views::iota(startRow, startRow + 3))
		for (const size_t j : std::views::iota(startColumn, startColumn + 3))
			if (field_[i][j] == value)
				return false;

	return true;
}

bool SudokuSolver::CheckValue_(const uint8_t value, const size_t row, const size_t column) const
{
	return CheckRow_(value, row, column) &&
		CheckColumn_(value, row, column) &&
		CheckBlock_(value, row, column);
}
