#include <stdio.h>
#include <string.h>

#include "question.h"

Question questions[MAX_QUESTIONS];
int questionCount = 0;

static const char* getDifficultyCategory(int difficulty) {
    if (difficulty <= 3) return "Easy";
    if (difficulty <= 6) return "Medium";
    return "Hard";
}
 
static void stripNewline(char* str) {
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void addQuestion(){
}

void editQuestion(){
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
}

int loadQuestions(const char* filename){
}