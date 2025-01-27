/**
 * @file
 * @brief Contient les fonctions du moteur graphique du jeu
 */
#ifndef GRAPHIQUE_H
#define GRAPHIQUE_H

#include <sys/time.h>
#include "MLV/MLV_image.h"
#include "MLV/MLV_text.h"

/** Définition d'une couleur transparente */
#define MLV_COLOR_CLEAR MLV_rgba(255,255,255,0)

/** Constante mathématique pi */
#define PI 3.14159265358979323846264338

/** Représente une case sur la fenêtre. Chaque fenêtre est divisée comme grille de cases */
typedef struct caseg{
    int col; /**< Colonne */
    int lig; /**< Ligne */
}Case;

/**
 * Charge toutes les images nécessaires au fonctionnement du jeu
 * @param images tableau ou stocker les adresse vers les images chargées
 * @return 1 si tout s'est bien passé, 0 sinon
 */
int images_init(MLV_Image *images[]);

/**
 * Libère toutes les images chargées par le jeu
 * @param images tableau d'images chargées
 * @param taille taille du tableau d'images
 */
void images_free(MLV_Image *images[], int taille);

/** 
 * Transforme le pixel d'un clic de souris en coordonnées de case 
 * @param mousex entier abscisse du pixel
 * @param mousey entier ordonnée du pixel
 * @return structure Case avec coordonnées
 */
Case mouse_to_square(int mousex, int mousey);

/** 
 * Ouvre une fenêtre vide à fond noir 
 * @param larg entier nombre de pixels de largeur
 * @param haut entier nombre de pixels de hauteur
 */
void window_open(int larg, int haut);

/** 
 * Dessine un token de forme et couleur donnés 
 * @param t structure Token à dessiner
 * @param c structure Case où dessiner
 * @param images liste des images chargées
 */
void token_draw(Token t, Case c, MLV_Image *images[]);

/** 
 * Dessine une liste de tokens sur une ligne
 * @param lst_tokens structure Liste de tokens à dessiner
 * @param nb_tokens entier nombre à dessiner
 * @param cible structure Case où dessiner
 * @param images liste des images chargées
 */
void token_draw_list(Liste lst_tokens, int nb_tokens, Case cible, MLV_Image *images[]);

/** 
 * Dessine un bouton avec une flèche dans la direction voulue
 * @param cible structure Case où dessiner
 * @param dir caractère
 * @param images liste des images chargées
 */
void button_add_draw(Case cible, char dir, MLV_Image *images[]) ;

/** 
 * Vérifie si un bouton d'ajout de jeton (gauche ou droit) a été cliqué
 * @param cible case cliquée originalement
 * @return 0 si non, 1 si gauche, 2 si droit
 */
int button_add_check(Case cible);

/** 
 * Vérifie si un token est cliqué 
 * @param nb_tokens entier du nombre de tokens alignés
 * @param cible structure Case où dessiner
 * @param lst_tokens structure Liste des tokens alignés
 * @return position entier du n-ième token sélectionné
 */
int token_select_check(int nb_tokens, Case cible, Liste lst_tokens);

/**
 * Si la position donnée est strictement positive, dessine la sélection
 * @param cible structure Case où dessiner
 * @param lst_tokens structure Liste des tokens alignés
 * @param nb_tokens entier du nombre de tokens alignés
 * @param position entier du n-ième token sélectionné
 * @param images liste des images chargées
 */
void token_select_draw(Case cible, Liste lst_tokens, int nb_tokens, int position, MLV_Image *images[]);

/** 
 * Transforme le score en une chaine de caractères
 * @param message chaîne de caractères
 * @param score entier valeur du score
 */
void score_to_str(char* message, int score);

/** 
 * Transforme le timer en une chaine de caractères
 * @param message chaîne de caractères
 * @param timer entier valeur du timer
 */
void timer_to_str(char* message, int timer);

/** 
 * Transforme le combo en une chaine de caractères
 * @param dest chaîne de caractères
 * @param combo combo actuel
 */
void combo_to_str(char *dest, int combo);

/** 
 * Renvoie le float de l'interval écoulé entre debut et maintenant 
 * @param debut structure timeval
 * @return flottant interval (en secondes) entre debut et l'heure actuel
 */
float time_usec(struct timeval debut);

/** 
 * Rafraichit l'affichage de base du jeu 
 * @param g structure Game des paramètres de la partie
 * @param cible structure Case de la case cliquée en dernier
 * @param images liste des images chargées
 * @param police police d'écriture
 */
void refresh_screen(Game g, Case cible, MLV_Image *images[], MLV_Font *police);

/** 
 * Dessine une l'aiguille de l'horloge
 * @param duree flottant représentant la durée écoulée (en secondes)
 */
void clock_draw(float duree);

#endif
