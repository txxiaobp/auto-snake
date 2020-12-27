#ifndef ALGORITHM_SELECTION_H
#define ALGORITHM_SELECTION_H

#include <QString>
#include <QObject>
#include <vector>
#include <map>

typedef enum
{
    ALGORITHM_BFS = 0,
    ALGORITHM_DIJKSTRA,
    ALGORITHM_MAX
}Algorithm_e;


class AlgorithmSelection : public QObject
{
    Q_OBJECT
public:
    AlgorithmSelection(Algorithm_e algo = ALGORITHM_BFS) : algo(algo)
    {
        algoStringMap[ALGORITHM_BFS] = "广度优先搜索";
        algoStringMap[ALGORITHM_DIJKSTRA] = "Dijkstra";
    }

    Algorithm_e getAlgo() { return algo; }
    QString getAlgoString() { return algoStringMap[algo]; }
    void setAlgo(Algorithm_e algo) { this->algo = algo; emit changeAlgorithm(); }
    std::map<Algorithm_e, QString> getAlgoStringMap() { return algoStringMap; }

signals:
    void changeAlgorithm();

private:
    Algorithm_e algo;
    std::map<Algorithm_e, QString> algoStringMap;
};

#endif // ALGORITHM_SELECTION_H