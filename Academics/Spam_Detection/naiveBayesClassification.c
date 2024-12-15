#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_EMAILS 100
#define MAX_WORD_LENGTH 50
#define SMALL_PROB 1e-6

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

typedef struct {
    WordCount words[MAX_WORDS];
    int totalWords;
    int emailCount;
} ClassData;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void tokenize(char *str, char words[MAX_WORDS][MAX_WORD_LENGTH], int *wordCount) {
    int i = 0, j = 0, k = 0;
    *wordCount = 0;
    while (str[i] != '\0') {
        if (isspace(str[i]) || ispunct(str[i])) {
            if (k > 0) {
                words[*wordCount][k] = '\0';
                toLowerCase(words[*wordCount]);
                (*wordCount)++;
                k = 0;
            }
        } else {
            if (k < MAX_WORD_LENGTH - 1) {
                words[*wordCount][k++] = str[i];
            }
        }
        i++;
    }
    if (k > 0) {
        words[*wordCount][k] = '\0';
        toLowerCase(words[*wordCount]);
        (*wordCount)++;
    }
}

int findWordIndex(WordCount *words, int wordCount, const char *word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void miniPrintClassData(ClassData *class) {
    printf("\n\tWORDS: ");
    for (int i = 0; i < class->totalWords; i++) {
        printf("%s (%d), ", class->words[i].word, class->words[i].count);
    }
    printf("\n\tTotal Words: %d, Total Emails: %d", class->totalWords, class->emailCount);
}

void printClassData(ClassData *spamData, ClassData *hamData) {
    printf("\n> Spam Data: ");
    miniPrintClassData(spamData);
    printf("\n> Ham Data: ");
    miniPrintClassData(hamData);
    printf("\n-------------------------------------------------------------------------");
}


void trainNaiveBayes(char emails[MAX_EMAILS][MAX_WORDS * MAX_WORD_LENGTH], char classifications[MAX_EMAILS][10], int emailCount, ClassData *spamData, ClassData *hamData) {
    for (int i = 0; i < emailCount; i++) {
        char words[MAX_WORDS][MAX_WORD_LENGTH];
        int wordCount;
        toLowerCase(emails[i]);
        tokenize(emails[i], words, &wordCount);

        ClassData *classData = (strcmp(classifications[i], "spam") == 0) ? spamData : hamData;
        classData->emailCount++;

        for (int j = 0; j < wordCount; j++) {
            int index = findWordIndex(classData->words, classData->totalWords, words[j]);
            if (index == -1) {
                index = classData->totalWords++;
                strncpy(classData->words[index].word, words[j], MAX_WORD_LENGTH);
                classData->words[index].word[MAX_WORD_LENGTH - 1] = '\0'; // Ensure null termination
                classData->words[index].count = 0;
            }
            classData->words[index].count++;
        }
    }
}

double calculateEachWordProbability(const char *word, ClassData *classData) {
    int index = findWordIndex(classData->words, classData->totalWords, word);
    int wordCount = (index != -1) ? classData->words[index].count : 0;
    //printf("\n word: %s word count: %d, total emails: %dprob: %f", word, wordCount, classData->emailCount,((double)wordCount / classData->emailCount));
    return ((double)wordCount / classData->emailCount);
}

double calculateClassProbability(char *email, ClassData *classData, ClassData *otherClassData) {
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    int wordCount;
    toLowerCase(email);
    tokenize(email, words, &wordCount);

    double classProb = (double)classData->emailCount / (classData->emailCount + otherClassData->emailCount);
    double priorWordProb = 1;


    for (int i = 0; i < wordCount; i++) {
        double wordProb = calculateEachWordProbability(words[i], classData);
        priorWordProb*=wordProb;
    }
    return classProb*priorWordProb;
}

const char* classifyEmail(char *email, ClassData *spamData, ClassData *hamData) {
    double spamProb = calculateClassProbability(email, spamData, hamData);
    double hamProb = calculateClassProbability(email, hamData, spamData);
    printf("Probabilities: %f(Spam) & %f(Ham)", spamProb, hamProb);

    return (spamProb > hamProb) ? "spam" : "ham";
}

int main() {
    char emails[MAX_EMAILS][MAX_WORDS * MAX_WORD_LENGTH];
    char classifications[MAX_EMAILS][10];
    int emailCount = 0;

    printf("Enter the number of training emails: ");
    scanf("%d", &emailCount);
    getchar(); // Consume newline

    for (int i = 0; i < emailCount; i++) {
        printf("Enter email %d: ", i + 1);
        fgets(emails[i], sizeof(emails[i]), stdin);
        emails[i][strcspn(emails[i], "\n")] = '\0'; // Remove newline

        printf("Enter classification (spam/ham) for email %d: ", i + 1);
        fgets(classifications[i], sizeof(classifications[i]), stdin);
        classifications[i][strcspn(classifications[i], "\n")] = '\0';
        toLowerCase(classifications[i]);
    }

    ClassData spamData = {0};
    ClassData hamData = {0};

    trainNaiveBayes(emails, classifications, emailCount, &spamData, &hamData);
    system("clear");
    printClassData(&spamData,&hamData);

    char email[MAX_WORDS * MAX_WORD_LENGTH];
    while (1) {
        printf("\nEnter an email to classify (or type 'exit' to quit): ");
        fgets(email, sizeof(email), stdin);
        email[strcspn(email, "\n")] = '\0';

        if (strcmp(email, "exit") == 0) {
            break;
        }

        const char *classification = classifyEmail(email, &spamData, &hamData);
        printf("\nThe email is classified as: %s\n", classification);
    }

    return 0;
}
