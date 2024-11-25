#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LEN 100

void storage_date(const char *username) {
    FILE *file = fopen("dados.txt", "a");
    if (file == NULL) {
        perror("Falta criar o arquivo");
        return;
    }

    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    fprintf(file, "%s\n%04d-%02d-%02d %02d:%02d:%02d\n",
            username,
            local->tm_year + 1900,
            local->tm_mon + 1,
            local->tm_mday,
            local->tm_hour,
            local->tm_min,
            local->tm_sec);

    fclose(file);
    printf("Ponto registrado para %s: %04d-%02d-%02d %02d:%02d:%02d\n",
           username,
           local->tm_year + 1900,
           local->tm_mon + 1,
           local->tm_mday,
           local->tm_hour,
           local->tm_min,
           local->tm_sec);
}

void read_dates(const char *username) {
    FILE *file = fopen("dados.txt", "r");
    if (file == NULL) {
        perror("Falta criar o arquivo");
        return;
    }

    char line[MAX_LEN];
    int user_found = 0;
    printf("Pontos de %s:\n", username);
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0 && line[strlen(username)] == '\n') {
            user_found = 1;
            continue;
        }
        if (user_found && line[0] != '\n' && line[0] != '\0') {
            printf("%s", line);
        }
        if (user_found && (line[0] == '\n' || line[0] == '\0')) {
            break;
        }
    }

    if (!user_found) {
        printf("Nenhum ponto encontrado para %s.\n", username);
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
}
   