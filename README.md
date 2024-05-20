# Centralized-Multi-User-Concurrent-Bank-Account-Manager
	
•	Project Design:

In this project, I have implemented a Centralized Multi-User Concurrent Bank Account Manager. The system has two important components.
1.	Bank Server: The server program implements the withdrawal and deposit transactions requested by the client using threads and locks. It also adds 2% interest in all bank accounts after every 60 seconds.
2.	Clients: It sends requests to the Server to perform withdrawal and deposit transactions in the bank account.


•	Steps to execute and run the project:

1.	Open two windows Terminal-1 and Terminal-2 for Server and Client respectively
2.	Compile the code on Terminal-1 by using the command:		make
3.	Run the server on Terminal-1 by using:	./server	
4.	On Terminal-2 execute the shell script:	bash script.sh


•	Working:

The server first reads the records from the “Record.txt” file and adds those records in accnumber[], accname[], and balance[] respectively. After that both server and client create sockets. On the server side, I have used the bind() and listen() functions and on the client side, I have used the connect() function to establish a connection between them. Once the connection is accepted on the server side, a thread is assigned to that client to implement the transaction by calling the handle_transaction() method, and the “Transaction.txt” file is opened on the client side to send the transaction request to the server one by one.
In handle_transaction(), the transaction request is received from the client and then split into accno, operation, and amount using strtok() function. After that, if the accno is found in the accnumber[] array, the requested operation is performed and an acknowledgment is sent to the client for the performed transaction.
Along with this server adds interest in all accounts every 60 seconds by implementing the add_interest() method.


•	Assumptions and Limitations:

Here I have hard-coded the port number in the server code and used that in the client code to connect, because of this if that port is not available at a given time the application will not work. I consider this as one of the limitations of my application. 
Second, I had to open and close the Records.txt file 2 times, once to calculate the number of records and a second time to fetch the records from the file. For this, I tried to use seekg() but it was not working properly. I’ll try to improve this in the next version.


•	Output:

![server_output](https://github.com/sakshi6557/Centralized-Multi-User-Concurrent-Bank-Account-Manager/assets/20897894/df3e540f-0dc0-4cc4-ab42-89a56d84cf09)
![client_output](https://github.com/sakshi6557/Centralized-Multi-User-Concurrent-Bank-Account-Manager/assets/20897894/fd4c2348-0bec-448b-8265-92a5c766c77b)



•	Results:

	Test1: Execution time taken per Client vs Number of Clients
 
Each client sends a transaction request after 2 seconds. Started with 10 clients spawned simultaneously. Here each client is going to implement 10 transactions.
The results show that maximum time is taken when the program is executed for the first time with 10 clients connected simultaneously and when the clients number reaches 50, the graph becomes more linear.
![image](https://github.com/sakshi6557/Centralized-Multi-User-Concurrent-Bank-Account-Manager/assets/20897894/ea9b3f86-b5f3-4205-9ec7-73acc51acdc3)

	Test2: Fixed Number of Clients per Client vs Varying Request Rate

The number of clients is fixed to be 25 and the number of transactions to 10 for each client. Increasing the time of request in client code by 0.1 every time. The result of the graph is as shown below:
![image](https://github.com/sakshi6557/Centralized-Multi-User-Concurrent-Bank-Account-Manager/assets/20897894/e0e42c77-5fc7-48da-befd-4ad3806bb3a7)


•	Referred links:

1.	https://stackoverflow.com/questions/14287942/cannot-convert-void-networkvoid-to-void-void -  to check how to use pthread but instead I found C++11 std::thread in this link.
2.	https://www.javatpoint.com/how-to-split-strings-in-cpp - to split the string.

