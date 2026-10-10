//  C:\Tony\DXF\Util.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <stack>
#include <utility>
#include <vector>

namespace Dados
{
struct ParI{size_t Hard, Tail;};

template <typename C>
void OrdenaQS(C &v)
{
    if(v.size() < 2) return;
    std::stack<ParI> w;
    w.push({0, v.size()});
    while(!w.empty())
    {
        ParI atual = w.top();
        w.pop();
        size_t p = atual.Hard, q;
        for(size_t j = atual.Hard; j < (atual.Tail - 1); j++) {if(v[j] < v[atual.Tail - 1]) {std::swap(v[p++], v[j]);}}
        std::swap(v[p], v[atual.Tail - 1]);
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
}

template <typename T>
void OrdenaVectorQS(std::vector<T> &v, bool repete)
{
    if(v.size() < 2) return;
    std::stack<ParI> w;
    w.push({0, v.size()});
    while(!w.empty())
    {
        ParI atual = w.top();
        w.pop();
        size_t p = atual.Hard, q;
        for(size_t j = atual.Hard; j < (atual.Tail - 1); j++) {if(v[j] < v[atual.Tail - 1]) {std::swap(v[p++], v[j]);}}
        std::swap(v[p], v[atual.Tail - 1]);
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
    if(!repete)
    {
        std::vector<T> u;
        u.swap(v);
        v.push_back(u[0]);
        for(size_t i = 1; i < u.size(); i++) if(u[i] != v[(v.size() - 1)]) v.push_back(u[i]);
    }
}

template <typename T>
void OrdenaVectorQS(std::vector<T> &v){OrdenaVectorQS(v, true);}

} // namespace Dados

//--EXEMPLO DE USO--
/*

void tOrdenaQS()
{
    std::vector<char> ch = {'M', 'u', 's', 'i', 'c', 'a', 'P', 'o', 'p', 'u', 'l', 'a', 'r', 'B', 'r', 'a', 's', 'i', 'l', 'e', 'i', 'r', 'a'};
    std::vector<double> db = {3.14, 2.78, -1.68, 45, 299761468};
    std::vector<int> in = {3, -2, 1, 2, 14, -78};
    for(auto e : ch) std::cout << e << ' ';
    std::cout << '\n';
    Dados::OrdenaQS<std::vector<char>>(ch);
    for(auto e : ch) std::cout << e << ' ';
    std::cout << '\n';
    for(auto e : db) std::cout << e << ' ';
    std::cout << '\n';
    Dados::OrdenaQS<std::vector<double>>(db);
    for(auto e : db) std::cout << e << ' ';
    std::cout << '\n';
    for(auto e : in) std::cout << e << ' ';
    std::cout << '\n';
    Dados::OrdenaQS<std::vector<int>>(in);
    for(auto e : in) std::cout << e << ' ';
    std::cout << '\n';
}
*/
//==============================================================================================================================================
