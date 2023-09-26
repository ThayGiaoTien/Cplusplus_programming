#include<iostream>
#include<vector>
#include"cRoom.h"


typedef struct{
    int day;
    int month;
    int year;
} DateTime;

typedef struct{
    cRoom* room;
    DateTime time_info;
    int duration;
} History_Booking;


class cClient
{
    private:
        std::string sName;
        std::string sSDT;
        std::string sAddress;
        std::vector<History_Booking*> vecHistoryBookings;
    public:
        cClient(std::string name, std::string sdt, std::string address);
        std::string GetClientSDT();
        std::string GetClientAddress();
        std::string GetClientName();

};