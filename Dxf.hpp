//  C:\Tony\DXF\Dxf.hpp
#pragma once
#include "Parse.hpp"

namespace DXF
{

enum class Grupo: std::uint8_t {GRUPO = 0, DOUBLE, INT16, INT32, INT64, STRING, HANDLE, BOOL, BINARY};

struct EDxf {Grupo grupo = Grupo::GRUPO; size_t indice = std::numeric_limits<size_t>::max();};

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
    //bool LerArquivo(const std::filesystem::path &fonte);
public:
    Dxf() = default;
    bool LerArquivo(const std::filesystem::path &fonte);
};
} // namespace DXF