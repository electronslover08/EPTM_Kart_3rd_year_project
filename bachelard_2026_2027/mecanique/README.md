# 🔩 Mécanique

Ce dossier contient les **modèles 3D et les pièces conçues avec Fusion** : supports du moteur de direction et de l'actionneur de frein, fixations, revêtement imprimé en 3D.

Licence : [CERN-OHL-P-2.0](../../.github/LICENSES/CERN-OHL-P-2.0.txt). En cas de modification, indique la **date** et une **brève description** du changement (exigence de la licence).

## Contenu (état au 27.11.2026)

| Dossier / fichier | Contenu |
|---|---|
| [`roues_crantees/`](roues_crantees/) | Roues à chevrons de la direction motorisée (46 dents, entraxe 92 mm) |
| `roues_crantees/HerringboneGears/` | Script Fusion `HerringboneGears.py` qui génère la paire de roues (diamètre primitif et nombre de dents demandés, charge radiale et contrainte de flexion affichées) |
| `roues_crantees/chevrons_z46_a92_h10_paire.step`, `Roues_crantees.step`, `RoueB.stl` | Paire de roues exportée (STEP, STL) |
| `roues_crantees/Roue_MOTEUR_PAPA.3mf` | Roue du réducteur, au format 3MF |

Les modèles 3D du moteur OMC 34E1K-85 et du réducteur EG34-G5 (fichiers STEP du constructeur) ne sont **pas** publiés ici : leurs droits de redistribution n'ont pas été vérifiés. Ils se téléchargent sur le site d'OMC StepperOnline.
