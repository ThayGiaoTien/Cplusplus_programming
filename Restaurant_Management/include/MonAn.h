
#include<iostream>

using namespace std;

class MonAn
{
    private: 
        int ID;
        float fPrice;
        string sName;
         static int id;
    public:
        MonAn(float price, string name);
        void UpdateMonAn(float price, string name);
       
        string GetName();
        float GetPrice();
        int GetID();
        
};
