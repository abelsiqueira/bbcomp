#include "base.h"

void dll_load() {
  DLL_LOAD
  if (DLL_LOAD_STATUS < 0) {
    printf("error loading library\n");
    exit(EXIT_FAILURE);
  }
}

void dll_close() {
  DLL_CLOSE
}

// sleep in microseconds
void u_sleep(long usec)
{
#ifdef _WIN32
	HANDLE timer;
	LARGE_INTEGER ft;
	ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
#else
	usleep(usec);
#endif
}

///////////////////////////////////////////////////////////
// network failure resilient functions
//

void safeLogin()
{
	while (1)
	{
		int result = login(USERNAME, PASSWORD);
		if (result != 0) return;
		const char* msg = errorMessage();
		printf("WARNING: login failed: %s\n", msg);
		u_sleep(LOGIN_DELAY_USECONDS);
		if (strcmp(msg, "already logged in") == 0) return;
	}
}

void safeSetTrack()
{
	while (1)
	{
		int result = setTrack(TRACKNAME);
		if (result != 0) return;
		printf("WARNING: setTrack failed: %s\n", errorMessage());
		safeLogin();
	}
}

int safeGetNumberOfProblems()
{
	while (1)
	{
		int result = numberOfProblems();
		if (result != 0) return result;
		printf("WARNING: numberOfProblems failed: %s\n", errorMessage());
		safeSetTrack();
	}
}

void safeSetProblem(int problemID)
{
	while (1)
	{
		int result = setProblem(problemID);
		if (result != 0) return;
		const char* msg = errorMessage();
		printf("WARNING: setProblem failed: %s\n", msg);
		if (memcmp(msg, "failed to acquire lock", 22) == 0) u_sleep(LOCK_DELAY_USECONDS);
		else safeSetTrack();
	}
}

void safeEvaluate(int problemID, double* point, double* value)
{
	while (1)
	{
		int result = evaluate(point, value);
		if (result != 0) return;
		printf("WARNING: evaluate failed: %s\n", errorMessage());
		safeSetProblem(problemID);
	}
}

int safeGetDimension(int problemID)
{
	while (1)
	{
		int result = dimension();
		if (result != 0) return result;
		printf("WARNING: dimension failed: %s\n", errorMessage());
		safeSetProblem(problemID);
	}
}

int safeGetBudget(int problemID)
{
	while (1)
	{
		int result = budget();
		if (result != 0) return result;
		printf("WARNING: budget failed: %s\n", errorMessage());
		safeSetProblem(problemID);
	}
}

int safeGetEvaluations(int problemID)
{
	while (1)
	{
		int result = evaluations();
		if (result >= 0) return result;
		printf("WARNING: evaluations failed: %s\n", errorMessage());
		safeSetProblem(problemID);
	}
}


// comparison function for qsort
int comp(const void* pp1, const void* pp2)
{
	double d1 = **(const double**)pp1;
	double d2 = **(const double**)pp2;
	return (d2 < d1) - (d1 < d2);
}

// simplistic bounds handling by truncation
double truncate2bounds(double value)
{ return fmax(0.0, fmin(1.0, value)); }
