#include "Dxf.hpp"
#include <cstring>

#include <iostream>

DXF::Grupo DXF::Dxf::getGrupo(int16_t gr)
{
    if(gr < 5) return Grupo::STRING;    //  string
    if(gr == 5) return Grupo::HANDLE;   //  handle
    if(gr < 10) return Grupo::STRING;
    if(gr < 60) return Grupo::DOUBLE;   //  double
    if(gr < 80) return Grupo::INT16;   //  i16
    if(gr < 90) return Grupo::GRUPO;   //  grupo
    if(gr < 100) return Grupo::INT32;  //  i32
    if(gr == 100) return Grupo::STRING;
    if(gr == 101) return Grupo::GRUPO;
    if(gr == 102) return Grupo::STRING;
    if(gr < 105) return Grupo::GRUPO;
    if(gr == 105) return Grupo::HANDLE;
    if(gr < 110) return Grupo::GRUPO;
    if(gr < 150) return Grupo::DOUBLE;
    if(gr < 160) return Grupo::GRUPO;
    if(gr < 170) return Grupo::INT64;  //  i64
    if(gr < 210) return Grupo::GRUPO;
    if(gr < 240) return Grupo::DOUBLE;
    if(gr < 270) return Grupo::GRUPO;
    if(gr < 290) return Grupo::INT16;
    if(gr < 300) return Grupo::BOOL;  //  bool
    if(gr < 310) return Grupo::STRING;
    if(gr < 320) return Grupo::BINARY;  //  bin
    if(gr < 370) return Grupo::HANDLE;
    if(gr < 390) return Grupo::INT16;
    if(gr < 430) return Grupo::GRUPO;
    if(gr < 440) return Grupo::STRING;
    if(gr < 460) return Grupo::INT32;
    if(gr < 470) return Grupo::DOUBLE;
    if(gr < 480) return Grupo::STRING;
    if(gr < 1000) return Grupo::GRUPO;
    if(gr < 1004) return Grupo::STRING;
    if(gr == 1004) return Grupo::BINARY;
    if(gr < 1010) return Grupo::GRUPO;
    if(gr < 1060) return Grupo::DOUBLE;
    if(gr < 1072) return Grupo::INT64;
    return Grupo::GRUPO;
}

DXF::Grupo DXF::Dxf::getGrupo(char *str)
{
    if(!str) return Grupo::GRUPO;
    char tx[128] = {};
    size_t j = 0;
    for(size_t i = 0; i < strlen(str); i++)
    {
        if(j >= 127) break;
        if((str[i] >= '0' && str[i] <= '9') || (j == 0 && str[i] == '-'))
            tx[j++] = str[i];
    }
    if(j == 0) return Grupo::GRUPO;
    tx[j] = 0;
    if(tx[0] == '-') return Grupo::GRUPO;
    return getGrupo((int16_t)std::stoll(tx));
}

DXF::Grupo DXF::Dxf::getGrupo(std::vector<char> &v)
{
    char tx[128];
    for(size_t i = 0; i < v.size(); i++) tx[i] = v[i];
    return getGrupo(tx);
}

std::vector<size_t> DXF::Dxf::QSOrdenaStr(std::vector<char> vStr, std::vector<size_t> vInd)
{
    std::vector<size_t> indices = {};
    if(vInd.size() < 2) return indices;
    indices.reserve(vInd.size());
    for(size_t i = 0; i < vInd.size(); i++) indices.push_back(i);
    std::stack<Dados::ParI> w;
    w.push({0, indices.size()});
    while(!w.empty())
    {
        Dados::ParI atual = w.top();
        w.pop();
        size_t p = atual.Hard, q;
        for(size_t i = atual.Hard; i < (atual.Tail - 1); i++)
        {
            size_t posi, posf = vInd[indices[i]];
            if(vInd[indices[i]] == 0) posi = 0;
            else posi = vInd[(indices[i] - 1)];
            char a[256];    //  particionar esse pedaço
            size_t k = 0;
            for(size_t j = posi; j < posf; j++, k++) a[k] = vStr[j];
            a[k] = 0;
            posf = vInd[indices[(atual.Tail - 1)]];
            if(vInd[indices[(atual.Tail - 1)]] == 0) posi = 0;
            else posi = vInd[(indices[(atual.Tail - 1)] - 1)];
            char b[256];
            k = 0;
            for(size_t j = posi; j < posf; j++, k++) b[k] = vStr[j];
            b[k] = 0;
            if(strcmp(a, b) < 0) std::swap(indices[p++], indices[i]);
        }
        std::swap(indices[p], indices[(atual.Tail - 1)]);
        q = p + 1;
        if(atual.Tail > (q + 1))
        {
            if(p > (atual.Hard + 1))
            {
                if((atual.Tail - q) > (p - atual.Hard))
                {
                    w.push({q, atual.Tail});
                    w.push({atual.Hard, p});
                }
                else
                {
                    w.push({atual.Hard, p});
                    w.push({q, atual.Tail});
                }
            }
            else w.push({q, atual.Tail});
        }
        else if(p > (atual.Hard + 1)) w.push({atual.Hard, p});
    }
    std::vector<size_t> ir = {};
    ir.reserve(indices.size());
    ir.push_back(indices[0]);
    for(size_t i = 1; i < indices.size(); i++)
    {
        size_t posi, posf = vInd[indices[i]];
        if(vInd[indices[i]] == 0) posi = 0;
        else posi = vInd[(indices[i] - 1)];
        char a[256];
        size_t k = 0;
        for(size_t j = posi; j < posf; j++, k++) a[k] = vStr[j];
        a[k] = 0;
        posf = vInd[ir[(ir.size() - 1)]];
        if(vInd[ir[(ir.size() - 1)]] == 0) posi = 0;
        else posi = vInd[(ir[(ir.size() - 1)] - 1)];
        char b[256];
        k = 0;
        for(size_t j = posi; j < posf; j++, k++) b[k] = vStr[j];
        b[k] = 0;
        if(strcmp(a, b) != 0) ir.push_back(indices[i]);
    }
    ir.shrink_to_fit();
    return ir;
}

bool DXF::Dxf::LerArquivo(const std::filesystem::path &fonte)
{
    //--Ler o arquivo--
    ARQ::Arquivo arq;
    if(!arq.SetArquivo(fonte)) return false;
    //======================================

    //--Obter a quantidade de linhas por tipo--
    size_t numLinhas = arq.GetLnTotal();
    char str[256];
    bool isGrupo = false;
    std::vector<size_t> quantidades = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    Grupo grupo;
    for(size_t i = 0; i < numLinhas; i++)
    {
        arq.GetLinha(i, str);
        isGrupo = !isGrupo;
        if(strlen(str) == 1 && str[0] < 32) isGrupo = !isGrupo;
        else
        {
            if(isGrupo)
            {
                grupo = getGrupo(str);
                switch (grupo) {
                case Grupo::BINARY:
                    quantidades[1]++;
                    break;
                case Grupo::BOOL:
                    quantidades[0]++;
                    break;
                case Grupo::DOUBLE:
                    quantidades[5]++;
                    break;
                case Grupo::HANDLE:
                    quantidades[7]++;
                    break;
                case Grupo::INT16:
                    quantidades[2]++;
                    break;
                case Grupo::INT32:
                    quantidades[3]++;
                    break;
                case Grupo::INT64:
                    quantidades[4]++;
                    break;
                case Grupo::STRING:
                    quantidades[6]++;
                    break;
                default:
                    break;
                }
            }
            else
            {
                if(grupo == Grupo::HANDLE) quantidades[9] += (size_t)strlen(str);
                else if(grupo == Grupo::STRING) quantidades[8] += (size_t)strlen(str);
            }
        }
    }
    //for(size_t i = 0; i < quantidades.size(); i++) std::cout << quantidades[i] << std::endl;
    //======================================================================================    //  ..ok..

    //--Povoar buffer's locais--
    std::vector<bool> vBool;
    std::vector<uint8_t> vBufBin;
    std::vector<int16_t> vBufI16;
    std::vector<int32_t> vBufI32;
    std::vector<int64_t> vBufI64;
    std::vector<double> vBufDbl;
    std::vector<char> vBufStr;
    std::vector<char> vBufHnd;
    std::vector<size_t> vIndStr;
    std::vector<size_t> vIndHnd;

    vBool.reserve(quantidades[0]);
    vBufBin.reserve(quantidades[1]);
    vBufI16.reserve(quantidades[2]);
    vBufI32.reserve(quantidades[3]);
    vBufI64.reserve(quantidades[4]);
    vBufDbl.reserve(quantidades[5]);
    vBufStr.reserve(quantidades[8]);
    vBufHnd.reserve(quantidades[9]);
    vIndStr.reserve(quantidades[6]);
    vIndHnd.reserve(quantidades[7]);

    size_t iStr = 0, iHnd = 0;

    isGrupo = false;
    grupo = Grupo::GRUPO;
    for(size_t i = 0; i < numLinhas; i++)
    {
        arq.GetLinha(i, str);
        isGrupo = !isGrupo;
        if(strlen(str) == 1 && str[0] < 32) isGrupo = !isGrupo;
        else
        {
            if(isGrupo) grupo = getGrupo(str);
            else
            {
                switch (grupo) {
                case Grupo::BINARY:
                    vBufBin.push_back((uint8_t)std::stoi(str));
                    break;
                case Grupo::BOOL:
                    vBool.push_back((bool)std::stoi(str));
                    break;
                case Grupo::DOUBLE:
                    vBufDbl.push_back((double)std::stod(str));
                    break;
                case Grupo::HANDLE:
                    iHnd += (size_t)strlen(str);
                    vIndHnd.push_back(iHnd);
                    for(size_t j = 0; j < (size_t)strlen(str); j++) vBufHnd.push_back(str[j]);
                    break;
                case Grupo::INT16:
                    vBufI16.push_back((uint16_t)std::stoull(str));
                    break;
                case Grupo::INT32:
                    vBufI32.push_back((uint32_t)std::stoull(str));
                    break;
                case Grupo::INT64:
                    vBufI64.push_back((uint64_t)std::stoull(str));
                    break;
                case Grupo::STRING:
                    iStr += (size_t)strlen(str);
                    vIndStr.push_back(iStr);
                    for(size_t j = 0; j < (size_t)strlen(str); j++) vBufStr.push_back(str[j]);
                    break;
                default:
                    break;
                }
            }
        }
    }
    //==========================================================================================    ..ok..

    //--Ordenar e remover repetições--
    if(vBufBin.size())
    {
        Dados::OrdenaVectorQS(vBufBin, false);
        vBufBin.shrink_to_fit();
    }
    if(vBufI16.size())
    {
        Dados::OrdenaVectorQS(vBufI16, false);
        vBufI16.shrink_to_fit();
    }
    if(vBufI32.size())
    {
        Dados::OrdenaVectorQS(vBufI32, false);
        vBufI32.shrink_to_fit();
    }
    if(vBufI64.size())
    {
        Dados::OrdenaVectorQS(vBufI64, false);
        vBufI64.shrink_to_fit();
    }
    if(vBufDbl.size())
    {
        Dados::OrdenaVectorQS(vBufDbl, false);
        vBufDbl.shrink_to_fit();
    }
    //  **TRABALHAR AQUI OS STR E HND**
    std::cout << "vBufStr.size(): " << vBufStr.size() << "\tvIndStr.size(): " << vIndStr.size() << std::endl;
    for(size_t i = 0; i < 128; i++) std::cout << vBufStr[i];
    std::cout << '\n';
    for(size_t i = 0; i < 16; i++) std::cout << ' ' << vIndStr[i];
    std::cout << '\n';

    std::vector<size_t> v = QSOrdenaStr(vBufStr, vIndStr);
    //for(size_t i = 0; i < v.size(); i++) std::cout << v[i] << std::endl;
    for(size_t i = 0; i < v.size(); i++)
    {
        size_t posi, posf = vIndStr[v[i]];
        if(v[i] == 0) posi = 0;
        else posi = vIndStr[(v[i] - 1)];
        for(size_t j = posi; j < posf; j++) std::cout << vBufStr[j];
        std::cout << '\n';
    }
    //===================================================

    //--Fazer dicionários--
    //===================================================
    return true;
}