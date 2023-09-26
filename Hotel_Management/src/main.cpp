#include<iostream>
#include<list>  

#include"cClient.h"
#include"cManager.h"

//#include"cRoom.h"

using namespace std;

void DisplayRoomStatus(const vector<cRoom*> &vec)
{
    cout<<"Room"<<"\t";
    for(auto&v: vec) cout<<v->GetRoomNumber()<<"\t";
    cout<<endl;

    cout<<"Status"<<"\t";
    for(auto &v: vec) (v->IsRoomAvailable()) ? (cout<<"V"<<"\t") : (cout<<"X"<<"\t");
    cout<<endl;
}
void BookARoom(vector<cRoom*> &vec, list<cClient*> &listClient)
{
    bool bExit= false;
    int n;
    cout<<"\t"<<"BOOK A ROOM"<<endl;
    cout<<endl;
    cout<<"1. Setup number of rooms.\n";
    cout<<"2. Update room's status. \n";
    cout<<"3. Booking. \n";
    cout<<"4. Check out.\n";
    cout<<"0. To back to main menu. \n";
    
    while(!bExit)
    {
        cout<<"_________________________________"<<endl;
        cout<< "Xin moi lua chon: "; cin>>n;
        switch (n)
        {
        case 0:
            bExit= true;
            break;
        case 1: 
            cout<<"Input number of rooms: \n"<<endl;
            int number;
            cin>>number;
            vec.clear();
            for(int i=0; i<number; i++)
            {
                int roomNumber= 100+i;
                cRoom* room = new cRoom(roomNumber);
                vec.push_back(room);
            }
            cout<<number<<" rooms has been setup! \n";
            break;

        case 2: 
            cout<<"\t"<<"Update room's status.\n"<<endl;
            DisplayRoomStatus(vec);
            break;

        case 3:
            cout<<"\t"<<"Booking\n"<<endl;
            DisplayRoomStatus(vec);
            cout<<"_____________\n";
            cout<<"Input room number: ";
            int room_number;
            cin>> room_number;
            // Check whether room is available for booking.
            int check=0;
            for(auto& v : vec)
            {
                if(v->GetRoomNumber()== (room_number))
                {
                    check++;
                    if(v->IsRoomAvailable())
                    {
                        cout<<"Romm is available, input client's info: \n";
                        string client_name;
                        string client_address;
                        string client_sdt;
                        cout<<"Input name: "; cin>>client_name;
                        cout<<"Input sdt: "; cin>>client_sdt;
                        cout<<"Input address: "; cin>>client_address;

                        // If client exist in listClient, update client.
                        // Otherwise, create new client.
                        for(auto&l : listClient)
                        {
                            if(l->GetClientSDT()==client_sdt)
                            {
                                // Update client
                                cout<<"Updated successfully!\n";
                            }
                            else 
                            {
                                cClient* new_client= new cClient(client_name, client_sdt, client_address);
                                listClient.push_back(new_client);
                                cout<<"Client's info has been successfully added to list clients."<<endl;
                            }   
                        }
                        // Once booked, set room status to not available.
                        cout<<"Booked successfully! \n";
                        v->UpdateRoomStatus(BOOKED);
                        break;
                    }
                    else
                    {
                        cout<<"Sorry, this room is not available now! Try another, please! "<<endl;
                        break;
                    } 
                }
            }
            if(check==0) cout<<"Room not found! Try again. \n";
            break;
        default: 
            cout<<"Input not found!\n";
        }
    }
    




}


int main()
{   
    // Global variable
    vector<cRoom*> vecRooms;
    list<cClient*> listClients;
     bool bQuit= false;
    int n;

    cClient* new_client= new cClient("Tien", "0347156920", "PhuTho");
    listClients.push_back(new_client);

   

    cout<<"\t"<<"_______HOTEL MANGAGEMENT______"<<endl;
    cout<<endl;
    cout<<"1. Book a room. \n";
    cout<<"2. Manage clients. \n";
    cout<<"3. Manage Employee. \n";
    cout<<"4. Check out. \n";
    cout<<"5. Security and access.\n";
    cout<<"6. Reports and statistics. \n";
    cout<<"7. Feedback and ratings.\n";           
    cout<<"0. To quit. \n";

    while(!bQuit)
    {
        cout<<"Xin moi nhap lua chon: ";
        cin>> n;
        cout<<"_________________________________"<<endl;
        
        switch(n)
        {
            case 0:
                bQuit= true;
                break;
            case 1:
                BookARoom(vecRooms, listClients);
                break;
            case 2: 
                
                break;
            case 3: 

                break;
            case 4: 

                break;
            case 5: 
                
                break;
            case 6:

                break;
            case 7:

                break;
        }
    }


    return 0;
}