/****************************************************************
 *                  FileName : GeneticAlgorithm.cpp             *
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
#include "GeneticAlgorithm.h"

using namespace std;


GeneticAlgorithm::GeneticAlgorithm(MAP_TYPE& origin, int LOOP_LIMITS, \
                                   int POPULATION_SIZE, int GENERATIONS, \
                                   int MUTATION_TIMES, double CROSSOVER_PROB, \
                                   double MUTATION_PROB, double OPT_COST, \
                                   int VERBOSE): LocalSearch(origin, LOOP_LIMITS, 0.99)
{
    this->POPULATION_SIZE = POPULATION_SIZE;
    this->GENERATIONS = GENERATIONS;
    this->OPT_COST = OPT_COST;
    this->GA_VERBOSE = VERBOSE;
    this->MUTATION_TIMES = MUTATION_TIMES;
    this->CROSSOVER_PROB = CROSSOVER_PROB;
    this->MUTATION_PROB = MUTATION_PROB;
    scores = vector<double>(POPULATION_SIZE, 0.0);
    chance_by_score = vector<double>(POPULATION_SIZE, 0.0);
    ELITE.first = INT_MAX;
    init();
    evaluate();
    keepBest();
}

void GeneticAlgorithm::init()
{
    LocalSearch::run();
    int seed = opt_hist.size() - 1 > 20 ? opt_hist.size() - 21 : 0;
    while (++seed < int(opt_hist.size())) {
        double p_len = opt_hist[seed].first;
        vector<int> tmp(opt_hist[seed].second, opt_hist[seed].second + N);
        population.push_back(make_pair(p_len, tmp));
    }
    for (int i = population.size(); i < POPULATION_SIZE; i++) {
        auto tmp_path = generateRandomPath();
        double p_len = currentCost(tmp_path);
        vector<int> tmp(tmp_path, tmp_path + N);
        population.push_back(make_pair(p_len, tmp));
        delete[] tmp_path;
    }
    cout << ">> Initialized the population with localsearch, as seeds." << endl;
}

void GeneticAlgorithm::runGA()
{
    int generation_cur = 0;
    while (generation_cur++ < GENERATIONS) {
        if (generation_cur % 500 == 0) {
            cout << "\n>> Current Generation(s): " << generation_cur << \
                 " | Best dist: " << ELITE.first << endl << endl;
        }
        vector<UNIT> last_population = population;
        selectUnit();
        crossOver();

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (population[i].first > last_population[i].first) {
                population[i] = last_population[i];
            }
        }
        last_population = population;
        for (int i = 0; i < MUTATION_TIMES; i++) {
            mutation(last_population);
            last_population = population;
        }
        evaluate();
        keepBest();
    }
    if (!checkPath(ELITE.second)) {
        cout << "Invalid!" << endl;
        for (auto i : ELITE.second) {
            cout << i << " ";
        }
    }
    report();
}

void GeneticAlgorithm::report()
{
    cout << ">> press [Enter] to save results.";
    getchar();
    ofstream output;
    string save_path = SAVEPATH;
    save_path += "result.ga.tour";
    output.open(save_path, ios::out);
    for (auto i : ELITE.second) {
        output << i << " ";
    }
    output << -1 << endl;
    output.close();

    save_path = SAVEPATH;
    save_path += "result.ga.hist";
    output.open(save_path, ios::out);
    output << "best_cost_hist" << endl;
    for (auto i : elite_hist) {
        output << i.first << " ";
    }
    output << endl;
    output << "best_route_hist" << endl;
    set<vector<int>> s;
    for (auto i : elite_hist) {
        if (s.find(i.second) != s.end()) {
            continue;
        }
        s.insert(i.second);
        output << i.first << endl;
        for (auto j : i.second) {
            output << j << " ";
        }
        output << endl;
    }
    output << EOF << endl;
    output.close();
}


void GeneticAlgorithm::keepBest()
{
    int cur_worst = 0;
    double cur_best = ELITE.second.empty() ? \
                      0 : score(ELITE.second);
    double origin_best = cur_best;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (scores[cur_worst] > scores[i]) {
            cur_worst = i;
        }
        if (cur_best < scores[i]) {
            ELITE = population[i];
            cur_best = scores[i];
        }
    }
    population[cur_worst] = ELITE;
    evaluate();
    if (GA_VERBOSE && score(ELITE.second) > origin_best) {
        cout << ">> New Best UNIT Found: Score From " << \
             origin_best << " to " << score(ELITE.second) << endl;
    }
    elite_hist.push_back(ELITE);
}

double inline GeneticAlgorithm::score(vector<int>& p)
{
    return 1.0 / (currentCost(p) - OPT_COST);
}

void GeneticAlgorithm::evaluate()
{
    double score_sum = 0.0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        scores[i] = score(population[i].second);
        score_sum += scores[i];
        chance_by_score[i] = scores[i] + (i ? chance_by_score[i - 1] : 0);
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        chance_by_score[i] /= score_sum;
    }

}

void GeneticAlgorithm::selectUnit()
{
    vector<UNIT> new_generation;
    int keep_elite_size = POPULATION_SIZE * 0.2;
    for (int i = 0; i < keep_elite_size; i++) {
        new_generation.push_back(ELITE);
    }
    for (int i = keep_elite_size; i < POPULATION_SIZE; i++) {
        int idx = 0;
        while (idx < keep_elite_size) {
            double prob = roulette();
            idx = lower_bound(chance_by_score.begin(), \
                              chance_by_score.end(), prob)\
                  - chance_by_score.begin();
        }
        new_generation.push_back(population[idx]);
    }
    population = new_generation;
}

void GeneticAlgorithm::crossOver()
{
    int elite_size = POPULATION_SIZE * 0.2;
    int i = elite_size;
    while (i < POPULATION_SIZE) {
        double prob = roulette();
        if (prob > CROSSOVER_PROB) {
            i++;
            continue;
        }

        int j = i;
        while (i == j) {
            j = int(roulette() * POPULATION_SIZE);
        }
        int cross_point = int(roulette() * N);
        cross(population[i], population[j], cross_point);
        solveCrossOverConflict(population[i], population[j]);
        population[i].first = currentCost(population[i].second);
        population[j].first = currentCost(population[j].second);
    }
}

void GeneticAlgorithm::cross(UNIT& i1, UNIT& i2, int pos)
{
    vector<int> tmp(N - pos, 0);
    copy(i2.second.begin() + pos, i2.second.end(), tmp.begin());
    copy(i1.second.begin() + pos, i1.second.end(), i2.second.begin() + pos);
    copy(tmp.begin(), tmp.end(), i1.second.begin() + pos);
}

void GeneticAlgorithm::solveCrossOverConflict(UNIT& i1, UNIT& i2)
{
    bool* visited1 = new bool[N + 1];
    bool* visited2 = new bool[N + 1];
    fill(visited1, visited1 + N + 1, false);
    fill(visited2, visited2 + N + 1, false);
    int i = 0, j = 0;
    while (i < N && j < N) {
        if (visited1[i1.second[i]] && visited2[i2.second[j]]) {
            int tmp = i1.second[i];
            i1.second[i] = i2.second[j];
            i2.second[j] = tmp;
            i++, j++;
        }
        if (i < N && !visited1[i1.second[i]]) {
            visited1[i1.second[i]] = true;
            i++;
        }
        if (j < N && !visited2[i2.second[j]]) {
            visited2[i2.second[j]] = true;
            j++;
        }
    }
    delete []visited1;
    delete []visited2;
}


void GeneticAlgorithm::mutation(vector<UNIT>& last_population)
{
    int elite_size = POPULATION_SIZE * 0.2;
    for (int i = elite_size; i < POPULATION_SIZE; i++) {
        double prob = roulette();
        if (prob > MUTATION_PROB) {
            continue;
        }
        mutate(population[i]);
    }
    for (int i = elite_size; i < POPULATION_SIZE; i++) {
        if (population[i].first > last_population[i].first) {
            population[i] = last_population[i];
        }
    }
}


void GeneticAlgorithm::mutate(UNIT& ind)
{
    double prob = roulette();
    buildCity2Path(city2path, ind.second);
    // int pos = select();
    int pos = int(roulette() * N);

    if (prob < 0.25) {
        (this->*n_ops_v[0])(pos, ind.second);
    }
    else if (prob < 0.5) {
        (this->*n_ops_v[1])(pos, ind.second);
    }
    else if (prob < 1) {
        (this->*n_ops_v[2])(pos, ind.second);
    }
    ind.first = currentCost(ind.second);
}


bool GeneticAlgorithm::checkPath(vector<int> path)
{
    set<int> s(path.begin(), path.end());
    return int(s.size()) == N;
}

double GeneticAlgorithm::getElite()
{
    return ELITE.first;
}