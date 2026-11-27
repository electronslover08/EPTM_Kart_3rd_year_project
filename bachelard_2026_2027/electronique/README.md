# ⚡ Électronique

Ce dossier contient les **schémas électroniques et les fichiers KiCad** du kart (KiCad 10) :

- **PCB de commande à distance** : lecture du récepteur radio, commande de l'accélération, du frein et de la direction, sélection du mode (manuel / à distance) ;
- **sécurités** : détection de la perte de liaison, arrêt d'urgence, chien de garde indépendant du microcontrôleur ;
- monitoring au volant et câblage.

Licence : [CERN-OHL-P-2.0](../../.github/LICENSES/CERN-OHL-P-2.0.txt). En cas de modification, indique la **date** et une **brève description** du changement (exigence de la licence).

## Contenu (état au 27.11.2026)

| Dossier | Contenu |
|---|---|
| [`001_controlleur_pwm/`](001_controlleur_pwm/) | Contrôleur PWM : lecture des 6 voies du récepteur, sortie RS-485. `projet_kicad_original/` = fichiers d'origine (ne pas modifier) ; `projet_kicad_version_eptm/` = version de travail pour la gravure EPTM (règles `.kicad_dru`, points de test dans `points_de_test.md`) |
| [`002_controleur_moteur/`](002_controleur_moteur/) | Contrôleur moteur : projet KICAD commencé (schéma, PCB vide) |
| [`xxx_modele_pcb/`](xxx_modele_pcb/) | Projet KiCad modèle, à copier pour un nouveau PCB |
| [`prompt_reprise_points_de_test_271126.md`](prompt_reprise_points_de_test_271126.md) | Note de reprise du PCB 001 : état, règles EPTM, points de test TP1 à TP7 |

## Bibliothèques tierces

Le symbole `lib_PIC18F45K22/PIC18F45K22-I_PT.kicad_sym` et l'empreinte TQFP-44 viennent de **SnapEDA / SnapMagic** (pièce Microchip PIC18F45K22-I/PT, récupérés le 26.11.2026). Ils ne sont **pas** sous les licences du reste du dépôt : SnapEDA les publie sous **CC BY-SA 4.0** avec l'exception « Design Exception 1.0 » (conditions complètes sur le site de SnapEDA). Source : <https://www.snapeda.com/parts/PIC18F45K22-I/PT/Microchip+Technology/view-part/>. Garde cette mention si tu modifies ces fichiers.

Le modèle 3D `lib_PIC18F45K22/3dmodels/PIC18F45K22-I_PT.step` est un modèle TQFP-44 **générique** retraité sous Open CASCADE (le modèle SnapEDA d'origine portait le nom `ATMEGA32-16AU`) : il ne représente que l'encombrement du boîtier. Les empreintes MAX485EESA (`lib_MAX485EESA`) sont des fichiers de bibliothèque de constructeur : leur origine exacte est à confirmer.
