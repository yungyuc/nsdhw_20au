#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;
class Line
{
private:
    //vector<int> vec0;
vector<int> vec0;
vector<int> vec1;
public:
    Line(){};
    Line(int p){
        vec0.resize(p);
        vec1.resize(p);
    }
    ~Line(){
        vec0.clear();
        vec1.clear();
    }
    int size() {
        return vec0.size();
    };
    void x(int it,int d){
        vec0[it]=d;
    }
    int x(int it){
        return vec0.at(it);
    }
    void y(int it,int d){
        vec1[it]=d;
    }
    int y(int it){
        return vec1.at(it);
    }

};
int main(int, char **)
{
    Line line(3);
    line.x(0,0); line.y(0, 1);
    line.x(1, 1); line.y(1, 3);
    line.x(2, 2); line.y(2, 5);

    Line line2(line);
    line2.x(0, 9);

    std::cout << "line: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line.x(it)
                  << " y = " << line.y(it) << std::endl;
    }

    std::cout << "line2: number of points = " << line.size() << std::endl;
    for (size_t it=0; it<line.size(); ++it)
    {
        std::cout << "point " << it << ":"
                  << " x = " << line2.x(it)
                  << " y = " << line2.y(it) << std::endl;
    }

    return 0;
}