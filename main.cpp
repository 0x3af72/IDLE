#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>

POINT trackMousePos() {
    POINT point;
    GetCursorPos(&point);
    return point;
}

void bounceCursor(int velX, int velY, int screenW, int screenH) {
    POINT expected = trackMousePos();
    while (true) {

        // Check if user is still idle
        POINT mousePos = trackMousePos();
        if (mousePos.x != expected.x || mousePos.y != expected.y) break;

        // Calculate next position
        expected.x += velX;
        expected.y += velY;

        // Calculate bounce
        if (expected.x <= 0) {
            expected.x = 0;
            velX *= -1;
        } else if (expected.x >= screenW) {
            expected.x = screenW - 1;
            velX *= -1;
        }
        if (expected.y <= 0) {
            expected.y = 0;
            velY *= -1;
        } else if (expected.y >= screenH) {
            expected.y = screenH - 1;
            velY *= -1;
        }

        // Move cursor
        SetCursorPos(expected.x, expected.y);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    int prevMouseX = 0;
    int prevMouseY = 0;
    int velX = 0;
    int velY = 0;

    while (true) {
        POINT mousePos = trackMousePos();
        if (GetAsyncKeyState(VK_LMENU) & 0x8000 && !(mousePos.x == prevMouseX && mousePos.y == prevMouseY)) {
            velX = mousePos.x - prevMouseX;
            velY = mousePos.y - prevMouseY;
            prevMouseX = mousePos.x;
            prevMouseY = mousePos.y;
        } else if (velX || velY) {
            bounceCursor(velX, velY, screenW, screenH);
            velX = 0;
            velY = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    return 0;
}
