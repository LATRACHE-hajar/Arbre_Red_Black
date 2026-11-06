#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>

using namespace std;

// Les couleurs possibles
enum Color {
    ROUGE,
    NOIR
};

// Structure d'un noeud
struct Node {
    int data;
    Color color;

    Node* left;
    Node* right;
    Node* parent;

    Node(int value) {
        data = value;
        color = ROUGE;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;
    Node* NIL;

    // Rotation gauche
    void rotationGauche(Node* x) {
        Node* y = x->right;

        x->right = y->left;

        if (y->left != NIL)
            y->left->parent = x;

        y->parent = x->parent;

        if (x->parent == NIL)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    // Rotation droite
    void rotationDroite(Node* y) {
        Node* x = y->left;

        y->left = x->right;

        if (x->right != NIL)
            x->right->parent = y;

        x->parent = y->parent;

        if (y->parent == NIL)
            root = x;
        else if (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;

        x->right = y;
        y->parent = x;
    }

    // Recherche interne : retourne le noeud
    Node* trouver(int value) {
        Node* x = root;

        while (x != NIL && x->data != value) {
            if (value < x->data)
                x = x->left;
            else
                x = x->right;
        }

        return x;
    }

    // Correction apres insertion
    void corrigerInsertion(Node* z) {

        while (z->parent->color == ROUGE) {

            if (z->parent == z->parent->parent->left) {

                Node* oncle = z->parent->parent->right;

                // Cas 1 : parent et oncle rouges
                if (oncle->color == ROUGE) {
                    z->parent->color = NOIR;
                    oncle->color = NOIR;
                    z->parent->parent->color = ROUGE;

                    z = z->parent->parent;
                }
                else {
                    // Cas 2 : gauche-droite
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotationGauche(z);
                    }

                    // Cas 3 : gauche-gauche
                    z->parent->color = NOIR;
                    z->parent->parent->color = ROUGE;

                    rotationDroite(z->parent->parent);
                }
            }
            else {

                Node* oncle = z->parent->parent->left;

                // Cas 1 : parent et oncle rouges
                if (oncle->color == ROUGE) {
                    z->parent->color = NOIR;
                    oncle->color = NOIR;
                    z->parent->parent->color = ROUGE;

                    z = z->parent->parent;
                }
                else {
                    // Cas 2 : droite-gauche
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotationDroite(z);
                    }

                    // Cas 3 : droite-droite
                    z->parent->color = NOIR;
                    z->parent->parent->color = ROUGE;

                    rotationGauche(z->parent->parent);
                }
            }
        }

        root->color = NOIR;
    }

    // Retourne le minimum d'un sous-arbre
    Node* minimum(Node* node) {
        while (node->left != NIL)
            node = node->left;

        return node;
    }

    // Remplacement d'un sous-arbre par un autre
    void transplanter(Node* u, Node* v) {

        if (u->parent == NIL)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }

    // Correction apres suppression
    void corrigerSuppression(Node* x) {

        while (x != root && x->color == NOIR) {

            if (x == x->parent->left) {

                Node* frere = x->parent->right;

                // Cas 1 : frere rouge
                if (frere->color == ROUGE) {
                    frere->color = NOIR;
                    x->parent->color = ROUGE;

                    rotationGauche(x->parent);

                    frere = x->parent->right;
                }

                // Cas 2 : les deux fils du frere sont noirs
                if (frere->left->color == NOIR &&
                    frere->right->color == NOIR) {

                    if (frere != NIL)
                        frere->color = ROUGE;

                    x = x->parent;
                }
                else {

                    // Cas 3 : fils droit noir
                    if (frere->right->color == NOIR) {

                        frere->left->color = NOIR;
                        frere->color = ROUGE;

                        rotationDroite(frere);

                        frere = x->parent->right;
                    }

                    // Cas 4 : fils droit rouge
                    frere->color = x->parent->color;
                    x->parent->color = NOIR;
                    frere->right->color = NOIR;

                    rotationGauche(x->parent);

                    x = root;
                }
            }
            else {

                Node* frere = x->parent->left;

                // Cas 1 : frere rouge
                if (frere->color == ROUGE) {
                    frere->color = NOIR;
                    x->parent->color = ROUGE;

                    rotationDroite(x->parent);

                    frere = x->parent->left;
                }

                // Cas 2 : les deux fils du frere sont noirs
                if (frere->right->color == NOIR &&
                    frere->left->color == NOIR) {

                    if (frere != NIL)
                        frere->color = ROUGE;

                    x = x->parent;
                }
                else {

                    // Cas 3 : fils gauche noir
                    if (frere->left->color == NOIR) {

                        frere->right->color = NOIR;
                        frere->color = ROUGE;

                        rotationGauche(frere);

                        frere = x->parent->left;
                    }

                    // Cas 4 : fils gauche rouge
                    frere->color = x->parent->color;
                    x->parent->color = NOIR;
                    frere->left->color = NOIR;

                    rotationDroite(x->parent);

                    x = root;
                }
            }
        }

        x->color = NOIR;
    }

    // Liberer la memoire
    void liberer(Node* node) {
        if (node != NIL) {
            liberer(node->left);
            liberer(node->right);
            delete node;
        }
    }

public:

    // Constructeur
    RedBlackTree() {

        NIL = new Node(0);
        NIL->color = NOIR;

        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;

        root = NIL;
    }

    // INSERTION
    void inserer(int value) {

        Node* z = new Node(value);

        z->left = NIL;
        z->right = NIL;

        Node* y = NIL;
        Node* x = root;

        while (x != NIL) {

            y = x;

            if (z->data < x->data)
                x = x->left;
            else if (z->data > x->data)
                x = x->right;
            else {
                delete z;
                return;
            }
        }

        z->parent = y;

        if (y == NIL)
            root = z;
        else if (z->data < y->data)
            y->left = z;
        else
            y->right = z;

        z->color = ROUGE;

        corrigerInsertion(z);
    }

    // RECHERCHE
    bool rechercher(int value) {
        return trouver(value) != NIL;
    }

    // SUPPRESSION
    bool supprimer(int value) {

        Node* z = trouver(value);

        // Valeur inexistante
        if (z == NIL)
            return false;

        Node* y = z;
        Node* x;

        Color couleurOriginale = y->color;

        // Cas 1 : pas de fils gauche
        if (z->left == NIL) {

            x = z->right;
            transplanter(z, z->right);
        }

        // Cas 2 : pas de fils droit
        else if (z->right == NIL) {

            x = z->left;
            transplanter(z, z->left);
        }

        // Cas 3 : deux fils
        else {

            y = minimum(z->right);

            couleurOriginale = y->color;
            x = y->right;

            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplanter(y, y->right);

                y->right = z->right;
                y->right->parent = y;
            }

            transplanter(z, y);

            y->left = z->left;
            y->left->parent = y;

            y->color = z->color;
        }

        delete z;

        // Corriger si le noeud supprime etait noir
        if (couleurOriginale == NOIR)
            corrigerSuppression(x);

        return true;
    }

    // Destructeur
    ~RedBlackTree() {
        liberer(root);
        delete NIL;
    }
};


int main() {

    // Modifier N pour chaque experience
    int N = 100;

    // Generateur aleatoire
    mt19937 generateur(42);

    // Creer N valeurs uniques
    vector<int> valeurs(N);

    for (int i = 0; i < N; i++)
        valeurs[i] = i;

    shuffle(valeurs.begin(), valeurs.end(), generateur);

    cout << fixed << setprecision(4);

    RedBlackTree arbreInsertion;

    auto debutInsertion = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++)
        arbreInsertion.inserer(valeurs[i]);

    auto finInsertion = chrono::high_resolution_clock::now();

    chrono::duration<double, micro> tempsInsertion =
        finInsertion - debutInsertion;

    // Construire l'arbre avant le chronometrage
    RedBlackTree arbreRecherche;

    for (int i = 0; i < N; i++)
        arbreRecherche.inserer(valeurs[i]);

    int nombreTrouve = 0;

    auto debutRecherche = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        if (arbreRecherche.rechercher(valeurs[i]))
            nombreTrouve++;
    }

    auto finRecherche = chrono::high_resolution_clock::now();

    chrono::duration<double, micro> tempsRecherche =
        finRecherche - debutRecherche;

    // Construire l'arbre avant le chronometrage
    RedBlackTree arbreSuppression;

    for (int i = 0; i < N; i++)
        arbreSuppression.inserer(valeurs[i]);

    int nombreSupprime = 0;

    auto debutSuppression = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        if (arbreSuppression.supprimer(valeurs[i]))
            nombreSupprime++;
    }

    auto finSuppression = chrono::high_resolution_clock::now();

    chrono::duration<double, micro> tempsSuppression =
        finSuppression - debutSuppression;

    // Calcul du temps moyen
    double moyenneRecherche = tempsRecherche.count() / N;
    double moyenneSuppression = tempsSuppression.count() / N;

    cout << "\n========== RESULTATS RED-BLACK ==========\n";

    cout << "Nombre de valeurs : " << N << endl;

    cout << "\nInsertion de " << N << " elements : "
         << tempsInsertion.count() << " microsecondes" << endl;

    cout << "Recherche de " << N << " elements : "
         << tempsRecherche.count() << " microsecondes" << endl;

    cout << "Suppression de " << N << " elements : "
         << tempsSuppression.count() << " microsecondes" << endl;

    cout << "\nTemps moyen d'une recherche : "
     << moyenneRecherche << " microsecondes" << endl;

    cout << "Temps moyen d'une suppression : "
     << moyenneSuppression << " microsecondes" << endl;

    cout << "\n=========================================\n";

    return 0;
}
