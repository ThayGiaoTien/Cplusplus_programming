#include <iostream>
#include<list>
#include<vector>
#include<utility>

#include"Table.h"
//#include"MonAn.h"


using namespace std;
typedef enum{
    FREE,
    ORDER,
    PAY
} TableStatus;

// UTILITIES====================================

void DisplayMonAn(MonAn* mon)
{
    cout<<"ID"<<"\t"<<"|"<<"\t"<<"Gia"<<"\t"<<"|"<<"\t"<<"Ten Mon"<<endl;
    cout<<mon->GetID()-100<<"\t"<<"|"<<"\t"<<mon->GetPrice()<<"\t"<<"|"<<"\t"<<mon->GetName()<<endl;
}

void DisplayMenu(list<MonAn*> &list)
{
    cout<<"ID"<<"\t"<<"|"<<"\t"<<"Gia"<<"\t"<<"|"<<"\t"<<"Ten Mon"<<endl;
    for(auto &l : list)
    {
        cout<<l->GetID()-100<<"\t"<<"|"<<"\t"<<l->GetPrice()<<"\t"<<"|"<<"\t"<<l->GetName()<<endl;
    }
}

void ThemMonAn(list<MonAn*> &list)
{
    bool bExit=false;
    while(!bExit)
    {
        cout<<"Moi ban nhap mon an moi:\n";
        cout<<"Nhap exit de quay lai"<<endl;
        cout<<"______________________\n";
        string name;
        if(name=="exit") bExit= true;
        float price;
        cout<<"1. Nhap ten: ";
        cin>>name;
        for(auto &l:list)
        {
            if(l->GetName() == name)
            {
                cout<<"Mon an da ton tai. Vui long thu lai! \n";\
                return ;
            }
        }
        cout<<"2. Nhap gia: ";
        cin>>price;
        MonAn* monMoi= new MonAn(price, name);
        list.push_back(monMoi);
        cout<<"Mon an moi da duoc them vao danh sach."<<endl;
        DisplayMonAn(monMoi);
        cout<<"______________________\n";
        bExit= true;
    }
    
}
void SuaThongTin(list<MonAn*> &list)
{
    bool bExit= false;
    while(!bExit)
    {
        cout<<"Xin hay nhap ten mon an ban muon sua:\n";
        cout<<"Nhap exit de quay lai"<<endl;
        cout<<"______________________\n";
        string name;
        if(name=="exit") bExit= true;
        cin>>name;
        for(auto &l: list)
        {
            if(l->GetName()== name)
            {
                bool bExist= true;
                string n;
                do
                {
                    cout<<"Ten moi: ";
                    cin>>n;
                    for(auto &k: list)
                    {
                        if(k->GetName() == n && k->GetID()!= l->GetID())
                        {
                            cout<<"Ohh, ten mon an nay da ton tai trong menu. Vui long thu lai.\n";
                            cout<<"__________________________"<<endl;
                            bExist= true;
                        }
                        else bExist= false;
                    }
                } while(bExist);
                cout<<"Gia moi: ";
                float price;
                cin>>price;
                l->UpdateMonAn(price, n);
                cout<<"Mon an da duoc thay doi thong tin thanh cong!"<<endl;
                DisplayMonAn(l);
                bExit= true;
            }
            else 
            {
                cout<<"Ohh, khong tim thay ten mon an nay!! Xin vui long thu lai! "<<endl;
                cout<<"Nhap 0 de quay lai. "<<endl;
                int n;
                cin>>n;
                if(n==0) bExit= true;
            }
        }
    }
    cout<<"______________________\n";
}

void XoaMonKhoiMenu(list<MonAn*> &listMenu)
{   
    bool bExit= false;
    while(!bExit)
    {
        cout<<"Xin hay nhap ten mon an ban muon xoa:\n";
        cout<<"Nhap exit de quay lai"<<endl;
        cout<<"______________________\n";
        string name;
        cin>>name;
        if(name=="exit") bExit= true;
        list<MonAn*>::iterator it;
        for(it= listMenu.begin(); it!=listMenu.end(); ++it )
        {
            if((*it)->GetName()==name)
            {
                string confirm;
                cout<<"Ban co chac chan muon xoa mon an nay? y/n"<<endl;
                cin>>confirm;
                if(confirm=="y")
                {
                    listMenu.erase(it);
                    cout<<"Mon an '"<<name<<"' da duoc xoa khoi menu thanh cong!"<<endl;
                    bExit= true;
                }
                else cout<<"Mon an nay chua duoc xoa! "<<endl;
            }
            else cout<<"Khong tim thay mon an. Vui long thu lai! "<<endl;
        }
    }
     cout<<"______________________\n";
}

void DisplayTables(vector<Table*> vec)
{
    cout<<"\t"<<"TABLES"<<endl;
    cout<<"BAN"<<"\t";
    for(int i=0; i<vec.size(); i++)
    {
        cout<<vec[i]->GetID()<<"\t";
    }
    cout<<endl;
    cout<<"Status"<<"\t";
    for(auto &v: vec)
    {   
        int n= v->GetStatus();
        string status;
        switch(n)
        {
            case 0: 
                status= "FREE";
                break;
            case 1: 
                status= "ORDER";
                break;
            case 2:
                status= "PAY";
                break;
        }
        cout<<status<<"\t";
    }
    cout<<endl;
    cout<<"______________________\n";
}

void GoiThemMon(list<MonAn*> &listMenu, vector<Table*> &vec, int soban)
{
    vec[soban]->SetStatus(1);
    cout<<"Xin moi ban nhap ten mon an: ";
    string name;
    cin>>name;
    cout<<"Moi nhap so luong: ";
    int soluong;
    cin>>soluong;
    list<MonAn*>::iterator it;
    for(it= listMenu.begin(); it!=listMenu.end(); ++it )
    {
        if((*it)->GetName() == name)
        {
            vec[soban]->GoiThemMon(*it, soluong);
            cout<<"Mon an "<< name<< " da duoc them thanh cong. So luong: "<<soluong<<endl;
        }
        else 
        {
            cout<<"Ten mon an khong dung. Thu lai! "<<endl;
        }
    }
}
void XoaBotMon(list<MonAn*> &listMenu, vector<Table*> &vec, int soban)
{
    cout<<"Xin moi ban nhap ten mon an: ";
    string name;
    cin>>name;
    list<MonAn*>::iterator it;
    for(it= listMenu.begin(); it!=listMenu.end(); ++it )
    {
        if((*it)->GetName() == name)
        {
            vec[soban]->XoaBotMon(*it);
            cout<<"Mon an "<< name<< " da duoc xoa thanh cong." <<endl;
        }
        else cout<<"Ten mon an khong dung. Thu lai! "<<endl;
    }
}


int main()
{
    // Write C++ code here
    list<MonAn*> listMonAn;
    vector<Table*> vecTables;
    vecTables.reserve(2);
    
    int nSoBan=0;

    MonAn* mon1= new MonAn(90, "com");
    MonAn* mon2= new MonAn(100, "pho");

    listMonAn.push_back(mon1);
    listMonAn.push_back(mon2);

    
    while(1)
    {
        int input;
       
        cout<<"----------------------------"<<endl;
        cout<<"\t"<<"1. Manager"<<endl;
        cout<<"\t"<<"2. Staff"<<endl;
        cout<<"----------------------------"<<endl;
        cout<<"Xin moi nhap lua chon: ";
        
        cin>>input;
        if(input==1)
        {
            cout<<"\t"<<"MANAGER"<<endl;
            cout<<"____________________"<<endl;
            cout<<"1. Set so ban.\n";
            cout<<"2. Them mon va menu. \n";
            cout<<"3. Sua thong tin mon. \n";
            cout<<"4. Xoa mon khoi menu. \n";
            cout<<"5. Danh sach mon. \n";
            cout<<"0. To get back to main menu.\n";
            cout<<"____________________"<<endl;

            bool bExit= false;
            while(!bExit)
            {
                int n;
                cout<<"\t"<<"MANAGER"<<endl;
                cout<<"Xin moi nhap lua chon: ";
                cin>> n;
                switch (n)
                {
                case 0:
                    bExit= true;    
                    break;
                case 1:
                    cout<<"Nhap vao so ban: ";
                    cin>>nSoBan;
                    if(nSoBan>0)
                    {
                        vecTables.reserve(nSoBan);
                        for(int i=0; i<nSoBan; i++)
                        {
                            Table* newTable= new Table();
                            vecTables.push_back(newTable);
                        }
                        cout<<"Thiet lap so ban thanh cong! \n";
                    } 
                    else cout<<"So ban khong hop le. Vui long thu lai. "<<endl;
                    break;
                case 2:
                    ThemMonAn(listMonAn);
                    break;
                case 3:
                    SuaThongTin(listMonAn);
                    break;
                case 4:
                    XoaMonKhoiMenu(listMonAn);
                    break;
                case 5:
                    DisplayMenu(listMonAn);
                    break;
                default:
                    break;
                }
            }
        }
        else if(input ==2)
        {
            cout<<"\t"<<"STAFF"<<endl;
            cout<<"____________________"<<endl;
            int ban;
            int luachon;
            bool bExit= false;

            DisplayTables(vecTables);

            cout<<"Xin moi ban chon ban: ";
            cin>>ban;
            cout<<"____________________"<<endl;
            cout<<"\t"<<"BAN "<<ban<<endl;

            cout<<"____________________"<<endl;
            cout<<"1. Them mon. \n";
            cout<<"2. Xoa mon \n";
            cout<<"3. Danh sach cac mon da order. \n";
            cout<<"4. Thanh toan. \n";
            cout<<"0. To get back to main menu.\n";

            while (!bExit)
            {
                cout<<"____________________"<<endl;
                cout<<"Moi ban nhap lua chon: ";
                cin>>luachon;

                switch(luachon)
                {
                    case 0:
                        bExit= true;
                        break;
                    case 1:
                        DisplayMenu(listMonAn);
                        GoiThemMon(listMonAn, vecTables, ban);
                        break;  
                    case 2:
                        DisplayMenu(listMonAn);
                        XoaBotMon(listMonAn, vecTables, ban);
                        break;
                    case 3:
                        cout<<"\t"<<"Danh sach cac mon da order: "<<endl;
                        vecTables[ban]->Display();
                        break;
                    case 4:
                        vecTables[ban]->SetStatus(PAY);
                        vecTables[ban]->Display();
                        vecTables[ban]->ThanhToan();

                        cout<<"Ban co chac chan muon thanh toan? (y/n)"<<endl;
                        string confirm;
                        cin>>confirm;
                        if(confirm=="y"|| confirm=="yes")
                        {
                            cout<<"Cam on quy khach, xin hen gap lai! "<<endl;
                            vecTables[ban]->SetStatus(FREE);
                        }
                        else cout<<"Ban chua thanh toan. Vui long thu lai sau!"<<endl;
                }
            }
        }
        else cout<<"Ban nhap sai roi, vui long thu lai di! "<<endl;
    }
    
    return 0;
}