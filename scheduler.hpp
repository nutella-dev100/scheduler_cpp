#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.hpp"
#include <vector>
#include <string>
#include <utility>

void run_fcfs(std::vector<proc> &procs);
void run_round_robin(std::vector<proc> &procs, int quantum);

void print_gantt(const std::string &title, const std::vector<std::pair<int, int>> &chart, const std::vector<proc> &procs);
void print_metrics(const std::vector<proc> & procs);

#endif