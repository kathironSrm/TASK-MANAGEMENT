#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_TASKS 100
#define MAX_THREADS 5
struct Task {
 char description[100];
 int priority;
 int completed;
};
struct Task tasks[MAX_TASKS];
int taskCount = 0;
pthread_t threads[MAX_THREADS];
pthread_mutex_t mutex;
void* taskRunner(void* arg) {
 int taskIndex = (int)arg;
 printf("Task %d started: %s\n", taskIndex + 1, tasks[taskIndex].description);
 // Simulate task execution (sleep for a random duration)
 int sleepDuration = rand() % 5 + 1;
 sleep(sleepDuration);
 printf("Task %d completed: %s\n", taskIndex + 1, tasks[taskIndex].description);
 tasks[taskIndex].completed = 1;
 return NULL;
}
void addTask() {
 if (taskCount < MAX_TASKS) {
 struct Task newTask;
 printf("Enter task description: ");
 scanf(" %[^\n]", newTask.description);
 printf("Enter task priority (1-5): ");
 scanf("%d", &newTask.priority);
 newTask.completed = 0;
 tasks[taskCount] = newTask;
 taskCount++;
 printf("Task added successfully.\n");
 } else {
 printf("Task manager is full. Cannot add more tasks.\n");
 }
}
void listTasks() {
 printf("Task List:\n");
 for (int i = 0; i < taskCount; i++) {
 printf("Task %d: %s (Priority: %d, Completed: %s)\n", i + 1,
tasks[i].description, tasks[i].priority, tasks[i].completed ? "Yes" : "No");
 }
}
void deleteTask() {
 int taskIndex;
 printf("Enter the task number to delete: ");
 scanf("%d", &taskIndex);
 if (taskIndex >= 1 && taskIndex <= taskCount) {
 for (int i = taskIndex - 1; i < taskCount - 1; i++) {
 tasks[i] = tasks[i + 1];
 }
 taskCount--;
 printf("Task deleted successfully.\n");
 } else {
 printf("Invalid task number.\n");
 }
}
void executeTasks() {
 int threadIndexes[MAX_THREADS];
 for (int i = 0; i < MAX_THREADS; i++) {
 threadIndexes[i] = i;
 }
 for (int i = 0; i < taskCount && i < MAX_THREADS; i++) {
 pthread_create(&threads[i], NULL, taskRunner, &threadIndexes[i]);
 }
 for (int i = 0; i < taskCount && i < MAX_THREADS; i++) {
 pthread_join(threads[i], NULL);
 }
}
int main() {
 int choice;
 pthread_mutex_init(&mutex, NULL);
 while (1) {
 printf("\nTask Manager Menu:\n");
 printf("1. Add Task\n");
 printf("2. List Tasks\n");
 printf("3. Delete Task\n");
 printf("4. Execute Tasks\n");
 printf("5. Exit\n");
 printf("Enter your choice: ");
 scanf("%d", &choice);
 switch (choice) {
 case 1:
 pthread_mutex_lock(&mutex);
 addTask();
 pthread_mutex_unlock(&mutex);
 break;
 case 2:
 pthread_mutex_lock(&mutex);
 listTasks();
 pthread_mutex_unlock(&mutex);
 break;
 case 3:
 pthread_mutex_lock(&mutex);
 deleteTask();
 pthread_mutex_unlock(&mutex);
 break;
 case 4:
 pthread_mutex_lock(&mutex);
 executeTasks();
 pthread_mutex_unlock(&mutex);
 break;
 case 5:
 pthread_mutex_destroy(&mutex);
 printf("Goodbye!\n");
 exit(0);
 default:
 printf("Invalid choice. Please try again.\n");
 }
 }
 return 0;
}