//  C:\Tony\DXF\Dxf.cpp
#include "Dxf.hpp"
#include <fstream>
#include <cstring>
#include <stack>
#include <charconv>

//--PROVISÓRIO PRA MONITORAMENTO--  =========================================================
#include <iostream>
void DXF::Dxf::Valida()
{
    Ler("C:/Tony/Projeto/Inscopia.dxf");
}
//===========================================================================================

//--LINHA-- =================================================================================
void DXF::Linha::getString(std::ifstream *file, char *tx)
{
    file->clear();
    file->seekg(Posi, std::ios::beg);
    size_t i = 0;
    for(size_t j = Posi; j <= Posf; j++)
    {
        if (i >= 127) break;
        tx[i++] = file->get();
    }
    tx[--i] = 0;
    while (i > 0 && (tx[i - 1] == '\r' || tx[i - 1] == '\n'))
    {
        i--;
        tx[i] = '\0';
    }
}

bool DXF::Linha::igual_(std::ifstream *file, Linha &outra)
{
    if(Tipo != outra.Tipo) return false;
    if(Tipo == NULO || outra.Tipo == NULO) return false;
    char tx0[128], tx1[128];
    this->getString(file, tx0);
    outra.getString(file, tx1);
    //f(Tipo == STR) return !strcmp(tx0, tx1);   //  substituir essa linha
    if(Tipo == STR) {
        char* p0 = tx0;
        char* p1 = tx1;
        while(*p0 != '\0' && *p0 <= 32) p0++;
        while(*p1 != '\0' && *p1 <= 32) p1++;
        return (strcmp(p0, p1) == 0);
    }
    if(Tipo == DBL) return (std::stod(tx0) == std::stod(tx1));
    return (std::stoll(tx0) == std::stoll(tx1));
}

bool DXF::Linha::maior_(std::ifstream *file, Linha &outra)
{
    if(Tipo != outra.Tipo) return false;
    if(Tipo == NULO || outra.Tipo == NULO) return false;
    char tx0[128], tx1[128];
    this->getString(file, tx0);
    outra.getString(file, tx1);
    if(Tipo == STR) {
        char* p0 = tx0;
        char* p1 = tx1;
        while(*p0 != '\0' && *p0 <= 32) p0++;
        while(*p1 != '\0' && *p1 <= 32) p1++;
        return (strcmp(p0, p1) > 0);
    }
    if(Tipo == DBL) return (std::stod(tx0) > std::stod(tx1));
    return (std::stoll(tx0) > std::stoll(tx1));
}

DXF::Linha& DXF::Linha::operator =(const Linha outra)
{
    Posi = outra.Posi;
    Posf = outra.Posf;
    Tipo = outra.Tipo;
    return *this;
}
//===========================================================================================

//--DXF--   =================================================================================

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

size_t DXF::Dxf::PivotaStr(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst, Trecho tre)
{
    size_t p = tre.head, t = tre.tail, h = tre.head + 1;
    for(size_t i = h; i <= t; i++)
    {
        Linha lni = lns.at(lst.at(i)), lnp = lns.at(lst.at(p));
        if(lnp.maior_(file, lni))
        {
            std::swap(lst.at(i), lst.at(p));
            if(++p != i) std::swap(lst.at(i), lst.at(p));
        }
    }
    return p;
}

void DXF::Dxf::OrdenaStr(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst)
{
    std::stack<Trecho> pilha;
    Trecho tr;
    tr.head = 0;
    tr.tail = lst.size() - 1;
    pilha.push(tr);
    while(!pilha.empty())
    {
        size_t h = pilha.top().head, t = pilha.top().tail;
        size_t p = PivotaStr(file, lns, lst, pilha.top());
        pilha.pop();
        if(p > (h + 1))
        {
            tr.head = h;
            tr.tail = p - 1;
            pilha.push(tr);
        }
        if(t > (p - 1))
        {
            tr.head = p + 1;
            tr.tail = t;
            pilha.push(tr);
        }
    }
}

std::vector<size_t> DXF::Dxf::OrdenaStrExclusivo(std::ifstream *file, std::vector<Linha> &lns, std::vector<size_t> &lst)
{
    std::vector<size_t> r = {};
    r.reserve(lst.size());
    OrdenaStr(file, lns, lst);
    r.push_back(lst[0]);
    for(auto ie : lst)
    {
        char tx[128], txa[128];
        Linha e = lns[ie], e0 = lns[r[r.size() - 1]];
        e.getString(file, tx);
        e0.getString(file, txa);
        if(!std::strcmp(tx, txa)) continue;
        r.push_back(ie);
    }
    return r;
}

void DXF::Dxf::Ler(const std::filesystem::path &nome)   //  **particionar o 'Ler'**
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //=======================================================================================

    //--Criar a variável local de quantidades--
    std::vector<size_t> tamanhos;
    tamanhos.reserve(16);
    tamanhos.push_back((size_t)arq.tellg());    //  tamanhos[0] <- número de caracteres no
                                                //  arquivo
    if(tamanhos.at(0) <= 0) return;
    tamanhos.push_back(QuantLinhas(&arq));    //  tamanhos[1] <- número de strings no arquivo
    //=======================================================================================

    //--Povoar linhas--
    std::vector<Linha> linhas = {};
    linhas.reserve(tamanhos.at(1));
    linhas = LerLinhas(&arq, tamanhos.at(1));
    //=======================================================================================

    //--Fazer dicionário de strings--               **modificar pra criar os 5 dicionários**
    //  --Adquirir a quantidade de linhas tipo STR--    **achar as 5 quantidades**
    tamanhos.push_back(0);      //  tamanhos[2] <- número de dados a serem armazenados como
                                //  strings
    for(auto ln : linhas) if(ln.Tipo == STR) tamanhos.at(2)++;
    //=======================================================================================

    //  --Adquirir as linhas tipo STR--     **extender pra os 5 tipos**
    std::vector<size_t> iStrs = {};
    iStrs.reserve(tamanhos.at(2));
    for(size_t i = 0; i < linhas.size(); i++) if(linhas.at(i).Tipo == STR) iStrs.push_back(i);
    //=======================================================================================

    //  --Ordenar e remover duplicatas de strings--     **extender pra os 5 tipos**
    std::vector<size_t> ordenado = {};
    ordenado = OrdenaStrExclusivo(&arq, linhas, iStrs);
    ordenado.shrink_to_fit();
    //=======================================================================================

    //  --Apresentar lista ordenada--
    for(size_t i = 0; i < ordenado.size(); i++)
    {
        char tx[64];
        linhas.at(ordenado.at(i)).getString(&arq, tx);
        std::cout << i << '\t' << iStrs[i] << '\t' << '\'' << tx << '\'' << std::endl;
    }
    //=======================================================================================

    //  --Adquirir o número de caracteres do buffer de char e carregar dicionário--
    //  **extender pra os 5 tipos**
    tamanhos.push_back(ordenado.size());    //  tamanhos[3] <- número de strings no dicionário
    dicioStr.dict.reserve(tamanhos.at(3) + 1);
    dicioStr.dict.push_back(0);
    tamanhos.push_back(0);    //  tamanhos[4] <- número de caracteres no dicionário
    for(size_t qc : ordenado)
    {
        char tx[64];
        linhas.at(qc).getString(&arq, tx);
        tamanhos[4] += (size_t)std::strlen(tx);
        dicioStr.dict.push_back(tamanhos[4]);
    }
    dicioStr.buffer.reserve(tamanhos[4]);
    for(size_t qc : ordenado)
    {
        char tx[64];
        linhas.at(qc).getString(&arq, tx);
        for(size_t i = 0; i < std::strlen(tx); i++) dicioStr.buffer.push_back(tx[i]);
    }
    //=======================================================================================

    //  --Apresentar dicionário--
    for(size_t i = 1; i < dicioStr.dict.size(); i++)
    {
        char tx[64];
        dicioStr.getStr(i, tx);
        std::cout << '\'' << tx << '\'' << std::endl;
    }
    //=======================================================================================

    //  --Ordenar as linhas tipo STR--
    //{
    //  [0 - chrs no arquivo],
    //  [1 - linhas no arquivo],
    //  [2 - strings],
    //  [3 - chars de string],
    //  [3 - doubles],
    //  [4 - uint16],
    //  [5 - uint32],
    //  [6 - uint64],
    //  [8 - linhas nulas],
    //  [9 - linhas de grupo]
    //}
}
//===========================================================================================

//--DXF::DxfDoc--   =========================================================================
void DXF::DxfDoc::valida()
{
    Ler("C:/Tony/Projeto/Inscopia.dxf");
}

DXF::TipoDado DXF::DxfDoc::getTipoDeStr(char *tx)
{
    if(tx[strlen(tx)] != 0) return TipoDado::INDEFINIDO;
    int j = 0;
    for(auto i = 0; i < strlen(tx); i++)
    {
        if((tx[i] < '0' || tx[i] > '9') && tx[i] != ' ') return TipoDado::INDEFINIDO;
        if(tx[i] >= '0' && tx[i] <= '9') j++;
    }
    if(!j) return TipoDado::INDEFINIDO;
    int v = std::stoi(tx);
    if((v >= 0 && v < 10) || v == 100 || v == 102 || (v >= 300 && v < 310) || (v >= 470 && v < 480) || (v >= 1000 && v < 1010))
        return TipoDado::STRING;
    if((v >= 10 && v < 60) || (v >= 110 && v < 150) || (v >= 210 && v < 240) || (v >= 460 && v < 470) || (v >= 1010 && v < 1060))
        return TipoDado::DOUBLE;
    if((v >= 60 && v < 80) || (v >= 270 && v < 290) || (v >= 370 && v < 390)) return TipoDado::INT16;
    if((v >= 90 && v < 100) || (v >= 440 && v < 460) || (v >= 1060 && v < 1072)) return TipoDado::INT32;
    if(v >= 160 && v < 170) return TipoDado::INT64;
    if(v == 5 || v == 105 || (v >= 320 && v < 370)) return TipoDado::HANDLE;
    if(v >= 290 && v < 300) return TipoDado::BOOL;
    if(v == 1004 || (v >= 310 && v < 320)) return TipoDado::BINARIO;
    return TipoDado::INDEFINIDO;
}

std::vector<size_t> DXF::DxfDoc::getQuantidades(std::ifstream *file, size_t n)
{
    file->clear();
    file->seekg(0, std::ios::beg);
    std::vector<size_t> r = {};
    r.reserve(12);
    for(auto i = 0; i < 12; i++) r.push_back(0);
    r[0] += n;  //  n° de caracteres do arquivo
    char c, d = 10, tx[128], *p;
    TipoDado Tipo = TipoDado::INDEFINIDO;
    for(size_t i = 0; i < r[0]; i++)
    {
        c = d;
        d = file->get();
        if((c == 10 || c == 13) && (d == 10 || d == 13)) continue;
        if((c == 10 || c == 13) && (d != 10 && d != 13))    //  --início da linha--
        {
            p = tx;
            *p = d;
            p++;
        }
        else if((c != 10 && c != 13) && (d == 10 || d == 13))   //  --fim da linha--
        {
            if(strlen(tx) == 1 && tx[0] < 32) continue;
            *p = 0;
            if(!(r[1] % 2))     //  grupo
            {
                std::cout << '\'' << tx << "\'\t";
                Tipo = getTipoDeStr(tx);
                switch (Tipo) {
                case TipoDado::DOUBLE:
                    r[2]++;
                    break;
                case TipoDado::INT16:
                    r[3]++;
                    break;
                case TipoDado::INT32:
                    r[4]++;
                    break;
                case TipoDado::INT64:
                    r[5]++;
                    break;
                case TipoDado::STRING:
                    r[6]++;
                    break;
                case TipoDado::HANDLE:
                    r[7]++;
                    r[11] += (size_t)strlen(tx);
                    break;
                case TipoDado::BOOL:
                    r[8]++;
                    break;
                case TipoDado::BINARIO:
                    r[9]++;
                    break;
                default:
                    std::cout << "Tipo indefinido na linha " << (r[1] + 1) << '\t';
                }
            }
            else    //  não grupo;
            {
                std::cout << '\'' << tx << '\'' << std::endl;
                if(Tipo == TipoDado::STRING) r[10] += (size_t)strlen(tx);
                else if(Tipo == TipoDado::HANDLE) r[11] += (size_t)strlen(tx);
            }
            if(strlen(tx) == 1 && tx[0] < 32) std::cout << "\nVeio aqui" << std::endl;
            r[1]++;
            if(strlen(tx) == 1 && tx[0] < 32) r[1]--;
        }
        else    //  meio de linha
        {
            *p = d;
            p++;
        }
    }
    return r;
}

void DXF::DxfDoc::Povoar(std::ifstream *file, size_t n)
{
    file->clear();
    file->seekg(0, std::ios::beg);
    std::vector<size_t> q = {};
    q.reserve(12);
    for(auto i = 0; i < 12; i++) q.push_back(0);
    q[0] += n;  //  n° de caracteres do arquivo
    char c, d = 10, tx[128], *p;
    TipoDado Tipo = TipoDado::INDEFINIDO;
    for(size_t i = 0; i < q[0]; i++)
    {
        c = d;
        d = file->get();
        if((c == 10 || c == 13) && (d == 10 || d == 13)) continue;
        if((c == 10 || c == 13) && (d != 10 && d != 13))    //  --início da linha--
        {
            p = tx;
            *p = d;
            p++;
        }
        else if((c != 10 && c != 13) && (d == 10 || d == 13))   //  --fim da linha--
        {
            if(strlen(tx) == 1 && tx[0] < 32) continue;
            *p = 0;
            if(!(q[1] % 2))     //  grupo
            {
                Tipo = getTipoDeStr(tx);
                switch (Tipo) {
                case TipoDado::DOUBLE:
                    q[2]++;
                    break;
                case TipoDado::INT16:
                    q[3]++;
                    break;
                case TipoDado::INT32:
                    q[4]++;
                    break;
                case TipoDado::INT64:
                    q[5]++;
                    break;
                case TipoDado::STRING:
                    q[6]++;
                    break;
                case TipoDado::HANDLE:
                    q[7]++;
                    break;
                case TipoDado::BOOL:
                    q[8]++;
                    break;
                case TipoDado::BINARIO:
                    q[9]++;
                    break;
                default:
                    break;
                }
            }
            else    //  não grupo;
            {
                if(Tipo == TipoDado::STRING) q[10] += (size_t)strlen(tx);
                else if(Tipo == TipoDado::HANDLE) q[11] += (size_t)strlen(tx);
            }
            q[1]++;
            if(strlen(tx) == 1 && tx[0] < 32) q[1]--;
        }
        else    //  meio de linha
        {
            *p = d;
            p++;
        }
    }
    //  **monitoramento**
    int jj = 0;
    for(auto e : q) std::cout << jj++ << '\t' << e << std::endl;
    //  **fim do monitoramento**
    std::vector<double> vDbl = {};
    std::vector<std::uint8_t> vBin = {};
    std::vector<std::int16_t> vInt16 = {};
    std::vector<std::int32_t> vInt32 = {};
    std::vector<std::int64_t> vInt64 = {};
    std::vector<char> vStr = {};
    std::vector<size_t> iStr = {};
    std::vector<char> vHnd = {};
    std::vector<size_t> iHnd = {};
    std::vector<bool> vBoo = {};
    if(q[2]) vDbl.reserve(q[2]);
    if(q[3]) vInt16.reserve(q[3]);
    if(q[4]) vInt32.reserve(q[4]);
    if(q[5]) vInt64.reserve(q[5]);
    if(q[6]) iStr.reserve(q[6] + 1);
    if(q[7]) iHnd.reserve(q[7] + 1);
    if(q[8]) vBoo.reserve(q[8]);
    if(q[9]) vBin.reserve(q[9]);
    if(q[10]) vStr.reserve(q[10]);
    if(q[11]) vHnd.reserve(q[11]);
    file->clear();
    file->seekg(0, std::ios::beg);
    d = 10;
    Tipo = TipoDado::INDEFINIDO;
    size_t sPos = 0, hPos = 0;
    for(size_t i = 0; i < q[0]; i++)
    {
        c = d;
        d = file->get();
        if((c == 10 || c == 13) && (d == 10 || d == 13)) continue;
        if((c == 10 || c == 13) && (d != 10 && d != 13))    //  --início da linha--
        {
            p = tx;
            *p = d;
            p++;
        }
        else if((c != 10 && c != 13) && (d == 10 || d == 13))   //  --fim da linha--
        {
            if(strlen(tx) == 1 && tx[0] < 32) continue;
            *p = 0;
            if(!(q[1] % 2)) Tipo = getTipoDeStr(tx);
            else    //  não grupo;
            {
                switch (Tipo)
                {
                case TipoDado::DOUBLE:
                {
                    double vd;
                    auto [pd, ed] = std::from_chars(tx, tx + std::strlen(tx), vd);
                    if(ed == std::errc{} && *pd == '\0') vDbl.push_back(vd);
                }
                    break;
                case TipoDado::BINARIO:
                {
                    std::uint8_t vb;
                    auto [pb, eb] = std::from_chars(tx, tx + std::strlen(tx), vb);
                    if(eb == std::errc{} && *pb == '\0') vBin.push_back(vb);
                }
                    break;
                case TipoDado::INT16:
                {
                    std::int16_t vI16;
                    auto [pI, ei] = std::from_chars(tx, tx + std::strlen(tx), vI16);
                    if(ei == std::errc{} && *pI == '\0') vInt16.push_back(vI16);
                }
                    break;
                case TipoDado::INT32:
                {
                    std::int32_t vI32;
                    auto [pIi, eii] = std::from_chars(tx, tx + std::strlen(tx), vI32);
                    if(eii == std::errc{} && *pIi == '\0') vInt32.push_back(vI32);
                }
                    break;
                case TipoDado::INT64:
                {
                    std::int16_t vI64;
                    auto [pIii, eiii] = std::from_chars(tx, tx + std::strlen(tx), vI64);
                    if(eiii == std::errc{} && *pIii == '\0') vInt64.push_back(vI64);
                }
                    break;
                case TipoDado::BOOL:
                {
                    std::uint8_t vbo;
                    auto [pbo, ebo] = std::from_chars(tx, tx + std::strlen(tx), vbo);
                    if(ebo == std::errc{} && *pbo == '\0') vBin.push_back((vbo > 0));
                }
                    break;
                case TipoDado::STRING:
                {
                    for(auto i = 0; i < strlen(tx); i++) vStr.push_back(tx[i]);
                    iStr.push_back(sPos);
                    sPos += strlen(tx);
                }
                    break;
                case TipoDado::HANDLE:
                {
                    for(auto i = 0; i < strlen(tx); i++) vHnd.push_back(tx[i]);
                    iHnd.push_back(hPos);
                    hPos += strlen(tx);
                }
                    break;
                default:
                    break;
                }
            }
        }
        else    //  meio de linha
        {
            *p = d;
            p++;
        }
    }
    if(q[6]) iStr.push_back(sPos);
    if(q[7]) iHnd.push_back(hPos);
    //  **monitoramento**
    if(q[2])
    {
        std::cout << "\tDouble: " << vDbl.size() << std::endl;
        for(auto e : vDbl) std::cout << e << std::endl;
    }
    if(q[3])
    {
        std::cout << "\tInt16_t: " << vInt16.size() << std::endl;
        for(auto e : vInt16) std::cout << e << std::endl;
    }
    if(q[4])
    {
        std::cout << "\tInt32_t: " << vInt32.size() << std::endl;
        for(auto e : vInt32) std::cout << e << std::endl;
    }
    if(q[5])
    {
        std::cout << "\tInt64_t: " << vInt64.size() << std::endl;
        for(auto e : vInt64) std::cout << e << std::endl;
    }
    if(q[6])
    {
        std::cout << "\tString: " << iStr.size() << std::endl;
        for(auto e : iStr) std::cout << e << std::endl;
    }
    if(q[7])
    {
        std::cout << "\tHandle: " << iHnd.size() << std::endl;
        for(auto e : iHnd) std::cout << e << std::endl;
    }
    if(q[8])
    {
        std::cout << "\tBool:\n";
        for(auto e : vBoo) std::cout << (int)e << std::endl;
    }
    if(q[9])
    {
        std::cout << "\tBinario:\n";
        for(auto e : vBin) std::cout << (uint8_t)e << std::endl;
    }
    if(q[10])
    {
        std::cout << "\tCaracteres de String:\n";
        for(auto e : vStr) std::cout << (char)e;
        std::cout << std::endl;
    }
    if(q[11])
    {
        std::cout << "\tCaracteres de Handle:\n";
        for(auto e : vHnd) std::cout << (char)e;
        std::cout << std::endl;
    }
    //  **fim do monitoramento**
}

void DXF::DxfDoc::Ler(const std::filesystem::path &nome)
{
    //--Abrir o arquivo--
    std::ifstream arq(nome, std::ios::in | std::ios::binary | std::ios::ate);
    if(!arq.is_open()) return;
    //=======================================================================================

    //--Adquirir quantidades--
    size_t chEmArq = (size_t)arq.tellg();
    if(!chEmArq) return;
    /*
    std::vector<size_t> quantidades = getQuantidades(&arq, chEmArq);
    //=======================================================================================

    //--Apresentar quantidades  **monitoramento**--
    size_t moniInt = 0;
    for(auto e : quantidades) std::cout << moniInt++ << '\t' << e << std::endl;
    //=======================================================================================
    */

    //--Povoar--    **Ajuntar o 'Adquirir quantidades' com o atual**
    Povoar(&arq, chEmArq);
    //=======================================================================================
}
//===========================================================================================