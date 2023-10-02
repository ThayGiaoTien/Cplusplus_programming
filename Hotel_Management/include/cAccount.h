#pragma once
#include<iostream>
#include<list>
using namespace std;

typedef enum
{
    CLIENT, 
    EMPLOYEE,
    MANAGER,
} Authorization;

typedef enum{
    USER,
    RECEPTIONIST,
    CLEANING_STAFF,
    ACCOUNTANT
} Position;

// Base class
class cAccount
{
    protected:    
        string sUserName;
        string sUserPassword;
        string sSDT;
        int nPosition;
        int nAuthorization;
        int nUserID;
        static int id;
    public:
        cAccount(string username, string password);
        string GetUserName();
        string GetPassWord();
        int GetAuthorization();
        int GetID();
        int GetPosition();

        void EditPassWord(string new_password);
        void EditSDT(string new_sdt);
        virtual void SetAuthorization(int authorization)=0;
        virtual void SetPosition(int position)=0;
};

class cAccountUser: public cAccount
{
    public:
        cAccountUser(string username, string password);
        void SetAuthorization(int authorization) override;
        void SetPosition(int position) override;
};

class cAccountEmployee: public cAccount
{
    public:
        cAccountEmployee(string username, string password, int position);
        void SetPosition(int position) override;
        void SetAuthorization(int authorization) override;
};

class cAccountManager: public cAccount
{
    public:
        cAccountManager(string username, string password);
        void EditEmployeeAccount(int employee_id, list<cAccount*> &list_acc, int position);
        void DeleteEmployeeAccount(int employee_id, list<cAccount*> &list_acc);
        void SetNewEmployeeAccount(int user_id, list<cAccount*> &list_acc);
        void SetPosition(int position) override;
        void SetAuthorization(int position) override;
};
