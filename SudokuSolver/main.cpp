#define NOMINMAX
#include <Windows.h>

#include "SudokuEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HeapSetInformation(nullptr,
        HeapEnableTerminationOnCorruption,
        nullptr,
        0);

    SudokuEngine sudokuSolver;

    if (sudokuSolver.Initialize(825, 825))
        sudokuSolver.Run();

    return 0;
}