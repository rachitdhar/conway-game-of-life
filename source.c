#include "helper.h"
#include "source.h"

static bool cell_state[GRID_WIDTH][GRID_HEIGHT] = { 0 };
static bool is_paused = TRUE;
static int alive_cell_count = 0;

void handleTimeTick()
{
    int neighbors[GRID_WIDTH][GRID_HEIGHT] = { 0 };
    countAliveNeighbors(neighbors);

    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            aliveDeadRulesDecision(neighbors, i, j);
        }
    }
}

void handleMouseLeftClick(long long lParam)
{
    // client area coordinates of mouse click
    int xpos = LOWORD(lParam);
    int ypos = HIWORD(lParam);

    int grid_x = xpos / CELL_SIZE;
    int grid_y = ypos / CELL_SIZE;

    if (grid_x < GRID_WIDTH && grid_y < GRID_HEIGHT)
        cell_state[grid_x][grid_y] = !cell_state[grid_x][grid_y];
}

void aliveDeadRulesDecision(int neighbors[GRID_WIDTH][GRID_HEIGHT], int i, int j)
{
    if (neighbors[i][j] < 2 || neighbors[i][j] > 3) cell_state[i][j] = FALSE;
    if (neighbors[i][j] == 3) cell_state[i][j] = TRUE;
}

void countAliveNeighbors(int neighbors[GRID_WIDTH][GRID_HEIGHT])
{
    // i = 0
    for (int j = 0; j < GRID_HEIGHT; ++j) {
        if (cell_state[0][j]) {
            neighbors[1][j]++;
            neighbors[GRID_WIDTH - 1][j]++;

            if (j == 0) {
                neighbors[GRID_WIDTH - 1][GRID_HEIGHT - 1]++;
                neighbors[0][GRID_HEIGHT - 1]++;
                neighbors[1][GRID_HEIGHT - 1]++;
            }
            else {
                neighbors[GRID_WIDTH - 1][j - 1];
                neighbors[0][j - 1]++;
                neighbors[1][j - 1]++;
            }

            if (j == GRID_HEIGHT - 1) {
                neighbors[GRID_WIDTH - 1][0]++;
                neighbors[0][0]++;
                neighbors[1][0]++;
            }
            else {
                neighbors[GRID_WIDTH - 1][j + 1]++;
                neighbors[0][j + 1]++;
                neighbors[1][j + 1]++;
            }
        }
    }

    // i = GRID_WIDTH - 1
    for (int j = 0; j < GRID_HEIGHT; ++j) {
        if (cell_state[GRID_WIDTH - 1][j]) {
            neighbors[GRID_WIDTH - 2][j]++;
            neighbors[0][j]++;

            if (j == 0) {
                neighbors[GRID_WIDTH - 2][GRID_HEIGHT - 1]++;
                neighbors[GRID_WIDTH - 1][GRID_HEIGHT - 1]++;
                neighbors[0][GRID_HEIGHT - 1]++;
            }
            else {
                neighbors[GRID_WIDTH - 2][j - 1];
                neighbors[GRID_WIDTH - 1][j - 1]++;
                neighbors[0][j - 1]++;
            }

            if (j == GRID_HEIGHT - 1) {
                neighbors[GRID_WIDTH - 2][0]++;
                neighbors[GRID_WIDTH - 1][0]++;
                neighbors[0][0]++;
            }
            else {
                neighbors[GRID_WIDTH - 2][j + 1]++;
                neighbors[GRID_WIDTH - 1][j + 1]++;
                neighbors[0][j + 1]++;
            }
        }
    }

    for (int i = 1; i < GRID_WIDTH - 1; ++i) {
        for (int j = 1; j < GRID_HEIGHT - 1; ++j) {
            if (cell_state[i][j]) {
                neighbors[i - 1][j - 1]++;
                neighbors[i][j - 1]++;
                neighbors[i + 1][j - 1]++;
                neighbors[i - 1][j]++;
                neighbors[i + 1][j]++;
                neighbors[i - 1][j + 1]++;
                neighbors[i][j + 1]++;
                neighbors[i + 1][j + 1]++;
            }
        }
    }
}

void displayAliveCells(HDC hdc)
{
    alive_cell_count = 0;
    for (int i = 0; i < GRID_WIDTH; ++i) {
        for (int j = 0; j < GRID_HEIGHT; ++j) {
            if (cell_state[i][j]) {
                drawAtLocation(hdc, i, j);
                alive_cell_count++;
            }
        }
    }
}

void displayBottomStrip(HDC hdc)
{
    Rectangle(hdc, 0, WIN_HEIGHT, WIN_WIDTH, WIN_HEIGHT + BOTTOM_STRIP_HEIGHT);
}

void displayText(HDC hdc)
{
    // show living cell count
    char str[10] = "", text[34] = "";
    sprintf_s(str, sizeof(str), "%d", alive_cell_count);
    multi_concat(text, sizeof(text), 2, "Number of living cells: ", str);

    HFONT hFont = CreateFontA(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, TEXT_FONT);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetTextColor(hdc, TEXT_COLOR);
    SetBkMode(hdc, TRANSPARENT);

    SIZE text_size = { 0 };
    GetTextExtentPoint32A(hdc, text, lstrlenA(text), &text_size);
    TextOutA(hdc, WIN_CENTER * CELL_SIZE - text_size.cx / 2, WIN_HEIGHT + CELL_SIZE / 4, text, lstrlenA(text));

    // show whether paused or running
    const char* str_state = (is_paused) ? "-- PAUSED --" : "-- RUNNING --";
    TextOutA(hdc, CELL_SIZE, WIN_HEIGHT + CELL_SIZE / 4, str_state, lstrlenA(str_state));

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

void drawAtLocation(HDC hdc, int x, int y)
{
    Rectangle(hdc, x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE);
}

void handleDrawing(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    // clear background
    HBRUSH bgBrush = CreateSolidBrush(BG_COLOR);
    FillRect(hdc, &ps.rcPaint, bgBrush);
    DeleteObject(bgBrush);

    // display agent
    HBRUSH cellBrush = CreateSolidBrush(ALIVE_CELL_COLOR);
    HBRUSH oldBrush = SelectObject(hdc, cellBrush);
    displayAliveCells(hdc);
    SelectObject(hdc, oldBrush);
    DeleteObject(cellBrush);

    // display bottom strip
    HBRUSH bottomStripBrush = CreateSolidBrush(BOTTOM_STRIP_COLOR);
    oldBrush = SelectObject(hdc, bottomStripBrush);
    displayBottomStrip(hdc);
    SelectObject(hdc, oldBrush);
    DeleteObject(bottomStripBrush);

    // display score
    displayText(hdc);

    EndPaint(hwnd, &ps);
}

// Window procedure function to handle events
long long __stdcall WindowProc(HWND hwnd, UINT uMsg, unsigned long long wParam, long long lParam) {
    switch (uMsg) {
    case WM_PAINT:
        handleDrawing(hwnd);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_SPACE) is_paused = !is_paused;
        if (wParam == (int)'N') {
            memset(cell_state, 0, sizeof(cell_state));
            is_paused = TRUE;
            alive_cell_count = 0;
        }
        if (is_paused) InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_LBUTTONDOWN:
        handleMouseLeftClick(lParam);
        if (is_paused) InvalidateRect(hwnd, NULL, TRUE);
        break;
    case WM_TIMER:
        if (!is_paused) {
            handleTimeTick();
            InvalidateRect(hwnd, NULL, TRUE);   // Request complete window redraw
        }
        break;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

// Entry point for Windows applications
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow) {
    // setup the window
    WNDCLASSA wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BasicWindowClass";

    RegisterClassA(&wc);

    RECT adjustedRect = { 0, 0, WIN_WIDTH, WIN_HEIGHT + BOTTOM_STRIP_HEIGHT };
    AdjustWindowRect(&adjustedRect, WS_OVERLAPPEDWINDOW, FALSE);

    int actualWidth = adjustedRect.right - adjustedRect.left;
    int actualHeight = adjustedRect.bottom - adjustedRect.top;

    // create a window (having no maximize/minimize, and being non-resizeable)
    HWND hwnd = CreateWindowExA(
        0, "BasicWindowClass", "Conway's Game of Life",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        actualWidth,
        actualHeight,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    SetTimer(hwnd, 1, 100, NULL);    // Timer event every 100 ms

    MSG msg = { 0 };
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}