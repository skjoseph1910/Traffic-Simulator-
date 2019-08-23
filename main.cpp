#include <unistd.h>
#include <iostream>
#include <queue>
#include <sys/wait.h>
#include <sstream>
using namespace std;

struct car
{
    string plate;
    char d;
    int sec;
};

car store(string line)
{
    car cars;
    istringstream ss(line);
    string token, arr[3];
    int i=0;
    while (getline(ss, token, ' '))
    {
        arr[i]=token;
        i++;
    }
    cars.plate=arr[0];
    cars.d=arr[1][0];
    i=atoi(arr[2].c_str());
    cars.sec=i;
    return cars;
}

queue<car> organizeq(queue<car> q, char dir)
{
    queue<car> newq;
    while (!q.empty())
    {
        if (q.front().d==dir)
            newq.push(q.front());
        q.pop();
    }
    return newq;
}

char trafficUtil (queue<car> &dirQueue, char dir, int maxCars, int dirNum)
{
    string directions[] = {"Norhtbound", "Southbound", "Westbound", "Eastbound"};
    for (int i = 0;i < maxCars; i++)
    {
        if (!dirQueue.empty())
        {
            if (i==0)
                cout<<"Current direction: "<< directions[dirNum] <<endl;
            int f=fork();
            if (f==0)
            {
                cout<<"Car "<<dirQueue.front().plate<<" is using the intersection for "<<dirQueue.front().sec<<" sec(s)."<<endl;
                sleep(dirQueue.front().sec);
                exit(0);
            }
            else
                waitpid(-1,NULL,0);
            dirQueue.pop();
        }
    }
    if(dir == 'N')
        return 'E';
    else if(dir == 'E')
    {
        return 'S';
    }
    else if (dir == 'S')
        return 'W';
    else if(dir == 'W')
        return 'N';
    else
        return '\0';
}

void traffic(queue<car> e, queue<car> w, queue<car> n, queue<car> s, char dir, int num)
{
    while (!e.empty() || !n.empty() || !w.empty() || !s.empty())
    {
        if (dir=='N')
            dir = trafficUtil(n, dir,num, 0);
        else if (dir=='E')
            dir=trafficUtil(e, dir, num, 3);
        else if (dir=='W')
            dir=trafficUtil(w, dir, num, 2);
        else if (dir=='S')
            dir=trafficUtil(s, dir, num, 1);
    }
}


int main() 
{
    string direction, numofcar;
    getline(cin, direction);
    getline(cin, numofcar);

    car cars;
    queue<car> que, south, east, west, north;
    string line;
    while (getline(cin, line))
    {
        if (line=="")
            continue;
        cars=store(line);
        que.push(cars);
    }
    
    north=organizeq(que, 'N');
    south=organizeq(que, 'S');
    east=organizeq(que, 'E');
    west=organizeq(que, 'W');
    int i=atoi(numofcar.c_str());
    
    
    traffic(east, west, north, south, direction[0], i);

    return 0; 
} 
