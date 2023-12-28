
#undef NDEBUG
#include <assert.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include "generate_xy.h"
#include "XY.h"



double euclideanDistance( const XY& point1, const XY& point2 ){
    auto scarto = point1-point2;
    return std::sqrt(scarto*scarto);
}



double calculateKDistance( const std::vector<XY>& data, size_t pointIndex, int k ){

    // memoize
    static std::unordered_map<int,double> memo;
    if(memo.count(pointIndex)>0) return memo[pointIndex];

    std::vector<double> distances;

    for( size_t i = 0; i < data.size(); ++i ){
        if( i == pointIndex )continue;

        double distance = euclideanDistance(data[pointIndex], data[i]);
        distances.push_back(distance);
    }

    std::sort(distances.begin(), distances.end());

    return (memo[pointIndex] = distances[k - 1]);
}




double calculateReachabilityDistance( const std::vector<XY>& data, size_t pointIndex, size_t neighborIndex, int k ){
    double kDistance = calculateKDistance(data, neighborIndex, k);
    double distance = euclideanDistance(data[pointIndex], data[neighborIndex]);
    return std::max(kDistance, distance);
}



double calculateLOF( const std::vector<XY>& data, size_t pointIndex, int k ){
    double sum = 0.0;

    for( size_t i = 0; i < data.size(); ++i ){
        if( i == pointIndex )continue;

        double reachDist = calculateReachabilityDistance(data, pointIndex, i, k);
        
        double lrd = 0.0;
        for( size_t j = 0; j < data.size(); ++j ){
            if( j == pointIndex )continue;

            double reachDistToNeighbor = calculateReachabilityDistance(data, i, j, k);
            lrd += std::max( reachDistToNeighbor, reachDist );
        }

        lrd = lrd / (k * reachDist);
        sum += lrd;
    }

    return sum / (data.size() - 1);
}



int main(){
    // Example usage
    auto data = generate_xy();

    int k = 2; // Number of neighbors for LOF algorithm

    // Calculate LOF for each data point
    for( size_t i = 0; i < data.size(); ++i ){
        double lof = calculateLOF(data, i, k);
        std::cout << "LOF for point " << i << ": " << lof << std::endl;
    }

    return 0;
}


