#include <iostream>
#include <cassert>
#include "elf_parser.h"

// Test unitaire simple pour vérifier la lecture du header ELF
void test_elf_header() {
    try {
        ElfParser parser("/bin/ls");  // Utilise un fichier système valide
        parser.parse();
        
        const auto& hdr = parser.get_header();
        assert(hdr.is_valid);
        std::cout << "Test de lecture du header ELF : OK\n";
    } catch (const std::exception&) {
        std::cerr << "Erreur lors du test du header ELF.\n";
        throw;
    }
}

int main() {
    try {
        test_elf_header();
        std::cout << "Tous les tests passés avec succès !\n";
    } catch (const std::exception& e) {
        std::cerr << "Erreur dans le test : " << e.what() << std::endl;
        return 1;
    }

    return 0;
}