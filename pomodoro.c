
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

bool Compiling = true;
int Time;
int Sessions;
int session;
char Decision;
const int BarWidth = 20;
time_t totalWorkingTime = 0;
char content[100];

void ReadDataFromFile() {
    FILE *file = fopen("TOF.txt", "r");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    fgets(content, sizeof(content), file);
    fclose(file);
}

void AddDataToFile(int totalWorkingTime) {
    ReadDataFromFile();
    FILE *file = fopen("TOF.txt", "w+");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    int LastVal = atoi(content);
    int SumTOF = LastVal + totalWorkingTime;
    fprintf(file, "%d", SumTOF/60);
    fclose(file);
}

void ProgressBar(int percentage) {
    printf("[");
    for (int i = 0; i < BarWidth; i++) {
        if (i < percentage * BarWidth / 100) {
            printf("#");
        } else {
            printf("_");
        }
    }
    printf("] %d%%\r", percentage);
    fflush(stdout);
}

void clearScreenWindows() {
    system("cls");
}

void clearScreenLinux() {
    system("clear");
}

void SetTime() {
    printf("Set how many sessions you want to make: ");
    scanf("%d", &Sessions);

    printf("For how long you want to study in one session (in minutes)?: ");
    scanf("%d", &Time);
    Time *= 60;  // Convert to seconds
}

void Break() {
    printf("5 minutes of break\nJust try to relax\n");
    sleep(300);  // 5 minutes in seconds
    printf("Time to get back to work\n");
}

void Working() {
    time_t startTime, endTime;
    for (session = 1; session <= Sessions; session++) {
        startTime = time(NULL);

        printf("Session %d: \n", session);

        for (int counting = 1; counting <= Time; counting++) {
            int percentage = counting * 100 / Time;  // Calculate percentage
            ProgressBar(percentage);
            fflush(stdout);
            sleep(1);
        }

        endTime = time(NULL);
        totalWorkingTime += (endTime - startTime);

        printf("\nSession %d ended\n", session);
        AddDataToFile(totalWorkingTime);
        sleep(3);
        clearScreenWindows();
        clearScreenLinux();

        if (session != Sessions) {
            Break();
            clearScreenWindows();
            clearScreenLinux();
        } else {
            printf("Well done!\n");
            printf("Your total working time is: %ld minutes\n", totalWorkingTime / 60);

        }
    }

}

int main() {
    printf("Hello, welcome to pomodoro session\n");

    while (Compiling) {
        printf("What do you want to do? (W -> work, Q -> quit, S -> show how many hours you've been working in total): ");
        scanf(" %c", &Decision);
        Decision = toupper(Decision);

        switch (Decision) {
            case 'W':
                SetTime();
                Working();
                break;
            case 'Q':
                Compiling = false;
                break;
            case 'S':
                ReadDataFromFile();
                printf("Total time of work: %s minutes\n", content);
                break;
            default:
                printf("Invalid choice. Please choose again.\n");
        }
    }

    return 0;
}
