#pragma once
#include<iostream>
#include<vector>
#include"cRoom.h"
#include<ctime>

class cRoom;
typedef struct{
    const cRoom* room;
    char timeCheckIn;
    char timeCheckOut;
} History_Booking;


class cClient
{
    private:
        std::string sName;
        std::string sSDT;
        std::string sAddress;
        char* timeCheckIn;
        char* timeCheckOut;
        std::vector<History_Booking*> vecHistoryBookings;
    public:
        cClient(std::string name, std::string sdt, std::string address);
        std::string GetClientSDT();
        std::string GetClientAddress();
        std::string GetClientName();
        char* GetTimeCheckIn();
        void SetName(std::string name);
        void SetSDT(std::string sdt);
        void SetAddress(std::string address);
        void SetTimeCheckOut();  
        char* GetTimeCheckOut();
        void CheckOut();

};