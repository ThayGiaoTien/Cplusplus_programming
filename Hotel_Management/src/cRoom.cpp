#include"cRoom.h"

cRoom::cRoom(int roomNumber)
{
    nRoomNumber= roomNumber;
    bCleaned= true;
    nStatus= FREE;
}

int cRoom::GetRoomNumber()
{
    return nRoomNumber;
}

bool cRoom::IsRoomAvailable()
{
    return (bCleaned&& nStatus==FREE);
}

void cRoom::UpdateRoomStatus(const int status)
{
    nStatus= status;
}
