#include <stdio.h>

#include "question.h"
#include "game.h"

int main()
{
    int choice;

    loadQuestions("questions.txt");

    do {
        printf("\n===== WHO WANTS TO BE A MILLIONAIRE =====\n");
        printf("1. Start game\n");
        printf("2. Add question\n");
        printf("3. Edit question\n");
        printf("4. Show questions\n");
        printf("5. Exit\n");
        printf("Choose an option: ");

        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                startGame();
                break;
            case 2:
                addQuestion();
                break;
            case 3:
                editQuestion();
                break;
            case 4:
                showQuestions();
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please choose from 1 to 4.\n");
                break;
        }
    } while (choice != 5);

    saveQuestions("questions.txt");

    return 0;
}
// gcc main.c question.c game.c crypto.c -o stani_bogat.exe