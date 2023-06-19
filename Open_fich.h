#include <stdio.h>
#include <string.h>
#include <zip.h>

void lireContenuFichierArchive(struct zip* archive, const char* cheminFichier)
{
    struct zip_file* fichier = zip_fopen(archive, cheminFichier, 0);
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier);
        return;
    }

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = zip_fread(fichier, buffer, sizeof(buffer))) > 0)
    {
        fwrite(buffer, 1, bytesRead, stdout);
    }

    zip_fclose(fichier);
}
