#include <stdio.h>
#include <zip.h>

int inclureFichierArchivePassword(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination, const char* password)
{    
    FILE* fichier = fopen(cheminFichier, "rb"); // Ouverture du fichier source en mode binaire
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si l'ouverture du fichier échoue
        return -1;
    }

    fseek(fichier, 0, SEEK_END); // Positionnement à la fin du fichier pour obtenir sa taille
    long tailleFichier = ftell(fichier); // Obtention de la taille du fichier
    fseek(fichier, 0, SEEK_SET); // Retour à la position initiale du fichier
    
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive spécifiée par le chemin
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        fclose(fichier);
        return -1;
    }
    
    // Définir le mot de passe par défaut
    zip_set_default_password(archive, password); // Définition du mot de passe par défaut pour l'archive
    
    struct zip_source* source = zip_source_file(archive, cheminFichier, 0, -1); // Création d'une source à partir du fichier source
    if (!source)
    {
        printf("Impossible de créer la source pour le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si la création de la source échoue
        fclose(fichier);
        zip_close(archive);
        return -1;
    }

    struct zip_file* fichierDestination = zip_fopen(archive, cheminDestination, ZIP_FL_OVERWRITE); // Ouverture du fichier de destination dans l'archive avec option de remplacement
    if (!fichierDestination)
    {
        printf("Impossible de créer le fichier de destination dans l'archive : %s\n", cheminDestination); // Affiche un message d'erreur si l'ouverture du fichier de destination échoue
        zip_source_free(source);
        fclose(fichier);
        zip_close(archive);
        return -1;
    }

    char buffer[1024]; // Tampon pour lire les données du fichier source
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fichier)) > 0) // Lecture du fichier source dans des blocs de taille fixe
    {
        zip_fwrite(fichierDestination, buffer, bytesRead); // Écriture des données lues dans le fichier de destination dans l'archive
    }

    fclose(fichier); // Fermeture du fichier source
    zip_fclose(fichierDestination); // Fermeture du fichier de destination dans l'archive
    zip_source_free(source); // Libération de la source

    printf("Inclusion terminée : %s inclus dans l'archive avec le chemin : %s\n", cheminFichier, cheminDestination); // Affiche un message de confirmation de l'inclusion
    return 0;
}
