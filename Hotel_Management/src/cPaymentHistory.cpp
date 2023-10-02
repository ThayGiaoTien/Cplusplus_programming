#include"cPaymentHistory.h"

    cPaymentHistory::cPaymentHistory(cClient* client, char* date_time, float money)
    {
        nPaymentID= pay_id++;
        payClient= client;
        cDateTime= date_time;
        fMoney= money;
}

int cPaymentHistory::pay_id= 0;

int cPaymentHistory::GetPaymentID()
{
    return nPaymentID;
}
char* cPaymentHistory::GetPaymentDateTime()
{
    return cDateTime;
}
float cPaymentHistory::GetPaymentMoney()
{
    return fMoney;
}