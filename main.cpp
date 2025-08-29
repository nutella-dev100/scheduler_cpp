#include <iostream>
#include <vector>
#include "scheduler.hpp"

int main(){
    int n, quantum;
    std::cout << "Enter no. processes : ";
    std::cin >> n;

    std::vector<proc> procs;
    for(int i = 0; i < n; i++){
        int arrival, burst;
        std::cout << "Enter arrival time and burst time for the process " << i + 1 << ": ";
        std::cin >> arrival >> burst;
        procs.emplace_back(i + 1, arrival, burst);  //creates process in place and populates the variables in order
    }

    std::cout << "\nEnter the time quantum for Round Robin: ";
    std::cin >> quantum;
    std::cout << "\n";

    // --- FCFS Simulation ---
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "         First-Come, First-Served (FCFS)          " << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::vector<proc> fcfsProcesses = procs; // Create a copy for FCFS
    run_fcfs(fcfsProcesses);
    print_metrics(fcfsProcesses);
    std::cout << "\n\n";

    // --- Round Robin Simulation ---
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "               Round Robin (RR)                   " << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::vector<proc> rrProcesses = procs; // Create a fresh copy for RR
    // No need to reset here since we use a fresh copy
    run_round_robin(rrProcesses, quantum);
    print_metrics(rrProcesses);

    return 0;
}