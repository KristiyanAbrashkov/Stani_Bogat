#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"

static Question* currentQuestion = NULL;
static int randomSeeded = 0;
static int availableAnswers[4] = {1, 1, 1, 1};

#define CHOICE_5050 4
#define CHOICE_FRIEND 5
#define CHOICE_AUDIENCE 6

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

static void setCurrentQuestion(Question* question) {
	if (currentQuestion != question) {
		currentQuestion = question;

		for (int i = 0; i < 4; i++) {
			availableAnswers[i] = 1;
		}
	}
}

static int hintChance(int difficulty) {
	if (difficulty <= 3) {
		return 80;
	}

	if (difficulty <= 6) {
		return 60;
	}

	return 30;
}

static int readGameChoice() {
	char input[20];

	while (1) {
		printf("Your choice: ");

		if (fgets(input, sizeof(input), stdin) == NULL) {
			return -1;
		}

		if (input[0] >= 'a' && input[0] <= 'd') {
			return input[0] - 'a';
		}

		if (input[0] >= 'A' && input[0] <= 'D') {
			return input[0] - 'A';
		}

		if (input[0] == '1') {
			return CHOICE_5050;
		}

		if (input[0] == '2') {
			return CHOICE_FRIEND;
		}

		if (input[0] == '3') {
			return CHOICE_AUDIENCE;
		}

		printf("Invalid choice. Enter A-D or a lifeline number.\n");
	}
}

void startGame() {
	Question* gameQuestions[10];
	int lifeline5050Available = 1;
	int friendLifelineAvailable = 1;
	int audienceLifelineAvailable = 1;
	int difficulty;
	int questionNumber;
	int choice;

	seedRandomGenerator();
	currentQuestion = NULL;

	for (difficulty = 1; difficulty <= 10; difficulty++) {
		gameQuestions[difficulty - 1] = getRandomQuestion(difficulty);

		if (gameQuestions[difficulty - 1] == NULL) {
			printf("The game cannot start because there is no question "
			       "with difficulty %d.\n", difficulty);
			currentQuestion = NULL;
			return;
		}
	}

	printf("\nThe game has started! Answer all 10 questions to win.\n");

	for (questionNumber = 0; questionNumber < 10; questionNumber++) {
		setCurrentQuestion(gameQuestions[questionNumber]);

		while (1) {
			printf("\n========================================\n");
			printf("Question %d/10 | Difficulty: %d/10\n",
			       questionNumber + 1, currentQuestion->difficulty);
			printf("========================================\n");
			printf("%s\n\n", currentQuestion->question);

			for (int i = 0; i < 4; i++) {
				if (availableAnswers[i]) {
					printf("  %s) %s\n",
					       answerLabel(i), currentQuestion->answers[i]);
				}
			}

			printf("\nAvailable lifelines:\n");
			if (lifeline5050Available) {
				printf("  1) 50/50\n");
			}
			if (friendLifelineAvailable) {
				printf("  2) Phone a friend\n");
			}
			if (audienceLifelineAvailable) {
				printf("  3) Ask the audience\n");
			}
			printf("Enter A-D to answer or choose an available lifeline.\n");

			choice = readGameChoice();

			if (choice == -1) {
				printf("The game was interrupted.\n");
				currentQuestion = NULL;
				return;
			}

			if (choice == CHOICE_5050) {
				if (lifeline5050Available) {
					joker5050(currentQuestion);
					lifeline5050Available = 0;
				} else {
					printf("The 50/50 lifeline has already been used.\n");
				}
				continue;
			}

			if (choice == CHOICE_FRIEND) {
				if (friendLifelineAvailable) {
					friendHelp(currentQuestion);
					friendLifelineAvailable = 0;
				} else {
					printf("Phone a friend has already been used.\n");
				}
				continue;
			}

			if (choice == CHOICE_AUDIENCE) {
				if (audienceLifelineAvailable) {
					audienceHelp(currentQuestion);
					audienceLifelineAvailable = 0;
				} else {
					printf("Ask the audience has already been used.\n");
				}
				continue;
			}

			if (!availableAnswers[choice]) {
				printf("This answer was removed by the 50/50 lifeline.\n");
				continue;
			}

			if (choice != currentQuestion->correctAnswer) {
				printf("Wrong answer! The correct answer was %s.\n",
				       answerLabel(currentQuestion->correctAnswer));
				printf("Game over!\n");
				currentQuestion = NULL;
				return;
			}

			printf("Correct answer!\n");
			break;
		}
	}

	printf("\nCongratulations! You answered all 10 questions correctly!\n");
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
	setCurrentQuestion(&questions[selectedIndex]);
	return currentQuestion;
}

void joker5050(Question* q) {
	if (q != NULL) {
		setCurrentQuestion(q);
	}

	if (currentQuestion == NULL) {
		printf("No active question!\n");
		return;
	}

	int wrongAnswers[3];
	int wrongCount = 0;

	for (int i = 0; i < 4; i++) {
		if (i != currentQuestion->correctAnswer && availableAnswers[i]) {
			wrongAnswers[wrongCount++] = i;
		}
	}

	if (wrongCount <= 1) {
		printf("The 50/50 lifeline is already active.\n");
		return;
	}

	int keptWrongAnswer = wrongAnswers[rand() % wrongCount];

	for (int i = 0; i < 4; i++) {
		if (i != currentQuestion->correctAnswer && i != keptWrongAnswer) {
			availableAnswers[i] = 0;
		}
	}

	printf("50/50 removed two incorrect answers.\n");
}

int friendHelp(Question* q) {
	if (q != NULL) {
		setCurrentQuestion(q);
	}

	if (currentQuestion == NULL) {
		printf("No active question!\n");
		return -1;
	}

	int roll = rand() % 100;
	int suggestion = currentQuestion->correctAnswer;

	if (roll >= hintChance(currentQuestion->difficulty)) {
		do {
			suggestion = rand() % 4;
		} while (suggestion == currentQuestion->correctAnswer ||
		         !availableAnswers[suggestion]);
	}

	printf("Your friend thinks the answer is %s (%s).\n",
	       answerLabel(suggestion), currentQuestion->answers[suggestion]);

	return suggestion;
}

void audienceHelp(Question* q) {
	if (q != NULL) {
		setCurrentQuestion(q);
	}

	if (currentQuestion == NULL) {
		printf("No active question!\n");
		return;
	}

	int shares[4] = {0, 0, 0, 0};
	int otherIndexes[3] = {0, 0, 0};
	int otherCount = 0;

	for (int i = 0; i < 4; i++) {
		if (i != currentQuestion->correctAnswer && availableAnswers[i]) {
			otherIndexes[otherCount++] = i;
		}
	}

	int correctHasMostVotes =
		rand() % 100 < hintChance(currentQuestion->difficulty);
	int remainingVotes;

	if (correctHasMostVotes) {
		shares[currentQuestion->correctAnswer] = 55 + rand() % 16;
		remainingVotes = 100 - shares[currentQuestion->correctAnswer];
	} else {
		shares[currentQuestion->correctAnswer] = 10 + rand() % 16;
		shares[otherIndexes[0]] = 45 + rand() % 11;
		remainingVotes = 100 - shares[currentQuestion->correctAnswer]
		                 - shares[otherIndexes[0]];
	}

	for (int i = 0; i < otherCount; i++) {
		if (shares[otherIndexes[i]] != 0) {
			continue;
		}

		int answersLeft = 0;
		for (int j = i; j < otherCount; j++) {
			if (shares[otherIndexes[j]] == 0) {
				answersLeft++;
			}
		}

		if (answersLeft == 1) {
			shares[otherIndexes[i]] = remainingVotes;
			remainingVotes = 0;
		} else {
			int votes = rand() % (remainingVotes + 1);
			shares[otherIndexes[i]] = votes;
			remainingVotes -= votes;
		}
	}

	if (remainingVotes > 0) {
		shares[otherIndexes[0]] += remainingVotes;
	}

	printf("The audience votes:\n");
	for (int i = 0; i < 4; i++) {
		if (availableAnswers[i]) {
			printf("%s: %d%%\n", answerLabel(i), shares[i]);
		}
	}
}
