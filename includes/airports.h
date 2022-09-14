#include "airlines.h"

// fonctions pour remplissage des SD
void init_hash_airp(Airport **);
void collisions_airp(Airport ***, int, char *);
void read_airport_from_line(Airport **, char *);
void ajout_alph_airp(Airport **, Airport *);
void creation_hash_airports(FILE *, Airport **);
airl_infos *search_airl_infos(airl_infos *, char *);

// fonctions pour requetes
void init_tab_airl_inf(airl_infos **);
void search_min_tab_airl_inf(airl_infos **, int *);
void recup_most_del_airl_inf(airl_infos *, airl_infos **);

