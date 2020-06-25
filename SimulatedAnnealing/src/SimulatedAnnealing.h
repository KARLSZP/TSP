/****************************************************************
 *                  FileName : SimulatedAnnealing.h             *
 *                    Author : 17341137 Karl                    *
 *              Created Date : June 6, 2020                     *
 *                   Updated : June 11, 2020 - Add Function     *
 *                             June 11, 2020 - Fix Bug          *
 *                             June 12, 2020 - Modify Perfomance*
 *                             June 22, 2020 - Last Check       *
 *==============================================================*
 * @Functions:                                                  *
 *   SimulatedAnnealing::SimulatedAnnealing(MAP_TYPE& origin,   *
 *                       double LOOSE_COEF, double TEMP_INIT,   *
 *                       double TEMP_END, int LOOPS_PER_TEMP,   *
 *                       double ANNEALING_COEF, int VERBOSE)    *
 *                        - Consructor.                         *
 *   SimulatedAnnealing::runSA() - start Simulated Annealing.   *
 *   SimulatedAnnealing::run() - override run() in LocalSearch. *
 *   SimulatedAnnealing::report() - save result to files.       *
 *                                                              *
 *   SimulatedAnnealing::Metropolis(int pos, vector<int>& p)    *
 *       - criterion for acceptance of worse solution.          *
 *==============================================================*/
#include "LocalSearch.h"

using namespace std;

class SimulatedAnnealing: public LocalSearch {
public:

    SimulatedAnnealing(MAP_TYPE& origin, double LOOSE_COEF, double TEMP_INIT, double TEMP_END, int LOOPS_PER_TEMP, double ANNEALING_COEF, int VERBOSE);

    void runSA();

    void run() {
        runSA();
    }

    void report();

    bool Metropolis(double delta, double temp_cur);

private:
    double TEMP_INIT;           // Initial temperature
    double TEMP_END;            // Terminal temperature
    int LOOPS_PER_TEMP;         // Loop times per temperature do
    double ANNEALING_COEF;      // Coefficient to control annealing speed
};