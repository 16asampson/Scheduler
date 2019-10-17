
#include<vector>   // process vector
#include<iostream> // cout, cin
#include<chrono>   // sleep duration
#include<thread>   // sleep_for
#include<iomanip>  // setw 

#include "schedulers.h"    // scheduler function(s) and data objects

using namespace std;


//constant controlling how long to sleep between each time step
const chrono::duration<int, std::milli> SLEEP_TIME = chrono::milliseconds(500);


int main(int argc, char* argv[])
{
    string fname;
    vector<Process> procList;
    int curTime = 0, input, schedChoice, numProc, procIdx, timeQuantum;
    bool done;
    string tempStr;

    //if the user didnt give us a filename, then use a default
    if(argc == 1)
    {
        fname = "./procList.txt";
    }
    //otherwise grab the name they gave us
    else
    {
        fname = argv[1];
    }
    
    //read in the process list and store the total number of processes
    readInProcList(fname, procList);
    numProc = procList.size();

    cout << "\nWelcome to the scheduler simulator IV mk17 1.32.87a, now with Process support!\n\n";

    //controlled prompt for which scheduler to run
    schedChoice = -1;
    do
    {
        cout << "What scheduler would you like to test?\n\n"
            << "1) Round Robin\n"
            << "2) Shortest Process Next\n"
            << "3) Shortest Remaining Time\n"
            << "4) Highest Response Ratio Next\n";
        cout << "\n--> ";
        cin >> input;

        if(input > 0 && input <= 4)
        {
            schedChoice = input;
        }
        else
        {
            cout << "INVALID ENTRY\n\n";
        }
    } while (schedChoice == -1);
    
    //if the scheduler selected needs a time quantum, ask for it
    if(schedChoice == 1)
    {
        cout << "Enter the time quantum you would like to use: ";
        cin >> timeQuantum;
    }
    
    //output the header for the run table
    cout << "\nStarting simulation\n"
         << "   O: Process scheduled\n"
         << "   X: Process completed\n"
         << "   !: Completed process scheduled more time than needed\n\n"
         << "Time ";
    tempStr = "-----";
    for(int i = 0; i < numProc; ++i)
    {
        cout << "| " << setw(4) << procList[i].id.substr(0,4) << " ";
        tempStr += "-------";
    }
    cout << "| IDLE |\n" << tempStr << "--------\n";

    //pre-run init
    done = false;
    curTime = 0;
    procIdx = -1;

    //while not all processes have completed:
    while(!done)
    {
        //get the process to schedule next using the indicated scheduler
        switch(schedChoice)
        {
            //Round Robin
            case 1:
                procIdx = RoundRobin(curTime, procList, timeQuantum);
                break;

            //Shortest Process Next
            case 2:


                // TODO set procIdx to the proper index for the next process to be scheduled using SPN
                procIdx = ShortestNext(curTime,procList);

                break;

            //Shortest Remaining Time
            case 3:


                // TODO set procIdx to the proper index for the next process to be scheduled using SRT
                procIdx = ShortestTimeRemaining(curTime, procList);

                break;

            //Highest Response Ratio Next
            case 4:


                // TODO set procIdx to the proper index for the next process to be scheduled using HRRN
                procIdx = HighestResponseTime(curTime, procList);

                break;
        }

        //if we were given a valid process index
        if(procIdx >= 0 && procIdx < procList.size())
        {
            //update the details for the scheduled process
            ++procList[procIdx].timeScheduled;
            if(procList[procIdx].totalTimeNeeded == procList[procIdx].timeScheduled)
            {
                procList[procIdx].isDone = true;
                procList[procIdx].timeFinished = curTime;
            }
        }

        // output the row for the time step, denoting which process was selected
        // also check and see if all processes have finished running yet
        cout << setw(4) << curTime;
        done = true;
        for(int i = 0; i < numProc; ++i)
        {
            done = done && procList[i].isDone;

            if(i == procIdx)
            {
                if(procList[i].isDone)
                {
                    if(procList[i].timeScheduled > procList[i].totalTimeNeeded)
                    {
                        cout << " |   ! ";
                    }
                    else
                    {
                        cout << " |   X ";
                    }
                }                
                else
                {
                    cout << " |   O ";
                }
            }
            else
            {
                cout << " |     ";
            }
        }
        // output for the IDLE process
        if(procIdx < 0 || procIdx >= numProc)
        {
            cout << " |   O ";
        }
        else
        {
            cout << " |     ";
        }
        cout << " |" << endl;

        //if we aren't done yet, sleep for a bit and move on to the next time step
        if(!done)
        {
            this_thread::sleep_for(SLEEP_TIME);
            curTime = curTime + 1;
        }
    }

    //its done! output the run statistics
    cout << "\n\nRun Statistics:\n";



    //TODO output the requested run stats
    cout << "Finnish Time   |";
    for(int i = 0; i < numProc; ++i)
    {
        cout << "   " << procList[i].timeFinished +1;

    }
    cout << endl;

    cout << "Turnaround Time   |";
    for(int i = 0; i < numProc; ++i)
    {
        cout << "   " << procList[i].timeFinished - procList[i].startTime +1;

    }
    cout << endl;

    cout << "Normalized Time   |";
    for(int i = 0; i < numProc; ++i)
    {
        double turnAroundTime = procList[i].timeFinished - procList[i].startTime +1;
        double norm = turnAroundTime/(procList[i].totalTimeNeeded);
        cout << "   " << norm;
    }
    cout << endl;

    return 0;
}