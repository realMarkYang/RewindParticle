#include "raylib.h"
#include <math.h>
#include <stdint.h>

static uint32_t Hash(uint32_t x) {
    x ^= x >> 16; x *= 0x7feb352d;
    x ^= x >> 15; x *= 0x846ca68b;
    x ^= x >> 16; return x;
}

static float Rand01(uint32_t seed, uint32_t i, uint32_t k) {
    return (Hash(seed ^ Hash(i * 7919u + k)) & 0xFFFFFF) / (float)0x1000000;
}

typedef struct { Vector2 pos; float t0; uint32_t seed; int count; float life; } Burst;

static void DrawBurst(Burst b, float t) {
    float age = t - b.t0;
    if (age < 0 || age > b.life) return;
    for (int i = 0; i < b.count; i++) {
        float ang = Rand01(b.seed, i, 0) * 2 * PI;
        float spd = 80 + Rand01(b.seed, i, 1) * 200;
        Vector2 p = {
            b.pos.x + cosf(ang) * spd * age,    //normal x move
            b.pos.y + sinf(ang) * spd * age + 0.5f * 400 * age * age  // gravity
        };

        Color pcolor = {0};
        pcolor.r = Rand01(b.seed, i, 2) * 255;
        pcolor.g = Rand01(b.seed, i, 3) * 255;
        pcolor.b = Rand01(b.seed, i, 4) * 255;
        pcolor.a = 255;
        DrawCircleV(p, 3, Fade(pcolor, 1.0f - age / b.life));
    }
}

int main(void) {
    InitWindow(800, 600, "rewindable particles");
    SetTargetFPS(60);

    Burst bursts[64]; int n = 0;
    float t = 0;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (IsKeyDown(KEY_SPACE)) t -= dt; else t += dt;   // rewind
        if (t < 0) t = 0;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && n < 64) {
            // Spawn a new event at the current time point, discarding any future events.
            int k = 0;
            for (int i = 0; i < n; i++) if (bursts[i].t0 <= t) bursts[k++] = bursts[i];
            n = k;
            bursts[n++] = (Burst){ GetMousePosition(), t, (uint32_t)GetRandomValue(1, 1 << 30), 80, 2.0f };
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < n; i++) DrawBurst(bursts[i], t);
        DrawText(IsKeyDown(KEY_SPACE) ? "<< REWIND" : "PLAY >>", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
}