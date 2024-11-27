#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define MAX_STRING_LENGTH 100

// Task structure
typedef struct {
    char date[MAX_STRING_LENGTH];
    char type[MAX_STRING_LENGTH];
    char title[MAX_STRING_LENGTH];
    char status[MAX_STRING_LENGTH]; // Task status: "Not Started", "In Progress", "Completed", "Archived"
} Task;

// Function prototypes
void loadTasksFromFile(Task tasks[], int* taskCount, const char* filename);
void saveTasksToFile(const Task tasks[], int taskCount, const char* filename);
void printTasks(const Task tasks[], int taskCount);
void updateTaskStatus(Task tasks[], int taskCount);

int main() {
    Task tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    // Load tasks from file
    loadTasksFromFile(tasks, &taskCount, "database.txt");

    while (1) {
        printf("\nTo-Do List\n");
        printf("1. Show Tasks\n");
        printf("2. Update Task Status\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);
        getchar(); // Consume the newline character

        switch (choice) {
        case 1:
            system("cls"); // Clear the screen (use "cls" for Windows)
            printTasks(tasks, taskCount);
            break;
        case 2:
            updateTaskStatus(tasks, taskCount);
            break;
        case 3:
            saveTasksToFile(tasks, taskCount, "database.txt");
            return 0;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

void loadTasksFromFile(Task tasks[], int* taskCount, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open file %s for reading.\n", filename);
        return;
    }

    fscanf(file, "%d\n", taskCount); // Read the total number of tasks

    for (int i = 0; i < *taskCount; i++) {
        fscanf(file, "%99[^,], %99[^,], %99[^,], %99[^\n]\n", tasks[i].date, tasks[i].type, tasks[i].title, tasks[i].status); // Read date, type, title, status
        int endMarker;
        fscanf(file, "%d\n", &endMarker); // Read the end marker (0)
        if (endMarker != 0) {
            printf("Error in file format. Expected end marker '0'.\n");
            fclose(file);
            return;
        }
    }

    fclose(file);
}

void saveTasksToFile(const Task tasks[], int taskCount, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open file %s for writing.\n", filename);
        return;
    }

    fprintf(file, "%d\n", taskCount); // Write the total number of tasks

    for (int i = 0; i < taskCount; i++) {
        fprintf(file, "%s, %s, %s, %s\n0\n", tasks[i].date, tasks[i].type, tasks[i].title, tasks[i].status); // Write date, type, title, status, and end marker
    }

    fclose(file);
}

void printTasks(const Task tasks[], int taskCount) {
    if (taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("\n%-15s %-15s %-30s %-15s\n", "Date", "Type", "Title", "Status");
    printf("-------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < taskCount; i++) {
        printf("%-15s %-15s %-30s %-15s\n", tasks[i].date, tasks[i].type, tasks[i].title, tasks[i].status);
    }
}

void updateTaskStatus(Task tasks[], int taskCount) {
    system("cls"); // Clear the screen (use "cls" for Windows)
    if (taskCount == 0) {
        printf("No tasks available to update.\n");
        return;
    }

    printTasks(tasks, taskCount);
    printf("\nEnter the task number to update the status (1 to %d): ", taskCount);
    int index;
    scanf("%d", &index);
    getchar(); // Consume the newline character

    if (index < 1 || index > taskCount) {
        printf("Invalid task number.\n");
        return;
    }

    printf("\nEnter new status (Not Started, In Progress, Completed, Archived): ");
    fgets(tasks[index - 1].status, MAX_STRING_LENGTH, stdin);
    tasks[index - 1].status[strcspn(tasks[index - 1].status, "\n")] = '\0'; // Remove newline

    printf("Task status updated successfully.\n");
    saveTasksToFile(tasks, taskCount, "database.txt");
}
