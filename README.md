# FR
# Projet d'Une application pour analyser des vols aériens internes aux Etats-Unis en 2014.(projet tutoré PA)

Ce dépôt `GitLab` contient un  projet codé en`C` pour lequel la compilation, les tests... ont été automatisés.

Le code de ce projet se fonde sur les structures de données et les algorithmes vus en programmation avancée au Semestre 6. Il sert à analyser des vols aériens aux Etats-Unis et affiche différentes commandes demander par l'utilisateur. 

## Contenu du dépôt

- `README.md` le fichier que vous être en train de lire
- `Makefile` permet la compilation du code (cible `main`)
- `.clang-format` contient les règles de formatage de code
- le répertoire `includes` contient les déclarations de fonctions
- le répertoire `src` contient les sources du projet
- le répertoire `data` contient  les fichiers `cvs` à analyser

# Les requêtes accessibles à l'utilisateur

- `show-airports <airline_id>`  : affiche tous les aéroports depuis lesquels la compagnie aérienne `<airline_id>` opère des vols
- `show-airlines <port_id>`: affiche les compagnies aériens qui ont des vols qui partent de l'aéroport `<port_id>`
- `show-flights <port_id> <date> [<time>] [limit=<xx>]` : affiche les vols qui partent de l'aéroport à la date, avec optionnellement une heure de début, et limité à xx vols
- `most-delayed-flights`     : donne les 5 vols qui ont subis les plus longs retards à l'arrivée
- `most-delayed-airlines`    : donne les 5 compagnies aériennes qui ont, en moyenne, le plus de retards
- `delayed-airline <airline_id>`    : donne le retard moyen de la compagnie aérienne passée en paramètre
- `most-delayed-airlines-at-airport <airport_id>`    : donne les 3 compagnies aériennes avec le plus de retard d'arrivé à l'aéroport passée en paramètre
- `changed-flights <date>` : les vols annulés ou déviés à la date <date> (format M-D)
- `avg-flight-duration <port_id> <port_id>`: calcule le temps de vol moyen entre deux aéroports
- `find-itinerary <port_id> <port_id> <date> [<time>] [limit=<xx>]`: trouve un ou plusieurs itinéraires entre deux aéroports à une date donnée (l'heure est optionnel, requête peut être limité à `xx` propositions, il peut y avoir des escales)
- `find-multicity-itinerary <port_id_depart> <port_id_dest1> <date> [<time>] <port_id_dest2> <date> [<time>] ... <port_id_destN> <date> [<time>]`: trouve un itinéraire multiville qui permet de visiter plusieurs villes (il peut y avoir des escales pour chaque vol intermediaire)
- `quit`       : quitter l'application

>_VEUILLEZ RESPECTER L'ORDRE DES ARGUMENTS AINSI QUE LA SYNTAXE:_
- un temps : HHMM  exemple _`1230`_
- une date : M-J   exemple _`12-4`_
- un code IATA : AAA pour un aeroport, AA pour une compagnie aérienne
- une limite de donnée à afficher : limit=x exemple _`limit=3`_


# ENG
# Porgramming Project : commercial flights data management Application
>_Tutored Project, Advanced Programming Class, Polytech Lille_

This `GitLab` Repository contains a project coded in `C` in which the compilation, the tests have been automated.

This project's coding uses the data structures and algorithm theories seen in class during the sixth semester. 

It analises local flights data in the USA in 2014 and allows the user to make requests on this data.

## Repository's Content

- `README.md` The one you are currently reading.
- `Makefile` compile all the files.
- `.clang-format` the coding format rules
- the `includes` directory contains the functions' declarations.
- the`src` directory contains the source code files.
- the `data` directory contains the CVS files

## The requests
Here are the few requests the user can ask the application

- `show-airports <airline_id>`  : displays all the airports in which the airline `<airline_id>` has flights departing and arriving.
- `show-airlines <port_id>`: displays the airlines which have flights departing from the airport `<port_id>`.
- `show-flights <port_id> <date> [<time>] [limit=<xx>]` : display the flights that departs from the airport `<port_id>` at the date `<date>`, with optionally a departure time `<time>` and a limit of `<xx>` flights.
- `most-delayed-flights`     : displays the five flights with the biggest delays.
- `most-delayed-airlines`    : displays the five airlines with the biggest average delays.
- `delayed-airline <airline_id>`    : displays the average delay of the airline `<airline_id>`.
- `most-delayed-airlines-at-airport <airport_id>`    : displays the three airlines with the biggest delays at the airport `<airport_id>`.
- `changed-flights <date>` : displays the cancelled flights at the date <date> (M-D format)
- `avg-flight-duration <port_id1> <port_id2>`: Calculate the average time it takes to go from `<port_id1>` and `<port_id2>`
- `find-itinerary <port_id1> <port_id2> <date> [<time>] [limit=<xx>]`: find an intinerary from <port_id1> to <port_id2>` the date `<date>`, with optionnal `<time>` and intinerary limit `<xx>` (Stops are possible)
trouve un ou plusieurs itinéraires entre deux aéroports à une date donnée (l'heure est optionnel, requête peut être limité à `xx` propositions, il peut y avoir des escales)
- `find-multicity-itinerary <port_id_depart> <port_id_dest1> <date> [<time>] <port_id_dest2> <date> [<time>] ... <port_id_destN> <date> [<time>]`: find a multicity intinerary from <port_id_depart> to <port_id_dest1>` to ... to <port_id_destN> the date `<date>`, with optionnal `<time>` (Stops are possible for each stop)
- `quit`       : quit

>_PLEASE RESPECT THE ARGUMENTS' ORDER:_
- a time : HHMM  example _`1230`_
- a date : M-J   example _`12-4`_
- a IATA code : AAA for an airport, AA for an Airline
- a limit to display : limit=x example _`limit=3`_
