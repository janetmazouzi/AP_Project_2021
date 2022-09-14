#include "fonctions.h"

// fonctions pour remplir les SD
void init_airline(Airline_node **);
void init_tab_airl(Airline_node **);
void read_airline_from_line(Airline_node **, char *);
void add_airline_abr(Airline_node **, Airline_node *);
void fill_airline_tree(FILE *, Airline_node **);

Airline_node *search_airline_ABR(Airline_node *, char *);
void search_min_tab(Airline_node **, int *);
void recup_most_del_airl(Airline_node **, Airline_node *);

// Fonctions pour requetes
void print_airl_if_dep(Airline_node *, airl_infos *);
