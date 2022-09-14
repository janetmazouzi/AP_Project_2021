#include "../includes/commandes.h"

// debug
void print_hash_airp(Airport *H[]) {
  for (int i = 0; i < SIZE_HASH_AIRPORT; i++) {
    if (H[i] != NULL) {
      print_airport(H[i]);
    }
  }
}

void free_airl_tree2(Airline_node *A) {
  if (A == NULL)
    return;
  free_airl_tree2(A->left);
  free_airl_tree2(A->right);
  free(A);
}

int main() {
  // Ouverture des fichiers
  FILE *f_flights, *f_airports, *f_airlines;
  f_flights = fopen("data/flights.csv", "r");
  f_airports = fopen("data/airports.csv", "r");
  f_airlines = fopen("data/airlines.csv", "r");

  if ((f_airports == NULL) || (f_flights == NULL) ||
      (f_airlines == NULL)) {
    printf("Erreur lors de l'ouverture des fichiers de "
           "données.\nFermeture de l'application.\n");
    return 1;
  }
  // Création et initialisation des Structures de données
  Airport *Hash_airports[SIZE_HASH_AIRPORT];
  init_hash_airp(Hash_airports);

  hash_cell_fl *Hash_flights[SIZE_HASH_FLIGHTS];
  init_hash_fl(Hash_flights);
  Airline_node *Airline_tree;
  Airline_tree = NULL;
  Flight *most_del_fl[SIZE_TAB_DELAY];
  init_tab_fl(most_del_fl, SIZE_TAB_DELAY);
  Airline_node *most_del_airl[SIZE_TAB_DELAY];
  init_tab_airl(most_del_airl);

  // Remplissage des SD
  creation_hash_airports(f_airports, Hash_airports);
  fill_airline_tree(f_airlines, &Airline_tree);
  recup_flights(f_flights, Hash_flights, &Airline_tree, Hash_airports,
                most_del_fl);

  recup_most_del_airl(most_del_airl, Airline_tree);

  // Fermeture des fichiers
  fclose(f_airports);
  fclose(f_airlines);
  fclose(f_flights);

  // Récup des requetes
  char line[100];
  char *token;
  char *token2;
  char temp[50];
  char commande[50];
  char arg[100];
  int q = 0;

  printf("¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤\nCher.e.s client.e.s, "
         "Bienvenue.\nVeuillez rentrer les "
         "commandes dont vous avez besoin.\nPour quitter, rentrez "
         "quit.\n¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤\n\n");

  while ((!q) && (fgets(line, 100, stdin) != NULL)) {
    printf(">%s", line);

    token = strtok(line, " ");
    strcpy(temp, token);
    token = strtok(NULL, "\n");
    if (token != NULL) {
      strcpy(commande, temp);
      strcpy(arg, token);
    } else {
      token2 = strtok(temp, "\n");
      strcpy(commande, token2);
    }

    if (!strcmp(commande, "show-airports")) {
      if ((arg != NULL) && (strlen(arg) == AIRL_LEN))
        show_airports(arg, Hash_airports);
      else
        print_err_lect();

    } else if (!strcmp(commande, "show-airlines")) {
      printf("%s,%ld\n", arg, strlen(arg));
      if ((arg != NULL) && (strlen(arg) == AIRP_LEN))
        show_airlines(arg, Airline_tree, Hash_airports);
      else
        print_err_lect();

    } else if (!strcmp(commande, "show-flights")) {
      if ((arg != NULL) &&
          (strlen(arg) >= DATE_LEN_MIN + AIRP_LEN + 1))
        show_flights(arg, Hash_flights);
      else
        print_err_lect();

    } else if (!strcmp(commande, "most-delayed-airlines"))
      most_delayed_airlines(most_del_airl);

    else if (!strcmp(commande, "most-delayed-flights"))
      most_delayed_flights(most_del_fl);

    else if (!strcmp(commande, "delayed-airline")) {
      if ((arg != NULL) && (strlen(arg) == AIRL_LEN))
        delayed_airlines(arg, Airline_tree);
      else
        print_err_lect();

    } else if (!strcmp(commande, "changed-flights")) {
      if ((arg != NULL) && (strlen(arg) >= DATE_LEN_MIN))
        changed_flights(arg, Hash_flights);
      else
        print_err_lect();

    } else if (!strcmp(commande,
                       "most-delayed-airlines-at-airport")) {
      if ((arg != NULL) && (strlen(arg) == AIRP_LEN))
        most_delayed_airlines_at_airport(arg, Hash_airports,
                                         Airline_tree);
      else
        print_err_lect();

    } else if (!strcmp(commande, "avg-flight-duration")) {
      if ((arg != NULL) && (strlen(arg) == 2 * AIRP_LEN + 1))
        avg_flight_duration(arg, Hash_flights);
      else
        print_err_lect();

    } else if (!strcmp(commande, "find-itinerary")) {
      if ((arg != NULL) &&
          (strlen(arg) >= 2 * AIRP_LEN + DATE_LEN_MIN + 2))
        find_itinerary(arg, Hash_flights);
      else
        print_err_lect();

    } else if (!strcmp(commande, "find-multicity-itinerary")) {
      if ((arg != NULL) &&
          (strlen(arg) >= 3 * AIRP_LEN + 2 * DATE_LEN_MIN + 4))
        find_multicity_itinerary(arg, Hash_flights);
      else
        print_err_lect();

    } else if (!strcmp(commande, "quit"))
      q = 1;

    else
      print_err_lect();
    printf("\n");
  }
  printf("Merci d'avoir choisi nos services. A bientôt\n");
  quit(Hash_flights, &Airline_tree, Hash_airports);
  // tests
  //  print_airline_tree(Airline_tree);
  // show_airlines("LAX",Airline_tree, Hash_airports);

  return 0;
}
