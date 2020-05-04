#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include "format.h"
#include "ncurses_display.h"
#include "system.h"
#include <algorithm>

  


using std::string;
using std::to_string;

string formatted( string s , int l){
return s+string(l>int(s.length()) ? l - s.length():0, ' ');
} 
string upTime = "";
// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)

string NCursesDisplay::ProgressBar(float percent , bool flag) {
  string result{""};
  int size{50};
  float bars{percent * size};
  if(flag) {
    for (int i{0}; i < size; ++i) {
      result += i <= bars ? '|' : ' ';
    }
  }
  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + " %%";
}

void NCursesDisplay::DisplaySystem(System& system, WINDOW* window) {
  int row = 0;
  mvwprintw(window, ++row, 2, ("OS: " + system.OperatingSystem()).c_str());
  mvwprintw(window, ++row, 2, ("Kernel: " + system.Kernel()).c_str());
  for (float i :  system.Cpu().Utilization()){
    row++;
    mvwprintw(window, row, 2, ("CPU "+to_string(row-2)+": ").c_str());
    if(i>0.70)
      wattron(window, COLOR_PAIR(4));
    else if (i>0.40)
      wattron(window, COLOR_PAIR(3));
    else 
      wattron(window, COLOR_PAIR(2));
    mvwprintw(window, row, 10, "");
    wprintw(window, ProgressBar(i,getmaxx(stdscr) >66 ).c_str());
    wattroff(window, getattrs(window));
  }
  mvwprintw(window, ++row, 2, "Memory: ");
  wattron(window, COLOR_PAIR(1));
  mvwprintw(window, row, 10, "");
  wprintw(window, ProgressBar(system.MemoryUtilization(),getmaxx(stdscr) >66 ).c_str());
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, ++row, 2,("Total Processes: " + to_string(system.TotalProcesses())).c_str());
  mvwprintw(window, ++row, 2,("Running Processes: " + to_string(system.RunningProcesses())).c_str());
  upTime=  Format::ElapsedTime(system.UpTime());
  mvwprintw(window, ++row, 2,("Up Time: " +upTime).c_str());
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(std::vector<Process>& processes,WINDOW* window, int n) {
  int row{0};
  int const pid_column{9};
  int const user_column{10};
  int const cpu_column{7};
  int const ram_column{10};
  int const time_column{int(upTime!=""? upTime.length():20)+2};
  int const command_column{40}; 
  wattron(window, COLOR_PAIR(2));
  int j =2;
  mvwprintw(window, ++row, j, "PID");j+=pid_column;
  mvwprintw(window, row, j, "USER");j+=user_column;
  mvwprintw(window, row, j, "CPU[%%]");j+=cpu_column;
  mvwprintw(window, row, j, "RAM[MB]");j+=ram_column;                                         
  mvwprintw(window, row, j, "TIME+");j+=time_column;
  mvwprintw(window, row, j, "COMMAND");j+=command_column;
  wattroff(window, COLOR_PAIR(2));
  for (int i = 0; i < n; ++i) {
    j=2;
    mvwprintw(window, ++row, j, formatted(to_string(processes[i].Pid()),pid_column).c_str());j+=pid_column;
    mvwprintw(window, row, j, formatted(" "+ processes[i].User(),user_column).c_str()); j+=user_column;
    float cpu = processes[i].CpuUtilization() * 100;
    mvwprintw(window, row, j,formatted(to_string(cpu).substr(0, 4),cpu_column).c_str());j+=cpu_column;
    mvwprintw(window, row, j,formatted(processes[i].GetRam(),ram_column).c_str());j+=ram_column;
    mvwprintw(window, row, j,formatted(Format::ElapsedTime(processes[i].UpTime()),time_column).c_str());j+=time_column;
    mvwprintw(window, row, j,processes[i].Command().substr(0, window->_maxx - 46).c_str());j+=command_column;
  }
}



bool sortcol( const Process& v1, const Process& v2 ) { 
 return v1.vmsize >v2.vmsize ; 
} 
  

void NCursesDisplay::Display(System& system) {
  initscr();      // start ncurses
  noecho();       // do not print input values
  cbreak();       // terminate ncurses on ctrl + c
  start_color();  // enable color
  int x_max{getmaxx(stdscr)};
  WINDOW* system_window = newwin(8 + system.Cpu().Utilization().size() , x_max - 1, 0, 0);
  WINDOW* process_window =newwin(3 + 20, x_max - 1, system_window->_maxy + 1, 0);

  while (1){
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_RED,COLOR_BLACK);
    box(system_window, 0, 0);
    box(process_window, 0, 0);
    DisplaySystem(system, system_window);
    std::vector<Process>& processes = system.Processes();
    std::sort(processes.begin(), processes.end(),sortcol);
    DisplayProcesses(processes, process_window, 20);
    wrefresh(system_window);
    wrefresh(process_window);
    refresh();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  endwin();
}