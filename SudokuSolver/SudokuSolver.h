#pragma once

#include <array>

using Array9_9 = std::array<std::array<uint8_t, 9>, 9>;

class SudokuSolver
{
public:
	SudokuSolver(const Array9_9& field);

	bool Solve();

private:
	bool Solve_Impl_(size_t row, size_t column);

	bool CheckField_();

	bool CheckRow_(const uint8_t value, const size_t row, const size_t column) const;
	bool CheckColumn_(const uint8_t value, const size_t row, const size_t column) const;
	bool CheckBlock_(const uint8_t value, const size_t row, const size_t column) const;
	bool CheckValue_(const uint8_t value, const size_t row, const size_t column) const;

private:
	Array9_9 field_;

public:
	Array9_9 GetField() const { return field_; }
};

