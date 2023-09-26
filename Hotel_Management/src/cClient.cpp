#include"cClient.h"


cClient::cClient(std::string name, std::string sdt, std::string address)
{
    sSDT= sdt;
    sName = name;
    sAddress= address;
}

std::string cClient::GetClientAddress()
{
    return sAddress;
}
std::string cClient::GetClientName()
{
    return sName;
}
std::string cClient::GetClientSDT()
{
    return sSDT;
}