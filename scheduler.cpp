#include "scheduler.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <string>
#include <numeric>


void run_fcfs(std::vector<proc> &procs){
    sort(procs.begin(), procs.end(), [](const proc &a, const proc &b){
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    std::vector<std::pair<int, int>> gantt_chart;

    for(auto &p : procs){
        //If idle, start immediately
        if(current_time < p.arrival_time) current_time = p.arrival_time;

        
        p.resp_time =  current_time - p.arrival_time;
        p.wait_time = p.resp_time;
        gantt_chart.push_back({p.pid, current_time});   //pid and time at which started processing
        current_time += p.burst_time;   //time taken to process this task
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;

        print_gantt("FCFS", gantt_chart, procs);
    }
}

void run_round_robin(std::vector<proc> &procs, int quantum){
    std::queue<int> ready_queue;
    std::vector<std::pair<int, int>> gantt_chart;
    int current_time = 0;
    int completed = 0;
    int n = procs.size();

    //sort by arrival time
    sort(procs.begin(), procs.end(), [](const proc &a, const proc &b){
        return a.arrival_time < b.arrival_time;
    });

    std::vector<bool> in_queue(n + 1, false);
    int next_proc_idx = 0;

    while(completed < n){
        while(next_proc_idx < n && procs[next_proc_idx].arrival_time <= current_time){
            if(!in_queue[procs[next_proc_idx].pid]){
                ready_queue.push(next_proc_idx);
                in_queue[procs[next_proc_idx].pid] = true;
            }
            next_proc_idx++;
        }

        if(ready_queue.empty()){
            if(next_proc_idx < n){
                current_time = procs[next_proc_idx].arrival_time;
            }
            else{
                break;
            }
            continue;
        }

        int curr_proc_idx = ready_queue.front();
        ready_queue.pop();
        in_queue[procs[curr_proc_idx].pid] = false;
        proc &curr_proc = procs[curr_proc_idx];

        if(!curr_proc.has_started){
            curr_proc.resp_time = current_time - curr_proc.arrival_time;
            curr_proc.has_started = true;
        }

        gantt_chart.push_back({curr_proc.pid, current_time});

        int time_slice = std::min(curr_proc.remaining_time, quantum);
        current_time += time_slice;
        curr_proc.remaining_time -= time_slice;

        while(next_proc_idx < n && procs[next_proc_idx].arrival_time <= current_time){
            if(!in_queue[procs[next_proc_idx].pid]){
                ready_queue.push(next_proc_idx);
                in_queue[procs[next_proc_idx].pid] = true;
            }
            next_proc_idx++;
        }

        if(curr_proc.remaining_time > 0){
            //If proc is still not finished, add again to ready_queue
            ready_queue.push(curr_proc_idx);
            in_queue[curr_proc.pid] = true;
        }
        else{
            curr_proc.completion_time = current_time;
            curr_proc.turnaround_time = curr_proc.completion_time - curr_proc.arrival_time;
            curr_proc.wait_time = curr_proc.turnaround_time - curr_proc.burst_time;
            completed++;
        }
    }
    print_gantt("RR", gantt_chart, procs);
}

void print_gantt(const std::string &title, const std::vector<std::pair<int, int>> &chart, const std::vector<proc> &procs){
    if(chart.empty()) return;
    //chart contains pid, start_time

    std::cout << "Gantt Chart - " << title << std::endl;

    //top
    std::cout << "+";
    for(size_t i = 0; i < chart.size(); i++){
        int end_time;
        if(i != chart.size() - 1){
            //if this is not the last proc, end time is same as start time of next proc
            end_time = chart[i + 1].second;
        }
        else{
            //if this is the last process, get the completion time
            for(auto &p : procs){
                if(p.pid == chart[i].first){
                    end_time = p.completion_time;
                }
            }
        }

        int duration = end_time - chart[i].second;
        for(int j = 0; j < duration * 2 + 3; j++) std::cout << '-';
        std::cout << '+';
    }
    std::cout << std::endl;

    //pids
    std::cout << '|';
    for(size_t i = 0; i < chart.size(); i++){
        int end_time;
        if(i != chart.size() - 1){
            //if this is not the last proc, end time is same as start time of next proc
            end_time = chart[i + 1].second;
        }
        else{
            //if this is the last process, get the completion time
            for(auto &p : procs){
                if(p.pid == chart[i].first){
                    end_time = p.completion_time;
                }
            }
        }

        int duration = end_time - chart[i].second;
        std::cout << "P" << chart[i].first << std::string(duration * 2, ' ') << "|";
    }
    std::cout << std::endl;

    //bottom
    std::cout << '+';
    for(size_t i = 0; i < chart.size(); i++){
        int end_time;
        if(i != chart.size() - 1){
            //if this is not the last proc, end time is same as start time of next proc
            end_time = chart[i + 1].second;
        }
        else{
            //if this is the last process, get the completion time
            for(auto &p : procs){
                if(p.pid == chart[i].first){
                    end_time = p.completion_time;
                }
            }
        }

        int duration = end_time - chart[i].second;
        for(int j = 0; j < duration * 2 + 3; j++) std::cout << "-";
        std::cout << "+";
    }
    std::cout << std::endl;

    std::cout << chart[0].second;
    for(size_t i = 0; i < chart.size(); i++){
        int end_time;
        if(i != chart.size() - 1){
            //if this is not the last proc, end time is same as start time of next proc
            end_time = chart[i + 1].second;
        }
        else{
            //if this is the last process, get the completion time
            for(auto &p : procs){
                if(p.pid == chart[i].first){
                    end_time = p.completion_time;
                }
            }
        }

        int duration = end_time - chart[i].second;
        std::cout << std::string(duration * 2 + 3 - (end_time >= 10 ? 1 : 0), ' ') << end_time;
    }
    std::cout << std::endl;
}

void print_metrics(const std::vector<proc> & procs){
    double totalTAT = 0, totalWT = 0, totalRT = 0;

    int n = procs.size();
    std::vector<proc> sorted_procs = procs;
    sort(sorted_procs.begin(), sorted_procs.end(), [](const proc& a, const proc& b){
        return a.pid < b.pid;
    });

    std::cout << "\nProcess Metrics:" << std::endl;
    std::cout << "+---------+--------------+------------+-----------------+--------------+---------------+--------------+" << std::endl;
    std::cout << "| Proc ID | Arrival Time | Burst Time | Completion Time | Turnaround T |  Waiting Time | Response Time |" << std::endl;
    std::cout << "+---------+--------------+------------+-----------------+--------------+---------------+--------------+" << std::endl;

    for(const auto &p : sorted_procs){
        std::cout << "| " << std::setw(7) << p.pid
                  << "| " << std::setw(12) << p.arrival_time
                  << "| " << std::setw(10) << p.burst_time
                  << "| " << std::setw(15) << p.completion_time
                  << "| " << std::setw(12) << p.turnaround_time
                  << "| " << std::setw(13) << p.wait_time
                  << "| " << std::setw(13) << p.resp_time << " |" << std::endl;
        totalTAT += p.turnaround_time;
        totalWT += p.wait_time;
        totalRT += p.resp_time;
    }

    std::cout << "+---------+--------------+------------+-----------------+--------------+---------------+--------------+" << std::endl;

    std::cout << "\nAverage Metrics:" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  - Average Turnaround Time: " << totalTAT / n << std::endl;
    std::cout << "  - Average Waiting Time:    " << totalWT / n << std::endl;
    std::cout << "  - Average Response Time:   " << totalRT / n << std::endl;
}