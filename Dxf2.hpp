//  C:\Tony\DXF2\Dxf2.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <vector>
#include <filesystem>

namespace DXF {
enum CodGrupo{STRING, DOUBLE, I16, I32, I64};

struct Fresta{size_t posi = std::numeric_limits<size_t>::max(), posf = std::numeric_limits<size_t>::max();};

struct charView
{
    std::vector<char> &textos;
    Fresta fresta;
    bool operator ==(Fresta &outra)
    {
        if((fresta.posf - fresta.posi) != (outra.posf - outra.posi)) return false;
        if(fresta.posi == outra.posi) return true;
        for(size_t i = fresta.posi; i < fresta.posf; i++)
            if(textos.at(i) != textos.at(outra.posi + i - fresta.posi)) return false;
        return true;
    }
    bool operator >(Fresta &outra)
    {
        size_t lf = fresta.posf - fresta.posi, lo = outra.posf - outra.posi, l;
        l = (lf < lo)? lf : lo;
        for(size_t i = 0; i < l; i++)
        {
            if(textos.at(fresta.posi + i) > textos.at(outra.posi + i))return true;
            if(textos.at(fresta.posi + i) < textos.at(outra.posi + i))return false;
        }
        return (lf > lo);
    }
    bool operator <(Fresta &outra)
    {
        size_t lf = fresta.posf - fresta.posi, lo = outra.posf - outra.posi, l;
        l = (lf < lo)? lf : lo;
        for(size_t i = 0; i < l; i++)
        {
            if(textos.at(fresta.posi + i) < textos.at(outra.posi + i))return true;
            if(textos.at(fresta.posi + i) > textos.at(outra.posi + i))return false;
        }
        return (lf < lo);
    }
};

class Dxf
{
private:
    std::vector<Fresta> GetBufBruto(const std::filesystem::path &nome);
    size_t NumDeLinhas(const std::filesystem::path &nome);
    std::uint32_t NumPorTipo(const std::filesystem::path &nome, std::uint32_t &str, std::uint32_t &dbl,
                             std::uint32_t &i2, std::uint32_t &i4, std::uint32_t &i8, std::uint32_t &chr);
    void Ler(const std::filesystem::path &nome);
public:
    Dxf() = default;

    void Valida();
};
} // namespace DXF