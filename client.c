#include "base.h"

///////////////////////////////////////////////////////////
// Nelder Mead simplex algorithm
//

void solveProblem(int problemID)
{
	// set the problem
	safeSetProblem(problemID);

	// obtain problem properties
	int bud = safeGetBudget(problemID);
	int dim = safeGetDimension(problemID);
	int evals = safeGetEvaluations(problemID);

	// output status
	if (evals == bud)
	{
		printf("problem %d: already solved\n", problemID);
		return;
	}
	else if (evals == 0)
	{
		printf("problem %d: starting from scratch\n", problemID);
	}
	else
	{
		printf("problem %d: continuing from evaluation %d\n", problemID, evals);
	}

	// reserve memory for the simplex and for a number of intermediate points
	double** simplex = (double**)malloc((dim + 1) * sizeof(double*));
	for (int j=0; j<=dim; j++)
	{
		// simplex[j][0] = function value
		// simplex[j][1..dim] = coordinates
		simplex[j] = (double*)malloc((dim+1) * sizeof(double));
		for (int i=0; i<dim; i++) simplex[j][i+1] = (i == j) ? 0.8 : 0.2;
	}
	int simplex_evaluated = 0;
	double* x0 = (double*)malloc(dim * sizeof(double));
	double* xr = (double*)malloc(dim * sizeof(double));
	double* xe = (double*)malloc(dim * sizeof(double));
	double* xc = (double*)malloc(dim * sizeof(double));

	// recover algorithm state from saved evaluations
	for (int e=0; e<evals; e++)
	{
		double value;
		int result = history(e, x0, &value);
		if (result == 0)
		{
			// note: this evaluation is lost
			printf("WARNING: history failed.\n");
		}
		else
		{
			if (simplex_evaluated <= dim)
			{
				double* p = simplex[simplex_evaluated];
				p[0] = value;
				for (int i=0; i<dim; i++) p[i+1] = x0[i];
				simplex_evaluated++;
			}
			else
			{
				qsort(simplex, dim+1, sizeof(double*), &comp);
				double* worst = simplex[dim];
				if (value < simplex[dim][0])
				{
					worst[0] = value;
					for (int i=0; i<dim; i++) worst[i+1] = x0[i];
				}
			}
		}
	}

	// optimization loop
	while (safeGetEvaluations(problemID) < bud)
	{
		if (simplex_evaluated <= dim)
		{
			// evaluate the initial simplex
			double value;
			safeEvaluate(problemID, &simplex[simplex_evaluated][1], &value);
			simplex[simplex_evaluated][0] = value;
			simplex_evaluated++;
		}
		else
		{
			// step of the simplex algorithm
			qsort(simplex, dim+1, sizeof(double*), &comp);
			double* best = simplex[0];
			double* worst = simplex[dim];

			// compute centroid
			for (int i=0; i<dim; i++)
			{
				double sum = 0.0;
				for (int j=0; j<dim; j++) sum += simplex[j][i+1];
				x0[i] = sum / (double)dim;
			}

			// reflection
			for (int i=0; i<dim; i++) xr[i] = truncate2bounds(2.0 * x0[i] - worst[i+1]);
			double f_xr;
			safeEvaluate(problemID, xr, &f_xr);
			if (best[0] <= f_xr && f_xr < worst[0])
			{
				// replace worst point with reflected point
				worst[0] = f_xr;
				for (int i=0; i<dim; i++) worst[i+1] = xr[i];
			}
			else if (f_xr < best[0])
			{
				if (safeGetEvaluations(problemID) >= bud) break;

				// expansion
				for (int i=0; i<dim; i++) xe[i] = truncate2bounds(3.0 * x0[i] - 2.0 * worst[i+1]);
				double f_xe;
				safeEvaluate(problemID, xe, &f_xe);
				if (f_xe < f_xr)
				{
					// replace worst point with expanded point
					worst[0] = f_xe;
					for (int i=0; i<dim; i++) worst[i+1] = xe[i];
				}
				else
				{
					// replace worst point with reflected point
					worst[0] = f_xr;
					for (int i=0; i<dim; i++) worst[i+1] = xr[i];
				}
			}
			else
			{
				if (safeGetEvaluations(problemID) >= bud) break;

				// contraction
				for (int i=0; i<dim; i++) xc[i] = truncate2bounds(0.5 * x0[i] + 0.5 * worst[i+1]);
				double f_xc;
				safeEvaluate(problemID, xc, &f_xc);
				if (f_xc < worst[0])
				{
					// replace worst point with contracted point
					worst[0] = f_xc;
					for (int i=0; i<dim; i++) worst[i+1] = xc[i];
				}
				else
				{
					// reduction
					for (int j=1; j<=dim; j++)
					{
						if (safeGetEvaluations(problemID) >= bud) break;
						for (int i=0; i<dim; i++) simplex[j][i+1] = truncate2bounds(0.5 * best[i+1] + 0.5 * simplex[j][i+1]);
						safeEvaluate(problemID, &simplex[j][1], &simplex[j][0]);
					}
				}
			}
		}
	}

	// free memory
	for (int i=0; i<=dim; i++) free(simplex[i]);
	free(simplex);
	free(x0);
	free(xr);
	free(xe);
	free(xc);
}


int main()
{
	// connect to black box library
	DLL_LOAD
	if (DLL_LOAD_STATUS < 0)
	{
		printf("error loading library\n");
		exit(EXIT_FAILURE);
	}

	// setup
	if (configure(1, LOGFILEPATH) == 0) { printf("configure() failed: %s\n", errorMessage()); return EXIT_FAILURE; }
	safeLogin();
	safeSetTrack();
	int n = safeGetNumberOfProblems();

	// solve all problems in the track
	// for (int i=0; i<n; i++) solveProblem(i);

	// quit cleanly
	DLL_CLOSE
}
