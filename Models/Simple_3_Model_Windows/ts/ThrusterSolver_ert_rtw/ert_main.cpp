//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: ert_main.cpp
//
// Code generated for Simulink model 'ThrusterSolver'.
//
// Model version                  : 1.43
// Simulink Coder version         : 9.8 (R2022b) 13-May-2022
// C/C++ source code generated on : Thu Oct 12 04:58:06 2023
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Linux 64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include <stdio.h>
#include <stdlib.h>
#include "ThrusterSolver.h"
#include "ThrusterSolver_private.h"
#include "rtwtypes.h"
#include "limits.h"
#include "rt_nonfinite.h"
#include "linuxinitialize.h"
#define UNUSED(x)                      x = x

static ThrusterSolver ThrusterSolver_Obj;// Instance of model class

#define NAMELEN                        16

// Function prototype declaration
void exitFcn(int sig);
void *terminateTask(void *arg);
void *baseRateTask(void *arg);
void *subrateTask(void *arg);
volatile boolean_T stopRequested = false;
volatile boolean_T runModel = true;
sem_t stopSem;
sem_t baserateTaskSem;
pthread_t schedulerThread;
pthread_t baseRateThread;
void *threadJoinStatus;
int terminatingmodel = 0;
void *baseRateTask(void *arg)
{
  runModel = (rtmGetErrorStatus(ThrusterSolver_Obj.getRTM()) == (nullptr));
  while (runModel) {
    sem_wait(&baserateTaskSem);
    ThrusterSolver_Obj.step();

    // Get model outputs here
    stopRequested = !((rtmGetErrorStatus(ThrusterSolver_Obj.getRTM()) ==
                       (nullptr)));
    runModel = !stopRequested;
  }

  runModel = 0;
  terminateTask(arg);
  pthread_exit((void *)0);
  return NULL;
}

void exitFcn(int sig)
{
  UNUSED(sig);
  rtmSetErrorStatus(ThrusterSolver_Obj.getRTM(), "stopping the model");
}

void *terminateTask(void *arg)
{
  UNUSED(arg);
  terminatingmodel = 1;

  {
    runModel = 0;
  }

  // Terminate model
  ThrusterSolver_Obj.terminate();
  sem_post(&stopSem);
  return NULL;
}

int main(int argc, char **argv)
{
  rtmSetErrorStatus(ThrusterSolver_Obj.getRTM(), 0);

  // Initialize model
  ThrusterSolver_Obj.initialize();

  // Call RTOS Initialization function
  myRTOSInit(0.001, 0);

  // Wait for stop semaphore
  sem_wait(&stopSem);

#if (MW_NUMBER_TIMER_DRIVEN_TASKS > 0)

  {
    int i;
    for (i=0; i < MW_NUMBER_TIMER_DRIVEN_TASKS; i++) {
      CHECK_STATUS(sem_destroy(&timerTaskSem[i]), 0, "sem_destroy");
    }
  }

#endif

  return 0;
}

//
// File trailer for generated code.
//
// [EOF]
//
