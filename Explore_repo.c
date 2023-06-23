#include <stdio.h>
#include <string.h>
#include <zip.h>

void explorerArchiveRepertoire(struct zip* archive, const char* cheminRepertoire)
{
    int numFichiers = zip_get_num_entries(archive, 0); // Obtient le nombre de fichiers dans l'archive
    printf("Nombre de fichiers dans le répertoire : %s - %d\n", cheminRepertoire, numFichiers);

    for (int i = 0; i < numFichiers; i++)
    {
        struct zip_stat fileStat; // Structure pour stocker les informations sur le fichier
        if (zip_stat_index(archive, i, 0, &fileStat) < 0) // Récupère les informations du fichier à l'index i
        {
            printf("Erreur lors de la récupération des informations du fichier %d\n", i); // Affiche une erreur si la récupération des informations échoue
            continue; // Passe à l'itération suivante
        }

        const char* nomFichier = fileStat.name; // Nom du fichier

        // Vérifie si le nom du fichier commence par le chemin du répertoire
        if (strncmp(nomFichier, cheminRepertoire, strlen(cheminRepertoire)) == 0)
        {
            const char* nomRelatif = nomFichier + strlen(cheminRepertoire); // Nom relatif du fichier

            // Vérifie si le nom relatif se termine par '/' pour identifier un répertoire
            if (nomRelatif[strlen(nomRelatif) - 1] == '/')
            {
                printf("Répertoire : %s\n", nomRelatif); // Affiche le nom relatif en tant que répertoire
            }
            else
            {
                printf("Fichier : %s\n", nomRelatif); // Affiche le nom relatif en tant que fichier
            }
        }
    }
}
