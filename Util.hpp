//  C:\Tony\DXF\Util.hpp
#pragma once
#include <cstdlib>
#include <cstdint>
#include <stack>
#include <utility>
#include <vector>
#include <limits>

#include <iostream>

namespace Dados
{
struct ParI{size_t Hard, Tail;};

template <typename C>
void OrdenaQS(C &v);

template <typename T>
void OrdenaVectorQS(std::vector<T> &v, bool repete);

template <typename T>
void OrdenaVectorQS(std::vector<T> &v);

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
