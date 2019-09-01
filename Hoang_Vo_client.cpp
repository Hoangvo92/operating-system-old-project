#include <iostream>
#include <cctype>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <string>
#include <sstream>

using namespace std;


void decode1(int subcode[], int EM[], int EMi[])
{
	int i;
	for (i = 0; i < 4; i++)
	{
		EMi[i] = EM[i] * subcode[i];
	}
	for (i = 4; i < 8; i++)
	{
		EMi[i] = EM[i] * subcode[i - 4];
	}
	for (i = 8; i < 12; i++)
	{
		EMi[i] = EM[i] * subcode[i - 8];
	}
}
void divideCode(int arr[], int * a, int * b, int * c)
{
	int i;
	for (i = 0; i < 4; i++)
	{
		*a = *a + arr[i];
	}
	for (i = 4; i < 8; i++)
	{
		*b = *b + arr[i - 4];
	}
	for (i = 8; i < 12; i++)
	{
		*c = *c + arr[i - 8];
	}
	*a = *a / 4;
	*b = *b / 4;
	*c = *c / 4;
	if (*a == -1) *a = 0;
	if (*b == -1) *b = 0;
	if (*c == -1) *c = 0;
}


int decode2(int subcode[], int EM[])
{ /* the final function for decoding*/
	int EMi[12], a = 0, b = 0, c = 0;
	decode1(subcode, EM, EMi);
	divideCode(EMi, &a, &b, &c);
	return (a * 4 + b * 2 + c);

}

void stringToInt(char arr1[], int arr2[])
{

	// create an array with size as string
	// length and initialize with 0

	int j = 0, i;

	// Traverse the string
	for (i = 0; arr1[i] != '\0'; i++) {



		arr2[j] = arr1[i];
		j++;

	}
}
int main(int argc, char *argv[])
{
	char code[50], EM[50];
	int resultEM[12], resultCode[4];
	int ret, result;


	int sockfd, portno, msg, receiveV;


	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	//receive the infor from input file
	int pNum[3], intValue[3];
	int order = 0;
	while (cin >> pNum[order] >> intValue[order])
	{
		order = order + 1;
	}
	//connecting port from server
	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout << "ERROR opening socket";
		exit(0);
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		cout << "ERROR connecting";
		exit(0);
	}

	//create child processes
	pid_t pid;

	int childNum;
	for (childNum = 1; childNum < 4; childNum++)
	{
		if ((pid = fork()) == 0)  	// child process
		{


			//sending the information to client
			bzero(buffer, 256);
			printf("Child %d, sending value: %d to child process %d\n", childNum, intValue[childNum - 1], pNum[childNum - 1]);
			ret = sprintf(buffer, "%d %d", intValue[childNum - 1], pNum[childNum - 1]); //put childNum, value, and portnumber into a string
			msg = write(sockfd, buffer, strlen(buffer));
			if (msg < 0)
			{
				cout << "ERROR writing to socket";
				exit(0);
			}



			//receive the message - rewrite
			bzero(buffer, 256);
			receiveV = read(sockfd, buffer, 256);
			if (msg < 0)
			{
				cout << "ERROR writing to socket";
				exit(0);
			}


			bzero(code, 50);
			bzero(EM, 50);

			sscanf(buffer, "%s %s", code, EM);

			stringToInt(code, resultCode);
			stringToInt(EM, resultEM);
			result = decode2(resultCode, resultEM);
			printf("CHILD %d\n", childNum);
			printf("Signal: %s\n", EM);
			printf("Code:   %s\n", code);
			printf("Received Value = %d\n\n", result);

		}
	}
	wait(NULL);


	return 0;
}
