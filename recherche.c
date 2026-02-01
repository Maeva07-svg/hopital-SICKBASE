#include<stdio.h>

// Constantes
#define MAX_PATIENTS 1000
#define MAX_NAME 100
#define MAX_SYMPTOMS 20
#define MAX_SYMPTOM_LENGTH 100
#define MAX_MALADIES 10
#define MAX_SYMPTOMES_PAR_MALADIE 10
#define FICHIER_PATIENTS "patients.dat"
#define FICHIER_ID "dernier_id.dat"

// Structure Patient
typedef struct
{
    int id;
    char nom[MAX_NAME];
    char prenom[MAX_NAME];
    int age;
    char genre; // M/F
    char telephone[20];
    char email[100];
    char adresse[200];
    char date_naissance[20];
    char groupe_sanguin[5];
    char symptomes[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes;
    char diagnostic[200];
    char medecin_traitant[MAX_NAME];
    char date_admission[20];
    char notes[500];
} Patient;

// Structure pour une maladie
typedef struct
{
    char nom[100];
    char symptomes[MAX_SYMPTOMES_PAR_MALADIE][MAX_SYMPTOM_LENGTH];
    int nb_symptomes;
    char description[300];
    char traitement[300];
    char precautions[200];
} Maladie;

Patient patients[MAX_PATIENTS];
int nombrePatients = 0;
int dernierID = 0;

Maladie maladies[MAX_MALADIES];
int nombreMaladies = 8; // 8 maladies prédéfinies

void sauvegarderDernierID()
{
    FILE *fichier = fopen(FICHIER_ID, "wb");
    if (fichier != NULL)
    {
        fwrite(&dernierID, sizeof(int), 1, fichier);
        fclose(fichier);
    }
}

void chargerDernierID()
{
    FILE *fichier = fopen(FICHIER_ID, "rb");
    if (fichier != NULL)
    {
        fread(&dernierID, sizeof(int), 1, fichier);
        fclose(fichier);
    }
    else
    {
        dernierID = 0;
    }
}

void sauvegarderPatients()
{
    FILE *fichier = fopen(FICHIER_PATIENTS, "wb");
    if (fichier == NULL)
    {
        printf("ERREUR: Impossible de sauvegarder les patients!\n");
        return;
    }

    fwrite(&nombrePatients, sizeof(int), 1, fichier);

    for (int i = 0; i < nombrePatients; i++)
    {
        fwrite(&patients[i], sizeof(Patient), 1, fichier);
    }

    fclose(fichier);
    sauvegarderDernierID();
}

void chargerPatients()
{
    FILE *fichier = fopen(FICHIER_PATIENTS, "rb");
    if (fichier == NULL)
    {
        printf("Aucune donnee existante. Initialisation d'une nouvelle base.\n");
        return;
    }

    fread(&nombrePatients, sizeof(int), 1, fichier);

    for (int i = 0; i < nombrePatients; i++)
    {
        fread(&patients[i], sizeof(Patient), 1, fichier);
    }

    fclose(fichier);
}

// ============================================
// FONCTIONS UTILITAIRES
// ============================================

void viderBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pause()
{
    printf("\nAppuyez sur Entree pour continuer...");
    viderBuffer();
}

void clearInput()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int genererID()
{
    dernierID++;
    sauvegarderDernierID();
    return dernierID;
}

int rechercherParID(int id)
{
    for (int i = 0; i < nombrePatients; i++)
    {
        if (patients[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

void rechercherPatient()
{
    system("cls");
    printf("\n=== RECHERCHE DE PATIENT ===\n\n");

    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre.\n");
        pause();
        return;
    }

    int id;
    printf("Entrez l'ID du patient: ");
    scanf("%d", &id);
    viderBuffer();

    int index = rechercherParID(id);

    if (index != -1)
    {
        afficherDetailsCompletsPatient(index);

        printf("\nOptions:\n");
        printf("1. Modifier ce patient\n");
        printf("2. Effectuer un diagnostic\n");
        printf("3. Supprimer ce patient\n");
        printf("4. Retour\n");
        printf("Votre choix: ");

        int action;
        scanf("%d", &action);
        viderBuffer();

        if (action == 1)
        {
            modifierPatient(index);
        }
        else if (action == 2)
        {
            diagnosticPatientExistant(index);
        }
        else if (action == 3)
        {
            supprimerPatient(index);
        }
    }
    else
    {
        printf("Patient non trouve.\n");
        pause();
    }
}
