#include "schedulers.h"

//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}



//Shortest Process Next Implementation. In general, this function maintains adouble ended queue
//of processes that are candidates for scheduling (the ready Variable) and always schedules the 
//Process with the shortest service time on that list, if availible/
int ShortestNext(const int& curTime, const vector<Process>& procList)
{
    
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }
    


    //check if process has started if not check for shorter and switch
    if (procList[ready[0]].timeScheduled == 0)
    {
        //compare the service time between all processes in the queue, get the id
        //of the process with the smallest Service time.
        int min =0;
        for(int i = 0, i_end = ready.size(); i < i_end; ++i)
        {
            if(procList[ready[min]].totalTimeNeeded > procList[ready[i]].totalTimeNeeded)
                min = i;   
        }
        //Remove the smallest process from the queue and re-add it to the front.
        int temp = ready[min];
        ready.erase(ready.begin() + min);
        ready.push_front(temp);
    }

    //The Process must run to completion before it can select a new process to exicute
    static int timeToNextSched = procList[ready[0]].totalTimeNeeded;


    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        int min=0;
        //compare the service time between all processes in the queue, get the id
        //of the process with the smallest Service time.
        for(int i = 0, i_end = ready.size(); i < i_end; ++i)
        {
            if(procList[ready[min]].totalTimeNeeded > procList[ready[i]].totalTimeNeeded)
                min = i;   
        }
        //Remove the smallest process from the queue and re-add it to the front.
        int temp = ready[min];
        ready.erase(ready.begin() + min);
        ready.push_front(temp);
        //reset timeToNextSched to runtime for smallest process
        timeToNextSched = procList[ready[0]].totalTimeNeeded;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}


//Given a process how return how many cylces it has to process to completion
//return how much time process has left to completion.
int TimeRemaining(Process proc)
{
    int temp = proc.totalTimeNeeded - proc.timeScheduled;
    return temp;
}



//Shortest Time Remaining Implementation. In general, this function maintains adouble ended queue
//of processes that are candidates for scheduling (the ready Variable) and always schedules the 
//Process with the shortest time  Remaining on that list, if availible/
int ShortestTimeRemaining(const int& curTime, const vector<Process>& procList)
{
    //static int timeToNextSched;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
            //cout << procList[i].id << " " << procList[i].totalTimeNeeded << endl;
        }
    }
    int shortest =0;
    //search queue for a algorithm with a shorter time remaining. if found set as active process
    for(int i = 0, i_end = ready.size(); i < i_end; ++i)
    {
        if(TimeRemaining(procList[ready[shortest]]) > TimeRemaining(procList[ready[i]]))
        {
            shortest = i;
        }
        //cout << procList[ready[shortest]].id << "=" << TimeRemaining(procList[ready[shortest]]) <<" " << endl;

    }

    int temp = ready[shortest];
    ready.erase(ready.begin() + shortest);
    ready.push_front(temp);
    
    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor
        ready.pop_front();
        

        //Now that the Current process has been removed from queue search find the process 
        //with the shortest remaining process time again and put it on top of queue
        int shortest =0;
        //search queue for a algorithm with a shorter time remaining. if found set as active process
        for(int i = 0, i_end = ready.size(); i < i_end; ++i)
        {
            if(TimeRemaining(procList[ready[shortest]]) > TimeRemaining(procList[ready[i]]))
            {
                shortest = i;
            }
            //cout << procList[ready[shortest]].id << "=" << TimeRemaining(procList[ready[shortest]]) <<" " << endl;

        }
        //update the ready queue to have process with shortest time left on front of queue
        int temp = ready[shortest];
        ready.erase(ready.begin() + shortest);
        ready.push_front(temp);
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        idx = ready[0];
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
    }

    // return back the index of the process to schedule next
    return idx;
}


//get the HRRT Ratio for the specified process
//return the Ratio of the process after calculation
double getWaitRatio(Process proc, const int& curTime)
{
    //get the wait time since last scheduled
    int timeWaiting = curTime - proc.startTime;
    //calculate the ratio for the waiting process based of totalWaitTime
    double ratio = (timeWaiting + proc.totalTimeNeeded)/proc.totalTimeNeeded;
    return ratio;
}



//Highest Response Time. In general, this function maintains adouble ended queue
//of processes that are candidates for scheduling (the ready Variable) and always schedules the 
//Process with the shortest service time on that list, if availible/
int HighestResponseTime(const int& curTime, const vector<Process>& procList)
{
    
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }
    


    //check if process has started if not check for shorter and switch
    if (procList[ready[0]].timeScheduled == 0)
    {
        //compare the service time between all processes in the queue, get the id
        //of the process with the smallest Service time.
        int highestR =0;
        for(int i = 0, i_end = ready.size(); i < i_end; ++i)
        {
            if(getWaitRatio(procList[ready[highestR]], curTime) < getWaitRatio(procList[ready[i]],curTime))
                highestR = i;   
        }
        //Remove the smallest process from the queue and re-add it to the front.
        int temp = ready[highestR];
        ready.erase(ready.begin() + highestR);
        ready.push_front(temp);
    }

    //The Process must run to completion before it can select a new process to exicute
    static int timeToNextSched = procList[ready[0]].totalTimeNeeded;


    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        int min=0;
        //compare the service time between all processes in the queue, get the id
        //of the process with the smallest Service time.
        int highestR =0;
        for(int i = 0, i_end = ready.size(); i < i_end; ++i)
        {
            if(getWaitRatio(procList[ready[highestR]], curTime) < getWaitRatio(procList[ready[i]],curTime))
                highestR = i;   
        }
        //Remove the smallest process from the queue and re-add it to the front.
        int temp = ready[highestR];
        ready.erase(ready.begin() + highestR);
        ready.push_front(temp);
        //reset timeToNextSched to runtime for smallest process
        timeToNextSched = procList[ready[0]].totalTimeNeeded;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}
