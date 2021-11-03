//
// Created by balint.galgoczi on 2021.08.10..
//
#include <algorithm>
#include <map>
#include "util.h"
#include <set>
#include <iomanip>
#include <random>

std::vector<double> getDistribution(double x_mu, double x_sigma, size_t numberOfValues){
    // shortened compared to your example:
    std::mt19937 gen((std::random_device())());
    // create temporary (anonymous)     ^^
    // instance and call it immediately    ^^
    // afterwards

    std::normal_distribution<double> nd(x_mu, x_sigma);
    std::vector<double> result;
    result.reserve(numberOfValues);
    while(numberOfValues-- > 0)
    {
        // shorter than above: using result of previous
        // function (functor!) call directly as argument to next one
        result.push_back(nd(gen));
    }

    // finally something familiar from python:
    return result;
}

std::vector<std::vector<double>> transpose(const std::vector<std::vector<double>> &m)
{
    std::vector<std::vector<double>> result(m[0].size(), std::vector<double>(m.size()));

    for (std::vector<double>::size_type i(0); i < m[0].size(); ++i)
        for (std::vector<double>::size_type j(0); j < m.size(); ++j)
            result[i][j] = m[j][i];

    return result;
}

double arithmetic_mean(const std::vector<double> &vector){
    double sum = 0;
    for (int i = 0; i < vector.capacity(); i++)
    {
        sum += vector[i];
    }

    return sum / vector.capacity();
}

int binarySearch(const std::vector<std::pair<int, double>> &vec, double &item, int s1, int s2) {
    if (s1 > s2)
        return -1;

    auto middle = (s1 + s2) / 2;

    if (item == vec.at(middle).second)
        return middle;

    if (item > vec.at(middle).second)
        return binarySearch(vec, item, middle + 1, s2);
    else
        return binarySearch(vec, item, s1, middle - 1);
}

int searchVector(const std::vector<std::pair<int, double>> &vec, double &item) {
    return binarySearch(vec, item, 0, vec.size() - 1);
}

bool sortByVal(const std::pair<int, double> &a,
               const std::pair<int, double > &b)
{
    return (a.second < b.second);
}

std::vector<std::pair<int, double>> sortByAscending(std::map<int, double>& M)
{
    std::vector<std::pair<int, double>> vec;

    // copy key-value pairs from the map to the vector
    std::map<int, double> :: iterator it2;
    for (it2=M.begin(); it2!=M.end(); it2++)
    {
        vec.push_back(std::make_pair(it2->first, it2->second));
    }

    // // sort the vector by increasing order of its pair's second value
    sort(vec.begin(), vec.end(), sortByVal);

    return vec;
}


std::vector<int> randomWeightedIndices(int draws, std::vector<double> weights)
{
    int nweights = weights.capacity();
    std::vector<int> returnedindices(draws, 0);
    std::vector<double> cumulativeWeights(nweights, 0.0);

    std::random_device randomDevice;
    std::mt19937 randomGen(randomDevice());
    std::uniform_real_distribution<double> uniformdistribution(0.0, 1.0);

    // calculate the cumulative weights
    double s = weights[0];
    cumulativeWeights[0] = s;
    for (int i = 1; i < nweights; ++i) {       
        s += weights[i];     
        cumulativeWeights[i] = s;
    }

    // for each random draw
    for (int i = 0; i < draws; ++i) {
        double r = uniformdistribution(randomGen);
        // this is not optimal, use binary search instead
        for (int k = 0; k < nweights; ++k) {
            if (r < cumulativeWeights[k]) {
                returnedindices[i] = k;
                break;
            }
        }
    }
    return returnedindices;
}



void histogram(std::vector<double> data, int nbins ){
    int n = data.capacity();
    double histmax = 20.0;
    // auto [dmin, dmax] = std::minmax_element(begin(data), end(data));
    double dmin = 1.0e20;
    double dmax = -1.0e20;
    std::vector<double> bins(nbins, 0.0);
    std::vector<double> bounds(nbins+1, 0.0);
    
    // determine histogram bins
    for (int i = 0; i < n; ++i)
    {
        if (data[i] < dmin)
        {
            dmin = data[i];
        }
        if (data[i] > dmax)
        {
            dmax = data[i];
        }
    }
    double binsize = (dmax-dmin) / (float)nbins;
    for (int i = 0; i < (nbins + 1); ++i)
    {   
        bounds[i] = dmin + i*binsize;
    }    


    // histogram: this is inefficient but whatever:
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < nbins; ++j)
        {
            if (bounds[j] < data[i] && data[i] < bounds[j+1])
                bins[j] += 1.0;
        }
    }

    // normalize
    double maxcount = 0.0;
    for (int i = 0; i < nbins; ++i)
    {
        if (bins[i] > maxcount)
            maxcount = bins[i];
    }
    std::cout << maxcount << "\n";
    for (int i = 0; i < nbins; ++i)
    {
        bins[i] = bins[i] * (histmax/maxcount);
    }


    // plotting    
    for (int i = 0; i < 20 + (int)round(histmax); ++i)
    {
        std::cout << "_";
    }
    std::cout << "\n";
    for (int i = 0; i < nbins; ++i)
    {
        int counts = (int)round(bins[i]);
        std::cout << std::setw(12) << 0.5*(bounds[i]+bounds[i+1]) << " |";
        for (int j = 0; j < counts; ++j)
        {
            std::cout << "X";
        }
        std::cout << "\n";
    }

    std::cout << "samples: " << n << ", bins: " << nbins << '\n';
    std::cout << "min = " << dmin << ", max = " << dmax << ", bin size: " << binsize << '\n';
    return;
}

std::vector<double> resampling(std::vector<double> vector, std::vector<int> indices){
    std::vector<double> resampledResult(vector.capacity());
    for (int i = 0; i < vector.capacity(); ++i) {
        resampledResult[i]= vector[indices[i]];
    }
    return resampledResult;
}

std::vector<std::vector<double>> resampling(std::vector<std::vector<double>> vector, std::vector<int> indices){
    std::vector<std::vector<double>> resampledResult(vector.capacity());
    for (int i = 0; i < vector.capacity(); ++i) {
        resampledResult[i]= vector[indices[i]];
    }
    return resampledResult;
}