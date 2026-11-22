/* Student Record Management System
   Console program, records are saved in students_anu.txt */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define FILE_NAME "students_anu.txt"

struct Student {
    int id;
    char name[50];
    int age;
    char course[30];
    float marks;
};

struct Student students[MAX_STUDENTS];
int count = 0;

/* ---------- input helpers ---------- */

void read_line(char *buf, int size)
{
    if (fgets(buf, size, stdin) == NULL) {
        printf("\nInput closed. Exiting.\n");
        exit(0);
    }
    /* if the line was too long, throw away the rest */
    if (strchr(buf, '\n') == NULL) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

int read_int(const char *prompt, int min, int max)
{
    char line[100];
    char *end;
    long value;

    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        value = strtol(line, &end, 10);
        if (end == line || *end != '\0') {
            printf("  Please enter a whole number.\n");
        } else if (value < min || value > max) {
            printf("  Number must be between %d and %d.\n", min, max);
        } else {
            return (int)value;
        }
    }
}

float read_marks(const char *prompt)
{
    char line[100];
    char *end;
    float value;

    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        value = strtof(line, &end);
        if (end == line || *end != '\0') {
            printf("  Please enter a number.\n");
        } else if (value < 0 || value > 100) {
            printf("  Marks must be between 0 and 100.\n");
        } else {
            return value;
        }
    }
}

/* text can't be empty and can't have '|' because that is used in the file */
void read_text(const char *prompt, char *dest, int size)
{
    char line[200];

    while (1) {
        printf("%s", prompt);
        read_line(line, sizeof(line));
        if (strlen(line) == 0) {
            printf("  This can't be empty.\n");
        } else if (strlen(line) >= (size_t)size) {
            printf("  Too long (max %d characters).\n", size - 1);
        } else if (strchr(line, '|') != NULL) {
            printf("  Please don't use the | character.\n");
        } else {
            strcpy(dest, line);
            return;
        }
    }
}

/* ---------- file handling ---------- */

void load_students(void)
{
    FILE *fp = fopen(FILE_NAME, "r");
    char line[200];
    struct Student s;

    if (fp == NULL) {
        return;   /* no file yet, start empty */
    }

    while (count < MAX_STUDENTS && fgets(line, sizeof(line), fp) != NULL) {
        if (sscanf(line, "%d|%49[^|]|%d|%29[^|]|%f",
                   &s.id, s.name, &s.age, s.course, &s.marks) == 5) {
            students[count] = s;
            count++;
        }
    }
    fclose(fp);
}

void save_students(void)
{
    FILE *fp = fopen(FILE_NAME, "w");
    int i;

    if (fp == NULL) {
        printf("Error: could not save to %s\n", FILE_NAME);
        return;
    }
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%d|%s|%.1f\n", students[i].id, students[i].name,
                students[i].age, students[i].course, students[i].marks);
    }
    fclose(fp);
}

/* ---------- main features ---------- */

/* returns the position of the student in the array, or -1 */
int find_student(int id)
{
    int i;
    for (i = 0; i < count; i++) {
        if (students[i].id == id) {
            return i;
        }
    }
    return -1;
}

void print_student(struct Student s)
{
    printf("ID: %d\n", s.id);
    printf("Name: %s\n", s.name);
    printf("Age: %d\n", s.age);
    printf("Course: %s\n", s.course);
    printf("Marks: %.1f\n", s.marks);
}

void add_student(void)
{
    struct Student s;

    if (count >= MAX_STUDENTS) {
        printf("The record list is full (%d students).\n", MAX_STUDENTS);
        return;
    }

    s.id = read_int("Enter ID: ", 1, 999999);
    if (find_student(s.id) != -1) {
        printf("A student with ID %d already exists.\n", s.id);
        return;
    }
    read_text("Enter name: ", s.name, sizeof(s.name));
    s.age = read_int("Enter age: ", 15, 100);
    read_text("Enter course: ", s.course, sizeof(s.course));
    s.marks = read_marks("Enter marks (0-100): ");

    students[count] = s;
    count++;
    save_students();
    printf("Student added.\n");
}

void search_student(void)
{
    int id = read_int("Enter ID to search: ", 1, 999999);
    int pos = find_student(id);

    if (pos == -1) {
        printf("No student found with ID %d.\n", id);
    } else {
        printf("\nStudent found:\n");
        print_student(students[pos]);
    }
}

void update_student(void)
{
    int id = read_int("Enter ID of the student to update: ", 1, 999999);
    int pos = find_student(id);
    int choice;

    if (pos == -1) {
        printf("No student found with ID %d.\n", id);
        return;
    }

    printf("\nCurrent details:\n");
    print_student(students[pos]);

    printf("\nWhat do you want to change?\n");
    printf("1. Name\n2. Age\n3. Course\n4. Marks\n");
    choice = read_int("Choice: ", 1, 4);

    if (choice == 1) {
        read_text("New name: ", students[pos].name, sizeof(students[pos].name));
    } else if (choice == 2) {
        students[pos].age = read_int("New age: ", 15, 100);
    } else if (choice == 3) {
        read_text("New course: ", students[pos].course, sizeof(students[pos].course));
    } else {
        students[pos].marks = read_marks("New marks (0-100): ");
    }

    save_students();
    printf("Record updated.\n");
}

void delete_student(void)
{
    int id = read_int("Enter ID of the student to delete: ", 1, 999999);
    int pos = find_student(id);
    char answer[10];
    int i;

    if (pos == -1) {
        printf("No student found with ID %d.\n", id);
        return;
    }

    printf("Delete %s (ID %d)? (y/n): ", students[pos].name, id);
    read_line(answer, sizeof(answer));
    if (answer[0] != 'y' && answer[0] != 'Y') {
        printf("Cancelled.\n");
        return;
    }

    /* move everything after it one step back */
    for (i = pos; i < count - 1; i++) {
        students[i] = students[i + 1];
    }
    count--;
    save_students();
    printf("Student deleted.\n");
}

void display_all(void)
{
    int i;

    if (count == 0) {
        printf("No records yet.\n");
        return;
    }

    printf("\n%-8s %-22s %-5s %-24s %-6s\n", "ID", "Name", "Age", "Course", "Marks");
    printf("-----------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) {
        printf("%-8d %-22s %-5d %-24s %-6.1f\n", students[i].id, students[i].name,
               students[i].age, students[i].course, students[i].marks);
    }
    printf("\nTotal students: %d\n", count);
}

int main(void)
{
    int choice;

    load_students();
    printf("=== Student Record System ===\n");
    printf("Loaded %d record(s) from file.\n", count);

    do {
        printf("\n1. Add student\n");
        printf("2. Search by ID\n");
        printf("3. Update student\n");
        printf("4. Delete student\n");
        printf("5. Display all students\n");
        printf("0. Exit\n");
        choice = read_int("Enter choice: ", 0, 5);

        switch (choice) {
            case 1: add_student(); break;
            case 2: search_student(); break;
            case 3: update_student(); break;
            case 4: delete_student(); break;
            case 5: display_all(); break;
            case 0: printf("Goodbye!\n"); break;
        }
    } while (choice != 0);

    return 0;
}
