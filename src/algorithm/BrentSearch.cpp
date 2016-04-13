// Brent Search// Created by aditya gautam on 3/25/16.//#include "BrentSearch.hpp"#ifdef BAZEL#include "algorithm/AlgorithmOptions.hpp"#include "model/LinearMixedModel.hpp"#else#include "AlgorithmOptions.hpp"#include "../model/LinearMixedModel.hpp"#endif// Default parametersBrentSearch::BrentSearch(){    a = 0.0;    b = 0.0;    c = 0.0;    m = 0.0;    e = 0.0;    t = 0.0;    delta = 0.05;}// Setters and Getters Methodsvoid BrentSearch::set_a(double a){    this->a = a;}void BrentSearch::set_b(double b){    this->b = b;}void BrentSearch::set_c(double c){    this->c = c;}void BrentSearch::set_m(double m){    this->m = m;}void BrentSearch::set_e(double e){    this->e = e;}void BrentSearch::set_t(double t){    this->t = e;}double BrentSearch::get_a(){    return this->a;}double BrentSearch::get_b(){    return this->b;}double BrentSearch::get_c(){    return this->c;}double BrentSearch::get_m(){    return this->m;}double BrentSearch::get_e(){    return this->e;}double BrentSearch::get_t(){    return this->t;}// Get all the basic brent parameteres in vector formvector<double> BrentSearch::get_brent_params(){    vector<double> params;    params.push_back(get_a());    params.push_back(get_b());    params.push_back(get_c());    params.push_back(get_m());    params.push_back(get_e());    params.push_back(get_t());    return params;}/* Brent's search for finding a global minimum between the given interval [a,b]. *    Parameters: a, b -> Starting point and the end point of the search c -> Prior knowledge about the global minimum point m -> Bound on second derivative (keep it to 0 as of now) e -> positive tolerance parameter t -> Positive error tolerance. x -> Value at which the objective function is attaining the minimal value. This Method and functions is inspired from Brent search : https://en.wikipedia.org/wiki/Brent%27s_method */void BrentSearch::run(LinearMixedModel *model) {    double a0,a2,a3,d0,d1,d2,h,m2,macheps,p,q,qs,r,s,sc,y,y0,y1,y2,y3,yb,z0,z1,z2;    double a,b,c,m,e,t,x=0.0,best_lambda_val=0.0;    int k;    vector<double> params =  this->get_brent_params();    a = params.at(0);    b = params.at(1);    c = params.at(2);    m = params.at(3);    e = params.at(4);    t = params.at(5);    // Set the interval based on the best lambda calculated by the Grid Search    best_lambda_val = model->get_lambda();    a = best_lambda_val - delta;    b = best_lambda_val + delta;    a0 = b;    x = a0;    a2 = a;    y0 = model->f(b);    yb = y0;    y2 = model->f(a);    y = y2;    if ( y0 < y ) {        y = y0;    }    else {        x = a;    }    if ( m <= 0.0 || b <= a ) {        model->set_lambda(x);        model->calculate_beta(x);        return;    }    macheps = t*t;    m2 = 0.5 * ( 1.0 + 16.0 * macheps ) * m;    if ( c <= a || b <= c ) {        sc = 0.5 * ( a + b );    }    else {        sc = c;    }    y1 = model->f ( sc );    k = 3;    d0 = a2 - sc;    h = 9.0 / 11.0;    if ( y1 < y ) {        x = sc;        y = y1;    }    while(1) {        d1 = a2 - a0;        d2 = sc - a0;        z2 = b - a2;        z0 = y2 - y1;        z1 = y2 - y0;        r = d1 * d1 * z0 - d0 * d0 * z1;        p = r;        qs = 2.0 * ( d0 * z1 - d1 * z0 );        q = qs;        if ( k < 1000000 || y2 <= y )        {            while(1)            {                if ( q * ( r * ( yb - y2 ) + z2 * q * ( ( y2 - y ) + t ) ) <                     z2 * m2 * r * ( z2 * q - r ) )                {                    a3 = a2 + r / q;                    y3 = model->f ( a3 );                    if ( y3 < y ) {                        x = a3;                        y = y3;                    }                }                k = ( ( 1611 * k ) % 1048576 );                q = 1.0;                r = ( b - a ) * 0.00001 * ( double ) ( k );                if ( z2 <= r ){                    break;                }            }        }        else        {            k = ( ( 1611 * k ) % 1048576 );            q = 1.0;            r = ( b - a ) * 0.00001 * ( double ) ( k );            while ( r < z2 )            {                if ( q * ( r * ( yb - y2 ) + z2 * q * ( ( y2 - y ) + t ) ) <                     z2 * m2 * r * ( z2 * q - r ) )                {                    a3 = a2 + r / q;                    y3 = model->f ( a3 );                    if ( y3 < y )                    {                        x = a3;                        y = y3;                    }                }                k = ( ( 1611 * k ) % 1048576 );                q = 1.0;                r = ( b - a ) * 0.00001 * ( double ) ( k );            }        }        r = m2 * d0 * d1 * d2;        s = sqrt ( ( ( y2 - y ) + t ) / m2 );        h = 0.5 * ( 1.0 + h );        p = h * ( p + 2.0 * r * s );        q = q + 0.5 * qs;        r = - 0.5 * ( d0 + ( z0 + 2.01 * e ) / ( d0 * m2 ) );        if ( r < s || d0 < 0.0 ) {            r = a2 + s;        }        else {            r = a2 + r;        }        if ( p*q > 0 ) {            a3 = a2 + p / q;        }        else {            a3 = r;        }        while(1) {            a3 = max(a3,r);            if ( b <= a3 ){                a3 = b;                y3 = yb;            } else {                y3 = model->f ( a3 );            }            if ( y3 < y ) {                x = a3;                y = y3;            }            d0 = a3 - a2;            if ( a3 <= r )                break;            p = 2.0 * ( y2 - y3 ) / ( m * d0 );            if ( ( 1.0 + 9.0 * macheps ) * d0 <= abs(p) )                break;            if ( 0.5 * m2 * ( d0 * d0 + p * p ) <= ( y2 - y ) + ( y3 - y ) + 2.0 * t )                break;            a3 = 0.5 * ( a2 + a3 );            h = 0.9 * h;        }        if ( b <= a3 )            break;        a0 = sc;        sc = a2;        a2 = a3;        y0 = y1;        y1 = y2;        y2 = y3;    }    cout << "Brent Search : Optimal neg loglikelihood = " << y << " at lambda = "  << x << endl;    model->set_lambda(x);    model->calculate_beta(x);}