// CAR RENTAL MAANGEMENT SYSTEM PROJECT
// SUBMITTED BY FAIZAN 2023-CS-622
//  SECTION A
// SUBMITTED TO SIR AIZAZ AKMAL

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

ifstream fin;
ofstream fout;
int no_of_cars_in_file = 0;
int no_of_cus_in_file = 0;
// Function prototypes
void header();
int main_menu();
bool isvalidplate(string, int);
bool isvalidcnic(string, int);
bool isvaliduserid(string, int);
// Customer functionalities
void customer_login();
void customer_page(int);
void rent_car(int);
void return_car(int);
void generate_reciept(int, int, string, int, double);

// Admin functionalities
void add_car();
void remove_car();
void update_car();
void Admin_login();
void view_utilization();
void add_customer();
void remove_customer();
void update_customer();
void view_cars(string);
void view_customer();
void view_maint_report();
void view_upcoming_maint();
void send_for_maintenance(int);
void view_all_customers();

void countrecords();
string getfield(string, int);
// File handling functions (cars)
void load_carsdata_from_file();
void store_carsdata_to_file(int index);
void changecarsdata();
void update_car_rentalreport(string filename, string customer_name, int hours, string date, int rent);
void generate_car_rentalreport(int);

// File handling functions (customers)
void load_cusdata_from_file();
void store_cusdata_tofile(int);
void changecusdata();
void updaterentalreport(string, int, int, string, string, string, int);
void generaterentalreport(int, string);

struct Admin
{
    string admin_id;
    int admin_pass;
} admin;

void update_admin_pass()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Change Password\n\n"
         << endl;
    SetConsoleTextAttribute(h, 15);
    int temp;
    cout << "Enter previous password: \n";
    cin >> temp;

    if (temp == admin.admin_pass)
    {
        fout.open("admin.txt", ios::trunc);
        cout << "Enter New Password: ";
        cin >> temp;
        admin.admin_pass = temp;
        fout << admin.admin_pass;

        fout.close();
        cout << "Admin Password Successfully Updated\n";
        Sleep(700);
    }
    else
    {
        cout << "Wrong Password\n";
        Sleep(700);
    }
}
void load_admin_file()
{
    string temp;
    fin.open("admin.txt");
    if (fin.is_open())
    {
        getline(fin, temp);
        admin.admin_pass = stoi(temp);
    }

    fin.close();
}

struct Car
{
    string category;
    string brand;
    string model;
    double mileage;
    string license_plate;
    double rent;
    bool status; // true : not rented
    int times_rented;
    double total_hours_rented;
    double income_generated;
    int hoursremain_before_maintanence;
    int times_maintained;
    void set_data(int index)
    {
        do
        {
            cout << "Enter the category of car (Luxury/Hybrid/Sedan): ";
            cin >> category;
            cout << "Enter the number plate of car: ";
            cin.ignore();
            getline(cin, license_plate);
            if (isvalidplate(license_plate, index))
            {
                cout << "Enter the brand name of car: ";
                cin >> brand;
                cout << "Enter the model of car: ";
                cin >> model;
                cout << "Enter the mileage of car (kms): ";
                cin >> mileage;
                cout << "Enter the rent of car (Pkr/hr): ";
                cin >> rent;
                status = true;
                times_rented = 0;
                total_hours_rented = 0;
                income_generated = 0;
                hoursremain_before_maintanence = 0;
                times_maintained = 0;
            }
            else
            {
                cout << "Car with this number plate already exists in the system\n";
                cout << "Please enter the data of car again\n";
            }
        } while (!isvalidplate(license_plate, index));
    }
    // this function is final confirmation of rent and returns the rent amount
    int rent_car(double hours)
    {
        status = false;
        times_rented++;
        total_hours_rented += hours;
        hoursremain_before_maintanence += hours;

        return rent;
    }

    void show_data()
    {

        cout << left << setw(15) << brand << left << setw(15) << model << left << setw(15) << license_plate << left << setw(15) << mileage << setw(15) << left << rent;
        if (status)
            cout << left << setw(30) << "              Availabe" << endl;
        else
            cout << left << setw(30) << "              Rented" << endl;
    }

    void utilization_report()
    {
        cout << "Times Rented: " << times_rented << endl;
        cout << "Hours Rented: " << total_hours_rented << endl;
        cout << "Total Income Generated: " << income_generated << " Pkr" << endl;
        cout << "Total Mileage: " << mileage << " KMs" << endl;
    }
};

struct Customer
{
    string customerid;
    string customerpassword;
    string tel; // telephone number
    string cnic;
    string address;
    double money_spent;
    int times_rented = 0;
    bool status;            // true=has already rented a car
    string car_rented = ""; // stores the no_plate of car this customer has rented

    void set_data(int index)
    {
        bool validcnic = false;
        bool validid = false;

        do
        {
            money_spent = 0;
            cout << "Enter the username of customer: \n";
            cin.ignore();
            getline(cin, customerid);

            cout << "Enter the password of customer: \n";
            cin >> customerpassword;
            cout << "Enter the cnic of customer (xxxxxxxxxx): \n";
            cin >> cnic;
            if (isvalidcnic(cnic, index) && isvaliduserid(customerid, index))
            {
                validcnic = true;
                validid = true;
                cout << "Enter the number of customer: \n";
                cin >> tel;
                cout << "Enter the address of customer: \n";
                cin.ignore();
                getline(cin, address);
                status = false;
            }

        } while (!validcnic || !validid);
        cout << "done" << endl;
    }

    void show_data()
    {

        cout << left << setw(15) << customerid << left << setw(15) << customerpassword << left << setw(15) << tel << left << setw(15) << cnic << setw(15) << left << times_rented;
    }
};

Car *cars;
int no_of_cars = 0;

void changecarsdata()
{
    fout.open("carsdata.txt", ios::trunc);
    for (int i = 0; i < no_of_cars; i++)
    {
        store_carsdata_to_file(i);
    }
    fout.close();
}
void add_car_to_array()
{

    Car *new_cars = new Car[no_of_cars + 1];
    for (int i = 0; i < no_of_cars; i++)
    {
        new_cars[i] = cars[i];
    }

    cout << "Enter data for car " << no_of_cars + 1 << ":\n";

    new_cars[no_of_cars].set_data(no_of_cars);

    delete[] cars;
    cars = new_cars;
    no_of_cars++;

    changecarsdata();
}

void remove_car_from_array()
{
    Car *new_cars = new Car[no_of_cars];
    int j = 0;
    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].license_plate != "-1")
        {
            new_cars[j++] = cars[i];
        }
    }

    delete[] cars;
    cars = new_cars;

    no_of_cars--;

    changecarsdata();
}
//--------------------------------------------

Customer *customers;
int no_of_customers = 0;

void changecusdata()
{
    fout.open("cusdata.txt", ios::trunc);
    for (int i = 0; i < no_of_customers; i++)
    {

        store_cusdata_tofile(i);
    }
    fout.close();
}

void add_customer_to_array()

{
    Customer *new_customers = new Customer[no_of_customers + 1];
    for (int i = 0; i < no_of_customers; i++)
    {
        new_customers[i] = customers[i];
    }

    cout << "Enter data for customer " << no_of_customers + 1 << ":\n";
    new_customers[no_of_customers].set_data(no_of_customers);

    delete[] customers;
    customers = new_customers;
    new_customers = NULL;
    no_of_customers++;

    changecusdata();
}

void remove_customer_from_array()
{
    Customer *new_customers = new Customer[no_of_customers];
    int j = 0;
    for (int i = 0; i < no_of_customers; i++)
    {
        if (customers[i].customerid != "-1")
        {
            new_customers[j++] = customers[i];
        }
    }

    delete[] customers;
    customers = new_customers;
    no_of_customers--;

    changecusdata();
}

int main()
{
    admin = {"admin", 12345};
    countrecords();
    load_admin_file();
    load_carsdata_from_file();
    load_cusdata_from_file();

    int who = -1;
    while (who != 2)
    {
        SetConsoleTextAttribute(h, 15);

        who = main_menu();
        switch (who)
        {
        case 0: // 0=admin login
            Admin_login();
            break;
        case 1: // 1=customer login
            if (no_of_customers > 0)
                customer_login();
            else
            {
                cout << "Sorry No customers in database Right now\n";
                Sleep(700);
            }
            break;
        case 2: // 2=quit

            return 0;
            break;
        default:
            cout << "Invalid Option" << endl;
            break;
        }
    }
}

void header()
{
    SetConsoleTextAttribute(h, 15);
    cout << "\n\n\n\n\n\n\n\n\n\n\n";
    cout << setw(100) << "        <><><>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>--<><><>" << endl;
    cout << setw(100) << "       <><><>---<>---<>---<>---<>---<>---<< CAR RENTAL MANAGEMENT SYSTEM >>---<>---<>---<>---<>---<>---<><><>" << endl;
    cout << setw(100) << "        <><><>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>---<>--<><><>" << endl;
    cout << "\n";
}
void admin_portal_header()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
}

int main_menu()
{
    system("cls");
    header();
    int selected = 0;
    char key;

    do
    {
        system("cls");
        header();
        if (selected == 0)
        {
            SetConsoleTextAttribute(h, 13);
            cout << ">Admin Login\n";
            SetConsoleTextAttribute(h, 15);
            cout << "Customer Login\nQuit";
        }
        else if (selected == 1)
        {
            SetConsoleTextAttribute(h, 15);
            cout << "Admin Login\n";
            SetConsoleTextAttribute(h, 13);
            cout << ">Customer Login\n";
            SetConsoleTextAttribute(h, 15);
            cout << "Quit";
            ;
        }
        else
        {
            SetConsoleTextAttribute(h, 15);
            cout << "Admin Login\n";
            SetConsoleTextAttribute(h, 15);
            cout << "Customer Login\n";
            SetConsoleTextAttribute(h, 13);
            cout << ">Quit";
            ;
        }

        key = _getch();

        switch (key)
        {
        case 72: // UP
            if (selected > 0)
                selected--;
            break;
        case 80: // DOWN
            if (selected < 2)
                selected++;
            break;
        case 13: // Enter
            switch (selected)
            {
            case 0:
                return 0;
                break;
            case 1:
                return 1;
                break;
            case 2:
                return 2;
                break;
            }
        }
    } while (key != 27);
}

// Check validity functions
bool isvalidcnic(string cnic, int index)

{
    // check for length (length of cnic must be 13)
    int length;
    length = cnic.size();
    if (length != 13)
    {
        cout << "Error: CNIC must be 13 digits\n";
        cout << "Please Enter data again\n";
        return false;
    }

    // check for same cnic of other customers
    for (int i = 0; i < no_of_customers; i++)
    {
        if (cnic == customers[i].cnic && i != index)
        {
            cout << "Error: CNIC already exists\n";
            cout << "Please Enter data again\n";

            return false;
            break;
        }
    }
    return true;
}
bool isvaliduserid(string id, int index)
{
    for (int i = 0; i < no_of_customers; i++)
    {
        if (id == customers[i].customerid && i != index)
        {
            cout << "Error: UserID already exists\n";
            cout << "Please Enter data again\n";

            return false;
            break;
        }
    }
    return true;
}
bool isvalidplate(string plate, int index)
{
    for (int i = 0; i < no_of_cars; i++)
    {
        if (plate == cars[i].license_plate && i != index)
        {
            cout << "PLate No already exists\n";
            return false;
            break;
        }
    }
    return true;
}

// customer login

void customer_login()
{
    int attempts = 0;
    while (attempts < 3)
    {
        Sleep(200);
        system("cls");
        header();
        string id;
        string password;
        bool idfound = false, passfound = false;
        int index = -1;
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Customer Login\n\n"
             << endl;
        SetConsoleTextAttribute(h, 15);
        if (attempts != 0)
            cout << "\t\t\t\t\t" << 3 - attempts << " Attempts Remaining " << endl;
        cout << "\t\t\t\t\tEnter your username: ";
        cin.ignore();
        getline(cin, id);
        cout << "\t\t\t\t\tEnter your password: ";
        cin >> password;
        for (int i = 0; i < no_of_customers + 1; i++)
        {
            if (id == customers[i].customerid)
            {
                idfound = true;
                index = i;
            }
            if (password == customers[index].customerpassword) // working on it
            {
                passfound = true;
                cout << "Successfully Logged into Customer Portal...." << endl;
                system("pause");
                customer_page(index);
                return;
            }
        }

        if (!idfound)
        {
            cout << "Wrong Username" << endl;
        }
        if (!passfound)
        {
            cout << "Wrong Password" << endl;
        }
        attempts++;
    }
    return;
}

// Customer functions
void generate_reciept(int customerindex, int carindex, string date, int hours, double rent)
{

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Customer Login > Customer Portal > Rent Car > Receipt\n\n"
         << endl;
    SetConsoleTextAttribute(h, 15);

    cout << "\t\t==============================================\n";
    cout << "\t\t           Car Rental Receipt\n";
    cout << "\t\t==============================================\n";
    cout << "\t\tCustomer Name: " << customers[customerindex].customerid << "\n";
    cout << "\t\tCustomer CNIC: " << customers[customerindex].cnic << "\n";
    cout << "\t\tCustomer Telephone: " << customers[customerindex].tel << "\n";
    cout << "\t\tDate: " << date << "\n";
    cout << "\t\t==============================================\n";
    cout << "\t\tCar Details:\n";
    cout << "\t\tBrand: " << cars[carindex].brand << "\n";
    cout << "\t\tModel: " << cars[carindex].model << "\n";
    cout << "\t\tLicense Plate: " << cars[carindex].license_plate << "\n";
    cout << "\t\t==============================================\n";
    cout << "\t\tRental Details:\n";

    cout << "\t\tRental Duration: " << hours << " hours\n";
    cout << "\t\t==============================================\n";
    cout << "\t\tPayment Details:\n";
    cout << "\t\tTotal Amount: " << rent << " PKR\n";
    cout << "\t\t==============================================\n";
    cout << "\t\tThank you for choosing our service!\n";
    cout << "\t\t==============================================\n";
    updaterentalreport(customers[customerindex].customerid, customerindex, hours, date, cars[carindex].brand, cars[carindex].model, rent);
    update_car_rentalreport(cars[carindex].license_plate, customers[customerindex].customerid, hours, date, rent);
}
void return_car(int customerindex)
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Customer Login > Customer Portal > Return Car\n\n"
         << endl;
    SetConsoleTextAttribute(h, 15);
    double km;
    cout << "Rented car number plate: " << customers[customerindex].car_rented << "\n";
    cout << "How many kilometers did you drive?: ";
    cin >> km;
    cout << "THANK YOU FOR USING OUR SERVICE\n";
    customers[customerindex].status = false;

    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].license_plate == customers[customerindex].car_rented)
        {
            cars[i].status = true;
            cars[i].mileage += km;
            if (cars[i].hoursremain_before_maintanence > 12)
                send_for_maintenance(i);
            break;
        }
    }
    changecusdata();
    changecarsdata();
    system("pause");
}

void rent_car(int customerindex)
{

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Customer Login > Customer Portal > Rent Car\n\n"
         << endl;
    SetConsoleTextAttribute(h, 15);

    string plate;
    bool carfound = false;
    int index = -1;

    cout << "Enter the number plate of car you want to rent: \n";
    cin.ignore();
    getline(cin, plate);

    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].license_plate == plate)
        {
            carfound = true;
            index = i;
            break;
        }
    }

    if (carfound && cars[index].status == true)
    {
        char choice;

        cout << left << setw(15) << "Brand" << left << setw(15) << "Model" << left << setw(15) << "Plate Number" << left << setw(15) << "Mileage(KMs)" << left << setw(30) << "Rent Price(Pkr/hr)" << left << setw(15) << "Status" << endl;

        cars[index].show_data();
        cout << "\n\n\n";
        cout << "Do you want to proceed further?(y/n)";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            customers[customerindex].status = true;
            int hours = 0;

            string date;
            double rent;
            cout << "Enter the number of hours you want to rent this car: \n";
            cin >> hours;
            cout << "Enter the date of transaction (dd/mm/yyyy): ";
            cin >> date;
            rent = cars[index].rent_car(hours) * hours;
            cout << "Total Amount of Rent for " << hours << " is " << rent << " Pkr\n";
            customers[customerindex].money_spent += rent;
            customers[customerindex].times_rented++;
            customers[customerindex].car_rented = cars[index].license_plate;
            cars[index].income_generated += rent;
            cout << "For confirmation: ";
            system("pause");
            cout << endl;
            changecarsdata();
            changecusdata();
            generate_reciept(customerindex, index, date, hours, rent);
            system("pause");
            return;
        }
        else
        {

            return;
        }
    }

    else if (cars[index].status == false)
    {
        cout << "Car is not available at the moment....\n";
        Sleep(700);
    }
    else
    {
        cout << "No car found....\n";
        Sleep(700);
    }
}
void customer_page(int customerindex)
{
    int choice = 0;
    do
    {
        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Customer Login > Customer Portal\n\n"
             << endl;
        SetConsoleTextAttribute(h, 15);

        cout << "Choose one of the following options\n";
        cout << "1. Rent a car\n";
        cout << "2. Return a car\n";
        cout << "3. View available cars\n";
        cout << "4. Generate a rental report\n";
        cout << "5. Log out\n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            if (customers[customerindex].status == false && no_of_cars != 0)
                rent_car(customerindex);
            else if (no_of_cars == 0)
            {
                cout << "Sorry no cars available the moment\n";
                Sleep(700);
            }
            else
            {
                cout << "Error: You can't rent 2 cars at one time!\n";
                Sleep(700);
            }
            break;
        case 2:
            if (customers[customerindex].status == true)
                return_car(customerindex);
            else
            {
                cout << "Error: You did not rent a car!\n";
                Sleep(700);
            }
            break;
        case 3:
            if (no_of_cars > 0)
                view_cars("customer");
            else
            {
                cout << "No Cars available\n";
                Sleep(700);
            }
            break;
        case 4:
            generaterentalreport(customerindex, "customer");
            break;

        case 5:
            return;
            break;

        default:
            break;
        }
    } while (choice != 5);
}

void view_maint_report()
{

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > View Maintenance Report\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);

    cout << "These Cars were sent recently for maintenance\n";

    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].times_maintained > 0 && cars[i].hoursremain_before_maintanence < 6)

        {

            cout << "Number Plate: " << cars[i].license_plate << endl;
            cout << "Brand: " << cars[i].brand << endl;
            cout << "Model: " << cars[i].model << endl;
            cout << "Total mileage: " << cars[i].mileage << " Km" << endl;
            cout << "Times maintained: " << cars[i].times_maintained << endl;
            cout << "==================================\n";
        }
    }

    system("pause");
}

void view_upcoming_maint()
{

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > View Upcoming Maintenance\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    cout << "These cars will be sent for next maintenance:\n\n\n";

    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].hoursremain_before_maintanence > 6 && (12 - cars[i].hoursremain_before_maintanence) > 0)

        {

            cout << "Number Plate:                       " << cars[i].license_plate << endl;
            cout << "Brand:                              " << cars[i].brand << endl;
            cout << "Model:                              " << cars[i].model << endl;
            cout << "Next Maintenance in hours(rented):  " << 12 - cars[i].hoursremain_before_maintanence << endl;
            cout << "Times maintained:                   " << cars[i].times_maintained << endl;
            cout << "=====================================\n";
        }
    }

    system("pause");
}
// cars will be sent to maintanence if they have been rented for more than 12 hours
void send_for_maintenance(int index)
{
    cars[index].times_maintained++;
    cars[index].hoursremain_before_maintanence = 0;
}

void view_customer()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > View Customer\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    string name;
    bool namefound = false;
    int index = -1;
    cout << "Enter the name of customer you want to view data of: ";
    cin.ignore();
    getline(cin, name);
    for (int i = 0; i < no_of_customers; i++)
    {
        if (name == customers[i].customerid)
        {
            namefound = true;
            index = i;
            break;
        }
    }
    if (namefound)
    {
        cout << "Customer Data\n";
        cout << "Name: " << customers[index].customerid << endl;
        cout << "Password: " << customers[index].customerpassword << endl;
        cout << "Cnic: " << customers[index].cnic << endl;
        cout << "Tel: " << customers[index].tel << endl;
        system("pause");
        return;
    }
    else
    {
        cout << "No custmer found.....\n";
        Sleep(700);
        return;
    }
}

void view_utilization()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > View Utilization Report\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    string plate;
    cout << "Enter the number plate of car to check its utilization: \n";
    cin.ignore();
    getline(cin, plate);
    cout << "\n\n";
    for (int i = 0; i < no_of_cars; i++)
    {
        if (cars[i].license_plate == plate)
        {
            cars[i].utilization_report();
            system("pause");
            break;
        }
    }
}

void view_all_customers()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > View Customers\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);

    cout << "Main > Admin Login > Admin Portal > View Customers\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    cout << left << setw(15) << "Username" << left << setw(15) << "Password" << left << setw(15) << "Telephone" << left << setw(15) << "CNIC" << left << setw(30) << "Cars Rented" << left << setw(15) << endl;
    for (int i = 0; i < no_of_customers; i++)
    {
        customers[i].show_data();
        cout << endl;
    }
    cout << endl;
    system("pause");
}

void view_cars(string mode)
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    if (mode == "admin")
        cout << "Main > Admin Login > Admin Portal > View Cars\n\n---------------------------" << endl;
    else
        cout << "Main > Customer Login > Customer Portal > View Cars\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    string category;
    cout << "Enter the category of cars you want to view (Luxury/Hybrid/Sedan)" << endl;
    cin >> category;

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    if (mode == "admin")
        cout << "Main > Admin Login > Admin Portal > View Cars\n\n---------------------------" << endl;
    else
        cout << "Main > Customer Login > Customer Portal > View Cars\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);

    cout << left << setw(15) << "Brand" << left << setw(15) << "Model" << left << setw(15) << "Plate Number" << left << setw(15) << "Mileage(KMs)" << left << setw(30) << "Rent Price(Pkr/hr)" << left << setw(15) << "Status" << endl;
    for (int i = 0; i < no_of_cars; i++)
    {
        if (category == cars[i].category)
        {
            cars[i].show_data();
        }
    }

    system("pause");
}

void add_car()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Add Car\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    Car *cars = new Car[1];
    add_car_to_array();
    cout << "Adding Car....\n";
    Sleep(700);
    cout << "Car Successfully Added\n";
    system("pause");
}

void remove_car()

{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Remove Car\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    string plate;
    int index = -1;
    cout << "Enter number plate of car you want to remove: ";
    cin.ignore();
    getline(cin, plate);
    bool namefound = false;
    int i = 0;
    for (i = 0; i < no_of_cars; i++)
    {
        if (cars[i].license_plate == plate && cars[i].status == true)
        {
            cars[i].license_plate = "-1";
            namefound = true;
            index = i;
            break;
        }
    }

    if (namefound)

    {
        char choice;
        cout << "Details of the car are following: \n";
        cout << "Brand:   " << cars[index].brand << endl;
        cout << "Model:   " << cars[index].model << endl;
        cout << "Mileage: " << cars[index].mileage << endl;
        cout << "Rent:    " << cars[index].rent << " Pkr/hr" << endl;

        cout << "Do you want to remove this car? (y/n)";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
            remove_car_from_array();
        else
        {
            cars[index].license_plate = plate;
            return;
        }
    }
    else if (cars[i].status == false)
    {
        cout << "You can't remove a car that is rented" << endl;
        Sleep(700);
        return;
    }
    else
    {
        cout << "Invalid Plate number\n";
        return;
    }
    cout << "Removing Data....\n";
    Sleep(700);
    cout << "Car data successfully removed\n";
    system("pause");
}

void update_car()
{
    system("cls");
    header();
    string name;
    bool namefound = false;
    int index = -1;
    int choice;
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Update Car\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    cout << "Enter the No. Plater of Car you want to update data of: \n";
    cin.ignore();
    getline(cin, name);
    for (int i = 0; i < no_of_cars; i++)
    {
        if (name == cars[i].license_plate)
        {
            namefound = true;
            index = i;
            break;
        }
    }
    if (namefound && cars[index].status == true)
    {
        cout << "Previous Data: \n";
        cout << "1. Brand: " << cars[index].brand << endl;
        cout << "2. Model: " << cars[index].model << endl;
        cout << "3. No. Plate: " << cars[index].license_plate << endl;
        cout << "4. Rent: " << cars[index].rent << " (Pkr/hr)" << endl;
        cout << "What do you want to update?\n";
        cin >> choice;
        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Admin Login > Admin Portal > Update customer\n\n---------------------------" << endl;
        SetConsoleTextAttribute(h, 15);

        switch (choice)
        {
        case 1:
            cin.ignore();

            cout << "Please enter the new Brand: ";

            getline(cin, cars[index].brand);

            break;
        case 2:
            cout << "Please enter the Model: ";
            cin >> cars[index].model;
            break;
        case 3:
            cin.ignore();
            do
            {
                cout << "Please enter new No. Plate: ";

                getline(cin, cars[index].license_plate);
            } while (!isvalidplate(cars[index].license_plate, index));
            break;
        case 4:
            cout << "Please enter new Rent: ";
            cin >> cars[index].rent;
            break;

        default:
            cout << "Wrong Option" << endl;
            break;
        }
        changecarsdata();
        cout << "Data successfully updated...." << endl;

        Sleep(700);
    }
    else
    {
        cout << "No customer found....." << endl;
        Sleep(700);
    }
}

//--------------------------------------------------
void add_customer()
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Add customer\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);

    ;

    Customer *customers = new Customer[1];

    add_customer_to_array();
}

void remove_customer()
{

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Remove Customer\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    string name;
    cout << "Enter name of customer you want to remove: ";
    cin.ignore();
    getline(cin, name);
    bool namefound = false;
    for (int i = 0; i < no_of_customers; i++)
    {
        if (customers[i].customerid == name && customers[i].status == false)
        {
            customers[i].customerid = "-1";
            namefound = true;
            break;
        }
    }

    if (namefound)
    {
        remove_customer_from_array();
    }
    else
    {
        cout << "Invalid name or Customer has a rented car" << endl;
        Sleep(800);
        return;
    }

    cout << "Removing Data....\n";
    Sleep(700);
    cout << "Customer data successfully removed\n";
    system("pause");
}

void update_customer()
{
    system("cls");
    header();
    string name;
    bool namefound = false;
    int index = -1;
    int choice;
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Update customer\n\n---------------------------" << endl;
    SetConsoleTextAttribute(h, 15);
    cout << "Enter the name of customer you want to update data of: \n";
    cin.ignore();
    getline(cin, name);
    for (int i = 0; i < no_of_customers; i++)
    {
        if (name == customers[i].customerid)
        {
            namefound = true;
            index = i;
            break;
        }
    }
    if (namefound)
    {
        cout << "Previous Data: \n";
        cout << "1. Name: " << customers[index].customerid << endl;
        cout << "2. Password: " << customers[index].customerpassword << endl;
        cout << "3. Cnic: " << customers[index].cnic << endl;
        cout << "4. Tel: " << customers[index].tel << endl;
        cout << "What do you want to update?\n";
        cin >> choice;
        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Admin Login > Admin Portal > Update customer\n\n---------------------------" << endl;
        SetConsoleTextAttribute(h, 15);

        switch (choice)
        {
        case 1:
            cin.ignore();
            do
            {
                cout << "Please enter the new name: ";

                getline(cin, customers[index].customerid);
            } while (!isvaliduserid(customers[index].customerid, index));

            break;
        case 2:
            cout << "Please enter the password: ";
            cin >> customers[index].customerpassword;
            break;
        case 3:
            do
            {
                cout << "Please enter new CNIC for this customer: ";
                cin >> customers[index].cnic;
            } while (!isvalidcnic(customers[index].cnic, index));
            break;
        case 4:
            cout << "Please enter new telephone number: ";
            cin >> customers[index].tel;
            break;

        default:
            cout << "Wrong Option" << endl;
            break;
        }
        changecusdata();
        cout << "Data successfully updated...." << endl;

        Sleep(700);
    }
    else
    {
        cout << "No customer found....." << endl;
        Sleep(700);
    }
}

void Admin_page()
{
    int choice = 0;
    string name;
    string no_plate;
    bool platefound = false;
    bool namefound = false;

    while (choice != 6)

    {
        int choice2 = 0;
        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Admin Login > Admin Portal\n\n---------------------------" << endl;
        SetConsoleTextAttribute(h, 15);
        cout << "Choose one of the following options\n";
        cout << "1. Manage Cars\n";
        cout << "2. Manage Customers\n";
        cout << "3. Generate Rental Reports\n";
        cout << "4. View Other Reports\n";
        cout << "5. Change Password\n";
        cout << "6. Log Out\n";
        cin >> choice;

        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Admin Login > Admin Portal\n\n---------------------------" << endl;
        SetConsoleTextAttribute(h, 15);

        switch (choice)
        {
        case 1:
            while (choice2 != 5)
            {
                admin_portal_header();
                cout << "Choose one of the following options\n";
                cout << "1. Add a car\n";
                cout << "2. Remove a car\n";
                cout << "3. View available cars\n";
                cout << "4. Update Car Data\n";
                cout << "5. Go Back\n";
                cin >> choice2;
                switch (choice2)
                {
                case 1:
                    add_car();
                    break;
                case 2:
                    remove_car();
                    break;
                case 3:
                    if (no_of_cars > 0)
                        view_cars("admin");
                    else
                    {
                        cout << "No Cars Available\n";
                        Sleep(700);
                    }
                    break;
                case 4:
                    update_car();
                    break;
                case 5:
                    break;
                default:
                    cout << "Invalid Choice";
                    break;
                }
            }
            break;
            while (choice2 != 6)
            {
            case 2:
                admin_portal_header();
                cout << "Choose one of the following options\n";
                cout << "1. View all customers\n";
                cout << "2. Add a customer\n";
                cout << "3. Update a customer data\n";
                cout << "4. Remove a customer\n";
                cout << "5. View Single Customer Data\n";
                cout << "6. Go Back\n";
                cin >> choice2;
                switch (choice2)
                {
                case 1:
                    view_all_customers();
                    break;
                case 2:
                    add_customer();
                    break;
                case 3:
                    update_customer();
                    break;
                case 4:
                    remove_customer();
                    break;
                case 5:
                    view_customer();
                    break;
                case 6:
                    return;
                default:
                    cout << "Invalid Choice";
                    break;
                }
            }
            break;

        case 3:

            while (choice2 != 3)

            {
                admin_portal_header();

                cout << "Choose one of the following options\n";
                cout << "1. Generate Car Rental Report\n";
                cout << "2. Generate Customer Rental Report\n";
                cout << "3. Go Back\n";
                cin >> choice2;

                switch (choice2)
                {
                case 1:
                {
                    cout << "Enter the Number Plate of car to generate its rental report: \n";
                    cin.ignore();
                    getline(cin, no_plate);
                    for (int i = 0; i < no_of_cars; i++)
                    {
                        if (no_plate == cars[i].license_plate)
                        {
                            platefound = true;
                            generate_car_rentalreport(i);
                            break;
                        }
                    }
                    if (!platefound)
                    {
                        cout << "Error: No Car found\n";
                        Sleep(700);
                    }
                }
                break;
                case 2:
                {
                    cout << "Enter the name of customer: \n";
                    cin.ignore();
                    getline(cin, name);
                    for (int i = 0; i < no_of_customers; i++)
                    {
                        if (name == customers[i].customerid)
                        {

                            namefound = true;
                            generaterentalreport(i, "admin");
                            break;
                        }
                    }
                    if (!namefound)
                    {
                        cout << "Error: No Customer found\n";
                        Sleep(700);
                    }
                }
                break;
                case 3:

                    break;
                default:
                    break;
                }
            }
            break;
        case 4:

            while (choice2 != 4)
            {
                admin_portal_header();
                cout << "1. View Utilization Report\n";
                cout << "2. View Maintenance Report\n";
                cout << "3. Check Upcoming Maintenance\n";
                cout << "4. Go Back\n";
                cin >> choice2;
                switch (choice2)
                {
                case 1:
                    view_utilization();
                    break;
                case 2:
                    view_maint_report();
                case 3:
                    view_upcoming_maint();
                case 4:
                    break;
                default:
                    break;
                }
            }
            break;
        case 5:
            admin_portal_header();
            update_admin_pass();
            break;
        case 6:
            return;
        }
    }
    /*while (choice != 14)
    {
        system("cls");
        header();
        SetConsoleTextAttribute(h, 13);
        cout << "Main > Admin Login > Admin Portal\n\n---------------------------" << endl;
        SetConsoleTextAttribute(h, 15);

        cout << "Choose one of the following options\n";
        cout << "1. Add a car\n";
        cout << "2. Remove a car\n";
        cout << "3. View available cars\n";
        cout << "4. View Utilization Report\n";
        cout << "5. Add a customer\n";
        cout << "6. Update a customer data\n";
        cout << "7. Remove a customer\n";
        cout << "8. View Customer Data\n";
        cout << "9. Generate Customer Rental Report\n";
        cout << "10. Generate Car Rental Report\n";
        cout << "11. View Maintenance Report\n";
        cout << "12. Check Upcoming Maintenance\n";
        cout << "13. Change Password\n";
        cout << "14. Save changes and Log Out\n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            add_car();
            break;
        case 2:
            remove_car();
            break;
        case 3:
            view_cars("admin");
            break;
        case 4:
            view_utilization();
            break;
        case 5:
            add_customer();
            break;
        case 6:
            update_customer();
            break;
        case 7:
            remove_customer();
            break;
        case 8:
            view_customer();
            break;
        case 9:
            cout << "Enter the name of customer: \n";
            cin.ignore();
            getline(cin, name);
            for (int i = 0; i < no_of_customers; i++)
            {
                if (name == customers[i].customerid)
                {

                    namefound = true;
                    generaterentalreport(i, "admin");
                    break;
                }
            }
            if (!namefound)
            {
                cout << "Error: No Customer found\n";
                Sleep(700);
            }
            break;
        case 10:
            cout << "Enter the Number Plate of car to generate its rental report: \n";
            cin.ignore();
            getline(cin, no_plate);
            for (int i = 0; i < no_of_cars; i++)
            {
                if (no_plate == cars[i].license_plate)
                {
                    platefound = true;
                    generate_car_rentalreport(i);
                    break;
                }
            }
            if (!platefound)
            {
                cout << "Error: No Car found\n";
                Sleep(700);
            }

            break;
        case 11:
            view_maint_report();
            break;
        case 12:
            view_upcoming_maint();
            break;
        case 13:
            update_admin_pass();
            break;
        case 14:
            return;
            break;

        default:
            break;
        }
    }*/
}

void Admin_login()
{
    string id;
    int password;
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login\n\n";
    SetConsoleTextAttribute(h, 15);
    cout << "\t\t\t\t\tEnter your username: ";
    cin >> id;
    cout << "\t\t\t\t\tEnter your password: ";
    cin >> password;
    if (id == admin.admin_id && password == admin.admin_pass)
    {
        Admin_page();
        return;
    }
    else
    {
        cout << "Wrong Password or Username\n";
        Sleep(500);
        return;
    }
}

void store_carsdata_to_file(int index)
{
    int i = index;
    if (fout.is_open())
    {

        fout << cars[i].license_plate << ",";
        fout << cars[i].category << ",";
        fout << cars[i].brand << ",";
        fout << cars[i].model << ",";
        fout << cars[i].rent << ",";
        fout << cars[i].mileage << ",";
        fout << cars[i].status << ",";

        fout << cars[i].income_generated << ",";
        fout << cars[i].times_rented << ",";
        fout << cars[i].total_hours_rented << ",";
        fout << cars[i].times_maintained << ",";
        fout << cars[i].hoursremain_before_maintanence;

        fout << endl;
    }
    else
    {
        cerr << "Error in saving cars data\n";
    }
}

void countrecords()
{
    fin.open("carsdata.txt");
    string temp;

    if (fin.is_open())
    {
        while (getline(fin, temp))
        {
            no_of_cars_in_file++;
        }
        no_of_cars = no_of_cars_in_file;
    }
    else
    {
        cout << "Could not load data of cars\n";
    }
    fin.close();

    fin.open("cusdata.txt");

    if (fin.is_open())
    {
        while (getline(fin, temp))
        {
            no_of_cus_in_file++;
        }
        no_of_customers = no_of_cus_in_file++;
    }
    else
    {
        cout << "Could not load data of customers\n";
    }
    fin.close();
}

string getfield(string record, int field)
{
    int commacount = 1;
    string item = "";
    for (int i = 0; i < record.length(); i++)
    {

        if (record[i] == ',')
        {
            commacount++;
        }
        else if (commacount == field)
        {
            item = item + record[i];
        }
    };
    return item;
}

void load_carsdata_from_file()
{
    fin.open("carsdata.txt");
    string temp;
    int i = 0;

    if (fin.is_open())
    {
        Car *new_cars = new Car[no_of_cars_in_file];
        for (int i = 0; i < no_of_cars_in_file; i++)
        {
            getline(fin, temp);
            new_cars[i].license_plate = getfield(temp, 1);
            new_cars[i].category = getfield(temp, 2);
            new_cars[i].brand = getfield(temp, 3);
            new_cars[i].model = getfield(temp, 4);
            new_cars[i].rent = std::stod(getfield(temp, 5));
            new_cars[i].mileage = stod(getfield(temp, 6));
            istringstream(getfield(temp, 7)) >> new_cars[i].status;
            new_cars[i].income_generated = stod(getfield(temp, 8));
            new_cars[i].times_rented = stoi(getfield(temp, 9));
            new_cars[i].total_hours_rented = stod(getfield(temp, 10));
            new_cars[i].times_maintained = stoi(getfield(temp, 11));
            new_cars[i].hoursremain_before_maintanence = stoi(getfield(temp, 12));
        }
        cars = new_cars;
    }

    else
    {
        cerr << "Error: Could not load data of cars\n";
    }
    fin.close();
}

//-----------------------------------------------------------

void store_cusdata_tofile(int index)
{
    int i = index;

    if (fout.is_open())
    {
        fout << customers[i].cnic << ",";
        fout << customers[i].customerid << ",";
        fout << customers[i].customerpassword << ",";
        fout << customers[i].status << ",";
        fout << customers[i].money_spent << ",";
        fout << customers[i].times_rented << ",";
        fout << customers[i].car_rented << ",";
        fout << customers[i].address << ",";
        fout << customers[i].tel;
        fout << endl;
    }
}

void load_cusdata_from_file()
{
    fin.open("cusdata.txt");
    string temp;
    int i = 0;
    if (fin.is_open())
    {
        Customer *new_customers = new Customer[no_of_cus_in_file];
        for (int i = 0; i < no_of_cus_in_file; i++)
        {
            getline(fin, temp);
            if (temp != "")
            {
                new_customers[i].cnic = getfield(temp, 1);
                new_customers[i].customerid = getfield(temp, 2);
                new_customers[i].customerpassword = getfield(temp, 3);
                istringstream(getfield(temp, 4)) >> new_customers[i].status;
                new_customers[i].money_spent = std::stod(getfield(temp, 5));
                new_customers[i].times_rented = stoi(getfield(temp, 6));
                new_customers[i].car_rented = getfield(temp, 7);
                new_customers[i].address = getfield(temp, 8);
                new_customers[i].tel = getfield(temp, 9);
            }
        }
        customers = new_customers;
    }
    fin.close();
}

void updaterentalreport(string filename, int index, int hours, string date, string brand, string model, int rent)
{

    fout.open(filename + ".txt", ios::app);
    if (fout.is_open())
    {
        fout << date << ",";
        fout << rent << ",";
        fout << brand << ",";
        fout << model << ",";
        fout << hours << endl;
    }
    else
    {
        cerr << "Error in updating rental report\n";
    }

    fout.close();
}

void generaterentalreport(int index, string mode)
{
    string name = customers[index].customerid;
    string temp;
    string date;
    int hours;
    int amount;
    string model, brand;
    fin.open(customers[index].customerid + ".txt");

    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    if (mode == "customer")
        cout << "Main > Customer Login > Customer Portal > Rental Report\n\n"
             << endl;
    else
        cout << "Main > Admin Login > Admin Portal > Customer Rental Report\n\n"
             << endl;
    SetConsoleTextAttribute(h, 15);

    cout << " ==============================" << endl;
    cout << "   Customer Rental Report     " << endl;
    cout << " ==============================" << endl;

    cout << "Customer Username: " << name << endl;
    cout << "Customer CNIC: " << customers[index].cnic << endl;
    cout << "Total Number of Rentals: " << customers[index].times_rented << endl;
    cout << "Transaction History: " << endl;

    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|" << right << setw(15) << "DATE" << setw(15) << "|" << right << setw(15) << "AMOUNT" << setw(15) << "|" << right << setw(15) << "CAR NAME" << setw(15) << "|" << right << setw(15) << "RENTAL DURATION(hrs)" << setw(10) << "|" << endl;

    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
    while (getline(fin, temp))
    {
        date = getfield(temp, 1);
        amount = stoi(getfield(temp, 2));
        brand = getfield(temp, 3);
        model = getfield(temp, 4);
        hours = stoi(getfield(temp, 5));

        cout << "|" << right << setw(15) << date << setw(15);
        cout << "|" << right << setw(15) << amount << setw(15);
        cout << "|" << right << setw(15) << model;

        cout << right << setw(15) << "|" << right << setw(15) << hours << setw(15) << "|";
        cout << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "\n Total Amount Spent: " << customers[index].money_spent << " Pkr" << endl;

    fin.close();
    system("pause");
}

void update_car_rentalreport(string filename, string customer_name, int hours, string date, int rent)
{

    fout.open(filename + ".txt", ios::app);
    if (fout.is_open())
    {
        fout << date << ",";
        fout << rent << ",";
        fout << customer_name << ",";
        fout << hours << endl;
    }
    else
    {
        cerr << "Error in updating rental report\n";
    }

    fout.close();
}

void generate_car_rentalreport(int index)
{
    system("cls");
    header();
    SetConsoleTextAttribute(h, 13);
    cout << "Main > Admin Login > Admin Portal > Car Rental Report\n\n"
         << endl;
    SetConsoleTextAttribute(h, 15);
    string temp;
    string date;
    string name;
    double rent;
    int hours;

    fin.open(cars[index].license_plate + ".txt");

    cout << " ==============================" << endl;
    cout << "        Car Rental Report     " << endl;
    cout << " ==============================" << endl;

    cout << "\n\nCar Name: " << cars[index].brand << " " << cars[index].model << endl;
    cout << "License Plate: " << cars[index].license_plate << endl;
    cout << "Current Mileage: " << cars[index].mileage << " Km" << endl;
    cout << "\nRental History: " << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;
    cout << "|" << right << setw(15) << "DATE" << setw(15) << "|" << right << setw(15) << "NAME" << setw(15) << "|" << right << setw(15) << "AMOUNT" << setw(15) << "|" << right << setw(15) << "RENTAL DURATION(hrs)" << setw(10) << "|" << endl;

    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

    while (getline(fin, temp))
    {
        date = getfield(temp, 1);
        rent = stod(getfield(temp, 2));
        name = getfield(temp, 3);
        hours = stoi(getfield(temp, 4));
        cout << "|" << right << setw(15) << date << setw(15);
        cout << "|" << right << setw(15) << name << setw(15);
        cout << "|" << right << setw(15) << rent;

        cout << right << setw(15) << "|" << right << setw(15) << hours << setw(15) << "|";
        cout << endl;
    }
    cout << "-------------------------------------------------------------------------------------------------------------------------" << endl;

    cout << "\nTotal Rentals: " << cars[index].times_rented << endl;
    cout << "Total Income Gnerated: " << cars[index].income_generated << " Pkr" << endl;
    system("pause");
    fin.close();
}