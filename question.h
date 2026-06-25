#ifndef QUESTION_H
#define QUESTION_H

#define MAX_TEXT 256
#define MAX_QUESTIONS 1000

#define ANSWERS_COUNT 4

typedef struct{
    char question[MAX_TEXT];
    int difficulty;
    char answers[ANSWERS_COUNT][MAX_TEXT];
    int correctAnswer;
} Question;

extern Question questions[MAX_QUESTIONS];
extern int questionCount;

void addQuestion();
void editQuestion();
void showQuestions();
void printQuestion(int index);

int saveQuestions(const char* filename);
int loadQuestions(const char* filename);

#endif