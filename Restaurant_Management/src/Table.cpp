
#include"Table.h"   

typedef enum{
    FREE,
    ORDER,
    PAY
} TableStatus;

Table::Table()
{
    ID= id++;
    nStatus= 0;
    cout<<"Table "<<this->GetID()<< " da duoc tao.\n";
}

int Table::GetStatus()
{
    return Table::nStatus;
}
void Table::SetStatus(int n)
{
    Table::nStatus= n;
}
int Table::GetID()
{
    return Table::ID;
}
void Table::GoiThemMon(MonAn* mon, int soluong)
{
    listGoiMon.push_back(make_pair(mon, soluong));
}

void Table::XoaBotMon(MonAn* mon)
{
    list<pair<MonAn*, int>>::iterator it;
    for(it= listGoiMon.begin(); it!= listGoiMon.end(); ++it)
    {
        if(mon->GetName()==(*it).first->GetName())
        {
            listGoiMon.erase(it);
        }
        else
        {
            cout<<"Khong tim thay mon an can xoa. Vui long thu lai."<<endl;
        }
    }
}
void Table::Display()
{
    cout<<"ID"<<"\t"<<"|"<<"\t"<<"Gia"<<"\t"<<"|"<<"\t"<<"Ten Mon"<<"\t"<<"|"<<"\t"<<"So luong"<<endl;
    for(auto &l : listGoiMon)
    {
        cout<<l.first->GetID()<<"\t"<<"|"<<"\t"<<l.first->GetPrice()<<"\t"<<"|"<<"\t"<<l.first->GetName()
        <<"\t"<<"|"<<"\t"<<l.second<<endl;
    }
}
void Table::ThanhToan()
{
    float sum= 0;
    for(auto &l: listGoiMon)
    {
        sum += l.first->GetPrice()*l.second;
    }
    cout<<"Gia tri don hang: "<<sum<<" VND"<<endl;
    cout<<"VAT: 10%."<<endl; 
    cout<<"Tong so tien can thanh toan: "<< sum*1.1f <<" VND."<<endl;
}

int Table::id = 0;