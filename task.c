#include <stdio.h>
#include <string.h>

#define MAX_NAME 32
#define MAX_DEPT 50
#define MAX_GRADES 24

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char name[MAX_NAME];
    char facultyNumber[15];
    float averageGrade;
    int semester;
    Date enrollmentDate;
    char department[MAX_DEPT];
    int grades[MAX_GRADES];
    int gradesCount;
    int is_deleted;
} Student;

// --- ПРОВЕРКИ ---

int facultyNumberExists(char fn[]) {
    FILE *f = fopen("students.dat", "rb");
    if (!f) return 0;

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.facultyNumber, fn) == 0 && s.is_deleted == 0) {
            fclose(f);
            return 1;
        }
    }

    fclose(f);
    return 0;
}

int validName(char name[]) {
    int count = 0;
    int len = strlen(name);

    if (len == 0 || len > 30) return 0;

    for (int i = 0; i < len; i++) {
        if (name[i] == ' ') count++;
    }

    return count == 2;
}

int validDepartment(char dept[]) {
    if (strcmp(dept, "Обща") == 0) return 1;
    if (strcmp(dept, "СУ") == 0) return 1;
    if (strcmp(dept, "ИУТ") == 0) return 1;
    if (strcmp(dept, "ИИТ") == 0) return 1;
    if (strcmp(dept, "АЕЗ") == 0) return 1;
    if (strcmp(dept, "БЕ") == 0) return 1;
    return 0;
}

// --- СРЕДЕН УСПЕХ ---

Student calculateAverage(Student s) {
    float sum = 0;
    int count = 0;

    for (int i = 0; i < s.gradesCount; i++) {
        if (s.grades[i] >= 2 && s.grades[i] <= 6) {
            sum += s.grades[i];
            count++;
        }
    }

    if (count == 0) s.averageGrade = 0;
    else s.averageGrade = sum / count;

    return s;
}

// --- ВЪВЕЖДАНЕ ---

Student inputStudent() {
    Student s;
    int i, c;
    char temp[150];

    s.is_deleted = 0;

    // ИМЕ
    do {
        printf("Име (Име Презиме Фамилия, до 30 символа): ");
        scanf("%149[^\n]", temp);
        while ((c = getchar()) != '\n' && c != EOF);

        if (!validName(temp)) printf("Грешно име!\n");

    } while (!validName(temp));

    strcpy(s.name, temp);

    // ФН
    do {
        printf("ФН: ");
        scanf("%14s", temp);
        while ((c = getchar()) != '\n' && c != EOF);

        if (strlen(temp) == 0 || strlen(temp) > 10) {
            printf("Невалиден ФН!\n");
        } else if (facultyNumberExists(temp)) {
            printf("Този ФН вече съществува!\n");
            temp[0] = '\0';
        }

    } while (strlen(temp) == 0);

    strcpy(s.facultyNumber, temp);

    // СЕМЕСТЪР
    do {
        printf("Семестър (1-8): ");
        scanf("%d", &s.semester);
        while ((c = getchar()) != '\n' && c != EOF);
    } while (s.semester < 1 || s.semester > 8);

    // ДАТА
    do {
        printf("Дата (ДД.ММ.ГГГГ): ");
        scanf("%d.%d.%d", &s.enrollmentDate.day, &s.enrollmentDate.month, &s.enrollmentDate.year);
        while ((c = getchar()) != '\n' && c != EOF);
    } while (s.enrollmentDate.day < 1 || s.enrollmentDate.day > 31 ||
             s.enrollmentDate.month < 1 || s.enrollmentDate.month > 12 ||
             s.enrollmentDate.year < 1900 || s.enrollmentDate.year > 2100);

    // КАТЕДРА
    do {
        printf("Катедра (Обща, СУ, ИУТ, ИИТ, АЕЗ, БЕ): ");
        scanf("%49[^\n]", temp);
        while ((c = getchar()) != '\n' && c != EOF);

        if (!validDepartment(temp)) printf("Невалидна катедра!\n");

    } while (!validDepartment(temp));

    strcpy(s.department, temp);

    // ОЦЕНКИ
    int choice;
    do {
        printf("Добавяне на оценки сега? (1-да / 0-не): ");
        scanf("%d", &choice);
        while ((c = getchar()) != '\n' && c != EOF);
    } while (choice != 0 && choice != 1);

    if (choice == 1) {
        s.gradesCount = s.semester * 3;

        for (i = 0; i < s.gradesCount; i++) {
            do {
                printf("Оценка %d: ", i + 1);
                scanf("%d", &s.grades[i]);
                while ((c = getchar()) != '\n' && c != EOF);
            } while (s.grades[i] < 2 || s.grades[i] > 6);
        }
    } else {
        s.gradesCount = 0;
        printf("Студентът няма оценки!\n");
    }

    s = calculateAverage(s);
    return s;
}

// --- ПРИНТ ---

void printStudent(Student s) {
    if (s.is_deleted == 0) {
        printf("%s | ФН: %s | Катедра: %s | Сем: %d | Успех: %.2f",
               s.name, s.facultyNumber, s.department, s.semester, s.averageGrade);

        if (s.gradesCount == 0) printf(" | Няма оценки");

        printf("\n");
    }
}

// --- ОПЕРАЦИИ ---

void addStudent() {
    FILE *f = fopen("students.dat", "ab");
    if (!f) return;

    Student s = inputStudent();
    fwrite(&s, sizeof(Student), 1, f);

    fclose(f);
}

void showAll() {
    FILE *f = fopen("students.dat", "rb");
    if (!f) return;

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        printStudent(s);
    }

    fclose(f);
}

void showByDepartment() {
    FILE *f = fopen("students.dat", "rb");
    if (!f) return;

    char dept[MAX_DEPT];
    printf("Катедра: ");
    scanf("%49[^\n]", dept);

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.department, dept) == 0 && s.is_deleted == 0) {
            printStudent(s);
        }
    }

    fclose(f);
}

void searchByFN() {
    FILE *f = fopen("students.dat", "rb");
    if (!f) return;

    char fn[15];
    printf("ФН: ");
    scanf("%14s", fn);

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.facultyNumber, fn) == 0 && s.is_deleted == 0) {
            printStudent(s);
            fclose(f);
            return;
        }
    }

    printf("Не е намерен!\n");
    fclose(f);
}

void deleteStudent() {
    FILE *f = fopen("students.dat", "rb+");
    if (!f) return;

    char fn[15];
    printf("ФН: ");
    scanf("%14s", fn);

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.facultyNumber, fn) == 0 && s.is_deleted == 0) {
            s.is_deleted = 1;
            fseek(f, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, f);
            printf("Изтрит!\n");
            fclose(f);
            return;
        }
    }

    printf("Не е намерен!\n");
    fclose(f);
}

void changeSemester() {
    FILE *f = fopen("students.dat", "rb+");
    if (!f) return;

    char fn[15];
    printf("ФН: ");
    scanf("%14s", fn);

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.facultyNumber, fn) == 0 && s.is_deleted == 0) {

            int newSem, oldSem, c;

            do {
                printf("Нов семестър (1-8): ");
                scanf("%d", &newSem);
                while ((c = getchar()) != '\n' && c != EOF);
            } while (newSem < 1 || newSem > 8);

            oldSem = s.semester;

            // АКО НЯМА ОЦЕНКИ ДОСЕГА
            if (s.gradesCount == 0) {
                printf("Студентът няма оценки. Въведи всички.\n");

                s.semester = newSem;
                s.gradesCount = newSem * 3;

                for (int i = 0; i < s.gradesCount; i++) {
                    do {
                        printf("Оценка %d: ", i + 1);
                        scanf("%d", &s.grades[i]);
                        while ((c = getchar()) != '\n' && c != EOF);
                    } while (s.grades[i] < 2 || s.grades[i] > 6);
                }
            }
            else {
                // УВЕЛИЧАВАМЕ СЕМЕСТЪРА
                if (newSem > oldSem) {
                    int oldCount = oldSem * 3;
                    int newCount = newSem * 3;

                    printf("Въведи новите оценки:\n");

                    for (int i = oldCount; i < newCount; i++) {
                        do {
                            printf("Оценка %d: ", i + 1);
                            scanf("%d", &s.grades[i]);
                            while ((c = getchar()) != '\n' && c != EOF);
                        } while (s.grades[i] < 2 || s.grades[i] > 6);
                    }

                    s.gradesCount = newCount;
                    s.semester = newSem;
                }

                // НАМАЛЯВАМЕ СЕМЕСТЪРА
                else if (newSem < oldSem) {
                    s.semester = newSem;
                    s.gradesCount = newSem * 3;

                    printf("Излишните оценки са премахнати.\n");
                }
            }

            s = calculateAverage(s);

            fseek(f, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, f);

            printf("Семестърът е обновен!\n");
            fclose(f);
            return;
        }
    }

    printf("Не е намерен!\n");
    fclose(f);
}

void changeGrade() {
    FILE *f = fopen("students.dat", "rb+");
    if (!f) return;

    char fn[15];
    printf("ФН: ");
    scanf("%14s", fn);

    Student s;
    while (fread(&s, sizeof(Student), 1, f)) {
        if (strcmp(s.facultyNumber, fn) == 0 && s.is_deleted == 0) {

            int c;

            // АКО НЯМА ОЦЕНКИ -> СЪЗДАВАМЕ
            if (s.gradesCount == 0) {
                printf("Студентът няма оценки. Ще въведеш всички.\n");

                s.gradesCount = s.semester * 3;

                for (int i = 0; i < s.gradesCount; i++) {
                    do {
                        printf("Оценка %d: ", i + 1);
                        scanf("%d", &s.grades[i]);
                        while ((c = getchar()) != '\n' && c != EOF);
                    } while (s.grades[i] < 2 || s.grades[i] > 6);
                }
            }
            else {
                int index, newGrade;

                do {
                    printf("Номер на оценка (1-%d): ", s.gradesCount);
                    scanf("%d", &index);
                    while ((c = getchar()) != '\n' && c != EOF);
                } while (index < 1 || index > s.gradesCount);

                do {
                    printf("Нова оценка: ");
                    scanf("%d", &newGrade);
                    while ((c = getchar()) != '\n' && c != EOF);
                } while (newGrade < 2 || newGrade > 6);

                s.grades[index - 1] = newGrade;
            }

            s = calculateAverage(s);

            fseek(f, -sizeof(Student), SEEK_CUR);
            fwrite(&s, sizeof(Student), 1, f);

            printf("Оценките са обновени!\n");
            fclose(f);
            return;
        }
    }

    printf("Не е намерен!\n");
    fclose(f);
}

// --- MAIN ---

int main() {
    int choice;

    do {
        printf("\n1. Добави\n2. Всички\n3. По катедра\n4. По ФН\n5. Изтрий\n6. Смени семестър\n7. Смени оценка\n0. Изход\n");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addStudent(); break;
            case 2: showAll(); break;
            case 3: showByDepartment(); break;
            case 4: searchByFN(); break;
            case 5: deleteStudent(); break;
            case 6: changeSemester(); break;
            case 7: changeGrade(); break;
        }

    } while (choice != 0);

    return 0;
}