# Neper

Neper est une bibliothèque permettant l'analyse de pages web. Elle a été développée dans le cadre du projet Paxo pour permettre l'implémentation d'un navigateur internet minimaliste. 

L'ambition de Neper est, partant d'une page web quelconque, de réaliser les fonctions suivantes : 
  
- Analyse de l'HTML et construction d'un DOM Tree (`src/html`)
- Analyse des styles par défaut et du css de la page (`src/css` et `src/styles`)
- Construction, à partir du DOM Tree précedemment construit et stylisé, d'un arbre abstrait représentant la page et facilement convertible en une interface spécifique à un système (`src/abstract`)