Equipe : 
Gerry Quentin (GERQ20059306)

Step :
Lancer premake.bat pour generer la solution vs15
Dans le dossier Project/VisualStudio2015 ouvrir le fichier projet EtherealDream.sln

L'aspect graphique du TP a été fait par mon propre engine EtherealDream. 
Le code qui vous interesse se trouve uniquement dans le dossier Classes/IA_WW.

Issue :
Si jamais vous rencontrer l'erreur C1083, il suffit de modifier les proprietes de stdafx.cpp, 
onglet precompiled Headers puis mettre Precompiled Header à Create (/YC)