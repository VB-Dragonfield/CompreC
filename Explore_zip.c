#include <stdio.h>
#include <string.h>
#include <zip.h>

void explorerArchiveZip(const char* cheminArchive)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL);
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive);
        return;
    }

    int numFichiers = zip_get_num_entries(archive, 0);
    printf("Nombre de fichiers dans l'archive : %d\n", numFichiers);

    for (int i = 0; i < numFichiers; i++)
    {
        struct zip_stat fileStat;
        if (zip_stat_index(archive, i, 0, &fileStat) < 0)
        {
            printf("Erreur lors de la récupération des informations du fichier %d\n", i);
            continue;
        }

        const char* nomFichier = fileStat.name;

        // Vérifie si le nom se termine par '/' pour identifier un répertoire
        if (nomFichier[strlen(nomFichier) - 1] == '/')
        {
            printf("Répertoire : %s\n", nomFichier);
        }
        else
        {
            printf("Fichier : %s\n", nomFichier);
        }
    }

    zip_close(archive);
}
