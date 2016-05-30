#include "assignment1.h"
#include "assignment2.h"
#include "workload.h"
#include "scheduler.h"
#include "governor.h"
#include <limits.h>

// Note: Deadline of each workload is defined in the "workloadDeadlines" variable.
// i.e., You can access the dealine of the BUTTON thread using workloadDeadlines[BUTTON]
// See also deadlines.c and workload.h

// Assignment: You need to implement the following two functions.

// learn_workloads(SharedVariable* v):
// This function is called at the start part of the program before actual scheduling
// - Parameters
// sv: The variable which is shared for every function over all threads
void learn_workloads(SharedVariable* sv) {
	// TODO: Fill the body
	// This function is executed before the scheduling simulation.
	// You need to calculate the execution time of each thread here.

  long long beginTime;
  long long endTime;

  int i;
  for(i = 0; i < 8; ++i)
  {
    sv->deadlines[i] = LLONG_MAX;
    sv->prev_alive[i] = 0; 
  }
  sv->avgTimeToSched = 0;
  sv->numTimesSched = 0;
  sv->prev_thread = 0;
	// Thread functions for workloads: 
	// thread_button, thread_twocolor, thread_temp, thread_track,
	// thread_shock, thread_rgbcolor, thread_aled, thread_buzzer

  beginTime = get_current_time_us();
  thread_button(&sv);
  endTime = get_current_time_us();

  sv->exec_times[0] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_twocolor(&sv);
  endTime = get_current_time_us();

  sv->exec_times[1] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_temp(&sv);
  endTime = get_current_time_us();

  sv->exec_times[2] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_track(&sv);
  endTime = get_current_time_us();

  sv->exec_times[3] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_shock(&sv);
  endTime = get_current_time_us();

  sv->exec_times[4] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_rgbcolor(&sv);
  endTime = get_current_time_us();

  sv->exec_times[5] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_aled(&sv);
  endTime = get_current_time_us();

  sv->exec_times[6] = endTime - beginTime;

  beginTime = get_current_time_us();
  thread_buzzer(&sv);
  endTime = get_current_time_us();

  sv->exec_times[7] = endTime - beginTime;

	// Tip 1. You can call each workload function here like:
	// thread_button();

	// Tip 2. You can get the current time here like:
	// long long curTime = get_current_time_us();
}


// select_task(SharedVariable* sv, const int* aliveTasks):
// This function is called while runnning the actual scheduler
// - Parameters
// sv: The variable which is shared for every function over all threads
// aliveTasks: an array where each element indicates whether the corresponed task is alive(1) or not(0).
// idleTime: a time duration in microsecond. You can know how much time was waiting without any workload
//           (i.e., it's larger than 0 only when all threads are finished and not reache the next preiod.)
// - Return value
// TaskSelection structure which indicates the scheduled task and the CPU frequency
TaskSelection select_task(SharedVariable* sv, const int* aliveTasks, long long idleTime) {
	// TODO: Fill the body

	// This function is executed inside of the scheduling simulation.
    // You need to implement an energy-efficient EDF (Earliest Deadline First) scheduler.
  long long closest = LLONG_MAX;
  int nextTask = 0;
  int nextFreq = 0;
  int currTime = 0;
  
  //how much err we want to add in estimating execution finish time
  long long delta = 1000;
  
  //int initialTime = get_scheduler_elapsed_time_us();
  //sv->numTimesSched++;
  
  int currThread;
  for(currThread = 0; currThread < 8; ++currThread) {
    currTime = get_scheduler_elapsed_time_us(); 
    nextFreq = 0;
   
    //if task isn't alive we don't care to schedule it
    if(aliveTasks[currThread] == 1)
    {
    
      //if task first time (re)scheduled, set new deadline
      if(sv->prev_alive[currThread] == 0) 
      {
        sv->deadlines[currThread] = currTime + workloadDeadlines[currThread];
      }
   
      //if task is going to miss deadline, pick it and set freq high
      int finishTime = sv->exec_times[currThread] + currTime;//sv->avgTimeToSched;
      if( (finishTime + delta) >= sv->deadlines[currThread] )
      {
        nextTask = currThread;
        nextFreq = 1;
        sv->prev_alive[currThread] = aliveTasks[currThread]; 
        break;
      }
    
      //task estimated to finish in time
      //check if task is alive and has the next closest deadline
      if( sv->deadlines[currThread] < closest ) 
      {
        closest = sv->deadlines[currThread];
        nextTask = currThread;
      } 
    }
    sv->prev_alive[currThread] = aliveTasks[currThread]; 
  }
  
  //sv->runningSchedTime += currTime - initialTime;
  //sv->avgTimeToSched = sv->runningSchedTime / sv->numTimesSched;
  //printDBG("sv->avgTimeToSched = %lld\n", sv->avgTimeToSched);
  
  sv->prev_thread = nextTask;
  
  TaskSelection sel;
  sel.task = nextTask;
  sel.freq = nextFreq;
  
  //printDBG("nextTask = %d\n", nextTask);
  
  return sel; 
}

        //look for earliest deadline
	// Tip 1. You may get the current time elapsed in the scheduler here like:
	// long long curTime = get_scheduler_elapsed_time_us();
  // Also, do not make any interruptable / IO tasks in this function.
	// You can use printfDBG instead of printf.
  
	// Sample scheduler: Round robin
	// It selects a next thread using aliveTasks.
	
/**
  static int prev_selection = -1;

	int i = prev_selection + 1;
	while(1) {
		if (i == NUM_TASKS)
			i = 0;

		if (aliveTasks[i] == 1) {
			prev_selection = i;
			break;
		}
		++i;
	}

	// The retun value can be specified like this:
	TaskSelection sel;
	sel.task = prev_selection; // The thread ID which will be scheduled. i.e., 0(BUTTON) ~ 7(BUZZER)
	sel.freq = 1; // Request the maximum frequency (if you want the minimum frequency, use 0 instead.)

    return sel;
}
**/
