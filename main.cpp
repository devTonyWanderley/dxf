//  C:\Tony\DXF\main.cpp
#include <iostream>
#include "Dxf.hpp"

//--Dxf.hpp / cpp--
void tDxf()
{
    DXF::Dxf arquivo;
    arquivo.valida("C:/Tony/Soft/cpp/integrado/sistemaIntegrado/CPP/INSTANCIA/TMP/20260710152627.tmp");
}

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

void tOrdenaVectorQS()
{
    std::vector<char> ch = {'M', 'u', 's', 'i', 'c', 'a', 'P', 'o', 'p', 'u', 'l', 'a', 'r', 'B', 'r', 'a', 's', 'i', 'l', 'e', 'i', 'r', 'a'};
    std::vector<double> db = {3.14, 2.78, -1.68, 45, 299761468, 3.14, -1.68};
    //std::vector<int> in = {3, -2, 1, 2, 14, -78};
    for(auto e : ch) std::cout << e << ' ';
    std::cout << '\n';
    Dados::OrdenaVectorQS(ch, false);
    for(auto e : ch) std::cout << e << ' ';
    std::cout << '\n';

    for(auto e : db) std::cout << e << ' ';
    std::cout << '\n';
    Dados::OrdenaVectorQS(db, false);
    for(auto e : db) std::cout << e << ' ';
    std::cout << '\n';
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    tDxf();
    //tOrdenaQS();
    //tOrdenaVectorQS();
    return 0;
}