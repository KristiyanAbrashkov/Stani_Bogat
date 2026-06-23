#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

static Question* currentQuestion = NULL;
static int randomSeeded = 0;

static void seedRandomGenerator(void) {
	if (!randomSeeded) {
		srand((unsigned)time(NULL));
		randomSeeded = 1;
	}
}

static const char* answerLabel(int index) {
	switch (index) {
		case 0: return "A";
		case 1: return "B";
		case 2: return "C";
		case 3: return "D";
		default: return "?";
	}
}

void startGame() {
	seedRandomGenerator();
	currentQuestion = NULL;
}

Question* getRandomQuestion(int difficulty) {
	seedRandomGenerator();

	if (questionCount <= 0) {
		currentQuestion = NULL;
		return NULL;
	}

	int matchingIndexes[MAX_QUESTIONS];
	int matchingCount = 0;

	for (int i = 0; i < questionCount; i++) {
		if (questions[i].difficulty == difficulty) {
			matchingIndexes[matchingCount++] = i;
		}
	}

	if (matchingCount == 0) {
		currentQuestion = NULL;
		return NULL;
	}

	int selectedIndex = matchingIndexes[rand() % matchingCount];
	currentQuestion = &questions[selectedIndex];
	return currentQuestion;
}

void joker5050(Question* q) {
	Question* question = NULL;

	if (q != NULL) {
		question = q;
	} else {
		question = currentQuestion;
	}

	if (question == NULL) {
		printf("No active question!\n");
		return;
	}

	int wrongAnswers[3];
	int wrongCount = 0;

	for (int i = 0; i < 4; i++) {
		if (i != question->correctAnswer) {
			wrongAnswers[wrongCount++] = i;
		}
	}

	int keptWrongAnswer = wrongAnswers[rand() % wrongCount];

	printf("50/50 lifeline - remaining options:\n");
	printf("%s: %s\n", answerLabel(question->correctAnswer), question->answers[question->correctAnswer]);
	printf("%s: %s\n", answerLabel(keptWrongAnswer), question->answers[keptWrongAnswer]);
}

int friendHelp(Question* q) {
	Question* question = NULL;

	if (q != NULL) {
		question = q;
	} else {
		question = currentQuestion;
	}

	if (question == NULL) {
		printf("No active question!\n");
		return -1;
	}

	int chanceOfCorrect = 80 - (question->difficulty - 1) * 12;
	if (chanceOfCorrect < 35) {
		chanceOfCorrect = 35;
	}

	int roll = rand() % 100;
	int suggestion = question->correctAnswer;

	if (roll >= chanceOfCorrect) {
		do {
			suggestion = rand() % 4;
		} while (suggestion == question->correctAnswer);
	}

	printf("Your friend thinks the answer is %s (%s).\n", answerLabel(suggestion), question->answers[suggestion]);

	return suggestion;
}

void audienceHelp(Question* q) {
    Question* question = q != NULL ? q : currentQuestion;

	if (question == NULL) {
		printf("No active question!\n");
		return;
	}

	int correctShare = 55 - (question->difficulty - 1) * 8;
	if (correctShare < 25) {
		correctShare = 25;
	}

	int remainingShare = 100 - correctShare;
	int shares[4] = {0, 0, 0, 0};
	shares[question->correctAnswer] = correctShare;

	int otherIndexes[3];
	int otherCount = 0;
	for (int i = 0; i < 4; i++) {
		if (i != question->correctAnswer) {
			otherIndexes[otherCount++] = i;
		}
	}

	int first = rand() % (remainingShare + 1);
	int second = rand() % (remainingShare - first + 1);
	int third = remainingShare - first - second;

	shares[otherIndexes[0]] = first;
	shares[otherIndexes[1]] = second;
	shares[otherIndexes[2]] = third;

	printf("The audience votes:\n");
	for (int i = 0; i < 4; i++) {
		printf("%s: %d%%\n", answerLabel(i), shares[i]);
	}
}