#include <stdio.h>
#include <string.h>
#include <zip.h>

int extraireFichierArchive(struct zip* archive, const char* cheminFichier, const char* cheminDestination)
{
    // Définir le mot de passe par défaut
    zip_set_default_password(archive, password);    
  
    struct zip_file* fichier = zip_fopen(archive, cheminFichier, 0);
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier);
        return -1;
    }

    FILE* fichierDestination = fopen(cheminDestination, "wb");
    if (!fichierDestination)
    {
        printf("Impossible de créer le fichier de destination : %s\n", cheminDestination);
        zip_fclose(fichier);
        return -1;
    }

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = zip_fread(fichier, buffer, sizeof(buffer))) > 0)
    {
        fwrite(buffer, 1, bytesRead, fichierDestination);
    }

    fclose(fichierDestination);
    zip_fclose(fichier);

    printf("Extraction terminée : %s extrait vers %s\n", cheminFichier, cheminDestination);
    return 0;
}
