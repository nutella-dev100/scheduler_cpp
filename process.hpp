#ifndef PROCESS_H
#define PROCESS_H

struct proc{
    int pid;
    int arrival_time;
    int burst_time;

    int remaining_time;
    int completion_time;
    int turnaround_time;
    int wait_time;
    int resp_time;
    bool has_started;

    proc(int id, int at, int bt) :
        pid(id),
        arrival_time(at),
        burst_time(bt),
        remaining_time(bt),
        completion_time(0),
        turnaround_time(0),
        wait_time(0),
        resp_time(-1),
        has_started(false){}

    void reset(){
        remaining_time = burst_time;
        completion_time = 0;
        turnaround_time = 0;
        wait_time = 0;
        resp_time = -1;
        has_started = false;
    }

};

#endif