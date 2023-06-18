#include <stdio.h>
#include <string.h>
#include <zip.h>

void explorerArchiveRepertoire(struct zip* archive, const char* cheminRepertoire)
{
    int numFichiers = zip_get_num_entries(archive, 0);
    printf("Nombre de fichiers dans le répertoire : %s - %d\n", cheminRepertoire, numFichiers);

    for (int i = 0; i < numFichiers; i++)
    {
        struct zip_stat fileStat;
        if (zip_stat_index(archive, i, 0, &fileStat) < 0)
        {
            printf("Erreur lors de la récupération des informations du fichier %d\n", i);
            continue;
        }

        const char* nomFichier = fileStat.name;

        // Vérifie si le nom du fichier commence par le chemin du répertoire
        if (strncmp(nomFichier, cheminRepertoire, strlen(cheminRepertoire)) == 0)
        {
            const char* nomRelatif = nomFichier + strlen(cheminRepertoire);

            // Vérifie si le nom relatif se termine par '/' pour identifier un répertoire
            if (nomRelatif[strlen(nomRelatif) - 1] == '/')
            {
                printf("Répertoire : %s\n", nomRelatif);
            }
            else
            {
                printf("Fichier : %s\n", nomRelatif);
            }
        }
    }
}
