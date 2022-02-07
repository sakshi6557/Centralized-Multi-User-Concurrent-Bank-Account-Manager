#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<fstream>
#include<netdb.h>
#include<string>
#include<string.h>
#include<thread>
#include<vector>
#include<pthread.h>
#include<mutex>

std::mutex mutex_lock;  

#define MAXDATASIZE 256
//#define NUM_THREADS 10

using namespace std;


//static pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
//static int clientSocket;
std::vector<std::thread> threads;

int accno, recordno, balance[256], amount, accnumber[256], i = 0;
char* trans;
char operation[2];
string accounts, accname[256], line;
fstream records;
bool flag = true;

void perform_transaction( int clientSocket);
void add_interest(int recordno);

int main()
{

	// to calculate the number of records
	records.open("Records.txt");
	while(getline(records, line)){ recordno ++; }
	records.close();
	//records.seekg(0);	tried to use seekg() but it was not working properly, only one record was getting fetched from the file.
	
	//to get the records from "Records.txt" file and store it in different arrays
	records.open("Records.txt");
	for(i=0;i<recordno;i++)
	{
		records >> accnumber[i];  //fetching account number from file
		records >> accname[i];		
		records >> balance[i];
	}
	//closing the "Record.txt" file
	records.close();
	
	//creating a server socket
	int server = socket(AF_INET, SOCK_STREAM, 0);

	if ( server < 0)
	{
		cout << "Can't create a socket"<<endl;
		return -1;
	}

	// bind the socket to a IP/port
	sockaddr_in sock;
	sock.sin_family = AF_INET;
	sock.sin_addr.s_addr = INADDR_ANY;
	sock.sin_port =htons(54020);

	if(bind(server, (sockaddr*)&sock, sizeof(sock)) == -1)
	{
		cout <<"Cannot bind IP/port to socket"<<endl;
		return -2;
	}
	
	// mark the socket for listening in
	if(listen(server, SOMAXCONN) < 0)
	{
		cout << "Cannot listen"<<endl;
		return -3;
	}
	threads.push_back(std::thread(&add_interest, recordno));	//to add interest in all bank accounts

	//accept a call
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientSocket;

	while(clientSocket = accept(server, (sockaddr*)&client, &clientSize))	//accept the client connection
		{
		if(clientSocket < 0)
		{
			cerr << "Problem with client connection" <<endl;
			return -4;
		}
		else
		{
			cout << "\n Connected to the client successfully.." << endl;
		}

		//handle_transaction(clientSocket);  //for single client-server connection
		
		threads.push_back(std::thread(&perform_transaction, clientSocket));	//to implement the transaction
		/*
		Tried to implement using pthread
		 for(i=0; i<count;i++)
		 {
		 //handle_transaction(clientSocket);  //for single client-server connection
		 //threads.push_back(std::thread(&handle_transaction, clientSocket));
		output = pthread_create(&pthreads[i],NULL, handle_transaction,NULL);
		 i = count - 1;
		 }
		for(int j=0; j<NUM_THREADS; j++){  output = pthread_join(pthreads[j],0);}
		 */
		}
}
void add_interest(int recordno)
{
	sleep(60);
	cout << endl <<" Added 2 percent interest in all accounts"<<endl;
	for (i=0;i<recordno;i++)
	{
		mutex_lock.lock();
		balance[i] = balance[i] + balance[i]*0.02;	//adding 2 percent interest
		cout <<" "<< accnumber[i] <<" - "<< accname[i] << " - " << balance[i] <<endl;
		mutex_lock.unlock();
	}
	threads.push_back(std::thread(&add_interest, recordno));	//to add interest after every 60 seconds 
}

void perform_transaction( int clientSocket)	
{
	char buffer[4096];
	int byteRecv = 0, flag = 0;
	while(byteRecv = read(clientSocket, buffer, 4096))	// Receiving transaction from the client
	{
		if(byteRecv < 0)
		{
			cout << "error";
			break;
		}

		if(strlen(buffer) == 0)
		{
			continue;
		}

		// To display transaction request on server side
		cout << "\n Received transaction request from client is - "<< buffer  << endl;	//message received from client
		cout << " Thread number assigned to client is: " << pthread_self() <<endl;	//printing thread id assigned to the client
				trans = strtok(buffer, " ");	//to split the string at space
				trans = strtok(NULL, " ");
				if (trans) 
				{
					accno = atoi(trans);
				}

				trans = strtok(NULL, " ");
				if (trans) 
				{
					strcpy(operation, trans);
				}

				trans = strtok(NULL, " ");
		 		if (trans) 
				{
					amount = atoi(trans);
				}

		for (i=0;i<recordno;i++)
		{
			if (accno == accnumber[i])
			{	
				mutex_lock.lock();	//apply lock
				//Printing details found in Record.txt
				cout << "\n Found Account Number in Records" <<endl;
				cout << " The account details are as following-  " <<endl;
				cout << " Account Number: " << accnumber[i] <<endl;
				cout << " Name of Account Holder: " << accname[i] <<endl;
				cout << " Account Balance: " << balance[i] <<endl;
				
				if (strcmp(operation, "w") == 0)	//For withdrawal transaction
				{
					if (balance[i] - amount > 0)	//if user has sufficient balance to perform the withdrawal transaction
					{
						balance[i] = balance[i] - amount;	//withdrawl operation
						cout<<" "<< amount << " withdrawn from account" <<endl;
						cout<<" Current account balance is "<< balance[i]<<"" <<endl;

						cout << " Sending acknowldgement of withdrawal to the client.." <<endl;
						byteRecv = write(clientSocket, " Amount withdrawn from account successfully! ", 50);	// Sending response to the client
						if (byteRecv < 0)
						{
							cerr<< " Error while sending message to client" <<endl;
						}

					}
					else	//if account balance is insufficient
					{
						cout<<"\n Insufficient balance in account to perform the transaction!"<<endl;
						cout<<" Current account balance is "<< balance[i]<<"" <<endl;
						
						cout << " Sending acknowldgement" <<endl;
						byteRecv = write(clientSocket, " Insufficient balance - Couldn't withdraw the amount!", 100);	// Sending response to the client
						if (byteRecv < 0)
						{
							cerr << " Error while sending message to client"<<endl;
						}
					}
				}
				else if (strcmp(operation, "d") == 0)	//For deposit transaction
				{

					balance[i] = balance[i] + amount;	//deposit operation
					cout<<" "<< amount << " is deposited in account"<<endl;
					cout<<" Current account balance is " << balance[i] <<endl;

					cout << " Sending acknowldgement of deposit to the client.. "<<endl;
					byteRecv = write(clientSocket, " Amount deposited in account successfully! ", 50);	// Sending response to the client
					if (byteRecv < 0)
					{
						cerr << " Error while sending message to client"<<endl;
					}
				}	
				else	// If the operation in client requested transaction is invalid
				{
					cout << " Invalid transaction type! Please enter a new transaction type. " <<endl;
					
					cout << " Sending acknowldgement" <<endl;
					byteRecv = write(clientSocket, " Invalid operation type", 50);	// Sending response to the client
					if (byteRecv < 0)
					{
						cerr << " Error while sending message to client"<<endl;
					}
				}	
				mutex_lock.unlock();	//release the lock
				flag = 1;
			}
		}
		if (flag == 0)	// If the account number in client requested transaction is invalid
		{
			cout<<"\n Couldn't found the account! \n"<<endl;
			
			cout << " Sending acknowldgement" <<endl;
			byteRecv = write(clientSocket, "Account not found, Try again!", 50);	// Sending response to the client
			if (byteRecv < 0)
				cerr << " Error writing to socket"<<endl;
		}        		
	}
}

