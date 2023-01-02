#pragma once
#include <iostream>
#include <vector>

#include "random"
#include "raylib.h"
using namespace std;

const int ARENA_WIDTH = 25.0;
const int ARENA_LENGTH = 25.0;

float convertArenaToWindowSize(int maxArena, float locationInArena,
                               int maxWindow) {
  return (float)(floor((maxWindow * locationInArena) / maxArena));
}

enum Directions { UP, DOWN, RIGHT, LEFT };

struct SnakePart {
  float posX, posY;
  Directions direction;
  void draw() {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    DrawRectangle(convertArenaToWindowSize(ARENA_WIDTH, this->posX, width),
                  convertArenaToWindowSize(ARENA_LENGTH, this->posY, height),
                  convertArenaToWindowSize(ARENA_WIDTH, 1, width),
                  convertArenaToWindowSize(ARENA_LENGTH, 1, height), BLACK);
  }

  Rectangle getRect() {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    return Rectangle{
        .x = convertArenaToWindowSize(ARENA_WIDTH, this->posX, width),
        .y = convertArenaToWindowSize(ARENA_LENGTH, this->posY, height),
        .width = convertArenaToWindowSize(ARENA_WIDTH, 1, width),
        .height = convertArenaToWindowSize(ARENA_LENGTH, 1, height)};
  }
};

struct Apple {
  float posX = floor(rand() % ARENA_WIDTH);
  float posY = floor(rand() % ARENA_LENGTH);

  void draw(vector<SnakePart> player) {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    for (int i = 1; i < player.size(); i++) {
      if (CheckCollisionRecs(player[i].getRect(), this->getRect())) {
        this->posX = floor(rand() % ARENA_WIDTH);
        this->posY = floor(rand() % ARENA_LENGTH);
        return this->draw(player);
      }
    }
    DrawRectangle(convertArenaToWindowSize(ARENA_WIDTH, this->posX, width),
                  convertArenaToWindowSize(ARENA_LENGTH, this->posY, height),
                  convertArenaToWindowSize(ARENA_WIDTH, 1, width),
                  convertArenaToWindowSize(ARENA_LENGTH, 1, height), RED);
  }

  Rectangle getRect() {
    float width = GetScreenWidth();
    float height = GetScreenHeight();
    return Rectangle{
        .x = convertArenaToWindowSize(ARENA_WIDTH, this->posX, width),
        .y = convertArenaToWindowSize(ARENA_LENGTH, this->posY, height),
        .width = convertArenaToWindowSize(ARENA_WIDTH, 1, width),
        .height = convertArenaToWindowSize(ARENA_LENGTH, 1, height)};
  }
};

vector<SnakePart> summonPlayer() {
  vector<SnakePart> player;
  for (int i = 0; i < 4; i++) {
    player.push_back(SnakePart{.posX = (float)(floor(ARENA_WIDTH / 2.0 - i)),
                               .posY = (float)(floor(ARENA_LENGTH / 2.0)),
                               .direction = Directions::RIGHT});
  }
  return player;
}
