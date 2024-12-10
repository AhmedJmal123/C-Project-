#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "voitures.txt"

typedef struct Voiture {
    int id;
    char marque[50];
    char modele[50];
    float prix;
    int disponible;
    struct Voiture* suivant;
} Voiture;

Voiture* chargerVoitures() {
    FILE *fichier = fopen(FILE_NAME, "r");
    if (fichier == NULL) {
        printf("Fichier non trouvé, un nouveau fichier sera créé.\n");
        return NULL;
    }

    Voiture* tete = NULL;
    Voiture* courant = NULL;

    while (1) {
        Voiture* nouvelleVoiture = malloc(sizeof(Voiture));
        if (fscanf(fichier, "%d %49s %49s %f %d", &nouvelleVoiture->id, nouvelleVoiture->marque, nouvelleVoiture->modele, &nouvelleVoiture->prix, &nouvelleVoiture->disponible) != 5) {
            free(nouvelleVoiture);
            break;
        }
        nouvelleVoiture->suivant = NULL;

        if (tete == NULL) {
            tete = nouvelleVoiture;
        } else {
            courant->suivant = nouvelleVoiture;
        }
        courant = nouvelleVoiture;
    }

    fclose(fichier);
    return tete;
}

void sauvegarderVoitures(Voiture* tete) {
    FILE *fichier = fopen(FILE_NAME, "w");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour la sauvegarde.\n");
        return;
    }

    Voiture* courant = tete;
    while (courant != NULL) {
        fprintf(fichier, "%d %s %s %.2f %d\n", courant->id, courant->marque, courant->modele, courant->prix, courant->disponible);
        courant = courant->suivant;
    }

    fclose(fichier);
}

int idExiste(Voiture* tete, int id) {
    Voiture* courant = tete;
    while (courant != NULL) {
        if (courant->id == id) {
            return 1;
        }
        courant = courant->suivant;
    }
    return 0;
}

void ajouterVoiture(Voiture** tete) {
    Voiture* nouvelleVoiture = malloc(sizeof(Voiture));
    if (!nouvelleVoiture) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }

    int id;
    printf("Entrez l'ID : ");
    scanf("%d", &id);

    if (idExiste(*tete, id)) {
        printf("Erreur : une voiture avec cet ID existe déjà.\n");
        free(nouvelleVoiture);
        return;
    }

    nouvelleVoiture->id = id;
    printf("Entrez la marque : ");
    scanf("%s", nouvelleVoiture->marque);
    printf("Entrez le modele : ");
    scanf("%s", nouvelleVoiture->modele);
    printf("Entrez le prix : ");
    scanf("%f", &nouvelleVoiture->prix);

    nouvelleVoiture->disponible = 1;
    nouvelleVoiture->suivant = *tete;
    *tete = nouvelleVoiture;

    printf("Voiture ajoutée avec succès !\n");
}

void afficherVoitures(Voiture* tete) {
    if (tete == NULL) {
        printf("Aucune voiture disponible.\n");
        return;
    }

    printf("\nListe des voitures disponibles :\n");
    Voiture* courant = tete;
    while (courant != NULL) {
        if (courant->disponible) {
            printf("ID: %d | Marque: %s | Modele: %s | Prix: %.2f | Disponible: Oui\n",
                   courant->id, courant->marque, courant->modele, courant->prix);
        }
        courant = courant->suivant;
    }
}

void vendreVoiture(Voiture* tete) {
    int id;
    printf("Entrez l'ID de la voiture à vendre : ");
    scanf("%d", &id);

    Voiture* courant = tete;
    while (courant != NULL) {
        if (courant->id == id) {
            if (!courant->disponible) {
                printf("Cette voiture a déjà été vendue.\n");
                return;
            }

            courant->disponible = 0;
            printf("Voiture vendue avec succès !\n");
            return;
        }
        courant = courant->suivant;
    }

    printf("Voiture avec ID %d introuvable.\n", id);
}

void libererListe(Voiture* tete) {
    Voiture* courant = tete;
    while (courant != NULL) {
        Voiture* temp = courant;
        courant = courant->suivant;
        free(temp);
    }
}

int main() {
    Voiture* listeVoitures = chargerVoitures();

    int choix;
    do {
        printf("\nMenu :\n");
        printf("1. Ajouter une voiture\n");
        printf("2. Afficher les voitures\n");
        printf("3. Vendre une voiture\n");
        printf("4. Quitter\n");
        printf("Entrez votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterVoiture(&listeVoitures);
                sauvegarderVoitures(listeVoitures);
                break;
            case 2:
                afficherVoitures(listeVoitures);
                break;
            case 3:
                vendreVoiture(listeVoitures);
                sauvegarderVoitures(listeVoitures);
                break;
            case 4:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide.\n");
        }
    } while (choix != 4);

    libererListe(listeVoitures);
    return 0;
}
