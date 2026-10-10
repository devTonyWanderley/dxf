//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include "Parse.hpp"

namespace DXF
{

enum class Grupo: std::uint8_t {GRUPO = 0, DOUBLE = 1, INT16 = 2, INT32 = 3, INT64 = 4, STRING = 5, HANDLE = 6, BOOL = 7, BINARY = 8};

class Dxf
{
private:
    //  Não ordenado
    std::vector<bool> mBool = {};

    //  Ordenação de elementos individuais
    std::vector<uint8_t> mBufBin = {};
    std::vector<int16_t> mBufI16 = {};
    std::vector<int32_t> mBufI32 = {};
    std::vector<int64_t> mBufI64 = {};
    std::vector<double> mBufDbl = {};

    //  Ordenação de conjuntos
    std::vector<char> mBufStr = {};
    std::vector<char> mBufHnd = {};

    //  Conjunto de índices diretores
    std::vector<size_t> mIndStr = {};
    std::vector<size_t> mIndHnd = {};

    Grupo getGrupo(int16_t gr);
    Grupo getGrupo(char *str);
    Grupo getGrupo(std::vector<char> &v);
    void getTexto(size_t n, std::vector<char> &vStr, std::vector<size_t> &vInd, char *tx);
    std::vector<size_t> QSOrdenaStr(std::vector<char> &vStr, std::vector<size_t> &vInd);
    void LAPovoar(ARQ::Arquivo &fonte);
    bool LerArquivo(const std::filesystem::path &fonte);
public:
    Dxf() = default;
    bool valida(const std::filesystem::path &fonte){return LerArquivo(fonte);}


    //Grupo getGrupo(int16_t gr);
    //Grupo getGrupo(char *str);
    //Grupo getGrupo(std::vector<char> &v);
};
} // namespace DXF