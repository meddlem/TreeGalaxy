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
    vector<double> CM;
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
    
    void getpoints(vector<pos*>& results);
};

bool node::isleafnode(void) const{
  return quadrant[0]==NULL;
}

int node::get_quadrant(const pos* point) const{
  int quad = 0;
  if((point->x > origin[0]) && (point->y < origin[1])){
    quad = 1;
  }
  else if((point->x < origin[0]) && (point->y < origin[1])){
    quad = 2;
  }
  else if((point->x < origin[0]) && (point->y > origin[1])){
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
      //cout << point->x << "\n";

      //make the quadrants
      double halfDim_new = halfDim/2;
      vector<double> origin_new(2);

      for(int i=0; i<4; i++){
        origin_new[0] = origin[0] + ((i<2)?1:-1)*halfDim/sqrt(2);
        origin_new[1] = origin[1] + ((i==0||i==3)?1:-1)*halfDim/sqrt(2);
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

double randU(){ // Random number between 1, -1
  double r = 2*rand()/double(RAND_MAX) - 1;
  return r; 
}

void node::getpoints(vector<pos*> &results){
  if(isleafnode()){
    if(data != NULL){
      results.push_back(data);
      //cout << "(" << data->x << " " << data->y << ")" << "\n";
    }
  }
  else{
    for(int i=0; i<4; i++){
      quadrant[i] -> getpoints(results);
    }
  }
}

//double defbox(vector<pos> p){ 
//}

int main(void){
  vector<double> origin = {0,0};
  node* root = new node(2,origin);
  
  vector<pos> p;
  int n = 100;

  // insert particles into tree..
  for(int i = 0; i<n; i++){
    p.push_back (pos());
    p[i].x = (i+1)/double(n);
    p[i].y = (i+1)/double(n);
  }
  
  for(int i = 0; i<n; i++){
    root -> insert_particle(&p[i]);
  }
  /*p.push_back (pos());
  p[0].x = 1;
  p[0].y = -1;
  p.push_back (pos());
  p[1].x = 1;
  p[1].y = 1;
  p.push_back (pos());
  p[2].x = 1.1;
  p[2].y = 1.1;*/

  // return list of results
  vector<pos*> results;
  root -> getpoints(results);
  cout << results[1]->x << "\n";
  cout << results[1]->y << "\n";
  return 0;
}
