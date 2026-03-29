#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <iomanip>
#include <fstream>

using namespace std;

static const string SEPERATOR = "#//#";
static const string file_name = "clients data file.txt";

struct stClientData
{
    string account_number;
    string pin_code;
    string name;
    string phone;
    double account_balance;
    bool mark_is_deleted = false;
};

string ReadAccountNumber()
{
    cout << "Please Enter Account Number?";
    string accountNumber;
    getline(cin >> ws, accountNumber);
    return accountNumber;
}

void EndOfMenuOption()
{
    cout << "\nPress any Key To Continue...";
    system("pause>0");
}

void ShowOptionScreen(string title)
{
    system("cls");
    cout << "\n===========================================\n";
    cout << "\t\t" << title;
    cout << "\n===========================================\n";
}

string convertRecordToLine(stClientData client_data, string seperator)
{
    string str = "";
    str += client_data.account_number + seperator;
    str += client_data.pin_code + seperator;
    str += client_data.name + seperator;
    str += client_data.phone + seperator;
    str += to_string(client_data.account_balance);
    return str;
}

vector<string> splitString(string str, string SEPERATOR)
{
    vector<string> vString;
    string word = "";
    int pos = 0;
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

stClientData convertLineToRecord(string str, string seperator)
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

void SaveDataToFileFromStart(string file_name, vector<stClientData> vClientData)
{
    fstream clients_data_file;
    clients_data_file.open(file_name, ios::out);
    if (clients_data_file.is_open())
    {
        for (stClientData& client_data : vClientData)
            if (!client_data.mark_is_deleted)
            {
                clients_data_file << convertRecordToLine(client_data, SEPERATOR) << endl;
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
    clients_data_file.open(file_name, ios::in); //Read Mode
    if (clients_data_file.is_open())
    {
        string line;
        while (getline(clients_data_file, line))
        {
            vClientData.push_back(convertLineToRecord(line, SEPERATOR));
        }
        clients_data_file.close();
    }
    return vClientData;
}

bool FindClientByAccountNumber(string account_number, vector<stClientData> vClientData, stClientData& client)
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

stClientData ReadClientData()
{
    stClientData client_data;
    cout << "Enter Account Number? ";
    getline(cin >> ws, client_data.account_number);
    while (FindClientByAccountNumber(client_data.account_number, LoadClientDataFromFile(), client_data))
    {
        cout << "Account Number already exists, please enter another account number? ";
        getline(cin >> ws, client_data.account_number);
    }
    while (client_data.account_number.empty())
    {
        cout << "Account Number can't be empty, please enter account number? ";
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

void AddNewClient()
{
    cout << "Adding New Client\n\n";

    stClientData client_data;
    client_data = ReadClientData();
    SaveNewDataToFile(file_name, convertRecordToLine(client_data, SEPERATOR));
}

void AddClients()
{
    char ch;
    do
    {
        AddNewClient();
        cout << "Client Added Successfully, do you want to add more clients?(y)Yes (otherwise)No";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
}

void PrintAccountCard(stClientData client_data)
{
    cout << "\nThe following are the client details:\n\n";

    cout << "Account Number  : " << client_data.account_number << endl;
    cout << "Pin Code        : " << client_data.pin_code << endl;
    cout << "Name            : " << client_data.name << endl;
    cout << "Phone           : " << client_data.phone << endl;
    cout << "Account Balance : " << client_data.account_balance << endl;
}

void MarkIsDeleted(string account_number, vector<stClientData>& vClientData)
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
            MarkIsDeleted(account_number, vClientData);
            SaveDataToFileFromStart(file_name, vClientData);
            cout << "\nClient Deleted Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

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

            SaveDataToFileFromStart(file_name, vClientData);
            cout << "\nClient Updated Successfully.\n";
        }
    }
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

void FindClient(string account_number, vector<stClientData> vClientData)
{
    stClientData client;
    if (FindClientByAccountNumber(account_number, vClientData, client))
        PrintAccountCard(client);
    else
        cout << "\nClient with Account Number (" << account_number << ") is Not Found!\n";
}

void PrintClientRecord(stClientData client_data)
{
    cout << "| " << setw(18) << client_data.account_number;
    cout << "| " << setw(13) << client_data.pin_code;
    cout << "| " << setw(38) << client_data.name;
    cout << "| " << setw(18) << client_data.phone;
    cout << "| " << setw(15) << client_data.account_balance;
}

void printClientsData(vector<stClientData> vClientData)
{
    cout << setw(60) << "Client List (" << vClientData.size() << ") Client(s).";
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

void ShowMenu()
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
    cout << "\t[6] Exit.\n";
    cout << "\n===========================================\n";
    cout << "Choose what do you want to do? [1 to 6]? ";
}

void PerformMainMenuOption()
{
    short option;
    vector<stClientData> vClientData = LoadClientDataFromFile();
    ShowMenu();
    cin >> option;
    switch (option)
    {
    case 1:
        system("cls");
        printClientsData(vClientData);
        EndOfMenuOption();
        break;
    case 2:
        ShowOptionScreen("Add New Client Screen");
        AddClients();
        EndOfMenuOption();
        break;
    case 3:
        ShowOptionScreen("Delete Client Screen");
        DeleteClient(ReadAccountNumber(), vClientData);
        EndOfMenuOption();
        break;
    case 4:
        ShowOptionScreen("Update Client Info Screen");
        UpdateClient(ReadAccountNumber(), vClientData);
        EndOfMenuOption();
        break;
    case 5:
        ShowOptionScreen("Find Client Screen");
        FindClient(ReadAccountNumber(), vClientData);
        EndOfMenuOption();
        break;
    case 6:
        ShowOptionScreen("Program Ends :-)");
        EndOfMenuOption();
        exit(0);
        break;
    default:
        break;
    }

}

int main() {
    while (true)
    {
        PerformMainMenuOption();
    }
    system("pause>0");
}