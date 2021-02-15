#ifndef ALGORITHM_SELECTION_H
#define ALGORITHM_SELECTION_H

#include "pub_include.h"
#include "config.h"
#include <QString>
#include <QObject>
#include <vector>
#include <map>


class AlgorithmSelection : public QObject
{
    Q_OBJECT
public:
    AlgorithmSelection(Config &config);

    Algorithm_e getAlgo() { return algo; }
    QString getAlgoString() { return algoStringMap[algo]; }
    void setAlgo(Algorithm_e algo);
    std::map<Algorithm_e, QString> getAlgoStringMap() { return algoStringMap; }

signals:
    void changeAlgorithm();

private:
    Config& config;
    Algorithm_e algo;
    std::map<Algorithm_e, QString> algoStringMap;
};

#endif // ALGORITHM_SELECTION_H
