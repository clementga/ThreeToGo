#include <MLV/MLV_all.h>
#include <time.h>
#include <sys/time.h>
#include "threetogo.h"
#include "graphique.h"
#include "moteur.h"
#include "token.h"

int game_init(Game *game) {
	/* Allocations des listes */
	game->queue = malloc(sizeof(Liste));
	if (game->queue == NULL)
		return 0;
	
	game->lst_tokens = malloc(sizeof(Liste));
	if (game->lst_tokens == NULL) {
		free(game->queue);
		return 0;
	}

	/* Initialisation du jeu */
	*(game->queue) = NULL;
	if (init_queue(game->queue) != 1) {
		return 0;
	}
	*(game->lst_tokens) = NULL;
	game->nb_tokens = 0;
	game->score = 0;
	game->combo = 0;
	game->timer = 0;

	return 1;
}

void game_free(Game *game) {
	free_liste(game->queue);
	free_liste(game->lst_tokens);
	free(game->queue);
	free(game->lst_tokens);
	game->queue = NULL;
	game->lst_tokens = NULL;
}

int game_loop(Game *game, MLV_Image *images[], MLV_Font *police, MLV_Sound *sounds[]) {
	int check = 0;
	int point_gain = 0;
	int move_done = 0;
	
	int mousex, mousey; /* Stocke des coordonnées de clic */
	Case cible = {0, 0}, ciblebis; /* Stocke des coordonées de case */

	Token *token_clique; /* Stocke l'adresse d'un token qui a été cliqué */
	int attend_clique = 0; /* Booléen indiquant un second clic est attendu après avoir cliqué sur un token */
	
	/* Stocke un temps de manière précise pour l'horloge et l'initialise */
	struct timeval debut;
	gettimeofday(&debut, NULL);

	MLV_Event event;
	MLV_Button_state button;

	/* Boucle principale */
	while (time_usec(debut) < DUREE_MAX) {

		/* Traitement de la file d'évènement */
		do {
			/* Récuparation du prochain évènement sur la liste */
			event = MLV_get_event(NULL, NULL, NULL, NULL, NULL, &mousex, &mousey, NULL, &button);

			/* Clic de bouton */
			if (event == MLV_MOUSE_BUTTON && button == MLV_PRESSED) {
				if (attend_clique == 0) {
					cible = mouse_to_square(mousex, mousey);
			
					/* Si le clic est sur un bouton d'ajout et qu'il reste de la place */
					if(game->nb_tokens < MAX_TOKENS)
						switch(button_add_check(cible)) {
							case 1 : 
								if (add_left(game->queue, game->lst_tokens) != 1) {
									return 0;
								}
								move_done = 1;
								break;
							case 2 : 
								if (add_right(game->queue, game->lst_tokens) != 1) {
									return 0;
								}
								move_done = 1;
								break;
						}
					
					/* Si le clic est sur un jeton de la liste */
					if ((check = token_select_check(game->nb_tokens, cible, *(game->lst_tokens))) != 0) {
						/* On cherche et stocke l'adresse du token cliqué */
						token_clique = *(game->lst_tokens);
						if (check != game->nb_tokens) {
							int i;
							for(i = 0 ; i < check ; i++) {
								token_clique = token_clique->next;
							}
						}
						/* On attend un second clic */
						attend_clique = 1;
					}
				}
				else if (attend_clique == 1) {
					ciblebis = mouse_to_square(mousex, mousey);
					
					if (ciblebis.col == cible.col) {
						/* Choix du haut : on décale les formes sur la gauche */
						if (ciblebis.lig == cible.lig - 1) {
							shift_commoncolor_left(game->lst_tokens, token_clique);
							move_done = 1;
						}
						/* Choix du bas : on décale les couleurs sur la gauche */
						else if (ciblebis.lig == cible.lig + 1) {
							shift_commonshape_left(game->lst_tokens, token_clique);
							move_done = 1;
						}
					}
					cible = mouse_to_square(0, 0);
					attend_clique = 0;
				}

				/* Vérification des combinaisons */
				if (move_done == 1) {
					point_gain = check_combinations(game->lst_tokens, game->combo + 1);

					if (point_gain == 0) {
						game->combo = 0;
					}
					else {
						game->combo += 1;
						switch (game->combo) {
							case 2:
								MLV_play_sound(sounds[0], 1.0);
								break;
							case 3:
								MLV_play_sound(sounds[1], 1.0);
								break;
							case 4:
								MLV_play_sound(sounds[2], 1.0);
								break;
							case 5:
								MLV_play_sound(sounds[3], 1.0);
								break;
							case 6:
								MLV_play_sound(sounds[4], 1.0);
								break;
							case 7:
								MLV_play_sound(sounds[5], 1.0);
								break;
						}
					}

					if (game->score == 0 && point_gain > 0) {
						MLV_play_sound(sounds[6], 1.0);
					}
					else if (game->combo < 2 && point_gain >= 1000) {
						MLV_play_sound(sounds[7], 1.0);
					}

					game->score += point_gain;
					game->nb_tokens = length(*(game->lst_tokens));
					move_done = 0;
				}
				

			}
		} while (event != MLV_NONE);

		/* Rafraichissement de l'écran */
		refresh_screen(*game, cible, images, police); 
		MLV_actualise_window();
		MLV_delay_according_to_frame_rate();

		/* Met à jour le timer de la partie */
		game->timer = time_usec(debut); 
	}
    return 1;
}
