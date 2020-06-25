/****************************************************************
 *                  FileName : LocalSearch.cpp                  *
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
 *                            int BASE_WEIGHT, int EARLY_STOP,  *
 *                            int VERBOSE) - Consructor.        *
 *   LocalSearch::select()                                      *
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
#include "LocalSearch.h"

using namespace std;


LocalSearch::LocalSearch(MAP_TYPE& origin, int LOOP_LIMITS, \
                         double LOOSE_COEF, int EARLY_STOP, \
                         int VERBOSE): TSPbase(origin)
{
    this->LOOP_LIMITS = LOOP_LIMITS;
    this->LOOSE_COEF = LOOSE_COEF;
    this->EARLY_STOP = EARLY_STOP;
    this->VERBOSE = VERBOSE;
    early_stop_counter = 0;

    n_ops.push_back(&LocalSearch::exchangeTwoNode);
    n_ops.push_back(&LocalSearch::reverseSequence);
    n_ops.push_back(&LocalSearch::optThree);

    n_ops_v.push_back(&LocalSearch::exchangeTwoNode);
    n_ops_v.push_back(&LocalSearch::reverseSequence);
    n_ops_v.push_back(&LocalSearch::optThree);


    delta_d = 0.0;
    for (int i = 1; i <= N; i++) {
        double mini = INT_MAX, second_mini = INT_MAX;
        int mini_idx = 0, second_mini_idx = 0;
        for (int j = 1; j <= N; j++) {
            if (i == j) {
                continue;
            }
            if (dist_map[i * (N + 1) + j] <= mini) {
                mini = dist_map[i * (N + 1) + j];
                mini_idx = j;
            }
        }
        for (int j = 1; j <= N; j++) {
            if (i == j || j == mini_idx) {
                continue;
            }
            if (dist_map[i * (N + 1) + j] <= second_mini) {
                second_mini = dist_map[i * (N + 1) + j];
                second_mini_idx = j;
            }
        }
        delta_d += mini;
        closest_city[i] = {mini_idx, second_mini_idx};
    }
    delta_d *= (3.0 / N);
    for (int i = 1; i <= N; i++) {
        double bound = delta_d + dist_map[i * (N + 1) + closest_city[i][0]];
        for (int j = 1; j <= N; j++) {
            if (i == j) {
                continue;
            }
            if (dist_map[i * (N + 1) + j] <= bound) {
                adj_city[i].push_back(j);
            }
            if (adj_city[i].size() < 5) {
                adj_city[i].push_back(int(roulette()*N));
            }
        }
    }

    node_satisfication = new double[N + 1];
    node_active_value = new double[N + 1];
    node_weights = new double[N + 1];
    fill(node_satisfication, node_satisfication + N + 1, 0.0);
    fill(node_active_value, node_active_value + N + 1, 0.0);
    fill(node_weights, node_weights + N + 1, 0.0);
    for (int i = 0; i < N; i++) {
        node_satisfication[path[i]] = getSatisfication(i);
        node_weights[path[i]] = (1 - node_satisfication[path[i]]);
    }
    for (int i = 2; i <= N; i++) {
        node_weights[i] += node_weights[i - 1];
    }
    delta_v = N * 10 * (1 - LOOSE_COEF);

}

static double t_up = 0.0;

double LocalSearch::getSatisfication(int pos)
{
    int i = path[pos];
    int j = path[(pos + N - 1) % N], k = path[(pos + 1) % N];
    int s = closest_city[i][0], t = closest_city[i][1];
    double d_cur = dist_map[i * (N + 1) + j] + dist_map[i * (N + 1) + k];
    double d_best = dist_map[i * (N + 1) + s] + dist_map[i * (N + 1) + t];
    return exp(-((d_cur - d_best) * (d_cur - d_best) / 200000));
}

double LocalSearch::getActivity(double active_value)
{
    return exp(-active_value * active_value / 10000);
}

/**
 * Return a random path index,
 * according to the weights.
 */
int LocalSearch::select()
{
    double _choose = roulette() * node_weights[N];
    int pos = int(upper_bound(node_weights + 1, node_weights + N + 1, _choose)
                  - node_weights);
    return city2path[pos];
}

/**
 * base : int, a path index.
 * -------------------------
 * Return a random path index,
 * adjacent to base.
 */
int LocalSearch::chooseNode(int base)
{
    int base_city = path[base];
    int _choose = base_city;
    int adj_len = adj_city[path[base]].size();
    int target = rand() % adj_len;
    _choose = adj_city[base_city][target];
    return city2path[_choose];
}

void LocalSearch::search()
{
    double prob = roulette();
    int pick = select();

    if (prob < 0.2) {
        (this->*n_ops[0])(pick, this->path);
    }
    else if (prob < 0.5) {
        (this->*n_ops[1])(pick, this->path);
    }
    else if (prob < 1) {
        (this->*n_ops[2])(pick, this->path);
    }
    buildCity2Path(city2path, path);
}

void LocalSearch::updateNodeWeight()
{
    for (int i = 0; i < N; i++) {
        int city = path[i];
        node_weights[city] = (1 - node_satisfication[city]) *\
                             getActivity(node_active_value[city]);
    }
    for (int i = 2; i <= N; i++) {
        node_weights[i] += node_weights[i - 1];
    }
}

bool LocalSearch::checkPath()
{
    set<int> s(path, path + N);
    return int(s.size()) == N;
}

bool LocalSearch::checkPath(int* path)
{
    set<int> s(path, path + N);
    return int(s.size()) == N;
}

bool LocalSearch::earlyStop()
{
    return ++early_stop_counter == EARLY_STOP;
}

void LocalSearch::run()
{
    int counter = 1;
    auto t_start = clock();
    while (counter++ <= LOOP_LIMITS) {
        if (earlyStop()) {
            break;
        }
        if (VERBOSE && (counter % int(LOOP_LIMITS * 0.1)) == 0) {
            cout << "\n>> Current epoch: " << counter;
            cout << " / " << LOOP_LIMITS << " in " << \
                 double(clock() - t_start) / CLOCKS_PER_SEC << " s." << endl << endl;
            t_start = clock();
            t_up = 0.0;
        }
        backUp();

        double pre_dist = currentCost();
        search();

        updateNodeWeight();

        double new_dist = currentCost();
        if (pre_dist > new_dist) {
            early_stop_counter = 0;
            updateOptCost(new_dist);
            saveHistory(new_dist);
            if (VERBOSE) {
                cout << ">> Current Optimal Solution Updated:" << endl;
                cout << ">> From " << pre_dist << " to " << new_dist << "." << endl;
            }
        }
        else {
            recover();
        }
    }
    if (!checkPath()) {
        cout << "Invalid!" << endl;
        for (int i = 0; i < N; i++) {
            cout << path_bak[i] << " ";
        }
        for (int i = 0; i < N; i++) {
            cout << path[i] << " ";
        }
    }
    report();
}

void LocalSearch::saveHistory(double new_dist)
{
    int* new_path = new int[N];
    memcpy(new_path, path, N * sizeof(int));
    opt_hist.push_back(make_pair(new_dist, new_path));
}

void LocalSearch::report()
{
    ofstream output;
    string save_path = SAVEPATH;
    save_path += "result.ls.tour";
    output.open(save_path, ios::out);
    for (int i = 0; i < N; i++) {
        output << path[i] << " ";
    }
    output << -1 << endl;
    output.close();

    save_path = SAVEPATH;
    save_path += "result.ls.hist";
    output.open(save_path, ios::out);
    output << "best_cost_hist" << endl;
    for (auto i : opt_hist) {
        output << i.first << " ";
    }
    output << endl;
    output << "best_route_hist" << endl;
    set<int*> s;
    for (auto i : opt_hist) {
        if (s.find(i.second) != s.end()) {
            continue;
        }
        s.insert(i.second);
        output << i.first << endl;
        for (int j = 0; j < N; j++) {
            output << i.second[j] << " ";
        }
        output << endl;
    }
    output << EOF << endl;
    output.close();
}

void LocalSearch::exchangeTwoNode(int pos, int* p)
{
    int target = chooseNode(pos);
    int tmp_pos = p[pos];
    p[pos] = p[target];
    p[target] = tmp_pos;
    for (int i = 0; i < N; i++) {
        if (i == target || i == pos) {
            node_active_value[p[i]] *= LOOSE_COEF;
            node_active_value[p[i]] += delta_v / 2.0;
            continue;
        }
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    node_satisfication[p[(pos + N - 1) % N]] = getSatisfication((pos + N - 1) % N);
    node_satisfication[p[pos]] = getSatisfication(pos);
    node_satisfication[p[(pos + 1) % N]] = getSatisfication((pos + 1) % N);
    node_satisfication[p[(target + N - 1) % N]] = getSatisfication((target + N - 1) % N);
    node_satisfication[p[target]] = getSatisfication(target);
    node_satisfication[p[(target + 1) % N]] = getSatisfication((target + 1) % N);
}

void LocalSearch::reverseSequence(int pos, int* p)
{
    int target = chooseNode(pos);
    int begin = min(pos, target);
    int end = max(pos, target);
    int K = end - begin + 1;
    while (begin < end) {
        int tmp_pos = p[begin];
        p[begin] = p[end];
        p[end] = tmp_pos;
        begin++;
        end--;
    }
    begin = min(pos, target);
    end = max(pos, target);
    for (int i = 0; i < N; i++) {
        if (i >= begin && i <= end) {
            node_active_value[p[i]] *= LOOSE_COEF;
            node_active_value[p[i]] += delta_v / K;
            continue;
        }
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
    node_satisfication[p[begin]] = getSatisfication(begin);
    node_satisfication[p[end]] = getSatisfication(end);
    node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
}

void LocalSearch::reverseSequence(int pos1, int pos2, int* p)
{
    int begin = min(pos1, pos2);
    int end = max(pos1, pos2);
    while (begin < end) {
        int tmp_pos = p[begin];
        p[begin] = p[end];
        p[end] = tmp_pos;
        begin++;
        end--;
    }
}

void LocalSearch::optThree(int pos, int* p)
{
    int A = pos == N - 1 ? pos - 1 : pos;
    int B = pos == N - 1 ? pos : pos + 1;
    int C = 0, D = 0, E = 0, F = 0, tmp;
    while (A == C || A == D || A == E || A == F || \
           C == B || C == E || C == F || \
           E == B || E == D) {
        tmp = chooseNode(A);
        C = tmp == N - 1 ? tmp - 1 : tmp;
        D = tmp == N - 1 ? tmp : tmp + 1;
        tmp = chooseNode(C);
        E = tmp == N - 1 ? tmp - 1 : tmp;
        F = tmp == N - 1 ? tmp : tmp + 1;
    }
    double candi_dist[6][6] = {0};
    candi_dist[0][1] = dist_map[path[A] * (N + 1) + path[B]];
    candi_dist[0][2] = dist_map[path[A] * (N + 1) + path[C]];
    candi_dist[0][4] = dist_map[path[A] * (N + 1) + path[E]];
    candi_dist[0][3] = dist_map[path[A] * (N + 1) + path[D]];
    candi_dist[1][3] = dist_map[path[B] * (N + 1) + path[D]];
    candi_dist[1][4] = dist_map[path[B] * (N + 1) + path[E]];
    candi_dist[1][5] = dist_map[path[B] * (N + 1) + path[F]];
    candi_dist[2][3] = dist_map[path[C] * (N + 1) + path[D]];
    candi_dist[2][4] = dist_map[path[C] * (N + 1) + path[E]];
    candi_dist[2][5] = dist_map[path[C] * (N + 1) + path[F]];
    candi_dist[3][5] = dist_map[path[D] * (N + 1) + path[F]];
    candi_dist[4][5] = dist_map[path[E] * (N + 1) + path[F]];
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            candi_dist[j][i] = candi_dist[i][j];
        }
    }

    int candidate;
    double dist = INT_MAX;
    double candidates[7];
    candidates[0] = candi_dist[0][4] + candi_dist[1][5] + candi_dist[2][3];
    candidates[1] = candi_dist[0][1] + candi_dist[2][4] + candi_dist[3][5];
    candidates[2] = candi_dist[0][3] + candi_dist[1][5] + candi_dist[2][4];
    candidates[3] = candi_dist[0][2] + candi_dist[1][3] + candi_dist[4][5];
    candidates[4] = candi_dist[0][2] + candi_dist[1][4] + candi_dist[3][5];
    candidates[5] = candi_dist[0][4] + candi_dist[1][3] + candi_dist[2][3];
    candidates[6] = candi_dist[0][3] + candi_dist[1][4] + candi_dist[2][5];
    for (int i = 0; i < 7; i++) {
        if (candidates[i] < dist) {
            dist = candidates[i];
            candidate = i;
        }
    }

    for (int i = 0; i < N; i++) {
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    if (candidate > 2) {
        reverseSequence(B, C, p);
        int begin = min(B, C), end = max(B, C);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
    if (candidate == 0 || candidate == 2 || candidate == 5 || candidate == 6) {
        reverseSequence(A, F, p);
        int begin = min(A, F), end = max(A, F);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
    if (candidate == 1 || candidate == 2 || candidate == 4 || candidate == 6) {
        reverseSequence(D, E, p);
        int begin = min(D, E), end = max(D, E);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
}


void LocalSearch::exchangeTwoNode(int pos, vector<int>& p)
{
    int target = chooseNode(pos);
    int tmp_pos = p[pos];
    p[pos] = p[target];
    p[target] = tmp_pos;
    for (int i = 0; i < N; i++) {
        if (i == target || i == pos) {
            node_active_value[p[i]] *= LOOSE_COEF;
            node_active_value[p[i]] += delta_v / 2.0;
            continue;
        }
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    node_satisfication[p[(pos + N - 1) % N]] = getSatisfication((pos + N - 1) % N);
    node_satisfication[p[pos]] = getSatisfication(pos);
    node_satisfication[p[(pos + 1) % N]] = getSatisfication((pos + 1) % N);
    node_satisfication[p[(target + N - 1) % N]] = getSatisfication((target + N - 1) % N);
    node_satisfication[p[target]] = getSatisfication(target);
    node_satisfication[p[(target + 1) % N]] = getSatisfication((target + 1) % N);
}

void LocalSearch::reverseSequence(int pos, vector<int>& p)
{
    int target = chooseNode(pos);
    int begin = min(pos, target);
    int end = max(pos, target);
    int K = end - begin + 1;
    while (begin < end) {
        int tmp_pos = p[begin];
        p[begin] = p[end];
        p[end] = tmp_pos;
        begin++;
        end--;
    }
    begin = min(pos, target);
    end = max(pos, target);
    for (int i = 0; i < N; i++) {
        if (i >= begin && i <= end) {
            node_active_value[p[i]] *= LOOSE_COEF;
            node_active_value[p[i]] += delta_v / K;
            continue;
        }
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
    node_satisfication[p[begin]] = getSatisfication(begin);
    node_satisfication[p[end]] = getSatisfication(end);
    node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
}

void LocalSearch::optThree(int pos, vector<int>& p)
{
    int A = pos == N - 1 ? pos - 1 : pos;
    int B = pos == N - 1 ? pos : pos + 1;
    int C = 0, D = 0, E = 0, F = 0, tmp;
    while (A == C || A == D || A == E || A == F || \
           C == B || C == E || C == F || \
           E == B || E == D) {
        tmp = chooseNode(A);
        C = tmp == N - 1 ? tmp - 1 : tmp;
        D = tmp == N - 1 ? tmp : tmp + 1;
        tmp = chooseNode(C);
        E = tmp == N - 1 ? tmp - 1 : tmp;
        F = tmp == N - 1 ? tmp : tmp + 1;
    }
    double candi_dist[6][6] = {0};
    candi_dist[0][1] = dist_map[path[A] * (N + 1) + path[B]];
    candi_dist[0][2] = dist_map[path[A] * (N + 1) + path[C]];
    candi_dist[0][4] = dist_map[path[A] * (N + 1) + path[E]];
    candi_dist[0][3] = dist_map[path[A] * (N + 1) + path[D]];
    candi_dist[1][3] = dist_map[path[B] * (N + 1) + path[D]];
    candi_dist[1][4] = dist_map[path[B] * (N + 1) + path[E]];
    candi_dist[1][5] = dist_map[path[B] * (N + 1) + path[F]];
    candi_dist[2][3] = dist_map[path[C] * (N + 1) + path[D]];
    candi_dist[2][4] = dist_map[path[C] * (N + 1) + path[E]];
    candi_dist[2][5] = dist_map[path[C] * (N + 1) + path[F]];
    candi_dist[3][5] = dist_map[path[D] * (N + 1) + path[F]];
    candi_dist[4][5] = dist_map[path[E] * (N + 1) + path[F]];
    for (int i = 0; i < 6; i++) {
        for (int j = i + 1; j < 6; j++) {
            candi_dist[j][i] = candi_dist[i][j];
        }
    }

    int candidate;
    double dist = INT_MAX;
    double candidates[7];
    candidates[0] = candi_dist[0][4] + candi_dist[1][5] + candi_dist[2][3];
    candidates[1] = candi_dist[0][1] + candi_dist[2][4] + candi_dist[3][5];
    candidates[2] = candi_dist[0][3] + candi_dist[1][5] + candi_dist[2][4];
    candidates[3] = candi_dist[0][2] + candi_dist[1][3] + candi_dist[4][5];
    candidates[4] = candi_dist[0][2] + candi_dist[1][4] + candi_dist[3][5];
    candidates[5] = candi_dist[0][4] + candi_dist[1][3] + candi_dist[2][3];
    candidates[6] = candi_dist[0][3] + candi_dist[1][4] + candi_dist[2][5];
    for (int i = 0; i < 7; i++) {
        if (candidates[i] < dist) {
            dist = candidates[i];
            candidate = i;
        }
    }

    for (int i = 0; i < N; i++) {
        node_active_value[p[i]] *= LOOSE_COEF;
    }
    if (candidate > 2) {
        reverseSequence(B, C, p);
        int begin = min(B, C), end = max(B, C);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
    if (candidate == 0 || candidate == 2 || candidate == 5 || candidate == 6) {
        reverseSequence(A, F, p);
        int begin = min(A, F), end = max(A, F);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
    if (candidate == 1 || candidate == 2 || candidate == 4 || candidate == 6) {
        reverseSequence(D, E, p);
        int begin = min(D, E), end = max(D, E);
        node_satisfication[p[(begin + N - 1) % N]] = getSatisfication((begin + N - 1) % N);
        node_satisfication[p[begin]] = getSatisfication(begin);
        node_satisfication[p[end]] = getSatisfication(end);
        node_satisfication[p[(end + 1) % N]] = getSatisfication((end + 1) % N);
        for (int i = begin; i <= end; i++) {
            node_active_value[p[i]] += delta_v / fabs(end - begin + 1);
        }
    }
}
void LocalSearch::reverseSequence(int pos1, int pos2, vector<int>& p)
{
    int begin = min(pos1, pos2);
    int end = max(pos1, pos2);
    while (begin < end) {
        int tmp_pos = p[begin];
        p[begin] = p[end];
        p[end] = tmp_pos;
        begin++;
        end--;
    }
}

LocalSearch::~LocalSearch()
{
    delete[] node_weights;
    delete[] node_satisfication;
    delete[] node_active_value;
    for (auto i : opt_hist) {
        delete[] i.second;
    }
}
