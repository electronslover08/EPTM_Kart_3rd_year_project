# 002 – Contrôleur MOTEUR : idées et décisions

## Rôle dans la chaîne RS485
- 001 = maître : une seule sortie RS485 (J9). Tous les autres modules (002, …) ont une entrée ET une sortie RJ12 pour la guirlande.
- Brochage RJ12 : 1 et 6 = +5 V (BUS_5V), 2 et 5 = GND, 3 = A, 4 = B.
- Polarisation (390 Ω / 390 Ω) uniquement sur le maître. Terminaison 120 Ω (R5) uniquement sur le dernier module de la chaîne.

## Alimentation
- BUS_5V (traverse le module de J9 à J11) et +5V (rail local) sont deux nets séparés.
- Cavalier JP1 : 1-2 = alim locale (J10 + fusible F1) ; 2-3 = alim par le bus RS485 ; ouvert = module non alimenté.
- Une seule source à la fois (pas de diode OR : la chute d'une Schottky dépasse la marge de 0,25 V du MAX485, Vcc 4,75–5,25 V).
- Piste BUS_5V ≥ 1 mm.

## À mesurer / décider
- Longueur et section des câbles RJ12.
- Courant que peut fournir le 5 V du kart.
- Consommation réelle du PIC à 20 MHz.



pour la marche arrière faudrait gérer le bouton physique directement via un 4e relais.