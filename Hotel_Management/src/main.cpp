#include<iostream>
#include<list>  

#include"cClient.h"
#include"cAccount.h"
#include"cRoom.h"
#include"cPaymentHistory.h"
#include"cRating.h"



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
        cout<< "[BOOK A ROOM]Xin moi lua chon: "; cin>>n;
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
    cout<<"[CHECK OUT]Choose a paymet method: "<<endl;
    cout<<"1. Credit card.\n";
    cout<<"2. Internet banking.\n";
    cout<<"0. Return.\n";
    cout<<"######################\n";
}
void CheckOut( vector<cRoom*> &vec, const int room_number,
        vector<cRating*> &vecRate, vector<cPaymentHistory*> &vecPay)
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
                cout<<"[CHECK OUT]Are you sure want to check out? y/n: ";
                string checkout_choice;
                cin>>checkout_choice;
                if(checkout_choice=="y"||checkout_choice=="yes")  
                {
                    DisplayBill(v);
                    cout<<"[CHECK OUT]Confirm payment: y/n: ";
                    string confirm_payment;
                    cin>>confirm_payment;
                    if(confirm_payment=="y"||confirm_payment=="yes")
                    {
                        cout<<"Payment is processing\n";
                        for(int i=0; i<20; i++) cout<<"#";
                        cout<<endl;
                        v->UpdateRoomStatus(FREE);
                        v->GetClient()->CheckOut();
                        cPaymentHistory* new_payment= new cPaymentHistory(v->GetClient(), v->GetClient()->GetTimeCheckOut(), 1000.0f);
                        vecPay.push_back(new_payment);
                        cout<<"Payment was successfully! Finished check out! \n";
                        
                        // Rating.
                        cout<<"Please give us some feed back! "<<endl;
                        string comment;
                        int rate;
                        cout<<"Input your stars: (1-5) "; cin>>rate;
                        cout<<"Leave a comment: "; cin>>comment;
                        cRating* new_rate= new cRating(rate, comment, v->GetClient());      
                        vecRate.push_back(new_rate);
                        cout<<"Thanks for your rating! <3\n";
                        
                        // Reset room state
                        v->SetClient(nullptr);
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
    string username="";
    string password="";
    int check_login=0;
    cout<<"[LOGIN]Input username: "; cin>>username;
    cout<<"[LOGIN]Input password: "; cin>>password;
    for(auto& l: listAccount)
    {
        if(l->GetPassWord()==password && l->GetUserName()==username && l->GetAuthorization()==MANAGER
        ||l->GetPassWord()==password && l->GetUserName()==username && l->GetAuthorization()==EMPLOYEE &&l->GetPosition()== RECEPTIONIST)
        {
            check_login++;
            return true;
        }
        else if(l->GetPassWord()==password && l->GetUserName()==username)
        {
            check_login++;
            cout<<"You don't have permission to login. \n";
            return false;
        } 
    }
    if(check_login==0) cout<<"Username or password is incorrect! Try again.\n";
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
    cout<<"[CLIENT MANAGEMENT/UPDATE]Input your choice: "; cin>>editChoice;

    switch(editChoice)
    {
        case 0: break ;
        case 1: 
            cout<<"[CLIENT MANAGEMENT/UPDATE]Input new name: "; cin>>client_name;
            client->SetName(client_name);
            cout<<"Name changed\n";
            break;
        case 2:
            cout<<"[CLIENT MANAGEMENT/UPDATE]Input new address: "; cin>>client_address;
            client->SetAddress(client_address);
            cout<<"Address changed. \n";
            break;
        case 3:
            cout<<"[CLIENT MANAGEMENT/UPDATE]Input new std: "; cin>>client_sdt;
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
    cout<<"[CLIENT MANAGEMENT/DELETE]Input client's sdt: "; cin>>client_sdt; 
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
        cout<<"[CLIENT MANAGEMENT]In put your choice: "; cin>>clientActionChoice;
        switch(clientActionChoice)
        {
            case 0: bExit= true; break;
            case 1: 
                DisplayClientInfo(listClient);
                cout<<"[CLIENT MANAGEMENT/UPDATE]Input client' sdt: ";
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

void ManagerAction(list<cAccount*>& list_acc)
{
    bool bManagerAction= true;
    string username, password;
    int check_manager_login=0;
    int manager_action;
    cout<<"Input manager's username: "; cin>>username;
    cout<<"Input manager's password: "; cin>>password;
    while(bManagerAction)
    {
        for(auto &l: list_acc)
        {
            if(l->GetPassWord()==password && l->GetUserName()==username &&l->GetAuthorization()==MANAGER)
            {
                check_manager_login++;
                int account_id;
                int check_account_exist=0;

                cout<<"\t"<<"___MANAGER ACTION___"<<endl;
                cout<<"1. Edit employee's position.\n";
                cout<<"2. Delete employee's account.\n";
                cout<<"3. Add employee's account.\n";
                cout<<"4. Display all employee.\n";
                cout<<"0. Return\n";
                cout<<"________________\n";
                cout<<"[MANAGER ACTION]Input your choice: "; cin>>manager_action;
                switch(manager_action)
                {
                    case 0: 
                        bManagerAction= false;
                        break;
                    case 1:
                        cout<<"[CHANGE POSITION]Input employee's ID: ";
                        cin>>account_id;
                        for(auto& l1: list_acc)
                        {
                            if(l1->GetID()==account_id)
                            {
                                check_account_exist++;
                                int new_position;
                                cout<<"[MANAGER ACTION]Input new position: "; cin>>new_position;
                                l1->SetPosition(new_position);
                                cout<<"[MANAGER ACTION]Employee's position has been changed! \n";
                            }
                        }
                        if(check_account_exist==0) cout<<"Employee not found!\n";
                        break;
                    case 2:
                        cout<<"[CHANGE AUTHORIZATION]Input employee's ID: ";
                        cin>>account_id;
                        for(auto& l1: list_acc)
                        {
                            if(l1->GetID()==account_id)
                            {
                                check_account_exist++;
                                l1->SetPosition(CLIENT);
                                cout<<"[MANAGER ACTION]This employee no longer works! \n";
                            }
                        }   
                        if(check_account_exist==0) cout<<"Employee not found!\n";
                        break;
                    case 3:
                        cout<<"[DELETE EMPLOYEE]Input account ID: "; cin>>account_id;
                        for(auto &l1: list_acc)
                        {
                            if(l1->GetID()== account_id)
                            {
                                check_account_exist++;
                                int new_position;
                                cout<<"[MANAGER ACTION]Input new position: "; cin>>new_position;
                                l1->SetAuthorization(EMPLOYEE);
                                l1->SetPosition(new_position);
                                cout<<"This user became employee of our hotel.\n";
                            }
                        }
                        if(check_account_exist==0) cout<<"Account ID not found!\n";
                        break;
                    case 4:
                        cout<<"[DISPLAY]________Employee list_______\n";
                        cout<<"ID"<<"\t"<<"Name"<<"\t\t"<<"Position"<<endl;
                        for(auto &l1: list_acc)
                            if(l1->GetAuthorization()==EMPLOYEE) 
                                cout<<l1->GetID()<<"\t"<<l1->GetUserName()<<"\t"<<l1->GetPosition()<<endl;
                        break;
                }
            }  
        }
        if(check_manager_login==0) 
        {
            cout<<"[MANAGER ACTION]Username or password is incorrect! \n";
            bManagerAction= false;
        }
    }
    
}



int main()
{   
    // Global variable========================================
    vector<cRoom*> vecRooms;
    list<cClient*> listClients;
    list<cAccount*> listAccounts;
    vector<cPaymentHistory*> vecPayments; 
    vector<cRating* > vecRates;

    bool bQuit= false;
    int n;

    // Set default states=====================================
    cClient* new_client= new cClient("Tien", "0347156920", "Hanoi");
    listClients.push_back(new_client);
    cClient* new_client1 = new cClient("Linh", "069690000", "ThaiBinh");
    listClients.push_back(new_client1);

    // Create some accounts;
    string username= "tiendepzai";
    string password= "hoilamgi";
    cAccount* manager_acc= new cAccountManager(username, password);
    manager_acc->SetAuthorization(MANAGER);
    listAccounts.push_back(manager_acc);

    string username1= "linhxinhgai";
    string password1= "hoilamgi";
    cAccount* receptionist= new cAccountEmployee(username1, password1, RECEPTIONIST);
    listAccounts.push_back(receptionist);

    string username2= "chidethuong";
    string password2= "hoidebiet";
    cAccountEmployee* user2 = new cAccountEmployee(username2, password2, ACCOUNTANT);
    listAccounts.push_back(user2);

    for(int i=0; i<10; i++)
    {
        int roomNumber= 100+i;
        cRoom* room = new cRoom(roomNumber);
        vecRooms.push_back(room);
    }

    // Create some default rating
    cRating* rate1= new cRating(5, "Tuyetvoi", new_client);
    cRating* rate2= new cRating(4,"Nhucut", new_client1);
    vecRates.push_back(rate1);
    vecRates.push_back(rate2);

    // Main program=================================================
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
        cout<<"[MAIN]Xin moi nhap lua chon: ";
        cin>> n;
        cout<<"_________________________________"<<endl;
        
        switch(n)
        {
            case 0:
                bQuit= true;
                break;
            case 1:
            cout<<"\t"<<"___BOOKING___"<<endl;
                BookARoom(vecRooms, listClients);
                break;
            case 2: 
                cout<<"\t"<<"CLIENT MANAGEMENT\n"<<endl;
                cout<<"1. Sign in\n";
                cout<<"2. Sign up\n";
                cout<<"0. Return. \n";
                int input2;
                cout<<"[CLIENT MANAGEMENT]Input your choice: ";
                cin>>input2;
                switch (input2)
                {
                    case 0: break;
                    case 1:
                        if(bLogin(listAccounts))
                            ClientManagement(listClients);
                }
                break;
                
            case 3: 
                cout<<"__________EMPLOYEE MANAGEMENT_________\n";
                ManagerAction(listAccounts);

                break;
            case 4: 
                cout<<"__________CHECK OUT___________\n"<<endl;
                cout<<"[CHECK OUT]Input room number: ";
                int room_number;
                cin>>room_number;
                CheckOut(vecRooms, room_number, vecRates, vecPayments);
                break;
            case 5: 
                break;
            case 6:
                break;
            case 7:
                cout<<"\t___FEEDBACK & RATING___\n";
                cout<<vecRates.size()<<endl;
                cout<<"RATING"<<"\t"<<"Cient"<<"\t"<<"Comment"<<"\t"<<"Time"<<endl;
                for(auto &v: vecRates)
                {
                    cout<<v->getClient()->GetClientName()<<"\t"<<v->getRate()<<"\t"<<
                    v->getComment()<<"\t"<<v->getDateTime()<<endl;
                    
                }

                break;
            default:
                cout<<"This feature is not available!\n";
                break;
        }
    }


    return 0;
}