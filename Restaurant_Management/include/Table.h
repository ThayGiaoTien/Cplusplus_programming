#include"MonAn.h"
#include<list>


// typedef enum{
//     FREE,
//     ORDER,
//     PAY
// } TableStatus; 

class Table
{
    private:
        int ID;
        list<pair<MonAn*, int>> listGoiMon;
        int nStatus;
        int static id;

    public:
        Table();
        int GetStatus();
        void SetStatus(int status);
        int GetID();
        void GoiThemMon(MonAn* mon, int soluong);
        void XoaBotMon(MonAn* mon);
        void Display();
        void ThanhToan();
};   