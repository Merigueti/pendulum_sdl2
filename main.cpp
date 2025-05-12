#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class Pendulum
{
private:
    float x1, y1;
    float x2, y2;
    float r;

    float angle = M_PI / 4;

    float gravity = 1;
    float friction = 0.999;
    float angularVelocity = 0;
    float angularAcceleration = 0;

public:
    Pendulum(float x1, float y1, float r) : x1(x1), y1(y1), r(r) {}

    void updatePosition()
    {
        angularAcceleration = -1 * gravity * sin(angle) / r;
        angularVelocity += angularAcceleration;
        angularVelocity *= friction;
        angle += angularVelocity;
        x2 = (r * sin(angle)) + x1;
        y2 = (r * cos(angle)) + y1;
    }

    void render(SDL_Renderer *renderer)
    {
        lineRGBA(renderer, x1, y1, x2, y2, 180, 180, 180, 255);
        filledCircleRGBA(renderer, x2, y2, 30, 242, 203, 5, 255);
    }
};

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Pendulum v0.0.1",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (!renderer)
    {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Pendulum pendulum(WINDOW_WIDTH / 2, 0, 200);
    Pendulum pendulum2(WINDOW_WIDTH / 2, 0, 400);

    bool running = true;

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        pendulum.updatePosition();
        pendulum2.updatePosition();
        pendulum.render(renderer);
        pendulum2.render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}