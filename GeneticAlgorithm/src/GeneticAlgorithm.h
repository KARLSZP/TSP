/****************************************************************
 *                  FileName : GeneticAlgorithm.h               *
 *                    Author : 17341137 Karl                    *
 *              Created Date : June 12, 2020                    *
 *                   Updated : June 13, 2020 - Modify Perfomance*
 *                             June 23, 2020 - Last Check       *
 *==============================================================*
 * @Functions:                                                  *
 *   GeneticAlgorithm::GeneticAlgorithm(MAP_TYPE& origin,       *
 *                     int LOOP_LIMITS, int POPULATION_SIZE,    *
 *                     int GENERATIONS, int MUTATION_TIMES = 3, *
 *                     double CROSSOVER_PROB = 0.7,             *
 *                     double MUTATION_PROB = 0.2,              *
 *                     double OPT_COST = 0.0, int VERBOSE = 0)  *
 *                     - Consructor.                            *
 *   GeneticAlgorithm::runGA() - start Genetic Algorithm.       *
 *   GeneticAlgorithm::run() - override run() in LocalSearch.   *
 *   GeneticAlgorithm::init() - Initialization.                 *
 *   GeneticAlgorithm::keepBest() - Keep the elite, the elite   *
 *                                  never crossover or mutate.  *
 *   GeneticAlgorithm::selectUnit() - select from population.   *
 *   GeneticAlgorithm::crossOver() - crossoveer genetically.    *
 *   GeneticAlgorithm::cross(UNIT& i1, UNIT& i2, int pos)       *
 *                     - crossover between i1 and i2.           *
 *   GeneticAlgorithm::solveCrossOverConflict()                 *
 *                        - solve conflicts(same city in 1 path)*
 *   GeneticAlgorithm::mutation(vector<UNIT> last_population)   *
 *                     - mutate genetically.                    *
 *   GeneticAlgorithm::mutate(UNIT& ind) - unit ind mutates.    *
 *   GeneticAlgorithm::score(vector<int>& p)                    *
 *                     - calculate score of path `p`.           *
 *   GeneticAlgorithm::evaluate() - evaluate current population.*
 *   GeneticAlgorithm::checkPath() - check path's validity.     *
 *   GeneticAlgorithm::getElite() - return elite solution.      *
 *   GeneticAlgorithm::report() - save result to files.         *
 *==============================================================*/
#include "LocalSearch.h"

using namespace std;

typedef pair<double, vector<int>> UNIT;

class GeneticAlgorithm: public LocalSearch {
public:

    GeneticAlgorithm(MAP_TYPE& origin, int LOOP_LIMITS, \
                     int POPULATION_SIZE, int GENERATIONS, \
                     int MUTATION_TIMES = 3, double CROSSOVER_PROB = 0.7, \
                     double MUTATION_PROB = 0.2, double OPT_COST = 0.0, \
                     int VERBOSE = 0);

    void runGA();

    void run() {
        runGA();
    }

    void init();

    void keepBest();

    void selectUnit();

    void crossOver();
    void cross(UNIT& i1, UNIT& i2, int pos);
    void solveCrossOverConflict(UNIT& i1, UNIT& i2);

    void mutation(vector<UNIT>& last_population);
    void mutate(UNIT& ind);

    double inline score(vector<int>& p);
    void evaluate();

    bool checkPath(vector<int> path);

    double getElite();
    void report();

private:

    int POPULATION_SIZE;            // The size of the population
    int GENERATIONS;                // Iterations the population will do
    int MUTATION_TIMES;             // Times to try to mutate per loop
    int GA_VERBOSE;                 // GA Verbose Message
    double CROSSOVER_PROB;          // The probability to crossover
    double MUTATION_PROB;           // The probability to mutate
    double OPT_COST;                // The optimal cost of the tsp
    UNIT ELITE;                     // The best unit in the population
    vector<UNIT> elite_hist;        // History of elites
    vector<UNIT> population;        // Population of units
    vector<double> scores;          // Scores to evaluate unit
    vector<double> chance_by_score; // Accumulation of scores
};