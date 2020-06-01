#include <iostream>
#include "str_templates.h"

StrTemplate heading()
{
    StrTemplate h;

    h() +
    "//Copyright(2020) DFG" +
    "//ss" +
    "";

    return h;
}

StrTemplate f2()
{
    StrTemplate fun;

    fun() + "1+1";
    return fun;
}

StrTemplate for_body()
{
    StrTemplate b;

    b() +
    "if (i == 0)"+
    "{          "+
    "    f2($1); "+
    "}";

    b.replace(1, f2());
    return b;
}

int main()
{
    StrTemplate main;

    main() +
    "$1                             "+
    "int main(void)                 "+
    "{                              "+
    "    int x = f($2);             "+
    "    for (int i = 0; i<x; i++)  "+
    "    {                          "+
    "        $for_body$             "+
    "    }                          "+
    "}";

    main.replace(1, heading());
    main.replace(2, "\"hello\"");
    main.replace("for_body", for_body());

    main.changeIndent4to8();
    main.changeBracingAllmanToKR();

    std::cout << main.text();
}
