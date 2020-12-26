#ifndef ALGORITHM_SELECTION_H
#define ALGORITHM_SELECTION_H

#include <QString>
#include <QObject>

typedef enum
{
    ALGORITHM_BFS,
    ALGORITHM_DIJKSTRA,
    ALGORITHM_MAX
}Algorithm_e;


class AlgorithmSelection : public QObject
{
    Q_OBJECT
public:
    AlgorithmSelection(Algorithm_e algo = ALGORITHM_BFS) : algo(algo) {}
    Algorithm_e getAlgo() { return algo; }
    QString getAlgoString() { return algoString[int(algo)]; }
    void setAlgo(Algorithm_e algo) { this->algo = algo; emit changeAlgorithm(); }

signals:
    void changeAlgorithm();

private:
    Algorithm_e algo;
    QString algoString[ALGORITHM_MAX] = { "广度优先搜索", "Dijkstra" };
};

#endif // ALGORITHM_SELECTION_H
