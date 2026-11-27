# Direction du kart — cotes reconstruites d'après les photos (mise à jour 27.11.2026, 10e révision)

Cotes tirées de 4 photos du kart, mises à l'échelle avec la colonne Ø20 et la cote donnée par Nathanaël (≈ 225 mm entre le bloc alu et le boulon de la pièce jaune). Précision estimée : ± 1 cm. À confirmer au mètre avant fabrication. Le bas de colonne a été validé visuellement par Nathanaël le 25.11.2026.

Révision 8 (26.11.2026) : seule la section « Motorisation retenue » change. La transmission 62/31 + réducteur 10:1 est remplacée par moteur Trinamic QSH8618 + réducteur OMC EG34-G5 (5:1) + deux roues chevrons 46/46 (1:1). Toutes les cotes du kart ci-dessous sont inchangées.

Révision 9 (27.11.2026) : réponse d'OMC reçue. Le Trinamic pose trois problèmes (centrage, manchon Ø12, vis) ; le choix en cours devient le kit moteur + driver OMC 1-CL86T-P85-V41 avec le réducteur EG34-G5, en attente de confirmations écrites (voir « Évolution du 27.11.2026 »). Le script Fusion s'exécute sans erreur et demande maintenant le diamètre des roues.

Révision 10 (27.11.2026) : simulation 3D refaite (v2) avec les fichiers réels : STEP OMC du moteur 34E1K-85 et du réducteur EG34-G5, 3MF de la roue « Roue MOTEUR PAPA ». Nouvelles sections « Cotes relevées dans les fichiers CAO » et « Implantation v2 ». La longueur du moteur passe de 112,3 à 134,1 mm (codeur compris). Toutes les cotes du kart sont inchangées.

Repère : X vers l'avant, Y vers le haut (0 = tôle de plancher), Z vers la droite du pilote. Origine au sol sous la soudure de la patte haute. `s` = distance le long de la colonne depuis cette soudure, vers le volant. `w` = distance perpendiculaire à la colonne, vers le haut et l'avant.

## À savoir avant tout : les photos ne sont pas prises dans la même position de volant
- Photo 1 (trois quarts) : pattes de direction pointant vers le bas et l'arrière (≈ −25° autour de la colonne, 0° = pattes droit vers le bas).
- Photos 2 et 4 (vue du pilote, vue de dessus) : pattes couchées vers la droite du pilote (≈ +78°), volant braqué à gauche.
- Écart entre les deux : ≈ 103° de rotation de colonne.
- Conséquence : on ne peut pas trianguler une pièce mobile (pattes, bloc, rotules, biellettes) en mélangeant la photo 1 avec les photos 2 ou 4.

## Colonne
- Tube chromé Ø20, incliné d'environ 46° par rapport à l'horizontale. Toute la colonne tourne : aucun tube fixe ne l'entoure.
- Tout ce qui est soudé ou serré sur la colonne tourne avec elle : pattes et bloc alu, bague laiton, moyeu, volant (et la roue dentée prévue).
- Soudure de la patte haute : s = 0, à 8,5 cm du plancher. Soudure de la patte basse : s = −3,7 cm.
- Entrée dans le nœud bleu du châssis : s ≈ −9 cm.
- Bague laiton (avec vis de blocage) : s ≈ 22 cm (de 21,7 à 22,9). Rondelle noire entre la bague et la pièce jaune.
- Moyeu doré épaulé : de s ≈ 30 cm à s ≈ 39,4 cm ; Ø ≈ 36 mm en bas, ≈ 42 mm au milieu, ≈ 65 mm sous le volant ; vis radiales.
- Portion de colonne libre : de s ≈ 1 cm à s ≈ 21,7 cm.

## Pièce jaune et haut de l'arceau (mesurés sur les photos 1, 3 et 4, pièces fixes)
- La pièce jaune est le palier haut de la colonne ; elle est serrée entre les deux plats du haut de l'arceau.
- Grosse vis CHC qui traverse l'arceau et la pièce jaune d'un côté à l'autre : s ≈ 25,4 cm, w ≈ 2 cm au-dessus de l'axe. Tête côté droit, bout fileté dépassant côté gauche.
- Trou libre dans l'oreille de la pièce jaune, parallèle à cette vis : s ≈ 25,4 cm, w ≈ 3,7 cm (erreur de recoupement entre 3 photos : 0,2 cm). Diamètre ≈ 8 mm, à mesurer.
- Entraxe vis ↔ trou libre : ≈ 17 mm.
- Une petite vis sous la grosse, sur le plat droit de l'arceau : s ≈ 24,8 cm, w ≈ 0.
- Matière de la pièce jaune : inconnue (aspect d'impression 3D), à vérifier.

## Bas de colonne (pattes plates)
- Deux pattes acier plates, perpendiculaires à la colonne, soudées à 3,7 cm l'une de l'autre et tournées du même côté. La patte basse remonte d'environ 1 cm vers son bout.
- Bloc alu posé sur la patte haute : ≈ 55 × 46 × 10 mm (± 5 mm). 5 perçages : 2 vis fraisées, 1 trou vide, 1 trou central, 1 vis CHC.
- Les 2 vis fraisées : à ≈ 52 mm de l'axe de la colonne, entraxe ≈ 24 mm. Elles traversent le bloc, la patte haute, une entretoise (≈ 10 mm), une rotule et la patte basse.
- Rotule de la biellette droite : sur la vis de droite, entre les deux pattes (visible sur la photo 1).
- Rotule de la biellette gauche : cachée sur les 4 photos ; hypothèse : sur la vis de gauche, entre les deux pattes comme la droite.
- Hauteur des rotules au-dessus du plancher : ≈ 3 à 4 cm en position photo 1, ≈ 5 à 7 cm en position photos 2 et 4.
- Dans le modèle, le bout du bloc passe à moins de 1 cm du tube argenté quand les pattes pointent vers le bas : en dessous de la précision, à regarder sur le kart.

## Arceau bleu (support de la pièce jaune)
- Deux jambes Ø ≈ 22, écartées de ± 2,8 cm en haut et ± 8,5 cm en bas, penchées vers l'avant : pied à X ≈ −26, haut à X ≈ −16, Y ≈ 28. Extrémités hautes aplaties et arrondies.
- 4 pattes à trou soudées à l'arrière des jambes : ce sont de petits tubes **verticaux**, trou ouvert sur le dessus (corrigé par Nathanaël le 25.11.2026). Hautes : dessus à (−19,6 ; 24 ; ± 4,5), ≈ 24 mm de haut. Basses : dessus à (−25,6 ; 10 ; ± 8,1), ≈ 16 mm de haut. La jambe passe tout contre chaque patte (la patte est soudée sur son flanc).
- Une gaine blanche spiralée (câbles du volant) est attachée par des colliers sur la jambe gauche, côté intérieur (photo 1).

## Châssis autour
- Plancher : tôle noire percée d'un seul tenant, X ≈ −37 à +18, Z ≈ −12,5 à +17,5 (photo 4).
- Tube argenté soudé (ajout d'un ancien projet) : transversal, X ≈ −5 à −6, axe à Y ≈ 2 à 3, de Z ≈ −11 à +18,7 ; gros cordon de soudure entre Z ≈ +8,5 et +17.
- Tube blanc : transversal, X ≈ +1,7, Y ≈ 2, de Z ≈ −9 à +14 ; colliers à vis à Z ≈ −4,5 et +7 (photos 1, 2, 4).
- Câble de frein gainé bleu foncé avec deux repères rouges : il longe le côté gauche vers X ≈ −19 à −6, passe par-dessus le tube argenté, sous le bas de colonne, puis repart vers l'avant droit (position ± 2 cm).
- Tube bleu avant : transversal, X ≈ +3 à +4, sous le bas de la colonne ; colliers bleus à Z ≈ −12,7 et +17.
- Longeron droit à Z ≈ +18 ; côté gauche, tube à Z ≈ −10,5 (peu visible sur les photos).
- Mécanisme doré à câbles côté gauche, vers X ≈ −14, Z ≈ −13,5 : 2 molettes moletées à (−17 ; 8 ; −12,4 et −14,6), ressorts vers l'avant jusqu'à X ≈ −11,5, chapes à X ≈ −10, câbles ensuite vers l'avant gauche.
- Siège : bord avant de la coque à X ≈ −34 à −38 au centre, plus en avant sur les côtés (photo 4) ; une vis de fixation vers (−40 ; 4 ; −6,4).

## Motorisation retenue (26.11.2026, transmission validée par Nathanaël : « partons du principe que ça va fonctionner avec le 5:1 »)

Principe : moteur pas à pas + réducteur planétaire OMC StepperOnline EG34-G5 (5:1) + deux roues chevrons 46/46 (1:1). Rapport total 5:1. Moteur en cours de choix : OMC 34E1K-85 (kit 1-CL86T-P85-V41) ; le Trinamic QSH8618-96-55-700 reste la solution de repli.

Abandonné :
- 1re version : deux bagues de guidage sur la colonne + deux liens anti-rotation vers l'arceau (Nathanaël ne veut rien monter sur la colonne en dehors de la roue dentée).
- 2e version (25.11.2026) : SHW86118-18-600 + réducteur 5:1 + roues 62/31 (10:1 au total), entraxe 93 mm.
- Roues 2:1 derrière le G5 (pignon 31 dents sur la sortie du réducteur) : effort sur la sortie ≈ 1153 N à l'arrêt contre 1000 N de limite supposée (marge 0,87), inertie ramenée ×4, manuel plus dur moteur coupé.

### Évolution du 27.11.2026 : réponse d'OMC et passage à un moteur OMC (en attente de confirmations)

Réponse d'OMC (technique) :
- **Centrage :** alésage du réducteur Ø73H7 (73,000 à 73,030 mm) contre un centrage du Trinamic à 73,02 ± 0,05 (72,970 à 73,070 mm) : jusqu'à 0,07 mm d'interférence au pire cas. OMC parle d'incompatibilité mécanique « pas complète ».
- **Manchon :** pas de manchon Ø12 en standard (paroi trop mince) ; adaptateur Ø19 vers Ø12 sur mesure possible, sans prix ni délai.
- **Vis :** entrée prévue pour des vis M5, taraudage de 12 mm ; vis fournies ≈ 20 mm, donc suffisantes si la bride moteur fait au moins 8 mm d'épaisseur (à mesurer, sinon vis plus courtes). Méplat accepté (serrage à pince) ; concentricité moteur/accouplement à soigner.
- **Limites :** 90 N·m est le couple continu et aussi le maximum admissible. Charge radiale 1000 N à 20 mm de la bride de sortie (confirmée). Pas de donnée sur la charge axiale ni sur le couple de rétro-entraînement ; rétro-entraînement « non recommandé » (raison non donnée). Poids exact du réducteur : 2,9 kg.
- **Logistique :** livraison en Suisse en 5 à 7 jours ouvrables par express, transporteur au choix ; DDP (OMC dédouane et paie les droits) ou DAP ; code douanier 8483409090, valeur déclarée = prix du réducteur.
- **Paiement :** net 30 jours après réception possible pour les clients universitaires, commandes sous 500 USD ; à demander si l'EPTM est éligible.
- Lubrification à vie, aucune maintenance.

Conséquences :
- Le plan de roues : le centre de la denture doit rester à ≤ 20 mm de la bride de sortie (roue de 10 mm de large placée vers 15 à 25 mm sur l'arbre de 40 mm). Marge 1000 / 777 = 1,29 avec le Trinamic. **Voir la révision 10 : la roue du 3MF met la denture à 22,7 mm.**
- Rétro-entraînement : le kart garde la conduite manuelle moteur coupé, donc le volant entraînerait le réducteur par la sortie. Question ouverte posée à OMC (limite de vitesse d'entrée, durée de vie).

Choix en cours : kit OMC **1-CL86T-P85-V41** (130,77 USD ; 46 unités annoncées en stock en Chine) + réducteur **EG34-G5** (85,04 USD) = 215,81 USD hors livraison. **Aucune commande n'est passée** : Nathanaël collecte des informations et un devis ; la commande sera faite plus tard par l'école.
- Moteur 34E1K-85 : boucle fermée avec encodeur optique 1000 PPR (4000 CPR), 8,5 N·m, 6 A, 0,54 Ω, 5 mH, 112,3 mm (sans le codeur ; 134,1 mm codeur compris d'après le STEP), 3,6 kg, arbre Ø14 × 37 mm avec clavette de 25 mm, fils de 270 mm.
- Driver CL86T-V41 : 0 à 8 A crête (5,7 A efficaces), alimentation 18 à 80 VAC ou 24 à 110 VDC (batterie 48 V compatible), 7 à 16 mA sur les signaux logiques, impulsions jusqu'à 200 kHz. Remplace le DSH-600 de Transmotec. Réglage du courant et niveau logique des signaux STEP/DIR/ENA/ALARM : à lire dans le manuel CL86T-V41.
- Câbles du kit : moteur 1,7 m, encodeur 1,7 m, RS232 de débogage. Pas d'alimentation.
- Longueur moteur + réducteur : 134,1 + 110 = 244,1 mm codeur compris (et non 222,3 mm) ; masse 3,6 + 2,9 = 6,5 kg (Trinamic : 5,7 kg).
- Charge radiale à l'arrêt, roues Ø92, moteur 8,5 N·m à pleine puissance : C_out = 5 × 0,96 × 8,5 = 40,8 N·m, F = 40,8 / (0,046 × cos(20°)) ≈ 944 N, marge 1000 / 944 = 1,06. **Régler le courant à ≈ 4,9 A** (6,0 × 7,0 / 8,5) pour retrouver 777 N (couple supposé proportionnel au courant).
- Plafond de couple : avec 1000 N admissibles, le couple maximal à la colonne vaut ≈ 1000 × cos(20°) × r_colonne × 0,95, soit ≈ 41 N·m pour r = 46 mm, quel que soit le moteur. Pour aller plus loin, agrandir la roue de la colonne.
- Arguments : arbre Ø14 = entrée native du réducteur (pas d'adaptateur), un seul fournisseur, un colis, un DDP, une facture sous 500 USD ; l'encodeur détecte un décrochage de pas, ce qu'une boucle ouverte ne voit pas.

Alternatives étudiées :
- 34HS46-6004S1 (boucle ouverte) : 8,5 N·m, 6 A, 0,58 Ω, 5 mH, 118 mm, 4,0 kg, arbre Ø14 à clavette de 32 mm, en stock, ≈ 78 CAD. Repli si la boucle fermée pose problème.
- 34HS46-5004D (méplat Ø14, 114 mm) : 11 mH, donc moins de couple à vitesse élevée avec 48 V ; écarté.
- 34HS59-6004S (12 N·m, 6 A, 0,7 Ω, 7,6 mH, 151,5 mm, ≈ 5,6 kg brut) : écarté. À pleine puissance C_out = 57,6 N·m et F ≈ 1333 N, marge 0,75 ; il faudrait le brider à 3,5 A (6,0 × 7,0 / 12,0).
- Trinamic QSH8618-96-55-700 : solution de repli seulement si OMC fournit l'adaptateur Ø19 vers Ø12 et confirme le centrage.

Premier mail à OMC (27.11.2026), demande d'informations (rédigé comme une intention de commander ; le mail suivant précise qu'il s'agit seulement d'une demande de devis) : centrage du 34E1K-85 face à l'alésage Ø73H7 ; arbre à clavette dans le manchon à pince (sinon version à méplat) ; rétro-entraînement ; charge axiale ; jeu 10 ou 15 arcmin ; stock ; devis DDP complet avec livraison ; éligibilité de l'EPTM au net 30 ; réglage du CL86T à ≈ 4,9 A et signaux logiques 5 V.

Réponse d'OMC du 27.11.2026 (2e échange) :
- **Centrage :** celui du 34E1K-85 est Ø73 0/−0,05 mm (72,95 à 73,00), compatible avec l'alésage Ø73H7 du réducteur (73,000 à 73,030) : jeu diamétral de 0 à 0,08 mm. Confirmé par OMC.
- **Arbre :** le manchon à pince du EG34-G5 n'accepte pas un arbre muni de sa clavette ; il faut retirer la clavette. La rainure reste donc ouverte sous la pince. Couple transmissible de la pince avec rainure vide : demandé à OMC (couple moteur à transmettre : 8,5 N·m à 6 A, ≈ 6,9 N·m à 4,9 A).
- **Rétro-entraînement :** possible sans effet sur la durée de vie pour une direction manuelle à très basse vitesse de sortie ; garder la vitesse sous 10 tr/min (pas précisé si c'est la vitesse de sortie ou d'entrée : demandé). Leur réserve initiale visait les hautes vitesses avec de fortes charges.
- **Charge axiale admissible :** 800 N. Pire cas des chevrons (une seule moitié porte la charge, à l'arrêt, 33,6 N·m en sortie) : F_t = 33,6 / 0,046 ≈ 730 N, F_ax = 730 × tan(30°) ≈ 422 N, marge 800 / 422 ≈ 1,9.
- **Jeu du réducteur :** 10 arcmin (tableau et titre de la page concordent, il n'y a plus de doute), soit 0,167° en sortie ; avec les dents (≈ 0,12°), ≈ 0,29° à la colonne.
- **Stock :** kit et réducteur en stock. Livraison par FedEx ; le devis total a été envoyé par OMC sous forme d'image (montants non reportés ici).
- **Paiement :** net 30 jours après réception pour les établissements publics, commandes sous 500 USD.
- **CL86T-V41 :** le courant se règle par le logiciel livré (≈ 4,9 A possible) ; les entrées STEP/DIR/ENA acceptent une logique 5 V par les DIP switches du dessus.

Panier OMC vu le 27.11.2026, à titre indicatif (aucune commande passée) : EG34-G5 54,28 € ; kit 1-CL86T-P85-V41 119,00 € ; matériel 173,28 € ; FedEx Express 113,28 € ; taxes estimées 26,08 € ; total 312,64 € pour 8,41 kg. La livraison représente 36 % du total, soit 65 % de la valeur du matériel et ≈ 13,5 €/kg ; les taxes ≈ 9,1 % de (matériel + livraison). Sous 500 USD tant que l'euro vaut moins de 1,60 $. Le prix de la livraison paraît très élevé : question posée à OMC (calcul du poids facturé, poids volumétrique, surcharges).

Dernier mail à OMC (demande de devis, en attente d'envoi, version du 27.11.2026 à 11 h 30) : précise qu'il ne s'agit pas d'une commande, que rien ne doit être traité ni expédié, et que l'école commandera plus tard. Il demande : 1) un devis formel adressé à l'école en DDP avec TVA suisse et durée de validité ; 2) la durée de garantie du stock et des prix ; 3) pourquoi la livraison FedEx coûte autant (calcul du poids facturé, poids volumétrique, surcharges) ; 4) le couple de la pince rainure vide ; 5) le sens de la limite de 10 tr/min (sortie ou entrée) ; 6) confirmation des vis M5 × 20 mm avec la bride moteur de 9 mm du STEP (11 mm de filet engagé sur 12 mm). Retirés à la demande de Nathanaël : les conditions de la facture à 30 jours (déjà donnée : net 30 pour les établissements publics sous 500 USD) et l'option de livraison moins chère.

### Cotes relevées dans les fichiers CAO (révision 10)

Mesures faites sur la géométrie exacte des fichiers (lecture OpenCascade), pas sur les pages web.

Moteur OMC 34E1K-85 (`34E1K-85.STEP`) :
- Bride carrée 85,8 mm, épaisseur 9 mm ; 4 trous Ø6,5 sur Ø98,4 (carré de 69,6). Les vis M5 de 20 mm fournies avec le réducteur prennent donc 11 mm dans les 12 mm de filet.
- Centrage Ø73, 1,6 mm de haut ; gorge Ø73 à Ø82 de 0,5 mm autour.
- Arbre Ø14, 37 mm depuis la face de la bride, clavette de 5 mm sur 25 mm (de 12 à 37 mm).
- Longueurs depuis la face de la bride : flasque avant 15 mm, stator jusqu'à 94,8 mm, flasque arrière jusqu'à 112,3 mm, **codeur jusqu'à 134,1 mm**.
- Boîtier de sortie des câbles sur une face, près de l'arrière : 30,4 × 31 mm, 8,5 mm de saillie (de 91,3 à 122,3 mm de la bride). Les câbles en sortent vers l'avant, le long de la face.

Réducteur OMC EG34-G5 (`EG34-G5.STEP`) :
- Longueur de la face d'entrée à la face de la bride de sortie : 110,0 mm. Arbre de sortie : 40,2 mm au-delà de la bride.
- Entrée : bride carrée 90 mm à coins coupés Ø119 ; alésage de centrage Ø73, 8 mm de profondeur ; 4 taraudages M5 (perçage Ø4,2, 20 mm) sur Ø98,4 ; 2 trous d'accès Ø13 pour le serrage du manchon, sur les côtés.
- Corps Ø90.
- Sortie : bride carrée 90 mm à coins coupés Ø119, 11,5 mm d'épaisseur, **4 trous lisses Ø6,5 sur Ø100 (vis M6, pas de taraudage)** ; centrage Ø80, 3 mm de haut ; épaulement Ø25 de 2,2 mm ; arbre Ø20 (de 5,2 à 40,2 mm de la bride) ; rainure de clavette 6 × 3,5 mm de 7,2 à 37,2 mm de la bride. Une clavette 6 × 6 dépasse donc de 2,5 mm.
- Le STEP ne contient pas les roulements de sortie : impossible de recalculer la charge radiale pour une autre distance que 20 mm.

Roue `Roue MOTEUR PAPA.3mf` (objet « RoueB ») :
- 46 dents, Ø de tête 95,46, Ø de pied 87,67 (Ø primitif 92 : conforme au script), denture de 10 mm, chevrons symétriques.
- **Moyeu Ø45 × 35 mm** (12,5 mm de chaque côté de la denture), **alésage Ø20,5 sans rainure de clavette**.
- 8 trous radiaux Ø ≈ 4 (4 de chaque côté du moyeu, à 0°, 90°, 180°, 270°, à 11,8 mm du plan médian) : vis de blocage.
- Volume 91,9 cm³ : 721 g en acier, 248 g en aluminium.
- Calage : la roue de la colonne doit être **le miroir** de cette roue. Contrôle numérique : deux roues identiques se chevauchent de 3,2 mm en bout de denture ; avec le miroir et un calage de 3,95° (demi-pas : 3,91°), aucun sommet d'une roue n'est dans l'autre, à −40°, −25°, 0°, +30°, +78° et +85° de braquage.

Conséquences pour la roue du réducteur :
- Le moyeu repose sur l'épaulement Ø25 (à 5,2 mm de la bride) et arrive juste au bout de l'arbre (40,2 mm) : **le milieu de la denture est à 22,7 mm de la bride, au lieu de ≤ 20 mm.**
- Charge admissible estimée à 22,7 mm : 1000 × 20 / 22,7 ≈ 881 N. Hypothèse prudente (limite fixée par le moment à la bride) ; la vraie valeur est au moins celle-là.
- Charge radiale avec l'angle de pression apparent du chevron (α_t = atan(tan(20°) / cos(30°)) = 22,8°) : à 4,9 A, Ft = 33,3 / 0,046 = 724 N, F = 724 / cos(22,8°) = 786 N → marge 881 / 786 = 1,12. À 6 A : F = 962 N → marge 0,92, **à éviter**.
- Pour revenir à 20 mm : raccourcir le moyeu de 2,7 mm côté réducteur (12,5 → 9,8 mm). Les trous de vis de blocage de ce côté tombent alors à 1 mm du bord : il faut les supprimer et passer par une rainure de clavette.
- L'alésage sans rainure ne passe pas sur la clavette de l'arbre. Couple à transmettre à 4,9 A bloqué : 33,3 N·m, soit 3,3 kN tangentiels au rayon de 10 mm. Solution proposée : rainure de 6 mm dans l'alésage (ajustement Ø20 H7 au lieu de Ø20,5).
- Sur la colonne (tube chromé Ø20), même effort de 3,3 kN à reprendre par les vis de blocage : risque de glissement et de marquage du chrome. À traiter (lamages pour les vis, ou serrage par collier).

### Implantation v2 (révision 10, simulation « Direction du kart 3D — v2 »)

Repère de la colonne, en cm. Moteur et réducteur au-dessus de la colonne (côté +w), parallèles à elle, comme dans la v1.
- Roue de la colonne : milieu de la denture à s = 19,3 ; moyeu de s = 17,55 à 21,05, soit 6,5 mm sous la bague laiton.
- Roue du réducteur : même plan, axe à w = 9,2 (entraxe 92 mm).
- Face de la bride de sortie : s = 17,03. Face d'entrée du réducteur et face de la bride moteur : s = 6,03. Arrière du codeur : s = −7,38.
- Boîtier des câbles du moteur tourné vers +w (dessus), câbles vers l'avant au ras du plancher ; place du driver à définir.
- Chape (proposition) : platine de 8 mm sur la bride de sortie (alésage Ø80 sur le centrage, 4 vis M6 × 25 avec écrous), de w = 2,9 à 15,2 et |z| ≤ 6,9 ; 2 joues de 5 mm à |z| = 6,9 à 7,4, reprises en haut comme dans la v1 (tige à la place de la grosse vis de l'arceau, tige dans le trou de la pièce jaune, 2 équerres sur les pattes hautes) ; voile avant de 5 mm (w = 14,7 à 15,2, s = 17,83 à 20,6) qui raidit la chape dans le sens latéral.

Jeux du modèle (± 1 cm sur les cotes du kart) :
- Bas de colonne ↔ moteur : 2,7 cm aux rondelles soudées des pattes (à tout angle) ; 2,8 cm au pire pour les bouts des pattes, le bloc et les rotules (à +85°). Biellettes ↔ moteur : 4,2 cm.
- Moyeu de la roue de la colonne ↔ platine : 6,5 mm (radial). Denture de la colonne ↔ platine : 9,7 mm (axial). Roue du réducteur ↔ voile avant : 7,3 mm. Écrous de la platine ↔ roue du réducteur : ≈ 3 mm avec rondelle de 1,6 mm et écrou M6 de 5 mm (vis M6 × 25 qui ne dépassent pas de l'écrou).
- **Roue de la colonne ↔ gaine blanche (collier) : 13 mm.** La roue tourne : fixer la gaine à l'écart.
- Joues ↔ jambes de l'arceau : 15 mm. Câbles moteur ↔ tubes bleus avant : 22 mm.

Efforts sur la chape à 4,9 A, moteur bloqué : Ft = 724 N (latéral, selon z) et Fr = 304 N (selon w) au point de contact, à 6,1 cm sous la ligne de fixation (s = 25,4). Moment latéral ≈ 724 × 0,061 = 44 N·m. C'est un effort hors du plan des joues : le voile avant et la platine doivent le reprendre. Platine alu de 8 mm sous le moment de 786 × 0,0227 = 17,8 N·m : contrainte ≈ 19 MPa (calcul simplifié : moment partagé entre les deux joues, 2 bandes de 23 et 20 mm de large autour de l'alésage Ø80), large marge pour un alu 6082.

### Composants (version Trinamic, solution de repli ; chiffres des fiches, vérifiés sauf mention)
- Moteur Trinamic QSH8618-96-55-700 : couple de maintien 7 N·m, 5,5 A par phase, 0,45 Ω, 4,5 mH, longueur 96 mm, 2,8 kg ; bride 86 × 86 mm (carré de 69,6 mm), centrage Ø73,02 ± 0,05 ; arbre Ø12 avec méplat ; charge radiale max 220 N à 20 mm.
- Réducteur OMC EG34-G5 : 5:1, 2800 g (3,01 kg brut), longueur 110 mm, bride 90 × 90 mm, sortie Ø20 × 40 avec clavette, entrée Ø14 ou Ø19 avec manchon libre de 14 mm, rendement 96 %, couple admissible 90 N·m, couple d'urgence 180 N·m. Prix affiché 85,04 $.
- Masse moteur + réducteur ≈ 2,8 + 2,9 = 5,7 kg (réducteur : 2,9 kg exact selon OMC) ; longueur cumulée 96 + 110 = 206 mm (hors arbre de sortie de 40 mm).
- Roues chevrons : module transversal 2 mm, 46 dents, entraxe 92 mm, hélice 30°, angle de pression normal 20°, largeur 10 mm, jeu 0,10 mm. Ø primitif 92 mm, Ø de tête 95,46 mm, Ø de pied 87,67 mm, Ø de base 84,81 mm. Module normal 1,732 mm (non standard) ; variante standard : z = 40, module normal 2, β = 29,59°, même entraxe.
- Driver DSH-600 : réglage conseillé 5,4 A nominal (le moteur est à 5,5 A) ; SW4 demi-courant à l'arrêt ; la définition de « 6 A nominal / 7,2 A max » (efficace ou crête) est à confirmer auprès de Transmotec.

### Efforts (couple de maintien 7 N·m, linéaire avec le courant)
- Couple utile moteur en service ≈ 4,12 N·m (hypothèse 5/8,5 reprise de la version précédente, non vérifiée).
- Couple à la colonne : 17,6 à 18,8 N·m en service, 31,5 à 33,6 N·m moteur bloqué (rendement engrenage 0,95).
- Charge radiale sur la sortie du réducteur avec une roue de 46 dents, F = C / (r · cos(α)) : ≈ 458 N en service, ≈ 777 N à l'arrêt. Limite 1000 N à 20 mm de la bride (confirmée par OMC le 27.11.2026) → marges ×2,2 et ×1,3. Avec l'angle apparent du chevron (22,8°) au lieu de 20°, ces valeurs augmentent de 1,9 % (cos(20°) / cos(22,8°) = 1,019).
- Couple dans le réducteur à l'arrêt : 33,6 N·m pour 90 N·m admissibles (marge ≈ 2,7).
- Contrainte de flexion de Lewis (Y ≈ 0,43) : ≈ 97 MPa à l'arrêt pour 10 mm de largeur, ≈ 49 MPa pour 20 mm. Roues en acier ou en alu usiné (ou impression métal) ; PLA/PETG exclus.
- Variante 10:1 (EG34-G10 ou EG34-G10-D13, mêmes cotes de montage 110 mm / 90 × 90) : couple admissible 39 N·m. À l'arrêt le moteur donnerait ≈ 67 N·m → limiter le courant à ≈ 3,2 A. En service ≈ 39,6 N·m : à la limite. À ne garder que si l'effort mesuré au volant dépasse 50 N. Les roues 46/46 resteraient valables.

### Ce qui n'est pas connu
- Le couple réel à fournir à la colonne n'est pas mesuré (pas de dynamomètre disponible). Méthodes sans appareil : seau et corde sur le volant (C = m · g · R ; ≈ 9,5 kg pour 13 N·m à R = 0,14 m), ou le moteur lui-même (réglage du driver à 2 A ≈ 11,6 N·m à la colonne, si les définitions de courant sont équivalentes), ou le dynamomètre de l'école.
- Jeu du réducteur et charge axiale : désormais connus (10 arcmin et 800 N, voir le 2e échange avec OMC).
- Couple transmissible par la pince d'entrée avec la rainure de clavette vide : demandé à OMC.
- Charge radiale admissible à 22,7 mm : non donnée par OMC (estimation prudente 881 N ci-dessus).
- Raison du prix de la livraison FedEx (113,28 € pour 8,41 kg) : demandée à OMC.

### Fichiers produits
- `chevrons_z46_a92_h10_roueA.step`, `_roueB.step`, `_paire.step` (CadQuery, validés : solides valides, hauteur 10,000 mm, 0 mm³ d'interférence de 0° à 7°, 3,07 mm³ si l'entraxe baisse de 0,3 mm).
- `HerringboneGears.py` (script Fusion, deux corps dans le composant racine pour un document « pièce »). S'exécute sans erreur dans Fusion (confirmé par Nathanaël le 27.11.2026). Depuis le 27.11.2026 il demande le diamètre primitif (40 à 200 mm) puis le nombre de dents (12 à 120), propose le nombre de dents qui garde le module normal de 1,732 mm, et affiche la charge radiale à l'arrêt et la contrainte de flexion. Test avec un faux module `adsk` sur 60, 70, 80, 92, 120 et 130,5 mm : contours fermés, pas d'interférence. À 92 mm : 777 N à l'arrêt, 458 N en service, 98 MPa. Si le sens des chevrons est inversé, passer `FLIP_HAND` à `True`.
- Chemin local de Nathanaël : `N:/A_EPTM/Professionel/EPTM_Troisieme_annee/Inf-Z_PROJET/Conceptions 3D/roues crantées direction/HerringboneGears/HerringboneGears.py`.
- Simulation 3D v1 (62/31, moteur 86 × 118) : https://claude.ai/artifact/4wV7RGULmdfFm5GwrqLgm6 — inchangée, c'est celle du cahier des charges.
- **Simulation 3D v2** (révision 10) : https://claude.ai/artifact/LnsyPs3qEng5kk36GazUyw ; copie autonome dans `doc/simulations/moteur direction kart/Direction du kart 3D v2.html` (three.js r128 chargé depuis cdnjs : connexion internet nécessaire). Maillages issus des fichiers CAO, écart de forme ≤ 0,2 mm. Jeux affichés et recalculés dans la page.

### Points à vérifier
- Dernier mail à OMC (voir ci-dessus) ; leur demander aussi la charge radiale admissible à 22,7 mm de la bride. Retirer la clavette de l'arbre moteur avant montage (demande d'OMC).
- Roue du 3MF : rainure de clavette de 6 mm, longueur du moyeu côté réducteur, roue de la colonne en miroir, fixation sur le tube chromé.
- Le moteur ne porte que le réducteur (2,9 kg), aucune charge de roue. Masse moteur + réducteur 6,5 kg en porte-à-faux sous la platine : moment statique ≈ 45 N × 0,12 m ≈ 5,4 N·m (composante perpendiculaire à l'axe), à multiplier par les chocs de roulage ; un appui arrière du moteur est à étudier.
- Au kart : matière de la pièce jaune ; diamètre de la grosse vis et du trou libre ; jeu radial de la colonne dans la pièce jaune ; trous des pattes hautes et place autour ; passage de la gaine blanche (13 mm de la roue dans le modèle) ; place du moteur entre les jambes d'un pilote assis ; dureté du volant en manuel moteur coupé (il entraîne le moteur à travers le 5:1).
