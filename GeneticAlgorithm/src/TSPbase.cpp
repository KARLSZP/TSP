/****************************************************************
 *                  FileName : TSPbase.cpp                      *
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
#include "TSPbase.h"

using namespace std;

TSPbase::TSPbase(MAP_TYPE& origin)
{
    N = origin.size();
    citys = MAP_TYPE(origin);
    dist_map = new double[(N + 1) * (N + 1)];
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            dist_map[i * (N + 1) + j] = distance(citys[i], citys[j]);
        }
    }
    path = generateRandomPath();
    path_bak = generateRandomPath();
    city2path = new int[N + 1];
    city2path_bak = new int[N + 1];
    fill(city2path, city2path + N + 1, 0);
    fill(city2path_bak, city2path_bak + N + 1, 0);
    buildCity2Path(city2path, path);
    buildCity2Path(city2path_bak, path_bak);
    opt_cost = currentCost();
}

double TSPbase::distance(COORD city1, COORD city2)
{
    return sqrt((city1.first - city2.first) * (city1.first - city2.first) + \
                (city1.second - city2.second) * (city1.second - city2.second));
}

double TSPbase::currentCost()
{
    double res = 0.0;
    // Node index from 0 to N-1,
    // Distances: 0~1, 1~2, ..., N-2~N-1
    for (int i = 0; i < N - 1; i++) {
        // res += distance(citys[path[i]], citys[path[i + 1]]);
        res += dist_map[path[i] * (N + 1) + path[i + 1]];
    }
    // Distance: N-1 ~ 0, as a round trip.
    // res += distance(citys[path[0]], citys[path[N - 1]]);
    // cout << path[0] << " " << path[N - 1] << endl;
    res += dist_map[path[0] * (N + 1) + path[N - 1]];
    return res;
}

double TSPbase::currentCost(int* path)
{
    double res = 0.0;
    // Node index from 0 to N-1,
    // Distances: 0~1, 1~2, ..., N-2~N-1
    for (int i = 0; i < N - 1; i++) {
        // res += distance(citys[path[i]], citys[path[i + 1]]);
        res += dist_map[path[i] * (N + 1) + path[i + 1]];
    }
    // Distance: N-1 ~ 0, as a round trip.
    res += dist_map[path[0] * (N + 1) + path[N - 1]];
    return res;
}

double TSPbase::currentCost(vector<int>& path)
{
    double res = 0.0;
    // Node index from 0 to N-1,
    // Distances: 0~1, 1~2, ..., N-2~N-1
    for (int i = 0; i < N - 1; i++) {
        // res += distance(citys[path[i]], citys[path[i + 1]]);
        res += dist_map[path[i] * (N + 1) + path[i + 1]];
    }
    // Distance: N-1 ~ 0, as a round trip.
    res += dist_map[path[0] * (N + 1) + path[N - 1]];
    return res;
}

double TSPbase::roulette()
{
    return (double(rand())) / (RAND_MAX);
}

int* TSPbase::generateRandomPath()
{
    int* random_path = new int[N];
    for (int i = 0; i < N; i++) {
        random_path[i] = i + 1;
    }
    for (int i = 0; i < N; i++) {
        int city1 = rand() % N;
        int city2 = rand() % N;
        int tmp = random_path[city2];
        random_path[city2] = random_path[city1];
        random_path[city1] = tmp;
    }
    return random_path;
}

double TSPbase::getOptCost()
{
    return opt_cost;
}

void TSPbase::updateOptCost(double new_dist)
{
    opt_cost = new_dist;
}

void TSPbase::backUp()
{
    for (int i = 0; i < N; i++) {
        path_bak[i] = path[i];
        city2path_bak[i + 1] = city2path[i + 1];
    }
}

void TSPbase::recover()
{
    for (int i = 0; i < N; i++) {
        path[i] = path_bak[i];
        city2path[i + 1] = city2path_bak[i + 1];
    }
}

void TSPbase::buildCity2Path(int* city2path, int* p)
{
    for (int i = 0; i < N; i++) {
        city2path[p[i]] = i;
    }
}

void TSPbase::buildCity2Path(int* city2path, vector<int>& p)
{
    for (int i = 0; i < N; i++) {
        city2path[p[i]] = i;
    }
}

TSPbase::~TSPbase()
{
    delete[] dist_map;
    delete[] path;
    delete[] path_bak;
    delete[] city2path;
    delete[] city2path_bak;
}