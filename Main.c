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
    const char* help = NULL;
    const char* openArchive = NULL;
    const char* testBruteforce = NULL;
    const char* testBruteforceDictionary = NULL;
    const char* usePassword = NULL;
    const char* fileExtract = NULL;
    const char* fileInclude = NULL;
    int option;

    while ((option = getopt(argc, argv, "h:o:b:d:p:e:i:")) != -1) {
        switch (option) {
            case 'h':
                cheminArchive = optarg;
                break;
            case 'o':
                cheminRepertoire = optarg;
                break;
            case 'b':
                cheminFichier = optarg;
                break;
            case 'd':
                cheminDestination = optarg;
                break;
            case 'p':
                cheminRepertoire = optarg;
                break;
            case 'e':
                cheminFichier = optarg;
                break;
            case 'i':
                cheminDestination = optarg;
                break;
            default:
                printf("Utilisation :\n-h       Show this help\n-o </chemin/archive.zip>        Open a zip file for browsing\n
                -b      Try to bruteforce the password\n-d </chemin/file.txt>     Try to bruteforce the password with a dictionary\n
                -p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n
                -i </chemin/file>        Include this file\n");
                return 1;
        }
    }

    if (help != NULL) {
        printf("Utilisation :\n-h       Show this help\n-o </chemin/archive.zip>        Open a zip file. if it's the only one option you can browsing.\n
        -b      Try to bruteforce the password\n-d </chemin/file.txt>     Try to bruteforce the password with a dictionary\n
        -p <password>       Use this password\n-e </chemin/archive/file>        Extract this file\n
        -i </chemin/file>        Include this file\n");
    }

    if (openArchive != NULL) {
        explorerArchiveZip();
    }

    if (testBruteforce != NULL) {
        bruteForceZipPassword();
    }

    if (testBruteforceDictionary != NULL) {
        bruteForceZipPasswordDictionnary();
    }

    if (usePassword != NULL && openArchive != NULL) {
        explorerArchiveZipPassword();
    }

    if (openArchive != NULL && usePassword != NULL && fileExtract != NULL) {
        extraireFichierArchivePassword();
    }

    if (openArchive != NULL && usePassword != NULL && fileInclude != NULL) {
        inclureFichierArchivePassword();
    }

    if (openArchive != NULL && fileExtract != NULL) {
        extraireFichierArchive();
    }

    if (openArchive != NULL && fileInclude != NULL) {
        inclureFichierArchive();
    }

    else
    {
        printf("***Option(s) manquante(s)***")
    }
    

    return 0;
}
