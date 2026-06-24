#include <stdio.h>
#include <string.h>
#include "crypto.h"
#include "question.h"

Question questions[MAX_QUESTIONS];
int questionCount = 0;

static const char* getDifficultyCategory(int difficulty) {
    if (difficulty <= 3) return "Easy";
    if (difficulty <= 6) return "Medium";
    return "Hard";
}
 
static void stripNewline(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void addQuestion(){
    Question q;
    int filled[7];
    int allFilled;
    int choice;
    int i;
 
    if(questionCount >= MAX_QUESTIONS){
        printf("Error: maximum number of questions reached (%d).\n", MAX_QUESTIONS);
        return;
    }
 
    memset(&q, 0, sizeof(Question));
    memset(filled, 0, sizeof(filled));
 
    while(1){
        allFilled = filled[0] && filled[1] && filled[2] && filled[3] && filled[4] && filled[5] && filled[6];
 
        printf("\n--- Add new question ---\n");
 
        printf("1) Question text: ");
        if(filled[0]) printf("%s\n", q.question);
        else printf("(not set)\n");
 
        printf("2) Difficulty: ");
        if(filled[1]) printf("%d/10 (%s)\n", q.difficulty, getDifficultyCategory(q.difficulty));
        else printf("(not set)\n");
 
        for (i = 0; i < ANSWERS_COUNT; i++){
            printf("%d) Answer %c: ", 3 + i, 'A' + i);
            if (filled[2 + i]) printf("%s\n", q.answers[i]);
            else printf("(not set)\n");
        }
 
        printf("7) Correct answer: ");
        if (filled[6]) printf("%c\n", 'A' + q.correctAnswer);
        else printf("(not set)\n");
 
        if(allFilled) printf("8) Confirm and add\n");
        printf("0) Cancel\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();
 
        switch (choice) {
            case 1:
                printf("Question text: ");
                fgets(q.question, MAX_TEXT, stdin);
                stripNewline(q.question);
                filled[0] = 1;
                break;
            case 2:
                do{
                    printf("Difficulty (1-10): ");
                    scanf("%d", &q.difficulty);
                    getchar();
                }while(q.difficulty < 1 || q.difficulty > 10);
                filled[1] = 1;
                break;
            case 3: case 4: case 5: case 6:
                printf("Answer %c: ", 'A' + (choice - 3));
                fgets(q.answers[choice - 3], MAX_TEXT, stdin);
                stripNewline(q.answers[choice - 3]);
                filled[choice - 1] = 1;
                break;
            case 7:
                do{
                    printf("Correct answer (1-4): ");
                    scanf("%d", &q.correctAnswer);
                    getchar();
                }while(q.correctAnswer < 1 || q.correctAnswer > ANSWERS_COUNT);
                q.correctAnswer--;
                filled[6] = 1;
                break;
            case 8:
                if(allFilled) {
                    questions[questionCount] = q;
                    questionCount++;
                    printf("Question added successfully!\n");
                    return;
                }
                printf("Please fill in all fields first.\n");
                break;
            case 0:
                printf("Cancelled.\n");
                return;
            default:
                printf("Invalid choice! Please select a number from the menu.\n");
                break;
        }
    }
}

void editQuestion(){
    int i;
    int choice;
    int field;
    Question* q;
    char filter[MAX_TEXT] = "";
 
    if(questionCount == 0){
        printf("No questions to edit.\n");
        return;
    }
 
    printf("\n--- Edit question ---\n");

    printf("Enter search keyword to filter (or press Enter to show all): ");
    fgets(filter, sizeof(filter), stdin);
    stripNewline(filter);
 
    printf("\nAvailable questions:\n");
    for(i = 0; i < questionCount; i++){
        if(strlen(filter) > 0 && strstr(questions[i].question, filter) == NULL) {
            continue;
        }
        printf("  %d. [%d/10] %s\n", i + 1, questions[i].difficulty, questions[i].question);
    }

    do{
        printf("\nSelect question number to edit (1-%d): ", questionCount);
        scanf("%d", &choice);
        getchar();
    }while(choice < 1 || choice > questionCount);
 
    q = &questions[choice - 1];
 
    printf("\nWhich field do you want to change?\n");
    printf("1) Question text\n");
    printf("2) Difficulty\n");
    printf("3) Answer A\n");
    printf("4) Answer B\n");
    printf("5) Answer C\n");
    printf("6) Answer D\n");
    printf("7) Correct answer\n");
 
    do{
        printf("Choice (1-7): ");
        scanf("%d", &field);
        getchar();
    }while(field < 1 || field > 7);
 
    switch(field){
        case 1:
            printf("New text: ");
            fgets(q->question, MAX_TEXT, stdin);
            stripNewline(q->question);
            break;
        case 2:
            do{
                printf("New difficulty (1-10): ");
                scanf("%d", &q->difficulty);
                getchar();
            }while(q->difficulty < 1 || q->difficulty > 10);
            break;
        case 3: case 4: case 5: case 6:
            printf("New answer %c: ", 'A' + (field - 3));
            fgets(q->answers[field - 3], MAX_TEXT, stdin);
            stripNewline(q->answers[field - 3]);
            break;
        case 7:
            do{
                printf("New correct answer (1-4): ");
                scanf("%d", &q->correctAnswer);
                getchar();
            }while(q->correctAnswer < 1 || q->correctAnswer > ANSWERS_COUNT);
            q->correctAnswer--;
            break;
    }
 
    printf("Question edited successfully!\n");
}

void printQuestion(int index) {
    Question* q;
    int i;
 
    if (index < 0 || index >= questionCount){
        printf("Error: invalid question index.\n");
        return;
    }
 
    q = &questions[index];
 
    printf("\n========================================\n");
    printf("Question #%d | Difficulty: %d/10 (%s)\n", index + 1, q->difficulty, getDifficultyCategory(q->difficulty));
    printf("========================================\n");
    printf("%s\n\n", q->question);
 
    for (i = 0; i < ANSWERS_COUNT; i++){
        printf("  %c) %s\n", 'A' + i, q->answers[i]);
    }
    printf("========================================\n");
}

int saveQuestions(const char* filename){
    FILE* f;
    int i, j;
    char temp[MAX_TEXT];

    f = fopen(filename, "w");
    if(f == NULL){
        printf("Error: file '%s' cannot be opened for writing.\n", filename);
        return 0;
    }
    fprintf(f, "%d\n", questionCount);
    for(i = 0; i < questionCount; i++){
        strcpy(temp, questions[i].question);
        encryptText(temp);
        fprintf(f, "%s\n", temp);
        fprintf(f, "%d\n", questions[i].difficulty);
        for(j = 0; j < ANSWERS_COUNT; j++){
            strcpy(temp, questions[i].answers[j]);
            encryptText(temp);
            fprintf(f, "%s\n", temp);
        }
        fprintf(f, "%d\n", questions[i].correctAnswer);
    }

    fclose(f);
    printf("Saved %d questions to '%s'.\n",questionCount,filename);
    return 1;
}

int loadQuestions(const char* filename){
    FILE* f;
    int i, j;
    int savedCount;

    f = fopen(filename, "r");

    if(f == NULL){
        printf("Error: file '%s' cannot be opened for reading.\n", filename);
        return 0;
    }

    if(fscanf(f, "%d", &savedCount) != 1){
        printf("Error: Invalid file format.\n");
        fclose(f);
        return 0;
    }

    fgetc(f);

    if(savedCount > MAX_QUESTIONS){
        printf("Warning: File contains %d questions, but maximum allowed is %d.\n",
               savedCount,
               MAX_QUESTIONS);

        questionCount = MAX_QUESTIONS;
    }
    else{
        questionCount = savedCount;
    }

    for(i = 0; i < questionCount; i++){

        fgets(questions[i].question, MAX_TEXT, f);
        stripNewline(questions[i].question);
        decryptText(questions[i].question);

        fscanf(f, "%d", &questions[i].difficulty);
        fgetc(f);

        for(j = 0; j < ANSWERS_COUNT; j++){

            fgets(questions[i].answers[j], MAX_TEXT, f);
            stripNewline(questions[i].answers[j]);

            decryptText(questions[i].answers[j]);
        }

        fscanf(f, "%d", &questions[i].correctAnswer);
        fgetc(f);
    }

    fclose(f);

    printf("Successfully loaded %d questions from '%s'.\n",
           questionCount,
           filename);

    return 1;
}