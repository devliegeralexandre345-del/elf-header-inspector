#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "elf_parser.h"

ElfParser::ElfParser(const std::string& filename) : filename_(filename) {}

void ElfParser::parse() {
    if (!read_elf_header()) return;
    read_section_headers();
    determine_architecture_name();
}

bool ElfParser::read_elf_header() {
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filename_);
    }

    // Lecture du header ELF
    char magic[16];
    file.read(magic, 16);
    
    // Vérification de l'en-tête ELF (0x7F 'E', 'L', 'F')
    if (magic[0] != '\x7f' || magic[1] != 'E' || magic[2] != 'L' || magic[3] != 'F') {
        std::cerr << "Erreur: Ce n'est pas un fichier ELF valide." << std::endl;
        return false;
    }

    // Vérification de la taille du header (64-bit)
    if (magic[4] != 2) {  // EI_CLASS = 2 pour 64-bit
        std::cerr << "Erreur: Ce fichier n'est pas un ELF 64-bit." << std::endl;
        return false;
    }

    header_.is_valid = true;
    
    file.seekg(0x18); // Offset du point d'entrée dans le header
    file.read(reinterpret_cast<char*>(&header_.entry_point_address), sizeof(header_.entry_point_address));
    
    file.seekg(0x10);
    file.read(reinterpret_cast<char*>(&header_.machine), sizeof(header_.machine));
    
    return true;
}

bool ElfParser::read_section_headers() {
    std::ifstream file(filename_, std::ios::binary);
    if (!file.is_open()) return false;

    // Lecture du header ELF pour obtenir le nombre de sections et l'offset des headers
    char e_ident[16];
    file.read(e_ident, 16); 
    
    std::uint16_t shoff;   // Offset du premier section header
    std::uint16_t shentsize;
    std::uint16_t shnum;
    
    file.seekg(0x20);
    file.read(reinterpret_cast<char*>(&shoff), sizeof(shoff));
    file.read(reinterpret_cast<char*>(&shentsize), sizeof(shentsize));
    file.read(reinterpret_cast<char*>(&shnum), sizeof(shnum));

    // Lecture des noms de sections (nécessite un accès au string table)
    std::uint64_t shstrtab_offset = 0;
    
    if (shnum == 0) return false;   // Pas de section à lire
    
    file.seekg(shoff);
    for (std::size_t i = 0; i < shnum; ++i) {
        SectionHeader sec_header{};
        file.read(reinterpret_cast<char*>(&sec_header.name_offset), sizeof(sec_header.name_offset));
        file.read(reinterpret_cast<char*>(&sec_header.type), sizeof(sec_header.type));
        file.read(reinterpret_cast<char*>(&sec_header.flags), sizeof(sec_header.flags));
        file.read(reinterpret_cast<char*>(&sec_header.address), sizeof(sec_header.address));
        file.read(reinterpret_cast<char*>(&sec_header.offset), sizeof(sec_header.offset));
        file.read(reinterpret_cast<char*>(&sec_header.size), sizeof(sec_header.size));
        file.read(reinterpret_cast<char*>(&sec_header.link), sizeof(sec_header.link));
        file.read(reinterpret_cast<char*>(&sec_header.info), sizeof(sec_header.info));
        file.read(reinterpret_cast<char*>(&sec_header.addralign), sizeof(sec_header.addralign));
        file.read(reinterpret_cast<char*>(&sec_header.entsize), sizeof(sec_header.entsize));

        header_.sections.push_back(std::move(sec_header));
    }
    
    return true;
}

void ElfParser::determine_architecture_name() {
    switch (header_.machine) {
        case 0x03: header_.architecture_name = "Intel 80386"; break;
        case 0x14: header_.architecture_name = "PowerPC"; break;
        case 0x28: header_.architecture_name = "ARM"; break;
        case 0x32: header_.architecture_name = "IA-64 (Itanium)"; break;
        case 0x3E: header_.architecture_name = "AMD x86-64"; break;
        case 0xB7: header_.architecture_name = "AArch64 (ARMv8)"; break;
        default:   header_.architecture_name = "Inconnue"; break;
    }
}

void ElfParser::print_info() const {
    std::cout << "=== Informations sur le fichier ELF ===\n";
    std::cout << "Architecture : " << header_.architecture_name << " (Machine ID: 0x" 
              << std::hex << header_.machine << std::dec << ")\n";
    std::cout << "Point d'entrée : 0x" << std::hex << header_.entry_point_address << std::dec << "\n";
    std::cout << "Sections:\n";

    for (const auto& sec : header_.sections) {
        std::cout << "  - Offset: 0x" << std::hex << sec.offset 
                  << " | Taille: 0x" << sec.size 
                  << " | Type: 0x" << sec.type
                  << " | Adresse virtuelle: 0x" << sec.address << std::dec << "\n";
    }
}