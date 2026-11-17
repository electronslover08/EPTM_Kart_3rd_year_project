# 🏎️ Projet kart 2026–2027

| | |
|---|---|
| **Élève responsable** | Nathanaël Bachelard |
| **Année scolaire** | 2026–2027 |
| **Sujet** | Pilotage à distance du kart |
| **Cahier des charges** | **Version 2 (en vigueur)** : [PDF](documentation/cahier_des_charges/Cahier_des_charges_kart_Bachelard_v2.pdf) · [Word](documentation/cahier_des_charges/Cahier_des_charges_kart_Bachelard_v2.docx)<br>Version 1 (rénovation, remplacée) : [PDF](documentation/cahier_des_charges/Cahier_des_charges_kart_Bachelard.pdf) · [Word](documentation/cahier_des_charges/Cahier_des_charges_kart_Bachelard.docx) |

## 🎯 Objectif

Rendre le kart **pilotable à distance**, tout en gardant le **pilotage manuel** :

```
radiocommande ──► récepteur ──► PCB de commande ──► accélération (contrôleur moteur existant)
                                       │         ──► freinage (actionneur sur les nouveaux freins)
                                       │         ──► direction (un moteur avec réducteur)
                                       └── sécurités : perte de liaison, arrêt d'urgence, chien de garde
```

Le moteur est conservé. Les freins, actuellement dangereux, sont changés, et les pneus remplacés.

> [!NOTE]
> **L'idée a changé en cours de route.** Mon idée de départ était de télécommander le kart. Après l'avoir examiné aux portes ouvertes, j'ai d'abord proposé de le rénover avant tout (cahier des charges **version 1**). Après discussion avec le professeur, le projet **revient finalement en partie à l'idée de départ** : le pilotage à distance, avec seulement les remises en état qu'il exige (cahier des charges **version 2**). Les deux versions sont conservées pour garder la trace de ce choix.

## Contenu du dossier

| Dossier | Contenu |
|---|---|
| [`assets/`](assets/) | Images du projet |
| [`documentation/`](documentation/) | Cahier des charges, rapports, notices |
| [`electronique/`](electronique/) | Schémas et PCB (KiCad) |
| [`logiciel/`](logiciel/) | Code et firmware |
| [`mecanique/`](mecanique/) | Modèles 3D et pièces (Fusion) |
