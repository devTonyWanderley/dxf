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

void DXF::Dxf::getTexto(size_t n, std::vector<char> &vStr, std::vector<size_t> &vInd, char *tx)
{
    size_t posi, posf = vInd[n];
    if(n == 0) posi = 0;
    else posi = vInd[n - 1];
    size_t i = 0;
    for(size_t j = posi; j < posf; i++, j++) tx[i] = vStr[j];
    tx[i] = 0;
}

std::vector<size_t> DXF::Dxf::QSOrdenaStr(std::vector<char> &vStr, std::vector<size_t> &vInd)
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
            char a[256];
            getTexto(indices[i], vStr, vInd, a);
            char b[256];
            getTexto(indices[(atual.Tail - 1)], vStr, vInd, b);
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
        char a[256];
        getTexto(indices[i], vStr, vInd, a);
        char b[256];
        getTexto(ir[(ir.size() - 1)], vStr, vInd, b);
        if(strcmp(a, b) != 0) ir.push_back(indices[i]);
    }
    ir.shrink_to_fit();
    return ir;
}

void DXF::Dxf::LAPovoar(ARQ::Arquivo &fonte)
{
    size_t n = fonte.GetLnTotal();
    std::vector<size_t> q = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    char str[256];
    bool isGrupo = false;
    Grupo grupo;
    for(size_t i = 0; i < n; i++)
    {
        fonte.GetLinha(i, str);
        isGrupo = !isGrupo;
        if(strlen(str) == 1 && str[0] < 32) isGrupo = !isGrupo;
        else
        {
            if(isGrupo)
            {
                grupo = getGrupo(str);
                switch (grupo)
                {
                case Grupo::BOOL:
                    q[0]++;
                    break;
                case Grupo::BINARY:
                    q[1]++;
                    break;
                case Grupo::INT16:
                    q[2]++;
                    break;
                case Grupo::INT32:
                    q[3]++;
                    break;
                case Grupo::INT64:
                    q[4]++;
                    break;
                case Grupo::DOUBLE:
                    q[5]++;
                    break;
                case Grupo::STRING:
                    q[6]++;
                    break;
                case Grupo::HANDLE:
                    q[7]++;
                    break;
                default:
                    break;
                }
            }
            else
            {
                if(grupo == Grupo::STRING) q[8] += (size_t)strlen(str);
                else if(grupo == Grupo::HANDLE) q[9] += (size_t)strlen(str);
            }
        }
    }
    mBool.reserve(q[0]);
    mBufBin.reserve(q[1]);
    mBufI16.reserve(q[2]);
    mBufI32.reserve(q[3]);
    mBufI64.reserve(q[4]);
    mBufDbl.reserve(q[5]);
    mBufStr.reserve(q[6]);
    mBufHnd.reserve(q[7]);
    mIndStr.reserve(q[8]);
    mIndHnd.reserve(q[9]);
    size_t iStr = 0, iHnd = 0;
    isGrupo = false;
    grupo = Grupo::GRUPO;
    for(size_t i = 0; i < n; i++)
    {
        fonte.GetLinha(i, str);
        isGrupo = !isGrupo;
        if(strlen(str) == 1 && str[0] < 32) isGrupo = !isGrupo;
        else
        {
            if(isGrupo) grupo = getGrupo(str);
            else
            {
                switch (grupo) {
                case Grupo::BOOL:
                    mBool.push_back((bool)std::stoi(str));
                    break;
                case Grupo::BINARY:
                    mBufBin.push_back((uint8_t)std::stoi(str));
                    break;
                case Grupo::INT16:
                    mBufI16.push_back((uint16_t)std::stoull(str));
                    break;
                case Grupo::INT32:
                    mBufI32.push_back((uint32_t)std::stoull(str));
                    break;
                case Grupo::INT64:
                    mBufI64.push_back((uint64_t)std::stoull(str));
                    break;
                case Grupo::DOUBLE:
                    mBufDbl.push_back((double)std::stod(str));
                    break;
                case Grupo::STRING:
                    iStr += (size_t)strlen(str);
                    mIndStr.push_back(iStr);
                    for(size_t j = 0; j < (size_t)strlen(str); j++) mBufStr.push_back(str[j]);
                    break;
                case Grupo::HANDLE:
                    iHnd += (size_t)strlen(str);
                    mIndHnd.push_back(iHnd);
                    for(size_t j = 0; j < (size_t)strlen(str); j++) mBufHnd.push_back(str[j]);
                    break;
                default:
                    break;
                }
            }
        }
    }
    if(mBufBin.size())
    {
        Dados::OrdenaVectorQS(mBufBin, false);
        mBufBin.shrink_to_fit();
    }
    if(mBufI16.size())
    {
        Dados::OrdenaVectorQS(mBufI16, false);
        mBufI16.shrink_to_fit();
    }
    if(mBufI32.size())
    {
        Dados::OrdenaVectorQS(mBufI32, false);
        mBufI32.shrink_to_fit();
    }
    if(mBufI64.size())
    {
        Dados::OrdenaVectorQS(mBufI64, false);
        mBufI64.shrink_to_fit();
    }
    if(mBufDbl.size())
    {
        Dados::OrdenaVectorQS(mBufDbl, false);
        mBufDbl.shrink_to_fit();
    }
    if(mIndStr.size())
    {
        std::vector<size_t> v = QSOrdenaStr(mBufStr, mIndStr);
        size_t nChar = 0;
        for(size_t i = 0; i < v.size(); i++)
        {
            getTexto(v[i], mBufStr, mIndStr, str);
            nChar += strlen(str);
        }
        std::vector<char> u = {};
        u.reserve(nChar);
        std::vector<size_t> w = {};
        w.reserve(v.size());
        for(size_t i = 0; i < v.size(); i++)
        {
            getTexto(v[i], mBufStr, mIndStr, str);
            for(size_t j = 0; j < strlen(str); j++) u.push_back(str[j]);
            w.push_back(u.size());
        }
        mBufStr.swap(u);
        mBufStr.shrink_to_fit();
        mIndStr.swap(w);
        mIndStr.shrink_to_fit();
    }
    if(mIndHnd.size())
    {
        std::vector<size_t> v = QSOrdenaStr(mBufHnd, mIndHnd);
        size_t nChar = 0;
        for(size_t i = 0; i < v.size(); i++)
        {
            getTexto(v[i], mBufHnd, mIndHnd, str);
            nChar += strlen(str);
        }
        std::vector<char> u = {};
        u.reserve(nChar);
        std::vector<size_t> w = {};
        w.reserve(v.size());
        for(size_t i = 0; i < v.size(); i++)
        {
            getTexto(v[i], mBufHnd, mIndHnd, str);
            for(size_t j = 0; j < strlen(str); j++) u.push_back(str[j]);
            w.push_back(u.size());
        }
        mBufHnd.swap(u);
        mBufHnd.shrink_to_fit();
        mIndHnd.swap(w);
        mIndHnd.shrink_to_fit();
    }
    /*
    */
}

bool DXF::Dxf::LerArquivo(const std::filesystem::path &fonte)
{
    ARQ::Arquivo arq;
    if(!arq.SetArquivo(fonte)) return false;
    LAPovoar(arq);

    std::cout
        << "mBool.size(): " << mBool.size()
        << "\nmBufBin.size(): " << mBufBin.size()
        << "\nmBufI16.size(): " << mBufI16.size()
        << "\nmBufI32.size(): " << mBufI32.size()
        << "\nmBufI64.size(): " << mBufI64.size()
        << "\nmBufDbl.size(): " << mBufDbl.size()
        << "\nmBufStr.size(): " << mBufStr.size()
        << "\nmBufHnd.size(): " << mBufHnd.size()
        << "\nmIndStr.size(): " << mIndStr.size()
        << "\nmIndHnd.size(): " << mIndHnd.size()
        << std::endl;

    return true;
}