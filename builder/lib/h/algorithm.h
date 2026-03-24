#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <string>
#include <memory>
#include <vector>
class Algorithm{
    public:
    Algorithm();
    ~Algorithm();
    //sort
    void printVector(std::vector<int>& v, int lineBreakEvery=5, bool useSpace=true);
    void selectionSort(std::vector<int>& v);   //n^2
    void bubbleSort(std::vector<int>& v);   //n^2
    void mergeSort(std::vector<int>& v, int size=0);    //n*log n
    int quickSort();    //n*log n
    void heapify(std::vector<int>& v);
    int heapSort(std::vector<int>& v);
    //배열을 활용한 이진 트리
    int binaryTree(std::vector<int>& v);
    
};
namespace tree{
    int left(int n);
    int right(int n);
    };
#endif // ALGORITHM_H