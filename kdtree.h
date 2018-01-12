#ifndef KDTREE_H
#define KDTREE_H

#include "point.h"
#include <vector>
#include <set>
#include <climits>
#include <iostream>
#include <algorithm>

#define SAMPLE_N 20

template<class T>
class kdtree {
public:
    
    kdtree(){};

    kdtree(std::vector<Point<T>> pts, int depth=0, kdtree<T> *parent=nullptr, int maxPts=3) : depth(depth), parent(parent), maxPts(maxPts) {

        if(pts.size()==0) return;
        
        d = pts[0].dim;
        axis = depth % d;

        if(pts.size()<=maxPts) {
            this->pts = pts;
            median = Point<T>({0});
        } else{
            setMedian(pts);
            splitPoints(median,pts);
        }
    };

    kdtree(Point<T> pt, int depth=0, kdtree<T> *parent=nullptr): depth(depth), parent(parent){
        axis = depth % pt.dim;
        median = Point<T>({0});
        pts.push_back(pt);
        median = Point<T>({0});
    };

    ~kdtree(){
        if(left!=nullptr) delete left;
        if(right!=nullptr) delete right;
    };

    void setMedian(std::vector<Point<T>> pts){
        std::vector<Point<T>> samplePts;
            
        if(pts.size()>SAMPLE_N){
            std::set<int> generated;
            int num;
            for (size_t i = 0; i < SAMPLE_N; i++)
            {
                while(generated.size()<i){
                    num = rand() % pts.size();
                    generated.insert(num);
                }
                samplePts.push_back(pts[num]);
            }
        } else {
            samplePts = pts;
        }

        std::sort(samplePts.begin(), samplePts.end(), compare_points_by_axis<T>(axis));
        if(samplePts.size()%2 == 0){
            median = (samplePts[samplePts.size()/2]+samplePts[(samplePts.size()/2)-1])/2;
        } else median = samplePts[samplePts.size()/2];
    }

    void splitPoints(Point<T> median, std::vector<Point<T>> pts){
            std::vector<Point<T>> leftPts;
            for(std::vector<Point<T>>::iterator pt = pts.begin(); pt!= pts.end();){
                if((*pt)[axis] <= median[axis]){
                    leftPts.push_back(*pt);
                    pt = pts.erase(pt);
                } else ++pt;
            }

            if(leftPts.size()>0) left = new kdtree<T>(leftPts, depth+1, this);
            if(pts.size()>0) right = new kdtree<T>(pts,depth+1, this);
    }

    Point<T> findNN(Point<T> pt) {
        if(!this->left && !this->right){
            Point<T> nearest = pts[0];
            for(size_t i=1;i<pts.size();i++){
                if(pt.Distance2(pts[i]) < pt.Distance2(nearest)) nearest=pts[i];
            }
            return nearest;
        }

        Point<T> nearestChild;
        if(pt[axis] <= this->median[axis]) nearestChild = this->left->findNN(pt);
        else nearestChild = this->right->findNN(pt);

        if(pt.Distance2(this->median)<pt.Distance2(nearestChild)){
            if(pt[axis] > this->median[axis] && this->left) nearestChild = this->left->findNN(pt);
            else if(this->right) nearestChild = this->right->findNN(pt);
        }

        return nearestChild;
    }

    void findNeighborList(Point<T> pt, T r, std::vector<Point<T>> &nlist){
        if(!this->left && !this->right){
            for(std::vector<Point<T>>::iterator tpt=pts.begin(); tpt!=pts.end();++tpt){
                if((*tpt).Distance2(pt)<=r*r) nlist.push_back(*tpt);
            }
        } else if(abs(pt[axis]-this->median[axis])<=r){
            if(this->left != nullptr) this->left->findNeighborList(pt,r,nlist);
            if(this->right != nullptr) this->right->findNeighborList(pt,r,nlist);
        }

        return;
    }

    void add(Point<T> pt){
        if(!this->left && !this->right){
            this->pts.push_back(pt);
            if(this->pts.size() > maxPts) {
                setMedian(pts);
                splitPoints(median,pts);
                this->pts.clear();
            }
        } else {
            if(pt[axis] <= this->median[axis]) 
                this->left->add(pt);
            else 
                this->right->add(pt);
        }
    }

    void add(std::vector<Point<T>> pts){
        for(std::vector<Point<T>>::iterator pt=pts.begin(); pt!=pts.end(); ++pt) this->add(*pt);
    }

    std::vector<Point<T>> toVector(){
        std::vector<Point<T>> out;
        this->toVector(out);
        return out;
    }

    void toVector(std::vector<Point<T>> &vec){
        vec.insert(vec.end(), this->pts.begin(), this->pts.end());
        if(this->left) this->left->toVector(vec);
        if(this->right) this->right->toVector(vec);
        return;
    }

    kdtree<T> balanced(){
        std::vector<Point<T>> pts = this->toVector();
        return kdtree<T>(pts);
    }

    void remove(Point<T> pt){
        if(this->left && left->pts.size()>0){
            std::vector<Point<T>>::iterator toRemove = std::find(left->pts.begin(), left->pts.end(),pt);
            if(toRemove != left->pts.end()) left->pts.erase(toRemove);
            if(left->pts.size()==0){
                median = Point<T>({0});
                pts = right->pts;
                delete left;
                left = nullptr;
            }
        }

        if(this->right && right->pts.size()>0){
            std::vector<Point<T>>::iterator toRemove = std::find(right->pts.begin(), right->pts.end(),pt);
            if(toRemove != right->pts.end()) right->pts.erase(toRemove);
            if(right->pts.size()==0){
                median = Point<T>({0});
                pts = right->pts;
                delete right;
                right = nullptr;
            }
        }

        if(this->left && pt[axis] <= this->median[axis]) this->left->remove(pt);
        else if (this->right) this->right->remove(pt);
    }
    
    friend std::ostream &operator<<(std::ostream &os, kdtree<T> &tree) { 
        os << tree.median;
        os << "<";
        for(std::vector<Point<T>>::iterator pt=tree.pts.begin(); pt!=tree.pts.end(); ++pt) os << *pt;
        os << ">,[";
        if(tree.left!=nullptr) os << *(tree.left);
        os << "],[";
        if(tree.right!=nullptr) os << *(tree.right);
        os << "]";
        return os;
    }

    size_t d;
    int depth;
    int axis;
    Point<T> median;
    kdtree<T> *left = nullptr;
    kdtree<T> *right = nullptr;
    kdtree<T> *parent = nullptr;
    std::vector<Point<T>> pts;
    int maxPts = 5;

};

#endif