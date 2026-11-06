#pragma once
#include <cstdint>
#include <string>
#include <vector>

class ElfParser {
public:
    struct SectionHeader {
        std::uint64_t name_offset;
        std::uint32_t type;
        std::uint64_t flags;
        std::uint64_t address;
        std::uint64_t offset;
        std::uint64_t size;
        std::uint32_t link;
        std::uint32_t info;
        std::uint64_t addralign;
        std::uint64_t entsize;
        std::string name;
    };

    struct ElfHeader {
        std::uint16_t machine;  // Architecture
        std::uint64_t entry_point_address;  // Point d'entrée
        std::vector<SectionHeader> sections;
        std::string architecture_name;
        bool is_valid = false;
    };

    explicit ElfParser(const std::string& filename);
    ~ElfParser() = default;

    const ElfHeader& get_header() const { return header_; }
    void parse();
    void print_info() const;

private:
    std::string filename_;
    ElfHeader header_;
    bool read_elf_header();
    bool read_section_headers();
    void determine_architecture_name();
};