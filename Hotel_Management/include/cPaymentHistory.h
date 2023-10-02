#pragma once
#include<iostream>
#include<ctime>
#include"cClient.h"


using namespace std;

class cPaymentHistory
{
    private:
        int nPaymentID;
        static int pay_id;
        cClient* payClient;
        char* cDateTime;
        float fMoney;

    public:
        cPaymentHistory(cClient* client, char* date_time, float money);
        int GetPaymentID();
        char* GetPaymentDateTime();
        float GetPaymentMoney();

};