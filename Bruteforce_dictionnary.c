#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

int bruteForceZipPasswordDictionnary(const char* archivePath, const char* charset, int maxLength, const char* dictionaryPath) {
    int charsetLength = strlen(charset);
    int* indices = (int*)malloc(maxLength * sizeof(int));

    struct zip* archive = zip_open(archivePath, 0, 'r');
    if (!archive) {
        printf("Erreur lors de l'ouverture de l'archive.\n");
        return -1;
    }

    FILE* dictionary = fopen(dictionaryPath, "r");
    if (!dictionary) {
        printf("Erreur lors de l'ouverture du dictionnaire.\n");
        zip_close(archive);
        return -1;
    }

    int foundPassword = 0;
    char password[256];

    while (fgets(password, sizeof(password), dictionary)) {
        int passwordLength = strlen(password);
        if (password[passwordLength - 1] == '\n')
            password[--passwordLength] = '\0';

        if (passwordLength <= maxLength) {
            int result = zip_set_default_password(archive, password);
            if (result == 0) {
                printf("Mot de passe trouvé : %s\n", password);
                foundPassword = 1;
                break;
            }
        }
    }

    fclose(dictionary);
    zip_close(archive);
    free(indices);

    if (!foundPassword) {
        printf("Mot de passe introuvable.\n");
        return -1;
    }

    return 0;
}
