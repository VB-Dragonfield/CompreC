#include <stdio.h>
#include <string.h>
#include <zip.h>

void lireContenuFichierArchive(struct zip* archive, const char* cheminFichier)
{
    struct zip_file* fichier = zip_fopen(archive, cheminFichier, 0); // Ouverture du fichier spécifié dans l'archive
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si l'ouverture du fichier échoue
        return;
    }

    char buffer[1024]; // Tampon pour lire les données du fichier dans l'archive
    int bytesRead;
    while ((bytesRead = zip_fread(fichier, buffer, sizeof(buffer))) > 0) // Lecture du contenu du fichier dans l'archive en blocs de taille fixe
    {
        fwrite(buffer, 1, bytesRead, stdout); // Écriture des données lues sur la sortie standard (console)
    }

    zip_fclose(fichier); // Fermeture du fichier dans l'archive
}
