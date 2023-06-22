#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Bruteforce_basic.h>
#include <Bruteforce_dictionnary.h>
#include <Explore_zip.h>
#include <Explore_zip_with_password.h>
#include <Extract_fich.h>
#include <Extract_fich_with_password.h>
#include <Include_fich.h>
#include <Include_fich_with_password.h>
#include <zip.h>

int main(int argc, char* argv[]) {
    const char* help = NULL; // Pointeur vers l'option d'aide
    const char* openArchive = NULL; // Pointeur vers le fichier d'archive à ouvrir
    const char* testBruteforce = NULL;
    const char* testBruteforceDictionary = NULL; // Pointeur vers le fichier de dictionnaire pour le bruteforce
    const char* usePassword = NULL; // Pointeur vers le mot de passe à utiliser
    const char* fileExtract = NULL; // Pointeur vers le fichier à extraire de l'archive
    const char* fileInclude = NULL; // Pointeur vers le fichier à inclure dans l'archive
    const char* cheminDestination = NULL; // Pointeur vers le chemin de destination
    int option;

    char charsetMDP[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ123456789@_-+/#*$€"; // Ensemble de caractères pour le bruteforce
    int lengthMDP = 24; // Longueur du mot de passe pour le bruteforce

    while ((option = getopt(argc, argv, "ho:bD:p:e:i:d:")) != -1) { // Boucle pour lire les options de ligne de commande
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
            default:
                printf("Utilisation :\n-h       Show this help\n-o </chemin/archive.zip>        Open a zip file for browsing\n-b      Try to bruteforce the password\n-D </chemin/file.txt>     Try to bruteforce the password with a dictionary\n-p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n-i </chemin/file>        Include this file\n-d </chemin/destination>        Destination where extract or include.\n");
                return 1;
        }
    }

    if (help != NULL) {
        printf("Utilisation :\n-h       Show this help\n-o </chemin/archive.zip>        Open a zip file for browsing\n-b      Try to bruteforce the password\n-D </chemin/file.txt>     Try to bruteforce the password with a dictionary\n-p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n-i </chemin/file>        Include this file\n-d </chemin/destination>        Destination where extract or include.\n");
    }

    if (openArchive != NULL) {
        explorerArchiveZip(openArchive); // Explore l'archive spécifiée
    }

    if (testBruteforce != NULL) {
        bruteForceZipPassword(openArchive, charsetMDP, lengthMDP); // Tente le bruteforce du mot de passe de l'archive
    }

    if (testBruteforceDictionary != NULL) {
        bruteForceZipPasswordDictionnary(openArchive, testBruteforceDictionary); // Tente le bruteforce du mot de passe de l'archive avec un dictionnaire
    }

    if (usePassword != NULL && openArchive != NULL) {
        explorerArchiveZipPassword(openArchive, usePassword); // Explore l'archive avec le mot de passe spécifié
    }

    if (openArchive != NULL && usePassword != NULL && fileExtract != NULL) {
        extraireFichierArchivePassword(openArchive, fileExtract, cheminDestination, usePassword); // Extrait un fichier de l'archive avec le mot de passe spécifié
    }

    if (openArchive != NULL && usePassword != NULL && fileInclude != NULL) {
        inclureFichierArchivePassword(openArchive, fileInclude, cheminDestination, usePassword); // Inclut un fichier dans l'archive avec le mot de passe spécifié
    }

    if (openArchive != NULL && fileExtract != NULL) {
        extraireFichierArchive(openArchive, fileExtract, cheminDestination); // Extrait un fichier de l'archive
    }

    if (openArchive != NULL && fileInclude != NULL) {
        inclureFichierArchive(openArchive, fileInclude, cheminDestination); // Inclut un fichier dans l'archive
    }

    else
    {
        printf("***Option(s) manquante(s)***"); // Affiche un message d'erreur si une ou plusieurs options sont manquantes
    }
    

    return 0;
}

