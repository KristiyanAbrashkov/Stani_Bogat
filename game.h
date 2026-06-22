#ifndef GAME_H
#define GAME_H

#include "question.h"

void startGame();

Question* getRandomQuestion(int difficulty);

void joker5050(Question* q);

int friendHelp(Question* q);

void audienceHelp(Question* q);

#endif