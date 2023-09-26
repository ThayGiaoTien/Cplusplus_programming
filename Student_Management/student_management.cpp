#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<iomanip>  
#include<fstream>
#include<algorithm>


typedef enum{
    NAM, NU, UNKNOWN
} Sex;

typedef enum{
    GIOI,
    KHA,
    TRUNG_BINH,
    YEU
} HocLuc;

class cStudent;
std::list<cStudent*> listStudents; 

class cStudent
{
private:
    int ID;
    std::string sName;
    int nAge;
    int sex;
    float fDiemToan;
    float fDiemVan;
    float fDiemAnh;

public:
    static int id;
    cStudent(std::string name, int tuoi, int s, float toan, float van, float anh)
    {   
        ID= id;
        id++;
        sName= name;
        nAge= tuoi;
        sex= s;
        fDiemToan= toan;
        fDiemVan= van;
        fDiemAnh= anh;
    }
    void Update(std::string name, int tuoi, int s, float toan, float van, float anh)
    {
        sName= name;
        nAge= tuoi;
        sex= s;
        fDiemToan= toan;
        fDiemVan= van;
        fDiemAnh= anh;
    }

    // Methods to get properties==========================================
    int GetID()
    {
        return this->ID;
    }
    std::string GetName()
    {
        return this->sName;
    }
    int GetAge()
    {
        return this->nAge;
    }
    int GetSex()
    {
        return this->sex;
    }
    float GetDiemToan() 
    {
        return this->fDiemToan;
    }
    float GetDiemVan()
    {
        return this->fDiemVan;
    }
    float GetDiemAnh()
    {
        return this->fDiemAnh;
    }

    float GetAvaragePoint()
    {
        return (fDiemAnh+fDiemToan+fDiemVan)/3.0f;
    }
    HocLuc GetHocLuc()
    {
        float fAvr= GetAvaragePoint();
        if(fAvr>=9.0f)   return GIOI;
        else if(fAvr>= 7.0f && fAvr<9.0f)   return KHA;
        else if(fAvr>=5.0f && fAvr<7.0f) return TRUNG_BINH;
        else return YEU;
    }

};


int cStudent::id= 1000;


void AddStudent(std::list<cStudent*> &list)
{
    std::string name;
    int tuoi;
    Sex sex;
    int key;
    float diemToan, diemVan, diemAnh;

    std::cout<<"Input name: ";
    std::cin>>name; 

    std::cout<<"Input tuoi: ";
    while(1)
    {
        std::cin>>tuoi;
        if(tuoi>=18 && tuoi<50) break;
        else std::cout<<"tuoi must in range 18-50\n";
    }
    std::cout<<"Sex: (0 - Male/ 1 - Female) ";
    std::cin>>key;
    
    switch (key)
    {
    case 0:
        sex= NAM;
        break;
    case 1: 
        sex= NU;
        break;
    default:
        sex= UNKNOWN;
        break;
    }

    std::cout<<"Diem toan: ";
    std::cin>>diemToan;

    std::cout<<"Diem van: ";
    std::cin>>diemVan;

    std::cout<<"Diem anh: ";
    std::cin>>diemAnh;

    cStudent* newStudent= new cStudent(name, tuoi, sex, diemToan, diemVan, diemAnh);
    listStudents.push_back(newStudent);
    //delete newStudent;


}
void DisplayInfo(cStudent* student)
{
    std::string s;
    if(student->GetSex()==0) s= "Nam";
    else if(student->GetSex()==1) s= "Nu";
    else s="Unknown";

    std::string hl;
    switch (student->GetHocLuc())
    {
    case GIOI:
        hl= "Gioi";
        break;
    case KHA:
        hl= "Kha";
        break;
    case TRUNG_BINH:
        hl= "Trung Binh";
        break;
    case YEU:
        hl= "Yeu";
        break;

    }
    std::cout<<(student->GetID()-1000)<<"\t"<<student->GetName()<<"\t"<<(student->GetAge())<<"\t"<<s
        <<"\t"<< std::fixed << std::setprecision(2)<<(student->GetDiemToan())<<"\t"
        << std::fixed << std::setprecision(2)<<(student->GetDiemVan())<<"\t"
        << std::fixed << std::setprecision(2)<<(student->GetDiemAnh())<<"\t"
        << std::fixed << std::setprecision(2)<<(student->GetAvaragePoint())<<"\t"<<"\t"
        <<hl<<"\n";
}

void UpdateInfo(std::list<cStudent*> &list)
{
    // Find the student by id, edit its infor.
    std::cout<<"Input student ID: ";
    int student_id;
    std::cin>>student_id;
    int check=0;
    std::list<cStudent*>::iterator  it;
    for(it= list.begin(); it!= list.end(); ++it)
    {
        if((*it)->GetID()== student_id+1000)
        {
            check++;
            int n;
            std::string sName= (*it)->GetName();
            int nAge= (*it)->GetAge();
            int sex= (*it)->GetSex();
            float fDiemToan= (*it)->GetDiemToan();
            float fDiemVan= (*it)->GetDiemVan();
            float fDiemAnh = (*it)->GetDiemAnh();

            std::cout<<"1. Update name."<<std::endl;
            std::cout<<"2. Update age."<<std::endl;
            std::cout<<"3. Update sex."<<std::endl;
            std::cout<<"4. Update diem toan."<<std::endl;
            std::cout<<"5. Update diem van."<<std::endl;
            std::cout<<"6. Update diem anh."<<std::endl;
            std::cout<<"7. Delete student."<<std::endl;
            std::cout<<"0. To exit."<<std::endl;
            while(1)
            {
                std::cout<<"Input your choice: ";
                std::cin>> n;
                if(n==0) break;
                switch(n)
                {
                    case 1:
                        std::cout<<"Input new name: ";
                        std::cin>>sName;
                        break;
                    case 2:
                        std::cout<<"Input new age: ";
                        std::cin>>nAge;
                        break;
                    case 3:
                        std::cout<<"Input new sex: ";
                        std::cin>>sex;
                        break;
                    case 4:
                        std::cout<<"Input new diem toan: ";
                        std::cin>>fDiemToan;
                        break;
                    case 5:
                        std::cout<<"Input diem van: ";
                        std::cin>>fDiemVan;
                        break;
                    case 6:
                        std::cout<<"Input diem anh: ";
                        std::cin>>fDiemAnh;
                        break;
                
                }
                
                (*it)->Update(sName,nAge, sex,fDiemToan,fDiemVan, fDiemAnh);
            }
            
        }
        if(check==0)
            std::cout<<"Invalid ID!! Try input again! \n";
    }

}


void DisplayStudents(const std::list<cStudent*> &list)
{
    std::cout<<"The list of students is: \n";
    std::cout<<"ID" <<"\t"<<"Name"<<"\t"<<"Age"<<"\t"<<"Sex"<<"\t"<<"Toan"<<"\t"<<"Van"<<"\t"<<"Anh"<<"\t"<<"Avr.Point"<<"\t"<<"Hoc luc"<<"\n";
    for(auto &s: list)
    {
        DisplayInfo(s);
    }
}
void FindInfoByName(std::list<cStudent*> list)
{   
    std::string name;
    
    std::cout<<"'q' to quit.\n";
    
    while(1)
    {
        std::cout<<"Input student's name: ";
        std::cin>>name;
        if(name=="q") break;

        int check=0;
        std::list<cStudent*>::iterator  it;
        std::cout<<"ID" <<"\t"<<"Name"<<"\t"<<"Age"<<"\t"<<"Sex"<<"\t"<<"Toan"<<"\t"<<"Van"<<"\t"<<"Anh"<<"\t"<<"Avr.Point"<<"\t"<<"Hoc luc"<<"\n";
        for(it= list.begin(); it!= list.end(); ++it)
        {
            if(name==(*it)->GetName())
            {
                check++;
                DisplayInfo(*it);
            }
        }
        if(check==0) std::cout<<"No result! Try again!\n";
    }
    
}

void SortById(std::list<cStudent*> &list)
{
    list.sort([](cStudent* a, cStudent* b){return a->GetID()>b->GetID();});
    std::cout<<"After sorted by ID, ";
    DisplayStudents(list);
}
void SortByName(std::list<cStudent*> &list)
{
    list.sort([](cStudent*a, cStudent* b){return a->GetName()<b->GetName();});
    std::cout<<"After sorted by name, ";
    DisplayStudents(list);
}
void SortByAvrPoint(std::list<cStudent*> &list)
{
    list.sort([](cStudent* a, cStudent* b){return a->GetAvaragePoint()>b->GetAvaragePoint();});
    std::cout<<"After sorted by avarage point, ";
    DisplayStudents(list);
}
void SaveStudentList(std::list<cStudent*> list)
{
    std::ofstream file("database.txt");
    if(file.is_open())
    {
        std::list<cStudent*>::iterator it;
        for(it = list.begin(); it!= list.end(); ++it)
        {
            file<<(*it)->GetName()<<"\t"<<(*it)->GetAge()<<"\t"<<(*it)->GetSex()<<"\t"<<(*it)->GetDiemToan()<<"\t"<<
            (*it)->GetDiemVan()<<"\t"<<(*it)->GetDiemAnh()<<"\t"<<(*it)->GetAvaragePoint()<<"\t"<<
            (*it)->GetHocLuc()<<"\n";       
        }
        std::cout<<"Saved!\n";
        file.close();
    } 
    else std::cout<<"Can't open file. Try again!\n";
}
void LoadStudentList(std::list<cStudent*> &list)
{
    std::ifstream file("database.txt", std::ios::in| std::ios::binary);
    if(file.is_open())
    {
        while(!file.eof() )
        {
            std::string name;
            int age;
            int sex;
            float diemToan;
            float diemVan;
            float diemAnh;
            float diemTB;
            int xepLoai;
           
            file>>name>>age>>sex>>diemToan>>diemVan>>diemAnh>>diemTB>>xepLoai;
            if(name!= "")
            {
                cStudent* student = new cStudent(name, age, sex, diemToan, diemVan, diemAnh);
                listStudents.push_back(student);
            }
            else break;
           
        }
        std::cout<<"Load from database successfully! \n";
    }
    else
        std::cout<<"Fail to load from database! \n";
}

int main()
{

    
    // std::ofstream file_obj;
    // file_obj.open("Input.txt", std::ios::in);


    // 1. Add student
    // 2. Update info by id
    // 3. Sort students by id
    // 4. Find info by name(input name - output info)
    // 5. Sort by average point.
    // 6. Sort by name(a-z)
    // 7. Display list of all students.
    // cStudent* tien= new cStudent("Tien", 27, 0, 9.0f, 6.9f, 9.0f);
    // cStudent* linh = new cStudent("Linh", 27, 1, 9.0f, 9.0f, 9.0f);
    // listStudents.push_back(tien);
    // listStudents.push_back(linh);
    LoadStudentList(listStudents);


    int n;
    
    std::cout<<"1. Add student.\n";
    std::cout<<"2. Update info by ID.\n";
    std::cout<<"3. Sort students by ID.\n";
    std::cout<<"4. Find info by name(input-name output-info).\n";
    std::cout<<"5. Sort by average point.\n";
    std::cout<<"6. Sort by name(a-z).\n";
    std::cout<<"7. Display list of all students.\n";
    std::cout<<"8. Save. \n";

    while(1)
    {
        std::cout<<"=======================================\n";
        std::cout<<"Input your choice: \n";
        while(1) 
        {
            std::cin>>n;
            if(n<1 || n>8)
                std::cout<<"Invalid input. Try again!\n";
            else break;
        }

        switch (n)
        {
        case 1: 
            AddStudent(listStudents);
            std::cout<<"A new student has been added to the list.\n";
            break;
        case 2: 
            UpdateInfo(listStudents);
            break;
        case 3: 
            SortById(listStudents);
            break;
        case 4: 
            FindInfoByName(listStudents);
            break;
        case 5: 
            SortByAvrPoint(listStudents);
            break;
        case 6: 
            SortByName(listStudents);
            break;
        case 7:
            DisplayStudents(listStudents);
            break;
        case 8: 
            SaveStudentList(listStudents);
            break;
        }
    }
    
    return 0;
}
/*https://cplusplus.com/doc/tutorial/files/*/
