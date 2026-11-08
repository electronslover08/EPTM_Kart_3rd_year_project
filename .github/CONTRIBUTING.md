# Contribuer au projet kart

Ce projet est repris chaque année par un nouvel élève. Toute contribution passe par une **pull request** (PR).

## Organisation du dépôt

Le dépôt contient **un dossier par année scolaire**, au nom de l'élève responsable du projet :

```
bachelard_2026_2027/
nom_2027_2028/
nom_2028_2029/
…
```

Chaque dossier d'année contient toujours les mêmes sous-dossiers :

| Dossier | Contenu |
|---|---|
| `assets/` | Images du projet |
| `documentation/` | Cahier des charges, rapports, notices |
| `electronique/` | Schémas et PCB (KiCad) |
| `logiciel/` | Code et firmware |
| `mecanique/` | Modèles 3D et pièces (Fusion) |

Le dossier `.github/` contient le fonctionnement du dépôt (README, ce guide, licences, modèles d'issues et de pull requests). Il est commun à toutes les années.

## Reprendre le projet (élève responsable)

Au début de ton année, crée ton dossier à partir du **dossier le plus récent**. Ainsi, tu pars de la dernière version de tous les fichiers.

1. **Nomme ton dossier** `nom_AAAA_AAAA` : ton nom de famille en minuscules, sans accents ni espaces, puis l'année scolaire. Par exemple, `dupont_2027_2028`.
2. **Copie le dossier le plus récent** sous ce nouveau nom, dans une nouvelle branche :
   ```bash
   git checkout -b reprise-dupont-2027-2028
   cp -r bachelard_2026_2027 dupont_2027_2028
   ```
3. **Mets à jour le `README.md` de ton dossier** : élève responsable, année scolaire, sujet et lien vers ton cahier des charges.
4. **Ajoute ta ligne** au tableau « Projets par année » du fichier `.github/README.md`.
5. **Ouvre une pull request** avec ces changements.

Pendant ton année, tu travailles **uniquement dans ton dossier**. Les dossiers des années précédentes sont des archives : on n'y touche que pour corriger une erreur, et en l'expliquant dans la PR.

## Proposer une modification (pull request)

1. **Fork** le dépôt : bouton « Fork » en haut à droite de la page GitHub.
2. Dans ton fork, **crée une branche** au nom explicite, par exemple `amelioration-monitoring`.
3. Fais tes modifications dans le **dossier de l'année en cours** et **commite-les** avec des messages clairs, qui disent ce qui change.
4. **Ouvre une pull request** vers la branche `main` de ce dépôt, en indiquant :
   - ce que tu as modifié ;
   - pourquoi tu l'as modifié ;
   - comment tu l'as testé, le cas échéant.
5. La PR sera relue, puis acceptée ou commentée.

Pour les fichiers matériels (schémas, KiCad, Fusion), la licence CERN-OHL-P demande de signaler chaque modification avec sa **date** et une **brève description**. La description de ta PR peut servir à ça.

## Demander un accès administrateur

L'accès administrateur au dépôt est réservé aux **élèves de l'école**.

1. Ouvre une **issue** avec le modèle « Demande d'accès administrateur ».
2. Ta demande doit permettre de **prouver que tu es bien élève de l'école**. La preuve attendue te sera précisée en réponse à ton issue.
3. **Si l'appartenance à l'école ne peut pas être prouvée, l'accès sera refusé.**

Les issues sont publiques : n'y publie aucun document personnel (pièce d'identité, carte d'élève, etc.).

## Licences

En contribuant, tu acceptes que ta contribution soit publiée sous les mêmes licences que le reste du projet (voir [LICENSE.md](LICENSE.md)) : CERN-OHL-P-2.0 pour le matériel, MIT pour le code, CC BY 4.0 pour la documentation.
