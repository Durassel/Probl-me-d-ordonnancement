#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

// Stockage des durées et contraintes
typedef struct {
    int		                nbTaches;		            // Tâches numérotées de 0 à nbTaches - 1
    char*	                nomTaches;		            // Tâches représentées par des caractères
    int*	                durees;			            // Durées des tâches représentées par des entiers
    vector<vector<char> >   contraintes;	            // Contraintes représentées par des caractères (Tableau 2D)
} tt_contraintes;

// Stockage du graphe construit à partir des contraintes
typedef struct {
    int		                nbSommets;	                // Nombre de tâches du graphe (nbTaches + 2 : alpha + omega)
    bool**	                adj;		                // Matrice d'adjacence
    int**                   val;		                // Matrice des valeurs (val[x][y] significative si adj[x][y] = true)
} tt_graphe;

// Ajout d'une contrainte au tableau dynamique des contraintes
void ajouterContrainte(tt_contraintes *lesContraintes, char tacheCourante, char contrainteCourante) {
    int indice = 0;
    // On cherche déjà l'indice où l'on va ajouter la contrainte
    for (int i = 0; i < lesContraintes->nbTaches; i++) {
        if (lesContraintes->nomTaches[i] == tacheCourante) {
            indice = i;
        }
    }
    // Ajout de la contrainte
    bool ajout = true;
    for (int i = 0; i < lesContraintes->contraintes[indice].size(); i++) {
        // Si la contrainte est déjà présente dans notre tableau, il n'est pas nécessaire de l'ajouter une seconde fois
        if (lesContraintes->contraintes[indice].size() != 0 && lesContraintes->contraintes[indice][i] == contrainteCourante) {
            ajout = false;
        }
    }
    // Si ajout vaut 'true', la contrainte n'existe pas dans le tableau, on l'ajoute (dans le cas contraire, on ne l'ajoute pas)
    if (ajout == true) {
        lesContraintes->contraintes[indice].push_back(contrainteCourante);
    }
}

// Affichage de la matrice adjacente (celle compris dans notre structure de données)
void affichageAdj(tt_graphe *leGraphe, tt_contraintes *lesContraintes) {
    // Indice de début et fin de projet
    int debut   = 0;
    int fin     = leGraphe->nbSommets - 1;

    // Header du tableau (Lettres des sommets)
    cout << endl << endl << "Matrice d\'adjacence booleenne :" << endl << endl;
    printf("%3c ", ' '); // Pour gérer l'alignement des colonnes
    printf("%3c ", 'a'); // Le graphe commence par le début de projet (alpha : lettre a)
    for (int ligne = 0; ligne < leGraphe->nbSommets-2; ligne++) {
        printf("%3c ", lesContraintes->nomTaches[ligne]);
    }
    printf("%3c \n", 'z'); // Le graphe se finit par la fin de projet (omega : lettre z)

    // Contenu du tableau
    for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
        // Affichage des sommets
        if (ligne == debut)
            printf("%3c ", 'a');
        else if (ligne == fin)
            printf("%3c ", 'z');
        else
            printf("%3c ", lesContraintes->nomTaches[ligne - 1]);

        // Affichage des valeurs de la matrice adjacente
        for (int colonne = 0; colonne < leGraphe->nbSommets; colonne++) {
            if (leGraphe->adj[ligne][colonne]) {
                printf("%3d ", 1);
            } else {
                printf("%3d ", 0);
            }
        }
        cout << endl;
    }
    cout << endl;
}
// Surchage (Affichage de la matrice adjacente temporaire lors du calcul du rang : car modification de la matrice durant le calcul)
void affichageAdj(bool** adj, int nbSommets, tt_contraintes *lesContraintes) {
    // Indice de début et fin de projet
    int debut   = 0;
    int fin     = nbSommets - 1;

    // Header du tableau (Lettres des sommets)
    cout << endl << endl << "Matrice d\'adjacence booleenne :" << endl << endl;
    printf("%3c ", ' '); // Pour gérer l'alignement des colonnes
    printf("%3c ", 'a'); // Le graphe commence par le début de projet (alpha : lettre a)
    for (int ligne = 0; ligne < nbSommets-2; ligne++) {
        printf("%3c ", lesContraintes->nomTaches[ligne]);
    }
    printf("%3c \n", 'z'); // Le graphe se finit par la fin de projet (omega : lettre z)

    // Contenu du tableau
    for (int ligne = 0; ligne < nbSommets; ligne++) {
        // Affichage des sommets
        if (ligne == debut)
            printf("%3c ", 'a');
        else if (ligne == fin)
            printf("%3c ", 'z');
        else
            printf("%3c ", lesContraintes->nomTaches[ligne - 1]);

        for (int colonne = 0; colonne < nbSommets; colonne++) {
            if (adj[ligne][colonne]) {
                printf("%3d ", 1);
            } else {
                printf("%3d ", 0);
            }
        }
        cout << endl;
    }
    cout << endl;
}

// Affichage de la matrice de valeurs
void affichageVal(tt_graphe *leGraphe, tt_contraintes *lesContraintes) {
    // Indice de début et fin de projet
    int debut   = 0;
    int fin     = leGraphe->nbSommets - 1;

    // Header du tableau (Lettres des sommets)
    cout << endl << endl << "Matrice des valeurs :" << endl << endl;
    printf("%3c ", ' '); // Pour gérer l'alignement des colonnes
    printf("%3c ", 'a'); // Le graphe commence par le début de projet (alpha : lettre a)
    for (int ligne = 0; ligne < leGraphe->nbSommets-2; ligne++) {
        printf("%3c ", lesContraintes->nomTaches[ligne]);
    }
    printf("%3c \n", 'z'); // Le graphe se finit par la fin de projet (omega : lettre z)

    // Contenu du tableau
    for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
        // Affichage des sommets
        if (ligne == debut)
            printf("%3c ", 'a');
        else if (ligne == fin)
            printf("%3c \n", 'z');
        else
            printf("%3c ", lesContraintes->nomTaches[ligne - 1]);
        // Affichage des valeurs de la matrice des valeurs
        for (int colonne = 0; colonne < leGraphe->nbSommets; colonne++) {
            if (leGraphe->val[ligne][colonne] != -1)
                printf("%3d ", leGraphe->val[ligne][colonne]);
            else
                printf("%3c ", ' ');
        }
        cout << endl;
    }
    cout << endl;
}

// Affichage d'un quelconque tableau
void affichageTab(int *tab, int taille, tt_contraintes *lesContraintes) {
    // Header du tableau (Lettres des sommets)
    printf("%3c ", 'a'); // Le graphe commence par le début de projet (alpha : lettre a)
    for (int i = 0; i < lesContraintes->nbTaches; i++) {
        printf("%3c ", lesContraintes->nomTaches[i]);
    }
    printf("%3c \n", 'z'); // Le graphe se finit par la fin de projet (omega : lettre z)
    // Contenu du tableau
    for (int i = 0; i < taille; i++) {
        if (tab[i] == -1) // Les cases vides (égales à -1) ne sont pas affichées (remplacées par des espaces)
            printf("%3c ", ' ');
        else
            printf("%3d ", tab[i]);
    }
    cout << endl << endl;
}

// Calcul du nombre de prédécesseur(s) d'un sommet à partir de la matrice adjacente passée en argument
int getNbPredecesseur(int indice, bool** nouvelleAdj, int nbSommets) {
    int sum = 0;
    // Pour connaitre le nombre de prédécesseur(s) d'un sommet, il suffit de compter le nombre de case(s) à 'true' dans la colonne du sommet
    for (int i = 0; i < nbSommets; i++) {
        // Si on trouve 'true', un arc existe, on l'additionne à la somme
        if(nouvelleAdj[i][indice] == true)
            sum++;
    }
    return sum;
}

// Calcul du rang
void calculRang(int *rang, tt_graphe *leGraphe, tt_contraintes *lesContraintes) {
    // Déclaration de variables permettant le calcul du rang du graphe
    vector<int>     entrees;                                // Tableau contenant le(s) début(s) de projet au cours du calcul du rang (lors de la suppression de(s) début(s) de projet)
    int             nbPredecesseur[leGraphe->nbSommets];    // Tableau contenant le nombre de prédécesseur(s) de chaque sommet
    int             sum = 0;                                // Condition d'arrêt du calcul du rang (toutes les cases ont été remplies)
    int             indiceRang = 0;                         // Compteur du rang (commence à 0)

    // Utilisation d'un tableau temporaire pour la matrice adjacente (pour ne pas modifier leGraphe->adj dans le main : leGraphe a été passé par pointeur)
    bool** nouvelleAdj;
    nouvelleAdj = new bool*[leGraphe->nbSommets];
    for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
        nouvelleAdj[ligne] = new bool[leGraphe->nbSommets];
    }
    // Initialisation (avec les mêmes valeurs que la matrice adjacente du Graphe)
    for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
        for (int colonne= 0; colonne < leGraphe->nbSommets; colonne++) {
            nouvelleAdj[ligne][colonne] = leGraphe->adj[ligne][colonne];
        }
    }

    // Initialisation des tableaux locaux à la fonction
    for (int i = 0; i < leGraphe->nbSommets; i++) {
        nbPredecesseur[i] = 0;
        rang[i] = -1; // Pour un affichage plus propre
    }

    // Calcul du nombre de prédécesseur(s) de chaque sommet (du graphe initial)
    for (int i = 0; i < leGraphe->nbSommets; i++) {
        nbPredecesseur[i] = getNbPredecesseur(i, nouvelleAdj, leGraphe->nbSommets);
    }

    // Boucle permettant de calculer tous les rangs
    do {
        // Réinitialisation de la somme (condition d'arrêt)
        sum = 0;

        // Traces d'exécution
        affichageAdj(nouvelleAdj, leGraphe->nbSommets, lesContraintes);
        cout << "Nombre de predecesseurs :" << endl;
        affichageTab(nbPredecesseur, leGraphe->nbSommets, lesContraintes);

        // Détection de(s) nouveau(x) début(s) de projet
        for (int i = 0; i < leGraphe->nbSommets; i++) {
            // Le(s) début(s) de projet n'ont pas de prédécesseur (== 0)
            if (nbPredecesseur[i] == 0) {
                // On vérifie que le début de projet n'a pas déjà été inséré dans le tableau 'entrees'
                bool ajout = true;
                for (int j = 0; j < entrees.size(); j++) { // On parcours le tableau d'entrées
                    if (entrees[j] == i) {
                        ajout = false;
                    }
                }
                // Si le début de projet n'existe pas dans 'entrees', on l'ajoute
                if (ajout == true) {
                    entrees.push_back(i);
                    rang[i] = indiceRang;
                }
            }
        }

        // Condition d'arrêt (tous les sommets ont été supprimés / tous les sommets n'ont plus aucun prédécesseur)
        for (int i = 0; i < leGraphe->nbSommets; i++) {
            sum += nbPredecesseur[i]; // Si la somme du nombre des prédécesseurs de chaque sommet est nulle, le tableau a été rempli
        }

        // Suppression des arcs du début de projet (en supprimant ces arcs, le nombre de prédécesseurs des sommets diminuent au fur et à mesure : technique de calcul du rang vu en TD)
        for (int i = 0; i < entrees.size(); i++) {                                      // Pour chaque début de projet
            for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
                if (ligne == entrees[i]) {
                    for (int colonne = 0; colonne < leGraphe->nbSommets; colonne++) {   // On supprime les arcs sortant du début de projet (colonne(s) contenant 'true')
                        if (nouvelleAdj[ligne][colonne] == true) {
                            nouvelleAdj[ligne][colonne] = false;
                        }
                    }
                }
            }
        }

        // Calcul du nombre de prédécesseur(s) de chaque sommet (pour la nouvelle matrice : le(s) début(s) de projet a(ont) été supprimé(s))
        for (int i = 0; i < leGraphe->nbSommets; i++) {
            nbPredecesseur[i] = getNbPredecesseur(i, nouvelleAdj, leGraphe->nbSommets);
        }
        // Traces d'exécution
        cout << "Rang :" << endl;
        affichageTab(rang, leGraphe->nbSommets, lesContraintes);
        // Passage au rang suivant
        indiceRang++;
        getchar();
    } while(sum > 0);
}

// Calcul du maximum d'un tableau
int Max(int *tab, int taille) {
    int maximum = 0; // Int minimum : pour éviter tous bugs (arcs à valeur négative par exemple)
    for (int i = 0; i < taille; i++) {
        // Si on trouve une valeur supérieure au maximum actuel, on remplace notre maximum
        if (maximum < tab[i])
            maximum = tab[i];
    }
    return maximum;
}
// Surchage (même fonction mais avec un vector en argument)
int Max(vector<int> tab, int taille) {
    int maximum = 0; // Int minimum : pour éviter tous bugs (arcs à valeur négative par exemple)
    for (int i = 0; i < taille; i++) {
        // Si on trouve une valeur supérieure au maximum actuel, on remplace notre maximum
        if (maximum < tab[i])
            maximum = tab[i];
    }
    return maximum;
}

// Calcul du minimum d'un tableau
int Min(vector<int> tab, int taille) {
    int minimum = 2147483647; // Int maximum : pour éviter tous bugs
    for (int i = 0; i < taille; i++) {
        // Si on trouve une valeur inférieure au minimum actuel, on remplace notre minimum
        if (minimum > tab[i])
            minimum = tab[i];
    }
    return minimum;
}

// Calcul du calendrier au plus tôt
void calendrierAuPlusTot(int *auPlusTot, int *rang, tt_graphe *leGraphe, tt_contraintes *lesContraintes) {
    cout << "Calendrier au plus tot :" << endl << endl;
    // On calcul le rang maximum contenu dans notre tableau rang
    int rangMax = Max(rang, leGraphe->nbSommets);

    // On parcours tous les rangs (de 0 à rangMax) dans l'ordre
    for (int indiceRang = 0; indiceRang <= rangMax; indiceRang++) {
        cout << endl << "Rang : " << indiceRang << endl; // Traces d'exécution
        for (int i = 0; i < leGraphe->nbSommets; i++) {
            vector<int> maximum; // On enregistre les calculs des dates au plus tôt de chaque sommet pour ne garder que le maximum (formule de la date au plus tôt)
            // On parcours dans l'ordre croissant des rangs (les rangs des sommets ne sont pas forcément triés)
            if (indiceRang == rang[i]) {
                // Traces d'exécution
                if (indiceRang == 0)
                    cout << "\ta :" << endl;
                else if (indiceRang == rangMax)
                    cout << "\tz :" << endl;
                else
                    cout << "\t" << (char)('A' + i - 1) << " :" << endl;

                if (indiceRang == 0) {
                    auPlusTot[i] = 0; // Initialisation de la date au rang 0 à 0
                } else {
                    // On parcours chaque prédécesseur du sommet afin d'additionner la valeur de l'arc à la date au plus tôt, pour stocker cette valeur dans le tableau maximum
                    for (int j = 0; j < leGraphe->nbSommets; j++) {
                        if (leGraphe->val[j][i] != -1) {
                            maximum.push_back(leGraphe->val[j][i] + auPlusTot[j]); // Ajout du potentiel maximum
                            // Traces d'exécution
                            cout << "\t\tD+tot(";
                            if (j == 0)
                                cout << "a) + v(";
                            else if (indiceRang == rangMax)
                                cout << "z) + v(";
                            else
                                cout << (char)('A' + j - 1) << ") + v(";

                            if (j == 0)
                                cout << "a," << (char)('A' + i - 1) << ")";
                            else if (indiceRang == rangMax)
                                cout << "z," << (char)('A' + i - 1) << ")";
                            else
                                cout << (char)('A' + j - 1) << "," << (char)('A' + i - 1) << ")";
                            cout << " = " << auPlusTot[j] << " + " << leGraphe->val[j][i] << " = " << auPlusTot[j] + leGraphe->val[j][i] << endl;
                        }
                    }
                }
                // La date au plus tôt est le maximum
                auPlusTot[i] = Max(maximum, maximum.size()); // On effectue cette opération pour tous les sommets
                cout << "\t\tDate au plus tot : Maximum : " << auPlusTot[i] << endl << endl; // Traces d'exécution
            }
        }
        getchar();
    }
    // Traces d'exécution
    cout << endl << "Calendrier au plus tot :" << endl << endl;
    affichageTab(auPlusTot, leGraphe->nbSommets, lesContraintes);
}

// Calcul du calendrier au plus tard
void calendrierAuPlusTard(int *auPlusTard, int *auPlusTot, int *rang, tt_graphe *leGraphe, tt_contraintes *lesContraintes) {
    cout << endl << "Calendrier au plus tard :" << endl << endl;
    // On calcul le rang maximum contenu dans notre tableau rang
    int rangMax = Max(rang, leGraphe->nbSommets);

    // On parcours tous les rangs (de rangMax à 0) dans l'ordre
    for (int indiceRang = rangMax; indiceRang >= 0; indiceRang--) {
        cout << endl << "Rang : " << indiceRang << endl; // Traces d'exécution
        for (int i = leGraphe->nbSommets - 1; i >= 0; i--) {
            vector<int> minimum; // On enregistre les calculs des dates au plus tard de chaque sommet pour ne garder que le minimum (formule de la date au plus tard)
            // On parcours dans l'ordre croissant des rangs (les rangs des sommets ne sont pas forcément triés)
            if (indiceRang == rang[i]) {
                // Traces d'exécution
                if (indiceRang == 0)
                    cout << "\ta :" << endl;
                else if (indiceRang == rangMax)
                    cout << "\tz :" << endl;
                else
                    cout << "\t" << (char)('A' + i - 1) << " :" << endl;

                if (indiceRang == rangMax) {
                    auPlusTard[i] = auPlusTot[leGraphe->nbSommets - 1]; // Initialisation de la date au plus tard de fin de projet (même valeur que la date au plus tôt de fin de projet)
                    minimum.push_back(auPlusTard[i]);
                } else {
                    // On parcours chaque prédécesseur du sommet afin de soustraire la date au plus tard à la valeur de l'arc, pour stocker cette valeur dans le tableau minimum
                    for (int j = 0; j < leGraphe->nbSommets; j++) {
                        if (leGraphe->val[i][j] != -1) {
                            minimum.push_back(auPlusTard[j] - leGraphe->val[i][j]); // Ajout du potentiel minimum
                            // Traces d'exécution
                            cout << "\t\tD+tard(";
                            if (j == 0)
                                cout << "a) - v(";
                            else if (j == leGraphe->nbSommets - 1)
                                cout << "z) - v(";
                            else
                                cout << (char)('A' + j - 1) << ") - v(";

                            if (indiceRang == 0)
                                cout << "a," << (char)('A' + j - 1) << ")";
                            else if (j == leGraphe->nbSommets - 1)
                                cout << "z," << (char)('A' + j - 1) << ")";
                            else
                                cout << (char)('A' + i - 1) << "," << (char)('A' + j - 1) << ")";
                            cout << " = " << auPlusTard[j] << " - " << leGraphe->val[i][j] << " = " << auPlusTard[j] - leGraphe->val[i][j] << endl;
                        }
                    }
                }
                // La date au plus tard est le minimum
                auPlusTard[i] = Min(minimum, minimum.size()); // On effectue cette opération pour tous les sommets
                cout << "\t\tDate au plus tard : Minimum : " << auPlusTard[i] << endl << endl; // Traces d'exécution
                // Vérification du bon déroulement de l'algorithme
                if (indiceRang == 0 && auPlusTard[0] != auPlusTot[0]) {
                    cout << "\t\tErreur lors de l'algorithme : auPlusTard(a) = " << auPlusTard[0] << " different de auPlusTot(a) = " << auPlusTot[0] << "." << endl;
                }
            }
        }
        getchar();
    }
    // Traces d'exécution
    cout << endl << "Calendrier au plus tard :" << endl << endl;
    affichageTab(auPlusTard, leGraphe->nbSommets, lesContraintes);
}

// Calcul de la marge totale
void calculMargeTotale(int *margeTotale, int *auPlusTard, int *auPlusTot, int nbSommets) {
    // Il suffit de faire la différence entre les dates au plus tard et les dates au plus tôt
    for (int i = 0; i < nbSommets; i++) {
        margeTotale[i] = auPlusTard[i] - auPlusTot[i];
    }
}

// Calcul des marges libres
void calculMargeLibre(int *margeLibre, int *auPlusTot, tt_graphe *leGraphe) {
    // On calcul la marge libre de chaque sommet
    for (int i = 0; i < leGraphe->nbSommets; i++) {
        vector<int> minimum; // La formule pour calculer la marge libre fait intervenir le minimum
        // Traces d'exécution
        if (i == 0)
            cout << "a :" << endl;
        else if (i == leGraphe->nbSommets - 1)
            cout << "z :" << endl;
        else
            cout << (char)('A' + i - 1) << " :" << endl;

        // On parcours la matrice de valeurs afin de trouver le(s) successeur(s) du sommet
        for (int j = 0; j < leGraphe->nbSommets; j++) {
            if (i == leGraphe->nbSommets - 1) {
                minimum.push_back(0); // La fin de projet a une marge libre de 0
            } else if (leGraphe->val[i][j] != -1) { // Si le sommet a un successeur, on calcule la marge libre
                // On sauvegarde les valeurs dans un tableau pour, à la fin, ne garder que le minimum
                minimum.push_back(auPlusTot[j] - leGraphe->val[i][j] - auPlusTot[i]); // Date au plus tôt du successeur - la valeur de l'arc - la date au plus tôt du sommet en question
                // Traces d'exécution
                cout << "\tD+tot(";
                if (j == 0)
                    cout << "a";
                else if (j == leGraphe->nbSommets - 1)
                    cout << "z";
                else
                    cout << (char)('A' + j - 1);

                cout << ") - val(";
                if (i == 0)
                    cout << "a";
                else if (i == leGraphe->nbSommets - 1)
                    cout << "z";
                else
                    cout << (char)('A' + i - 1);
                cout << ",";

                if (j == 0)
                    cout << "a";
                else if (j == leGraphe->nbSommets - 1)
                    cout << "z";
                else
                    cout << (char)('A' + j - 1);

                cout << ") - D+tot(";
                if (i == 0)
                    cout << "a";
                else if (i == leGraphe->nbSommets - 1)
                    cout << "z";
                else
                    cout << (char)('A' + i - 1);
                cout << ") = " << auPlusTot[j] << " - " << leGraphe->val[i][j] << " - " << auPlusTot[i] << " = " << auPlusTot[j] - leGraphe->val[i][j] - auPlusTot[i] << endl;
            }
        }
        // On garde le minimum
        margeLibre[i] = Min(minimum, minimum.size());
        cout << "\tMinimum : " << margeLibre[i] << endl << endl;
        getchar();
    }
}

// Affichage des calendrier selon la méthode de Gantt
void gantt(int *calendrier, tt_contraintes *lesContraintes) { // On prend en paramètres un calendrier (que ce soit au plus tôt ou au plus tard)
    int debut = calendrier[0];
    int fin = calendrier[lesContraintes->nbTaches + 1];

    printf("%3c", ' '); // Pour gérer l'alignement des colonnes
    // Affichage du temps (de 0 à date de fin de projet)
    for (int i = debut; i <= fin; i++) {
        printf("%3d ", i);
    }

    cout << endl;
    // Affichage de la durée de toutes les tâches
    for (int i = 0; i < lesContraintes->nbTaches; i++) {
        // Affichage du nom de la tâche
        printf("%2c%3c", lesContraintes->nomTaches[i], ' ');
        // Affichage du calendrier selon la méthode de Gantt
        int cpt = 0;
        // Affichage des espaces jusqu'au début de la tâche
        for (int j = 0; j < calendrier[i + 1]; j++) {
            printf("|%3c", ' ');
            cpt++;
        }
        // Affichage de la tâche sur toute sa durée
        for (int j = 0; j < lesContraintes->durees[i]; j++) {
            printf("|%2c%c", '=', ' ');
            cpt++;
        }
        // Affichage des derniers espaces jusqu'à la fin de projet
        for (int j = 0; j < fin - cpt; j++) {
            printf("|%3c", ' ');
        }
        cout << endl;
    }

    cout << endl << endl;
    getchar();
}

// Vérification de la présence d'un circuit
bool verifCircuit(char tacheDebutCircuit, char tacheCourante, char contrainteCourante, tt_contraintes *lesContraintes) { // Fonction récursive
    /*
    // Traces d'exécution
    cout << endl << "\tTache debut circuit : " << tacheDebutCircuit << endl;
    cout << "\tTache courante : " << tacheCourante << endl;
    cout << "\tContrainte courante : " << contrainteCourante << endl;
    */

    // Si l'on retombe sur la même tâche, il y a présence d'un circuit
    if (tacheDebutCircuit == contrainteCourante) {
        return true; // On retourne 'true' s'il y a un circuit (pour pouvoir supprimer l'arc par la suite)
    }

    // On parcours le tableau de contraintes
    for (int i = 0; i < lesContraintes->nbTaches; i++) {
        // Afin de voir où nous mène l'arc
        if (lesContraintes->nomTaches[i] == contrainteCourante && lesContraintes->contraintes[i].size() > 0) {
            // On regarde pour toutes les contraintes
            for (int j = 0; j < lesContraintes->contraintes[i].size(); j++) {
                // Appel de la même fonction mais avec la tâche courante et la contrainte courante suivante
                return verifCircuit(tacheDebutCircuit, contrainteCourante, lesContraintes->contraintes[i][j], lesContraintes);
            }
        }
    }

    // On retourne 'false' s'il n'y a pas de circuit
    return false;
}

int main(void) {
    // Pour faire tourner le programme en boucle
    char reponse;

    do {
        cout << "-------------------------------------------------------------------------------------------" << endl;
        cout << "\t\t\t\tProbleme d\'ordonnancement" << endl;
        cout << "-------------------------------------------------------------------------------------------" << endl << endl;

        // Lecture des tâches et des contraintes à partir du fichier
        cout << "---------------------------------------------------" << endl;
        cout << "\t\tLecture du fichier" << endl;
        cout << "---------------------------------------------------" << endl << endl;

        cout << "Veuillez entrer le nom du fichier .txt a ouvrir (sans extension) : ";
        string fichier;
        cin >> fichier;
        cin.ignore(1, '\n'); // Vider le buffer d'entrée clavier pour que les getchar() (pause) fonctionnent
        fichier += ".txt";
        ifstream F(fichier.c_str());
        bool erreur = false;
        if (!F) { // Erreur lors de l'ouverture du fichier
            cerr << "Impossible d'ouvrir le fichier." << endl;
            erreur = true;
        }

        if (erreur == false) {
            // Déclaration de nos variables
            tt_contraintes* lesContraintes = new tt_contraintes;
            char tacheCourante, contrainteCourante;
            int dureeTache;

            // Lecture du fichier : Ligne 1 : Nombre de tâches
            F >> lesContraintes->nbTaches;
            cout << "Nombre de taches : " << lesContraintes->nbTaches << endl;

            // Allocation dynamique des tableaux
            lesContraintes->nomTaches = new char[lesContraintes->nbTaches];     // Indices : de 0 à nbTaches-1
            lesContraintes->durees = new int[lesContraintes->nbTaches];         // Indices : de 0 à nbTaches-1
            lesContraintes->contraintes.resize(lesContraintes->nbTaches);       // Indices : de 0 à nbTaches-1

            // Lecture du fichier : Ligne 2 jusqu'à nbTaches + 1 : Durée des tâches
            cout << endl << "Durees :" << endl;
            for (int i = 0; i < lesContraintes->nbTaches; i++) {
                F >> tacheCourante;
                lesContraintes->nomTaches[i] = tacheCourante; // Chaque case du tableau reçoit une tâche (lettre)
                F >> dureeTache;
                lesContraintes->durees[i] = dureeTache; // Chaque case du tableau reçoit la durée de la tâche (au même indice)
                cout << tacheCourante << "\t" << lesContraintes->durees[i] << endl;
            }

            // Lecture du fichier : Lignes suivantes : Contraintes (une par ligne)
            cout << endl<<"Contraintes :" << endl;
            for (int i = 0; i < lesContraintes->nbTaches; i++) {
                F >> tacheCourante;
                F >> contrainteCourante;
                cout << lesContraintes->nomTaches[i] << "\t";

                int j = 0;
                bool contrainte = false; // Si le sommet n'a pas de contrainte, on ajoute un 0

                // On parcours toute la ligneverifCircuit jusqu'au caractère '.' pour remplir notre tableau de contraintes
                while (contrainteCourante != '.') {
                    // Ajout de la contrainte au tableau de contraintes
                    ajouterContrainte(lesContraintes, tacheCourante, contrainteCourante);
                    // Vérification de la présence d'un circuit
                    if (verifCircuit((char)('A' + i), (char)('A' + i), lesContraintes->contraintes[i][j], lesContraintes) == true) { // S'il y a un circuit
                        lesContraintes->contraintes[i].pop_back(); // On supprime la contrainte ajoutée précédemment
                        cout << "Creation de circuit : contrainte " << contrainteCourante << " supprime.";
                        contrainte = false;
                        j--;
                    } else {
                        cout << " | " << lesContraintes->contraintes[i][j]; // Affichage de la contrainte
                        contrainte = true;
                    }

                    j++;
                    F >> contrainteCourante; // Prochaine contrainte
                }

                if (contrainte == false) {
                    lesContraintes->contraintes[i].push_back(0);
                }
                cout << endl;
            }
            cout << endl;
            // Traces d'exécution
            for (int i = 0; i < lesContraintes->nbTaches; i++) {
                for (int j = 0; j < lesContraintes->contraintes[i].size(); j++) {
                    if (lesContraintes->contraintes[i][j] != 0) {
                        cout << lesContraintes->nomTaches[i] << " ne peut commencer que lorsque la tache " << lesContraintes->contraintes[i][j] << " est terminee." << endl;
                    }
                }
            }
            F.close();// Fermeture du fichier
            getchar();

            cout << endl << "---------------------------------------------------" << endl;
            cout << "\t\tCreation du graphe" << endl;
            cout << "---------------------------------------------------" << endl << endl;

            cout << "Proprietes du graphe :" << endl;
            // Déclaration et initialisation de nos structures de données
            tt_graphe* leGraphe = new tt_graphe;
            leGraphe->nbSommets = lesContraintes->nbTaches + 2; // En comptant alpha et omega (a et z)
            cout << "\t- nombre de sommets : " << leGraphe->nbSommets << endl;
            int debut = 0;
            int fin = lesContraintes->nbTaches + 1;
            cout << endl << "\t--> Ajout des sommets \"debut de projet\" (a) et \"fin de projet\" (z)" << endl;
            cout << "\t--> Creation de la matrice d'adjacence : adj[x][y] = vrai (s'il existe un arc de x vers y) ou faux." << endl;
            cout << "\t--> Creation de la matrice des valeurs : val[x][y] = valeur de l'arc de x vers y s'il existe." << endl;

            // Initialisation des matrices adjacentes et des valeurs
            leGraphe->adj = new bool*[leGraphe->nbSommets];
            leGraphe->val = new int*[leGraphe->nbSommets];
            for (int ligne = 0; ligne < leGraphe->nbSommets; ligne++) {
                leGraphe->adj[ligne] = new bool[leGraphe->nbSommets];
                leGraphe->val[ligne] = new int[leGraphe->nbSommets];
                // Initialisation
                for (int colonne = 0; colonne < leGraphe->nbSommets; colonne++) {
                    leGraphe->adj[ligne][colonne] = false;
                    leGraphe->val[ligne][colonne] = -1; // Pour l'affichage : -1 non affiché / 0 affiché (distinction arc de valeur 0 et arc inexistant)
                }
            }

            // Détection du début de projet (tâche qui n'a pas de contrainte)
            for (int i = 0; i < lesContraintes->nbTaches; i++) {
                // Si la tâche n'a pas de contrainte (pas de prédécesseur), le sommet est le début de projet
                if (lesContraintes->contraintes[i][0] == 0) {
                    leGraphe->adj[debut][i + 1] = true; // i + 1 : 0 correspond à 'a' (début de projet) et 1 à 'A' (or 'A' est à l'indice 0 dans notre tableau de contraintes)
                    leGraphe->val[debut][i + 1] = 0;
                }
            }

            // Détection de fin de projet (tâche qui n'a pas de successeur : tâche non présente dans les contraintes)
            for (int i = 0; i < lesContraintes->nbTaches; i++) { // On parcours toutes les tâches
                bool ajoutFin = true;
                // On parcours toutes nos contraintes
                for (int j = 0; j < lesContraintes->nbTaches; j++) {
                    for (int k = 0; k < lesContraintes->contraintes[j].size(); k++) {
                        // Si on trouve la tâche dans les contraintes, ce n'est pas la fin du projet
                        if (lesContraintes->nomTaches[i] == lesContraintes->contraintes[j][k]) {
                            ajoutFin = false;
                        }
                    }
                }
                // Si ajoutFin vaut toujours 'true', la tâche n'a pas été trouvée dans les contraintes, c'est une fin de projet
                if (ajoutFin == true) {
                    leGraphe->adj[i + 1][fin] = true;
                    leGraphe->val[i + 1][fin] = lesContraintes->durees[i];
                }
            }

            // Détection des arcs
            cout << "\t--> Creation des arcs associes aux contraintes de type \"x ne peut commencer que lorsque y est terminee\"" << endl;
            // On parcours toutes les tâches
            for (int i = 0; i < lesContraintes->nbTaches; i++) {
                // On parcours toutes nos contraintes (de chaque tâche)
                for (int j = 0; j < lesContraintes->nbTaches; j++) {
                    for (int k = 0; k < lesContraintes->contraintes[j].size(); k++) {
                        // Si je trouve une contrainte, je l'ajoute aux deux matrices
                        if (lesContraintes->nomTaches[i] == lesContraintes->contraintes[j][k]) {
                            // j + 1 car les les indices de nomTaches ne correspondend pas avec les indices de adj (car alpha est la première colonne)
                            leGraphe->adj[i + 1][j + 1] = true;
                            leGraphe->val[i + 1][j + 1] = lesContraintes->durees[i];
                            cout << "\t\t" << lesContraintes->contraintes[j][k] << " ----[" << leGraphe->val[i + 1][j + 1] << "]----> " << lesContraintes->nomTaches[j] << endl;
                        }
                    }
                }
            }
            // Affichage des traces d'exécution
            cout << "\t--> Recherche des points d'entree et des points de sortie - Ajout des arcs incidents au debut et a la fin de projet" << endl;
            for (int i = 0; i < leGraphe->nbSommets; i++) {
                if (leGraphe->val[0][i] == 0) {
                    cout << "\t\ta ----[" << leGraphe->val[0][i] << "]----> " << (char)('A' + i - 1) << endl;
                }
            }
            for (int i = 0; i < leGraphe->nbSommets; i++) {
                if (leGraphe->val[i][fin] != -1) {
                    cout << "\t\t" << (char)('A' + i - 1) << " ----[" << leGraphe->val[i][fin] << "]----> z" << endl;
                }
            }
            getchar();

            // Affichage de la matrice adjacente
            cout << "---------------------------------------------------" << endl;
            cout << "\t\tGraphe d\'ordonnancement" << endl;
            cout << "---------------------------------------------------" << endl << endl;
            cout << lesContraintes->nbTaches << " taches" << endl;
            cout << leGraphe->nbSommets << " sommets" << endl;
            affichageAdj(leGraphe, lesContraintes);
            affichageVal(leGraphe, lesContraintes);
            getchar();

            // Calcul du rang
            cout << "---------------------------------------------------" << endl;
            cout << "\t\tCalcul du rang" << endl;
            cout << "---------------------------------------------------";
            int rang[leGraphe->nbSommets];
            calculRang(rang, leGraphe, lesContraintes);

            // Calendriers
            cout << "---------------------------------------------------" << endl;
            cout << "\t\tCalendriers" << endl;
            cout << "---------------------------------------------------" << endl << endl;
            // Au plus tôt
            int auPlusTot[leGraphe->nbSommets]; // Déclaration du tableau auPlusTot
            calendrierAuPlusTot(auPlusTot, rang, leGraphe, lesContraintes);
            // Au plus tard
            int auPlusTard[leGraphe->nbSommets]; // Déclaration du tableau auPlusTard
            calendrierAuPlusTard(auPlusTard, auPlusTot, rang, leGraphe, lesContraintes);

            // Marges
            cout << "---------------------------------------------------" << endl;
            cout << "\t\tMarges" << endl;
            cout << "---------------------------------------------------" << endl << endl;
            // Marge totale
            int margeTotale[leGraphe->nbSommets]; // Déclaration du tableau margeTotale
            calculMargeTotale(margeTotale, auPlusTard, auPlusTot, leGraphe->nbSommets);
            cout << "Marge totale :" << endl << endl;
            affichageTab(margeTotale, leGraphe->nbSommets, lesContraintes);
            getchar();
            // Marge libre
            int margeLibre[leGraphe->nbSommets]; // Déclaration du tableau margeLibre
            cout << "Marge libre :" << endl << endl;
            calculMargeLibre(margeLibre, auPlusTot, leGraphe);
            cout << "Marge libre :" << endl << endl;
            affichageTab(margeLibre, leGraphe->nbSommets, lesContraintes);

            // Diagrammes de Gantt
            cout << "---------------------------------------------------" << endl;
            cout << "\tDiagrammes de Gantt" << endl;
            cout << "---------------------------------------------------" << endl << endl;
            cout << "Diagramme au plus tot :" << endl << endl;
            gantt(auPlusTot, lesContraintes);
            cout << "Diagramme au plus tard :" << endl << endl;
            gantt(auPlusTard, lesContraintes);
        }


        // Recommencer ou non le programme
        cout << "Voulez-vous recommencer le traitement avec un autre fichier ? O/N : " ;
        cin >> reponse;
        while (reponse != 'O' && reponse != 'N') {
            cout << "Erreur : veuillez repondre par O (oui) ou par N (non) : ";
            cin.ignore(1, '\n');
            cin >> reponse;
        }
    } while(reponse == 'O');

    return 0;
}
