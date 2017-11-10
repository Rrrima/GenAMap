//
// Created by aditya gautam on 4/2/16.
//

#include "GridSearch.hpp"

#ifdef BAZEL
#include "Models/LinearMixedModel.hpp"
#else
#include "../Models/LinearMixedModel.hpp"
#endif

GridSearch::GridSearch(){
    lambda_start_point = default_lambda_start_point;
    lambda_end_point = default_lambda_end_point;
    lambda_interval = default_lambda_interval;
}

GridSearch::GridSearch(const unordered_map<string, string> opts) {
    try {
        lambda_start_point = stod(opts.at("lambda_start_point"));
    } catch (std::out_of_range& oor) {
        lambda_start_point = default_lambda_start_point;
    }
    try {
        lambda_end_point = stod(opts.at("lambda_end_point"));
    } catch (std::out_of_range& oor) {
        lambda_end_point = default_lambda_end_point;
    }
    try {
        lambda_interval = stod(opts.at("lambda_interval"));
    } catch (std::out_of_range& oor) {
        lambda_interval = default_lambda_interval;
    }
}

void GridSearch::set_lambda_params(float s, float e, float i){
    this->lambda_start_point = s;
    this->lambda_end_point = e;
    this->lambda_interval = i;
}

void GridSearch::set_lambda_start_value(float start_val){
    this->lambda_start_point = start_val;
}

void GridSearch::set_lambda_end_value(float end_val){
    this->lambda_end_point = end_val;
}

void GridSearch::set_lambda_interval(float interval){
    this->lambda_interval = interval;
}

float GridSearch::get_lambda_interval(){
    return this->lambda_interval;
}

float GridSearch::get_lambda_start_value(){
    return this->lambda_start_point;
}

void GridSearch::shift_lambda_window(float shift) {
    this->lambda_start_point += shift;
    this->lambda_end_point += shift;
}

float GridSearch::get_lambda_end_value(){
    return this->lambda_end_point;
}


void GridSearch::run(shared_ptr<Model> model) {
    if (!model.get()) {
        throw runtime_error("Model not initialized");
    } else if (shared_ptr<LinearMixedModel> model = dynamic_pointer_cast<LinearMixedModel>(model)) {
        sub_run(model);
    } else {
        throw runtime_error("Bad type of model for Algorithm: GridSearch");
    }
}

void GridSearch::sub_run(shared_ptr<LinearMixedModel> model) {
    mtx.lock();
    float start_val = this->get_lambda_start_value(), end_val = get_lambda_end_value();
    float interval = this->get_lambda_interval(), best_val =0.0,curr_val =0.0;
    float temp_likehood=0.0,best_lambda=-1;
    // Do Grid search on the likelihood function and see for which lambda, it is getting to
    // the maximum. Maximum likelihood means the most optimal solution.

    for(float val = start_val; val <= end_val; val += interval){
        if(shouldStop) {
            break;
        }
        curr_val = model->f(val);
        if(curr_val >= best_val){
            best_val = curr_val;
            best_lambda = val;
        }
    }

    model->set_lambda(best_lambda);
    model->calculate_sigma(best_lambda);
    model->calculate_beta(best_lambda);
    mtx.unlock();
    return;
}
