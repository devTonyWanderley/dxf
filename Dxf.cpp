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

bool DXF::Dxf::LerArquivo(const std::filesystem::path &fonte)
{
    //--Ler o arquivo--
    ARQ::Arquivo arq;
    if(!arq.SetArquivo(fonte)) return false;
    //======================================

    /*--Testar a identificação de linha como grupo--
    size_t numLinhas = arq.GetLnTotal();
    std::cout << "numero de linhas: " << numLinhas << std::endl;
    char str[256];
    bool isGrupo = false;
    for(size_t i = 0; i < numLinhas; i++)
    {
        arq.GetLinha(i, str);
        isGrupo = !isGrupo;
        if(strlen(str) == 1 && str[0] < 32) isGrupo = !isGrupo;
        else
        {
            if(isGrupo)
            {
                Grupo grupo = getGrupo(str);
                char gStr[16];
                switch (grupo) {
                case Grupo::BINARY:
                    strcpy(gStr, "Binario");
                    break;
                case Grupo::BOOL:
                    strcpy(gStr, "Booleano");
                    break;
                case Grupo::DOUBLE:
                    strcpy(gStr, "Double");
                    break;
                case Grupo::HANDLE:
                    strcpy(gStr, "Handle");
                    break;
                case Grupo::INT16:
                    strcpy(gStr, "I16");
                    break;
                case Grupo::INT32:
                    strcpy(gStr, "I32");
                    break;
                case Grupo::INT64:
                    strcpy(gStr, "I64");
                    break;
                case Grupo::STRING:
                    strcpy(gStr, "String");
                    break;
                default:
                    strcpy(gStr, "Indefinido");
                    break;
                }
                std::cout << i << '\t' << gStr << "\t\'" << str << "\'\t";
            }
            else std::cout << '\'' << str << '\'' << std::endl;
        }
    }
    //====================================================================  //  ..ok..
*/

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
    for(size_t i = 0; i < quantidades.size(); i++) std::cout << quantidades[i] << std::endl;
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
            if(isGrupo)grupo = getGrupo(str);
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
    std::cout << "Booleano:" << std::endl;
    for(size_t i = 0; i < vBool.size(); i++) std::cout << vBool[i] << std::endl;
    std::cout << "Binario:" << std::endl;
    for(size_t i = 0; i < vBufBin.size(); i++) std::cout << vBufBin[i] << std::endl;
    std::cout << "I16:" << std::endl;
    for(size_t i = 0; i < vBufI16.size(); i++) std::cout << vBufI16[i] << std::endl;
    std::cout << "I32:" << std::endl;
    for(size_t i = 0; i < vBufI32.size(); i++) std::cout << vBufI32[i] << std::endl;
    std::cout << "I64:" << std::endl;
    for(size_t i = 0; i < vBufI64.size(); i++) std::cout << vBufI64[i] << std::endl;
    std::cout << "Double:" << std::endl;
    for(size_t i = 0; i < vBufDbl.size(); i++) std::cout << vBufDbl[i] << std::endl;
    std::cout << "String:" << std::endl;
    for(size_t i = 0; i < vBufStr.size(); i++) std::cout << vBufStr[i];
    std::cout << "\nHandle:" << std::endl;
    for(size_t i = 0; i < vBufHnd.size(); i++) std::cout << vBufHnd[i];
    std::cout << "iString:" << std::endl;
    for(size_t i = 0; i < vIndStr.size(); i++) std::cout << vIndStr[i] << std::endl;
    std::cout << "iHandle:" << std::endl;
    for(size_t i = 0; i < vIndHnd.size(); i++) std::cout << vIndHnd[i] << std::endl;
    //==========================================================================================    ..ok..

    //--Ordenar, remover repetições e fazer dicionários--
    //===================================================
    return true;
}