#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

int bruteForceZipPassword(const char* archivePath, const char* charset, int maxLength) {
    int charsetLength = strlen(charset); // Longueur de l'ensemble de caractères possibles pour le mot de passe
    int* indices = (int*)malloc(maxLength * sizeof(int)); // Tableau d'indices pour générer les combinaisons de caractères

    struct zip* archive = zip_open(archivePath, 0, 'r'); // Ouverture de l'archive en mode lecture seule
    if (!archive) {
        printf("Erreur lors de l'ouverture de l'archive.\n"); // Affiche une erreur si l'ouverture de l'archive échoue
        return -1;
    }

    int foundPassword = 0; // Indicateur indiquant si le mot de passe a été trouvé
    int passwordLength = 1; // Longueur initiale du mot de passe à tester

    while (passwordLength <= maxLength) { // Boucle pour tester les mots de passe de différentes longueurs
        int i;
        for (i = 0; i < passwordLength; i++) {
            indices[i] = 0; // Initialise tous les indices à 0 pour générer la première combinaison
        }

        char* password = (char*)malloc((passwordLength + 1) * sizeof(char)); // Alloue de la mémoire pour le mot de passe à tester
        password[passwordLength] = '\0'; // Ajoute le caractère de fin de chaîne

        while (1) { // Boucle pour générer et tester toutes les combinaisons possibles de caractères
            for (i = 0; i < passwordLength; i++) {
                password[i] = charset[indices[i]]; // Construit le mot de passe en utilisant les caractères correspondants aux indices
            }

            int result = zip_set_default_password(archive, password); // Définit le mot de passe par défaut pour l'archive
            if (result == 0) {
                printf("Mot de passe trouvé : %s\n", password); // Affiche le mot de passe trouvé
                foundPassword = 1;
                break;
            }

            int carry = 1;
            for (i = passwordLength - 1; i >= 0; i--) {
                indices[i] += carry; // Incrémente l'indice actuel
                if (indices[i] == charsetLength) { // Vérifie si l'indice dépasse la taille de l'ensemble de caractères
                    indices[i] = 0; // Réinitialise l'indice à 0
                    carry = 1; // Propage la retenue
                } else {
                    carry = 0; // Pas de retenue, arrête l'incrémentation
                    break;
                }
            }

            if (carry)
                break; // Si la retenue est propagée jusqu'à la première position, toutes les combinaisons ont été testées

        }

        free(password); // Libère la mémoire allouée pour le mot de passe

        if (foundPassword)
            break; // Si le mot de passe a été trouvé, arrête la boucle

        passwordLength++; // Incrémente la longueur du mot de passe à tester
    }

    zip_close(archive); // Ferme l'archive
    free(indices); // Libère la mémoire allouée pour les indices

    if (!foundPassword) {
        printf("Mot de passe introuvable.\n"); // Affiche un message si le mot de passe n'a pas été trouvé
        return -1;
    }

    return 0; // Retourne 0 pour indiquer que le processus s'est terminé avec succès
}
