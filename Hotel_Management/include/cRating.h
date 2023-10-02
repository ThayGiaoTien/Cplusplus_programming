#pragma once
#include<iostream>
#include"cClient.h"

using namespace std;

class cRating
{
    private:
        int nRate;
        cClient* ratingClient;
        char* cDateTime;
        string sComment;
    public:
        cRating(int rate, string comment, cClient* client);
        cClient* getClient();
        char* getDateTime();
        int getRate();
        string getComment();
};