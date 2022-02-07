#include <iostream>
#include <arpa/inet.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<netdb.h>
#include<fstream>
#include<string>
#include<string.h>

using namespace std;

int main()
{

fstream transactions;

//Create a socket
int clientsock = socket(AF_INET, SOCK_STREAM, 0);
if (clientsock < 0)
{
	return -1;
}

//create structure for the server we are connecting with
int port =  54020;
string ipAddress = "127.0.0.1";
sockaddr_in serv;
serv.sin_family = AF_INET;
serv.sin_port = htons(port);
inet_pton(AF_INET, ipAddress.c_str(), &serv.sin_addr);

//connect to the server on the socket
int connectServ = connect(clientsock, (sockaddr*)&serv, sizeof(serv));
if (connectServ < 0)
{
	return -2;
}

char buf[4096];
string operation;

transactions.open("Transactions.txt");	//open the "Transaction.txt" file
while(getline(transactions, operation))
{
	sleep(3);	//client will wait for 3 seconds before sending next transaction to the server
	cout << endl;
	cout << "Sending transaction details - "<< operation <<endl;
	int sendRes = write(clientsock, operation.c_str(), operation.size() +1);	//sending one transaction at a time to the server
	if (sendRes < 0)
	{
		cout<< "Could not send to server! \r\n";
		exit(1);
	}
	memset(buf, 0, 4096);	//Resetting the buffer to get the data from server
	int bytesReceived = read(clientsock, buf,4096);	//to receive data from the server
	if (bytesReceived < 0)
	{
		cout << "There was an error getting response from server \r\n";
	}
	else
	{
		cout << "Acknowledgement Message from Server :" << buf <<endl;
	}
}
close(clientsock);	//closing the client socket
transactions.close();	//to close the file
}
