#include <iostream>
#include "elf_parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ELF_FILE>" << std::endl;
        return 1;
    }

    ElfParser parser(argv[1]);
    try {
        parser.parse();
        if (!parser.get_header().is_valid) {
            std::cerr << "Erreur: Fichier ELF invalide ou non supporté." << std::endl;
            return 1;
        }
        parser.print_info();
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'analyse du fichier : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}