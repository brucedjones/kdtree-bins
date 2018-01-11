#ifndef KDTREE_H
#define KDTREE_H

#include "point.h"
#include <vector>
#include <set>
#include <climits>
#include <iostream>

#define SAMPLE_N 20

template<class T>
class kdtree {
public:
    
    kdtree(){};

    kdtree(std::vector<Point<T>> pts, int depth=0, kdtree<T> *parent=nullptr) : pts(pts), depth(depth), parent(parent) {

        if(pts.size()>1){
            d = pts[0].dim;
            axis = depth % d;

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
            
            T min = INT_MAX;
            T max = INT_MIN;
            for (size_t i = 0; i < samplePts.size(); i++)
            {
                if(samplePts[i][axis] < min) min = samplePts[i][axis];
                if(samplePts[i][axis] > max) max = samplePts[i][axis];
            }
            T medianSample = (max-min)/2;

            size_t closestID = 0;
            for (size_t i = 1; i < samplePts.size(); i++)
            {
                if(abs(samplePts[i][axis]-medianSample) < abs(samplePts[closestID][axis]-medianSample)) closestID = i;
            }

            median = samplePts[closestID];

            std::vector<Point<T>> leftPts;
            for(std::vector<Point<T>>::iterator pt = pts.begin(); pt!= pts.end();){
                bool remove = false;
                if((*pt) == median) remove = true;
                else {
                    if((*pt)[axis] <= median[axis]){
                        remove = true;
                        leftPts.push_back(*pt);
                    }
                }

                if(remove) pt = pts.erase(pt);
                else ++pt;
            }

            if(leftPts.size()>0) left = new kdtree<T>(leftPts, depth+1, this);
            if(pts.size()>0) right = new kdtree<T>(pts,depth+1, this);
        } else {
            median = pts[0];
        }
        
    };

    kdtree(Point<T> pt, int depth, kdtree<T> *parent): median(pt), depth(depth), parent(parent){
        axis = depth % pt.dim;
    };

    ~kdtree(){
        if(left!=nullptr) delete left;
        if(right!=nullptr) delete right;
    };

    Point<T> findNN(Point<T> pt) {
        if(!this->left && !this->right) return this->median;
        
        Point<T> nearestChild;
        if(pt[axis] <= this->median[axis]){
            if(!this->left) return this->median;
            nearestChild = this->left->findNN(pt);
        }
        else{
            if(!this->right) return this->median;
            nearestChild = this->right->findNN(pt);
        }  

        if(pt.Distance2(this->median)<pt.Distance2(nearestChild)){
            if(pt[axis] > this->median[axis] && this->left != nullptr){
                if(!this->left) return this->median;
                nearestChild = this->left->findNN(pt);
            }
            else {
                if(!this->right) return this->median;
                nearestChild = this->right->findNN(pt);
            }
        }

        if(pt.Distance2(this->median)<pt.Distance2(nearestChild)) return this->median;
        else return nearestChild;
    }

    void findNeighborList(Point<T> pt, T r, std::vector<Point<T>> &nlist){
        if(pt.Distance2(this->median)<=r*r) nlist.push_back(this->median);

        if(abs(pt[axis]-this->median[axis])<=r){
            if(this->left != nullptr) this->left->findNeighborList(pt,r,nlist);
            if(this->right != nullptr) this->right->findNeighborList(pt,r,nlist);
        }

        return;
    }

    void add(Point<T> pt){
        if(pt[axis] <= this->median[axis]){
            if(!this->left) this->left = new kdtree<T>(pt, depth+1, this);
            else this->left->add(pt);
        } else {
            if(!this->right) this->right = new kdtree<T>(pt, depth+1, this);
            else this->right->add(pt);
        }
    }

    std::vector<Point<T>> toVector(){
        std::vector<Point<T>> out;
        this->toVector(out);
        return out;
    }

    void toVector(std::vector<Point<T>> &vec){
        vec.push_back(this->median);
        if(this->left) this->left->toVector(vec);
        if(this->right) this->right->toVector(vec);
        return;
    }

    kdtree<T> balanced(){
        std::vector<Point<T>> pts = this->toVector();
        return kdtree<T>(pts);
    }

    void remove(Point<T> pt){
        if(this->left && this->left->median == pt){
            std::vector<Point<T>> left = this->left->toVector();
            left.erase(left.begin());
            delete this->left;
            this->left = new kdtree<T>(left,depth+1,this);
            return;
        }
        
        if(this->right && this->right->median == pt){
            std::vector<Point<T>> right = this->right->toVector();
            right.erase(right.begin());
            delete this->right;
            this->right = new kdtree<T>(right,depth+1,this);
            return;
        }

        if(pt[axis] <= this->median[axis] && this->left) this->left->remove(pt);
        else if (this->right) this->right->remove(pt);
    }
    
    friend std::ostream &operator<<(std::ostream &os, kdtree<T> &tree) { 
        os << tree.median << ",[";
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

};

#endif