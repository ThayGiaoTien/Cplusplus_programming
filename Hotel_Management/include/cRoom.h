#include<iostream>

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
    public:
        cRoom(int roomNumber);
        int GetRoomNumber();
        bool IsRoomAvailable();
        void UpdateRoomStatus(const int status);
};