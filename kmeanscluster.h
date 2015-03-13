#ifndef KMEANSCLUSTER_H
#define KMEANSCLUSTER_H
#include <vector>
#include <cstdlib>
#include <math.h>

/*
 * KMeans algorithm implemented in Templated C Class
 * So you can use this class to analize data from int to float
 *
 *
 * limitation :
 * - Generated centroid is float despite your data is double. For
 *  majority case this is enough.
 * - It can't process non numeric data
 * - It is only for two dimension data
 *
 * usage example
    int n = 3;
    int height = 20;
    int width = 20;
    vector<float> centroid = vector<float>(n);
    vector<vector<int> > data = vector<vector<int> >(height);
    for(int i = 0; i < height; i++){
        for(int j =0; j < width; j++ ){
            data[i].push_back(rand() % 255 + 0);
        }
    }
    KmeansCluster<int> k(data,n,20,20);
    centroid = k.genCentroid();
 **/
using namespace std;
typedef struct coordinate {
    int x;
    int y;
}coordinate;

template<class c_type> class KmeansCluster
{
public:
    KmeansCluster();
    KmeansCluster(vector<vector<c_type> > data, int nCentroid, int height,int width );
    void setData(vector<vector<c_type> > data);
    // generate new centroid and return the value
    vector<float> genCentroid();
    // get last generated centroid
    vector<float> getCentroid();
    // get clustered pos
     vector<vector<coordinate> > getClustered();

private:
    int nCentroid;
    int height;
    int width;
    vector<vector<c_type> > data;
    vector<vector<coordinate> > clustered;
    vector<float> centroid;

    void setFirstCentroid();
    c_type getMax();
    c_type getMin();
    float getMeans(vector<coordinate> cluster);
};

template <class c_type> KmeansCluster<c_type>::KmeansCluster()
{

}

template <class c_type> KmeansCluster<c_type>::KmeansCluster(vector<vector<c_type> > data, int nCentroid, int height,int width)
{
    this->data = data;
    this->nCentroid = nCentroid;
    this->height = height;
    this->width = width;
    clustered = vector<vector<coordinate> >(nCentroid,vector<coordinate>());
    centroid = vector<float>(nCentroid);
}

template<class c_type> void KmeansCluster<c_type>::setFirstCentroid()
{
    c_type maxV =getMax();
    c_type minV =getMin();
    for(int i = 0;i<nCentroid; i++)
    {
        centroid[i] = rand()%(int)maxV + (int)minV;
    }
}
template<class c_type> vector<vector<coordinate> > KmeansCluster<c_type>::getClustered()
{
    return clustered;
}

template<class c_type> vector<float> KmeansCluster<c_type>::genCentroid()
{
    bool swap = true;
    vector<vector<coordinate> > clusteredTmp = vector<vector<coordinate> >(nCentroid,vector<coordinate>());
    coordinate co;

    setFirstCentroid();
    // first iteration
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            //calculate distance from centroid
            vector<float> distance = vector<float>(nCentroid);
            for(int c=0;c<nCentroid;c++){
                distance[c]=abs((int)(centroid[c]-data[i][j]));
            }
            co.x = i;
            co.y = j;
            // get centroid with min distance
            int pos = 0;
            int tmp = distance[0];
            for(int k=0;k<nCentroid;k++){
                if(tmp>distance[k]){
                    tmp = distance[k];
                    pos = k;
                }
            }
            // insert point coordinate to cluster
            clustered[pos].push_back(co);
        }
    }
    //calculate new K from first iteration
    for(int i = 0;i<nCentroid;i++){
        centroid[i] = getMeans(clustered[i]);
    }
    while(swap)
    {
        for(int pin =0;pin<nCentroid;pin++){
            clusteredTmp[pin].clear();
        }
        swap = false;
        for(int i=0;i<nCentroid;i++){
            for(vector<coordinate>::iterator it = clustered[i].begin();it!=clustered[i].end();++it){
                coordinate co = *it;
                //calculate distance from centroid
                float distance[nCentroid];
                for(int c=0;c<nCentroid;c++){
                    distance[c]=abs((int)(centroid[c]-data[co.x][co.y]));
                }
                int tmp = distance[0];
                int pos = 0;
                for(int k=0;k<nCentroid;k++){
                    if(tmp>distance[k]){
                        tmp = distance[k];
                        pos = k;
                    }
                }
                if(pos != i){
                    swap = true;
                }
                clusteredTmp[pos].push_back(co);
            }
            int p=0;
            p++;
        }
        for(int pin =0;pin<nCentroid;pin++){
            clustered[pin].clear();
        }
        clustered = clusteredTmp;
        // calculate new K
        for(int i = 0;i<nCentroid;i++){
            centroid[i] = getMeans(clustered[i]);
        }
    }
    return centroid;
}

template<class c_type> c_type KmeansCluster<c_type>::getMax()
{
    c_type max = data[0][0];

    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(data[i][j]>max)
                max=data[i][j];
        }
    }
    return max;
}

template<class c_type> c_type KmeansCluster<c_type>::getMin()
{
    c_type min = data[0][0];
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(data[i][j]<min)
                min=data[i][j];
        }
    }
    return min;
}

template<class c_type> float KmeansCluster<c_type>::getMeans(vector<coordinate> cluster)
{
    float means;
    float sum=0;
    int n=0;
    coordinate co;
    for(vector<coordinate>::iterator it = cluster.begin();it!=cluster.end();++it){
        co = *it;
        sum = sum + data[co.x][co.y];
        n++;
    }
    means = sum/n;
    return means;
}
#endif // KMEANSCLUSTER_H
