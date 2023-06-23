#include <stdio.h>
#include <string.h>
#include <zip.h>

int extraireFichierArchivePassword(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination, const char* password)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive ZIP spécifiée par le chemin
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        return -1;
    }
    
    zip_set_default_password(archive, password); // Définition du mot de passe par défaut pour l'archive
    
    struct zip_file* fichier = zip_fopen(archive, cheminFichier, 0); // Ouverture du fichier spécifié dans l'archive
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si l'ouverture du fichier échoue
        return -1;
    }
    
    FILE* fichierDestination = fopen(cheminDestination, "wb"); // Création du fichier de destination en mode binaire
    if (!fichierDestination)
    {
        printf("Impossible de créer le fichier de destination : %s\n", cheminDestination); // Affiche un message d'erreur si la création du fichier de destination échoue
        zip_fclose(fichier);
        return -1;
    }

    char buffer[1024]; // Tampon pour lire les données du fichier
    int bytesRead;
    while ((bytesRead = zip_fread(fichier, buffer, sizeof(buffer))) > 0) // Lecture du fichier dans des blocs de taille fixe
    {
        fwrite(buffer, 1, bytesRead, fichierDestination); // Écriture des données lues dans le fichier de destination
    }

    fclose(fichierDestination); // Fermeture du fichier de destination
    zip_fclose(fichier); // Fermeture du fichier dans l'archive

    printf("Extraction terminée : %s extrait vers %s\n", cheminFichier, cheminDestination); // Affiche un message de confirmation de l'extraction
    return 0;
}
