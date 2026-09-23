//  C:\Tony\DXF\Dxf.cpp
#include "Dxf.hpp"
#include <fstream>

#include <iostream>

void DXF::viewChar::getTexto(char *tx)
{
    std::vector<char>::iterator it = itx + posi;
    for(size_t i = 0; i < (posf - posi); i++) tx[i] = *(it + i);
    tx[(posf - posi)] = 0;
}

void DXF::Dxf::Valida()
{
    //Ler("C:/Tony/Projeto/Inscopia.dxf");
    //AbreUmaVez("C:/Tony/Projeto/Inscopia.dxf");
    rLer("C:/Tony/Projeto/Inscopia.dxf");
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
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);   //  segunda e quarta passadas no arquivo
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
    //--Dicionário de strings--
    //  --Criar e dimensionar containeres--
    std::vector<char> chars;
    chars.reserve(nCh);
    std::vector<viewChar> oculos;
    oculos.reserve(ttexto);
    //  --Abrir arquivo--
    std::vector<Fresta> buffer = GetBufBruto(nome);
    std::ifstream arq(nome, std::ios::in | std::ios::binary);   //  quinta passada no arquivo
    //  --Ler strings
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
            char tx[128];
            size_t j = 0;
            for(size_t i = linha.posi; i < linha.posf; i++) tx[j++] = arq.get();
            tx[j] = 0;
            int num = std::stoi(tx);
            if((num >= 10 && num <= 59) || (num >= 110 && num <= 149) || (num >= 210 && num <= 239) || (num >= 460 && num <= 469) ||
                (num >= 1010 && num <= 1059)) continue;
            else if((num >= 60 && num <= 79) || (num >= 270 && num <= 289) || (num >= 370 && num <= 389)) continue;
            else if((num >= 90 && num <= 99) || (num >= 440 && num <= 459) || (num >= 1060 && num <= 1071)) continue;
            else if(num >= 160 && num <= 179) continue;
            else isStr = true;
        }
        else if(isStr)
        {
            size_t pos = chars.size();
            arq.seekg(linha.posi, std::ios::beg);
            for(size_t i = linha.posi; i < linha.posf; i++) chars.push_back(arq.get());
            viewChar v;
            v.setFonte(chars);
            v.setOculo(pos, chars.size());
            oculos.push_back(v);
            isStr = false;
        }
    }
    arq.close();
    for(auto vv : oculos)
    {
        char txt[128];
        vv.getTexto(txt);
        std::cout << txt << std::endl;
    }
}

void DXF::Dxf::AbreUmaVez(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //--Criar a variável local de quantidades--
    std::vector<size_t> tamanhos;
    tamanhos.reserve(8);    //  caracteres, linhas, ch do dicionario, string, double, uint16, uint32, uint64
    tamanhos.push_back((size_t)arq.tellg());
    if(tamanhos.at(0) <= 0) return;
    arq.seekg(0, std::ios::beg);
    char c, d = 0;
    size_t q = 0;
    for(size_t i = 0; i < tamanhos.at(0); i++)
    {
        c = d;
        d = arq.get();
        if((d == 10 || d == 13) && (c != 10 && c != 13)) q++;
    }
    tamanhos.push_back(q);
    //--Povoar buffer--
    std::vector<Fresta> buffer = {};
    buffer.reserve(tamanhos.at(1));
    arq.seekg(0, std::ios::beg);
    Fresta fr;
    d = 10;
    for(size_t i = 0; i < tamanhos.at(0); i++)
    {
        c = d;
        d = arq.get();
        if((c == 10 || c == 13) && (d != 10 && d != 13)) fr.posi = i;   //  início de linha
        else if((d == 10 || d == 13) && (c != 10 && c != 13))   //  fim de linha
        {
            fr.posf = i;
            buffer.push_back(fr);
        }
    }
    //--Obter as quantidades por tipo--
    size_t str, dbl, i2, i4, i8, chr;
    q = str = dbl = i2 = i4 = i8 = chr = 0;
    bool cod = false, isStr = false;

    std::cout
        << tamanhos.at(0)
        << " bytes em "
        << tamanhos.at(1)
        << " linhas, que correspondem a "
        << buffer.size()
        << ", o tamanho de \'buffer\'"
        << std::endl;
    arq.close();

    /*
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
    */
}

//  reestruturação
size_t DXF::Dxf::QuantLinhas(std::ifstream* file)
{
    file->clear();
    file->seekg(0, std::ios::end);
    size_t fim = (size_t)file->tellg();
    file->clear();
    file->seekg(0, std::ios::beg);
    size_t r = 0;
    char c, d = 0;
    for(size_t i = 0; i < fim; i++)
    {
        c = d;
        d = file->get();
        if((d == 10 || d == 13) && (c != 10 && c != 13)) r++;
    }
    return r;
}

DXF::TpLn DXF::Dxf::getTipo(std::ifstream *file, size_t posi, size_t posf)
{
    file->clear();
    file->seekg(posi, std::ios::beg);
    char tx[64];
    size_t i = 0;
    for(size_t j = posi; j <= posf; j++) tx[i++] = file->get();
    tx[--i] = 0;
    int k = std::stoi(tx);
    if((k >= 10 && k <= 59) || (k >= 110 && k <= 149) || (k >= 210 && k <= 239) || (k >= 460 && k <= 469) ||
        (k >= 1010 && k <= 1059)) return DBL;
    else if((k >= 60 && k <= 79) || (k >= 270 && k <= 289) || (k >= 370 && k <= 389)) return NN16;
    else if((k >= 90 && k <= 99) || (k >= 440 && k <= 459) || (k >= 1060 && k <= 1071)) return NN32;
    else if(k >= 160 && k <= 179) return NN64;
    return STR;
}

std::vector<DXF::Linha> DXF::Dxf::LerLinhas(std::ifstream *file, size_t n)
{
    file->clear();
    file->seekg(0, std::ios::end);
    size_t fim = (size_t)file->tellg();
    file->clear();
    file->seekg(0, std::ios::beg);
    std::vector<Linha> r = {};
    r.reserve(n);
    Linha ln;
    char c, d = 10;
    TpLn proxTipo;
    size_t posi, conta = 0;
    for(size_t i = 0; i < fim; i++)
    {
        c = d;
        d = file->get();
        if((c == 10 || c == 13) && (d != 10 && d != 13))    //  início
            posi = i;
        else if((c != 10 && c != 13) && (d == 10 || d == 13))   //  fim
        {
            if(((i - posi) == 1) && (c < 32)) continue;
            ln.Posi = posi;
            ln.Posf = i;
            if(conta % 2) ln.Tipo = proxTipo;
            else
            {
                proxTipo = getTipo(file, ln.Posi, ln.Posf);
                ln.Tipo = GRUPO;
            }
            r.push_back(ln);
            conta++;
        }
    }
    return r;
}

void DXF::Dxf::rLer(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //--Criar a variável local de quantidades--
    std::vector<size_t> tamanhos;
    tamanhos.reserve(16);
    tamanhos.push_back((size_t)arq.tellg());
    if(tamanhos.at(0) <= 0) return;
    tamanhos.push_back(QuantLinhas(&arq));
    //--Apresentar primeiros tamanhos--
    std::cout << tamanhos.at(0) << " caracteres em " << tamanhos.at(1) << " linhas" << std::endl;
    //--Povoar linhas--
    std::vector<Linha> linhas = {};
    linhas.reserve(tamanhos.at(1));
    linhas = LerLinhas(&arq, tamanhos.at(1));
    //--Apresentar linhas brutas--
    size_t conta = 0;
    for(auto ln : linhas)
    {
        std::cout << (int)ln.Tipo << " \'";
        arq.seekg(ln.Posi, std::ios::beg);
        for(size_t i = ln.Posi; i < ln.Posf; i++) std::cout << (char)arq.get();
        std::cout << '\'';
        if(conta % 2) std::cout << '\n';
        else std::cout << '\t';
        conta++;
    }
    //--Classificar linhas--
    //{
    //  [0 - chrs no arquivo],
    //  [1 - linhas no arquivo],
    //  [2 - linhas de grupo],
    //  [3 - doubles],
    //  [4 - uint16],
    //  [5 - uint32],
    //  [6 - uint64],
    //  [7 - strings],
    //  [8 - linhas nulas],
    //  [9 - chars de string]
    //}
}