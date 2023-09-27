#include"cClient.h"

cClient::cClient(std::string name, std::string sdt, std::string address)
{
    sSDT= sdt;
    sName = name;
    sAddress= address;
    // Getnerate time checkin
    char* time_checkin;
    time_t now = time(0);
    time_checkin= ctime(&now);
    timeCheckIn= time_checkin;
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
char* cClient::GetTimeCheckIn()
{
    return timeCheckIn;
}
void cClient::SetName(std::string name)
{
    sName= name;
}
void cClient::SetAddress(std::string address)
{
    sAddress= address;
}
void cClient::SetSDT(std::string sdt)
{
    sSDT= sdt;
}
void cClient::SetTimeCheckOut()
{
    // Getnerate time checkout
    time_t now = time(0);
    char* time_checkout= ctime(&now);
    timeCheckOut = time_checkout;
}
char* cClient::GetTimeCheckOut()
{
    return timeCheckOut;
}
void cClient::CheckOut()
{
    
}
