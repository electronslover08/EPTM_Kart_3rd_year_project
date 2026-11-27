Je reprends mon projet KiCad 10 « 001_Controlleur_PWM » (contrôleur PWM du kart, projet de 3e année EPTM). Réponds en français et justifie chaque choix technique avec des chiffres. Vérifie tout avant de livrer (identifiants uniques, cohérence schéma ↔ PCB, isolations). Propose la meilleure solution, pas la plus simple. Livre des fichiers complets, et seulement ceux que tu modifies.

## Où sont les fichiers (sur mon PC)
Dossier connecté : N:\A_EPTM\Professionel\EPTM_Troisieme_annee\Inf-Z_PROJET
Projet : …\Conceptions électroniques\001_Controlleur_PWM\Projet KICAD 10.0\001_Controlleur_PWM\
- Les fichiers d'origine à la racine ne doivent JAMAIS être modifiés.
- La version de travail est dans `Version_EPTM\` :
  - fichiers du projet : .kicad_pro, .kicad_sch, .kicad_pcb, .kicad_dru ;
  - fp-lib-table, sym-lib-table, lib_MAX485EESA\, lib_PIC18F45K22\ ;
  - placement_EPTM.png et points_de_test.md.
- J'ai peut-être modifié `Version_EPTM` dans KiCad depuis le 27.11.2026 (routage, vias). Compare d'abord les dates de modification. Travaille toujours sur l'état actuel des fichiers, jamais sur un souvenir.
- Avant toute modification, fais une copie de sauvegarde des fichiers que tu vas toucher.

## Le circuit
- **Entrées PWM :** 6 voies venant d'un récepteur Spektrum (J3 STR, J4 THR, J5–J8 AUX1–4). Pull-down de 10 k, puis tampon 74HCT541 (U1), puis filtre RC 1 k / 100 pF (R16–R21, C8–C13), puis le PIC18F45K22 en TQFP-44 (U3, quartz de 20 MHz Y1).
- **Sortie :** UART (TX1, RX1, DIR), transceiver MAX485 (U2), puis RJ12 J9. J9 transporte aussi VBUS (alimentation des autres modules) et GND. Polarisation de repos : R3 et R4 de 390 Ω, terminaison R5 de 120 Ω.
- **Programmation :** ICSP par J1 (RJ12). RESET passe par R1 (100 Ω) jusqu'à /MCLR, avec un tirage R2 de 47 k vers VDD.
- **Alimentation :** J10 (borne à vis) → F1 T150mA → VDD (alimentation de la carte), et J10 → F2 T1A → VBUS (vers J9).
- **Interface opérateur :** LED D1 ; J2 = BATT/PROG.

## Règles EPTM (gravure au perchlorure, trous non métallisés)
- Isolation 20 mil (0,508 mm), pistes de 20 mil (15 mil possible si très soigneux), 25 mil (0,635 mm) jusqu'au bord.
- Vias : cuivre 50 mil (1,27 mm), perçage 25 mil (mèche de 0,6 mm). Ils sont faits en fil de 0,5 mm soudé des deux côtés (« tricotage »). Objectif : environ 18–20 vias.
- Les traversants sont soudés uniquement au BOTTOM : leurs pastilles TOP ne sont pas connectables. Tous les CMS sont au BOTTOM.
- Seule exception : autour du PIC, pastilles réduites à 0,40 mm et règle `.kicad_dru` à 15 mil.

## État au 27.11.2026
- Placement terminé. Contour de carte (114 ; 69)–(189 ; 119) mm. 4 entretoises M3 en (119 ; 75), (184 ; 75), (119 ; 114), (184 ; 114).
- Logo EPTM et texte « BACNAT - 27.11.2026 / 001_Contrôleur_PWM V1 » en double, sur F.Cu et F.SilkS (1,4 mm / 0,381 mm).
- Zone GND sur le TOP (connect_pads no, isolation 0,508). Classes de nets : VDD à 1,016 mm ; VBUS, entrée et GND à 1,524 mm.
- Seulement 2 vias posés (pont VIN entre F1 et F2, côté TOP). Le routage n'est pas fait.
- 6 zones de franchissement prévues, à garder libres :
  - (150,6 ; 104,2) STR/THR ;
  - (171,6 ; 93,2) RS-485 A/B ;
  - (168,4 ; 88,6) ICSP ;
  - (133,0 ; 86,2) VDD vers J1 ;
  - (155,2 ; 81,2) masse de C10 ;
  - (126 ; 106) plan GND TOP.
- Une étude de routage automatique (indicative) donnait 35 vias et 4 nets en conflit : le centre de la carte est saturé.

## Points de test : décision prise et vérifiée
Ces 7 points ont été vérifiés : isolation 20 mil vis-à-vis des pastilles, pas sous un corps TOP, hors des zones de franchissement, hors des couloirs de l'étude de routage, au moins 4,9 mm entre eux. Empreinte : pastille ronde Ø2,2 mm, trou 1,0 mm, boucle Keystone 5000–5004 ou fil, soudée au BOTTOM.

| TP | Net | Position (x ; y) mm | Raccord court vers |
|---|---|---|---|
| TP1 | GND | 182,75 ; 108,00 | R4 pastille 1 |
| TP2 | GND | 131,50 ; 91,25 | R13 pastille 2 |
| TP3 | VDD | 122,50 ; 83,75 | J2 broche 2 |
| TP4 | RS485_A (actuellement Net-(U2-A)) | 177,50 ; 108,00 | R3 pastille 1 |
| TP5 | RS485_B (actuellement Net-(U2-B)) | 185,25 ; 103,75 | R4 pastille 2 |
| TP6 | AUX2 | 149,75 ; 83,50 | R19 pastille 2 |
| TP7 | STR_IN (net de J3 broche 1, avant le 74HCT541) | 118,00 ; 89,75 | J3 broche 1 |

Les autres signaux (STR, THR, AUX1, AUX3, AUX4, TX1, RX1, DIR, /MCLR) se mesurent sur des pastilles existantes au BOTTOM : c'est listé dans points_de_test.md. Ne jamais palper OSC1/OSC2.

## Ce que je veux mercredi
1. **Ajouter TP1 à TP7 dans le schéma ET dans le PCB,** pour que « Mettre à jour le PCB depuis le schéma » ne change rien.
   - Crée une bibliothèque locale `lib_TestPoint` : un symbole « TP » et deux empreintes. Une empreinte pour les signaux ; une variante GND avec la pastille en `(zone_connect 1)` (thermique), pour qu'en soudant le point aussi au TOP il relie le plan GND et remplace des vias.
   - Ajoute la bibliothèque dans fp-lib-table et sym-lib-table avec ${KIPRJMOD}.
   - Le schéma n'utilise que des global labels et des symboles power:GND / power:+5V. Le net VDD porte le global label « VDD ».
   - Pour A, B et STR_IN, ajoute des labels sur les nets existants, puis renomme ces nets de la même façon dans le PCB.
   - Ajoute une étiquette « ${VALUE} » sur F.SilkS et sur F.Cu à côté de chaque point de test.
   - Placement du bloc dans le schéma : zone libre x 200–282, y 113–158 mm, dans un cadre « Points de test ».
2. **Ensuite seulement :** dis-moi s'il vaut la peine de déplacer quelques passifs pour libérer une zone de boucles au centre (STR, THR, TX1, RX1, DIR, /MCLR). Chiffre ce que ça coûte en vias avant de le faire.
3. **À la fin :** liste ce que je dois contrôler dans KiCad (ERC, mise à jour du PCB depuis le schéma, DRC).

## Détails techniques déjà vérifiés
- Formats : PCB en version 20260206, schéma en 20260306. Fins de ligne CRLF. Pastilles écrites `(net "NOM")`. Empreintes BOTTOM déjà miroir dans le fichier.
- Le schéma contient les instances par nom de projet.
- `(zone_connect N)` sur une pastille prime sur le réglage de la zone. Valeurs : 0 = aucune, 1 = thermique, 2 = pleine.
- ${VALUE} est résolu dans les textes d'empreinte.
- Les bibliothèques officielles KiCad ne sont pas accessibles : GitLab est bloqué et le miroir GitHub date de 2020. D'où la bibliothèque locale.
