// black box library header and function (pointer) declarations
#include <bbcomplib.h>
DLL_DECLARATIONS

// additional headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// configuration
#define LOGFILEPATH "logs/"
#define USERNAME "demoaccount"
#define PASSWORD "demopassword"
#define TRACKNAME "trial"
#define LOGIN_DELAY_USECONDS  10000000  // 10 seconds
#define LOCK_DELAY_USECONDS   60000000  // 60 seconds

///////////////////////////////////////////////////////////
// auxiliary functions
//

#ifndef _WIN32
#include <unistd.h>
#endif

void u_sleep(long usec);
void safeLogin();
void safeSetTrack();
int safeGetNumberOfProblems();
void safeSetProblem(int problemID);
void safeEvaluate(int problemID, double* point, double* value);
int safeGetDimension(int problemID);
int safeGetBudget(int problemID);
int safeGetEvaluations(int problemID);
int comp(const void* pp1, const void* pp2);
double truncate2bounds(double value);
