//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <vector>
#include <filesystem>
#include <fstream>

namespace DXF {

enum TpLn: std::uint8_t {NULO, GRUPO, STR, DBL, NN16, NN32, NN64};

struct Linha
{
    size_t Posi = std::numeric_limits<size_t>::max();
    size_t Posf = std::numeric_limits<size_t>::max();
    TpLn Tipo = NULO;
    void getString(std::ifstream* file, char* tx);
    bool igual_(std::ifstream *file, Linha &outra);
    bool maior_(std::ifstream *file, Linha &outra);
    Linha& operator =(const Linha outra);
};

struct Trecho {size_t head; size_t tail;};

class Dxf
{
private:
    size_t QuantLinhas(std::ifstream* file);
    std::vector<Linha> LerLinhas(std::ifstream* file, size_t n);
    TpLn getTipo(std::ifstream* file, size_t posi, size_t posf);
    size_t PivotaStr(std::ifstream* file, std::vector<Linha> &lns, std::vector<size_t> &lst, Trecho tre);
    void OrdenaStr(std::ifstream* file, std::vector<Linha> &lns, std::vector<size_t> &lst);
    std::vector<size_t> OrdenaStrExclusivo(std::ifstream* file, std::vector<Linha> &lns, std::vector<size_t> &lst);
    void Ler(const std::filesystem::path &nome);
public:
    Dxf() = default;

    void Valida();
};
} // namespace DXF