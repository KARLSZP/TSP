/****************************************************************
 *                  FileName : TSPbase.h                        *
 *                    Author : 17341137 Karl                    *
 *              Created Date : June 5, 2020                     *
 *                   Updated : June 8, 2020 - Add Function      *
 *                             June 9, 2020 - Fix Bug           *
 *                             June 9, 2020 - Modify Perfomance *
 *                             June 22, 2020 - Last Check       *
 *==============================================================*
 * @Functions:                                                  *
 *   TSPbase::TSPbase(MAP_TYPE& origin) - Consructor.           *
 *   TSPbase::distance(COORD city1, COORD city2)                *
 *       - calculate EUC2D distance between city1 and city2.    *
 *   TSPbase::currentCost() - calculate cost of `private` path. *
 *   TSPbase::currentCost(vector<int> path)                     *
 *       - calculate cost of path.                              *
 *   TSPbase::roulette() - generate a random decimal in (0, 1). *
 *   TSPbase::generateRandomPath() - generate a random path.    *
 *   TSPbase::getOptCost() - return the best(least) cost so far.*
 *   TSPbase::updateOptCost(double new_dist)                    *
 *       - update current best cost with new_dist.              *
 *   TSPbase::backUp() - back up current path.                  *
 *   TSPbase::recover() - recover current path with backup.     *
 *==============================================================*/
#include<iostream>
#include<bits/stdc++.h>
#define SAVEPATH "../testcases/"

using namespace std;

typedef pair<double, double> COORD;
typedef unordered_map<int, COORD> MAP_TYPE;


class TSPbase {
public:

    TSPbase(MAP_TYPE& origin);
    ~TSPbase();

    double distance(COORD city1, COORD city2);

    double currentCost();
    double currentCost(int* path);
    double currentCost(vector<int>& path);

    double roulette();

    int* generateRandomPath();

    double getOptCost();

    void updateOptCost(double new_dist);

    void backUp();
    void recover();

    void buildCity2Path(int* city2path, int* p);
    void buildCity2Path(int* city2path, vector<int>& p);

protected:

    int N;                             // Dimension
    MAP_TYPE citys;                    // Citys map
    double* dist_map;                  // Dists map
    double opt_cost;                   // Optimal cost(least distance)
    int* path;                         // Current path
    int* city2path;                    // Map a city to its position in path
    int* path_bak;                     // Path backup
    int* city2path_bak;                // Map backup
};