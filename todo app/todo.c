#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Hope it works, lazy to test

typedef struct {
    char name[100];
    char description[100];
    char date[11];
    bool is_done;
} Todo;

typedef struct {
    char name[100];
    Todo todos[100];
    int num_todos;
} Profile;

typedef struct {
    Profile profiles[100];
    int num_profiles;
} TodoApp;

TodoApp app;
int current_profile_index;

void clear_terminal() {
    system("clear");
}

void display_menu() {
    printf("========== TODO APP ==========\n");
    printf("1. Add Todo\n");
    printf("2. Update Todo\n");
    printf("3. Remove Todo\n");
    printf("4. Check Todo\n");
    printf("5. List Todos\n");
    printf("6. Search Todos\n");
    printf("7. Switch Profile\n");
    printf("8. Create Profile\n");
    printf("9. Remove Profile\n");
    printf("10. List Profiles\n");
    printf("0. Quit\n");
}

void add_todo() {
    char name[100];
    char description[100];
    char date[11];
    Todo todo;

    printf("Enter the name of the todo: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter the description of the todo: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';

    printf("Enter the due date (YYYY-MM-DD) of the todo: ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    strcpy(todo.name, name);
    strcpy(todo.description, description);
    strcpy(todo.date, date);
    todo.is_done = false;

    Profile *current_profile = &app.profiles[current_profile_index];
    current_profile->todos[current_profile->num_todos++] = todo;

    printf("Todo added successfully.\n");
    getchar(); // Consume newline character
}

void update_todo() {
    int index;
    char name[100];
    char description[100];
    char date[11];
    Todo todo;

    printf("Enter the index of the todo to update: ");
    scanf("%d", &index);
    getchar();  // Consume newline character

    if (index >= 0 && index < app.profiles[current_profile_index].num_todos) {
        printf("Enter the new name of the todo: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';

        printf("Enter the new description of the todo: ");
        fgets(description, sizeof(description), stdin);
        description[strcspn(description, "\n")] = '\0';

        printf("Enter the new due date (YYYY-MM-DD) of the todo: ");
        fgets(date, sizeof(date), stdin);
        date[strcspn(date, "\n")] = '\0';

        strcpy(todo.name, name);
        strcpy(todo.description, description);
        strcpy(todo.date, date);
        todo.is_done = false;

        Profile *current_profile = &app.profiles[current_profile_index];
        current_profile->todos[index] = todo;

        printf("Todo updated successfully.\n");
    } else {
        printf("Invalid index.\n");
    }
    getchar(); // Consume newline character
}

void remove_todo() {
    int index;

    printf("Enter the index of the todo to remove: ");
    scanf("%d", &index);
    getchar();  // Consume newline character

    Profile *current_profile = &app.profiles[current_profile_index];

    if (index >= 0 && index < current_profile->num_todos) {
        for (int i = index; i < current_profile->num_todos - 1; i++) {
            current_profile->todos[i] = current_profile->todos[i + 1];
        }
        current_profile->num_todos--;

        printf("Todo removed successfully.\n");
    } else {
        printf("Invalid index.\n");
    }
    getchar(); // Consume newline character
}

void check_todo() {
    int index;

    printf("Enter the index of the todo to check: ");
    scanf("%d", &index);
    getchar();  // Consume newline character

    Profile *current_profile = &app.profiles[current_profile_index];

    if (index >= 0 && index < current_profile->num_todos) {
        current_profile->todos[index].is_done = true;
        printf("Todo checked successfully.\n");
    } else {
        printf("Invalid index.\n");
    }
    getchar(); // Consume newline character
}

void order_todos_by_date() {
    Profile *current_profile = &app.profiles[current_profile_index];

    for (int i = 0; i < current_profile->num_todos - 1; i++) {
        for (int j = 0; j < current_profile->num_todos - i - 1; j++) {
            Todo *todo1 = &current_profile->todos[j];
            Todo *todo2 = &current_profile->todos[j + 1];

            // Convert date strings to time structs
            struct tm tm1 = {0};
            strptime(todo1->date, "%Y-%m-%d", &tm1);
            time_t time1 = mktime(&tm1);

            struct tm tm2 = {0};
            strptime(todo2->date, "%Y-%m-%d", &tm2);
            time_t time2 = mktime(&tm2);

            if (difftime(time1, time2) > 0) {
                Todo temp = *todo1;
                *todo1 = *todo2;
                *todo2 = temp;
            }
        }
    }
}

void list_todos() {
    Profile *current_profile = &app.profiles[current_profile_index];

    if (current_profile->num_todos > 0) {
        printf("========== TODOS ==========\n");
        for (int i = 0; i < current_profile->num_todos; i++) {
            Todo *todo = &current_profile->todos[i];
            char status[4];
            strcpy(status, todo->is_done ? "[✔️]" : "[X]");
            printf("%d. %s %s - %s\n", i, status, todo->name, todo->date);
        }
    } else {
        printf("No todos found.\n");
    }
    getchar(); // Consume newline character
}

void search_todos() {
    char keyword[100];
    int count = 0;
    Profile *current_profile = &app.profiles[current_profile_index];

    printf("Enter the keyword to search in todos: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    printf("========== SEARCH RESULTS ==========\n");
    for (int i = 0; i < current_profile->num_todos; i++) {
        Todo *todo = &current_profile->todos[i];
        if (strstr(todo->name, keyword) || strstr(todo->description, keyword)) {
            char status[4];
            strcpy(status, todo->is_done ? "[x]" : "[ ]");
            printf("%d. %s %s - %s\n", i, status, todo->name, todo->date);
            count++;
        }
    }

    if (count == 0) {
        printf("No results found.\n");
    }
    getchar(); // Consume newline character
}

void switch_profile() {
    int index;

    printf("Enter the index of the profile to switch: ");
    scanf("%d", &index);
    getchar();  // Consume newline character

    if (index >= 0 && index < app.num_profiles) {
        current_profile_index = index;
        printf("Switched to profile: %s\n", app.profiles[current_profile_index].name);
    } else {
        printf("Invalid index.\n");
    }
    getchar(); // Consume newline character
}

void create_profile() {
    char name[100];

    printf("Enter the name of the new profile: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    strcpy(app.profiles[app.num_profiles].name, name);
    app.num_profiles++;

    printf("Profile '%s' created successfully.\n", name);
    getchar(); // Consume newline character
}

void remove_profile() {
    int index;

    printf("Enter the index of the profile to remove: ");
    scanf("%d", &index);
    getchar();  // Consume newline character

    if (index >= 0 && index < app.num_profiles) {
        for (int i = index; i < app.num_profiles - 1; i++) {
            app.profiles[i] = app.profiles[i + 1];
        }
        app.num_profiles--;

        printf("Profile removed successfully.\n");
    } else {
        printf("Invalid index.\n");
    }
    getchar(); // Consume newline character
}

void list_profiles() {
    if (app.num_profiles > 0) {
        printf("========== PROFILES ==========\n");
        for (int i = 0; i < app.num_profiles; i++) {
            printf("%d. %s\n", i, app.profiles[i].name);
        }
    } else {
        printf("No profiles found.\n");
    }
    getchar(); // Consume newline character
}

void load_profiles() {
    FILE *file = fopen("profiles.txt", "r");
    if (file != NULL) {
        char line[500];
        while (fgets(line, sizeof(line), file) != NULL) {
            char name[100];
            sscanf(line, "%[^|]s", name);

            Profile profile;
            strcpy(profile.name, name);
            profile.num_todos = 0;

            while (fgets(line, sizeof(line), file) != NULL) {
                if (line[0] == '\n') {
                    break;
                }

                Todo todo;
                sscanf(line, "%[^|]|%[^|]|%[^|]|%d", todo.name, todo.description, todo.date, &todo.is_done);

                profile.todos[profile.num_todos++] = todo;
            }

            app.profiles[app.num_profiles++] = profile;
        }

        fclose(file);
    }
}

void save_profiles() {
    FILE *file = fopen("profiles.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < app.num_profiles; i++) {
            Profile *profile = &app.profiles[i];

            fprintf(file, "%s|\n", profile->name);

            for (int j = 0; j < profile->num_todos; j++) {
                Todo *todo = &profile->todos[j];
                fprintf(file, "%s|%s|%s|%d\n", todo->name, todo->description, todo->date, todo->is_done);
            }

            fprintf(file, "\n");
        }

        fclose(file);
    }
}

int main() {
    load_profiles();

    if (app.num_profiles > 0) {
        current_profile_index = 0;
    } else {
        strcpy(app.profiles[app.num_profiles].name, "DEFAULT");
        app.num_profiles++;
        current_profile_index = 0;
    }

    while (true) {
        clear_terminal();
        display_menu();
        printf("Enter your choice: ");

        char choice[2];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = '\0';

        clear_terminal();

        if (strcmp(choice, "1") == 0) {
            add_todo();
        } else if (strcmp(choice, "2") == 0) {
            update_todo();
        } else if (strcmp(choice, "3") == 0) {
            remove_todo();
        } else if (strcmp(choice, "4") == 0) {
            check_todo();
        } else if (strcmp(choice, "5") == 0) {
            order_todos_by_date();
            list_todos();
        } else if (strcmp(choice, "6") == 0) {
            search_todos();
        } else if (strcmp(choice, "7") == 0) {
            switch_profile();
        } else if (strcmp(choice, "8") == 0) {
            create_profile();
        } else if (strcmp(choice, "9") == 0) {
            remove_profile();
        } else if (strcmp(choice, "10") == 0) {
            list_profiles();
        } else if (strcmp(choice, "0") == 0) {
            break;
        } else {
            printf("Invalid choice.\n");
        }

        printf("Press Enter to continue...");
        getchar();  // Wait for user input
    }

    save_profiles();

    return 0;
}