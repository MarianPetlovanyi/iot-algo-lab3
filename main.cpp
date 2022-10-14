#include <iostream>
#include <fstream>
#include <vector>
#define INT_MIN -2147483647

struct Indexes{
    int maxIndexI;
    int maxIndexJ;
};
struct Vertex{
    int exp,i,j;
    std::vector<Vertex*> nextPos;
    void setExp(int val){
        this->exp=val;
    }
};
void setEdges(Vertex *arr[], int size){
    for(int i=size-1;i>0;i--){
        for(int j=0;j<=i;j++ ){
            if(i-1>=0 && j-1>=0){
                arr[i][j].nextPos.push_back(&arr[i-1][j]);
                arr[i][j].nextPos.push_back(&arr[i-1][j-1]);
            }
            else if(i-1>=0 && j-1<0){
                arr[i][j].nextPos.push_back(&arr[i-1][j]);
            }
        }
    }
}
bool checkConnection(Vertex* arr[], Vertex* start, Vertex* end){
    for(int i=0;i<start->nextPos.size();i++){
        if(start->nextPos.at(i)==end){
            return true;
        }
    }
    return false;
}
Indexes getMaxExp(int* exp[], bool* set[], int size){
    int maxExp = INT_MIN;
    struct Indexes indexes;
    for(int i=size-1; i>=0;i--){
        for(int j=0;j<=i;j++){
            if(set[i][j]== false && exp[i][j]>=maxExp){
                maxExp = exp[i][j];
                indexes.maxIndexI = i;
                indexes.maxIndexJ = j;
            }
        }
    }
    return indexes;
}
int maxExpCareer(Vertex *arr[], Vertex* start,int count, int size){

    int* exp[size];
    bool* set[size];
    for (int i=0;i<size;i++){
        exp[i] = new int[i+1];
        set[i] = new bool[i+1];
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<=i;j++){
            exp[i][j]=INT_MIN;
            set[i][j]=false;
        }
    }


    exp[start->i][start->j] = start->exp;
    for (int i=0; i<count-1;i++){
        struct Indexes u = getMaxExp(exp, set, size);
        set[u.maxIndexI][u.maxIndexJ] = true;
        for(int j = size-1; j>=0;j--){
            for (int z = 0; z<=j;z++){
                if(!set[j][z] && checkConnection(arr, &arr[u.maxIndexI][u.maxIndexJ], &arr[j][z])
                    && exp[u.maxIndexI][u.maxIndexJ] !=INT_MIN
                    && exp[u.maxIndexI][u.maxIndexJ]+arr[j][z].exp>exp[j][z]){
                    exp[j][z] = exp[u.maxIndexI][u.maxIndexJ]+arr[j][z].exp;
                }
            }
        }

    }
    return exp[0][0];
}
int main() {
    int level;
    std::ifstream file("input.txt");
    std::string fullstr,regex, str;
    std::vector<int> elements;
    std::vector<int>::iterator it;
    while(!file.eof()) {
        file >> str;
        fullstr = fullstr + " " + str;
        elements.push_back(std::stoi(str));
    }
    sscanf(fullstr.c_str(), "%*c%d",&level);
    Vertex *career[level];

    for (int i=0;i<level;i++){
        career[i] = new Vertex[i+1];
    }
    it = elements.begin()+1;
    for(int i=0;i<level;i++){
        for(int j=0;j<=i;j++){
            career[i][j].setExp(*it);
            career[i][j].i=i;
            career[i][j].j=j;
            //std::cout<<career[i][j].exp<< " ";
            if(it!=elements.end())
                it++;
        }
        //std::cout<<"\n";
    }

    int count;
    for(int i=level;i>0;i--){
        count+=i;
    }

    setEdges(career, level);
    int maxExp=-1;
    for(int i=0;i<level;i++){
        int temp =maxExpCareer(career,&career[level-1][i],count,level);
        if (temp>maxExp){
            maxExp=temp;
        }
    }
    std::ofstream MyFile("out.txt");
    MyFile<<maxExp;
    MyFile.close();
    return 0;
}