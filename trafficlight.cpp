
#include <iostream>

#include <cctype>
#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <sstream>

using namespace std;
struct car {

	string plate;
	char dir;
	int time;
	string strdir;


};
int find_index(car [], int , char );
void OrderTraffic( car [], car [], int , int & ,
                   int & , char , char , char , char );
void sortarray(car [], car [], int , int & , char );
int main() {
    // to produce child functions
	pid_t pid;
	int i, l;

	//to store input information of cars, i use array with a certain limit numbers
	car inputcar[20];
	// order will keep track of the true length  of the input
	int order = 0;//
	int sizeOfList;//
	//the temporary variables
	string licence;
	char dir;
	int number;
    //The first direction
	char direction;
    int maxnum;//
	cin >> direction;  //current direction

	cin >> maxnum;     //maximum number of cars
    int loopNum;
	loopNum = maxnum; //save the maximum number of car allowed in the first direction
	while(cin>>licence>>dir>>number)
        {
		inputcar[order].plate = licence;
		inputcar[order].dir = dir;
		inputcar[order].time = number;
		order += 1;
	}
	sizeOfList = order;
    //New array to store sorted cars
	car orderOfCar [sizeOfList];//
    //Sort the input array
    sortarray(orderOfCar, inputcar, sizeOfList, maxnum, direction);

  //running fork() to form child processes
	for (i = 0; i < sizeOfList; i++)
    {     if ((i==0) || i>= (maxnum))
          {
         cout<<"Current direction: "<< orderOfCar[i].strdir<<endl;
          }
          if(( pid = fork())==0)   //child process
           {
               cout<<"Car "<< orderOfCar[i].plate << " is using the intersection for " << orderOfCar[i].time << " sec(s)"<<endl;
               sleep(orderOfCar[i].time);
               break; //finish executing the child process
           }
    wait(NULL);//wait until the program execute
    }


    return 0;
}


int find_index(car arr[], int num_elements, char searchkey)
{
   int i;
   for (i=0; i<num_elements; i++)
   {
	 if (arr[i].dir == searchkey)
	 {
	    return i;  /* it was found */
	 }
   }

    return -1; /* if it was not found */

}
void OrderTraffic( car orderOfCar[], car inputcar[], int sizeOfList, int & count2,
                   int & index, char dir1, char dir2, char dir3, char dir4)
{
    index = find_index(inputcar, sizeOfList, dir1);
    if (index!= -1)
        {
         orderOfCar[count2].dir = inputcar[index].dir;
         orderOfCar[count2].time = inputcar[index].time;
         orderOfCar[count2].plate = inputcar[index].plate;;
         inputcar[index].dir = 'O';
         count2 +=1;
         index = -1; //update index to the unused value

        }
    else
        {
        index = find_index(inputcar, sizeOfList, dir2);
        if (index!= -1)
           {
            orderOfCar[count2].dir = inputcar[index].dir;
            orderOfCar[count2].time = inputcar[index].time;
            orderOfCar[count2].plate = inputcar[index].plate;
            inputcar[index].dir = 'O';
            count2 +=1;
            index = -1; //update index to the unused value

            }
        else
            {
            index = find_index(inputcar, sizeOfList, dir3);
            if (index!= -1)
                {
                orderOfCar[count2].dir = inputcar[index].dir;
                orderOfCar[count2].time = inputcar[index].time;
                orderOfCar[count2].plate = inputcar[index].plate;
                inputcar[index].dir = 'O';
                count2 +=1;
                index = -1; //update index to the unused value

                }
            else{
                index = find_index(inputcar, sizeOfList, dir4);
                if (index!= -1)
                  {
                    orderOfCar[count2].dir = inputcar[index].dir;
                    orderOfCar[count2].time = inputcar[index].time;
                    orderOfCar[count2].plate = inputcar[index].plate;
                    inputcar[index].dir = 'O';
                    count2 +=1;
                    index = -1; //update index to the unused value

                   }
                 }

            }

        }
}

void sortarray(car orderOfCar[], car inputcar[], int sizeOfList, int & maxnum, char direction)
    {
        int order = sizeOfList;
        int count2 = 0;
	    int index = -1;
        int loopcount = 0;
        int loopNum = maxnum;//

	    while (loopNum !=0)
        {
          index = find_index(inputcar, sizeOfList, direction);

          if (index!= -1 && index<sizeOfList){ //
            orderOfCar[count2].dir = inputcar[index].dir;
            orderOfCar[count2].time = inputcar[index].time;
            orderOfCar[count2].plate = inputcar[index].plate;

            inputcar[index].dir = 'O';
            count2 +=1;
            loopcount += 1;//count number of possible cars
            index = -1; //update index to the unused value

            }
          else{

            break;
            }
          loopNum -= 1;

        }

         maxnum = loopcount; //Update the real number of cars could pass through the first direction
         order = order - loopcount;


	    while (order!= 0)
        {
          if (count2!=0)
            {
            if (orderOfCar[count2-1].dir=='E')
             {
            OrderTraffic( orderOfCar, inputcar, sizeOfList, count2,
                   index, 'S', 'W', 'N','E');
             }
            else if (orderOfCar[count2-1].dir =='S')
             {
            OrderTraffic( orderOfCar, inputcar, sizeOfList, count2,
                   index, 'W', 'N', 'E', 'S');
             }
            else if (orderOfCar[count2-1].dir =='W')
             {
            OrderTraffic( orderOfCar, inputcar, sizeOfList, count2,
                   index, 'N', 'E', 'S', 'W');

             }
            else if (orderOfCar[count2-1].dir =='N')
            {
            OrderTraffic( orderOfCar, inputcar, sizeOfList, count2,
                   index, 'E', 'S', 'W', 'N');
            }

           }
           else
           {
             orderOfCar[count2].dir = inputcar[count2].dir;
             orderOfCar[count2].time = inputcar[count2].time;
             orderOfCar[count2].plate = inputcar[count2].plate;
             inputcar[count2].dir = 'O';
             count2 +=1;

            }
           order -= 1;


        }

        for (int x=0; x< sizeOfList;x++)
        {
           switch (orderOfCar[x].dir)
           {
           case 'E':
               orderOfCar[x].strdir = "Eastbound";
               break;
           case 'S':
               orderOfCar[x].strdir = "Southbound";
               break;
           case 'N':
               orderOfCar[x].strdir = "Northbound";
               break;
           case 'W':
               orderOfCar[x].strdir = "Westbound";
               break;
           default :
               orderOfCar[x].strdir = " ";
               break;
            }


        }
        return;

    }
