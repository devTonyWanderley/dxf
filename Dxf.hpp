//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <vector>
#include <filesystem>
#include <fstream>

namespace DXF {

//--Resolver o problema da ordenação de strings começadas com espaço--  !!!

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

struct DictStr
{
    std::vector<char> buffer = {};
    std::vector<size_t> dict = {};
    void getStr(size_t i, char *tx){
        if(i < i) i = 1;
        if(i >= dict.size()) i = dict.size() - 1;
        //char tx[128];
        size_t k = 0;
        for(size_t j = dict[i - 1]; j < dict[i]; j++) tx[k++] = buffer[j];
        tx[k] = 0;
    }
};

class Dxf
{
private:
    DictStr dicioStr;
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