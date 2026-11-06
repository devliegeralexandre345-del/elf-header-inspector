# ELF Header Inspector

Outil CLI en C++20 pour inspecter les en-têtes ELF 64-bit.

## Fonctionnalités
- Affichage des sections du fichier ELF
- Identification de l'architecture cible
- Visualisation du point d'entrée (entry point)

## Compilation et Exécution

```bash
cmake -B build && cmake --build build
./build/elf_header_inspector /path/to/your/file.elf
```

## Structure du projet

- `src/main.cpp` : Point d'entrée principal de l'application.
- `src/elf_parser.cpp`, `include/elf_parser.h` : Logique pour lire et analyser les en-têtes ELF.
- `tests/test.cpp` : Tests unitaires basiques.

## Dépendances

Seule la STL C++20 est utilisée. Aucune bibliothèque externe requise.