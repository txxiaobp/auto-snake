#include "algorithm_selection.h"


AlgorithmSelection::AlgorithmSelection(Config &config)
    : config(config)
    , algo(config.getAlgorithm())
{
    algoStringMap[ALGORITHM_BFS] = "广度优先搜索";
    algoStringMap[ALGORITHM_DEBFS] = "双端广度优先搜索";
    algoStringMap[ALGORITHM_DIJKSTRA] = "Dijkstra";
    algoStringMap[ALGORITHM_ASTAR] = "A*";
    algoStringMap[ALGORITHM_DSTAR] = "D*";
    algoStringMap[ALGORITHM_DSTAR_LITE] = "D* Lite";
}

void AlgorithmSelection::setAlgo(Algorithm_e algo)
{
    this->algo = algo;
    config.setAlgorithm(algo);
    emit changeAlgorithm();
}
