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

Deux dossiers spéciaux, préfixés par `_`, s'affichent en tête du dépôt :

| Dossier | Rôle |
|---|---|
| `_modele_nom_AAAA_AAAA/` | Dossier **modèle vide** : la structure à copier chaque année. On n'y travaille jamais directement. |
| `_documentation_avant_2026/` | Documentation des projets réalisés **avant la création du dépôt** (rapports, schémas, photos…). Lis son README avant d'y ajouter un document : les droits d'auteur doivent être vérifiés. |

## Reprendre le projet (élève responsable)

Au début de ton année, crée ton dossier à partir du **dossier modèle**, puis reprends-y les fichiers de l'année précédente que tu continues. Ainsi, tu pars d'une structure propre et de la dernière version des fichiers techniques.

1. **Nomme ton dossier** `nom_AAAA_AAAA` : ton nom de famille en minuscules, sans accents ni espaces, puis l'année scolaire. Par exemple, `dupont_2027_2028`.
2. **Copie le dossier modèle** sous ce nouveau nom, dans une nouvelle branche :
   ```bash
   git checkout -b reprise-dupont-2027-2028
   cp -r _modele_nom_AAAA_AAAA dupont_2027_2028
   ```
3. **Reprends les fichiers techniques** du dossier le plus récent que tu vas continuer (KiCad, Fusion, code), par exemple :
   ```bash
   cp -r bachelard_2026_2027/electronique/. dupont_2027_2028/electronique/
   ```
   Ne recopie pas la documentation de l'année précédente (cahier des charges, rapports) : elle reste dans son dossier, qui sert d'archive.
4. **Remplis le `README.md` de ton dossier** (tous les `…`) : élève responsable, année scolaire, professeur, sujet, dossier repris et lien vers ton cahier des charges. Supprime l'encadré « dossier modèle ».
5. **Ajoute ta ligne** au tableau « Projets par année » du fichier `.github/README.md`.
6. **Ouvre une pull request** avec ces changements.

Si tu améliores la structure commune (un nouveau sous-dossier utile chaque année, par exemple), mets aussi à jour le dossier modèle dans la même pull request.

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

👉 **Remplis le [formulaire de demande d'accès](https://forms.cloud.microsoft/r/6yDiL06M1y).**

Tu devras y indiquer :
- ton prénom et ton nom ;
- ton adresse e-mail scolaire (@edu.vs.ch) ;
- le prénom et le nom du professeur responsable de ton projet ;
- ton nom d'utilisateur GitHub.

Ces informations ne sont pas publiées : seul le responsable du dépôt les voit. Elles servent uniquement à vérifier que tu es bien élève de l'école. **Si cette vérification n'est pas possible, l'accès sera refusé.** Si elle aboutit, tu recevras une invitation GitHub par e-mail pour rejoindre le dépôt.

N'ouvre pas d'issue pour demander un accès : les issues sont publiques.

## Licences

En contribuant, tu acceptes que ta contribution soit publiée sous les mêmes licences que le reste du projet (voir [LICENSE.md](LICENSE.md)) : CERN-OHL-P-2.0 pour le matériel, MIT pour le code, CC BY 4.0 pour la documentation.
