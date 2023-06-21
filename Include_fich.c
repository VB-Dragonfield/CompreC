#include <stdio.h>
#include <zip.h>

int inclureFichierArchive(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination)
{
    FILE* fichier = fopen(cheminFichier, "rb");
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier);
        return -1;
    }

    fseek(fichier, 0, SEEK_END);
    long tailleFichier = ftell(fichier);
    fseek(fichier, 0, SEEK_SET);

    struct zip* archive = zip_open(cheminArchive, 0, NULL);
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive);
        return;
    }

    struct zip_source* source = zip_source_file(archive, cheminFichier, 0, -1);
    if (!source)
    {
        printf("Impossible de créer la source pour le fichier : %s\n", cheminFichier);
        fclose(fichier);
        return -1;
    }

    struct zip_file* fichierDestination = zip_fopen(archive, cheminDestination, ZIP_FL_OVERWRITE);
    if (!fichierDestination)
    {
        printf("Impossible de créer le fichier de destination dans l'archive : %s\n", cheminDestination);
        zip_source_free(source);
        fclose(fichier);
        return -1;
    }

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fichier)) > 0)
    {
        zip_fwrite(fichierDestination, buffer, bytesRead);
    }

    fclose(fichier);
    zip_fclose(fichierDestination);
    zip_source_free(source);

    printf("Inclusion terminée : %s inclus dans l'archive avec le chemin : %s\n", cheminFichier, cheminDestination);
    return 0;
}
