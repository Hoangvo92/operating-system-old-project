#include <iostream>
#include <cctype>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <string>
#include <sstream>

using namespace std;

struct subcode {
	int key[4];
};


void fillArray3(int arr3[], int a, int b, int c)
{
	arr3[0] = a;
	arr3[1] = b;
	arr3[2] = c;
}
void fillArray4(int arr4[], int a, int b, int c, int d)
{
	arr4[0] = a;
	arr4[1] = b;
	arr4[2] = c;
	arr4[3] = d;
}
void binaryArr(int value, int arr[])
{
	switch (value) {
	case 0:
		fillArray3(arr, -1, -1, -1);
		break;
	case 1:
		fillArray3(arr, -1, -1, 1);
		break;
	case 2:
		fillArray3(arr, -1, 1, -1);
		break;
	case 3:
		fillArray3(arr, -1, 1, 1);
		break;
	case 4:
		fillArray3(arr, 1, -1, -1);
		break;
	case 5:
		fillArray3(arr, 1, -1, 1);
		break;
	case 6:
		fillArray3(arr, 1, 1, -1);
		break;
	case 7:
		fillArray3(arr, 1, 1, 1);
		break;
	default:
		printf("Error");
		break;
	}
}

void walshCode(int processNum, int arr[])
{
	switch (processNum) {
	case 1:
		fillArray4(arr, -1, 1, -1, 1);
		break;
	case 2:
		fillArray4(arr, -1, -1, 1, 1);
		break;
	case 3:
		fillArray4(arr, -1, 1, 1, -1);
		break;
	default:
		printf("Error");
		break;
	}

}
void arrayMul(int a, int arr4[], int length)
{
	int i;
	for (i = 0; i < length; i++)
	{
		arr4[i] = a * arr4[i];
	}
}
void merge3Arr(int subEm[], int arr1[], int arr2[], int arr3[], int len)
{
	int i;
	for (i = 0; i < len; i++) {
		subEm[i] = arr1[i];
	}
	for (; i < (2 * len); i++) {
		subEm[i] = arr2[i - len];
	}
	for (; i < (3 * len); i++) {
		subEm[i] = arr3[i - 2 * len];
	}
}
void encodeSub(int value, int cpid, int EM[])
{
	int arr3[3];
	binaryArr(value, arr3);
	int subarr1[4];
	int subarr2[4];
	int subarr3[4];
	walshCode(cpid, subarr1);
	walshCode(cpid, subarr2);
	walshCode(cpid, subarr3);
	arrayMul(arr3[0], subarr1, 4);
	arrayMul(arr3[1], subarr2, 4);
	arrayMul(arr3[2], subarr3, 4);
	merge3Arr(EM, subarr1, subarr2, subarr3, 4);
}

void encode(int value[], int EM[])
{ /* Input: the array of integer with
				  +Index order: the order of process
				  +each item is the integer value for the process
  */
	int EM1[12], EM2[12], EM3[12], i;
	encodeSub(value[0], 1, EM1);
	encodeSub(value[1], 2, EM2);
	encodeSub(value[2], 3, EM3);
	for (i = 0; i < 12; i++)
	{
		EM[i] = EM1[i] + EM2[i] + EM3[i];
	}

}

string intToString(int number[], int size)
{/*size: size of the integer array number
	convert into to char*/
	ostringstream os;
	for (int i = 0; i < size; i++)
	{
		os << number[i];
	}
	string str(os.str());

	return str;


}

/*void intToChar(int number[], char arr[], int sizeNum)
{/*sizeNum: size of the integer array number
	convert into to char*/
/*	int n, i;
	for (i = 0; i < sizeNum; i++)
	{
		n = sprintf(arr, "%d", number[i]);
	}
	n = sprintf(arr, "\0");
}*/
int main(int argc, char *argv[])
{
	//declare variables for saving messages
	int child = 0, value, des;
	int current; //keep track of the processNumber for response

	//encodeArr stores values in order of childprocess
	//destination store values in order of process receive
	//EM will save the encoding message
	int encodeArr[3] = { -1,-1,-1 }, //store values sent by child process to encode
		destination[3], //save the value received from child process
		fromChild[3], //save sender
		EM[12];   //save the encoding message
	struct subcode* walsh[3];  //save the walshcode key

	//server
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n, r;
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		cout << "ERROR opening socket";
		exit(1);
	}

	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
	{
		cout << "ERROR on Binding";
		exit(1);

	}

	listen(sockfd, 5);
	clilen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *)&clilen);
	if (newsockfd < 0)
	{
		cout << "ERROR on Accept";
		exit(1);
	}


	//read message from socket
	for (child = 0; child < 3; child++)
	{

		bzero(buffer, 256);
		n = read(newsockfd, buffer, 255);
		if (n < 0)
		{
			cout << "ERROR reading from socket";
			exit(1);
		}

		sscanf(buffer, "%d %d", &value, &des);
		encodeArr[child] = value;
		destination[des - 1] = value;
		fromChild[des - 1] = child + 1;
		printf("Here is the message from child %d: Value = %d, Destination= %d\n", (child + 1), value, des);
	}


	if ((encodeArr[0] != -1) && (encodeArr[1] != -1) && (encodeArr[2] != -1))
	{
		encode(encodeArr, EM);
		for (int e = 0; e < 3; e++)
		{
			walshCode(fromChild[e], walsh[e]->key);
		}
	}
	//send encoded messages
	string output1 = "", output2 = "";

	bzero(buffer, 256);
	//convert the subcode and the encoded result to string child 1
	output1= intToString(walsh[0]->key, 4);
	output2= intToString(EM, 12);
	//copy the two result into buffer
	r = sprintf(buffer, "%s %s", const_cast<char *>(output1.c_str()), const_cast<char *>(output2.c_str()));
	n = write(newsockfd, buffer, 256);
	if (n < 0)
	{
		cout << "ERROR writing to socket";
		exit(1);
	}

	output1 = "";
	bzero(buffer, 256);
	//convert the subcode to string child 2
	output1= intToString(walsh[1]->key,  4);
	//copy the two result into buffer
	r = sprintf(buffer, "%s %s", const_cast<char *>(output1.c_str()), const_cast<char *>(output2.c_str()));
	n = write(newsockfd, buffer, 256);
	if (n < 0)
	{
		cout << "ERROR writing to socket";
		exit(1);
	}
	output1 = "";
	bzero(buffer, 256);
	//convert the subcode and the encoded result to string child 3
	output1 = intToString(walsh[2]->key, 4);
	//copy the two result into buffer
	r = sprintf(buffer, "%s %s", output1, output2);
	n = write(newsockfd, buffer, 256);
	if (n < 0)
	{
		cout << "ERROR writing to socket";
		exit(1);
	}

	return 0;
}
