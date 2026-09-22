//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <vector>
#include <filesystem>
//#include <fstream> .. para a alteração pretendida: construtor lê o arquivo.

namespace DXF {
enum CodGrupo{STRING, DOUBLE, I16, I32, I64};

struct Fresta{size_t posi = std::numeric_limits<size_t>::max(), posf = std::numeric_limits<size_t>::max();};

class viewChar
{
private:
    std::vector<char>::iterator itx{};
    size_t posi = std::numeric_limits<size_t>::max(), posf = std::numeric_limits<size_t>::max();
public:
    viewChar() = default;

    void setFonte(std::vector<char> &c){itx = c.begin();}
    void setOculo(size_t i, size_t j){posi = i; posf = j;}
    void getTexto(char * tx);
};

class Dxf
{
private:
    //std::ifstream mArquivo(); .. O construtor deve abrir o arquivo uma vez só e fazer todas as operações
    std::vector<Fresta> GetBufBruto(const std::filesystem::path &nome);
    size_t NumDeLinhas(const std::filesystem::path &nome);
    std::uint32_t NumPorTipo(const std::filesystem::path &nome, std::uint32_t &str, std::uint32_t &dbl,
                             std::uint32_t &i2, std::uint32_t &i4, std::uint32_t &i8, std::uint32_t &chr);
    void Ler(const std::filesystem::path &nome);
    void AbreUmaVez(const std::filesystem::path &nome);
public:
    Dxf() = default;

    void Valida();
};
} // namespace DXF