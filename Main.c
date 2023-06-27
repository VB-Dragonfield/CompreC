#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zip.h>


void explorerArchiveRepertoire(struct zip* archive, const char* cheminRepertoire);
void explorerArchiveZip(const char* cheminArchive);
void explorerArchiveZipPassword(const char* cheminArchive, const char* password);
int extraireFichierArchive(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination);
int extraireFichierArchivePassword(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination, const char* password);
int inclureFichierArchive(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination);
int inclureFichierArchivePassword(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination, const char* password);
void lireContenuFichierArchive(struct zip* archive, const char* cheminFichier);
int bruteForceZipPassword(const char* archivePath, const char* charset, int maxLength, const char* fileDecrypt);
int bruteForceZipPasswordDictionnary(const char* archivePath, const char* dictionaryPath, const char* fileDecrypt);


int main(int argc, char* argv[]) {
    const char* help = NULL; // Pointeur vers l'option d'aide
    const char* openArchive = NULL; // Pointeur vers le fichier d'archive à ouvrir
    const char* testBruteforce = NULL;
    const char* testBruteforceDictionary = NULL; // Pointeur vers le fichier de dictionnaire pour le bruteforce
    const char* usePassword = NULL; // Pointeur vers le mot de passe à utiliser
    const char* fileExtract = NULL; // Pointeur vers le fichier à extraire de l'archive
    const char* fileInclude = NULL; // Pointeur vers le fichier à inclure dans l'archive
    const char* cheminDestination = NULL; // Pointeur vers le chemin de destination
    const char* fileDecrypt = NULL; // Pointeur vers le fichier à déchiffrer
    int option;

    char charsetMDP[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789"; // Ensemble de caractères pour le bruteforce
    int lengthMDP = 24; // Longueur du mot de passe pour le bruteforce

    while ((option = getopt(argc, argv, "ho:bD:p:e:i:d:f:")) != -1) { // Boucle pour lire les options de ligne de commande
        switch (option) {
            case 'h':
                help = "1"; // Affiche l'aide
                break;
            case 'o':
                openArchive = optarg; // Stocke le chemin vers le fichier d'archive à ouvrir
                break;
            case 'b':
                testBruteforce = "1"; // Active le test de bruteforce
                break;
            case 'D':
                testBruteforceDictionary = optarg; // Stocke le chemin vers le fichier de dictionnaire pour le bruteforce
                break;
            case 'p':
                usePassword = optarg; // Stocke le mot de passe à utiliser
                break;
            case 'e':
                fileExtract = optarg; // Stocke le chemin vers le fichier à extraire de l'archive
                break;
            case 'i':
                fileInclude = optarg; // Stocke le chemin vers le fichier à inclure dans l'archive
                break;
            case 'd':
                cheminDestination = optarg; // Stocke le chemin vers le fichier à inclure dans l'archive
                break;
            case 'f':
            	fileDecrypt = optarg; // Stocke le chemin vers le fichier à déchiffrer
            	break;
            default:
                printf("Utilisation :\n-h Show this help\n-o </chemin/archive.zip>        Open a zip file for browsing\n-b Try to bruteforce the password\n-D </chemin/file.txt>     Try to bruteforce the password with a dictionary\n-p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n-i </chemin/file>        Include this file\n-d </chemin/destination>        Destination where extract or include.\n-f <filename>	Filename to decrypt\n");
                return 1;
        }
    }

    if (help != NULL) {
        printf("Utilisation :\n-h Show this help\n-o </chemin/archive.zip>        Open a zip file for browsing\n-b Try to bruteforce the password\n-D </chemin/file.txt>     Try to bruteforce the password with a dictionary\n-p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n-i </chemin/file>        Include this file\n-d </chemin/destination>        Destination where extract or include.\n-f <filename>	Filename to decrypt\n");
    }

    if (openArchive != NULL && testBruteforce != NULL && fileDecrypt != NULL) {
        bruteForceZipPassword(openArchive, charsetMDP, lengthMDP, fileDecrypt); // Tente le bruteforce du mot de passe de l'archive
        exit(0);
    }

    if (openArchive != NULL && testBruteforceDictionary != NULL && fileDecrypt != NULL) {
        bruteForceZipPasswordDictionnary(openArchive, testBruteforceDictionary, fileDecrypt); // Tente le bruteforce du mot de passe de l'archive avec un dictionnaire
        exit(0);
    }

    if (openArchive != NULL && usePassword != NULL && fileExtract != NULL) {
        extraireFichierArchivePassword(openArchive, fileExtract, cheminDestination, usePassword); // Extrait un fichier de l'archive avec le mot de passe spécifié
        exit(0);
    }

    if (openArchive != NULL && usePassword != NULL && fileInclude != NULL) {
        inclureFichierArchivePassword(openArchive, fileInclude, cheminDestination, usePassword); // Inclut un fichier dans l'archive avec le mot de passe spécifié
        exit(0);
    }

    if (openArchive != NULL && fileExtract != NULL) {
        extraireFichierArchive(openArchive, fileExtract, cheminDestination); // Extrait un fichier de l'archive
        exit(0);
    }

    if (openArchive != NULL && fileInclude != NULL) {
        inclureFichierArchive(openArchive, fileInclude, cheminDestination); // Inclut un fichier dans l'archive
        exit(0);
    }

    if (openArchive != NULL && usePassword != NULL) {
        explorerArchiveZipPassword(openArchive, usePassword); // Explore l'archive avec le mot de passe spécifié
        exit(0);
    }
    
    if (openArchive != NULL) {
        explorerArchiveZip(openArchive); // Explore l'archive spécifiée
        exit(0);
    }

    else
    {
        printf("***Option(s) manquante(s)***"); // Affiche un message d'erreur si une ou plusieurs options sont manquantes
        exit(0);
    }
    

    return 0;
}

void explorerArchiveRepertoire(struct zip* archive, const char* cheminRepertoire)
{
    char answer[250];

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
    
    printf("\nVoulez vous explorer un répertoire ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
    scanf("%s", answer);

    if (strcmp(answer, "N") != 0){
        explorerArchiveRepertoire(archive, answer);
    }
        
    else {
        printf("\nVoulez vous lire le contenu d'un fichier ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
        scanf("%s", answer);
        
        if (strcmp(answer, "N") != 0){

            lireContenuFichierArchive(archive, answer);
        }
    }  

}

void explorerArchiveZip(const char* cheminArchive)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive ZIP spécifiée par le chemin
    char answer[250];

    
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


    printf("\nVoulez vous explorer un répertoire ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
    scanf("%s", answer);
    

    if (strcmp(answer, "N") != 0){

        explorerArchiveRepertoire(archive, answer);
    }
        
    else {
        printf("\nVoulez vous lire le contenu d'un fichier ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
        scanf("%s", answer);

        if (strcmp(answer, "N") != 0){

            lireContenuFichierArchive(archive, answer);
        }
    }

    zip_close(archive); // Ferme l'archive
}

void explorerArchiveZipPassword(const char* cheminArchive, const char* password)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive ZIP spécifiée par le chemin

    char answer[250];


    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        return;
    }

    zip_set_default_password(archive, password); // Définition du mot de passe par défaut pour l'archive

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

    printf("\nVoulez vous explorer un répertoire ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
    scanf("%s", answer);
    

    if (strcmp(answer, "N") != 0){

        explorerArchiveRepertoire(archive, answer);
    }
        
    else {
        printf("\nVoulez vous lire le contenu d'un fichier ? Si oui lequel (entrez l'ensemble du chemin au sein de l'archive (ATTENTION SENSIBLE A LA CASSE))\nSinon tapez 'N' :");
        scanf("%s", answer);
        

        if (strcmp(answer, "N") != 0){

            lireContenuFichierArchive(archive, answer);
        }
    }
    
    zip_close(archive); // Ferme l'archive
}

int extraireFichierArchive(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination)
{
    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive ZIP spécifiée par le chemin
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        return -1;
    }

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

int inclureFichierArchive(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination)
{
    FILE* fichier = fopen(cheminFichier, "rb"); // Ouverture du fichier source en mode binaire
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si l'ouverture du fichier échoue
        return -1;
    }

    fseek(fichier, 0, SEEK_END); // Positionnement à la fin du fichier pour obtenir sa taille
    fseek(fichier, 0, SEEK_SET); // Retour à la position initiale du fichier

    struct zip* archive = zip_open(cheminArchive, 0, NULL); // Ouverture de l'archive spécifiée par le chemin
    if (!archive)
    {
        printf("Impossible d'ouvrir l'archive : %s\n", cheminArchive); // Affiche un message d'erreur si l'ouverture de l'archive échoue
        fclose(fichier);
        return -1;
    }

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
        //zip_fwrite(fichierDestination, buffer, bytesRead); // Écriture des données lues dans le fichier de destination dans l'archive
    }

    fclose(fichier); // Fermeture du fichier source
    zip_fclose(fichierDestination); // Fermeture du fichier de destination dans l'archive
    zip_source_free(source); // Libération de la source

    printf("Inclusion terminée : %s inclus dans l'archive avec le chemin : %s\n", cheminFichier, cheminDestination); // Affiche un message de confirmation de l'inclusion
    return 0;
}

int inclureFichierArchivePassword(const char* cheminArchive, const char* cheminFichier, const char* cheminDestination, const char* password)
{    
    FILE* fichier = fopen(cheminFichier, "rb"); // Ouverture du fichier source en mode binaire
    if (!fichier)
    {
        printf("Impossible d'ouvrir le fichier : %s\n", cheminFichier); // Affiche un message d'erreur si l'ouverture du fichier échoue
        return -1;
    }

    fseek(fichier, 0, SEEK_END); // Positionnement à la fin du fichier pour obtenir sa taille
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
        //zip_fwrite(fichierDestination, buffer, bytesRead); // Écriture des données lues dans le fichier de destination dans l'archive
    }

    fclose(fichier); // Fermeture du fichier source
    zip_fclose(fichierDestination); // Fermeture du fichier de destination dans l'archive
    zip_source_free(source); // Libération de la source

    printf("Inclusion terminée : %s inclus dans l'archive avec le chemin : %s\n", cheminFichier, cheminDestination); // Affiche un message de confirmation de l'inclusion
    return 0;
}

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


int bruteForceZipPassword(const char* archivePath, const char* charset, int maxLength, const char* fileDecrypt) {

    int charsetLength = strlen(charset); // Longueur de l'ensemble de caractères possibles pour le mot de passe
    int* indices = (int*)malloc(maxLength * sizeof(int)); // Tableau d'indices pour générer les combinaisons de caractères
    int err;
    
    struct zip* archive = zip_open(archivePath, 0, &err); // Ouverture de l'archive en mode lecture seule
    if (!archive) {
        printf("Erreur lors de l'ouverture de l'archive.\n"); // Affiche une erreur si l'ouverture de l'archive échoue
        return -1;
    }

    int foundPassword = 0; // Indicateur indiquant si le mot de passe a été trouvé
    int passwordLength = 1; // Longueur initiale du mot de passe à tester

    while (passwordLength <= maxLength) { // Boucle pour tester les mots de passe de différentes longueurs
        int i;
        for (i = 0; i < passwordLength; i++) {
            indices[i] = 0; // Initialise tous les indices à 0 pour générer la première combinaison
        }

        char* password = (char*)malloc((passwordLength + 1) * sizeof(char)); // Alloue de la mémoire pour le mot de passe à tester
        password[passwordLength] = '\0'; // Ajoute le caractère de fin de chaîne

        while (1) { // Boucle pour générer et tester toutes les combinaisons possibles de caractères
            for (i = 0; i < passwordLength; i++) {
                password[i] = charset[indices[i]]; // Construit le mot de passe en utilisant les caractères correspondants aux indices
            }

	
            zip_set_default_password(archive, password);
            zip_file_t * result = zip_fopen_encrypted(archive, fileDecrypt, 0, password);
            if (result != NULL) {

                printf("Mot de passe trouvé : %s\n", password); // Affiche le mot de passe trouvé
                foundPassword = 1;
                break;
            }

            int carry = 1;
            for (i = passwordLength - 1; i >= 0; i--) {
                indices[i] += carry; // Incrémente l'indice actuel
                if (indices[i] == charsetLength) { // Vérifie si l'indice dépasse la taille de l'ensemble de caractères
                    indices[i] = 0; // Réinitialise l'indice à 0
                    carry = 1; // Propage la retenue
                } else {
                    carry = 0; // Pas de retenue, arrête l'incrémentation
                    break;
                }
            }

            if (carry)
                break; // Si la retenue est propagée jusqu'à la première position, toutes les combinaisons ont été testées

        }

        free(password); // Libère la mémoire allouée pour le mot de passe

        if (foundPassword)
            break; // Si le mot de passe a été trouvé, arrête la boucle

        passwordLength++; // Incrémente la longueur du mot de passe à tester
    }

    zip_close(archive); // Ferme l'archive
    free(indices); // Libère la mémoire allouée pour les indices

    if (!foundPassword) {
        printf("Mot de passe introuvable.\n"); // Affiche un message si le mot de passe n'a pas été trouvé
        return -1;
    }

    return 0; // Retourne 0 pour indiquer que le processus s'est terminé avec succès
}


int bruteForceZipPasswordDictionnary(const char* archivePath, const char* dictionaryPath, const char* fileDecrypt) {
    int err;
    struct zip* archive = zip_open(archivePath, 0, &err);

    if (!archive) {
        printf("Erreur lors de l'ouverture de l'archive.\n");
        return -1;
    }

    FILE* dictionary = fopen(dictionaryPath, "r");
    if (!dictionary) {
        printf("Erreur lors de l'ouverture du dictionnaire.\n");
        zip_close(archive);
        return -1;
    }

    int foundPassword = 0;
    char password[256];

    while (fgets(password, sizeof(password), dictionary)) {
        int passwordLength = strlen(password);
        if (password[passwordLength - 1] == '\n')
            password[--passwordLength] = '\0';

        zip_file_t * result = zip_fopen_encrypted(archive, fileDecrypt, 0, password);
      
        if (result != NULL) {
            printf("Mot de passe trouvé : %s\n", password);
            foundPassword = 1;
            break;
        }
    }

    fclose(dictionary);
    zip_close(archive);


    if (!foundPassword) {
        printf("Mot de passe introuvable.\n");
        return -1;
    }

    return 0;
}


