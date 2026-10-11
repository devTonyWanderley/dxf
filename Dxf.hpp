//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include "Parse.hpp"

namespace DXF
{

enum class Grupo: std::uint8_t {GRUPO = 0, DOUBLE = 1, INT16 = 2, INT32 = 3, INT64 = 4, STRING = 5, HANDLE = 6, BOOL = 7, BINARY = 8};

class Dxf
{
private:
    struct RegDxf{int16_t Cod = std::numeric_limits<int16_t>::max(); size_t Val = std::numeric_limits<size_t>::max(); bool flag = false;};

    struct Intervalo{size_t Hard, Tail;};

    //  Não ordenado:
    std::vector<bool> mBool = {};

    //  Ordenação de elementos individuais:
    std::vector<uint8_t> mBufBin = {};
    std::vector<int16_t> mBufI16 = {};
    std::vector<int32_t> mBufI32 = {};
    std::vector<int64_t> mBufI64 = {};
    std::vector<double> mBufDbl = {};

    //  Ordenação de conjuntos:
    std::vector<char> mBufStr = {};
    std::vector<char> mBufHnd = {};

    //  Conjunto de índices diretores:
    std::vector<size_t> mIndStr = {};
    std::vector<size_t> mIndHnd = {};

    //  O arquivo propriamente dito:
    std::vector<RegDxf> mLinhas = {};

    Grupo getGrupo(int16_t gr);
    Grupo getGrupo(char *str);
    Grupo getGrupo(std::vector<char> &v);
    void getTexto(size_t n, std::vector<char> &vStr, std::vector<size_t> &vInd, char *tx);
    void OrdenaBin(std::vector<uint8_t> &v);
    void OrdenaI16(std::vector<int16_t> &v);
    void OrdenaI32(std::vector<int32_t> &v);
    void OrdenaI64(std::vector<int64_t> &v);
    void OrdenaDbl(std::vector<double> &v);
    std::vector<size_t> QSOrdenaStr(std::vector<char> &vStr, std::vector<size_t> &vInd);
    size_t BbDbl(const double v);
    size_t BbI16(const int16_t v);
    size_t BbI32(const int32_t v);
    size_t BbI64(const int64_t v);
    size_t BbStr(const char* v);
    size_t BbHnd(const char* v);
    size_t BbBin(const uint8_t v);
    void LAPovoar(ARQ::Arquivo &fonte);
    bool LerArquivo(const std::filesystem::path &fonte);
public:
    Dxf() = default;

    void imprimeLinhaDxf(size_t indice);
    bool valida(const std::filesystem::path &fonte)
    {
        if(LerArquivo(fonte))
        {
            for(size_t i = 0; i < 500; i++) imprimeLinhaDxf(i);
            return true;
        }
        return false;
    }
};
} // namespace DXF