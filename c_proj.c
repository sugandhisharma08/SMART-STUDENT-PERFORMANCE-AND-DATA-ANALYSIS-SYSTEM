#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 6
#define MAX_NAME_LEN 50
#define MAX_INT_ARRAY 200
#define MAX_SECT 100

/* Student structure */
struct Student {
    int roll;
    char name[MAX_NAME_LEN];
    int marks[MAX_SUBJECTS];
    int attendance; /* percent */
};

/* Global data */
struct Student students[MAX_STUDENTS];
int studentCount = 0;
int subjectCount = 0;

/* Function prototypes - CRUD */
void initializeData(void);
void addStudents(void);
void displayAllStudents(void);
void displayStudentByIndex(int idx);
void updateStudentMarksByRoll(void);
void deleteStudentByNumber(void);
void searchByPartialName(void);

/* Function prototypes - Performance */
int totalMarksForStudentIndex(int idx);
float averageForStudentIndex(int idx);
char gradeFromAverage(float avg);
void performanceReport(void);
void sortByAverageDesc(void);
void sortByNameAsc(void);
void showTopKPerformers(void);

/* Function prototypes - Merge & arrays */
void mergeAnotherSection(void);
void mergeIntegerArraysExcludingDuplicates(void);

/* Function prototypes - Small experiments */
void swapWithoutThirdVariableDemo(void);
void bitwiseOperationsDemo(void);
void armstrongNumbersDemo(void);

/* Function prototypes - Matrix operations */
void matrixOperationsMenu(void);
void readMatrix(int A[][MAX_SUBJECTS], int r, int c, char name[]);
void printMatrix(int A[][MAX_SUBJECTS], int r, int c);
void matrixAddition(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r, int c);
void matrixSubtraction(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r, int c);
void matrixMultiplication(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r1, int c1, int r2, int c2);
void matrixTranspose(int A[][MAX_SUBJECTS], int T[][MAX_SUBJECTS], int r, int c);

/* Menu */
void mainMenu(void);

/* Utility */
void readStringSimple(char prompt[], char dest[], int maxlen);
int readIntSimple(char prompt[], int min, int max);
int findStudentIndexByRoll(int roll);

/* Implementation */
/* Initialize student array */
void initializeData(void) {
    int i, j;
    studentCount = 0;
    for (i = 0; i < MAX_STUDENTS; ++i) {
        students[i].roll = 0;
        students[i].name[0] = '\0';
        students[i].attendance = 0;
        for (j = 0; j < MAX_SUBJECTS; ++j) students[i].marks[j] = 0;
    }
}

/* Read a simple string (single word) */
void readStringSimple(char prompt[], char dest[], int maxlen) {
    printf("%s", prompt);
    scanf("%s", dest); /* single-token input */
}

/* Read a simple integer with bounds */
int readIntSimple(char prompt[], int min, int max) {
    int x;
    printf("%s", prompt);
    while (scanf("%d", &x) != 1) {
        while (getchar() != '\n'); /* flush */
        printf("Invalid input. Try again: %s", prompt);
    }
    while (getchar() != '\n'); /* flush newline */
    if (min <= max) {
        if (x < min) x = min;
        if (x > max) x = max;
    }
    return x;
}

/* Add students */
void addStudents(void) {
    int n, i, j;
    if (studentCount >= MAX_STUDENTS) {
        printf("Student database is full.\n");
        return;
    }
    n = readIntSimple("How many students to add? ", 1, MAX_STUDENTS - studentCount);
    for (i = 0; i < n; ++i) {
        int idx = studentCount;
        students[idx].roll = readIntSimple("Enter roll number: ", 1, 1000000);
        readStringSimple("Enter name (no spaces): ", students[idx].name, MAX_NAME_LEN);
        students[idx].attendance = readIntSimple("Enter attendance (0-100): ", 0, 100);
        for (j = 0; j < subjectCount; ++j) {
            char prompt[80];
            sprintf(prompt, "Enter marks for subject %d (0-100): ", j+1);
            students[idx].marks[j] = readIntSimple(prompt, 0, 100);
        }
        studentCount++;
        printf("Student added. Total students: %d\n", studentCount);
    }
}

/* Display all students */
void displayAllStudents(void) {
    int i, j;
    if (studentCount == 0) {
        printf("No student records.\n");
        return;
    }
    printf("No  Roll   Name");
    for (j = 0; j < subjectCount; ++j) printf("  S%d", j+1);
    printf("  Att\n");
    for (i = 0; i < studentCount; ++i) {
        printf("%-3d %-5d %-10s", i+1, students[i].roll, students[i].name);
        for (j = 0; j < subjectCount; ++j) printf(" %-3d", students[i].marks[j]);
        printf("  %3d\n", students[i].attendance);
    }
}

/* Display student by index */
void displayStudentByIndex(int idx) {
    int j;
    if (idx < 0 || idx >= studentCount) {
        printf("Invalid index.\n");
        return;
    }
    printf("Roll: %d\n", students[idx].roll);
    printf("Name: %s\n", students[idx].name);
    printf("Attendance: %d\n", students[idx].attendance);
    printf("Marks: ");
    for (j = 0; j < subjectCount; ++j) printf("%d ", students[idx].marks[j]);
    printf("\n");
}

/* Update student marks by roll */
void updateStudentMarksByRoll(void) {
    int roll = readIntSimple("Enter roll to update: ", 1, 1000000);
    int idx = findStudentIndexByRoll(roll);
    if (idx == -1) {
        printf("Student with roll %d not found.\n", roll);
        return;
    }
    int j;
    for (j = 0; j < subjectCount; ++j) {
        char prompt[80];
        sprintf(prompt, "Enter new marks for subject %d (0-100): ", j+1);
        students[idx].marks[j] = readIntSimple(prompt, 0, 100);
    }
    printf("Marks updated for roll %d.\n", roll);
}

/* Delete student by number (1-based) */
void deleteStudentByNumber(void) {
    if (studentCount == 0) {
        printf("No students to delete.\n");
        return;
    }
    int num = readIntSimple("Enter student number to delete (1..): ", 1, studentCount);
    int idx = num - 1;
    int i, j;
    for (i = idx; i < studentCount - 1; ++i) {
        students[i] = students[i+1];
    }
    studentCount--;
    printf("Deleted student number %d.\n", num);
}

/* Search by partial name (simple substring using strstr) */
void searchByPartialName(void) {
    char query[MAX_NAME_LEN];
    printf("Enter partial name (case-sensitive): ");
    scanf("%s", query);
    int found = 0;
    int i;
    for (i = 0; i < studentCount; ++i) {
        if (strstr(students[i].name, query) != NULL) {
            printf("Found student %d: Roll=%d Name=%s\n", i+1, students[i].roll, students[i].name);
            found = 1;
        }
    }
    if (!found) printf("No match found.\n");
}

/* Total marks for student */
int totalMarksForStudentIndex(int idx) {
    int sum = 0;
    int j;
    for (j = 0; j < subjectCount; ++j) sum += students[idx].marks[j];
    return sum;
}

/* Average for student */
float averageForStudentIndex(int idx) {
    if (subjectCount == 0) return 0.0f;
    return (float)totalMarksForStudentIndex(idx) / subjectCount;
}

/* Grade from average */
char gradeFromAverage(float avg) {
    if (avg >= 90.0f) return 'A';
    if (avg >= 80.0f) return 'B';
    if (avg >= 70.0f) return 'C';
    if (avg >= 60.0f) return 'D';
    return 'F';
}
/* Performance report */
void performanceReport(void) {
    if (studentCount == 0) {
        printf("No records.\n");
        return;
    }
    int i;
    printf("No  Roll  Name    Total  Avg   Grade\n");
    for (i = 0; i < studentCount; ++i) {
        int tot = totalMarksForStudentIndex(i);
        float avg = averageForStudentIndex(i);
        char g = gradeFromAverage(avg);
        printf("%-3d %-5d %-7s %-5d %-5.2f %-3c\n", i+1, students[i].roll, students[i].name, tot, avg, g);
    }
}

/* Sort by average descending */
void sortByAverageDesc(void) {
    if (studentCount < 2) {
        printf("Not enough records to sort.\n");
        return;
    }
    int i, j;
    for (i = 0; i < studentCount - 1; ++i) {
        for (j = 0; j < studentCount - i - 1; ++j) {
            if (averageForStudentIndex(j+1) > averageForStudentIndex(j)) {
                struct Student temp = students[j];
                students[j] = students[j+1];
                students[j+1] = temp;
            }
        }
    }
    printf("Sorted by average (descending).\n");
}

/* Sort by name ascending */
void sortByNameAsc(void) {
    if (studentCount < 2) {
        printf("Not enough records to sort.\n");
        return;
    }
    int i, j;
    for (i = 0; i < studentCount - 1; ++i) {
        for (j = i + 1; j < studentCount; ++j) {
            if (strcmp(students[i].name, students[j].name) > 0) {
                struct Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    printf("Sorted by name (A-Z).\n");
}

/* Show top K performers */
void showTopKPerformers(void) {
    if (studentCount == 0) {
        printf("No students.\n");
        return;
    }
    int k = readIntSimple("Enter K (top K): ", 1, studentCount);
    int indices[MAX_STUDENTS];
    int i;
    for (i = 0; i < studentCount; ++i) indices[i] = i;
    for (i = 0; i < k; ++i) {
        int best = i, j;
        for (j = i+1; j < studentCount; ++j) {
            if (totalMarksForStudentIndex(indices[j]) > totalMarksForStudentIndex(indices[best])) best = j;
        }
        int tmp = indices[i]; indices[i] = indices[best]; indices[best] = tmp;
    }
    printf("Top %d performers:\n", k);
    for (i = 0; i < k; ++i) {
        int idx = indices[i];
        printf("%d) Roll=%d Name=%s Total=%d Avg=%.2f\n", i+1, students[idx].roll, students[idx].name,
               totalMarksForStudentIndex(idx), averageForStudentIndex(idx));
    }
}

/* Merge another section into main DB excluding duplicates by roll or name */
void mergeAnotherSection(void) {
    int m = readIntSimple("Enter number of students in other section: ", 1, MAX_SECT);
    if (studentCount + m > MAX_STUDENTS) {
        printf("Not enough space; can add at most %d students.\n", MAX_STUDENTS - studentCount);
        m = MAX_STUDENTS - studentCount;
    }
    int i, j;
    for (i = 0; i < m; ++i) {
        int r = readIntSimple("Enter roll: ", 1, 1000000);
        char nm[MAX_NAME_LEN];
        readStringSimple("Enter name (no spaces): ", nm, MAX_NAME_LEN);
        int att = readIntSimple("Enter attendance (0-100): ", 0, 100);
        int dup = 0;
        for (j = 0; j < studentCount; ++j) {
            if (students[j].roll == r || strcmp(students[j].name, nm) == 0) { dup = 1; break; }
        }
        if (dup) {
            printf("Duplicate found (roll or name). Skipping.\n");
            for (j = 0; j < subjectCount; ++j) { readIntSimple("Enter marks (ignored): ", 0, 100); }
            continue;
        }
        students[studentCount].roll = r;
        strcpy(students[studentCount].name, nm);
        students[studentCount].attendance = att;
        for (j = 0; j < subjectCount; ++j) {
            char p[80];
            sprintf(p, "Enter marks for subject %d (0-100): ", j+1);
            students[studentCount].marks[j] = readIntSimple(p, 0, 100);
        }
        studentCount++;
        printf("Added %s (roll %d).\n", nm, r);
    }
    printf("Merge complete. Total students: %d\n", studentCount);
}

/* Merge integer arrays excluding duplicates */
void mergeIntegerArraysExcludingDuplicates(void) {
    int n1 = readIntSimple("Enter size of first array: ", 0, MAX_INT_ARRAY);
    int a[MAX_INT_ARRAY];
    int i, j;
    for (i = 0; i < n1; ++i) {
        char p[40];
        sprintf(p, "a[%d]: ", i);
        a[i] = readIntSimple(p, -1000000, 1000000);
    }
    int n2 = readIntSimple("Enter size of second array: ", 0, MAX_INT_ARRAY);
    int b[MAX_INT_ARRAY];
    for (i = 0; i < n2; ++i) {
        char p[40];
        sprintf(p, "b[%d]: ", i);
        b[i] = readIntSimple(p, -1000000, 1000000);
    }
    int res[MAX_INT_ARRAY * 2];
    int rsize = 0;
    for (i = 0; i < n1; ++i) {
        int v = a[i], found = 0;
        for (j = 0; j < rsize; ++j) if (res[j] == v) { found = 1; break; }
        if (!found) res[rsize++] = v;
    }
    for (i = 0; i < n2; ++i) {
        int v = b[i], found = 0;
        for (j = 0; j < rsize; ++j) if (res[j] == v) { found = 1; break; }
        if (!found) res[rsize++] = v;
    }
    printf("Merged array excluding duplicates (size=%d):\n", rsize);
    for (i = 0; i < rsize; ++i) printf("%d ", res[i]);
    printf("\n");
}

/* Swap two integers without third variable (arithmetic method) */
void swapWithoutThirdVariableDemo(void) {
    int a = readIntSimple("Enter a: ", -1000000, 1000000);
    int b = readIntSimple("Enter b: ", -1000000, 1000000);
    printf("Before swap: a=%d b=%d\n", a, b);
    a = a + b;
    b = a - b;
    a = a - b;
    printf("After swap: a=%d b=%d\n", a, b);
}

/* Bitwise AND/OR/XOR demo and smallest among results */
void bitwiseOperationsDemo(void) {
    int x = readIntSimple("Enter first integer: ", -1000000, 1000000);
    int y = readIntSimple("Enter second integer: ", -1000000, 1000000);
    int andv = x & y;
    int orv = x | y;
    int xorv = x ^ y;
    printf("AND = %d, OR = %d, XOR = %d\n", andv, orv, xorv);
    int smallest = andv;
    if (orv < smallest) smallest = orv;
    if (xorv < smallest) smallest = xorv;
    printf("Smallest among AND/OR/XOR is %d\n", smallest);
}

/* Armstrong numbers 1..10000 */
int isArmstrong(int n) {
    int tmp = n, digits = 0, sum = 0;
    while (tmp > 0) { digits++; tmp /= 10; }
    if (digits == 0) digits = 1;
    tmp = n;
    while (tmp > 0) {
        int d = tmp % 10;
        int p = 1;
        int i;
        for (i = 0; i < digits; ++i) p *= d;
        sum += p;
        tmp /= 10;
    }
    return sum == n;
}

void armstrongNumbersDemo(void) {
    int i;
    printf("Armstrong numbers from 1 to 10000:\n");
    for (i = 1; i <= 10000; ++i) if (isArmstrong(i)) printf("%d ", i);
    printf("\n");
}

/* Matrix operations */

/* Read matrix r x c into A */
void readMatrix(int A[][MAX_SUBJECTS], int r, int c, char name[]) {
    int i, j;
    for (i = 0; i < r; ++i) {
        for (j = 0; j < c; ++j) {
            char p[80];
            sprintf(p, "%s[%d][%d]: ", name, i, j);
            A[i][j] = readIntSimple(p, -1000000, 1000000);
        }
    }
}

/* Print matrix */
void printMatrix(int A[][MAX_SUBJECTS], int r, int c) {
    int i, j;
    for (i = 0; i < r; ++i) {
        for (j = 0; j < c; ++j) printf("%8d", A[i][j]);
        printf("\n");
    }
}

/* Matrix add */
void matrixAddition(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r, int c) {
    int i, j;
    for (i = 0; i < r; ++i) for (j = 0; j < c; ++j) R[i][j] = A[i][j] + B[i][j];
}

/* Matrix subtract */
void matrixSubtraction(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r, int c) {
    int i, j;
    for (i = 0; i < r; ++i) for (j = 0; j < c; ++j) R[i][j] = A[i][j] - B[i][j];
}

/* Matrix multiply */
void matrixMultiplication(int A[][MAX_SUBJECTS], int B[][MAX_SUBJECTS], int R[][MAX_SUBJECTS], int r1, int c1, int r2, int c2) {
    int i, j, k;
    for (i = 0; i < r1; ++i) {
        for (j = 0; j < c2; ++j) {
            R[i][j] = 0;
            for (k = 0; k < c1; ++k) R[i][j] += A[i][k] * B[k][j];
        }
    }
}

/* Matrix transpose */
void matrixTranspose(int A[][MAX_SUBJECTS], int T[][MAX_SUBJECTS], int r, int c) {
    int i, j;
    for (i = 0; i < r; ++i) for (j = 0; j < c; ++j) T[j][i] = A[i][j];
}

/* Matrix operations menu */
void matrixOperationsMenu(void) {
    int choice = readIntSimple("Matrix Menu: 1:Add 2:Sub 3:Mul 4:Transpose 5:Back. Enter: ", 1, 5);
    if (choice == 5) return;
    int A[MAX_SUBJECTS][MAX_SUBJECTS], B[MAX_SUBJECTS][MAX_SUBJECTS], R[MAX_SUBJECTS][MAX_SUBJECTS];
    int r1, c1, r2, c2;
    if (choice == 1 || choice == 2 || choice == 3) {
        r1 = readIntSimple("Rows of A: ", 1, MAX_SUBJECTS);
        c1 = readIntSimple("Cols of A: ", 1, MAX_SUBJECTS);
        r2 = readIntSimple("Rows of B: ", 1, MAX_SUBJECTS);
        c2 = readIntSimple("Cols of B: ", 1, MAX_SUBJECTS);
        if ((choice == 1 || choice == 2) && (r1 != r2 || c1 != c2)) {
            printf("For add/sub, dimensions must match.\n");
            return;
        }
        if (choice == 3 && c1 != r2) {
            printf("For multiplication, cols of A must equal rows of B.\n");
            return;
        }
        readMatrix(A, r1, c1, "A");
        readMatrix(B, r2, c2, "B");
        if (choice == 1) {
            matrixAddition(A, B, R, r1, c1);
            printf("Result (A + B):\n"); printMatrix(R, r1, c1);
        } else if (choice == 2) {
            matrixSubtraction(A, B, R, r1, c1);
            printf("Result (A - B):\n"); printMatrix(R, r1, c1);
        } else {
            matrixMultiplication(A, B, R, r1, c1, r2, c2);
            printf("Result (A x B):\n"); printMatrix(R, r1, c2);
        }
    } else if (choice == 4) {
        r1 = readIntSimple("Rows: ", 1, MAX_SUBJECTS);
        c1 = readIntSimple("Cols: ", 1, MAX_SUBJECTS);
        readMatrix(A, r1, c1, "A");
        matrixTranspose(A, R, r1, c1);
        printf("Transpose:\n"); printMatrix(R, c1, r1);
    } else {
        printf("Invalid matrix choice.\n");
    }
}

/* Find student index by roll */
int findStudentIndexByRoll(int roll) {
    int i;
    for (i = 0; i < studentCount; ++i) if (students[i].roll == roll) return i;
    return -1;
}
/* Main menu */
void mainMenu(void) {
    initializeData();
    subjectCount = readIntSimple("Enter number of subjects (1..6): ", 1, MAX_SUBJECTS);
    int choice = 0;
    do {
        printf("\nMain Menu\n");
        printf("1. Add Students\n");
        printf("2. Display All Students\n");
        printf("3. Update Student Marks (by roll)\n");
        printf("4. Delete Student (by number)\n");
        printf("5. Search by Partial Name\n");
        printf("6. Performance Report\n");
        printf("7. Sort by Average (desc)\n");
        printf("8. Sort by Name (asc)\n");
        printf("9. Show Top K Performers\n");
        printf("10. Merge Another Section\n");
        printf("11. Merge Integer Arrays (exclude duplicates)\n");
        printf("12. Swap Without Third Variable Demo\n");
        printf("13. Bitwise AND/OR/XOR Demo\n");
        printf("14. Armstrong Numbers 1..10000\n");
        printf("15. Matrix Operations\n");
        printf("16. Exit\n");
        choice = readIntSimple("Enter choice: ", 1, 16);
        switch (choice) {
            case 1: addStudents(); break;
            case 2: displayAllStudents(); break;
            case 3: updateStudentMarksByRoll(); break;
            case 4: deleteStudentByNumber(); break;
            case 5: searchByPartialName(); break;
            case 6: performanceReport(); break;
            case 7: sortByAverageDesc(); break;
            case 8: sortByNameAsc(); break;
            case 9: showTopKPerformers(); break;
            case 10: mergeAnotherSection(); break;
            case 11: mergeIntegerArraysExcludingDuplicates(); break;
            case 12: swapWithoutThirdVariableDemo(); break;
            case 13: bitwiseOperationsDemo(); break;
            case 14: armstrongNumbersDemo(); break;
            case 15: matrixOperationsMenu(); break;
            case 16: printf("Exiting program.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 16);
}
/* Program entry */
int main(void) {
    mainMenu();
    return 0;
}
