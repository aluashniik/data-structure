#include <stdio.h>
#include <stdlib.h>

int my_strcmp(const char *str1, const char *str2) {
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return *str1 - *str2;
        }
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

typedef struct {
    char fname[20];
    char lname[20];
} Student;

int compare(const void *a, const void *b) {
    const Student *std1 = (const Student *)a;
    const Student *std2 = (const Student *)b;

    int lname_cmp = my_strcmp(std1->lname, std2->lname);
    if (lname_cmp != 0) {
        return lname_cmp;
    } else {
        return my_strcmp(std1->fname, std2->fname);
    }
}

void my_sort(Student *students, int numstd) {
    for (int i = 0; i < numstd - 1; i++) {
        for (int j = i + 1; j < numstd; j++) {
            int lname_cmp = my_strcmp(students[i].lname, students[j].lname);

            if (lname_cmp > 0 || (lname_cmp == 0 && my_strcmp(students[i].fname, students[j].fname) > 0)) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

int main() {
    FILE *file = fopen("student.txt", "r");
    if (file == NULL) {
        perror("Could not open file");
        return 1;
    }

    int numstd;
    fscanf(file, "%d", &numstd);

    Student *students = (Student *)malloc(numstd * sizeof(Student));
    if (students == NULL) {
        perror("Memory allocation error");
        fclose(file);
        return 1;
    }

    for (int i = 0; i < numstd; i++) {
        fscanf(file, "%s %s", students[i].lname, students[i].fname);
    }

    fclose(file);

    my_sort(students, numstd);

    for (int i = 0; i < numstd; i++) {
        printf("%s %s\n", students[i].lname, students[i].fname);
    }

    free(students);

    return 0;
}
