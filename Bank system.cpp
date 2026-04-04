#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <fstream>

using namespace std;

void Login();
void ShowMainMenu();
void ShowTransactionMenu();
void ShowManageUserScreen();

static const string SEPERATOR = "#//#";
static const string ClientFileName = "clients data file.txt";
static const string UserFileName = "user data file.txt";

enum enReadMenuOption { Show_Client_List = 1, Add_New_Client, Delete_Client, Update_Client_Info, Find_Client, Transaction_Menu, Manage_User, _Logout };
enum enTransactionMenuOption { deposit = 1, withdraw, show_balance, main_menu };
enum enManageUser { list_users = 1, add_new_user, delete_user, update_user, find_user, Main_Menu };
enum enPermisions{
    P_ShowClientsList = 1,// 0 0 0 0 0 0 1
    P_AddNewClients = 2,// 0 0 0 0 0 1 0
    P_DeleteClient = 4,// 0 0 0 0 1 0 0
    P_UpdateClient = 8,// 0 0 0 1 0 0 0
    P_FindClient = 16,// 0 0 1 0 0 0 0
    P_Transacation = 32,// 0 1 0 0 0 0 0
    P_ManageUser = 64,// 1 0 0 0 0 0 0
    P_All = -1,// 1 1 1 1 1 1 1
};

//     structurs

struct stClientData
{
    string account_number;
    string pin_code;
    string name;
    string phone;
    double account_balance=0;
    bool mark_is_deleted = false;
};

struct stUserData
{
    string username;
    string pasword;
    short permissios = 0; // 0 0 0 0 0 0 0
    bool mark_is_deleted = false;
};

stUserData CurrentUser;

//    Read Data

string ReadAccountNumber()
{
    cout << "Please Enter Account Number?";
    string accountNumber;
    getline(cin >> ws, accountNumber);
    return accountNumber;
}

stUserData readUsernameAndPassword()
{
    stUserData User;
    cout << "Enter Username? ";
    getline(cin >> ws, User.username);
    cout << "Enter Password? ";
    getline(cin >> ws, User.pasword);
    return User;
}

string ReadUsername()
{
    stUserData User;
    cout << "Please enter username? ";
    getline(cin >> ws, User.username);
    return User.username;
}

short ReadPermissionsToSet()
{
    stUserData user;
    char answer;
    cout << "Do you want to give fill access? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_All;
        return user.permissios;
    }
    cout << "Do you want to give access to:\n";
    cout << "Show Client List? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_ShowClientsList;
    }
    cout << "Add New Clients? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_AddNewClients;
    }
    cout << "Delete Client? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_DeleteClient;
    }
    cout << "Update Client? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_UpdateClient;
    }
    cout << "Find Client? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_FindClient;
    }
    cout << "Transacation? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_Transacation;
    }
    cout << "Manage User? (y)yes (n)no? ";
    cin >> answer;
    if (answer == 'y' || answer == 'Y')
    {
        user.permissios = user.permissios | enPermisions::P_ManageUser;
    }
    return user.permissios;
}

double ReadDepositAmount()
{
    cout << "Please Enter Amount? ";
    double amount;
    cin >> amount;
    while (amount <= 0)
    {
        cout << "Amount must be greater than zero, please enter valid amount: ";
        cin >> amount;
    }
    return amount;
}

double ReadWithdrawAmount(stClientData Client)
{
    cout << "Please Enter Amount? ";
    double amount;
    cin >> amount;
    while (amount > Client.account_balance)
    {
        cout << "Amount Exceed the balance, you can withdraw up tp: "<<Client.account_balance<<endl;
        cout << "Please Enter Amount? ";
        cin >> amount;
    }
    return amount;
}

//      Go Back To Manue and Op screen

void GoBackToMainMenuOption()
{
    cout << "\nPress any Key To Continue...";
    system("pause>0");
    ShowMainMenu();
}

void GoBackToTransactionMenuOption()
{
    cout << "\nPress any Key To Continue...";
    system("pause>0");
    ShowTransactionMenu();
}

void ShowOptionScreen(string title)
{
    system("cls");
    cout << "\n===========================================\n";
    cout << "\t\t" << title;
    cout << "\n===========================================\n";
}

void GoBackToManageUserScreen()
{
    cout << "\nPress any Key To Continue...";
    system("pause>0");
    ShowManageUserScreen();
}

//      Record to Line And vic versa

string convertClientRecordToLine(stClientData client_data, string seperator)
{
    string str = "";
    str += client_data.account_number + seperator;
    str += client_data.pin_code + seperator;
    str += client_data.name + seperator;
    str += client_data.phone + seperator;
    str += to_string(client_data.account_balance);
    return str;
}

string ConvertUserRecordToLine(stUserData UserData, string SEPERATOR)
{
    string str = "";
    str += UserData.username + SEPERATOR;
    str += UserData.pasword + SEPERATOR;
    str += to_string(UserData.permissios);
    return str;
}

vector<string> splitString(string str, string SEPERATOR)
{
    vector<string> vString;
    string word = "";
    size_t pos = 0;
    while ((pos = str.find(SEPERATOR)) != string::npos)
    {
        word = str.substr(0, pos);
        vString.push_back(word);
        str.erase(0, pos + SEPERATOR.length());
    }
    if (!str.empty())
        vString.push_back(str);
    return vString;
}

stClientData convertClientLineToRecord(string str, string seperator)
{
    vector<string>Vstring = splitString(str, seperator);
    stClientData client_data;

    if (Vstring.size() == 5)
    {
        client_data.account_number = Vstring[0];
        client_data.pin_code = Vstring[1];
        client_data.name = Vstring[2];
        client_data.phone = Vstring[3];
        client_data.account_balance = stod(Vstring[4]);
    }

    return client_data;
}

stUserData ConvertUserLineToRecord(string str, string SEPERATOR)
{
    vector<string>vUser = splitString(str, SEPERATOR);
    stUserData stUser;
    stUser.username = vUser[0];
    stUser.pasword = vUser[1];
    stUser.permissios = stoi(vUser[2]);
    return stUser;
}

//      Files

void SaveClientDataToFileFromStart(string file_name, vector<stClientData>& vClientData)
{
    fstream clients_data_file;
    clients_data_file.open(file_name, ios::out);
    if (clients_data_file.is_open())
    {
        for (stClientData& client_data : vClientData)
            if (!client_data.mark_is_deleted)
            {
                clients_data_file << convertClientRecordToLine(client_data, SEPERATOR) << endl;
            }
        clients_data_file.close();
    }
}

void SaveUserDataToFileFromStart(string file_name, vector<stUserData>& vUsers)
{
    fstream clients_data_file;
    clients_data_file.open(file_name, ios::out);
    if (clients_data_file.is_open())
    {
        for (stUserData& user : vUsers)
            if (!user.mark_is_deleted)
            {
                clients_data_file << ConvertUserRecordToLine(user, SEPERATOR) << endl;
            }
        clients_data_file.close();
    }
}

void SaveNewDataToFile(string file_name, string result_line)
{
    fstream clients_data_file;
    clients_data_file.open(file_name, ios::out | ios::app);
    if (clients_data_file.is_open())
    {
        clients_data_file << result_line << endl;
        clients_data_file.close();
    }
}

vector<stClientData> LoadClientDataFromFile()
{
    vector<stClientData> vClientData;
    fstream clients_data_file;
    clients_data_file.open(ClientFileName, ios::in); //Read Mode
    if (clients_data_file.is_open())
    {
        string line;
        while (getline(clients_data_file, line))
        {
            vClientData.push_back(convertClientLineToRecord(line, SEPERATOR));
        }
        clients_data_file.close();
    }
    return vClientData;
}

vector<stUserData> LoadUserDataFromFile()
{
    vector<stUserData> vUsers;

    fstream UserDataFile;
    UserDataFile.open(UserFileName, ios::in); //Read Only
    
    if (UserDataFile.is_open())
    {
        string line;
        while (getline(UserDataFile, line))
        {
            vUsers.push_back(ConvertUserLineToRecord(line,SEPERATOR));
        }
        UserDataFile.close();
    }
    return vUsers;
}

//      Find Client or User

bool CheckAccessPermission(enPermisions permission)
{
    return ((CurrentUser.permissios & permission) == permission);
}

bool FindClientByAccountNumber(string account_number, vector<stClientData>& vClientData, stClientData& client)
{
    for (stClientData& client_data : vClientData)
    {
        if (client_data.account_number == account_number)
        {
            client = client_data;
            return true;
        }
    }
    return false;
}

bool FindUserByUsername(string username, vector<stUserData>& vUser, stUserData& client)
{
    for (stUserData &users : vUser)
    {
        if (users.username == username)
        {
            client = users;
            return true;
        }  
    }
    return false;
}

bool FindUserByUsernameAndPassword(string username, string Password)
{
    vector<stUserData> vUsers = LoadUserDataFromFile();

    for (stUserData& User : vUsers)
    {
        if (User.username == username && User.pasword == Password)
        {
            CurrentUser = User;
            return true;
        }
    }
    return false;
}

//      Read Full Data

stUserData ReadUserData(vector<stUserData> &Users)
{
    stUserData user;
    char answer;
    cout << "Enter Username? ";
    getline(cin >> ws, user.username);
    while (FindUserByUsername(user.username, Users, user))
    {
        cout << "Username already exists, please enter another account number? ";
        getline(cin >> ws, user.username);
    }
    cout << "Enter Password? ";
    getline(cin >> ws, user.pasword);
    user.permissios = ReadPermissionsToSet();
    return user;
}

stClientData ReadClientData(vector<stClientData> &vClientData)
{
    stClientData client_data;
    cout << "Enter Account Number? ";
    getline(cin >> ws, client_data.account_number);
    while (FindClientByAccountNumber(client_data.account_number, vClientData, client_data))
    {
        cout << "Account Number already exists, please enter another account number? ";
        getline(cin >> ws, client_data.account_number);
    }
    cout << "Enter PinCode? ";
    getline(cin, client_data.pin_code);
    cout << "Enter Name? ";
    getline(cin, client_data.name);
    cout << "Enter Phone? ";
    getline(cin, client_data.phone);
    cout << "Enter Account Balance? ";
    cin >> client_data.account_balance;

    return client_data;
}

//      Print Data and Lists

void RejectMessage()
{
    system("cls");
    cout << "\n___________________________________________\n";
    cout << "Access Denied,\n";
    cout << "You don\'t Have Permission To do this,\n";
    cout << "Please Conact your Admin.\n";
    cout << "___________________________________________\n";


}

void PrintAccountCard(stClientData& client_data)
{
    cout << "\nThe following are the client details:\n\n";

    cout << "Account Number  : " << client_data.account_number << endl;
    cout << "Pin Code        : " << client_data.pin_code << endl;
    cout << "Name            : " << client_data.name << endl;
    cout << "Phone           : " << client_data.phone << endl;
    cout << "Account Balance : " << client_data.account_balance << endl;
}

void PrintUsertCard(stUserData& user)
{
    cout << "\nThe following are the user details:\n\n";

    cout << "Username    : " << user.username << endl;
    cout << "Passwird    : " << user.pasword << endl;
    cout << "Permissions : " << user.permissios << endl;
}

void PrintClientRecord(stClientData& client_data)
{
    cout << "| " << setw(18) << client_data.account_number;
    cout << "| " << setw(13) << client_data.pin_code;
    cout << "| " << setw(38) << client_data.name;
    cout << "| " << setw(18) << client_data.phone;
    cout << "| " << setw(15) << client_data.account_balance;
}

void PrintUserRecord(stUserData& User)
{
    cout << "| " << setw(25) << User.username;
    cout << "| " << setw(25) << User.pasword;
    cout << "| " << setw(25) << User.permissios;
}

void PrintBalanceRecord(stClientData &client_data)
{
    cout << "| " << setw(26) << client_data.account_number;
    cout << "| " << setw(53) << client_data.name;
    cout << "| " << setw(23) << client_data.account_balance;
}

void ShowClientsScreen(vector<stClientData>& vClientData)
{
    cout << "\t\t\t\t\t Client List (" << vClientData.size() << ") Client(s).";
    cout << "\n_________________________________________________________________________________________________________________\n\n";
    cout << left << setw(20) << "| Account Number";
    cout << setw(15) << "| Pin Code";
    cout << setw(40) << "| Name";
    cout << setw(20) << "| Phone";
    cout << setw(17) << "| Account Balance";
    cout << "\n_________________________________________________________________________________________________________________\n";
    for (stClientData& str : vClientData)
    {
        PrintClientRecord(str);
        cout << "\n";
    }
    cout << "_________________________________________________________________________________________________________________\n";
}

void ListUsers(vector<stUserData>& vUsers)
{
    cout << "\t\t\t\t\t Users List (" << vUsers.size() << ") Client(s).";
    cout << "\n_________________________________________________________________________________________________________________\n\n";
    cout << left << setw(27) << "| Username";
    cout << setw(27) << "| Password";
    cout << setw(27) << "| Permissions";
    cout << "\n_________________________________________________________________________________________________________________\n";
    for (stUserData& str : vUsers)
    {
        PrintUserRecord(str);
        cout << "\n";
    }
    cout << "_________________________________________________________________________________________________________________\n";
}

void ShowTotalBalances(vector<stClientData>& vClientData)
{
    double TotalBalance = 0;
    cout << "\t\t\t\t\tClient List (" << vClientData.size() << ") Client(s).";
    cout << "\n_________________________________________________________________________________________________________________\n\n";
    cout << left << setw(28) << "| Account Number";
    cout << setw(55) << "| Name";
    cout << setw(25) << "| Account Balance";
    cout << "\n_________________________________________________________________________________________________________________\n";
    for (stClientData& str : vClientData)
    {
        PrintBalanceRecord(str);
        TotalBalance += str.account_balance;
        cout << "\n";
    }
    cout << "_________________________________________________________________________________________________________________\n";
    cout <<  "\t\t\t\t\tTotal Balance : " << TotalBalance << endl;
}

//      Add new Clients or Users

void AddNewUser(vector<stUserData>& vUsers)
{
    cout << "Adding New User\n\n";

    stUserData NewUser = ReadUserData(vUsers);
    SaveNewDataToFile(UserFileName, ConvertUserRecordToLine(NewUser, SEPERATOR));
}

void AddUsers(vector<stUserData>& vUsers)
{
    char ch='y';
    do
    {
        AddNewUser(vUsers);
        cout << "User Added Successfully, do you want to add more clients?(y)Yes (otherwise)No? ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
}

void AddNewClient(vector<stClientData>& vClientData)
{
    cout << "Adding New Client\n\n";

    stClientData client_data;
    client_data = ReadClientData(vClientData);
    SaveNewDataToFile(ClientFileName, convertClientRecordToLine(client_data, SEPERATOR));
}

void AddClients(vector<stClientData>& vClientData)
{
    char ch;
    do
    {
        AddNewClient(vClientData);
        cout << "Client Added Successfully, do you want to add more clients?(y)Yes (otherwise)No? ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
}

//      Delete

void MarkClientIsDeleted(string account_number, vector<stClientData>& vClientData)
{
    for (stClientData& client_data : vClientData)
    {
        if (client_data.account_number == account_number)
        {
            client_data.mark_is_deleted = true;
            return;
        }
    }
}

void DeleteClient(string account_number, vector<stClientData>& vClientData)
{
    stClientData client;
    char answer;

    if (FindClientByAccountNumber(account_number, vClientData, client))
    {
        PrintAccountCard(client);
        cout << "\nAre you sure you want delete this client? (y)Yes (othewise)No? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            MarkClientIsDeleted(account_number, vClientData);
            SaveClientDataToFileFromStart(ClientFileName, vClientData);
            cout << "\nClient Deleted Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

void MarkUserIsDeleted(string username, vector<stUserData>& vUsers)
{
    for (stUserData& user : vUsers)
    {
        if (user.username == username)
        {
            user.mark_is_deleted = true;
            return;
        }
    }
}

void DeleteUser(string username, vector<stUserData>& vUsers)
{
    stUserData user;
    char answer;

    if (username=="Admin")
    {
        cout << "You can\'t delete this User.\n";
        return;
    }

    if (FindUserByUsername(username, vUsers, user))
    {
        PrintUsertCard(user);
        cout << "\nAre you sure you want delete this user? (y)Yes (othewise)No? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            MarkUserIsDeleted(username, vUsers);
            SaveUserDataToFileFromStart(UserFileName, vUsers);
            cout << "\nClient Deleted Successfully.\n";
        }
    }
    else
        cout << "\nClient with username (" << username << ") is Not Found!\n";
}

//      Update

void ChangeClientRecord(string account_number, vector<stClientData>& vClientData)
{
    for (stClientData& client_data : vClientData)
    {
        if (account_number == client_data.account_number)
        {
            cout << "Enter New PinCode? ";
            getline(cin >> ws, client_data.pin_code);
            cout << "Enter New Name? ";
            getline(cin, client_data.name);
            cout << "Enter New Phone? ";
            getline(cin, client_data.phone);
            cout << "Enter New Account Balance? ";
            cin >> client_data.account_balance;
            return;
        }
    }
}

void UpdateClient(string account_number, vector<stClientData>& vClientData)
{
    stClientData client;
    char answer;

    if (FindClientByAccountNumber(account_number, vClientData, client))
    {
        PrintAccountCard(client);
        cout << "\nAre you sure you want update this client? (y)Yes (othewise)No? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            ChangeClientRecord(account_number, vClientData);

            SaveClientDataToFileFromStart(ClientFileName, vClientData);
            cout << "\nClient Updated Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

void ChangeUserRecord(string username, vector<stUserData>& vUsers)
{
    char answer;
    for (stUserData& user : vUsers)
    {
        if (username == user.username)
        {
            user.permissios = 0;    // 0 0 0 0 0 0 0
            cout << "Enter Password? ";
            getline(cin >> ws, user.pasword);
            cout << "Do you want to give fill access? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_All;
                return;
            }
            cout << "\nDo you want to give access to:\n\n";
            cout << "Show Client List? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_ShowClientsList;
            }
            cout << "Add New Clients? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_AddNewClients;
            }
            cout << "Delete Client? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_DeleteClient;
            }
            cout << "Update Client? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_UpdateClient;
            }
            cout << "Find Client? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_FindClient;
            }
            cout << "Transacation? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_Transacation;
            }
            cout << "Manage User? (y)yes (n)no? ";
            cin >> answer;
            if (answer == 'y' || answer == 'Y')
            {
                user.permissios = user.permissios | P_ManageUser;
            }
            return;
        }
    }
}

void UpdateUser(string username, vector<stUserData>& vUser)
{
    stUserData user;
    char answer;

    if (username == "Admin")
    {
        cout << "You can\'t Update this User.\n";
        return;
    }
    if (FindUserByUsername(username, vUser, user))
    {
        PrintUsertCard(user);
        cout << "\nAre you sure you want update this user? (y)Yes (othewise)No? ";
        cin >> answer;
        if (answer == 'y' || answer == 'Y')
        {
            ChangeUserRecord(username, vUser);

            SaveUserDataToFileFromStart(UserFileName, vUser);
            cout << "\nUser Updated Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account Number (" << username << ") is Not Found!\n";
}


//      Find

void FindClient(string account_number, vector<stClientData>& vClientData)
{
    stClientData client;
    if (FindClientByAccountNumber(account_number, vClientData, client))
        PrintAccountCard(client);
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

void FindUser(string username, vector<stUserData>& vUser)
{
    stUserData user;
    if (FindUserByUsername(username, vUser, user))
    {
        PrintUsertCard(user);
    }
    else
    {
        cout << "\nClient with Username (" << username << ") is Not Found!\n";

    }
}

//      Deposit and withdarw

void DepositBalanceToClientByAccountNumber(string AccountNumber, vector<stClientData>& vClient, double amount)
{
    cout << "\nAre you sure you want perform this transaction? (y)Yes (othewise)No? ";
    char answer;
    cin >> answer;
    cin.ignore();
    if (answer == 'y' || answer == 'Y')
    {
        for (stClientData& ClientData : vClient)
        {
            if (AccountNumber == ClientData.account_number)
            {
                ClientData.account_balance += amount;
                SaveClientDataToFileFromStart(ClientFileName, vClient);
                cout << "\nDone Successfully, New Balance is: " << ClientData.account_balance;
                break;
            }
        }
    }
}

void Deposit(string AccountNumber, vector<stClientData>& vClient)
{
	stClientData client;
    while (!FindClientByAccountNumber(AccountNumber, vClient, client))
    {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
		AccountNumber = ReadAccountNumber();
    }
    PrintAccountCard(client);
    double amount = ReadDepositAmount();
    DepositBalanceToClientByAccountNumber(AccountNumber, vClient, amount);
}

void Withdraw(string AccountNumber, vector<stClientData>& vClient)
{
    stClientData client;
    while (!FindClientByAccountNumber(AccountNumber, vClient, client))
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
        AccountNumber = ReadAccountNumber();
    }
    PrintAccountCard(client);
    double amount = ReadWithdrawAmount(client);
    DepositBalanceToClientByAccountNumber(AccountNumber, vClient, amount * -1);
}

//      Read Options

enReadMenuOption ReadMenuOption()
{
    short option;
    cin >> option;
    return (enReadMenuOption)option;
}

enTransactionMenuOption ReadTransactionOption()
{
    short option;
    cin >> option;
    return (enTransactionMenuOption)option;
}

enManageUser ReadManageUserOptions()
{
    short choice;
    cin >> choice;
    return (enManageUser)choice;
}

//      Performs and final result

void PerformManageUserMenu()
{
    vector<stUserData> vUser = LoadUserDataFromFile();
    enManageUser choice = ReadManageUserOptions();
    switch (choice)
    {
    case enManageUser::list_users:
        system("cls");
        ListUsers(vUser);
        GoBackToManageUserScreen();
        break;
    case enManageUser::add_new_user:
        ShowOptionScreen("Add New User Screen");
        AddUsers(vUser);
        GoBackToManageUserScreen();
        break;
    case enManageUser::delete_user:
        ShowOptionScreen("Delete User Screen");
        DeleteUser(ReadUsername(), vUser);
        GoBackToManageUserScreen();
        break;
    case enManageUser::update_user:
        ShowOptionScreen("Update User Screen");
        UpdateUser(ReadUsername(), vUser);
        GoBackToManageUserScreen();
        break;
    case enManageUser::find_user:
        ShowOptionScreen("Find User Screen");
        FindUser(ReadUsername(), vUser);
        GoBackToManageUserScreen();
        break;
    case enManageUser::Main_Menu:
        GoBackToMainMenuOption();
        break;
    default:
        break;
    }
}

void ShowManageUserScreen()
{
    system("cls");
    cout << "\n===========================================\n";
    cout << "\tManage User Menu Screen";
    cout << "\n===========================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "\n===========================================\n";
    cout << "Choose what do you want to do? [1 to 6]? ";
    PerformManageUserMenu();
}

void PerformTransactinsMenuOption()
{
    vector<stClientData> vClientData = LoadClientDataFromFile();
    enTransactionMenuOption option = ReadTransactionOption();
    switch (option)
    {
    case enTransactionMenuOption::deposit:
        ShowOptionScreen("Deposit Screen");
        Deposit(ReadAccountNumber(), vClientData);
        GoBackToTransactionMenuOption();
        break;
    case enTransactionMenuOption::withdraw:
        ShowOptionScreen("Withdraw Screen");
        Withdraw(ReadAccountNumber(), vClientData);
        GoBackToTransactionMenuOption();
        break;
    case enTransactionMenuOption::show_balance:
        system("cls");
        ShowTotalBalances(vClientData);
        GoBackToTransactionMenuOption();
        break;
    case enTransactionMenuOption::main_menu:
        GoBackToMainMenuOption();
        break;
    default:
        break;
    }
}

void ShowTransactionMenu()
{
    system("cls");
    cout << "\n===========================================\n";
    cout << "\tTransactions Menu Screen";
    cout << "\n===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "\n===========================================\n";
    cout << "Choose what do you want to do? [1 to 4]? ";
    PerformTransactinsMenuOption();
}

void PerformMainMenuOption()
{

    vector<stClientData> vClientData = LoadClientDataFromFile();
    enReadMenuOption option;
    option = ReadMenuOption();

    switch (option)
    {
    case enReadMenuOption::Show_Client_List:
        if (CheckAccessPermission(enPermisions::P_ShowClientsList))
        {
            system("cls");
            ShowClientsScreen(vClientData);
        }
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::Add_New_Client:
        if (CheckAccessPermission( enPermisions::P_AddNewClients))
        {
            ShowOptionScreen("Add New Client Screen");
            AddClients(vClientData);
        }
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::Delete_Client:
        if (CheckAccessPermission(enPermisions::P_DeleteClient))
        {
            ShowOptionScreen("Delete Client Screen");
            DeleteClient(ReadAccountNumber(), vClientData);
        }
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::Update_Client_Info:
        if (CheckAccessPermission(enPermisions::P_UpdateClient))
        {
            ShowOptionScreen("Update Client Info Screen");
            UpdateClient(ReadAccountNumber(), vClientData);
        }
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::Find_Client:
        if (CheckAccessPermission(enPermisions::P_FindClient))
        {
            ShowOptionScreen("Find Client Screen");
            FindClient(ReadAccountNumber(), vClientData);
        }
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::Transaction_Menu:
        if (CheckAccessPermission(enPermisions::P_Transacation))
        {
            system("cls");
            ShowTransactionMenu();
        }
        else
            RejectMessage();
        break;
    case enReadMenuOption::Manage_User:
        if (CheckAccessPermission(enPermisions::P_ManageUser))
            ShowManageUserScreen();
        else
            RejectMessage();
        GoBackToMainMenuOption();
        break;
    case enReadMenuOption::_Logout:
        Login();
        break;
    default:
        break;
    }
}

void ShowMainMenu()
{
    system("cls");
    cout << "\n===========================================\n";
    cout << "\t\tMain Menu Screen";
    cout << "\n===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions Menu.\n";
	cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "\n===========================================\n";
    cout << "Choose what do you want to do? [1 to 8]? ";
	PerformMainMenuOption();
}

void Login()
{
    bool LoginIsFailed = false;
    stUserData user;
    do
    {
        ShowOptionScreen("Login Screen");
        if (LoginIsFailed)
        {
            cout << "Invalid Username or Password!\n";
        }

        user = readUsernameAndPassword();
        LoginIsFailed = !FindUserByUsernameAndPassword(user.username, user.pasword);
    } while (LoginIsFailed);
    ShowMainMenu();

}

int main() {

    Login();

    system("pause>0");
}