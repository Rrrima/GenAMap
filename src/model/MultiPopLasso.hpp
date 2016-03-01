//
// Created by haohanwang on 2/2/16.
//

#ifndef ALGORITHMS_MULTIPOPLASSO_HPP
#define ALGORITHMS_MULTIPOPLASSO_HPP

#include "model/Model.hpp"
#include "Math/Math.hpp"
#include <iostream>
#include <vector>

using namespace std;

class MultiPopLasso : public Model {
private:
    double lambda;
    double mu;
    double gamma;
    VectorXd population;
    MatrixXd Z;
    long popNum;
    double L;
    bool initTrainingFlag;

    VectorXi removeCols;
    MatrixXd C;

    double groupPenalization();
    void reArrangeData();

    void removeColumns();
    MatrixXd normalizeData_col(MatrixXd);
    void formatData();

//    MatrixXd deriveMatrixA(double, long, double);
//    MatrixXd project(MatrixXd);

    vector<long> getPopulationIndex(long);
    void initC();

public:
    void setXY(MatrixXd, MatrixXd);
    void setLambda(double);
    void setPopulation(VectorXd);
    void setMu(double);
    void setGamma(double);

    void initBeta();
    double cost();
    MatrixXd predict();
    MatrixXd predict(MatrixXd);
    MatrixXd predict(MatrixXd, VectorXd);
    void initTraining();

    MatrixXd proximal_derivative();
    MatrixXd proximal_operator(MatrixXd, float);

    double getL();
    MatrixXd getBeta();
    MatrixXd getFormattedBeta();

    MultiPopLasso();
};


#endif //ALGORITHMS_MULTIPOPLASSO_HPP