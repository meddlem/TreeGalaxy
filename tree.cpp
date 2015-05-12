#include <vector>
#include <math.h>

struct pos {
 double x;
 double y;
};

//vector<pos>  position()

//pos[1].x.push

class node
{
  private:
    nodeCM *data; 
    vector<double> origin;
    double mass;
    double halfDim;
    vector<int> *quadrant[4];
  
  public:
    node(const double& halfDim, const& vector<double> origin)
    {
      origin(origin);
      halfDim(halfDim);

      for (int i=0; i<4; i++)
      {
        quadrant[i] = NULL;
      }
    }

    ~node(void)
    {
      for (int i=0; i<4; i++)
      {
        delete quadrant[i];
      }

    }

    //prototype
    bool isleafnode(void);

    int get_quadrant(pos* point);

    void insert_particle(pos* point);
};

bool node::isleafnode(void)
{
  return quadrant[0]==NULL;
}

int node::get_quadrant(pos point)
{
  int quad = 0;
  if(point.x > origin.x && point.y < origin.y)
  {
    quad = 1;
  }
  else if(point.x < origin.x && point.y < origin.y)
  {
    quad = 2;
  }
  else if(point.x < origin.x && point.y > origin.y)
  {
    quad = 3;
  }

  return quad 
}

void node::insert_particle(pos point)
{
  if(isleafnode()) 
  {
    if(data == NULL)
    {
      data = point;
    }
    else
    {
      nodeCM * point_old = data;
      data = NULL;

      //make the quadrants
      double halfDim_new = halfDim/2;
      vector< double> origin_new;

      for(int i=0;i<4;i++)
      {
        origin_new[0] = origin[0] + halfDim/sqrt(2)*((i<2)?1:-1);
        origin_new[1] = origin[1] + halfDim/sqrt(2)*((i==0||i==4)?1:-1);
        quadrant[i] = new node(halfDim_new, origin_new);
      }
      
      get_quadrant(point_old)
      get_quadrant(old)

      insert_particle(point_old)
      insert_particle(point)
    }
  
  }
}
