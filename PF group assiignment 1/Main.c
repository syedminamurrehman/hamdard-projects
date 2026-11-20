
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT 500
#define MAX_TESTS 100

struct TestResult {
    float wpm;
    float accuracy;
    int errors;
    double timeTaken;
};

char paragraphs[3][3][MAX_TEXT] = {
    {
        "C programming is a powerful language used to develop software.",
        "Practice makes a person better at every skill.",
        "Learning programming requires patience and consistency."
    },
    {
        "Programming is the process of creating instructions that computers can execute.",
        "Technology continues to change the way people communicate and work.",
        "A good programmer focuses on solving problems efficiently."
    },
    {
        "Computer science provides the foundation for modern technological innovation.",
        "Developing software requires logical thinking creativity and problem solving skills.",
        "Consistent practice improves programming knowledge and technical abilities."
    }
};

struct TestResult history[MAX_TESTS];
int testCount = 0;
float personalBest = 0;

void showMenu();
void startTest();
void showHistory();
void showBest();
void showInstructions();
void saveResult(struct TestResult result);

int selectDifficulty() {
    int choice;

    printf("\nSelect Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar();

    if (choice < 1 || choice > 3) {
        printf("Invalid choice! Defaulting to Easy.\n");
        choice = 1;
    }

    return choice - 1;
}

float calculateWPM(int characters, double seconds) {
    if (seconds <= 0)
        return 0;

    return (characters / 5.0) / (seconds / 60.0);
}

float calculateAccuracy(int correct, int total) {
    if (total == 0)
        return 0;

    return (correct * 100.0) / total;
}


int countErrors(char original[], char typed[]) {
    int errors = 0;
    int originalLength = strlen(original);
    int typedLength = strlen(typed);

    int maxLength = originalLength > typedLength
                    ? originalLength : typedLength;

    for (int i = 0; i < maxLength; i++) {

        char originalChar = (i < originalLength)
                            ? original[i] : '\0';

        char typedChar = (i < typedLength)
                         ? typed[i] : '\0';

        if (originalChar != typedChar) {
            errors++;
        }
    }

    return errors;
}
void startTest() {

    int difficulty = selectDifficulty();

    int randomIndex = rand() % 3;

    char *text = paragraphs[difficulty][randomIndex];

    char typed[MAX_TEXT];

    printf("\n====================================\n");
    printf("             TYPING TEST\n");
    printf("====================================\n");

    printf("\nType the following paragraph:\n\n");
    printf("%s\n\n", text);

    printf("Press ENTER when ready...");
    getchar();

    printf("\nStart typing:\n> ");

    time_t start = time(NULL);

    fgets(typed, MAX_TEXT, stdin);

    time_t end = time(NULL);

    double seconds = difftime(end, start);

    // Remove newline from input
    typed[strcspn(typed, "\n")] = '\0';

    int originalLength = strlen(text);
    int typedLength = strlen(typed);

    int errors = countErrors(text, typed);

    int correct = originalLength - errors;

    if (correct < 0)
        correct = 0;

    float wpm = calculateWPM(typedLength, seconds);
    float accuracy = calculateAccuracy(correct, originalLength);

    printf("\n====================================\n");
    printf("             TEST RESULTS\n");
    printf("====================================\n");

    printf("Time Taken     : %.0f seconds\n", seconds);
    printf("Characters     : %d\n", typedLength);
    printf("Correct Chars  : %d\n", correct);
    printf("Errors         : %d\n", errors);
    printf("Typing Speed   : %.2f WPM\n", wpm);
    printf("Accuracy       : %.2f%%\n", accuracy);

    if (wpm >= 50)
        printf("Performance    : EXCELLENT!\n");
    else if (wpm >= 30)
        printf("Performance    : GOOD!\n");
    else
        printf("Performance    : KEEP PRACTICING!\n");

    printf("====================================\n");

    struct TestResult result;

    result.wpm = wpm;
    result.accuracy = accuracy;
    result.errors = errors;
    result.timeTaken = seconds;

    if (testCount < MAX_TESTS) {
        history[testCount++] = result;
    }

    if (wpm > personalBest) {
        personalBest = wpm;
    }

    saveResult(result);
}

void showHistory() {

    printf("\n========== TEST HISTORY ==========\n");

    if (testCount == 0) {
        printf("No tests completed yet.\n");
        return;
    }

    for (int i = 0; i < testCount; i++) {
        printf("\nTest %d\n", i + 1);
        printf("WPM      : %.2f\n", history[i].wpm);
        printf("Accuracy : %.2f%%\n", history[i].accuracy);
        printf("Errors   : %d\n", history[i].errors);
    }
}

void showBest() {

    printf("\n========== PERSONAL BEST ==========\n");

    if (testCount == 0) {
        printf("No records available.\n");
    } else {
        printf("Your highest typing speed: %.2f WPM\n",
               personalBest);
    }
}

void showInstructions() {

    printf("\n========== INSTRUCTIONS ==========\n");

    printf("1. Select your difficulty level.\n");
    printf("2. Read the displayed paragraph.\n");
    printf("3. Press ENTER when ready.\n");
    printf("4. Type the paragraph as accurately as possible.\n");
    printf("5. Your WPM and accuracy will be calculated.\n");
    printf("6. Try to beat your personal best!\n");
}

void saveResult(struct TestResult result) {

    FILE *file = fopen("wpm_history.txt", "a");

    if (file == NULL) {
        printf("Unable to save result.\n");
        return;
    }

    fprintf(file,
            "WPM: %.2f | Accuracy: %.2f%% | Errors: %d | Time: %.0f sec\n",
            result.wpm,
            result.accuracy,
            result.errors,
            result.timeTaken);

    fclose(file);
}

void showMenu() {

    int choice=0;

    do {

        printf("\n\n====================================\n");
        printf("          WPM COUNTER\n");
        printf("       TYPING SPEED TESTER\n");
        printf("====================================\n");

        printf("1. Start Typing Test\n");
        printf("2. View Personal Best\n");
        printf("3. View Test History\n");
        printf("4. Instructions\n");
        printf("5. Exit\n");

        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            continue;
        }

        getchar();

        switch (choice) {

            case 1:
                startTest();
                break;

            case 2:
                showBest();
                break;

            case 3:
                showHistory();
                break;

            case 4:
                showInstructions();
                break;

            case 5:
                printf("\nThank you for using WPM Counter!\n");
                break;

            default:
                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 5);
}

// Main function
int main() {

    srand(time(NULL));

    printf("Welcome to WPM Counter!\n");

    showMenu();

    return 0;
}
