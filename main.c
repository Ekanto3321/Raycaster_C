#include "raylib.h"
#include "rlgl.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "const.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

void init()
{
    InitWindow(screenWidth, screenHeight, "raylib [core] example - input keys");
    SetTargetFPS(fps);
    angleincrement = 1.0f / rayPerDeg;
    rectSize = screenWidth / (fov * rayPerDeg);
    generateMap();
    srand((unsigned int)time(NULL));
}

int main(void)
{
    init();

    while (!WindowShouldClose())
    {
        controls();

        BeginDrawing();

        if (!mode)
        {
            render2D();
        }
        else
        {
            render3D();
        }

        ClearBackground(BLACK);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

void render2D()
{
    drawGrid();
    drawMap();

    for (float a = 0; a < fov; a += angleincrement) // fov rendering
    {
        float arg = rot_rad + (a - fov/2) * DEG2RAD;
        float x_dst = cos(arg);
        float y_dst = sin(arg);
        DrawLine(playerPos.x, playerPos.y, playerPos.x + checkCol(a, x_dst, y_dst, 1) * x_dst, playerPos.y + checkCol(a, x_dst, y_dst, 1) * y_dst, YELLOW);
    }

    drawPlayer();
}

void render3D()
{

    DisableCursor();
    DrawRectangle(0, 0, screenWidth, screenHeight / 2, (Color){10, 36, 74, 255});
    DrawRectangle(0, screenHeight / 2, screenWidth, screenHeight / 2, (Color){43, 43, 48, 255});

    for (float a = 0; a <= (fov * rayPerDeg); a += angleincrement)
    {
        float arg = rot_rad + (a - fov / 2.0f) * DEG2RAD;
        float x_dst = cos(arg);
        float y_dst = sin(arg);

        float distance = checkCol(a, x_dst, y_dst, 1);
        float distA = distance * cos(arg - rot_rad);
        if (distA < 0.01f)
            distA = 0.01f;
        float size = 300.0f / distA;
        float col = 255 - ((distA / 1600) * 255);

        DrawRectangle(a * rectSize * rayPerDeg, (screenHeight / 2)  - (size / 2) * heightscalar, rectSize, size  * heightscalar, (Color){col, col, col, 255});
    }

    DrawCircle(screenWidth / 2 - 2, screenHeight / 2 - 2, 4, RED);
}

float checkCol(int a, float x_dst, float y_dst, int target)
{
    int x, y;
    float dist = 0;
    while (map[((int)(playerPos.y + dist * y_dst) / gridSize) % mapy][((int)(playerPos.x + dist * x_dst) / gridSize) % mapx] != target && dist <= 1000)
    {   
            dist++;
    }
    return dist;
}

void controls()
{
    rot_rad = rot * DEG2RAD;

    if (IsKeyPressed(KEY_X))
    {
        mode = !mode;
        EnableCursor();
    }

    if (IsKeyDown(KEY_W))
    {
        playerPos.x += speed * cos(rot_rad);
        playerPos.y += speed * sin(rot_rad);
    }

    if (IsKeyDown(KEY_S))
    {
        playerPos.x -= speed * cos(rot_rad);
        playerPos.y -= speed * sin(rot_rad);
    }

    if (IsKeyDown(KEY_A) && !mode)
        rot -= rot_speed;
    if (IsKeyDown(KEY_D) && !mode)
        rot += rot_speed;

    if (IsKeyDown(KEY_R))
    {
        playerPos.x = screenWidth / 2;
        playerPos.y = screenHeight / 2;
    }

    if (IsKeyDown(KEY_F) && !mode)
    {
        playerPos.x = GetMouseX();
        playerPos.y = GetMouseY();
        rot += 20 * GetMouseWheelMove();
    }

    bool keystate;

    if (!mode)
    {
        if (IsKeyDown(KEY_E))
        {
            keystate = 1;
            editBlock(keystate);
        }
        else if (IsKeyDown(KEY_Q))
        {
            keystate = 0;
            editBlock(keystate);
        }
    }

    if (mode)
    {
        rot += sens * GetMouseDelta().x;
        rot_rad = rot * DEG2RAD;

        float rightX = cosf(rot_rad + PI / 2.0f);
        float rightY = sinf(rot_rad + PI / 2.0f);

        if (IsKeyDown(KEY_A))
        {
            playerPos.x -= speed * rightX;
            playerPos.y -= speed * rightY;
        }

        if (IsKeyDown(KEY_D))
        {
            playerPos.x += speed * rightX;
            playerPos.y += speed * rightY;
        }

        if (IsKeyPressed(KEY_G))
        {
            spawnBlockArcade(1);
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {   
            arcadeKill();
        }
    }
}

void drawGrid()
{
    for (int i = 0; i < screenHeight; i += gridSize)
    {
        DrawLine(0, i, screenWidth, i, GRAY);
    }
    for (int j = 0; j < screenWidth; j += gridSize)
    {
        DrawLine(j, 0, j, screenHeight, GRAY);
    }
}

void drawMap()
{

    for (int i = 0; i < mapx; i++)
    {
        for (int j = 0; j < mapy; j++)
        {
            if (map[j][i] == 1)
                DrawRectangle(i * gridSize, j * gridSize, gridSize, gridSize, LIGHTGRAY);
        }
    }
}

void drawPlayer()
{
    DrawLine(playerPos.x, playerPos.y, playerPos.x + 50 * cos(rot_rad), playerPos.y + 50 * sin(rot_rad), ORANGE);
    DrawCircle(playerPos.x, playerPos.y, 10.0f, WHITE);
}

void editBlock(bool keystate)
{
    if (keystate)
        map[(int)(GetMouseY() / gridSize)][(int)(GetMouseX() / gridSize)] = 1;
    else
        map[(int)(GetMouseY() / gridSize)][(int)(GetMouseX() / gridSize)] = 0;
}

void generateMap()
{

    for (int i = 0; i < mapx; i++)
    {
        for (int j = 0; j < mapy; j++)
        {
            if (i == 0 || j == 0 || i == mapx - 1 || j == mapy - 1)
                map[j][i] = 1;
        }
    }
}

void spawnBlockArcade(bool spawn)
{
    int rndx;
    int rndy;

    if (spawn)
    {
        for (int i = 0; i < enemy_cnt; i++)
        {
            rndx = 1 + rand() % (mapx - 2);
            rndy = 1 + rand() % (mapy - 2);

            map[rndy][rndx] = 1;
        }
    }
}

void arcadeKill()
{
    float arg = rot_rad;
    float x_dst = cos(arg);
    float y_dst = sin(arg);

    float dst = checkCol(rot_rad, x_dst, y_dst, 1);
    int y = ((int)(playerPos.y + dst * y_dst) / gridSize) % mapy;
    int x = ((int)(playerPos.x + dst * x_dst) / gridSize) % mapx;

    if (x > 0 && x < mapx - 1 && y > 0 && y < mapy - 1)
    {
        map[y][x] = 0;
    }
}
