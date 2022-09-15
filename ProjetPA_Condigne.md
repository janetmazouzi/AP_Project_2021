# Tutorat de Programmation Avancée (SE3 - 2020/2021)

Ce dépôt `GIT` contient le sujet et les données utiles pour la réalisation du
_petit_ projet du module de Programmation Avancée.

> __Attention__: ce dépôt risque d'évoluer au cours de l'avancée des projets.
N'hésitez pas à le consulter régulièrement.

## Résumé

L'objectif de ce projet est de vérifier que vous maîtrisez les principes vus en
cours de programmation avancée : structures de données complexes,
lecture / écriture de fichiers, compilation séparée et automatique, utilisation
de gestionnaire de version...

Pour cela, le travail demandé est de réaliser une application qui permet d'analyser 58592 vols aux États Unis en 2014. Votre travail est de charger ces fichiers pour effectuer un certain nombre de requêtes (lister des vols selon plusieurs critères, lister l'aéroligne avec plus de retards, trouver un itinéraire, ...)

## Contexte

Les données sont stockées dans trois fichiers `CSV` (_comma-separated values_) qui est un format texte permettant de stocker des tableaux. Chaque ligne du fichier correspond à une ligne du tableau et les différents éléments d'une ligne sont
séparés par un élément particulier (en général une virgule `,` mais d'autres sont possibles `\t`, `;`...). La première ligne sert à décrire le nom des différents champs.
Par exemple, le fichier `flights.csv`, qui contient les vols, a la structure suivante :

~~~
MONTH,DAY,WEEKDAY,AIRLINE,ORG_AIR,DEST_AIR,SCHED_DEP,DEP_DELAY,AIR_TIME,DIST,SCHED_ARR,ARR_DELAY,DIVERTED,CANCELLED
1,1,4,WN,LAX,SLC,1625,58.0,94.0,590,1905,65.0,0,0
1,1,4,UA,DEN,IAD,823,7.0,154.0,1452,1333,-13.0,0,0
1,1,4,MQ,DFW,VPS,1305,36.0,85.0,641,1453,35.0,0,0
1,1,4,AA,DFW,DCA,1555,7.0,126.0,1192,1935,-7.0,0,0
1,1,4,WN,LAX,MCI,1720,48.0,166.0,1363,2225,39.0,0,0
~~~
Les premiers trois champs correspondent aux mois (`1` ou janvier), jour (`1`) et jour de la semaine (`4` ou mercredi), le quatrième à la compagnie aérienne suivant les codes IATA (`WN` ou Southwest Airlines Co.), `ORG_AIR` correspond à l'aéroport d'origine ou départ (`LAX`), `DEST_AIR` correspond à l'aéroport destination ou d'arrivée (`SLC`), `SCHED_DEP` correspond à l'heure de départ prévu (`1625`), `DEP_DELAY` correspond au délais de départ en minutes (`58.0`), `AIR_TIME` correspond à la durée du vol en minutes (`94.0`), `DIST` correspond à la distance en miles (`590`), `SCHED_ARR` correspond à l'heure d'arrivée prévu (`1905`), `ARR_DELAY` correspond au retard à l'arrivée en minutes (`65.0`), `DIVERTED` est un booléen qui indique si le vol a été déviée (`0`), et `CANCELLED` est un booléen qui indique si le vol a été annulé (`0`).



De la même façon, le fichier `airports.csv` décrit les aéroports et villes correspondent aux code IATA de la façon suivante :
```
IATA_CODE,AIRPORT,CITY,STATE,COUNTRY,LATITUDE,LONGITUDE
ABE,Lehigh Valley International Airport,Allentown,PA,USA,40.652359999999994,-75.4404
ABI,Abilene Regional Airport,Abilene,TX,USA,32.41132,-99.6819
ABQ,Albuquerque International Sunport,Albuquerque,NM,USA,35.04022,-106.60918999999998
```

ainsi que le fichier `airlines.csv` qui contient les codes IATA pour chaque compagnie aérienne :
```
IATA_CODE,AIRLINE
UA,United Air Lines Inc.
AA,American Airlines Inc.
US,US Airways Inc.
```


## Cahier des charges

Il vous est demandé de réaliser un programme qui charge les fichiers de données `CSV` (ils sont disponibles dans le répertoire `data`) et qui permet de les interroger facilement.

Le volume de données est relativement important, par conséquent un soin particulier devra être apporté aux structures de données utilisées et à la rapidité d'exécution de votre programme. Les choix algorithmiques devront obligatoirement être expliqués et justifiés dans votre rapport.

Pour permettre d'évaluer automatiquement la performance de votre programme et permettre une utilisation automatique par redirection, votre programme devra fonctionner de la manière suivante :

1. charger le fichier de données
2. attendre une commande
3. traiter la commande
4. afficher le résultat de cette commande
5. revenir à l'étape 2

Les commandes seront les suivantes:


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
- `quit`       : quit


Pour information, les paramètres entre crochets `[ ]` sont optionnels et les paramètres entre `< >` indiquent une valeur à renseigner.
Les dates sont au format `M-J` et l'heure `HHMM`
Ainsi si votre exécutable s'appelle `projet_pa` il doit être possible de l'utiliser de la manière suivante:

~~~
$ ./projet_pa < data/requetes.txt
~~~

avec le fichier `requetes.txt` qui contient par exemple:

~~~
show-airports HA
show-airlines LAX
show-flights ATL 2-26
show-flights SLC 4-17 1600 limit=5
avg-flight-duration LAX JFK
find-itinerary PHX SAN 12-15
most-delayed-flights
most-delayed-airlines
changed-flights 5-15
find-itinerary DEN MCI 2-15 1030 limit=4
find-itinerary SAN JFK 6-15 1030 IAD 6-19 1215 SLC 6-21 SAN 6-25
~~~

## Précisions sur les requêtes

### `show-airports <airline_id>`

> Exemple et affichage attendu

~~~
> show-airports HA
HNL,Honolulu International Airport,Honolulu,HI
KOA,Kona International Airport at Keahole,Kailua/Kona,HI
LAS,McCarran International Airport,Las Vegas,NV
LAX,Los Angeles International Airport,Los Angeles,CA
LIH,Lihue Airport,Lihue,HI
OGG,Kahului Airport,Kahului,HI
PHX,Phoenix Sky Harbor International Airport,Phoenix,AZ
SFO,San Francisco International Airport,San Francisco,CA
~~~

### `show-airlines <port_id>`

> Exemple et affichage attendu

~~~
> show-airlines BOS
AA,American Airlines Inc.
B6,JetBlue Airways
DL,Delta Air Lines Inc.
NK,Spirit Air Lines
OO,Skywest Airlines Inc.
UA,United Air Lines Inc.
US,US Airways Inc.
VX,Virgin America
WN,Southwest Airlines Co.
~~~


### `show-flights <port_id> <date> [<time>] [limit=<xx>]`

> Exemple et affichage attendu

~~~
> show-flights ATL 2-26
2,26,4,DL,ATL,TPA,1920,24.0,67.0,406,2045,30.0,0,0
2,26,4,DL,ATL,SRQ,1040,13.0,71.0,444,1213,18.0,0,0
2,26,4,DL,ATL,DCA,1820,-2.0,64.0,547,2004,-26.0,0,0
2,26,4,DL,ATL,MSP,1057,12.0,121.0,907,1235,3.0,0,0
2,26,4,WN,ATL,LGA,645,-5.0,90.0,762,855,-29.0,0,0
2,26,4,DL,ATL,CAK,2104,-2.0,64.0,528,2250,-15.0,0,0
2,26,4,DL,ATL,LAX,1518,13.0,271.0,1947,1735,-4.0,0,0
2,26,4,DL,ATL,TLH,1939,16.0,42.0,223,2051,16.0,0,0
2,26,4,DL,ATL,MCI,1205,-5.0,114.0,692,1320,1.0,0,0
2,26,4,WN,ATL,BWI,1620,-5.0,71.0,577,1805,-21.0,0,0
2,26,4,DL,ATL,TUL,1045,-2.0,106.0,674,1200,-17.0,0,0
2,26,4,DL,ATL,HOU,1445,-5.0,110.0,696,1615,-26.0,0,0
2,26,4,DL,ATL,MIA,1735,15.0,92.0,594,1933,21.0,0,0
2,26,4,DL,ATL,BOS,905,23.0,108.0,946,1135,2.0,0,0
2,26,4,DL,ATL,GPT,1615,23.0,60.0,352,1644,8.0,0,0
2,26,4,DL,ATL,MCO,1155,189.0,68.0,404,1320,190.0,0,0
2,26,4,DL,ATL,PBI,822,65.0,81.0,545,1009,57.0,0,0
2,26,4,DL,ATL,ELP,1930,5.0,222.0,1282,2109,35.0,0,0
2,26,4,NK,ATL,IAH,1545,58.0,125.0,689,1708,61.0,0,0
2,26,4,DL,ATL,DFW,830,,,731,1003,,0,1
2,26,4,DL,ATL,TYS,1241,6.0,31.0,152,1331,2.0,0,0
2,26,4,DL,ATL,MLB,1526,10.0,70.0,442,1657,26.0,0,0
2,26,4,DL,ATL,RDU,1500,-2.0,48.0,356,1623,-14.0,0,0
2,26,4,DL,ATL,GRR,2040,1.0,87.0,640,2240,-11.0,0,0
~~~

### `most-delayed-flights`
> Exemple et affichage attendu

~~~
> most-delayed-airlines
9,24,4,EV,ORD,OMA,2229,-12.0,63.0,416,2359,-22.0,0,0
9,26,6,AS,SFO,SLC,2118,-3.0,76.0,599,2359,-4.0,0,0
9,29,2,F9,PHX,IAH,1925,-11.0,122.0,1009,2359,-13.0,0,0
9,4,5,DL,ATL,BWI,2205,0.0,94.0,577,2359,12.0,0,0
9,7,1,OO,LAX,SAN,2305,8.0,27.0,109,2359,-2.0,0,0
~~~

### `most-delayed-airlines`

> Exemple et affichage attendu

~~~
> most-delayed-airlines
AA,American Airlines Inc.
DL,Delta Air Lines Inc.
UA,United Air Lines Inc.
VX,Virgin America
WN,Southwest Airlines Co.
~~~

### `delayed-airline <airline_id>`

> Exemple et affichage attendu

~~~
> delayed-airline  AA
AA,American Airlines Inc.,180
~~~


### `most-delayed-airlines-by-airports <airport_id>`

> Exemple et affichage attendu

~~~
> most-delayed-airlines-by-airports  LAX
WN,Southwest Airlines Co.,30
NK,Spirit Air Lines,18
OO,Skywest Airlines Inc.,10
~~~


### `changed-flights <date>`

> Exemple et affichage attendu

~~~
> changed-flights 12-29
12,29,2,EV,ORD,DAY,1040,,,240,1254,,0,1
12,29,2,AA,ORD,MCO,921,,,1005,1306,,0,1
12,29,2,AA,LAX,ORD,600,,,1744,1213,,0,1
12,29,2,UA,IAH,SEA,2133,,,1874,29,,0,1
12,29,2,MQ,ORD,GRB,837,,,173,935,,0,1
12,29,2,EV,ORD,DSM,1552,,,299,1712,,0,1
~~~

### `avg-flight-duration <port_id> <port_id>`

> Exemple et affichage attendu

~~~
> avg-flight-duration LAX SFO
average: 54.9 minutes (416 flights)
~~~

### `find-itinerary <port_id> <port_id> <date> [<time>] [limit=<xx>]`

> Exemple et affichage attendu

~~~
> find-itinerary SFO CHA 6-15
6,15,1,WN,SFO,ATL,700,-5.0,250.0,2139,1450,-22.0,0,0
6,15,1,EV,ATL,CHA,1835,7.0,25.0,106,1927,-4.0,0,0
~~~


### `find-multicity-itinerary <port_id_depart> <port_id_dest1> <date> [<time>] <port_id_dest2> <date> [<time>] ... <port_id_destN> <date> [<time>]`

> Exemple et affichage attendu

~~~
> find-itinerary SAN JFK 6-15 1030 IAD 6-19 1215 SLC 6-21 SAN 6-25
...
~~~



## Déliverables

**La date de fin de projet est le dimanche 30 mai 2021 23:59 CEST. Tout retard entraînera des pénalités.**

Vous n'avez rien à imprimer, ni des documents à envoyer par e-mail. Il faudra donner l'accès du dépôt à votre correcteur et celui-ci corrigera à partir de la dernière version du dépôt ou de la branche `release` (si elle existe) avant la date limite. Les `push` qui suivraient la date limite ne seront pas considérés.

Sur votre dépôt devront figurer :

- le code source correctement commenté et indenté
- un `Makefile` pour la compilation automatique
- un `README.md` décrivant l'utilisation de votre programme
- Un fichier `Rapport.md` ou un rapport au format PDF (les autres formats ne sont pas acceptés) de 6 pages maximum qui explique vos choix de structures de données, vos choix de conception algorithmique et les principales étapes du déroulement de votre programme.

# Annexes

## Pour démarrer

Pour commencer à travailler, vous pouvez faire un fork du dépôt qui contient le fichier `README.md` que vous êtes en train de lire. Ne pas oublier d'ajouter les quatre intervenants dans la liste des personnes ayant accès à votre dépôt (menu `Settings` à droite puis Menu `Members` puis bouton `New Projet Member`).

On rappelle brièvement les commandes `GIT` que vous serez amené à utiliser :

- `git help cmd` : pour avoir de l'aide sur la commande `cmd`
- `git add fichier` : pour ajouter au dépôt un nouveau fichier ou un fichier modifié
- `git commit -m "message qui explique les modifications"` : pour sauvegarder les modifications dans le dépôt local
- `git push` : pour mettre à jour le serveur avec votre version du dépôt
- `git pull` : pour récupérer la dernière version du serveur
- `git status`: pour voir l'état de votre dépôt local par rapport au serveur
- `git log --pretty=format:"%h - %an, %ar : %s"`: afficher le log des commits

N'hésitez pas à vous référer au polycopié de programmation avancée ou le 'cheat sheet' pour git pour les détails de chacune des commandes.

## Évaluation
L’évaluation de ce projet sera faite en utilisant de manière intensive les outils de traitement automatique. D’une part parce que ce sont des outils qui sont utilisés dans le monde professionnel (et autant les maîtriser le plus tôt possible) et d’autre part puisque cela permet d’évaluer objectivement une partie de votre travail.

- Respect des conventions de codage avec `clang-format`
- Compilation automatique avec `Makefile`
- Qualité de code avec `clang-tidy` et `cppcheck`
- Respect des conventions des nommage des commits avec `commitlint` (optionnel)
- Détection automatique de plagiat avec `moss`
- Intégration continue avec `GitLab-CI`, `check`, `llvm-cov` / `gcov` (optionnel)
- Un exemple de dépôt GitLab conforme à ce qui est attendu est disponible à l’adresse suivante [https://gitlab.com/jeremie.dequidt/ima_prog_adv](https://gitlab.com/jeremie.dequidt/ima_prog_adv).

## Convention de nommage des commits
Pour etre plus facilement lisible, vos commits devront suivre une convention de nommage. La convention utilisée par AngularJS est devenue très populaire. Elle est décrite à l’adresse suivante [Commit Message Conventions](https://gist.github.com/stephenparish/9941e89d80e2bc58a153).


## Intégrer les changements du sujet dans votre dépôt suite à un fork

Si vous avez crée votre dépôt en utilisant la fonctionalité de fork, vous pouvez continuer à intégrer directement les mises-à-jour. Pour ce faire, vous devez :
1. Ajouter le dépôt comme un nouveau serveur distant différent à `origin`
2. Recupérer les changements en indicant d’utiliser ce dépôt et la branche master

Voici les commandes :
```bash
git remote add depot_sujet https://gitlab.univ-lille.fr/walter.rudametkin/projet_se3_2021.git
git pull depot_sujet master
```
En cas de conflit, vous devez les gérer manuellement. Si vous voulez annuler une merge, vous pouvez utiliser `git merge --abort` pour revenir à l’état juste avant le `git pull`.

En cas d’erreur, n’hésitez pas à consulter la commande `git remote -v` et `git remote remove <dépôt>` qui permettent d’afficher les dépôts distants et supprimer un dépôt distant, respectivement.

Une fois que vous avez intégré les changements, il faudra les partager sur votre dépôt distant avec
```bash
git push
```
