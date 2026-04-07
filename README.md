# Bank System Management - C++ Console Application

This is a console-based **Bank System Management Application** written in **C++**, designed to manage clients, users, and transactions in a simple bank system. It allows for secure login, role-based permissions, and operations such as adding, deleting, updating, and finding clients and users, as well as performing deposits and withdrawals.  

---

## Features

### User Management
- Add, update, delete, and list users.
- Assign role-based permissions:
  - Show Client List
  - Add New Clients
  - Delete Client
  - Update Client
  - Find Client
  - Transactions
  - Manage Users
  - Full Access

### Client Management
- Add, update, delete, and find clients.
- Store client details:
  - Account Number
  - Pin Code
  - Name
  - Phone
  - Account Balance

### Transactions
- Deposit money to a client account.
- Withdraw money from a client account (with balance validation).
- Show total balances of all clients.

### Access Control
- Permissions-based access to features.
- Admin and regular users supported.
- Prevent unauthorized actions with clear rejection messages.

### File Handling
- All clients and users are stored in `.txt` files:
  - `clients data file.txt`
  - `user data file.txt`
- Supports reading and writing client/user data with separators.

---

## File Structure

- `main.cpp` – The main program file containing all logic.
- `clients data file.txt` – Stores all client information.
- `user data file.txt` – Stores all user credentials and permissions.

---

## How It Works

1. **Login System**  
   - The user is prompted to enter a username and password.
   - The program verifies credentials and loads user permissions.

2. **Main Menu**  
   - Options displayed according to the user's permissions.
   - Main functionalities:
     - Show client list
     - Add, delete, update, find clients
     - Transactions menu
     - Manage users
     - Logout

3. **Transactions Menu**  
   - Deposit
   - Withdraw
   - Show total balances
   - Return to main menu

4. **Manage Users Menu**  
   - List users
   - Add new user
   - Delete user
   - Update user
   - Find user
   - Return to main menu

5. **Permissions**
   - Only users with specific permissions can perform certain actions.
   - Full access users can manage everything.

---

## Data Format

**Client Record (in `clients data file.txt`)**  
