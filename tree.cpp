#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

struct pos {
 double x;
 double y;
};

class node{
  private:
    pos *data; 
    vector<double> origin;
    double mass;
    double halfDim;
    node *quadrant[4];

  public:
    node(const double halfDim, const vector<double> origin)
      :origin(origin), halfDim(halfDim), data(NULL){
      for (int i=0; i<4; i++){
        quadrant[i] = NULL;
      }
    }

    ~node(void){
      for (int i=0; i<4; i++)
      {
        delete quadrant[i];
      }
    }

    //prototypes
    bool isleafnode(void) const;
    int get_quadrant(const pos* point) const;
    void insert_particle(pos* point);
};

bool node::isleafnode(void) const{
  return quadrant[0]==NULL;
}

int node::get_quadrant(const pos* point) const{
  int quad = 0;
  if(point->x > origin[1] && point->y < origin[2]){
    quad = 1;
  }
  else if(point->x < origin[1] && point->y < origin[2]){
    quad = 2;
  }
  else if(point->x < origin[1] && point->y > origin[2]){
    quad = 3;
  }

  return quad; 
}

void node::insert_particle(pos* point){
  if(isleafnode()){
    if(data == NULL){
      data = point;
      return;
    }
    else{
      pos* point_old = data;
      data = NULL;

      //make the quadrants
      double halfDim_new = halfDim/2;
      vector< double> origin_new;

      for(int i=0; i<4; i++){
        origin_new[0] = origin[0] + halfDim/sqrt(2)*((i<2)?1:-1);
        origin_new[1] = origin[1] + halfDim/sqrt(2)*((i==0||i==4)?1:-1);
        quadrant[i] = new node(halfDim_new, origin_new);
      }
      
      int qold = get_quadrant(point_old);
      int qnew = get_quadrant(point);

      quadrant[qold] -> insert_particle(point_old);
      quadrant[qnew] -> insert_particle(point);
    }
  }
  else{
  int q = get_quadrant(point);
  quadrant[q] -> insert_particle(point); 
  }
}

int main(void){
  return 0;
}
