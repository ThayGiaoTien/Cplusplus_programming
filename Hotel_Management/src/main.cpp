#include<iostream>
#include<list>  

#include"cClient.h"
#include"cAccount.h"

#include"cRoom.h"

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
    int checkBooking=0;
    cout<<"\t"<<"BOOK A ROOM"<<endl;
    cout<<endl;
    cout<<"1. Setup number of rooms.\n";
    cout<<"2. Update room's status. \n";
    cout<<"3. Booking. \n";
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
            for(auto& v : vec)
            {
                if(v->GetRoomNumber()== (room_number))
                {
                    checkBooking++;
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
                        int check_client_exist=false;
                        for(auto&l : listClient)
                        {
                            if(l->GetClientSDT()==client_sdt)
                            {
                                // Update client
                                check_client_exist++;
                                cout<<"Updated successfully!\n";
                            } 
                        }
                        if(check_client_exist==0)
                        {      
                            cClient* new_client= new cClient(client_name, client_sdt, client_address);
                            listClient.push_back(new_client);
                            v->SetClient(new_client);
                            cout<<"Client's info has been successfully added to list clients."<<endl;
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
            if(checkBooking==0) cout<<"Room not found! Try again. \n";
            break;

        default: 
            cout<<"Input not found!\n";
        }
    }
}
void DisplayBill( cRoom* &room)
{
    cout<<"\t"<<"___BILL___"<<endl;
    cout<<"Name: "<< "\t"<<room->GetClient()->GetClientName()<<endl;
    cout<<"SDT: "<<"\t"<<room->GetClient()->GetClientSDT()<<endl;
    cout<<"Address: "<<"\t"<<room->GetClient()->GetClientAddress()<<endl;
    cout<<"Check in: "<<"\t"<<room->GetClient()->GetTimeCheckIn();
    room->GetClient()->SetTimeCheckOut();
    cout<<"Check out: "<<"\t"<<room->GetClient()->GetTimeCheckOut()<<endl;
    cout<<"\t###"<<" Summary: "<< 1000 <<"$." <<endl;
}
void CheckOut( vector<cRoom*> &vec, const int room_number)
{   
    bool bCheckedOut= false;
    while(!bCheckedOut)
    {
        int check_room_found=0;
        for(auto &v: vec)
        {
            if(room_number==v->GetRoomNumber())
            {
                if(v->IsRoomAvailable()) 
                {
                    cout<<"This room have been already checked out! \n";
                    return;
                }
                check_room_found++;
                cout<<"Room"<<"\t"<<"Status"<<"\t"<<"Client"<<"\t"<<"SDT"<<endl;
                cout<<room_number<<"\t"<<((v->IsRoomAvailable())? "V" : "X")
                <<"\t"<<v->GetClient()->GetClientName()
                <<"\t"<<v->GetClient()->GetClientSDT()<<endl;
                cout<<"Are you sure want to check out? y/n: ";
                string checkout_choice;
                cin>>checkout_choice;
                if(checkout_choice=="y"||checkout_choice=="yes")  
                {
                    DisplayBill(v);
                    cout<<"Confirm payment: y/n: ";
                    string confirm_payment;
                    cin>>confirm_payment;
                    if(confirm_payment=="y"||confirm_payment=="yes")
                    {
                        cout<<"Payment is processing\n";
                        for(int i=0; i<20; i++) cout<<".";
                        cout<<endl;
                        v->UpdateRoomStatus(FREE);
                        v->GetClient()->CheckOut();
                        v->SetClient(nullptr);
                        cout<<"Payment was successfully! Finished check out! \n";
                        bCheckedOut= true;
                    } else "Payment wasn't successfully! Try again!\n";
                }
                else cout<<"Check out unsuccesfully! Try again! \n";
            }
        }
        if(check_room_found==0) cout<<"Room not found! \n";
    }
    
}

bool bLogin(list<cAccount*> &listAccount)
{
    string username;
    string password;
    cout<<"Input username: "; cin>>username;
    cout<<"Input password: "; cin>>password;
    for(auto& l: listAccount)
    {
        if(l->GetPassWord()==password && l->GetUserName()==username && l->GetAuthorization()==MANAGER)
            return true;
        else if(l->GetPassWord()==password && l->GetUserName()==username)
        {
            cout<<"You don't have permission to login. \n";
            return false;
        }
    }
    return false;
}

void DisplayClientInfo(list<cClient*> &listClient)
{
    cout<<"\t"<<"Client Info\n"<<endl;
    cout<<"Name"<<"\t\t"<<"SDT"<<"\t\t\t"<<"Address"<<"\t\t"<<"History"<<endl;
    for(auto&l: listClient)
        cout<<l->GetClientName()<<"\t\t"<<l->GetClientSDT()<<"\t\t"
        <<l->GetClientAddress()<<"\t\t"<<l->GetTimeCheckIn()<<endl;

}
void UpdateClientInfo(cClient* &client)
{
    cout<<"_______________\n";
    cout<<"1. Edit name\n";
    cout<<"2. Edit address\n";
    cout<<"3. Edit sdt\n";
    cout<<"0. Return.\n";
    int editChoice;
    string client_name;
    string client_address;
    string client_sdt;
    cout<<"Input your choice: "; cin>>editChoice;

    switch(editChoice)
    {
        case 0: break ;
        case 1: 
            cout<<"Input new name: "; cin>>client_name;
            client->SetName(client_name);
            cout<<"Name changed\n";
            break;
        case 2:
            cout<<"Input new address: "; cin>>client_address;
            client->SetAddress(client_address);
            cout<<"Address changed. \n";
            break;
        case 3:
            cout<<"Input new std: "; cin>>client_sdt;
            client->SetSDT(client_sdt);
            cout<<"SDT changed.\n";
            break;
    }
}

void DeleteClientInfo(list<cClient*> &listClient)
{
    string client_sdt;
    int checkClientDelete=0;
    DisplayClientInfo(listClient);
    cout<<"Input client's sdt: "; cin>>client_sdt; 
    list<cClient*>::iterator it;
    for(it= listClient.begin(); it!= listClient.end(); ++it)
    {
        if((*it)->GetClientSDT()== client_sdt)
        {
            checkClientDelete++;
            cout<<"Are you sure want to delete? y/n\n";
            string confirm;
            cin>>confirm;
            if(confirm=="y" || confirm=="yes")
            {
                listClient.erase(it);
                cout<<"Delete successfully!\n";
                break;
            }
            else cout<<"Delete unsuccessfully! \n";
        }
        //return  ;
    }
    if(checkClientDelete==0) cout<<"Not found this sdt! Try again. \n";
}
void ClientManagement(list<cClient*> &listClient)
{
    bool bExit= false;
    DisplayClientInfo(listClient);
    cout<<"________________________\n";
    cout<<"1. Edit client's info.\n";
    cout<<"2. Delete client's info.\n";
    cout<<"0. Return.\n";
    int clientActionChoice;
    string clientSDT;
    int checkClientInfo=0;

    while(!bExit)
    {
        cout<<"In put your choice: "; cin>>clientActionChoice;
        switch(clientActionChoice)
        {
            case 0: bExit= true;
            case 1: 
                DisplayClientInfo(listClient);
                cout<<"Input client' sdt: ";
                cin>>clientSDT;
                for(auto& l: listClient)
                {
                    if(l->GetClientSDT()==clientSDT)
                    {
                        checkClientInfo++;
                        UpdateClientInfo(l);
                        break;
                    }
                }
                if(checkClientInfo==0) cout<<"Client not found! Try again. \n";
                break;
            case 2: 
                DeleteClientInfo(listClient);
                break;
        }
    }
}




int main()
{   
    // Global variable
    vector<cRoom*> vecRooms;
    list<cClient*> listClients;
    list<cAccount*> listAccounts;

    bool bQuit= false;
    int n;

    cClient* new_client= new cClient("Tien", "0347156920", "Hanoi");
    listClients.push_back(new_client);
    cClient* new_client1 = new cClient("Linh", "069690000", "ThaiBinh");
    listClients.push_back(new_client1);


    // Create manager account;
    string username= "tiendepzai";
    string password= "hoilamgi";
    cAccount* manager_acc= new cAccount(username, password);
    manager_acc->SetAuthorization(MANAGER);
    listAccounts.push_back(manager_acc);
    string username1= "linhxinhgai";
    string password1= "hoilamgi";
    cAccount* manager_acc1= new cAccount(username1, password1);
    listAccounts.push_back(manager_acc1);

   

    cout<<"\t"<<"_______HOTEL MANGAGEMENT______"<<endl;
    cout<<endl;
    cout<<"1. Book a room. \n";
    cout<<"2. Client management. \n";
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
                cout<<"\t"<<"CLIENT MANAGEMENT\n"<<endl;
                cout<<"1. Sign in\n";
                cout<<"2. Sign up\n";
                cout<<"0. Return. \n";
                int input2;
                cout<<"Input your choice: ";
                cin>>input2;
                switch (input2)
                {
                    case 0: break;
                    case 1:
                        if(bLogin(listAccounts))
                        {
                            ClientManagement(listClients);

                        }
                        else cout<<"Username or password is incorrect! Try again!\n";
                }
                break;
                
            case 3: 
                cout<<"__________EMPLOYEE MANAGEMENT_________\n";
                break;
            case 4: 
                cout<<"__________CHECK OUT___________\n"<<endl;
                cout<<"Input room number: ";
                int room_number;
                cin>>room_number;
                CheckOut(vecRooms, room_number);
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