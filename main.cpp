#include <raylib.h>

#include <future>
#include <vector>

#include "objects.cpp"
using namespace std;

#define WIDTH 640.0
#define LENGTH 640.0

int main() {
  InitWindow(WIDTH, LENGTH, "Snake Game");
  SetTargetFPS(10);
  vector<SnakePart> player = summonPlayer();

  Apple apple = Apple{};
  Directions lastHeadDirection = player[0].direction;
  bool gameover = true;
  int score = 0;
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(LIGHTGRAY);
    if (gameover) {
      DrawRectangle(WIDTH / 2 - 90, LENGTH / 2 - 20, 150, 60, BLACK);
      DrawText(("Score: " + std::to_string(score)).c_str(),
               WIDTH / 2 - (85 + std::to_string(score).length() * 5),
               LENGTH / 2 - 80, 35, BLACK);
      DrawText("Start", WIDTH / 2 - 50, LENGTH / 2, 25, RAYWHITE);
      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        Rectangle button = Rectangle{.x = WIDTH / 2 - 90,
                                     .y = LENGTH / 2 - 20,
                                     .width = 150,
                                     .height = 60};
        if ((button.x <= mouse.x && mouse.x <= button.x + button.width) &&
            (button.y <= mouse.y && mouse.y <= button.y + button.height)) {
          score = 0;
          gameover = false;
        }
      }
      if (IsKeyPressed(KEY_ENTER)) {
        score = 0;
        gameover = false;
      }
      EndDrawing();
      continue;
    }
    if (IsKeyDown(KEY_UP) && player[0].direction != Directions::DOWN)
      player[0].direction = Directions::UP;
    else if (IsKeyDown(KEY_DOWN) && player[0].direction != Directions::UP)
      player[0].direction = Directions::DOWN;
    else if (IsKeyDown(KEY_RIGHT) && player[0].direction != Directions::LEFT)
      player[0].direction = Directions::RIGHT;
    else if (IsKeyDown(KEY_LEFT) && player[0].direction != Directions::RIGHT)
      player[0].direction = Directions::LEFT;

    apple.draw(player);
    for (int i = player.size() - 1; i >= 0; i--) {
      if (i != 0 &&
          CheckCollisionRecs(player[i].getRect(), player[0].getRect())) {
        gameover = true;
        player = summonPlayer();
        apple = Apple{};
        lastHeadDirection = player[0].direction;
        break;
      }
      switch (i) {
        case 0:
          lastHeadDirection = player[i].direction;
          break;
        case 1:
          player[i].direction = lastHeadDirection;
          break;
        default:
          player[i].direction = player[i - 1].direction;
          break;
      }
      if (player[i].direction == Directions::UP ||
          player[i].direction == Directions::DOWN) {
        player[i].posY += pow(-1.0, player[i].direction + 1);
        if (player[i].posY == ARENA_LENGTH) {
          player[i].posY = 0;
        } else if (player[i].posY < 0) {
          player[i].posY = ARENA_LENGTH - 1;
        }
      } else if (player[i].direction == Directions::RIGHT ||
                 player[i].direction == Directions::LEFT) {
        player[i].posX += pow(-1.0, player[i].direction);
        if (player[i].posX == ARENA_WIDTH) {
          player[i].posX = 0;
        } else if (player[i].posX < 0) {
          player[i].posX = ARENA_WIDTH - 1;
        }
      }
      player[i].draw();
    }
    if (player[0].posX == apple.posX && player[0].posY == apple.posY) {
      score++;
      apple = Apple{};
      SnakePart last = player[player.size() - 1];
      if (last.direction == Directions::UP ||
          last.direction == Directions::DOWN)
        player.push_back(
            SnakePart{.posX = last.posX,
                      .posY = (float)(last.posY + pow(-1.0, last.direction)),
                      .direction = last.direction});
      if (last.direction == Directions::RIGHT ||
          last.direction == Directions::LEFT)
        player.push_back(SnakePart{
            .posX = (float)(last.posX + pow(-1.0, last.direction + 1)),
            .posY = last.posY,
            .direction = last.direction});
    };
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
