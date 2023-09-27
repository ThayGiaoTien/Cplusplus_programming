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
cClient* cRoom::GetClient()
{
    return client;
}
void cRoom::SetClient(cClient* new_client)
{
    client= new_client;
}
