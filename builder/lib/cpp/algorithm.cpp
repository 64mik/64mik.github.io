#include "../../lib/h/algorithm.h"
#include <iostream>
#include <vector>

int tree::left(int n){
    return n*2+1; 
}
int tree::right(int n){
    return n*2+2;
}
void Algorithm::heapify(std::vector<int>& v){
    int l, r, root;
    for(int i =0; i<v.size();i++){
        l = tree::left(i);
        r = tree::right(i);
        if(v[i]>v[l]){std::swap(v[i],v[l]);}
        if(v[i]>v[r]){std::swap(v[i],v[l]);}
        
    }//미완 
}
int Algorithm::heapSort(std::vector<int>& v){
    //v를 받아서 내부에 값을 정렬 하는데, 일단 힙 상태인지 알 수 없잖아
    int root,left,right, max;
    max = v.size();
    for(int i=0; i<max; i++){
     //인덱스 123 받아서 트리로 만들어 힙구조로 바꿔   
    }

    return 0;
}
int Algorithm::binaryTree(std::vector<int>& v){
    
    return 0;
}

void Algorithm::mergeSort(std::vector<int>& v, int index){    //n*log n  ㅜ= 15  8(7까지) 7(14까지)
    if(index==-1){   //사이즈 초기화
        if(!v.empty()){
            index = v.size()-1;
        } //50이라면 49
    }
    int size = index/2;  //49/2 =24 /+ 1~index       49/2  0~24 25~49

    if(size>2){ //분할
            //24랑 25보내고 인덱스 -4

    }
    else{   //분할된 것의 계산
        //계산한것 만큼 인덱스 차감
        if(size==1){
            return;
        }
        else if(size==2){
            index-=2;
        }
    }
    int halfSize = halfSize/2;  //인덱스는 0~15 그럼 /2=> 7 실제론 0~7 8~n이어야함
    if(size <= 2){  //값이 하나 혹은 2개 맨 처음꺼 빼고 나누기 2 하면 되지 그럼 1~크기-1이잖 01 23 45 67 89 1011 12
        //35 72 112 124 52 10
    }//size - 2
    else{
        if(v.size()%2==0){  //크기가 짝수
            mergeSort(v,4);
        }
        else{   //홀수

        }//크기가 2 이하가 될 때까지 분해
    }
    mergeSort(v,5);
//재귀 필요, 분할
}
int Algorithm::quickSort(){    //n*log n
//재귀 필요, 분할 
return 0;
}
Algorithm::Algorithm(){
}
Algorithm::~Algorithm(){

}
//done

void Algorithm::printVector(std::vector<int>& v, int lineBreakEvery, bool useSpace){
    int count=0;
    for(int x : v){
        count++;
        std::cout<<x;
        if(useSpace){
            std::cout<<' ';
        }
        if(count % lineBreakEvery == 0){    //몫이 0 이면
            std::cout<<'\n';
        }
    }
}
void Algorithm::selectionSort(std::vector<int>& v){   //n^2, 맨 앞에 최소값
    size_t temp;
    for(size_t i=0; i<v.size();i++){   //i=0~n
        for(size_t j=i; j<v.size();j++){   //i=0일 때 j i~ n 0 0 01 02 03 10 11 12
            if(v.at(i)>v.at(j)){
                temp = v.at(i);
                v.at(i)=v.at(j);
                v.at(j)=temp;
            }
        }
    }
}
void Algorithm::bubbleSort(std::vector<int>& v){   //n^2    0 1 2 3 4 5 6
    int temp;
    for(size_t i = 0; i < v.size(); i++){
        for(size_t j = 0; j < v.size()-1; j++){
            if(v.at(j)>v.at(j+1)){
                temp = v.at(j);
                v.at(j) = v.at(j+1);
                v.at(j+1) = temp;
            }
        }
    }
}