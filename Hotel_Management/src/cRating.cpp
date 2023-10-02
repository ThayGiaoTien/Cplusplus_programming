#include"cRating.h"

cRating::cRating(int rate, string comment, cClient* client)
{
    nRate= rate;
    sComment= comment;
    ratingClient= client;
    cDateTime= client->GetTimeCheckOut();
}

cClient* cRating::getClient()
{
    return this->ratingClient;
}
int cRating::getRate()
{
    return this->nRate;
}
char* cRating::getDateTime()
{
    return this->cDateTime;
}
string cRating::getComment()
{
    return this->sComment;
}