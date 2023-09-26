#include"MonAn.h"


MonAn::MonAn(float price, string name)
{
    ID= id++;
    fPrice= price;
    sName= name;
}
void MonAn::UpdateMonAn(float price, string name)
{
    sName= name;
    fPrice= price;
}
string MonAn::GetName()
{
    return MonAn::sName;
}
float MonAn::GetPrice()
{
    return MonAn::fPrice;
}
int MonAn::GetID()
{
    return MonAn::ID;
}

int MonAn::id= 100;