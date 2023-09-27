#pragma once
#include<iostream>

#include"cClient.h"
class cClient;

typedef enum{
    FREE,
    BOOKED
} RoomStatus;

class cRoom
{
    private:
        int nRoomNumber;
        bool bCleaned;
        int nStatus ;
        cClient* client;

    public:
        cRoom(int roomNumber);
        int GetRoomNumber();
        bool IsRoomAvailable();
        void UpdateRoomStatus(const int status);
        void SetClient(cClient* new_client);
        cClient* GetClient();
};