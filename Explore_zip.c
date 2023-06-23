#include <stdio.h>
#include <string.h>
#include <zip.h>

void explorerArchiveZip(const char* cheminArchive)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive ZIP spécifiée par le chemin
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        return;
    }

    int numFichiers = zip_get_num_entries(archive, 0); // Obtient le nombre de fichiers dans l'archive
    printf("Nombre de fichiers dans l'archive : %d\n", numFichiers); // Affiche le nombre de fichiers

    for (int i = 0; i < numFichiers; i++)
    {
        struct zip_stat fileStat; // Structure pour stocker les informations sur le fichier
        if (zip_stat_index(archive, i, 0, &fileStat) < 0) // Récupère les informations du fichier à l'index i
        {
            printf("Erreur lors de la récupération des informations du fichier %d\n", i); // Affiche une erreur si la récupération des informations échoue
            continue; // Passe à l'itération suivante
        }

        const char* nomFichier = fileStat.name; // Nom du fichier

        // Vérifie si le nom se termine par '/' pour identifier un répertoire
        if (nomFichier[strlen(nomFichier) - 1] == '/')
        {
            printf("Répertoire : %s\n", nomFichier); // Affiche le nom en tant que répertoire
        }
        else
        {
            printf("Fichier : %s\n", nomFichier); // Affiche le nom en tant que fichier
        }
    }

    zip_close(archive); // Ferme l'archive
}
