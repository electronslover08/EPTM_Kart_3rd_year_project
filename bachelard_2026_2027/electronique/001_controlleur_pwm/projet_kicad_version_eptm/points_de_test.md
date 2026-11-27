# Points de test – 001_Contrôleur_PWM V1 (version EPTM)

Cotes en mm, repère KiCad (vue de dessus). Tous les CMS sont au BOTTOM.

## 1. Boucles de test à ajouter (place vérifiée)

Empreinte : pastille ronde Ø2,2 mm, trou 1,0 mm. Une boucle Keystone 5000‑5004 ou un fil plié, soudé côté BOTTOM.

Chaque position a été vérifiée contre :
- les pastilles existantes : isolation 20 mil, avec une marge de 0,25 mm ;
- les corps des composants côté TOP ;
- les 6 zones de franchissement prévues ;
- les couloirs de l'étude de routage.

| TP  | Signal               | Position (x ; y) | Raccord court vers | Mesure attendue                               |
|-----|----------------------|------------------|--------------------|-----------------------------------------------|
| TP1 | GND                  | 182,75 ; 108,00  | R4 pastille 1      | masse de la sonde (zone RS‑485)               |
| TP2 | GND                  | 131,50 ; 91,25   | R13 pastille 2     | masse de la sonde (zone entrées / PIC)        |
| TP3 | VDD (+5 V)           | 122,50 ; 83,75   | J2 broche 2        | 5,0 V ± 5 %, ondulation < 50 mVcc             |
| TP4 | RS485_A              | 177,50 ; 108,00  | R3 pastille 1      | A − B > +200 mV au repos (polarisation 390 Ω) |
| TP5 | RS485_B              | 185,25 ; 103,75  | R4 pastille 2      | trames différentielles pendant l'émission     |
| TP6 | AUX2 (côté PIC)      | 149,75 ; 83,50   | R19 pastille 2     | impulsion 1–2 ms, période ≈ 20 ms, 0/5 V      |
| TP7 | STR entrée récepteur | 118,00 ; 89,75   | J3 broche 1        | impulsion 1–2 ms, niveau du récepteur (~3,3 V) |

Les deux points GND peuvent aussi être soudés côté TOP. Ils relient alors le plan de masse TOP et remplacent des vias GND.

## 2. Signaux sans boucle : mesurer directement sur les pastilles existantes

Il n'y a pas de place pour une boucle au centre de la carte sans bloquer le routage (justification au §3). Toutes ces pastilles sont au BOTTOM.

| Signal | Pastille à palper                     | Mesure attendue                                |
|--------|---------------------------------------|------------------------------------------------|
| STR    | C13 pastille 1                        | 1–2 ms / 20 ms, 0/5 V (après tampon + RC)      |
| THR    | C12 pastille 1                        | idem                                           |
| AUX1   | C11 pastille 1                        | idem                                           |
| AUX3   | C9 pastille 1                         | idem                                           |
| AUX4   | C8 pastille 1                         | idem                                           |
| TX1    | U2 broche 4 (DI)                      | trames UART 0/5 V                              |
| RX1    | R7 pastille 2                         | 5 V au repos (tirage 10 k)                     |
| DIR    | R8 pastille 1                         | 0 V au repos, 5 V pendant l'émission           |
| /MCLR  | R1 pastille 2 (ou RESET : R2 pastille 1) | 5 V en fonctionnement                       |
| OSC1/2 | ne pas palper                         | une sonde de 10–15 pF fausse ou arrête le quartz 20 MHz |

## 3. Pourquoi seulement 7 boucles

L'étude de routage du placement actuel donnait déjà 35 vias et 4 nets en conflit, sans aucune boucle ajoutée.

En prenant ses couloirs comme obstacles, aucune position n'existe pour STR, THR, AUX1, AUX3, AUX4, TX1, RX1, DIR et /MCLR. Le test a été refait dans deux autres configurations, sans meilleur résultat :
- avec une pastille réduite à Ø2,0 mm ;
- avec un « via de test » de Ø1,27 mm côté bottom.

Chaque boucle au centre coûterait donc des vias supplémentaires. Pour en avoir à ces endroits, il faudrait d'abord déplacer quelques passifs.

## 4. Points d'alimentation déjà accessibles côté TOP

| Point        | Où mesurer                     |
|--------------|--------------------------------|
| VIN          | clip gauche de F1 ou de F2     |
| VDD          | clip droit de F1               |
| VBUS         | clip haut de F2                |
| GND          | borne 1 de J10                 |

Pour mesurer le courant : retirer F1 et placer l'ampèremètre entre ses deux clips.
