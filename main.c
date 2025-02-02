#include "app.h"
#include "ball.h"
#include "brick.h"
#include "cloud.h"
#include "color.h"
#include "level_parser.h"
#include "math_helper.h"
#include "paddle.h"
#include "texture.h"
#include "vector.h"
extern Color AVAILABLE_COLORS[];
App app;

int main(int argc, char* argv[]) {

    // Vector2* vectorA = vector2_Construct(-2, 5);
    // Vector2* vectorB = vector2_Construct(4, 3);
    // printf("%f\n", rad2Deg(vector2_AngleBetweenVector(vectorA, vectorB)));
    // vector2_Free(vectorA);
    // vector2_Free(vectorB);
    // return 0;
    if (!app_Init(&app)) {
        printf("Erreur SDL : %s\n", SDL_GetError());
        return 1;
    }

    CloudBackground* cloudBg = cloudbackground_Construct();

    const int paddleTipWidth = 30;
    const int paddleHeight = 20;
    const int paddleWidthWithTips = WINDOW_WIDTH / 5 + 2 * paddleTipWidth;
    const float paddleMass = 5.0f;
    Paddle* paddle = paddle_Construct(paddleWidthWithTips, paddleTipWidth, paddleHeight, paddleMass, color_Construct(255, 255, 255, 255), vector2_Construct((WINDOW_WIDTH / 2) - paddleWidthWithTips / 2, WINDOW_HEIGHT - paddleHeight - 20));

    const float ballSpeed = 550.0f;
    const int ballSize = 24;
    SDL_Texture* ballTexture = texture_Load("./assets/ball_16.png");


    Ball* ball = ball_Construct(ballTexture, vector2_Construct(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), ballSpeed, ballSize);
    SDL_Texture* brickTexture = texture_Load("./assets/brick14x8.png");
    Dimension brickDimension = brick_getDimension(BRICK_WIDTH, brickTexture);
    Level* level = level_ParseFile(&brickDimension, WINDOW_WIDTH, WINDOW_HEIGHT, brickTexture);

    SDL_Texture* cattails = texture_Load("./assets/cattails.png");
    SDL_Texture* cattailsdragonfly = texture_Load("./assets/cattails_dragonfly.png");
    SDL_Rect plant;
    plant.w = 150;
    plant.h = 150;
    plant.x = 0;
    plant.y = WINDOW_HEIGHT - plant.h;

    SDL_Rect plant2;
    plant2.w = 160;
    plant2.h = 160;
    plant2.x = WINDOW_WIDTH - plant2.w;
    plant2.y = WINDOW_HEIGHT - plant2.h;
    SDL_Rect src;
    src.w = 32;
    src.h = 32;
    src.x = 0;
    src.y = 0;
    int nbrFrames = 41;



    app.endFrame = SDL_GetTicks();
    while (app.isRunning) {
        while (SDL_PollEvent(&app.e)) {
            if (app.e.type == SDL_QUIT) {
                app.isRunning = false;
            }
        }
        paddle_Event(paddle, SDL_GetKeyboardState(NULL));
        ball_Event(ball, SDL_GetKeyboardState(NULL));
        app_SetDeltaTime(&app);

        paddle_Update(paddle);
        ball_Update(ball, paddle, level->bricks, &brickDimension, level->length);

        SDL_SetRenderDrawColor(app.renderer, 169, 224, 232, 255);
        SDL_RenderClear(app.renderer);

        cloudbackground_Draw(cloudBg);
        SDL_RenderCopy(app.renderer, cattails, NULL, &plant);
        src.x = src.w * (int)(SDL_GetTicks() / 100 % nbrFrames);
        SDL_RenderCopyEx(app.renderer, cattailsdragonfly, &src, &plant2, 0, NULL, SDL_FLIP_NONE);

        for (int i = 0; i < level->length; i++) {
            brick_Draw(level->bricks[i]);
        }

        paddle_Draw(paddle);
        ball_Draw(ball);


        SDL_RenderPresent(app.renderer);

        if (app.shouldCapFrameRate) {
            app_CapFrameRate(&app);
        }

        // app_printFrameRate(&app);
        app.endFrame = app.startFrame;
    }

    cloudbackground_Free(cloudBg);
    SDL_DestroyTexture(brickTexture);
    SDL_DestroyTexture(cattails);
    SDL_DestroyTexture(cattailsdragonfly);
    level_Free(level);
    paddle_Free(paddle);
    ball_Free(ball);
    app_Free(&app);
}