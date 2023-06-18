#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

int bruteForceZipPassword(const char* archivePath, const char* charset, int maxLength) {
    int charsetLength = strlen(charset);
    int* indices = (int*)malloc(maxLength * sizeof(int));

    struct zip* archive = zip_open(archivePath, 0, 'r');
    if (!archive) {
        printf("Erreur lors de l'ouverture de l'archive.\n");
        return -1;
    }

    int foundPassword = 0;
    int passwordLength = 1;

    while (passwordLength <= maxLength) {
        int i;
        for (i = 0; i < passwordLength; i++) {
            indices[i] = 0;
        }

        char* password = (char*)malloc((passwordLength + 1) * sizeof(char));
        password[passwordLength] = '\0';

        while (1) {
            for (i = 0; i < passwordLength; i++) {
                password[i] = charset[indices[i]];
            }

            int result = zip_set_default_password(archive, password);
            if (result == 0) {
                printf("Mot de passe trouvé : %s\n", password);
                foundPassword = 1;
                break;
            }

            int carry = 1;
            for (i = passwordLength - 1; i >= 0; i--) {
                indices[i] += carry;
                if (indices[i] == charsetLength) {
                    indices[i] = 0;
                    carry = 1;
                } else {
                    carry = 0;
                    break;
                }
            }

            if (carry)
                break;
        }

        free(password);

        if (foundPassword)
            break;

        passwordLength++;
    }

    zip_close(archive);
    free(indices);

    if (!foundPassword) {
        printf("Mot de passe introuvable.\n");
        return -1;
    }

    return 0;
}
