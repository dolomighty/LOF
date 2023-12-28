
#ifndef _LOF_h_
#define _LOF_h_

#undef NDEBUG
#include <assert.h>

#include <vector>
#include <cmath>
#include <chrono>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <string>

typedef std::unordered_map< std::string, std::vector<int>> GRID;
// GRID [hashgrid key, es. "123 45"] [dataset IDs, es. 1,4,45,6]

typedef std::vector< std::vector<int>> KNN; 
// KNN [0..dataset.size()] [0..k-1]


template<typename T>
class LOF {

    int m_dens_max;
    const int m_grid_granu = 20;
    GRID m_grid;
    void mk_grid( const std::vector<T>& Ts );

    KNN m_knn;
    void mk_knn( const std::vector<T>& Ts, int k );

    std::vector<double> m_lof;
    double m_lof_max;
    void mk_lof( const std::vector<T>& Ts );

    std::vector<int> find_knn(
        int point_index,
        const std::vector<T>& Ts,
        const int k
    ) const;

    double calc_dens( const std::vector<T>& Ts, int point_index );

public:
    LOF();
    LOF( const std::vector<T>& Ts );

#define GET(NAME) auto NAME(){ return m_##NAME; };
    GET( grid       );
    GET( dens_max   );
    GET( grid_granu );
    GET( knn        );
    GET( lof        );
    GET( lof_max    );
#undef GET

    void run( const std::vector<T>& Ts );

};




template<typename T>
void LOF<T>::run( 
    const std::vector<T>& Ts
){
    mk_knn(Ts,5);
    mk_grid(Ts);
    mk_lof(Ts);

//    for( auto [key,val]: m_grid){
//        // key = "123 456"
//        int x,y;
//        std::sscanf(key.c_str(),"%d %d",&x,&y);
//        int LOF = val.size();
////        fprintf(stderr,"%d %d %d\n", x,y,LOF );
//    }
}





template<typename T>
void LOF<T>::mk_grid( const std::vector<T>& Ts ){
    m_grid.clear();
    m_dens_max = 0;
    for( int i=0; i<Ts.size(); i++ ){
        auto key = Ts[i].hashgrid_key(m_grid_granu);
        m_grid[key].push_back(i);
        if(m_dens_max<m_grid[key].size())m_dens_max=m_grid[key].size();
    }
}






template<typename T>
std::vector<int> LOF<T>::find_knn(
    int point_index,
    const std::vector<T>& Ts,
    const int k
) const {

    struct ID {
        int i;
        double d;
        int operator < ( const ID& rhs ) const {
            return this->d < rhs.d;
        }
    };

    std::vector<ID> IDs;
    IDs.resize(Ts.size());

    for( int i=0; i<Ts.size(); i++ ){
        auto scarto = Ts[point_index]-Ts[i];
        IDs[i].i=i;
//        IDs[i].d=std::sqrt(scarto*scarto);
        IDs[i].d=scarto*scarto;
    }

    std::sort( IDs.begin(), IDs.end());

    std::vector<int> nearest;
    nearest.resize(k);
    for( int i=0; i<k; i++ ){
        nearest[i]=IDs[i+1].i;
//        fprintf(stderr,"nearest %d: %d @ %f\n"
//            ,i
//            ,IDs[i].i
//            ,IDs[i].d
//        );
    }

    return nearest;
}





template<typename T>
LOF<T>::LOF( const std::vector<T>& Ts ){
    // costruttore nop
    // permette l'inferenza del typename, es.
    // std::vector<XY> XYs;
    // auto d = LOF( XYs );
    // invece che
    // std::vector<XY> XYs;
    // std::vector<XY> d = LOF<std::vector<XY>>();
}











template<typename T>
void LOF<T>::mk_knn( const std::vector<T>& Ts, int k ){
    // O(n²)
    // non so quanto si possa ottimizzare.
    // però essendo LOF euristico, potremmo accontentarci
    // anche di un knn approssimato
    // che probabilmente è +veloce da comporre
    m_knn.clear();
    m_knn.resize(Ts.size());
    for( int i=0; i<Ts.size(); i++ ){
        m_knn[i]=find_knn( i, Ts, k );
    }    
}





template<typename T>
double LOF<T>::calc_dens( const std::vector<T>& Ts, int point_index ){

    // media dell'inverso delle distanze
    double avg = 0;
    for( auto neighbor: m_knn[point_index]){
        auto tf = Ts[neighbor]-Ts[point_index];
        avg += 1/std::sqrt(tf*tf);
    }
    return avg/m_knn[point_index].size();

//    // max dell'inverso delle distanze
//    double min = +1E9;
//    for( auto neighbor: m_knn[point_index]){
//        auto tf = Ts[neighbor]-Ts[point_index];
//        min = std::min( min, tf*tf );
//    }
//    return 1/std::sqrt(min);
}






template<typename T>
void LOF<T>::mk_lof( const std::vector<T>& Ts ){
    m_lof.clear();
    m_lof.resize(Ts.size());

    m_lof_max = 0;

    for( int t=0; t<Ts.size(); t++ ){

//        auto key = Ts[t].hashgrid_key(m_grid_granu);
//        int loc_dens = m_grid[key].size();
//
//        double knn_dens = 0;
//        for( auto o: m_knn[t]){
//            auto key = Ts[o].hashgrid_key(m_grid_granu);
//            knn_dens += m_grid[key].size();
//        }
//        knn_dens /= m_knn[t].size();


        double loc_dens = calc_dens(Ts,t);

        // max delle densità remote
        double max_rmt_dens = -1E9;
        for( auto o: m_knn[t]){
            max_rmt_dens = std::max( max_rmt_dens, calc_dens(Ts,o));
        }

//        m_lof[t] = loc_dens;
        m_lof[t] = max_rmt_dens / loc_dens;

        m_lof_max = std::max( m_lof_max, m_lof[t]);

//        fprintf(stderr,"%f = %f / %d\n"
//            ,m_lof[i]
//            ,knn_dens
//            ,loc_dens
//        );
    }    
}






#endif // _LOF_h_

