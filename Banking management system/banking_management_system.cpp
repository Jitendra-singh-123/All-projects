#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>
#include<windows.h>
using namespace std;



class Bankaccount
{
	int accno;
    char name[50];
	int deposit;
	char type;
public:
	void Openaccount()
{
	system("CLS");
	cout<<"\n\t\t\tEnter the Account No. : ";
	cin>>accno;
	cout<<"\n\n\t\t\tEnter the Name of the Account holder : ";
	cin.ignore();
	cin.getline(name,50);
	cout<<"\n\t\t\tEnter Type of the Account (C/S) : ";
	cin>>type;
	type=toupper(type);
	cout<<"\n\t\t\tEnter The Initial amount : ";
	cin>>deposit;
	cout<<"\n\n\t\t\tAccount Created..";
}

void displayaccount() const
{
	cout<<"\n\t\t\tAccount No. : "<<accno;
	cout<<"\n\t\t\tAccount Holder Name : ";
	cout<<name;
	cout<<"\n\t\t\tType of Account : "<<type;
	cout<<"\n\t\t\tBalance amount : "<<deposit;
}


void dep(int x)
{
	deposit+=x;
}

void draw(int x)
{
	deposit-=x;
}
void setwidth() const
{
	cout<<accno<<setw(28)<<" "<<name<<setw(24)<<" "<<type<<setw(21)<<deposit<<endl;
}

int getacno() const
{
	return accno;
}

int getdeposit() const
{
	return deposit;
}

char gettype() const
{
	return type;
}
};

void writeaccount()
{
	Bankaccount b;
	ofstream outFile;
	outFile.open("account.dat",ios::binary|ios::app);
	b.Openaccount();
	outFile.write(reinterpret_cast<char *> (&b), sizeof(Bankaccount));
	outFile.close();
}

void displaysp(int n)
{
	Bankaccount b;
	bool flag=false;
	ifstream inFile;
	inFile.open("account.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	cout<<"\n\t\t\tBALANCE DETAILS\n";
    	while(inFile.read(reinterpret_cast<char *> (&b), sizeof(Bankaccount)))
	{
		if(b.getacno()==n)
		{
			b.displayaccount();
			flag=true;
		}
	}
    inFile.close();
	if(flag==false)
		cout<<"\n\n\t\t\tAccount number does not exist";
}

void deleteaccount(int n)
{
	Bankaccount b;
	ifstream inFile;
	ofstream outFile;
	inFile.open("account.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	outFile.open("Temp.dat",ios::binary);
	inFile.seekg(0,ios::beg);
	while(inFile.read(reinterpret_cast<char *> (&b), sizeof(Bankaccount)))
	{
		if(b.getacno()!=n)
		{
			outFile.write(reinterpret_cast<char *> (&b), sizeof(Bankaccount));
		}
	}
    inFile.close();
	outFile.close();
	remove("account.dat");
	rename("Temp.dat","account.dat");
	cout<<"\n\nRecord Deleted ..";
}


void displayall()
{
	system("CLS");
	Bankaccount b;
	ifstream inFile;
	inFile.open("account.dat",ios::binary);
	if(!inFile)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	cout<<"\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tACCOUNT HOLDER LIST\n\n";
	cout<<"**********************************************************************************************************************\n";
	cout<<"A/c no.                           NAME                        Type             Balance\n";
	cout<<"**********************************************************************************************************************\n";

while(inFile.read(reinterpret_cast<char *> (&b), sizeof(Bankaccount)))
	{
		b.setwidth();
	}
	inFile.close();
}


void depositwithdraw(int n, int option)
{
	int amt;
	bool found=false;
	Bankaccount b;
	fstream File;
    File.open("account.dat", ios::binary|ios::in|ios::out);
	if(!File)
	{
		cout<<"File could not be open !! Press any Key...";
		return;
	}
	while(!File.eof() && found==false)
	{
		File.read(reinterpret_cast<char *> (&b), sizeof(Bankaccount));
		if(b.getacno()==n)
		{
			b.displayaccount();
			if(option==1)
			{
				cout<<"\n\n\t\t\tTO DEPOSITSS AMOUNT";
				cout<<"\n\n\t\t\tEnter The amount to be deposited: ";
				cin>>amt;
				b.dep(amt);
			}
		    if(option==2)
			{
				cout<<"\n\n\t\t\tTO WITHDRAW AMOUNT";
				cout<<"\n\n\t\t\tEnter The amount to be withdraw: ";
				cin>>amt;
				int bal=b.getdeposit()-amt;
				if(bal<0)
					cout<<"Insufficience balance";
				else
					b.draw(amt);
		      }
		 //     turns the unsigned result of the sizeof operator into a signed number, and multiplies it by -1.
			int pos=(-1)*static_cast<int>(sizeof(b));
			File.seekp(pos,ios::cur);
			File.write(reinterpret_cast<char *> (&b), sizeof(Bankaccount));
			cout<<"\n\n\t\t\tRecord Updated";
			found=true;
	       }
         }
    File.close();
	if(found==false)
		cout<<"\n\n\t\t\tRecord Not Found ";
}

int main()
{
	int ch;
	int num;
	while(1)
	{
	     system("color 0A");
	system("CLS");
	cout<<"\n**********************************************************************************************************************\n";
	cout<<"\t\t\tWELCOME TO PUNJAB NATIONAL BANKING SERVICE";
	cout<<"\n***********************************************************************************************************************\n";

		cout<<"\t\t\t\t    ::MAIN MENU::\n";
		cout<<"\n1. OPEN ACCOUNT";
		cout<<"\n2. DEPOSIT AMOUNT";
		cout<<"\n3. WITHDRAW AMOUNT";
		cout<<"\n4. BALANCE ENQUIRY";
		cout<<"\n5. ALL ACCOUNT HOLDER LIST";
		cout<<"\n6. CLOSE AN ACCOUNT";
		cout<<"\n7. EXIT";
		cout<<"\n\nSelect One option ";
		cin>>ch;

		switch(ch)
		{
            case 1:  writeaccount();
                                break;

		case 2:     system("CLS");
                        cout<<"\n\n\t\t\tEnter The account No. : "; cin>>num;
                        depositwithdraw(num, 1);
                        break;

		case 3:       system("CLS");
                            cout<<"\n\n\t\t\tEnter The account No. : "; cin>>num;
                            depositwithdraw(num, 2);
                            break;

		case 4:     system("CLS");
                            cout<<"\n\n\t\t\tEnter The account No. : "; cin>>num;
                            displaysp(num);
                            break;

		case 5:     displayall();
                        break;

		case 6:   system("CLS");
                        cout<<"\n\n\t\t\tEnter The account No. : "; cin>>num;
                        deleteaccount(num);
                        break;

		 case 7:      exit(1);
                            break;

		 default : cout<<"Invalid choice";
		}
		cin.ignore();
		cin.get();
    }
	return 0;
}
