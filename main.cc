// Mazen Yasser Elsayed 7234
// Shereen Mostafa Hassan Mabrouk 6844
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <list>
#include<math.h>
#include <sstream>
#include<iomanip>
#include <bits/stdc++.h>
using namespace std;
// Define a structure called process
typedef struct
{
    int start;
    int complete_time;
    float turnaround_time;
    float normturn_time;
    int wait_time;
    string name;
    int arrival_time;
    int service_time;
    int id;
    int waiting_time;
    int working_slots;
} Process;

typedef struct
{
    int name;
    int q;
} Algo;
// stoi function for old version c++
/*
// Converting string to integer
int stoi(string s)
{
    int num = 0;
    int n = s.length();
    for (int i = 0; i < n; i++)
        num = num * 10 + (int(s[i]) - 48);
    return num;
}*/
// Getting length of string
int len(string str)
{
    int length = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        length++;
    }
    return length;
}
// Get number of processes
int getNoOfProcesses()
{
    ifstream myfile ("input.txt");
    int current_line = 0;
    string line2;
    while (!myfile.eof())
    {
        current_line++;
        getline(myfile, line2);
        if (current_line == 4)
            break;
    }
    int noOfProcesses = stoi(line2);
    return noOfProcesses;
}
//Get number of algorithms
int getNoOfAlgos(string line)
{
    int noOfAlgos=0;
    for(int i=0; i<len(line); i++)
    {
        if(line[i]==',')
        {
            noOfAlgos++;
        }
    }
    return noOfAlgos+1;
}
/* Trace Code */
// Trace function of FCFS
void FCFSVisualizer(Process p[],int last,int noOfProcesses)
{
    cout<<"FCFS  ";
    int x=0;
    queue<Process> qpr;
    Process pr;
    int busy[last];
    for(int i=0; i<last; i++)
    {
        busy[i]=0;
    }
    for(int i=0; i<=last; i++)
    {
        cout<<i%10<<" ";
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int i=0; i<=last; i++)
    {
        for(int j=0; j<noOfProcesses; j++)
        {
            if(p[j].arrival_time==i)
            {
                qpr.push(p[j]);
            }
        }
    }
    while(!qpr.empty())
    {
        pr=qpr.front();
        qpr.pop();
        cout<<pr.name<<"     ";
        for(int i=0; i<=last; i++)
        {
            cout<<"|";
            if(pr.service_time!=0&&((pr.arrival_time<=i)))
            {
                if(busy[i]==1)
                {
                    cout<<".";
                    pr.arrival_time++;
                }
                else
                {
                    cout<<"*";
                    busy[i]=1;
                    pr.service_time--;
                }
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
}
// Trace Function of RoundRobin
void RoundRobinVisualizer(Process p[],int q,int last,int noOfProcesses)
{
    queue<Process> readyQueue;
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int currSlot=0;
    int processIndex=0;
    if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
    {
        readyQueue.push(p[processIndex]);
        processIndex++;
    }
    while(currSlot<last)
    {
        if(!readyQueue.empty())
        {
            Process pr=readyQueue.front();
            for (int i = 0; i < q && pr.service_time > 0; i++)
            {
                schedule[pr.id][currSlot]=1;
                pr.service_time--;
                currSlot++;
                if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
                {
                    readyQueue.push(p[processIndex]);
                    processIndex++;
                }
            }
            readyQueue.pop();
            if (pr.service_time > 0)
            {
                readyQueue.push(pr);
            }
        }
        else
        {
            currSlot++;
            if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
            {
                readyQueue.push(p[processIndex]);
                processIndex++;
            }
        }
    }
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    cout<<"RR-"<<q<<"  ";
    for(int i=0; i<=last; i++)
    {
        if(i==last)
        {
            cout<<i%10;
        }
        else
        {
            cout<<i%10<<" ";
        }
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int k=0; k<noOfProcesses; k++)
    {
        cout<<pp[k].name<<"     ";
        for(int l=0; l<last; l++)
        {
            cout<<"|";
            if(schedule[k][l]==1)
            {
                cout<<"*";
                pp[k].service_time--;
            }
            else if(pp[k].arrival_time<=l && pp[k].service_time!=0)
            {
                cout<<".";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"|";
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
}
// Trace Function of SPN
void SPNVisualizer(Process p[],int last,int noOfProcesses)
{
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int minimum=readyQueue[0].service_time;
            int currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(readyQueue[i].service_time<minimum)
                {
                    minimum=readyQueue[i].service_time;
                    currProcessIndex=i;
                }
            }
            while(readyQueue[currProcessIndex].service_time!=0)
            {
                schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
                readyQueue[currProcessIndex].service_time--;

                timeSlot++;
                for(int i=0; i<noOfProcesses; i++)
                {
                    if(p[i].arrival_time==timeSlot)
                    {
                        readyQueue.push_back(p[i]);
                    }
                }
            }
            readyQueue.erase(readyQueue.begin() + currProcessIndex);
        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    cout<<"SPN"<<"   ";
    for(int i=0; i<=last; i++)
    {
        cout<<i%10<<" ";
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int k=0; k<noOfProcesses; k++)
    {
        cout<<pp[k].name<<"     ";
        for(int l=0; l<last; l++)
        {
            cout<<"|";
            if(schedule[k][l]==1)
            {
                cout<<"*";
                pp[k].service_time--;
            }
            else if(pp[k].arrival_time<=l && pp[k].service_time!=0)
            {
                cout<<".";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"|";
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
// Trace Function of HRRN
}
float calcHRRN(int waiting_time,int service_time)
{
    return ((float)waiting_time + service_time) / (float)service_time;
}

void HRRNVisualizer(Process p[],int last,int noOfProcesses)
{
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int maximum=calcHRRN(readyQueue[0].waiting_time,readyQueue[0].service_time);
            int currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(calcHRRN(readyQueue[i].waiting_time,readyQueue[i].service_time)>maximum)
                {
                    maximum=calcHRRN(readyQueue[i].waiting_time,readyQueue[i].service_time);
                    currProcessIndex=i;
                }
            }
            while(readyQueue[currProcessIndex].service_time!=0)
            {
                schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
                readyQueue[currProcessIndex].service_time--;
                timeSlot++;
                for(int i=0; i<readyQueue.size(); i++)
                {
                    readyQueue[i].waiting_time++;
                }
                for(int i=0; i<noOfProcesses; i++)
                {
                    if(p[i].arrival_time==timeSlot)
                    {
                        readyQueue.push_back(p[i]);
                    }
                }
            }
            readyQueue.erase(readyQueue.begin() + currProcessIndex);
        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }
    Process pp[noOfProcesses];
    for(int i=0;i<noOfProcesses;i++)
    {
        pp[i]=p[i];
    }
    cout<<"HRRN"<<"  ";
    for(int i=0; i<=last; i++)
    {
        cout<<i%10<<" ";
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int k=0; k<noOfProcesses; k++)
    {
        cout<<pp[k].name<<"     ";
        for(int l=0; l<last; l++)
        {
            cout<<"|";
            if(schedule[k][l]==1)
            {
                cout<<"*";
                pp[k].service_time--;
            }
            else if(pp[k].arrival_time<=l && pp[k].service_time!=0)
            {
                cout<<".";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"|";
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";


}

// Trace Function of SRT
void SRTVisualizer(Process p[],int last,int noOfProcesses)
{
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int minimum=readyQueue[0].service_time;
            int currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(readyQueue[i].service_time<minimum)
                {
                    minimum=readyQueue[i].service_time;
                    currProcessIndex=i;
                }
            }
            schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
            readyQueue[currProcessIndex].service_time--;
            timeSlot++;
            for(int i=0; i<noOfProcesses; i++)
            {
                if(p[i].arrival_time==timeSlot)
                {
                    readyQueue.push_back(p[i]);
                }
            }
            if(readyQueue[currProcessIndex].service_time<=0)
            {
                readyQueue.erase(readyQueue.begin() + currProcessIndex);
            }
        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    cout<<"SRT"<<"   ";
    for(int i=0; i<=last; i++)
    {
        cout<<i%10<<" ";
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int k=0; k<noOfProcesses; k++)
    {
        cout<<pp[k].name<<"     ";
        for(int l=0; l<last; l++)
        {
            cout<<"|";
            if(schedule[k][l]==1)
            {
                cout<<"*";
                pp[k].service_time--;
            }
            else if(pp[k].arrival_time<=l && pp[k].service_time!=0)
            {
                cout<<".";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"|";
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
}
//Trace Function of Feedback
int noOfProcessesFeedback(deque<deque<Process> >readyQueue)
{
    int noOfProcesses = 0;
    for(int i = 0; i< readyQueue.size() ; i++)
    {
        if(!readyQueue[i].empty())
        {
            noOfProcesses = noOfProcesses + readyQueue[i].size();
        }
    }
    return noOfProcesses;
}
void feedbackVisualizer(Process p[],int q,int last,int noOfProcesses,int depth)
{
    int twoPowi=0;
    if(q==0)
    {
        twoPowi=1;
    }
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    deque<deque<Process> >readyQueue(depth);
    int currSlot=0;
    int processIndex=0;
    int currQueue=-1;
    if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
    {
        readyQueue[0].push_back(p[processIndex]);
        processIndex++;
    }
    while(currSlot<last)
    {
        for(int i=0; i<depth; i++)
        {
            if(!readyQueue[i].empty())
            {
                currQueue=i; //Get the queue with highest priority to dispatch from it
                break;
            }
        }
        if(currQueue==-1)
        {
            currSlot++;
            continue;
        }
        Process pr=readyQueue[currQueue].front();
        if(twoPowi==1)
        {
            q=pow(2,currQueue);
        }
        for(int i=0; i<q&&pr.service_time>0; i++)
        {
            schedule[pr.id][currSlot]=1;
            pr.service_time--;
            currSlot++;
            if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
            {
                readyQueue[0].push_back(p[processIndex]);
                processIndex++;
            }
        }
        readyQueue[currQueue].pop_front();
        if(pr.service_time>0)
        {
            if(currQueue+1<depth&&noOfProcessesFeedback(readyQueue)+1>1)
            {
                readyQueue[currQueue+1].push_back(pr);
            }
            else
            {
                readyQueue[currQueue].push_back(pr);
            }
        }
    }
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    if(twoPowi==0)
    {
        cout<<"FB-1"<<"  ";
    }
    else
    {
        cout<<"FB-2i"<<" ";
    }
    for(int i=0; i<=last; i++)
    {
        cout<<i%10<<" ";
    }
    cout<<" \n";
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
    cout<<" \n";
    for(int k=0; k<noOfProcesses; k++)
    {
        cout<<pp[k].name<<"     ";
        for(int l=0; l<last; l++)
        {
            cout<<"|";
            if(schedule[k][l]==1)
            {
                cout<<"*";
                pp[k].service_time--;
            }
            else if(pp[k].arrival_time<=l && pp[k].service_time!=0)
            {
                cout<<".";
            }
            else
            {
                cout<<" ";
            }
        }
        cout<<"|";
        cout<<" \n";
    }
    cout<<"-----";
    for(int i=0; i<=last; i++)
    {
        cout<<"--";
    }
    cout<<"-";
}
/* Status Code*/

// Stats Function of Fcfs
void FCFS_Statistics(Process process[],string status,int noofprocesses,int last)
{

    if (status.compare("stats")==0)
    {

        float norm_mean = 0;
        float total_turnaroundtime=0;
        float turnaroundmean=0;
        process[0].complete_time=process[0].service_time;
        process[0].turnaround_time=process[0].complete_time;
        process[0].normturn_time=process[0].turnaround_time/process[0].service_time;
        total_turnaroundtime= total_turnaroundtime+process[0].turnaround_time;
        for (int i=1; i<noofprocesses ; i++)
        {
            process[i].complete_time= process[i-1].complete_time+process[i].service_time;
            process[i].turnaround_time=process[i].complete_time-process[i].arrival_time;
            process[i].normturn_time=process[i].turnaround_time/process[i].service_time;
            total_turnaroundtime= total_turnaroundtime+process[i].turnaround_time;
        }
        turnaroundmean=total_turnaroundtime/noofprocesses;
        Process pp[noofprocesses];
    for(int i=0; i<noofprocesses; i++)
    {
        pp[i]=process[i];
    }
        cout<<"FCFS";
        cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noofprocesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (pp[i].service_time > 9999)
                cout << pp[i].service_time << "|";
            else if (pp[i].service_time > 999)
                cout << pp[i].service_time << " |";
            else if (pp[i].service_time > 99)
                cout << pp[i].service_time << "  |";
            else if (pp[i].service_time > 9)
                cout << " " << pp[i].service_time << "  |";
            else
                cout << "  " << pp[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noofprocesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noofprocesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noofprocesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noofprocesses);
        }
 
    }
      

}

// Stats Function of RoundRobin 

void RoundRobin_Statistics(Process process[],string status,int noofprocesses,int quantum)
{

    if (status.compare("stats")==0)
    {

        float norm_mean = 0;
        float total_turnaroundtime = 0 ;
        float turnaroundmean = 0 ;
        Process serv[noofprocesses];
        for (int i = 0; i < noofprocesses; i++)
        {
            serv[i]=process[i];
        }
        bool visited[noofprocesses+1]= {false};

        int complete[200];
        int done = 0,ct=0;
        queue<int> ready_queue;
        bool x=true;
        while(x ||done!=noofprocesses)
        {
            for (int i = 0; i < noofprocesses; i++)
            {
                if(!visited[i] && process[i].arrival_time==ct)
                {
                    ready_queue.push(i);
                    visited[i]=true;
                }
            }
            if(!ready_queue.empty())
            {

                int  p = ready_queue.front();
                ready_queue.pop();
                int checkmin=min(quantum, process[p].service_time);
                process[p].service_time = process[p].service_time-checkmin;
                for (int i = ct+1; i <= ct+checkmin; i++)
                {
                    for (int j = 0; j < noofprocesses; j++)
                    {
                        if(!visited[j] && process[j].arrival_time==i)
                        {
                            ready_queue.push(j);
                            visited[j]=true;
                        }
                    }
                }
                ct += checkmin;
                if( process[p].service_time==0)
                {
                    done++;
                    process[p].complete_time = ct;
                    complete[p]=process[p].complete_time;
                }
                else
                {
                    ready_queue.push(p);
                }
            }
            else
            {
                ct++;
            }
            x=false;
        }

        for (int i = 0; i < noofprocesses; i++)
        {
            process[i].complete_time = complete[i];
            process[i].turnaround_time = process[i].complete_time - process[i].arrival_time;
            process[i].normturn_time = (float)(process[i].turnaround_time)/ (float)(serv[i].service_time);
            total_turnaroundtime= total_turnaroundtime+process[i].turnaround_time;
        }
        turnaroundmean=total_turnaroundtime/noofprocesses;
        for(int i = 0 ; i < noofprocesses ; i ++)
        {
            process[i].service_time = serv[i].service_time;

        }
     Process pp[noofprocesses];
    for(int i=0; i<noofprocesses; i++)
    {
        pp[i]=process[i];
    }
        cout<<"RR-"<<quantum;
        cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noofprocesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (serv[i].service_time > 9999)
                cout << serv[i].service_time << "|";
            else if (serv[i].service_time > 999)
                cout << serv[i].service_time << " |";
            else if (serv[i].service_time > 99)
                cout << serv[i].service_time << "  |";
            else if (serv[i].service_time > 9)
                cout << " " << serv[i].service_time << "  |";
            else
                cout << "  " << serv[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noofprocesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noofprocesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noofprocesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noofprocesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noofprocesses);
        }
     
    }
}
// Stats function of spn

void SPN_Statstics(Process p[],int last,int noOfProcesses)
{
    float norm_mean = 0;
    float total_turnaroundtime=0;
    float turnaroundmean=0;
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    int currProcessIndex=0;
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int minimum=readyQueue[0].service_time;
            currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(readyQueue[i].service_time<minimum)
                {
                    minimum=readyQueue[i].service_time;
                    currProcessIndex=i;
                }
            }
            while(readyQueue[currProcessIndex].service_time!=0)
            {
                schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
                readyQueue[currProcessIndex].service_time--;


                timeSlot++;
                for(int i=0; i<noOfProcesses; i++)
                {
                    if(p[i].arrival_time==timeSlot)
                    {
                        readyQueue.push_back(p[i]);
                    }
                }
            }
            readyQueue.erase(readyQueue.begin() + currProcessIndex);

        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }
    for(int i=0; i<noOfProcesses; i++)
    {
        int temp;
        for(int j=0; j<last; j++)
        {
            if(schedule[i][j]==1)
            {
                temp=j;
            }
        }
        p[i].complete_time=temp+1;
        p[i].turnaround_time=p[i].complete_time-p[i].arrival_time;
        p[i].normturn_time=p[i].turnaround_time/p[i].service_time;
        total_turnaroundtime=total_turnaroundtime+p[i].turnaround_time;
    }
    turnaroundmean=total_turnaroundtime/noOfProcesses;
     Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }

    cout<<"SPN";
    cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noOfProcesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].service_time > 9999)
                cout << pp[i].service_time << "|";
            else if (pp[i].service_time > 999)
                cout <<pp[i].service_time << " |";
            else if (pp[i].service_time > 99)
                cout << pp[i].service_time << "  |";
            else if (pp[i].service_time > 9)
                cout << " " << pp[i].service_time << "  |";
            else
                cout << "  " << pp[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noOfProcesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noOfProcesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noOfProcesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noOfProcesses);
        }

}
// Stats Function of HRRN
float calcHRRNS(int waiting_time,int service_time)
{
    return ((float)waiting_time + service_time) / (float)service_time;
}
void HRRN_Statistics(Process p[],int last,int noOfProcesses)

{   float norm_mean = 0;
    float total_turnaroundtime=0;
    float turnaroundmean=0;
   int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int maximum=calcHRRN(readyQueue[0].waiting_time,readyQueue[0].service_time);
            int currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(calcHRRN(readyQueue[i].waiting_time,readyQueue[i].service_time)>maximum)
                {
                    maximum=calcHRRN(readyQueue[i].waiting_time,readyQueue[i].service_time);
                    currProcessIndex=i;
                }
            }
            while(readyQueue[currProcessIndex].service_time!=0)
            {
                schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
                readyQueue[currProcessIndex].service_time--;
                timeSlot++;
                for(int i=0; i<readyQueue.size(); i++)
                {
                    readyQueue[i].waiting_time++;
                }
                for(int i=0; i<noOfProcesses; i++)
                {
                    if(p[i].arrival_time==timeSlot)
                    {
                        readyQueue.push_back(p[i]);
                    }
                }
            }
            readyQueue.erase(readyQueue.begin() + currProcessIndex);
        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }

    for(int i=0; i<noOfProcesses; i++)
    {
        
        int temp;
        for(int j=0; j<last; j++)
        {
            if(schedule[i][j]==1)
            {
                temp=j;
            }
        }
        p[i].complete_time=temp+1;
        p[i].turnaround_time=p[i].complete_time-p[i].arrival_time;
        p[i].normturn_time=p[i].turnaround_time/p[i].service_time;
        total_turnaroundtime=total_turnaroundtime+p[i].turnaround_time;
        
    }
    turnaroundmean=total_turnaroundtime/noOfProcesses;
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    cout<<"HRRN";
    cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noOfProcesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].service_time > 9999)
                cout << pp[i].service_time << "|";
            else if (pp[i].service_time > 999)
                cout <<pp[i].service_time << " |";
            else if (pp[i].service_time > 99)
                cout << pp[i].service_time << "  |";
            else if (pp[i].service_time > 9)
                cout << " " << pp[i].service_time << "  |";
            else
                cout << "  " << pp[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noOfProcesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noOfProcesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noOfProcesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noOfProcesses);
        }



}
//Stats Function of SRT
void SRT_Statstics(Process p[],int last,int noOfProcesses)
{
    float norm_mean = 0;
    float total_turnaroundtime=0;
    float turnaroundmean=0;
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    int timeSlot=0;
    int processIndex=0;
    vector<Process> readyQueue;
    for(int i=0; i<noOfProcesses; i++)
    {
        if(p[i].arrival_time==timeSlot)
        {
            readyQueue.push_back(p[processIndex]);
        }
    }
    while(timeSlot<last)
    {
        if(readyQueue.size()>0)
        {
            int minimum=readyQueue[0].service_time;
            int currProcessIndex=0;
            for(int i=1; i<readyQueue.size(); i++)
            {
                if(readyQueue[i].service_time<minimum)
                {
                    minimum=readyQueue[i].service_time;
                    currProcessIndex=i;
                }
            }
            schedule[readyQueue[currProcessIndex].id][timeSlot]=1;
            readyQueue[currProcessIndex].service_time--;
            timeSlot++;
            for(int i=0; i<noOfProcesses; i++)
            {
                if(p[i].arrival_time==timeSlot)
                {
                    readyQueue.push_back(p[i]);
                }
            }
            if(readyQueue[currProcessIndex].service_time<=0)
            {
                readyQueue.erase(readyQueue.begin() + currProcessIndex);
            }
        }
        else
        {
            timeSlot++;
            if (processIndex < last && p[processIndex].arrival_time <= timeSlot)
            {
                readyQueue.push_back(p[processIndex]);
                processIndex++;
            }
        }
    }


    for(int i=0; i<noOfProcesses; i++)
    {
        int temp;
        for(int j=0; j<last; j++)
        {
            if(schedule[i][j]==1)
            {
                temp=j;
            }
        }
        p[i].complete_time=temp+1;
        p[i].turnaround_time=p[i].complete_time-p[i].arrival_time;
        p[i].normturn_time=p[i].turnaround_time/p[i].service_time;
        total_turnaroundtime=total_turnaroundtime+p[i].turnaround_time;
    }
    turnaroundmean=total_turnaroundtime/noOfProcesses;
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    cout<<"SRT";
   cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noOfProcesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].service_time > 9999)
                cout << pp[i].service_time << "|";
            else if (pp[i].service_time > 999)
                cout <<pp[i].service_time << " |";
            else if (pp[i].service_time > 99)
                cout << pp[i].service_time << "  |";
            else if (pp[i].service_time > 9)
                cout << " " << pp[i].service_time << "  |";
            else
                cout << "  " << pp[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noOfProcesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noOfProcesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noOfProcesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noOfProcesses);
        }


}
//Stats Function of feedback
void feedbackStatus(Process p[],int q,int last,int noOfProcesses,int depth){
    float norm_mean = 0;
    float total_turnaroundtime=0;
    float turnaroundmean=0;
 int twoPowi=0;
    if(q==0)
    {
        twoPowi=1;
    }
    int schedule[noOfProcesses][last];
    for(int i=0; i<noOfProcesses; i++)
    {
        for(int j=0; j<last; j++)
        {
            schedule[i][j]=0;
        }
    }
    deque<deque<Process> >readyQueue(depth);
    int currSlot=0;
    int processIndex=0;
    int currQueue=-1;
    if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
    {
        readyQueue[0].push_back(p[processIndex]);
        processIndex++;
    }
    while(currSlot<last)
    {
        for(int i=0; i<depth; i++)
        {
            if(!readyQueue[i].empty())
            {
                currQueue=i; //Get the queue with highest priority to dispatch from it
                break;
            }
        }
        if(currQueue==-1)
        {
            currSlot++;
            continue;
        }
        Process pr=readyQueue[currQueue].front();
        if(twoPowi==1)
        {
            q=pow(2,currQueue);
        }
        for(int i=0; i<q&&pr.service_time>0; i++)
        {
            schedule[pr.id][currSlot]=1;
            pr.service_time--;
            currSlot++;
            if (processIndex < noOfProcesses && p[processIndex].arrival_time <= currSlot)
            {
                readyQueue[0].push_back(p[processIndex]);
                processIndex++;
            }
        }
        readyQueue[currQueue].pop_front();
        if(pr.service_time>0)
        {
            if(currQueue+1<depth&&noOfProcessesFeedback(readyQueue)+1>1)
            {
                readyQueue[currQueue+1].push_back(pr);
            }
            else
            {
                readyQueue[currQueue].push_back(pr);
            }
        }}
         for(int i=0; i<noOfProcesses; i++)
    {
        int temp;
        for(int j=0; j<last; j++)
        {
            if(schedule[i][j]==1)
            {
                temp=j;
            }
        }
        p[i].complete_time=temp+1;
        p[i].turnaround_time=p[i].complete_time-p[i].arrival_time;
        p[i].normturn_time=p[i].turnaround_time/p[i].service_time;
        total_turnaroundtime=total_turnaroundtime+p[i].turnaround_time;
    }
    turnaroundmean=total_turnaroundtime/noOfProcesses;
    Process pp[noOfProcesses];
    for(int i=0; i<noOfProcesses; i++)
    {
        pp[i]=p[i];
    }
    if(twoPowi==0)
    {
        cout<<"FB-1"<<"  ";
    }
    else
    {
        cout<<"FB-2i"<<" ";
    }
    cout<<"\n";
        cout << "Process    |";
        for (int i = 0; i < noOfProcesses; i++)
            cout << "  " << pp[i].name << "  |";

        cout << "\nArrival    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].arrival_time > 9999)
                cout << pp[i].arrival_time << "|";
            else if (pp[i].arrival_time > 999)
                cout << pp[i].arrival_time << " |";
            else if (pp[i].arrival_time > 99)
                cout << pp[i].arrival_time << "  |";
            else if (pp[i].arrival_time > 9)
                cout << " " << pp[i].arrival_time << "  |";
            else
                cout << "  " << pp[i].arrival_time << "  |";
        }
        cout << "\nService    |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].service_time > 9999)
                cout << pp[i].service_time << "|";
            else if (pp[i].service_time > 999)
                cout <<pp[i].service_time << " |";
            else if (pp[i].service_time > 99)
                cout << pp[i].service_time << "  |";
            else if (pp[i].service_time > 9)
                cout << " " << pp[i].service_time << "  |";
            else
                cout << "  " << pp[i].service_time << "  |";
        }
        cout << " Mean|";
        cout << "\nFinish     |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if ( pp[i].complete_time> 9999)
                cout << pp[i].complete_time << "|";
            else if (pp[i].complete_time > 999)
                cout << pp[i].complete_time << " |";
            else if (pp[i].complete_time > 99)
                cout << pp[i].complete_time << "  |";
            else if (pp[i].complete_time > 9)
                cout << " " << pp[i].complete_time << "  |";
            else
                cout << "  " << pp[i].complete_time << "  |";
        }
        cout << "-----|";
        cout << "\nTurnaround |";
        for (int i = 0; i < noOfProcesses; i++)
        {
            if (pp[i].turnaround_time > 9999)
                cout << pp[i].turnaround_time << "|";
            else if (pp[i].turnaround_time > 999)
                cout << pp[i].turnaround_time << " |";
            else if (pp[i].turnaround_time > 99)
                cout << pp[i].turnaround_time << "  |";
            else if (pp[i].turnaround_time > 9)
                cout << " " << pp[i].turnaround_time << "  |";
            else
                cout << "  " << pp[i].turnaround_time << "  |";
        }
        if(turnaroundmean > 9.99) printf("%0.2f|", turnaroundmean);
        else printf(" %0.2f|", turnaroundmean);
        cout << "\nNormTurn   |";

        for (int i = 0; i < noOfProcesses; i++)
        {
            norm_mean += round(pp[i].normturn_time * 100) / 100;
            printf(" %0.2f|", pp[i].normturn_time);
        }
        if((float)norm_mean / (float)noOfProcesses > 9.99)
        {
            printf("%0.2f|\n", (float)norm_mean / (float)noOfProcesses);
        }
        else
        {
            printf(" %0.2f|", (float)norm_mean / (float)noOfProcesses);
        }

    }
int main()
{
    string status;
    cin>>status;
    string line;
    cin>>line;
    int currIndex = 0, i = 0;
    int startIndex = 0, endIndex = 0;
    string subStr;
    int tokenCounter=0;
    int currSub = 0, iSub = 0;
    int startSub = 0, endSub = 0;
    string subsub;
    int tokenCounterSub=0;
    int noOfAlgos=getNoOfAlgos(line);
    Algo algo[noOfAlgos];
    int algoCount=0;
    int algoCounter=0;
    if(len(line)==1)
    {
        algo[algoCount].name=stoi(line);
        algo[algoCount].q=0;
        algoCount++;
    }
    else
    {
        if(!(line.find(",") != std::string::npos)) // Only one algorithm
        {
            while (i <= len(line))
            {
                if (line[i] == '-' || i == len(line))
                {
                    endIndex = i;
                    subStr = "";
                    subStr.append(line, startIndex, endIndex - startIndex);
                    tokenCounter++;
                    if(tokenCounter==1)
                    {
                        algo[algoCount].name=stoi(subStr);
                    }
                    if(tokenCounter==2)
                    {
                        algo[algoCount].q=stoi(subStr);
                    }
                    currIndex += 1;
                    startIndex = endIndex + 1;
                }
                i++;
            }
            algoCount++;
            currIndex=0;
            startIndex=0;
            endIndex=0;
            i=0;
            tokenCounter=0;
        }
        else // More than algorithm
        {
            while (i <= len(line))
            {
                if (line[i] == ',' || i == len(line))
                {
                    endIndex = i;
                    subStr = "";
                    subStr.append(line, startIndex, endIndex - startIndex);
                    if(len(subStr)==1) // Not RR or Aging
                    {
                        algo[algoCount].name=stoi(subStr);
                        algo[algoCount].q=0;
                        algoCount++;
                    }
                    else
                    {
                        while (iSub <= len(subStr))
                        {
                            if (subStr[iSub] == '-' || iSub == len(subStr))
                            {
                                endSub = iSub;
                                subsub = "";
                                subsub.append(subStr, startSub, endSub - startSub);
                                tokenCounterSub++;
                                if(tokenCounterSub==1)
                                {
                                    algo[algoCount].name=stoi(subsub);
                                }
                                if(tokenCounterSub==2)
                                {
                                    algo[algoCount].q=stoi(subsub);
                                }
                                currSub += 1;
                                startSub = endSub + 1;
                            }
                            iSub++;
                        }
                        algoCount++;
                        currSub=0;
                        startSub=0;
                        endSub=0;
                        iSub=0;
                        tokenCounterSub=0;
                    }
                    currIndex += 1;
                    startIndex = endIndex + 1;
                }
                i++;
            }
            currIndex=0;
            startIndex=0;
            endIndex=0;
            i=0;
            tokenCounter=0;
        }
    }

    int lastInstance;
    cin>>lastInstance;
    int noOfProcesses;
    cin>>noOfProcesses;
    Process pro[noOfProcesses];
    string temp;
    int processCount=0;
    int id=0;
    for(int k=0; k<noOfProcesses; k++)
    {
        cin>>temp;
        while (i <= len(temp))
        {
            if (temp[i] == ',' || i == len(temp))
            {
                endIndex = i;
                subStr = "";
                subStr.append(temp, startIndex, endIndex - startIndex);
                tokenCounter++;
                if(tokenCounter==1)
                {
                    pro[processCount].name=subStr;
                }
                if(tokenCounter==2)
                {
                    pro[processCount].arrival_time=stoi(subStr);
                }
                if(tokenCounter==3)
                {
                    pro[processCount].service_time=stoi(subStr);
                    pro[processCount].id=id;
                    id++;
                    pro[processCount].working_slots=0;
                    pro[processCount].waiting_time=0;
                }
                currIndex += 1;
                startIndex = endIndex + 1;
            }
            i++;
        }
        currIndex=0;
        startIndex=0;
        endIndex=0;
        i=0;
        tokenCounter=0;
        processCount++;

    }
    if(status=="stats")
    {
        for(int i=0; i<algoCount; i++)
        {
            if(algo[i].name==1) //FCFS
            {
                FCFS_Statistics(pro,status,noOfProcesses,lastInstance);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==2) //RR
            {
                RoundRobin_Statistics(pro,status,noOfProcesses,algo[i].q);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==3) //SPN
            {
                SPN_Statstics(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==4) //SRT
            {
                SRT_Statstics(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==5) //HRRN
            {
                HRRN_Statistics(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==6) //FB-1
            {
                feedbackStatus(pro,1,lastInstance,noOfProcesses,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
                
            }
            else if(algo[i].name==7) //FB-2i
            {
                feedbackStatus(pro,0,lastInstance,noOfProcesses,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
                
            }
        }
    }
    else if(status=="trace")
    {
        for(int i=0; i<algoCount; i++)
        {
            if(algo[i].name==1) //FCFS
            {
                FCFSVisualizer(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==2) //RR
            {
                RoundRobinVisualizer(pro,algo[i].q,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==3) //SPN
            {
                SPNVisualizer(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==4) //SRT
            {
                SRTVisualizer(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==5) //HRRN
            {
                HRRNVisualizer(pro,lastInstance,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==6) //FB-1
            {
                feedbackVisualizer(pro,1,lastInstance,noOfProcesses,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
            else if(algo[i].name==7) //FB-2i
            {
                feedbackVisualizer(pro,0,lastInstance,noOfProcesses,noOfProcesses);
                cout<<"\n";
                cout<<"\n";
            }
        }
    }

    return 0;
}
