#include"cAccount.h"

int cAccount::id= 100;

cAccount::cAccount(string username, string password)
{
    ID= id++;
    sUserName= username;
    sUserPassword= password;
    nAuthorization= CLIENT;
}

string cAccount::GetUserName()
{
    return sUserName;
}
string cAccount::GetPassWord()
{
    return sUserPassword;
}
int cAccount::GetAuthorization()
{
    return nAuthorization;
}

void cAccount::SetAuthorization(int authorization)
{
    nAuthorization= authorization;
}