#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>

std::vector<std::vector<int>> create_matrix(size_t n,int prob){
    std::vector<std::vector<int>> matrix(n,std::vector<int>(n,0));

    for(size_t i=0;i<n;i++){
        for(size_t j=i+1;j<n;j++){
            int value=rand()%100<prob;
            matrix[i][j]=value;
            matrix[j][i]=value;
        }
    }
    return matrix;
}

void print_matrix(const std::vector<std::vector<int>>& matrix){
    for(size_t i=0;i<matrix.size();i++){
        for(size_t j=0;j<matrix[i].size();j++){
            std::cout<<matrix[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}

void BFS(const std::vector<std::vector<int>>& matrix,std::vector<bool>& NUM,size_t start,bool print=true){
    std::queue<size_t> Q;
    Q.push(start);
    NUM[start]=true;

    while(!Q.empty()){
        size_t v=Q.front();
        Q.pop();

        if(print)std::cout<<v+1<<"\n";

        for(size_t i=0;i<matrix.size();i++){
            if(matrix[v][i]==1&&!NUM[i]){
                NUM[i]=true;
                Q.push(i);
            }
        }
    }
}

std::vector<std::vector<int>> create_spisok(const std::vector<std::vector<int>>& matrix){
    std::vector<std::vector<int>> neighbors(matrix.size());

    for(size_t i=0;i<matrix.size();i++){
        for(size_t j=0;j<matrix[i].size();j++){
            if(matrix[i][j]==1)neighbors[i].push_back(j+1);
        }
    }
    return neighbors;
}

void print_spisok(const std::vector<std::vector<int>>& neighbors){
    for(size_t i=0;i<neighbors.size();i++){
        std::cout<<i+1<<": ";

        for(int schet:neighbors[i]){
            std::cout<<schet<<" ";
        }
        std::cout<<"\n";
    }
}

void BFS_spisok(const std::vector<std::vector<int>>& spisok,std::vector<bool>& NUM,size_t start){
    std::queue<size_t> Q;
    Q.push(start);
    NUM[start]=true;

    while(!Q.empty()){
        size_t v=Q.front();
        Q.pop();
        std::cout<<v+1<<"\n";

        for(int neighbor:spisok[v]){
            if(!NUM[neighbor-1]){
                NUM[neighbor-1]=true;
                Q.push(neighbor-1);
            }
        }
    }
}

struct Node{
    size_t vertex;
    Node* next;
};

void BFS_dop(const std::vector<std::vector<int>>& matrix,std::vector<bool>& NUM,size_t start,bool print=true){
    Node* first=new Node{start,nullptr};
    Node* last=first;
    NUM[start]=true;

    while(first!=nullptr){
        size_t v=first->vertex;
        Node* old=first;
        first=first->next;
        delete old;

        if(first==nullptr)last=nullptr;
        if(print)std::cout<<v+1<<"\n";

        for(size_t i=0;i<matrix.size();i++){
            if(matrix[v][i]==1&&!NUM[i]){
                NUM[i]=true;
                Node* node=new Node{i,nullptr};

                if(last==nullptr){
                    first=node;
                }
                else{
                    last->next=node;
                }

                last=node;
            }
        }
    }
}

double measure_time(const std::vector<std::vector<int>>& matrix,bool own_queue){
    std::vector<bool> NUM(matrix.size(),false);
    clock_t begin=clock();

    for(size_t i=0;i<matrix.size();i++){
        if(!NUM[i]){
            if(own_queue){
                BFS_dop(matrix,NUM,i,false);
            }
            else{
                BFS(matrix,NUM,i,false);
            }
        }
    }

    clock_t end=clock();
    return static_cast<double>(end-begin)/CLOCKS_PER_SEC;
}

int main(){
    srand(static_cast<unsigned>(time(nullptr)));

    int n;
    std::cout<<"Введите количество вершин: ";

    if(!(std::cin>>n)||n<=0){
        std::cout<<"НЕВЕРНЫЙ РАЗМЕР!\n";
        return 1;
    }

    int prob;
    std::cout<<"Введите вероятность ребра от 0 до 100: ";

    if(!(std::cin>>prob)||prob<0||prob>100){
        std::cout<<"НЕВЕРНАЯ ВЕРОЯТНОСТЬ!\n";
        return 1;
    }

    std::vector<std::vector<int>> matrix=create_matrix(n,prob);

    std::cout<<"\nМАТРИЦА СМЕЖНОСТИ:\n";
    print_matrix(matrix);

    std::vector<bool> NUM(n,false);

    std::cout<<"\nОБХОД В ШИРИНУ ПО МАТРИЦЕ:\n";

    for(size_t i=0;i<NUM.size();i++){
        if(!NUM[i]){
            BFS(matrix,NUM,i);
        }
    }

    std::vector<std::vector<int>> spisok=create_spisok(matrix);

    std::cout<<"\nСПИСОК СМЕЖНОСТИ:\n";
    print_spisok(spisok);

    NUM.assign(n,false);

    std::cout<<"\nОБХОД В ШИРИНУ ПО СПИСКУ СМЕЖНОСТИ:\n";

    for(size_t i=0;i<NUM.size();i++){
        if(!NUM[i]){
            BFS_spisok(spisok,NUM,i);
        }
    }

    NUM.assign(n,false);

    std::cout<<"\nОБХОД В ШИРИНУ С СОБСТВЕННОЙ ОЧЕРЕДЬЮ:\n";

    for(size_t i=0;i<NUM.size();i++){
        if(!NUM[i]){
            BFS_dop(matrix,NUM,i);
        }
    }

    std::cout<<"\nСРАВНЕНИЕ ВРЕМЕНИ В СЕКУНДАХ:\n";
    std::cout<<"Вершины std::queue Своя очередь\n";

    for(size_t k=1;k<=3;k++){
        size_t count=static_cast<size_t>(n)*k;

        std::vector<std::vector<int>> test_matrix=create_matrix(count,prob);

        double standard_time=measure_time(test_matrix,false);
        double own_time=measure_time(test_matrix,true);

        std::cout<<count<<"\t"<<standard_time<<"\t\t"<<own_time<<"\n";
    }

    return 0;
}