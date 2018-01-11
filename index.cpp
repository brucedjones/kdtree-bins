#include "point.h"
#include "kdtree.h"
#include <vector>
#include <iostream>

int main(){
    /*std::vector<Point<int>> pts = {Point<int>({2,0}),
        Point<int>({3,1}),
        Point<int>({4,2}),
        Point<int>({5,3}),
        Point<int>({6,4}),
        Point<int>({7,5})};
    
    kdtree<int> tree(pts);
    std::cout << tree << std::endl;

    Point<int> nearest = tree.findNN(Point<int>({2,3}));
    std::cout << nearest << std::endl;

    std::vector<Point<int>> neighbors;
    tree.findNeighborList(Point<int>({4,2}), 3, neighbors);
    std::cout << "Neighbors:" << std::endl;
    for(std::vector<Point<int>>::iterator pt = neighbors.begin(); pt != neighbors.end(); ++pt) std::cout << *pt << std::endl;

    tree.add(Point<int>({5,4}));
    std::cout << tree << std::endl;
    nearest = tree.findNN(Point<int>({5,2}));
    std::cout << nearest << std::endl;

    std::vector<Point<int>> ptsFromTree = tree.toVector();
    for(std::vector<Point<int>>::iterator pt=ptsFromTree.begin(); pt!=ptsFromTree.end(); ++pt) std::cout << *pt << std::endl;

    tree.add(Point<int>({5,4}));
    tree.add(Point<int>({1,4}));
    tree.add(Point<int>({2,3}));
    tree.add(Point<int>({1,1}));
    tree.add(Point<int>({0,7}));
    kdtree<int> balancedTree = tree.balanced();
    std::cout << "unbalanced tree: " << tree << std::endl;
    std::cout << "  balanced tree: " << balancedTree << std::endl;*/

    
    std::vector<Point<int>> pts2 = {Point<int>({2,0}),
        Point<int>({3,1}),
        Point<int>({4,2}),
        Point<int>({5,3}),
        Point<int>({6,4}),
        Point<int>({7,5})};
    kdtree<int> tree2(pts2);
    std::cout << tree2 << std::endl;
    tree2.remove(Point<int>({6,4}));
    std::cout << tree2 << std::endl;


    return 0;
}