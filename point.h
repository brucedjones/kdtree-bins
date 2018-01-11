#ifndef POINT_H
#define POINT_H

#include <vector>

template <class T>
class Point {
public:
    Point(){};
    Point(std::vector<T> xyz) : xyz(xyz){
        dim = xyz.size();
    };
    ~Point(){};

    bool operator==(const Point &rhs){
        return rhs.xyz==this->xyz;
    }

    T& operator[] (size_t i) {
          return xyz[i];
      }

    friend std::ostream &operator<<(std::ostream &os, Point<T> &pt) { 
        os << "{";
        for(size_t i = 0; i<pt.xyz.size()-1; i++){
            os << pt.xyz[i] << ",";
        }
        os << pt.xyz[pt.xyz.size()-1];
        os << "}";
        return os;
    }

    T Distance2(Point<T> pt){
        T dist = 0;
        size_t maxDim = pt.xyz.size();
        if(this->xyz.size() < maxDim) maxDim = this->xyz.size();
        for(size_t i=0; i<maxDim; i++) dist += (pt.xyz[i]-this->xyz[i])*(pt.xyz[i]-this->xyz[i]);
        return dist;
    }

    std::vector<T> xyz;
    size_t dim;

};

template<class T>
class compare_points_by_axis
{
private:
    int axis;
public:
    compare_points_by_axis(int axis):axis(axis){}

    bool operator()(Point<T> a,Point<T> b) const
    {
        return a[axis]<b[axis]; 
    }
};


#endif