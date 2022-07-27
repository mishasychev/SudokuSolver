#pragma once

#include <Engine.h>

#include <memory>

#include "SudokuField.h"

class SudokuEngine : public Engine
{
public:
	virtual ~SudokuEngine();

	void OnInitialize() override;

	void OnDraw(ID2D1HwndRenderTarget* renderTarget) override;

	void OnButtonReleased(Button button) override;

private:
	std::unique_ptr<SudokuField> field_{ nullptr };
};

