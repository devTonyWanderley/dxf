//  C:\Tony\DXF2\Dxf2.cpp
#include "Dxf2.hpp"
#include <fstream>

#include <iostream>

void DXF::Dxf::Valida()
{
    Ler("C:/Tony/Projeto/Inscopia.dxf");
}

size_t DXF::Dxf::NumDeLinhas(const std::filesystem::path &nome)
{
    size_t r = 0;
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);   //  primeira passada no arquivo
    if(!arq.is_open()) return r;
    size_t arqSize = (size_t)arq.tellg();
    if(arqSize <= 0) return r;
    arq.seekg(0, std::ios::beg);
    char c, d = 0;
    for(size_t i = 0; i < arqSize; i++)
    {
        c = d;
        d = arq.get();
        if((d == 10 || d == 13) && (c != 10 && c != 13)) r++;
    }
    arq.close();
    return r;
}

std::vector<DXF::Fresta> DXF::Dxf::GetBufBruto(const std::filesystem::path &nome)
{
    std::vector<Fresta> r = {};
    size_t numLn = NumDeLinhas(nome);
    if(!numLn) return r;
    r.reserve(numLn);
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);   //  segunda passada no arquivo
    if(!arq.is_open()) return r;
    size_t arqSize = (size_t)arq.tellg();
    if(arqSize <= 0) return r;
    arq.seekg(0, std::ios::beg);
    Fresta ln;
    char c, d = 10;
    for(size_t i = 0; i < arqSize; i++)
    {
        c = d;
        d = arq.get();
        if((c == 10 || c == 13) && (d != 10 && d != 13)) ln.posi = i;   //  início de linha
        else if((d == 10 || d == 13) && (c != 10 && c != 13))   //  fim de linha
        {
            ln.posf = i;
            r.push_back(ln);
        }
    }
    arq.close();
    return r;
}

std::uint32_t DXF::Dxf::NumPorTipo(const std::filesystem::path &nome, uint32_t &str, uint32_t &dbl,
                                   uint32_t &i2, uint32_t &i4, uint32_t &i8, std::uint32_t &chr)
{
    //  --Ler arquivo--
    std::vector<Fresta> buffer = GetBufBruto(nome);
    if(!buffer.size()) return std::numeric_limits<uint32_t>::max();
    std::ifstream arq(nome, std::ios::in | std::ios::binary);   //  terceira passada no arquivo
    if(!arq.is_open()) return std::numeric_limits<uint32_t>::max();
    //  --Contar a quantidade por tipo--
    uint32_t r = 0;
    str = dbl = i2 = i4 = i8 = chr = 0;
    bool cod = false, isStr = false;
    for(auto linha : buffer)
    {
        arq.seekg(linha.posi, std::ios::beg);
        cod = !cod;
        if(linha.posf == (linha.posi + 1))
        {
            if(arq.get() == 26) cod = !cod;
            arq.seekg(linha.posi, std::ios::beg);
        }
        if(cod)
        {
            r++;
            char tx[128];
            size_t j = 0;
            for(size_t i = linha.posi; i < linha.posf; i++) tx[j++] = arq.get();
            tx[j] = 0;
            int num = std::stoi(tx);
            if((num >= 10 && num <= 59) || (num >= 110 && num <= 149) || (num >= 210 && num <= 239) || (num >= 460 && num <= 469) ||
                (num >= 1010 && num <= 1059)) dbl++;
            else if((num >= 60 && num <= 79) || (num >= 270 && num <= 289) || (num >= 370 && num <= 389)) i2++;
            else if((num >= 90 && num <= 99) || (num >= 440 && num <= 459) || (num >= 1060 && num <= 1071)) i4++;
            else if(num >= 160 && num <= 179) i8++;
            else
            {
                str++;
                isStr = true;
            }
        }
        else if(isStr)
        {
            chr += (linha.posf - linha.posi);
            isStr = false;
        }
    }
    arq.close();
    return r;
}

void DXF::Dxf::Ler(const std::filesystem::path &nome)
{
    //  --Monitorar--
    //      --As quantidades--
    uint32_t total, ttexto, duplo, i16, i32, i64, nCh;
    total = NumPorTipo(nome, ttexto, duplo, i16, i32, i64, nCh);
    if(total == std::numeric_limits<uint32_t>::max()) return;
    std::cout
        << "------------------------------\nString:\t\t..\t"
        << ttexto
        << "\nDouble:\t\t..\t"
        << duplo
        << "\nUint_16:\t..\t"
        << i16
        << "\nUint_32:\t..\t"
        << i32
        << "\nUint_64:\t..\t"
        << i64
        << "\nTotal:\t\t..\t"
        << total
        << "\n-----------------------------"
        << "\nChar:\t\t..\t"
        << nCh
        << "\n-----------------------------"
        << std::endl;
    //  Pausa pra criar dicionário de strings
}