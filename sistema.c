#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100
#define MAX_POINTS_PER_DAY 4

void storage_date(const char *username) {
    FILE *file = fopen("dados.txt", "a+");
    if (file == NULL) {
        perror("Falta criar o arquivo");
        return;
    }

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    char current_date[MAX_LEN];
    snprintf(current_date, sizeof(current_date), "%04d-%02d-%02d",
             local->tm_year + 1900,
             local->tm_mon + 1,
             local->tm_mday);

    char line[MAX_LEN];
    int points_today = 0;

    // Contar quantas batidas de ponto o usuário já fez hoje
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0 && line[strlen(username)] == ' ') {
            char *date = line + strlen(username) + 1;
            if (strncmp(date, current_date, strlen(current_date)) == 0) {
                points_today++;
            }
        }
    }

    if (points_today >= MAX_POINTS_PER_DAY) {
        printf("Você já atingiu o limite de batidas por hoje.\n");
    } else {
        fprintf(file, "%s %04d-%02d-%02d %02d:%02d:%02d\n",
                username,
                local->tm_year + 1900,
                local->tm_mon + 1,
                local->tm_mday,
                local->tm_hour,
                local->tm_min,
                local->tm_sec);

        printf("Ponto registrado para %s: %04d-%02d-%02d %02d:%02d:%02d\n",
               username,
               local->tm_year + 1900,
               local->tm_mon + 1,
               local->tm_mday,
               local->tm_hour,
               local->tm_min,
               local->tm_sec);
    }

    fclose(file);
}

void read_dates(const char *username) {
    FILE *file = fopen("dados.txt", "r");
    if (file == NULL) {
        perror("Falta criar o arquivo");
        return;
    }

    char line[MAX_LEN];
    printf("Pontos de %s:\n", username);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0 && line[strlen(username)] == ' ') {
            printf("%s", line + strlen(username) + 1); // Imprime a data e hora
        }
    }

    fclose(file);
}

int user_exists(const char *username, const char *password) {
    FILE *file = fopen("dados.txt", "r");
    if (file == NULL) {
        return 0;
    }

    char line[MAX_LEN];
    char user_pass[MAX_LEN];
    snprintf(user_pass, sizeof(user_pass), "%s:%s", username, password);

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, user_pass, strlen(user_pass)) == 0) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void register_user(const char *username, const char *password) {
    FILE *file = fopen("dados.txt", "a");
    if (file == NULL) {
        perror("Falta criar o arquivo");
        return;
    }

    fprintf(file, "%s:%s\n", username, password);
    fclose(file);
    printf("Usuário %s registrado com sucesso.\n", username);
}

int main() {
    char action;
    char username[MAX_LEN];
    char password[MAX_LEN];

    while (1) {
        printf(" '1' logar, '2' cadastrar, 'q' sair: ");
        action = getchar();
        getchar(); // Limpar o buffer do teclado

        if (action == '1') {
            printf("Nome de usuário: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0'; // Remover o newline

            printf("Senha: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0'; // Remover o newline

            if (user_exists(username, password)) {
                printf("Login bem-sucedido!\n");
                while (1) {
                    printf(" '1' bater ponto, '2' verificar pontos, 'q' sair: ");
                    action = getchar();
                    getchar(); // Limpar o buffer do teclado

                    if (action == '1') {
                        storage_date(username);
                    } else if (action == '2') {
                        read_dates(username);
                    } else if (action == 'q') {
                        break;
                    } else {
                        printf("Digite uma entrada válida.\n");
                    }
                }
            } else {
                printf("Usuário ou senha incorretos.\n");
            }
        } else if (action == '2') {
            printf("Nome de usuário: ");
            fgets(username, sizeof(username), stdin);
            username[strcspn(username, "\n")] = '\0'; // Remover o newline

            printf("Senha: ");
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0'; // Remover o newline

            register_user(username, password);
        } else if (action == 'q') {
            break;
        } else {
            printf("Digite uma entrada válida.\n");
        }
    }

    return 0;
}