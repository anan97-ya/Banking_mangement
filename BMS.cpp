#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <exception>
using namespace std;

ifstream Inp;
ofstream Out;

string getCurrentTime()
{
	time_t now = time(0);
	tm *localtm = localtime(&now);
	stringstream ss;
	ss << put_time(localtm, "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

class User
{
protected:
	string username;
	string password;

public:
	User()
	{
	}
	User(string username, string password)
	{
		this->username = username;
		this->password = password;
	}

	virtual bool login(string username, string password) = 0;
	void setusername(string name)
	{
		username = name;
	}
	void setpassword(string pass)
	{
		password = pass;
	}
	void changePassword()
	{
		string newPassword;
		cout << "\t\t\tEnter new password: ";
		cin >> newPassword;
		password = newPassword;
		cout << "\t\t\tPassword changed successfully!" << endl;
	}
	string getname()
	{
		return username;
	}
	string getpassword()
	{
		return password;
	}
	virtual ~User() {}
};

class Customer : public User
{
private:
public:
	Customer()
	{
	}
	Customer(string username, string password)
			: User(username, password)
	{
	}

	bool login(string username, string password) override
	{
		string enteredPin, enteredpass;
		cout << "\t\t\tEnter the Username: ";
		cin >> enteredpass;
		cout << "\t\t\tEnter your PIN code: ";
		cin >> enteredPin;

		if (enteredPin == password && enteredpass == username)
		{
			cout << "\t\t\tLogin successful!" << endl;
			return true;
		}
		else
		{
			cout << "\t\t\tInvalid PIN code. Login failed." << endl;
			cout << "\t\t\tIf you want to change the password press Y" << endl;
			char y;
			cin >> y;
			if (y == 'Y' || y == 'y')
			{
				Customer::changePassword();
			}
			return false;
		}
	}
	~Customer()
	{
	}
};

class Admin : public User
{

public:
	Admin()
	{
		username = "admin";
		password = "admin";
	}
	Admin(string username, string password)
			: User(username, password)
	{
	}

	void display()
	{
		cout << "Admin" << endl;
	}

	bool login(string username, string password) override
	{
		string enter_Username, enter_Password;
		cout << "\t\t\tEnter admin username: ";
		cin >> enter_Username;
		cout << "\t\t\tEnter admin password: ";
		cin >> enter_Password;

		if (enter_Username == "admin" && enter_Password == "admin")
		{
			cout << "\t\t\tAdmin login successful!" << endl;
			return true;
		}
		else
		{
			cout << "\t\t\tInvalid admin credentials. Login failed." << endl;
			cout << "\t\t\tIf you want to change the password press Y" << endl;
			char y;
			cin >> y;
			if (y == 'Y' || y == 'y')
			{
				Admin::changePassword();
			}
			return false;
		}
	}
	~Admin()
	{
	}
};

class Account
{
protected:
	int accountNumber;
	string firstname;
	string lastname;
	double balance;
	string cardtype;
	string email;
	int phonenumber;

public:
	Account()
	{
		accountNumber = 0;
		firstname = "";
		lastname = "";
		balance = 0;
		cardtype = "";
		email = "";
		phonenumber = 0;
	}
	Account(string firstname, string lastname, int accountNumber, double balance, string cardtype, string email, int phonenumber)
	{
		this->accountNumber = accountNumber;
		this->balance = balance;
		this->cardtype = cardtype;
		this->email = email;
		this->phonenumber = phonenumber;
		this->firstname = firstname;
		this->lastname = lastname;
	}

	string getFirstName()
	{
		return firstname;
	}

	void setFirstName(string firstName)
	{
		firstname = firstName;
	}

	string getLastName()
	{
		return lastname;
	}

	void setLastName(string lastName)
	{
		lastname = lastName;
	}
	string getemail()
	{
		return email;
	}
	int getphone()
	{
		return phonenumber;
	}
	void setphone(int Num)
	{
		phonenumber = Num;
	}
	void setemail(string mail)
	{
		email = mail;
	}
	int getAccountNumber()
	{
		return accountNumber;
	}
	void setbalance(double bal)
	{
		balance = bal;
	}
	double getBalance()
	{
		return balance;
	}
	void setaccountnumber(int accno)
	{
		accountNumber = accno;
	}
	string getcardtype()
	{
		return cardtype;
	}
	void setcardtype(string type)
	{
		cardtype = type;
	}
	virtual void deposit(double amount) = 0;
	virtual void withdraw(double amount) = 0;
	virtual void display() const = 0;
	virtual void saveAccountData() = 0;
	virtual void serachaccount() = 0;
	virtual void Addaccount(int sz) = 0;
	virtual void updateaccount() = 0;
	virtual void deleteaccount(string filename, int size) = 0;

	virtual ~Account() {}
};

class SavingsAccount : public Account
{
private:
	string savedatafile;
	int size;
	Customer **customer;
	SavingsAccount **saveaccount;
	Admin admin;

public:
	SavingsAccount()
	{
		customer = nullptr;
		saveaccount = nullptr;
		size = 0;
	}
	SavingsAccount(string filename, int S)
	{
		this->savedatafile = filename;
		this->size = S;

		saveaccount = new SavingsAccount *[size];
		customer = new Customer *[size];
		for (int i = 0; i < size; i++)
		{
			saveaccount[i] = nullptr;
			customer[i] = nullptr;
		}

		Inp.open(filename);
		if (!Inp)
		{
			cout << "Failed to open Saving acount data file." << endl;
			return;
		}
		int index = 0;
		string line;
		while (getline(Inp, line))
		{

			istringstream SS(line);
			// Read data
			string fname, lname, user_nnam, pass_word, C_type, Email;
			int accountNO, phone_num;
			double bal;

			SS >> fname >> lname >> user_nnam >> pass_word >> accountNO >> bal >> C_type >> Email >> phone_num;

			customer[index] = new Customer(user_nnam, pass_word);
			saveaccount[index] = new SavingsAccount(fname, lname, accountNO, bal, C_type, Email, phone_num);

			index++;
			if (index >= size)
			{
				break; // to avoid buffer
			}
		}

		Inp.close();
	}

	SavingsAccount(string firstname, string lastname, int accountNumber, double balance, string cardtype, string email, int phonenumber)
			: Account(firstname, lastname, accountNumber, balance, cardtype, email, phonenumber)
	{
	}

	void deposit(double amount)
	{
		int accountno;
		cout << setw(30) << "\t\tEnter the account Number: ";
		cin >> accountno;

		bool found = false;
		int Index = -1;

		for (int i = 0; i < size; i++)
		{
			if (saveaccount[i]->accountNumber == accountno)
			{
				found = true;
				Index = i;
				break;
			}
		}
		if (found)
		{
			if (customer[Index]->login(customer[Index]->getname(), customer[Index]->getpassword()))
			{
				saveaccount[Index]->balance += amount;
				cout << setw(30) << "\t\tDeposit of $" << amount << " into account " << saveaccount[Index]->accountNumber << " successful." << endl;
				SavingsAccount::saveAccountData();
			}
		}
		else
		{
			cout << left << setw(30) << "\t\tAccount not found!" << endl;
		}
	}
	void withdraw(double amount)
	{
		int accountno;
		cout << setw(30) << "\t\tEnter the account Number: ";
		cin >> accountno;

		bool found = false;
		int Index = -1;

		for (int i = 0; i < size; i++)
		{
			if (saveaccount[i]->getAccountNumber() == accountno)
			{
				found = true;
				Index = i;
				break;
			}
		}
		if (found)
		{
			if (customer[Index]->login(customer[Index]->getname(), customer[Index]->getpassword()))
			{
				try
				{
					if (saveaccount[Index]->balance < amount)
					{
						throw runtime_error("Insufficient balance in account: ");
					}
					else
					{
						saveaccount[Index]->balance -= amount;
						cout << setw(30) << "\t\tWithdrawal of $" << amount << " from account " << saveaccount[Index]->accountNumber << " successful." << endl;
						SavingsAccount::saveAccountData();
					}
				}
				catch (runtime_error e)
				{
					cout << e.what() << endl;
					cout << "\t\t\tPlease Enter the sufficient balance less then your account balance" << endl;
					double Amount_W;
					cout << "\t\t\tEnter the Amount you want to withdraw" << endl;
					cin >> Amount_W;
					SavingsAccount::withdraw(Amount_W);
				}
			}
		}
		else
		{
			cout << "\t\t\t\tAccount not found" << endl;
		}
	}
	void updateaccount()
	{
		int updateaccount;
		SavingsAccount::display();

		cout << left << setw(30) << "\t\t\tEnter the account number update: ";
		cin >> updateaccount;
		string User, PASS, type, mail;
		double Balance;
		int accNo;
		int phone_num;
		bool found = false;
		int updateIndex = -1;
		string fname, lname;
		for (int i = 0; i < size; i++)
		{
			if (saveaccount[i]->accountNumber == updateaccount)
			{
				cout << setw(30) << "\t\tName: " << saveaccount[i]->getFirstName() << " " << saveaccount[i]->getLastName() << endl;
				cout << setw(30) << "\t\tUsername: " << customer[i]->getname() << endl;
				cout << setw(30) << "\t\tAccount Number: " << saveaccount[i]->getAccountNumber() << endl;
				cout << setw(30) << "\t\tBalance: $" << saveaccount[i]->balance << endl;
				cout << setw(30) << "\t\tCard Type: " << saveaccount[i]->getcardtype() << endl;
				cout << setw(30) << "\t\tEmail Address: " << saveaccount[i]->getemail() << endl;
				cout << setw(30) << "\t\tPhone Number: " << saveaccount[i]->getphone() << endl;
				found = true;
				updateIndex = i;
				break;
			}
		}
		if (found)
		{
			cout << setw(30) << "\t\tEnter User new First name: ";
			cin >> fname;
			cout << setw(30) << "\t\tEnter User new Last name: ";
			cin >> lname;
			cout << setw(30) << "\t\tEnter User new name: ";
			cin >> User;

			cout << setw(30) << "\t\tEnter new Password: ";
			cin >> PASS;

			cout << setw(30) << "\t\tEnter new  Card Type: ";
			cin >> type;

			cout << setw(30) << "\t\tEnter the new account Number: ";
			cin >> accNo;

			cout << setw(30) << "\t\tEnter the new Intial Balance: ";
			cin >> Balance;

			cout << setw(30) << "\t\tEnter the new Email address: ";
			cin >> mail;

			cout << setw(30) << "\t\tEnter the new Phone number: ";
			cin >> phone_num;

			customer[updateIndex]->setusername(User);
			customer[updateIndex]->setpassword(PASS);
			saveaccount[updateIndex]->setcardtype(type);
			saveaccount[updateIndex]->setaccountnumber(accNo);
			saveaccount[updateIndex]->setbalance(Balance);
			saveaccount[updateIndex]->setphone(phone_num);
			saveaccount[updateIndex]->setemail(mail);
			saveaccount[updateIndex]->setFirstName(fname);
			saveaccount[updateIndex]->setLastName(lname);

			cout << setw(30) << "\t\t\tAccount Edited successfully" << endl;
			SavingsAccount::saveAccountData();
		}
		else
		{
			cout << setw(30) << "\t\t\tAccount Not found" << endl;
		}
	}
	void Addaccount(int S)
	{
		size = S;

		Out.open(savedatafile, ios::app); // Open the file
		if (!Out)
		{
			cout << left << setw(30) << "File is not open!" << endl;
			return;
		}

		cout << left << setw(30) << "\t\t\t\tCUSTOMER ACCOUNT REGISTRATION" << endl;
		string us, pass, type;
		char c;
		bool added = false;
		do
		{
			cout << left << setw(30) << "\t\tEnter User new First name: ";
			string firstname;
			cin >> firstname;

			cout << setw(30) << "\t\tEnter User new Last name: ";
			string lastname;
			cin >> lastname;

			cout << setw(30) << "\t\tEnter User name: ";
			cin >> us;

			cout << setw(30) << "\t\tEnter Password: ";
			cin >> pass;

			cout << setw(30) << "\t\tEnter Card Type: ";
			cin >> type;

			int accountNumber;
			cout << setw(30) << "\t\tEnter the account Number: ";
			cin >> accountNumber;

			double balance;
			cout << setw(30) << "\t\tEnter the Initial Balance: ";
			cin >> balance;

			string email;
			cout << setw(30) << "\t\tEnter the Email address: ";
			cin >> email;

			int phoneNumber;
			cout << setw(30) << "\t\tEnter the Phone number: ";
			cin >> phoneNumber;

			added = true;
			if (added)
			{

				customer[size] = new Customer(us, pass);

				saveaccount[size] = new SavingsAccount(firstname, lastname, accountNumber, balance, type, email, phoneNumber);
				;

				size++;

				cout << left << setw(30) << "\t\tThe Account Registered Successfully at " << getCurrentTime() << endl;
				// Write the data to the file
				for (int i = size - 1; i < size; i++)
				{
					Customer *cust = customer[i];
					SavingsAccount *saveaccou = saveaccount[i];
					Out << left << setw(20) << saveaccou->getFirstName() << " "
							<< left << setw(20) << saveaccou->getLastName()
							<< left << setw(20) << cust->getname()
							<< left << setw(20) << cust->getpassword()
							<< left << setw(20) << saveaccou->getAccountNumber()
							<< left << setw(20) << saveaccou->getBalance()
							<< left << setw(20) << saveaccou->getcardtype()
							<< left << setw(20) << saveaccou->getemail()
							<< left << setw(20) << saveaccou->getphone()
							<< endl;
				}
				Out.close();
			}
			cout << left << setw(30) << "\t\tDo You Want To Add Another Record? (Y/N): ";

			cin >> c;
		} while (c == 'Y' || c == 'y');
	}

	void serachaccount()
	{
		int serachaccount;
		cout << setw(30) << "\t\t\tEnter the Account Number to serach: ";
		cin >> serachaccount;
		bool found1 = false;

		int Index = -1;
		for (int i = 0; i < size; i++)
		{
			if (saveaccount[i]->accountNumber == serachaccount)
			{

				found1 = true;
				Index = i;
				break;
			}
		}
		if (found1)
		{
			cout << "\t\t\tName: " << saveaccount[Index]->getFirstName() << " " << saveaccount[Index]->getLastName() << endl;
			cout << "\t\t\tUsername: " << customer[Index]->getname() << endl;
			cout << "\t\t\tAccount Number: " << saveaccount[Index]->getAccountNumber() << endl;
			cout << "\t\t\tBalance: $" << saveaccount[Index]->getBalance() << endl;
			cout << "\t\t\tCard Type: " << saveaccount[Index]->getcardtype() << endl;
			cout << "\t\t\tEmail Address: " << saveaccount[Index]->getemail() << endl;
			cout << "\t\t\tPhone Number: " << saveaccount[Index]->getphone() << endl;
		}

		else
		{
			cout << left << setw(30) << "Account number not found" << endl;
		}
	}
	void deleteaccount(string filename, int s)
	{
		int deleteacc;
		if (admin.login("admin", "admin"))
		{
			SavingsAccount::display();

			cout << left << setw(30) << "\t\tEnter the account you want to delete: ";
			cin >> deleteacc;
			bool found = false;
			int deleteIndex = -1;
			for (int i = 0; i < size; i++)
			{
				if (saveaccount[i]->getAccountNumber() == deleteacc)
				{
					cout << setw(30) << "\t\tName: " << saveaccount[i]->getFirstName() << " " << saveaccount[i]->getLastName() << endl;
					cout << setw(30) << "\t\tUsername: " << customer[i]->getname() << endl;
					cout << setw(30) << "\t\tAccount Number: " << saveaccount[i]->accountNumber << endl;
					cout << setw(30) << "\t\tBalance: $" << saveaccount[i]->balance << endl;
					cout << setw(30) << "\t\tCard Type: " << saveaccount[i]->getcardtype() << endl;
					cout << setw(30) << "\t\tEmail Address: " << saveaccount[i]->getemail() << endl;
					cout << setw(30) << "\t\tPhone Number: " << saveaccount[i]->getphone() << endl;

					found = true;
					deleteIndex = i;
					break;
				}
			}
			if (found)
			{
				char a;
				cout << "\n\t\t\tIf you want to delete this account enter Y...";
				cin >> a;
				if (a == 'Y' || a == 'y')
				{

					if (deleteIndex >= 0 && deleteIndex < size)
					{
						delete customer[deleteIndex];
						delete saveaccount[deleteIndex];
						for (int i = deleteIndex; i < size - 1; i++)
						{
							customer[i] = customer[i + 1];
							saveaccount[i] = saveaccount[i + 1];
						}
						size--;
						SavingsAccount::saveAccountData();
						cout << setw(30) << "\t\t\tAccount deleted successfully at " << getCurrentTime() << endl;
					}
				}
				else
				{
					cout << "\t\t\tAccount is not deleted" << endl;
				}
			}
			else
			{
				cout << left << setw(30) << "Account not found." << endl;
			}
		}
	}

	void saveAccountData()
	{
		Out.open(savedatafile, ios::out);

		if (!Out)
		{
			cout << setw(30) << "\t\t\tFailed to open saving account data file." << endl;
			return;
		}
		Customer *cust;
		SavingsAccount *saveaccou;
		// Write student
		for (int i = 0; i < size; i++)
		{
			cust = customer[i];
			saveaccou = saveaccount[i];
			Out << left << setw(20) << saveaccou->getFirstName() << " "
					<< left << setw(20) << saveaccou->getLastName()
					<< left << setw(20) << cust->getname()
					<< left << setw(20) << cust->getpassword()
					<< left << setw(20) << saveaccou->getAccountNumber()
					<< left << setw(20) << saveaccou->getBalance()
					<< left << setw(20) << saveaccou->getcardtype()
					<< left << setw(30) << saveaccou->getemail()
					<< left << setw(30) << saveaccou->getphone()
					<< endl;
		}
		Out.close();
	}

	void display() const
	{
		cout << left << setw(30) << "Account Type: Savings" << endl;
		cout << setw(50) << "================================CUSTOMER FOR SAVING ACCOUNT LIST=====================================" << endl;
		cout << left << setw(10) << "A/C"
				 << left << setw(20) << "Name"
				 << left << setw(20) << "No. Account"
				 << left << setw(20) << "Username"
				 << left << setw(15) << "Card Type"
				 << left << setw(30) << "Email Address"
				 << left << setw(15) << "Phone No."
				 << left << setw(15) << "Balance"
				 << endl;

		for (int i = 0; i < size; i++)
		{
			cout << left << setw(10) << "Saving" << left << setw(20) << saveaccount[i]->getFirstName() + " " + saveaccount[i]->getLastName()
					 << left << setw(20) << saveaccount[i]->getAccountNumber() << left << setw(20) << customer[i]->getname() << left << setw(15) << saveaccount[i]->getcardtype() << left << setw(30) << saveaccount[i]->getemail()
					 << left << setw(15) << saveaccount[i]->getphone() << left << setw(15) << saveaccount[i]->getBalance() << endl;
		}
	}

	~SavingsAccount()
	{
		SavingsAccount::saveAccountData();
		if (saveaccount != nullptr)

		{
			delete[] saveaccount;
		}
		if (customer != nullptr)
		{
			delete[] customer;
		}
	}
};

class CheckingAccount : public Account
{
private:
	string checkdatafile;
	int size;
	Customer **customer;
	CheckingAccount **checkaccount;
	Admin admin;
	double feecharge;

public:
	CheckingAccount()
	{
		customer = nullptr;
		checkaccount = nullptr;
		size = 0;
	}
	CheckingAccount(string filename, int size)
	{
		checkdatafile = filename;
		this->size = size;
		this->feecharge = 0.5;
		customer = new Customer *[size];
		checkaccount = new CheckingAccount *[size];

		for (int i = 0; i < size; i++)
		{
			checkaccount[i] = nullptr;
			customer[i] = nullptr;
		}

		checkdatafile = filename;

		Inp.open(filename);
		if (!Inp)
		{
			cout << setw(30) << "Failed to open Checking class data file." << endl;
			return;
		}
		int index = 0;
		string line;
		while (getline(Inp, line))
		{
			stringstream SS(line);

			// Read data
			string fname, lname, user_nnam, pass_word, C_type, Email;
			int accountNO, phone_num;
			double bal;

			SS >> fname >> lname >> user_nnam >> pass_word >> accountNO >> bal >> C_type >> Email >> phone_num;

			customer[index] = new Customer(user_nnam, pass_word);
			checkaccount[index] = new CheckingAccount(fname, lname, accountNO, bal, C_type, Email, phone_num);

			index++;
			if (index >= size)
			{

				break;
			}
		}

		Inp.close();
	}

	CheckingAccount(string firstname, string lastname, int accountNumber, double balance, string cardtype, string email, int phonenumber)
			: Account(firstname, lastname, accountNumber, balance, cardtype, email, phonenumber)
	{
		this->feecharge = 0.5;
	}

	void deposit(double amount)
	{
		cout << "\n======================================================================================\n";
		int accountno;
		cout << left << setw(30) << "\t\t\t\tEnter the account Number: ";
		cin >> accountno;

		bool found = false;
		int Index = -1;

		for (int i = 0; i < size; i++)
		{
			if (checkaccount[i]->accountNumber == accountno)
			{
				found = true;
				Index = i;
				break;
			}
		}
		if (found)
		{
			if (customer[Index]->login(customer[Index]->getname(), customer[Index]->getpassword()))
			{
				amount -= feecharge;
				checkaccount[Index]->balance += amount;
				cout << "\t\t\t\t\t\tFee charges $" << feecharge << " dedicated from your Balance\n";
				cout << left << setw(30) << "\t\t\tDeposit of $" << amount << " into account " << checkaccount[Index]->accountNumber << " successful at " << getCurrentTime() << endl;
				CheckingAccount::saveAccountData();
			}
		}
	}

	void withdraw(double amount)
	{

		int accountno;
		cout << left << setw(30) << "\t\t\tEnter the account Number: ";
		cin >> accountno;

		bool found = false;
		int Index = -1;

		for (int i = 0; i < size; i++)
		{
			if (checkaccount[i]->getAccountNumber() == accountno)
			{
				found = true;
				Index = i;
				break;
			}
		}
		if (found)
		{
			if (customer[Index]->login(customer[Index]->getname(), customer[Index]->getpassword()))
			{
				try
				{
					if (checkaccount[Index]->balance >= amount)
					{

						checkaccount[Index]->balance -= amount;
						checkaccount[Index]->balance -= feecharge;
						cout << "\t\t\t\t\tFee charges $" << feecharge << " dedicated from your Balance\n";
						cout << "\t\t\tWithdrawal of $" << amount << " from account " << accountNumber << " successful at " << getCurrentTime() << endl;
						CheckingAccount::saveAccountData();
					}
					else
					{
						throw runtime_error("\t\t\tInsufficient balance in account ");
					}
				}
				catch (runtime_error e)
				{
					cout << e.what() << endl;
					cout << "\t\t\tPlease Enter the sufficient balance less then your account balance" << endl;
					double Amount_W;
					cout << "\t\t\tEnter the Amount you want to withdraw" << endl;
					cin >> Amount_W;
					CheckingAccount::withdraw(Amount_W);
				}
			}
		}
		else
		{
			cout << "Account not found" << endl;
		}
	}

	void updateaccount()
	{
		int updateaccount;
		CheckingAccount::display();

		cout << "\t\t\t\t\t\tEnter the account number to update: ";
		cin >> updateaccount;
		string fname, lname;
		string User, PASS, type, mail;
		double Balance;
		int accNo;
		int phone_num;
		bool found = false;
		bool added = false;
		int updateIndex = -1;
		for (int i = 0; i < size; i++)
		{
			if (checkaccount[i]->accountNumber == updateaccount)
			{
				cout << setw(30) << "\t\t\tAccount Found! " << endl;
				cout << setw(30) << "\t\t\tName: " << checkaccount[i]->getFirstName() << " " << checkaccount[i]->getLastName() << endl;
				cout << setw(30) << "\t\t\tUsername: " << customer[i]->getname() << endl;
				cout << setw(30) << "\t\t\tAccount Number: " << checkaccount[i]->accountNumber << endl;
				cout << setw(30) << "\t\t\tBalance: $" << checkaccount[i]->balance << endl;
				cout << setw(30) << "\t\t\tCard type " << checkaccount[i]->getcardtype() << endl;
				cout << setw(30) << "\t\t\tEmial: " << checkaccount[i]->getemail() << endl;
				cout << setw(30) << "\t\t\tPhone Number: " << checkaccount[i]->getphone() << endl;
				found = true;
				updateIndex = i;
				break;
			}
		}
		if (found)
		{
			cout << setw(30) << "\t\t\t\tEnter User new First name: ";
			cin >> fname;

			cout << setw(30) << "\t\t\t\tEnter User new Last name: ";
			cin >> lname;

			cout << setw(30) << "\t\t\t\tEnter the new User name: ";
			cin >> User;

			cout << setw(30) << "\t\t\t\tEnter the new Password: ";
			cin >> PASS;

			cout << setw(30) << "\t\t\t\tEnter the new Card Type: ";
			cin >> type;

			cout << setw(30) << "\t\t\t\tEnter the new account Number: ";
			cin >> accNo;

			cout << setw(30) << "\t\t\t\tEnter the new Initial Balance: ";
			cin >> Balance;

			cout << setw(30) << "\t\t\t\tEnter the new Email address: ";
			cin >> mail;

			cout << setw(30) << "\t\t\t\tEnter the new Phone number: ";
			cin >> phone_num;
			added = true;
			if (added)
			{

				customer[updateIndex]->setusername(User);
				customer[updateIndex]->setpassword(PASS);
				checkaccount[updateIndex]->setcardtype(type);
				checkaccount[updateIndex]->setaccountnumber(accNo);
				checkaccount[updateIndex]->setbalance(Balance);
				checkaccount[updateIndex]->setphone(phone_num);
				checkaccount[updateIndex]->setemail(mail);
				checkaccount[updateIndex]->setFirstName(fname);
				checkaccount[updateIndex]->setLastName(lname);

				cout << setw(30) << "\t\t\t\tAccount Edited successfully at " << getCurrentTime() << endl;
				CheckingAccount::saveAccountData();
			}
		}
		else
		{
			cout << setw(30) << "Account Not found" << endl;
		}
	}

	void Addaccount(int s)
	{
		Out.open(checkdatafile, ios::app);
		if (!Out)
		{
			cout << left << setw(30) << "File is not open!" << endl;
			return;
		}

		cout << left << setw(30) << "\t\t\t\t\tCUSTOMER ACCOUNT REGISTRATION" << endl;
		string us, pass, type;
		char c;
		bool added;
		do
		{
			cout << setw(30) << "\t\tEnter User  First name: ";
			string firstname;
			cin >> firstname;

			cout << left << setw(30) << "\t\tEnter User Last name: ";
			string lastname;
			cin >> lastname;

			cout << left << setw(30) << "\t\tEnter User name: ";
			cin >> us;

			cout << left << setw(30) << "\t\tEnter Password: ";
			cin >> pass;

			cout << left << setw(30) << "\t\tEnter Card Type: ";
			cin >> type;

			int accountNumber;
			cout << left << setw(30) << "\t\tEnter the account Number: ";
			cin >> accountNumber;

			double balance;
			cout << left << setw(30) << "\t\tEnter the Initial Balance: ";
			cin >> balance;

			string email;
			cout << left << setw(30) << "\t\tEnter the Email address: ";
			cin >> email;

			int phoneNumber;
			cout << left << setw(30) << "\t\tEnter the Phone number: ";
			cin >> phoneNumber;
			added = true;
			if (added)
			{

				Customer *cust = new Customer(us, pass);
				CheckingAccount *checkacc = new CheckingAccount(firstname, lastname, accountNumber, balance, type, email, phoneNumber);

				customer[size] = cust;
				checkaccount[size] = checkacc;

				size++;
				// Write the data
				for (int i = size - 1; i < size; i++)
				{
					Customer *cust = customer[i];
					CheckingAccount *checkacc = checkaccount[i];
					Out << left << setw(20) << checkacc->getFirstName()
							<< left << setw(20) << checkacc->getLastName()
							<< left << setw(20) << cust->getname()
							<< left << setw(20) << cust->getpassword()
							<< left << setw(20) << checkacc->getAccountNumber()
							<< left << setw(20) << checkacc->getBalance()
							<< left << setw(20) << checkacc->getcardtype()
							<< left << setw(30) << checkacc->getemail()
							<< left << setw(30) << checkacc->getphone()
							<< endl;
				}

				Out.close();

				cout << left << setw(30) << "\t\t\t\t\tThe Account Registered Successfully at  " << getCurrentTime() << endl;
			}
			cout << left << setw(30) << "\t\t\t\tDo You Want To Add Another Record? (Y/N): ";
			cin >> c;
		} while (c == 'Y' || c == 'y');
	}
	void serachaccount()
	{
		int serachaccount;
		cout << left << setw(30) << "\t\t\tEnter the Acount Number to serach: ";
		cin >> serachaccount;
		bool found1 = false;

		int Index = -1;
		for (int i = 0; i < size; i++)
		{
			if (checkaccount[i]->getAccountNumber() == serachaccount)
			{
				if (customer[i]->login(customer[i]->getname(), customer[i]->getpassword()))
				{
					found1 = true;
					Index = i;
					break;
				}
			}
		}
		if (found1)
		{
			cout << setw(30) << "\t\t\tName: " << checkaccount[Index]->getFirstName() << " " << checkaccount[Index]->getLastName() << endl;
			cout << setw(30) << "\t\t\tAccount Holder Name: " << customer[Index]->getname() << endl;
			cout << setw(30) << "\t\t\tAccount Number: " << checkaccount[Index]->getAccountNumber() << endl;
			cout << setw(30) << "\t\t\tBalance: $" << checkaccount[Index]->getBalance() << endl;
			cout << setw(30) << "\t\t\tCard type: " << checkaccount[Index]->getcardtype() << endl;
			cout << setw(30) << "\t\t\tEmial: " << checkaccount[Index]->getemail() << endl;
			cout << setw(30) << "\t\t\tPhone Number: " << checkaccount[Index]->getphone() << endl;
		}

		else
		{
			cout << "\t\t\tAccount number not found" << endl;
		}
	}
	void deleteaccount(string filename, int s)
	{
		int deleteacc;
		if (admin.login("admin", "admin"))
		{
			CheckingAccount::display();
			cout << "\t\t\tEnter the account you want to delete: ";
			cin >> deleteacc;
			bool found = false;
			int deleteIndex = -1;
			for (int i = 0; i < size; i++)
			{
				if (checkaccount[i]->getAccountNumber() == deleteacc)
				{
					cout << "Account Found! " << endl;
					cout << setw(30) << "\t\t\tName: " << checkaccount[i]->getFirstName() << " " << checkaccount[i]->getLastName() << endl;
					cout << setw(30) << "\t\t\tAccount Holder Name: " << customer[i]->getname() << endl;
					cout << setw(30) << "\t\t\tAccount Number: " << checkaccount[i]->getAccountNumber() << endl;
					cout << setw(30) << "\t\t\tBalance: $" << checkaccount[i]->getBalance() << endl;
					cout << setw(30) << "\t\t\tCard type " << checkaccount[i]->getcardtype() << endl;
					cout << setw(30) << "\t\t\tEmial: " << checkaccount[i]->getemail() << endl;
					cout << setw(30) << "\t\t\tPhone Number: " << checkaccount[i]->getphone() << endl;
					found = true;
					deleteIndex = i;
					break;
				}
			}
			if (found)
			{
				char y;
				cout << "\t\t\t\tEnter Y to delete this account... ";
				cin >> y;
				if (y == 'Y' || y == 'y')
				{

					delete customer[deleteIndex];
					delete checkaccount[deleteIndex];
					for (int i = deleteIndex; i < size - 1; i++)
					{
						customer[i] = customer[i + 1];
						checkaccount[i] = checkaccount[i + 1];
					}
					size--;
					CheckingAccount::saveAccountData();
					cout << "\t\t\t\tAccount deleted successfully... at " << getCurrentTime() << endl;
				}
			}

			else
			{
				cout << "\t\t\tAccount not found." << endl;
			}
		}
	}

	void saveAccountData()
	{
		Out.open(checkdatafile, ios::out);

		if (!Out)
		{

			return;
		}
		Customer *cust;
		CheckingAccount *checkacc;
		for (int i = 0; i < size; i++)
		{
			cust = customer[i];
			checkacc = checkaccount[i];
			Out << left << setw(20) << checkacc->getFirstName()
					<< left << setw(20) << checkacc->getLastName()
					<< left << setw(20) << cust->getname()
					<< left << setw(20) << cust->getpassword()
					<< left << setw(20) << checkacc->getAccountNumber()
					<< left << setw(20) << checkacc->getBalance()
					<< left << setw(20) << checkacc->getcardtype()
					<< left << setw(30) << checkacc->getemail()
					<< left << setw(30) << checkacc->getphone()
					<< endl;
		}

		Out.close();
	}
	void display() const
	{
		cout << "Account Type: Checking" << endl;
		cout << setw(50) << "=====================================CUSTOMER FOR CHECKING ACCOUNT LIST================================" << endl;
		cout << left << setw(15) << "A/C"
				 << left << setw(18) << "Name"
				 << left << setw(15) << "No. Account"
				 << left << setw(20) << "Username"
				 << left << setw(15) << "Card Type"
				 << left << setw(30) << "Email Address"
				 << left << setw(30) << "Phone No."
				 << left << setw(5) << "Balance"
				 << endl;

		for (int i = 0; i < size; i++)
		{
			cout << left << setw(15) << "Checking"
					 << left << setw(18) << checkaccount[i]->getFirstName() + " " + checkaccount[i]->getLastName()
					 << left << setw(15) << checkaccount[i]->getAccountNumber()
					 << left << setw(20) << customer[i]->getname()
					 << left << setw(15) << checkaccount[i]->getcardtype()
					 << left << setw(30) << checkaccount[i]->getemail()
					 << left << setw(30) << checkaccount[i]->getphone()
					 << left << setw(5) << checkaccount[i]->getBalance()
					 << endl;
		}
	}
	~CheckingAccount()
	{
		CheckingAccount::saveAccountData();
		if (checkaccount != nullptr)
		{
			delete[] checkaccount;
		}
		if (customer != nullptr)
		{
			delete[] customer;
		}
	}
};
void aboutus()

{
	cout << setw(100) << "==================================================" << endl;
	cout << "\t\t\t\t\t\t\tAbout US " << endl;
	cout << setw(100) << "==================================================" << endl
			 << endl;
	cout << "\t\t\t\t\tIt is a Simple Bank Management System Project for our ";
}
int CountLinesInFile(const string &filename)
{
	ifstream file(filename);
	int count = 0;
	string line;
	while (getline(file, line))
	{
		if (!line.empty())
		{
			count++;
		}
	}
	file.close();
	return count;
}


int main()
{

	system("cls");

	cout << setw(100) << "==================================================" << endl;
	cout << setw(100) << "            Welcome to the Bank Managament System!         " << endl;
	cout << setw(100) << "==================================================" << endl;
	int S_Size = 0, C_Size = 0, St_Size = 0, P_Size = 0, R_Size = 0;
	S_Size = CountLinesInFile("save.txt");
	C_Size = CountLinesInFile("check.txt");

	Account* SA = new SavingsAccount("save.txt", S_Size);
	Account* CA = new CheckingAccount("check.txt", C_Size);
	bool AloggedIn = false;
	bool UloggedIn = false;

	int choice;
	cout << "\t\t\t\t\t\t\t\tAccount Type \n";
	cout << endl;
	cout << setw(60) << " \t\t                  [1] . Administrator\n";
	cout << setw(50) << "\t\t  [2] . User" << endl;
	cout << endl;
	cout << "\t\t\t\t\t\t\t\tEnter your Choice...!";
	cin >> choice;
	char x;
	switch (choice)
	{
	case 1:

		system("cls");
		AloggedIn = true;
		while (AloggedIn)
		{

			system("cls");
			cout << "\n\n";
			cout << "\t\t\t\t\t\t==================================================" << endl;
			cout << "\t\t\t\t\t\t\t\tWelcome to Admin Portal" << endl;
			cout << "\t\t\t\t\t\t==================================================" << endl;

			cout << "\n\t\t\t\t\t\t[1] . Saving account: \n";
			cout << "\t\t\t\t\t\t[2] . Checking account: \n";
		/* 	cout << "\t\t\t\t\t\t[3] . About US \n"; */
			cout << "\t\t\t\t\t\t[4] . Log Out !!! \n";
			cout << "\n\t\t\t\t\t\tSelect one option...!";
			int adchoice;
			cin >> adchoice;
			switch (adchoice)
			{
			case 1:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tSaving Account Admin Potral\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\n\t\t\t\t\t\tSelect one Option\n";
				cout << "\n\t\t\t\t\t\t[1] . View Customer Accounts\n";
				cout << "\t\t\t\t\t\t[2] . Customer Account Registration\n";
				cout << "\t\t\t\t\t\t[3] . Edit Customer Account\n";
				cout << "\t\t\t\t\t\t[4] . Delete Customer Account\n";
				cout << "\t\t\t\t\t\t[5] . Search Customer Account\n";
				cout << "\n\t\t\t\t\t\tSelect one Option...!";
				int svechoice;
				cin >> svechoice;
				switch (svechoice)
				{
				case 1:
					system("cls");
					SA->display();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu..... ";
					cin >> x;
					break;
				case 2:
					system("cls");
					SA->Addaccount(S_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 3:
					system("cls");
					SA->updateaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 4:
					system("cls");
					SA->deleteaccount("save.txt", S_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 5:
					system("cls");
					SA->serachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				}
				break;
			case 2:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\tChecking Account Admin Potral" << endl;
				cout << "\t\t\t\t\t\t==================================================" << endl;

				cout << "\n\n\t\t\t\t\t\t[1] . View Customer Accounts\n";
				cout << "\t\t\t\t\t\t[2] . Customer Account Registration\n";
				cout << "\t\t\t\t\t\t[3] . Edit Customer Account\n";
				cout << "\t\t\t\t\t\t[4] . Delete Customer Account\n";
				cout << "\t\t\t\t\t\t[5] . Search Customer Account\n";
				cout << "\n\t\t\t\t\t\t\tSelect one Option...!";
				int checkchoice;
				cin >> checkchoice;
				switch (checkchoice)
				{
				case 1:
					system("cls");
					//CA->display();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					//CA->Addaccount(C_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 3:
					system("cls");
					//CA->updateaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 4:
					system("cls");
					//CA->deleteaccount("check.txt", C_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 5:
					system("cls");
					//CA->serachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				}
				break;
			case 5:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tStudent Loan Admin Potral" << endl;
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\n\t\t\t\t\t\tSelect one Option\n\n";
				cout << "\t\t\t\t\t\t[1] . View Student Accounts\n";
				cout << "\t\t\t\t\t\t[2] . Student Account Registration\n";
				cout << "\t\t\t\t\t\t[3] . Edit Student Account\n";
				cout << "\t\t\t\t\t\t[4] . Delete Student Account\n";
				cout << "\t\t\t\t\t\t[5] . Search Student Account\n";
				cout << "\n\t\t\t\t\t\tSelect one Option...!";
				int studchoice;
				cin >> studchoice;
				switch (studchoice)
				{
				case 1:
					system("cls");
				 	//S_L->display();
					cout << "\n\n";
					cout << "\t\t\t\t\tEnter any Character to goto  Main menu... ";
					cin >> x;
					break;
				case 2:
					system("cls");
				 //S_L->addloan();
					cout << "\n\n";
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu... ";
					cin >> x;
					break;
				case 3:
				 	//S_L->updateaccount();
					cout << "\n\n";
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu... ";
					cin >> x;
					break;
				case 4:
					system("cls");

				 	//S_L->deleteAccount("student.txt", St_Size);
					cout << "\n\n";
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 5:
					system("cls");

				 	//S_L->sreachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				}
				break;
			case 6:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tPersonal Loan Admin Potral" << endl;
				cout << "\t\t\t\t\t\t==================================================\n" << endl;

				cout << "\t\t\t\t\t\t[1] . View Personal Accounts\n";
				cout << "\t\t\t\t\t\t[2] . Personal Account Registration\n";
				cout << "\t\t\t\t\t\t[3] . Edit Personal Account\n";
				cout << "\t\t\t\t\t\t[4] . Delete Personal Account\n";
				cout << "\t\t\t\t\t\t[5] . Search Personal Account\n";
				cout << "\n\t\t\t\t\t\t\tSelect one Option...!";

				int per_choice;
				cin >> per_choice;
				switch (per_choice)
				{
				case 1:

					system("cls");
					//PL->display();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					//PL->addloan();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 3:
					system("cls");
					//PL->updateaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 4:
					system("cls");
					//PL->deleteAccount("personal.txt", P_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 5:
					system("cls");
					//PL->sreachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}
				break;
			case 7:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\tRelative Investment Admin Potral" << endl;
				cout << "\t\t\t\t\t\t==================================================\n" << endl;

				cout << "\t\t\t\t\t\t[1] . View Relative Investment Accounts\n";
				cout << "\t\t\t\t\t\t[2] . Relative Investment Account Registration\n";
				cout << "\t\t\t\t\t\t[3] . Edit Relative Investment Account\n";
				cout << "\t\t\t\t\t\t[4] . Delete Relative Investment Account\n";
				cout << "\t\t\t\t\t\t[5] . Search Relative Investment Account\n\n";
				cout << "\n\t\t\t\t\t\t\tSelect one Option...!";
				int invest_choice;

				cin >> invest_choice;
				switch (invest_choice)
				{

				case 1:
					system("cls");
					//RI->display();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					//RI->addinvest();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 3:
					system("cls");
					//RI->updateaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 4:
					system("cls");
					//RI->deleteAccount("invest.txt", R_Size);
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;

				case 8:
					system("cls");
					//RI->sreachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				}
				break;
			case 9:
				system("cls");
				aboutus();
				cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
				cin >> x;
				break;
			case 4:
				system("cls");
				AloggedIn = false;
				cout << "\n\n\t\t\t\t\t\t Logging Out\n\n";
				cout << "\t\t\t\t\t\tAdmin Logout at " << getCurrentTime() << endl;
				break;
			default:
				cout << "\t\t\t\t\tInvalid input" << endl;
				cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
				cin >> x;
				break;
			}
		}
		break;
	case 2:
		system("cls");
		UloggedIn = true;
		while (UloggedIn)
		{

			system("cls");
			cout << "\n\n";
			cout << "\t\t\t\t\t\t==================================================" << endl;
			cout << "\t\t\t\t\t\t\t\tUser Potral" << endl;
			cout << "\t\t\t\t\t\t==================================================" << endl;

			cout << "\t\t\t\t\t\t[1] . Saving account: \n";
			cout << "\t\t\t\t\t\t[2] . Checking account: \n";
	    cout << "\t\t\t\t\t\t[3] . About US \n";
			cout << "\t\t\t\t\t\t[4] . Log Out !!! \n";

			cout << "\n\t\t\t\t\t\t\tSelect one option...!";
			int userchoice;
			cin >> userchoice;
			switch (userchoice)
			{
			case 1:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tSaving Account Potral\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;

				cout << "\t\t\t\t\t\t[1] . Balance Inquiry\n";
				cout << "\t\t\t\t\t\t[2] . Cash Withdraw:\n";
				cout << "\t\t\t\t\t\t[3] . Cash Deposit:\n";
				cout << "\t\t\t\t\t\t\tSelect one Option...!";
				int sachoice;

				cin >> sachoice;
				switch (sachoice)
				{
				case 1:
					system("cls");
					SA->serachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					cout << "\t\t\t\t\t\tEnter the Ammount you want to withdraw";
					double amount;
					cin >> amount;
					SA->withdraw(amount);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 3:
					system("cls");
					cout << "\t\t\t\t\t\tEnter the Ammount you want to Deposit";
					double de_amount;
					cin >> de_amount;

					SA->deposit(de_amount);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}

				break;
			case 2:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tChecking Account Potral\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\n\t\t\t\t\t\t[1] . Balance Inquiry\n";
				cout << "\t\t\t\t\t\t[2] . Cash Withdraw:\n";
				cout << "\t\t\t\t\t\t[3] . Cash Deposit:\n";
				int ckchoice;
				cout << "\n\t\t\t\t\t\t\tEnter your Choice...";
				cin >> ckchoice;
				switch (ckchoice)
				{
				case 1:
					system("cls");
					cout << "\n\t\t\t\t======================================================================================\n";
					//CA->serachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu... ";
					cin >> x;
					break;
				case 2:
					system("cls");
					cout << "\n\t\t\t\t======================================================================================\n";
					cout << "\t\t\t\t\t\tEnter the Ammount you want to withdraw... ";
					double amount;
					cin >> amount;
					//CA->withdraw(amount);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu... ";
					cin >> x;
					break;
				case 3:
					system("cls");
					cout << "\n\t\t\t\t======================================================================================\n";
					cout << "\t\t\t\t\t\tEnter the Ammount you want to Deposit.. ";
					double de_amount;
					cin >> de_amount;

					//CA->deposit(de_amount);
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\n======================================================================================\n";
					cout << "\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}
				break;
			case 5:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tStudent Loan Potral\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\n\t\t\t\t\t\t[1] . Loan Inquiry\n";
				cout << "\t\t\t\t\t\t[2] . Calculate Intrest:\n";

				int loanchoice;
				cout << "\n\t\t\t\t\t\t\tEnter your choice...";
				cin >> loanchoice;
				switch (loanchoice)
				{
				case 1:
					system("cls");
				 	//S_L->sreachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto  Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
				 	//S_L->calculateInterest();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}
				break;
			case 6:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tPersonal Loan Potral\n";
				cout << "\t\t\t\t\t\t================================================\n" << endl;
				cout << "\t\t\t\t\t\t[1] . Loan Inquiry\n";
				cout << "\t\t\t\t\t\t[2] . Calculate Intrest:\n";

				int ploanchoice;
				cout << "\n\t\t\t\t\t\t\tEnter your choice...";
				cin >> ploanchoice;
				switch (ploanchoice)
				{
				case 1:
					system("cls");
					//PL->sreachaccount();
					cout << "\t\t\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					//PL->calculateInterest();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}
				break;
			case 7:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\t\t\t\tRelative Investment Potral" << endl;
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\n\t\t\t\t\t\t[1] . Investment Inquiry\n";
				cout << "\t\t\t\t\t\t[2] . Calculate Intrest:\n";
				int invectchoice;
				cout << "\n\t\t\t\t\t\t\tEnter your Choice..." << endl;
				cin >> invectchoice;
				switch (invectchoice)
				{
				case 1:
					system("cls");
					//RI->sreachaccount();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				case 2:
					system("cls");
					//RI->calculateReturns();
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				default:
					system("cls");
					cout << "\t\t\tInvalid Input" << endl;
					cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
					cin >> x;
					break;
				}
				break;
			case 3:
				system("cls");
				cout << "\n\n";
				cout << "\t\t\t\t\t\t==================================================" << endl;
				cout << "\t\t\t\t\tAbout US " << endl;
				cout << "\t\t\t\t\t\t==================================================" << endl << endl;
				aboutus();
				cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
				cin >> x;
				break;
			case 4:
				system("cls");
				cout << "\n\n\n";
				cout << "\t\t\t\t\t\tThanks for using our servies" << endl;
				cout << "\t\t\t\t\t\tUser Logout at " << getCurrentTime() << endl;
				UloggedIn = false;

				break;

			default:
				system("cls");
				cout << "\t\t\t\t\tInvalid Input" << endl;
				cout << "\t\t\t\t\t\tEnter any Character to goto Main menu...";
				cin >> x;
				break;
			}
		}
		break;
	default:
		system("cls");
		cout << "\t\t\t\t\t\tInvalid Input" << endl;
		break;
	}  
	  	system("cls");
	return 0;
}
