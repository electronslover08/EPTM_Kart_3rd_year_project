# xxx_modele_pcb – modèle de module (RS485)

Base d'un nouveau module esclave : PIC18F45K22, quartz, port de programmation, RS485 entrée/sortie (RJ12), alimentation locale ou par le bus (JP1), 4 entretoises M3, logo EPTM.

## Utilisation
- Copier-coller : copier le dossier `xxx_modele_pcb` entier, le renommer, renommer aussi le sous-dossier `Projet KICAD 10.0\xxx_modele_pcb`, ouvrir le .kicad_pro puis « Enregistrer sous » pour renommer les fichiers.
- Modèle KiCad : Préférences > Configurer les chemins > KICAD_USER_TEMPLATE_DIR = `…\xxx_modele_pcb\Projet KICAD 10.0` ; le modèle apparaît dans « Nouveau projet à partir d'un modèle ».
- Dans le PCB : F8 (Mettre à jour le PCB depuis le schéma) pour importer les empreintes.
