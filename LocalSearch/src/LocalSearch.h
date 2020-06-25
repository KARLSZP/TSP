/****************************************************************
 *                  FileName : LocalSearch.h                    *
 *                    Author : 17341137 Karl                    *
 *              Created Date : June 5, 2020                     *
 *                   Updated : June 8, 2020 - Add Function      *
 *                             June 9, 2020 - Fix Bug           *
 *                             June 9, 2020 - Modify Perfomance *
 *                             June 12, 2020 - Add satification *
 *                                             and activity     *
 *==============================================================*
 * @Functions:                                                  *
 *   LocalSearch::LocalSearch(MAP_TYPE& origin, int LOOP_LIMITS,*
 *                            int LOOSE_COEF, int EARLY_STOP,   *
 *                            int VERBOSE) - Consructor.        *
 *   LocalSearch::chooseNode()                                  *
 *       - choose a Node according to its weight.               *
 *   LocalSearch::chooseNode(int base, int range)               *
 *       - choose a Node within (base - range, base + range)    *
 *   LocalSearch::propagateWeightChange(int node_num, int value)*
 *       - propagate weight change of node`node_num`.           *
 *   LocalSearch::search()                                      *
 *       - do neighbour operation to search for a new solution. *
 *   LocalSearch::checkPath() - check `private` path's validity.*
 *   LocalSearch::checkPath(vector<int> path)                   *
 *       - check path's validity.                               *
 *   LocalSearch::earlyStop() - check whether to early stop.    *
 *   LocalSearch::run() - start LocalSearch process.            *
 *   LocalSearch::report() - save result to files.              *
 *   LocalSearch::exchangeTwoNode(int pos, vector<int>& p)      *
 *       - Neighbour operation: exchange two node(city).        *
 *   LocalSearch::reverseSequence(int pos, vector<int>& p)      *
 *       - Neighbour operation: reverse a sub-sequence.         *
 *   LocalSearch::popToFront(int pos, vector<int>& p)           *
 *       - Neighbour operation: pop a node to front.            *
 *==============================================================*/
#include "TSPbase.h"

using namespace std;

class LocalSearch;
typedef void (LocalSearch::*nop)(int, int*);
typedef void (LocalSearch::*nop_v)(int, vector<int>&);
typedef pair<double, int*> Individual;
typedef unordered_map<int, vector<int>> ADJ_MAP_TYPE;

class LocalSearch: public TSPbase {
public:

    LocalSearch(MAP_TYPE& origin, int LOOP_LIMITS = 10000, double LOOSE_COEF = 0.99, int EARLY_STOP = 10000, int VERBOSE = 0);

    int select();

    int chooseNode(int base);

    double getSatisfication(int pos);

    double getActivity(double active_value);

    void updateNodeWeight();

    void search();

    void run();

    bool earlyStop();

    void saveHistory(double new_dist);

    void report();

    void exchangeTwoNode(int pos, int* p);
    void reverseSequence(int pos, int* p);
    void optThree(int pos, int* p);
    void reverseSequence(int pos1, int pos2, int* p);
    void exchangeTwoNode(int pos, vector<int>& p);
    void reverseSequence(int pos, vector<int>& p);
    void optThree(int pos, vector<int>& p);
    void reverseSequence(int pos1, int pos2, vector<int>& p);

    bool checkPath();
    bool checkPath(int* path);

    ~LocalSearch();

protected:

    int LOOP_LIMITS;                // Maximum Loop Times
    int EARLY_STOP;                 // Early Stop Epoch
    int VERBOSE;                    // Verbose Message
    int early_stop_counter;         // Early Stop Counter
    double LOOSE_COEF;              // LOOSE_COEF
    double delta_d;                 // parameter
    double delta_v;                 // parameter
    double* node_weights;           // Weight for selection
    double* node_satisfication;     // Satisfication for cities
    double* node_active_value;      // Activy for cities
    vector<Individual> opt_hist;    // History of optimal cost
    vector<nop> n_ops;              // Vector of neighbour operations
    vector<nop_v> n_ops_v;          // Vector of neighbour operations
    ADJ_MAP_TYPE closest_city;      // 2 Closest citys
    ADJ_MAP_TYPE adj_city;          // Adjacent citys
};
