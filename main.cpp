#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <fstream>
using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
int cellSize = 28;
int cellCount = 23;
int offset = 70;
double lastupdatetime = 0;
bool running = true;
bool paused = false;

bool ElementinDeque(deque<Vector2> deque, Vector2 element)
{
    for (unsigned int i = 0; i < deque.size(); i++)
    {
        if (Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currenttime = GetTime();
    if (currenttime - lastupdatetime >= interval)
    {
        lastupdatetime = currenttime;
        return true;
    }
    return false;
}

int LoadHighScore()
{
    ifstream file("highscore.txt");
    int highScore = 0;
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    return highScore;
}
void SaveHighScore(int highScore)
{
    ofstream file("highscore.txt");
    file << highScore;
    file.close();
}

class Snake
{
public:
    deque<Vector2> body = {Vector2{5, 5}, Vector2{4, 5}, Vector2{3, 5}};
    Vector2 direction = {1, 0};
    bool addsegment = false;

    void Draw()
    {
        for (unsigned int i = 0; i < body.size(); i++)
        {
            int x = body[i].x;
            int y = body[i].y;
            Rectangle segment =
                Rectangle{
                    offset + (float)x * cellSize,
                    offset + (float)y * cellSize,
                    (float)cellSize,
                    (float)cellSize};
            DrawRectangleRounded(segment, 0.7, 6, darkGreen);
        }
    }
    void Move()
    {
        body.push_front(Vector2Add(body[0], direction));

        if (addsegment == true)
        {
            addsegment = false;
        }
        else
        {
            body.pop_back();
        }
    }
    void reset()
    {
        body.clear();
        body.push_back({5, 5});
        body.push_back({4, 5});
        body.push_back({3, 5});
        direction = {1, 0};
    }
};
class Food
{
public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody)
    {
        Image image = LoadImage("Resources/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GetrandomPosition(snakeBody);
    }
    ~Food()
    {
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
    }

    Vector2 Generaterandomcell()
    {
        int x = GetRandomValue(0, cellCount - 1);
        int y = GetRandomValue(0, cellCount - 1);
        return Vector2{(float)x, (float)y};
    }

    Vector2 GetrandomPosition(deque<Vector2> snakeBody)
    {

        Vector2 position = Generaterandomcell();
        while (ElementinDeque(snakeBody, position))
        {
            position = Generaterandomcell();
        }
        return position;
    }
};

class Game
{
public:
    int score = 0;
    int highScore = 0;
    Snake snake = Snake();
    Food food = Food(snake.body);
    Sound eatSound;
    Sound wallSound;

    Game()
    {
        InitAudioDevice();
        eatSound = LoadSound("Resources/eat.mp3");
        wallSound = LoadSound("Resources/wall.mp3");
    }
    ~Game()
    {
        UnloadSound(eatSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw()
    {
        snake.Draw();
        food.Draw();
    }
    void Move()
    {
        if (running)
        {
            snake.Move();
            Checkcollisionwithfood();
            Checkcollisionwithedges();
            Checkcollisionwithitself();
        }
    }
    void Checkcollisionwithfood()
    {
        if (Vector2Equals(snake.body[0], food.position))
        {
            food.position = food.GetrandomPosition(snake.body);
            snake.addsegment = true;
            score++;
            PlaySound(eatSound);
        }
    }
    void Checkcollisionwithedges()
    {
        if (snake.body[0].x == cellCount || snake.body[0].x == -1)
        {
            Gameover();
        }
        if (snake.body[0].y == cellCount || snake.body[0].y == -1)
        {
            Gameover();
        }
    }
    void Checkcollisionwithitself()
    {
        deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if (ElementinDeque(headlessbody, snake.body[0]))
        {
            Gameover();
        }
    }
    void Gameover()
    {
        if (score > highScore)
        {
            highScore = score;
            SaveHighScore(highScore);
        }
        running = false;
        snake.reset();
        food.position = food.GetrandomPosition(snake.body);
        PlaySound(wallSound);
    }
};

int main()
{

    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake Game");
    SetTargetFPS(60);

    Game game = Game();
    game.highScore = LoadHighScore();


    while (WindowShouldClose() == false)
    {
        BeginDrawing();


        if (!running && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)))
        {
            running = true;
            game.score = 0;
            game.snake.reset();
        }

        if (!paused && eventTriggered(0.2))
        {
            game.Move();
        }

        if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
        {
            game.snake.direction = {0, -1};
            running = true;
        }
        if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
        {
            game.snake.direction = {0, 1};
            running = true;
        }
        if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
        {
            game.snake.direction = {-1, 0};
            running = true;
        }
        if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
        {
            game.snake.direction = {1, 0};
            running = true;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            paused = !paused;
        }
        if (paused && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) ||
                       IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_RIGHT)))
        {
            paused = false;
        }
        // Background
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, DARKGREEN);
        DrawText("--Retro Snake--", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %d", game.score), offset + 5, offset + cellSize * cellCount + 10, 40, darkGreen);
        DrawText(TextFormat("High Score: %d", game.highScore), offset + 10 + MeasureText(TextFormat("Score: %d", game.score), 40) + 100, offset + cellSize * cellCount + 10, 40, darkGreen);
        game.Draw();

        if (!running)
        {
            Font gameoverfont = LoadFont("Resources/game_over.ttf");
            float fontSize = 80;

            DrawTextEx(
                gameoverfont,
                "Game Over!",
                {(float)GetScreenWidth() / 2 - MeasureTextEx(gameoverfont, "Game Over!", fontSize, 2).x / 2,
                 (float)GetScreenHeight() / 2 - 80},
                fontSize,
                2,
                darkGreen);

            DrawText(
                TextFormat("Score: %d", game.score),
                GetScreenWidth() / 2 - MeasureText(TextFormat("Score: %d", game.score), 30) / 2,
                GetScreenHeight() / 2 + 10,
                30,
                darkGreen);
        }
        if (paused)
        {
            Font pausedfont = LoadFont("Resources/game_over.ttf");
            float fontSize = 80;
            DrawTextEx(
                pausedfont,
                "PAUSED",
                {(float)GetScreenWidth() / 2 - MeasureText("PAUSED", 60) / 2,
                 (float)GetScreenHeight() / 2 - 30},
                fontSize,
                2,
                darkGreen);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}