#include"cAccount.h"

int cAccount::id= 100;

cAccount::cAccount(string username, string password)
{
    nUserID= id++;
    sUserName= username;
    sUserPassword= password;
    nAuthorization= CLIENT;
    nPosition= USER;
}

string cAccount::GetUserName()
{
    return sUserName;
}
string cAccount::GetPassWord()
{
    return sUserPassword;
}
int cAccount::GetAuthorization()
{
    return nAuthorization;
}
int cAccount::GetID()
{
    return nUserID;
}
int cAccount::GetPosition()
{
    return nPosition;
}
void cAccount::EditPassWord(string new_password)
{
    sUserPassword= new_password;
}
void cAccount::EditSDT(string new_sdt)
{
    sSDT= new_sdt;
}

// Class cAccountUser==========================
// Just override two virtual functions
cAccountUser::cAccountUser(string username, string password) : cAccount(username, password){}
void cAccountUser::SetAuthorization(int authorization){}
void cAccountUser::SetPosition(int position) {}


// Class cAccountEmployee========================
cAccountEmployee::cAccountEmployee(string username, string password, int position): cAccount(username, password)
{
    //sUserName= username;
    //sUserPassword= password;
    nPosition= position;
    nAuthorization= EMPLOYEE;
}

void cAccountEmployee::SetPosition(int position)
{
    nPosition= position;
}
void cAccountEmployee::SetAuthorization(int authorization)
{
    nAuthorization= authorization;
}


// Class cAccountManager=============================
cAccountManager::cAccountManager(string username, string password) : cAccount(username, password)
{
    nAuthorization= MANAGER;
}
void cAccountManager::EditEmployeeAccount(int employee_id, list<cAccount*> &list_acc, int position)
{
    for(auto &l: list_acc)
    {
        if(l->GetID()== employee_id) l->SetPosition(position);
    }
}
void cAccountManager::DeleteEmployeeAccount(int employee_id, list<cAccount*> &list_acc)
{
    list<cAccount*>::iterator it;
    for(it=list_acc.begin(); it!= list_acc.end(); ++it)
    {
        if((*it)->GetID()==employee_id) (*it)->SetAuthorization(CLIENT);
    }
}
void cAccountManager::SetNewEmployeeAccount(int user_id, list<cAccount*> &list_acc)
{
    for(auto& l: list_acc)
    {
        if(l->GetID()== user_id) l->SetAuthorization(EMPLOYEE);
    }
}
void cAccountManager::SetPosition(int position) {}
void cAccountManager::SetAuthorization(int position) {}

