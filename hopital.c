#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "recherche.h"

void color(int couleurTexte, int couleurFond)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int couleur = couleurTexte + (couleurFond * 16);
    SetConsoleTextAttribute(hConsole, couleur);
}

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

// ============================================
// PROTOTYPES DES FONCTIONS
// ============================================
// Fonctions fichiers
void sauvegarderPatients();
void chargerPatients();
void sauvegarderDernierID();
void chargerDernierID();

// Fonctions patients
void ajouterNouveauPatient();
void afficherListePatientsSimple();
void afficherDetailsCompletsPatient(int index);
int rechercherParID(int id);
void rechercherPatient();
void modifierPatient(int index);
void supprimerPatient(int index);
void menuAffichagePatients();
void menuGestionPatients();
void afficherStatistiques();

// Fonctions diagnostic
void initialiserMaladies();
void diagnosticParSymptomes();
void diagnosticPatientExistant(int index_patient);
void afficherToutesMaladies();
void menuDiagnosticMedical();

// Fonctions utilitaires
void viderBuffer();
void pause();
int genererID();
void clearInput();
int comparerSymptomes(char symptome1[], char symptome2[]);
// ============================================

// ============================================
// INITIALISATION DES MALADIES
// ============================================

void initialiserMaladies()
{
    // Maladie 1: Paludisme
    strcpy(maladies[0].nom, "Paludisme");
    strcpy(maladies[0].symptomes[0], "fievre elevee");
    strcpy(maladies[0].symptomes[1], "frissons");
    strcpy(maladies[0].symptomes[2], "maux de tete");
    strcpy(maladies[0].symptomes[3], "vomissements");
    strcpy(maladies[0].symptomes[4], "fatigue intense");
    maladies[0].nb_symptomes = 5;
    strcpy(maladies[0].description, "Maladie parasitaire transmise par les moustiques");
    strcpy(maladies[0].traitement, "Medicaments antipaludeens (chloroquine, artemisinine)");
    strcpy(maladies[0].precautions, "Moustiquaire, repellents, consultation immediate");

    // Maladie 2: Typhoïde
    strcpy(maladies[1].nom, "Typhoide");
    strcpy(maladies[1].symptomes[0], "fievre progressive");
    strcpy(maladies[1].symptomes[1], "douleurs abdominales");
    strcpy(maladies[1].symptomes[2], "diarrhee");
    strcpy(maladies[1].symptomes[3], "maux de tete");
    strcpy(maladies[1].symptomes[4], "perte d'appetit");
    maladies[1].nb_symptomes = 5;
    strcpy(maladies[1].description, "Infection bacterienne due a Salmonella typhi");
    strcpy(maladies[1].traitement, "Antibiotiques (ciprofloxacine, ceftriaxone)");
    strcpy(maladies[1].precautions, "Eau potable, hygiene alimentaire");

    // Maladie 3: Angine
    strcpy(maladies[2].nom, "Angine");
    strcpy(maladies[2].symptomes[0], "maux de gorge");
    strcpy(maladies[2].symptomes[1], "difficulte a avaler");
    strcpy(maladies[2].symptomes[2], "fievre moderee");
    strcpy(maladies[2].symptomes[3], "ganglions enflés");
    maladies[2].nb_symptomes = 4;
    strcpy(maladies[2].description, "Inflammation des amygdales, virale ou bacterienne");
    strcpy(maladies[2].traitement, "Antibiotiques si bacterien, anti-inflammatoires");
    strcpy(maladies[2].precautions, "Repos, gargarismes, boissons chaudes");

    // Maladie 4: COVID-19
    strcpy(maladies[3].nom, "COVID-19");
    strcpy(maladies[3].symptomes[0], "fievre");
    strcpy(maladies[3].symptomes[1], "toux seche");
    strcpy(maladies[3].symptomes[2], "fatigue");
    strcpy(maladies[3].symptomes[3], "perte de gout ou odorat");
    strcpy(maladies[3].symptomes[4], "difficultes respiratoires");
    maladies[3].nb_symptomes = 5;
    strcpy(maladies[3].description, "Maladie respiratoire due au coronavirus SARS-CoV-2");
    strcpy(maladies[3].traitement, "Traitement symptomatique, antiviraux, oxygenotherapie");
    strcpy(maladies[3].precautions, "Vaccination, masque, distanciation, isolement");

    // Maladie 5: Migraine
    strcpy(maladies[4].nom, "Migraine");
    strcpy(maladies[4].symptomes[0], "maux de tete severes");
    strcpy(maladies[4].symptomes[1], "nausees");
    strcpy(maladies[4].symptomes[2], "vomissements");
    strcpy(maladies[4].symptomes[3], "sensibilite a la lumiere");
    strcpy(maladies[4].symptomes[4], "sensibilite au bruit");
    maladies[4].nb_symptomes = 5;
    strcpy(maladies[4].description, "Cephalee primaire intense et recurrente");
    strcpy(maladies[4].traitement, "Anti-inflammatoires, triptans, antalgiques");
    strcpy(maladies[4].precautions, "Eviter les declencheurs, repos dans le calme");

    // Maladie 6: Infection urinaire
    strcpy(maladies[5].nom, "Infection urinaire");
    strcpy(maladies[5].symptomes[0], "brulures urinaires");
    strcpy(maladies[5].symptomes[1], "envies frequentes d'uriner");
    strcpy(maladies[5].symptomes[2], "douleurs pelviennes");
    strcpy(maladies[5].symptomes[3], "urines troubles");
    maladies[5].nb_symptomes = 4;
    strcpy(maladies[5].description, "Infection bacterienne des voies urinaires");
    strcpy(maladies[5].traitement, "Antibiotiques, antalgiques urinaires");
    strcpy(maladies[5].precautions, "Boire beaucoup d'eau, hygiene intime");

    // Maladie 7: Grippe
    strcpy(maladies[6].nom, "Grippe");
    strcpy(maladies[6].symptomes[0], "fievre soudaine");
    strcpy(maladies[6].symptomes[1], "toux");
    strcpy(maladies[6].symptomes[2], "douleurs musculaires");
    strcpy(maladies[6].symptomes[3], "fatigue");
    strcpy(maladies[6].symptomes[4], "maux de tete");
    maladies[6].nb_symptomes = 5;
    strcpy(maladies[6].description, "Infection virale respiratoire saisonniere");
    strcpy(maladies[6].traitement, "Repos, paracetamol, antiviraux si necessaire");
    strcpy(maladies[6].precautions, "Vaccination annuelle, hygiene des mains");

    // Maladie 8: Gastroenterite
    strcpy(maladies[7].nom, "Gastroenterite");
    strcpy(maladies[7].symptomes[0], "nausees");
    strcpy(maladies[7].symptomes[1], "vomissements");
    strcpy(maladies[7].symptomes[2], "diarrhee");
    strcpy(maladies[7].symptomes[3], "crampes abdominales");
    strcpy(maladies[7].symptomes[4], "fievre legere");
    maladies[7].nb_symptomes = 5;
    strcpy(maladies[7].description, "Inflammation de l'estomac et des intestins");
    strcpy(maladies[7].traitement, "Rehydratation, alimentation legere, antispasmodiques");
    strcpy(maladies[7].precautions, "Hygiene alimentaire, lavage des mains frequent");
}

// ============================================
// FONCTIONS UTILITAIRES POUR DIAGNOSTIC
// ============================================

int comparerSymptomes(char symptome1[], char symptome2[])
{
    // Convertir en minuscules pour comparaison insensible à la casse
    char s1[MAX_SYMPTOM_LENGTH], s2[MAX_SYMPTOM_LENGTH];
    strcpy(s1, symptome1);
    strcpy(s2, symptome2);

    for(int i = 0; s1[i]; i++) s1[i] = tolower(s1[i]);
    for(int i = 0; s2[i]; i++) s2[i] = tolower(s2[i]);

    // Vérifier si s2 est contenu dans s1 ou vice versa
    return (strstr(s1, s2) != NULL || strstr(s2, s1) != NULL);
}

// ============================================
// FONCTIONS DE GESTION DES FICHIERS
// ============================================

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

// ============================================
// FONCTIONS POUR AJOUTER UN PATIENT
// ============================================

void ajouterNouveauPatient()
{
    system("cls");
    printf("\n=== AJOUT D'UN NOUVEAU PATIENT ===\n\n");

    if (nombrePatients >= MAX_PATIENTS)
    {
        printf("ERREUR: Capacite maximale atteinte!\n");
        pause();
        return;
    }

    Patient p;
    memset(&p, 0, sizeof(Patient));

    p.id = genererID();

    printf("INFORMATIONS PERSONNELLES\n\n");

    printf("Nom: ");
    fgets(p.nom, MAX_NAME, stdin);
    p.nom[strcspn(p.nom, "\n")] = '\0';

    printf("Prenom: ");
    fgets(p.prenom, MAX_NAME, stdin);
    p.prenom[strcspn(p.prenom, "\n")] = '\0';

    printf("Age: ");
    scanf("%d", &p.age);
    viderBuffer();

    printf("Genre (M/F): ");
    scanf("%c", &p.genre);
    viderBuffer();
    p.genre = toupper(p.genre);

    printf("Date de naissance (JJ/MM/AAAA): ");
    fgets(p.date_naissance, 20, stdin);
    p.date_naissance[strcspn(p.date_naissance, "\n")] = '\0';

    printf("Groupe sanguin (ex: A+, O-, etc.): ");
    fgets(p.groupe_sanguin, 5, stdin);
    p.groupe_sanguin[strcspn(p.groupe_sanguin, "\n")] = '\0';

    printf("Telephone: ");
    fgets(p.telephone, 20, stdin);
    p.telephone[strcspn(p.telephone, "\n")] = '\0';

    printf("Email: ");
    fgets(p.email, 100, stdin);
    p.email[strcspn(p.email, "\n")] = '\0';

    printf("Adresse: ");
    fgets(p.adresse, 200, stdin);
    p.adresse[strcspn(p.adresse, "\n")] = '\0';

    printf("\nINFORMATIONS MEDICALES\n\n");

    printf("Medecin traitant: ");
    fgets(p.medecin_traitant, MAX_NAME, stdin);
    p.medecin_traitant[strcspn(p.medecin_traitant, "\n")] = '\0';

    printf("Date d'admission (JJ/MM/AAAA): ");
    fgets(p.date_admission, 20, stdin);
    p.date_admission[strcspn(p.date_admission, "\n")] = '\0';

    printf("\nSYMPTOMES (Entrez 'fin' pour terminer)\n");
    printf("Exemples: fievre, toux, maux de tete, nausees, etc.\n\n");

    p.nb_symptomes = 0;
    for (int i = 0; i < MAX_SYMPTOMS; i++)
    {
        printf("Symptome %d: ", i + 1);
        char temp[MAX_SYMPTOM_LENGTH];
        fgets(temp, MAX_SYMPTOM_LENGTH, stdin);
        temp[strcspn(temp, "\n")] = '\0';

        if (strcmp(temp, "fin") == 0 || strlen(temp) == 0)
        {
            break;
        }

        strcpy(p.symptomes[i], temp);
        p.nb_symptomes++;
    }

    strcpy(p.diagnostic, "En attente de diagnostic");

    printf("\nNotes medicales (optionnel):\n");
    fgets(p.notes, 500, stdin);
    p.notes[strcspn(p.notes, "\n")] = '\0';

    patients[nombrePatients] = p;
    nombrePatients++;

    sauvegarderPatients();

    printf("\n=== PATIENT AJOUTE AVEC SUCCES ===\n");
    printf("ID Patient: %d\n", p.id);
    printf("Nom: %s %s\n", p.prenom, p.nom);
    printf("Age: %d ans\n", p.age);
    printf("Genre: %c\n", p.genre);
    printf("Telephone: %s\n", p.telephone);
    printf("Symptomes enregistres: %d\n", p.nb_symptomes);

    // Proposer un diagnostic immédiat
    if (p.nb_symptomes > 0)
    {
        printf("\nVoulez-vous effectuer un diagnostic maintenant? (O/N): ");
        char reponse;
        scanf("%c", &reponse);
        viderBuffer();

        if (toupper(reponse) == 'O')
        {
            diagnosticPatientExistant(nombrePatients - 1);
        }
    }

    pause();
}

// ============================================
// FONCTIONS D'AFFICHAGE DES PATIENTS
// ============================================

void afficherListePatientsSimple()
{
    system("cls");
    printf("\n=== LISTE DES PATIENTS ===\n\n");

    if (nombrePatients == 0)
    {
        printf("Aucun patient enregistre dans le systeme.\n");
        return;
    }

    printf("Nombre total de patients: %d\n\n", nombrePatients);

    printf("ID   | Nom & Prenom           | Age | Genre | Diagnostic\n");
    printf("-----|------------------------|-----|-------|-------------------\n");

    for (int i = 0; i < nombrePatients; i++)
    {
        printf("%-4d | %-10s %-10s | %-3d |   %c   | %s\n",
               patients[i].id,
               patients[i].prenom,
               patients[i].nom,
               patients[i].age,
               patients[i].genre,
               patients[i].diagnostic);
    }
}

void afficherDetailsCompletsPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient p = patients[index];

    printf("\n========================================\n");
    printf("FICHE PATIENT COMPLETE - ID: %d\n", p.id);
    printf("========================================\n");

    printf("\nINFORMATIONS PERSONNELLES:\n");
    printf("  Nom: %s\n", p.nom);
    printf("  Prenom: %s\n", p.prenom);
    printf("  Age: %d ans\n", p.age);
    printf("  Genre: %c\n", p.genre);
    printf("  Date de naissance: %s\n", p.date_naissance);
    printf("  Groupe sanguin: %s\n", p.groupe_sanguin);
    printf("  Telephone: %s\n", p.telephone);
    printf("  Email: %s\n", p.email);
    printf("  Adresse: %s\n", p.adresse);

    printf("\nINFORMATIONS MEDICALES:\n");
    printf("  Medecin traitant: %s\n", p.medecin_traitant);
    printf("  Date d'admission: %s\n", p.date_admission);
    printf("  Diagnostic: %s\n", p.diagnostic);

    printf("\nSYMPTOMES (%d):\n", p.nb_symptomes);
    if (p.nb_symptomes == 0)
    {
        printf("  Aucun symptome enregistre\n");
    }
    else
    {
        for (int i = 0; i < p.nb_symptomes; i++)
        {
            printf("  %d. %s\n", i + 1, p.symptomes[i]);
        }
    }

    if (strlen(p.notes) > 0)
    {
        printf("\nNOTES MEDICALES:\n");
        printf("  %s\n", p.notes);
    }

    printf("\n========================================\n");
}

// ============================================
// FONCTIONS DE RECHERCHE DE PATIENT
// ============================================

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

// ============================================
// FONCTION DE MODIFICATION DE PATIENT
// ============================================

void modifierPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient *p = &patients[index];
    int choix;
    char temp[500];

    do
    {
        system("cls");
        printf("\n=== MODIFICATION DU PATIENT ===\n");
        printf("Patient: %s %s (ID: %d)\n\n", p->prenom, p->nom, p->id);

        printf("1. Nom (%s)\n", p->nom);
        printf("2. Prenom (%s)\n", p->prenom);
        printf("3. Age (%d ans)\n", p->age);
        printf("4. Genre (%c)\n", p->genre);
        printf("5. Date de naissance (%s)\n", p->date_naissance);
        printf("6. Groupe sanguin (%s)\n", p->groupe_sanguin);
        printf("7. Telephone (%s)\n", p->telephone);
        printf("8. Email (%s)\n", p->email);
        printf("9. Adresse (%s)\n", p->adresse);
        printf("10. Medecin traitant (%s)\n", p->medecin_traitant);
        printf("11. Date d'admission (%s)\n", p->date_admission);
        printf("12. Diagnostic (%s)\n", p->diagnostic);
        printf("13. Symptomes (%d symptomes)\n", p->nb_symptomes);
        printf("14. Notes (%s)\n", strlen(p->notes) > 20 ? "...(tronque)" : p->notes);
        printf("15. Sauvegarder et quitter\n");
        printf("16. Annuler\n");
        printf("\nQuel champ modifier? (1-16): ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
            case 1:
                printf("Nouveau nom: ");
                fgets(p->nom, MAX_NAME, stdin);
                p->nom[strcspn(p->nom, "\n")] = '\0';
                break;

            case 2:
                printf("Nouveau prenom: ");
                fgets(p->prenom, MAX_NAME, stdin);
                p->prenom[strcspn(p->prenom, "\n")] = '\0';
                break;

            case 3:
                printf("Nouvel age: ");
                scanf("%d", &p->age);
                viderBuffer();
                break;

            case 4:
                printf("Nouveau genre (M/F): ");
                scanf("%c", &p->genre);
                viderBuffer();
                p->genre = toupper(p->genre);
                break;

            case 5:
                printf("Nouvelle date de naissance (JJ/MM/AAAA): ");
                fgets(p->date_naissance, 20, stdin);
                p->date_naissance[strcspn(p->date_naissance, "\n")] = '\0';
                break;

            case 6:
                printf("Nouveau groupe sanguin: ");
                fgets(p->groupe_sanguin, 5, stdin);
                p->groupe_sanguin[strcspn(p->groupe_sanguin, "\n")] = '\0';
                break;

            case 7:
                printf("Nouveau telephone: ");
                fgets(p->telephone, 20, stdin);
                p->telephone[strcspn(p->telephone, "\n")] = '\0';
                break;

            case 8:
                printf("Nouvel email: ");
                fgets(p->email, 100, stdin);
                p->email[strcspn(p->email, "\n")] = '\0';
                break;

            case 9:
                printf("Nouvelle adresse: ");
                fgets(p->adresse, 200, stdin);
                p->adresse[strcspn(p->adresse, "\n")] = '\0';
                break;

            case 10:
                printf("Nouveau medecin traitant: ");
                fgets(p->medecin_traitant, MAX_NAME, stdin);
                p->medecin_traitant[strcspn(p->medecin_traitant, "\n")] = '\0';
                break;

            case 11:
                printf("Nouvelle date d'admission (JJ/MM/AAAA): ");
                fgets(p->date_admission, 20, stdin);
                p->date_admission[strcspn(p->date_admission, "\n")] = '\0';
                break;

            case 12:
                printf("Nouveau diagnostic: ");
                fgets(p->diagnostic, 200, stdin);
                p->diagnostic[strcspn(p->diagnostic, "\n")] = '\0';
                break;

            case 13:
                printf("\n=== GESTION DES SYMPTOMES ===\n");
                printf("1. Ajouter un symptome\n");
                printf("2. Voir la liste des symptomes\n");
                printf("3. Supprimer tous les symptomes\n");
                printf("Votre choix: ");

                int choixSympt;
                scanf("%d", &choixSympt);
                viderBuffer();

                if (choixSympt == 1 && p->nb_symptomes < MAX_SYMPTOMS)
                {
                    printf("Nouveau symptome: ");
                    fgets(temp, MAX_SYMPTOM_LENGTH, stdin);
                    temp[strcspn(temp, "\n")] = '\0';
                    strcpy(p->symptomes[p->nb_symptomes], temp);
                    p->nb_symptomes++;
                }
                else if (choixSympt == 2)
                {
                    printf("\nSymptomes actuels (%d):\n", p->nb_symptomes);
                    for (int i = 0; i < p->nb_symptomes; i++)
                    {
                        printf("%d. %s\n", i+1, p->symptomes[i]);
                    }
                    pause();
                }
                else if (choixSympt == 3)
                {
                    p->nb_symptomes = 0;
                    printf("Tous les symptomes ont ete supprimes.\n");
                    pause();
                }
                break;

            case 14:
                printf("Nouvelles notes: ");
                fgets(p->notes, 500, stdin);
                p->notes[strcspn(p->notes, "\n")] = '\0';
                break;

            case 15:
                sauvegarderPatients();
                printf("\nModifications sauvegardees avec succes!\n");
                return;

            case 16:
                printf("\nModifications annulees.\n");
                return;

            default:
                printf("Choix invalide.\n");
        }

        if (choix != 15 && choix != 16)
        {
            printf("\nModification effectuee. Continuer? (O/N): ");
            char continuer;
            scanf("%c", &continuer);
            viderBuffer();

            if (toupper(continuer) != 'O')
            {
                sauvegarderPatients();
                printf("Modifications sauvegardees.\n");
                pause();
                return;
            }
        }

    } while(choix != 15 && choix != 16);
}

// ============================================
// FONCTION DE SUPPRESSION DE PATIENT
// ============================================

void supprimerPatient(int index)
{
    if (index < 0 || index >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    printf("\n=== SUPPRESSION DE PATIENT ===\n");
    printf("Patient: %s %s (ID: %d)\n", patients[index].prenom, patients[index].nom, patients[index].id);
    printf("Etes-vous sur de vouloir supprimer ce patient? (O/N): ");

    char confirmation;
    scanf("%c", &confirmation);
    viderBuffer();

    if (toupper(confirmation) == 'O')
    {
        // Déplacer tous les patients après l'index vers la gauche
        for (int i = index; i < nombrePatients - 1; i++)
        {
            patients[i] = patients[i + 1];
        }

        nombrePatients--;
        sauvegarderPatients();

        printf("Patient supprime avec succes.\n");
    }
    else
    {
        printf("Suppression annulee.\n");
    }

    pause();
}

// ============================================
// FONCTION D'AFFICHAGE DES STATISTIQUES
// ============================================

void afficherStatistiques()
{
    system("cls");
    printf("\n=== STATISTIQUES ===\n\n");
    printf("Nombre total de patients: %d\n", nombrePatients);
    printf("Dernier ID utilise: %d\n", dernierID);

    if (nombrePatients > 0)
    {
        int avec_diagnostic = 0, sans_diagnostic = 0;
        int hommes = 0, femmes = 0;
        int totalAge = 0;

        for (int i = 0; i < nombrePatients; i++)
        {
            if (patients[i].genre == 'M') hommes++;
            else femmes++;

            if (strcmp(patients[i].diagnostic, "En attente de diagnostic") != 0)
                avec_diagnostic++;
            else sans_diagnostic++;

            totalAge += patients[i].age;
        }

        printf("\nRepartition par genre:\n");
        printf("  Hommes: %d (%.1f%%)\n", hommes, (float)hommes/nombrePatients*100);
        printf("  Femmes: %d (%.1f%%)\n", femmes, (float)femmes/nombrePatients*100);

        printf("\nEtat des diagnostics:\n");
        printf("  Avec diagnostic: %d (%.1f%%)\n", avec_diagnostic, (float)avec_diagnostic/nombrePatients*100);
        printf("  Sans diagnostic: %d (%.1f%%)\n", sans_diagnostic, (float)sans_diagnostic/nombrePatients*100);

        printf("\nAge moyen des patients: %.1f ans\n", (float)totalAge/nombrePatients);

        // Statistiques par groupe d'âge
        int enfants = 0, adultes = 0, seniors = 0;
        for (int i = 0; i < nombrePatients; i++)
        {
            if (patients[i].age < 18) enfants++;
            else if (patients[i].age < 60) adultes++;
            else seniors++;
        }

        printf("\nRepartition par age:\n");
        printf("  Enfants (<18 ans): %d (%.1f%%)\n", enfants, (float)enfants/nombrePatients*100);
        printf("  Adultes (18-59 ans): %d (%.1f%%)\n", adultes, (float)adultes/nombrePatients*100);
        printf("  Seniors (60+ ans): %d (%.1f%%)\n", seniors, (float)seniors/nombrePatients*100);
    }

    printf("\nCapacite restante: %d/%d patients\n", MAX_PATIENTS - nombrePatients, MAX_PATIENTS);

    // Maladies les plus fréquentes
    if (nombrePatients > 0)
    {
        printf("\nMaladies diagnostiquees:\n");
        int maladies_comptes[MAX_MALADIES] = {0};

        for (int i = 0; i < nombrePatients; i++)
        {
            if (strcmp(patients[i].diagnostic, "En attente de diagnostic") != 0)
            {
                for (int m = 0; m < nombreMaladies; m++)
                {
                    if (strstr(patients[i].diagnostic, maladies[m].nom) != NULL)
                    {
                        maladies_comptes[m]++;
                        break;
                    }
                }
            }
        }

        for (int m = 0; m < nombreMaladies; m++)
        {
            if (maladies_comptes[m] > 0)
            {
                printf("  %s: %d cas\n", maladies[m].nom, maladies_comptes[m]);
            }
        }
    }

    pause();
}

// ============================================
// FONCTIONS DE DIAGNOSTIC MEDICAL
// ============================================

void afficherToutesMaladies()
{
    system("cls");
    printf("\n=== BASE DE DONNEES DES MALADIES ===\n\n");

    for (int i = 0; i < nombreMaladies; i++)
    {
        printf("\n%d. %s\n", i + 1, maladies[i].nom);
        printf("   Symptomes: ");
        for (int j = 0; j < maladies[i].nb_symptomes; j++)
        {
            printf("%s", maladies[i].symptomes[j]);
            if (j < maladies[i].nb_symptomes - 1) printf(", ");
        }
        printf("\n   Description: %s\n", maladies[i].description);
        printf("   Traitement: %s\n", maladies[i].traitement);
        printf("   Precautions: %s\n", maladies[i].precautions);
    }
}

void diagnosticParSymptomes()
{
    system("cls");
    printf("\n=== DIAGNOSTIC PAR SYMPTOMES ===\n\n");

    char symptomes_entres[MAX_SYMPTOMS][MAX_SYMPTOM_LENGTH];
    int nb_symptomes_entres = 0;

    printf("Entrez vos symptomes (un par ligne, 'fin' pour terminer):\n");
    printf("Exemples: fievre, toux, maux de tete, nausees, etc.\n\n");

    // Saisie des symptômes
    while (nb_symptomes_entres < MAX_SYMPTOMS)
    {
        printf("Symptome %d: ", nb_symptomes_entres + 1);
        fgets(symptomes_entres[nb_symptomes_entres], MAX_SYMPTOM_LENGTH, stdin);
        symptomes_entres[nb_symptomes_entres][strcspn(symptomes_entres[nb_symptomes_entres], "\n")] = '\0';

        if (strcmp(symptomes_entres[nb_symptomes_entres], "fin") == 0)
        {
            break;
        }

        if (strlen(symptomes_entres[nb_symptomes_entres]) > 0)
        {
            nb_symptomes_entres++;
        }
    }

    if (nb_symptomes_entres == 0)
    {
        printf("\nAucun symptome entre.\n");
        pause();
        return;
    }

    // Analyse des symptômes
    printf("\n=== ANALYSE EN COURS ===\n");
    printf("\nSymptomes entres (%d):\n", nb_symptomes_entres);
    for (int i = 0; i < nb_symptomes_entres; i++)
    {
        printf("  - %s\n", symptomes_entres[i]);
    }

    printf("\n=== RESULTATS DU DIAGNOSTIC ===\n\n");

    int trouve_diagnostic = 0;
    float meilleur_score = 0.0;
    int meilleure_maladie = -1;

    // Analyser chaque maladie
    for (int m = 0; m < nombreMaladies; m++)
    {
        int correspondances = 0;

        // Compter les correspondances de symptômes
        for (int s1 = 0; s1 < nb_symptomes_entres; s1++)
        {
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
            {
                if (comparerSymptomes(symptomes_entres[s1], maladies[m].symptomes[s2]))
                {
                    correspondances++;
                    break; // Un symptôme ne correspond qu'à un seul symptôme de la maladie
                }
            }
        }

        // Calculer le pourcentage de correspondance
        float score = (float)correspondances / maladies[m].nb_symptomes * 100;

        // Afficher les maladies avec au moins 40% de correspondance
        if (score >= 40.0)
        {
            trouve_diagnostic = 1;
            printf("\n-> %s (%.0f%% de correspondance)\n", maladies[m].nom, score);
            printf("   Symptomes correspondants: %d/%d\n", correspondances, maladies[m].nb_symptomes);
            printf("   Description: %s\n", maladies[m].description);
            printf("   Traitement recommande: %s\n", maladies[m].traitement);
            printf("   Precautions: %s\n", maladies[m].precautions);

            // Garder la meilleure correspondance
            if (score > meilleur_score)
            {
                meilleur_score = score;
                meilleure_maladie = m;
            }
        }
    }

    if (!trouve_diagnostic)
    {
        printf("\nAucune maladie ne correspond suffisamment aux symptomes entres.\n");
        printf("Consultez un medecin pour un diagnostic precis.\n");
    }
    else if (meilleure_maladie != -1 && meilleur_score >= 70.0)
    {
        printf("\n=== DIAGNOSTIC PROBABLE ===\n");
        printf("Maladie la plus probable: %s (%.0f%%)\n", maladies[meilleure_maladie].nom, meilleur_score);
        printf("Il est fortement recommande de consulter un medecin.\n");
    }

    pause();
}

void diagnosticPatientExistant(int index_patient)
{
    if (index_patient < 0 || index_patient >= nombrePatients)
    {
        printf("Erreur: Patient non trouve.\n");
        return;
    }

    Patient *p = &patients[index_patient];

    if (p->nb_symptomes == 0)
    {
        printf("\nCe patient n'a aucun symptome enregistre.\n");
        pause();
        return;
    }

    system("cls");
    printf("\n=== DIAGNOSTIC POUR %s %s (ID: %d) ===\n\n",
           p->prenom, p->nom, p->id);

    printf("Symptomes du patient (%d):\n", p->nb_symptomes);
    for (int i = 0; i < p->nb_symptomes; i++)
    {
        printf("  - %s\n", p->symptomes[i]);
    }

    printf("\n=== ANALYSE EN COURS ===\n\n");

    int trouve_diagnostic = 0;
    float meilleur_score = 0.0;
    int meilleure_maladie = -1;

    // Analyser chaque maladie
    for (int m = 0; m < nombreMaladies; m++)
    {
        int correspondances = 0;

        // Compter les correspondances de symptômes
        for (int s1 = 0; s1 < p->nb_symptomes; s1++)
        {
            for (int s2 = 0; s2 < maladies[m].nb_symptomes; s2++)
            {
                if (comparerSymptomes(p->symptomes[s1], maladies[m].symptomes[s2]))
                {
                    correspondances++;
                    break;
                }
            }
        }

        // Calculer le pourcentage de correspondance
        float score = (float)correspondances / maladies[m].nb_symptomes * 100;

        // Afficher les maladies avec au moins 40% de correspondance
        if (score >= 40.0)
        {
            trouve_diagnostic = 1;
            printf("\n-> %s (%.0f%% de correspondance)\n", maladies[m].nom, score);
            printf("   Symptomes correspondants: %d/%d\n", correspondances, maladies[m].nb_symptomes);

            if (score > meilleur_score)
            {
                meilleur_score = score;
                meilleure_maladie = m;
            }
        }
    }

    if (!trouve_diagnostic)
    {
        printf("\nAucune maladie ne correspond suffisamment aux symptomes du patient.\n");
    }
    else
    {
        printf("\n=== RECOMMANDATION ===\n");

        if (meilleur_score >= 70.0)
        {
            printf("\nDiagnostic probable: %s (%.0f%%)\n", maladies[meilleure_maladie].nom, meilleur_score);
            printf("Description: %s\n", maladies[meilleure_maladie].description);
            printf("Traitement: %s\n", maladies[meilleure_maladie].traitement);

            printf("\nVoulez-vous enregistrer ce diagnostic pour le patient? (O/N): ");
            char reponse;
            scanf("%c", &reponse);
            viderBuffer();

            if (toupper(reponse) == 'O')
            {
                strcpy(p->diagnostic, maladies[meilleure_maladie].nom);
                sauvegarderPatients();
                printf("Diagnostic enregistre avec succes!\n");
            }
        }
        else if (meilleur_score >= 50.0)
        {
            printf("\nPlusieurs maladies possibles. La plus probable: %s (%.0f%%)\n",
                   maladies[meilleure_maladie].nom, meilleur_score);
            printf("Il est recommande de faire des examens complementaires.\n");
        }
        else
        {
            printf("\nCorrespondance insuffisante pour un diagnostic precis.\n");
            printf("Consultez un medecin pour des examens plus pousses.\n");
        }
    }

    pause();
}

void menuDiagnosticMedical()
{
    int choix;

    do
    {
        system("cls");
        color(11, 0); // Cyan
        printf("\n=== SYSTEME DE DIAGNOSTIC MEDICAL ===\n\n");
        color(7, 0); // Blanc

        printf("1. Diagnostic par symptomes (nouveau patient)\n");
        printf("2. Diagnostic d'un patient existant\n");
        printf("3. Voir la base de donnees des maladies\n");
        printf("4. Retour au menu principal\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            diagnosticParSymptomes();
            break;
        case 2:
            if (nombrePatients == 0)
            {
                printf("\nAucun patient enregistre.\n");
                printf("Ajoutez d'abord un patient.\n");
                pause();
            }
            else
            {
                afficherListePatientsSimple();
                printf("\nEntrez l'ID du patient pour diagnostic: ");
                int id;
                scanf("%d", &id);
                viderBuffer();

                int index = rechercherParID(id);
                if (index != -1)
                {
                    diagnosticPatientExistant(index);
                }
                else
                {
                    printf("Patient non trouve.\n");
                    pause();
                }
            }
            break;
        case 3:
            afficherToutesMaladies();
            pause();
            break;
        case 4:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 4);
}

// ============================================
// MENUS
// ============================================

void menuAffichagePatients()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== AFFICHAGE DES PATIENTS ===\n\n");
        printf("1. Liste simple des patients\n");
        printf("2. Rechercher un patient\n");
        printf("3. Retour\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            afficherListePatientsSimple();
            pause();
            break;
        case 2:
            rechercherPatient();
            break;
        case 3:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 3);
}

void menuGestionPatients()
{
    int choix;

    do
    {
        system("cls");
        printf("\n=== GESTION DES PATIENTS ===\n\n");
        printf("1. Ajouter un nouveau patient\n");
        printf("2. Afficher/rechercher patients\n");
        printf("3. Statistiques\n");
        printf("4. Sauvegarder les donnees\n");
        printf("5. Retour au menu principal\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            ajouterNouveauPatient();
            break;
        case 2:
            menuAffichagePatients();
            break;
        case 3:
            afficherStatistiques();
            break;
        case 4:
            sauvegarderPatients();
            printf("Donnees sauvegardees avec succes!\n");
            pause();
            break;
        case 5:
            return;
        default:
            printf("Choix invalide.\n");
            pause();
        }

    } while(choix != 5);
}

void menuPrincipal()
{
    int choix;

    do
    {
        system("cls");
        color(10, 0);
        printf("  SICKBASE- SYSTEME DE GESTION HOSPITALIER    \n");
        printf("      Avec DIAGNOSTIC AUTOMATIQUE     \n");
        printf("               Version 3.0            \n");
        color(7, 0);

        printf("1. Gestion des patients\n");
        printf("2. Diagnostic medical\n");
        printf("3. Gestion du personnel\n");
        printf("4. Gestion des medicaments\n");
        printf("5. Archives\n");
        printf("6. Quitter\n");
        printf("\nVotre choix: ");

        scanf("%d", &choix);
        viderBuffer();

        switch(choix)
        {
        case 1:
            menuGestionPatients();
            break;
        case 2:
            menuDiagnosticMedical();
            break;
        case 3:
            printf("\nGestion du personnel (a implementer)\n");
            pause();
            break;
        case 4:
            printf("\nGestion des medicaments (a implementer)\n");
            pause();
            break;
        case 5:
            printf("\nArchives (a implementer)\n");
            pause();
            break;
        case 6:
            sauvegarderPatients();
            printf("\nDonnees sauvegardees. Au revoir!\n");
            break;
        default:
            printf("\nChoix invalide!\n");
            pause();
        }

    } while(choix != 6);
}

// ============================================
// FONCTION PRINCIPALE
// ============================================

int main()
{
    system("cls");
    color(14, 0);
    printf("\nCHARGEMENT DU SYSTEME HOSPITALIER...\n");
    color(7, 0);

    // Initialiser la base de données des maladies
    initialiserMaladies();

    // Charger les données existantes
    chargerDernierID();
    chargerPatients();

    printf("\nSysteme de diagnostic medical charge!\n");
    printf("Base de connaissances: %d maladies\n", nombreMaladies);
    printf("Patients dans la base: %d\n", nombrePatients);

    Sleep(1500);

    menuPrincipal();

    return 0;
}
