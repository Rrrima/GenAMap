//// Created by aditya gautam on 4/2/16.//#include "GridSearch.hpp"#ifdef BAZEL#include "algorithm/AlgorithmOptions.hpp"#include "model/LinearMixedModel.hpp"#else#include "AlgorithmOptions.hpp"#include "../model/LinearMixedModel.hpp"#endifGridSearch::GridSearch(){    lambda_start_point = -10.0;    lambda_end_point = 10.0;    lambda_interval = 0.2;}void GridSearch::set_lambda_params(double s, double e, double i){    this->lambda_start_point = s;    this->lambda_end_point = e;    this->lambda_interval = i;}void GridSearch::set_lambda_start_value(double start_val){    this->lambda_start_point = start_val;}void GridSearch::set_lambda_end_value(double end_val){    this->lambda_end_point = end_val;}void GridSearch::set_lambda_interval(double interval){    this->lambda_interval = interval;}double GridSearch::get_lambda_interval(){    return this->lambda_interval;}double GridSearch::get_lambda_start_value(){    return this->lambda_start_point;}void GridSearch::shift_lambda_window(double shift) {    this->lambda_start_point += shift;    this->lambda_end_point += shift;}double GridSearch::get_lambda_end_value(){    return this->lambda_end_point;}void GridSearch::run(LinearMixedModel *model) {    double start_val = this->get_lambda_start_value(), end_val = get_lambda_end_value();    double interval = this->get_lambda_interval(), best_val =0.0,curr_val =0.0;    double temp_likehood=0.0,best_lambda=-1;    /* Do Grid search on the likelihood function and see for which lambda, it is getting to     * the maximum. Maximum likelihood means the most optimal solution.     */    for(double val = start_val;val<=end_val;val=val+interval){        curr_val = model->f(val);        if(curr_val >= best_val){            best_val = curr_val;            best_lambda = val;        }    }    model->set_lambda(best_lambda);    model->calculate_sigma(best_lambda);    model->calculate_beta(best_lambda);    cout << "Best Grid Search value occurs at = " <<  best_lambda << endl;    return;}