#pragma once
#include<iostream>
using namespace std;

typedef enum
{
    CLIENT, 
    MANAGER
} Authorization;


class cAccount
{
    private:    
        string sUserName;
        string sUserPassword;
        int nAuthorization;
        int ID;
        static int id;
    public:
        cAccount(string username, string password);
        string GetUserName();
        string GetPassWord();
        int GetAuthorization();
        void SetAuthorization(int authorization);

};