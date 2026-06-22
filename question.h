#ifndef QUESTION_H
#define QUESTION_H

#define MAX_TEXT 256
#define MAX_QUESTIONS 1000

typedef struct{
    char question[MAX_TEXT];
    int difficulty;
    char answers[4][MAX_TEXT];
    int correctAnswer;

} Question;

extern Question questions[MAX_QUESTIONS];
extern int questionCount;

void addQuestion();
void editQuestion();

int saveQuestions(const char* filename);
int loadQuestions(const char* filename);

#endif