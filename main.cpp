//  C:\Tony\DXF\main.cpp
#include <iostream>
#include "Dxf.hpp"

//--Dxf.hpp / cpp--
void tDxf()
{
    DXF::Dxf arquivo;
    arquivo.valida("C:/Tony/Soft/cpp/integrado/sistemaIntegrado/CPP/INSTANCIA/TMP/20260710152627.tmp");
}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    tDxf();
    return 0;
}