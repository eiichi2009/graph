#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>

static uint64_t
xor128(int seed=0)
{
  static uint64_t x = 123456789;
  static uint64_t y = 362436069;
  static uint64_t z = 521288629;
  static uint64_t w = seed;
  uint64_t t = x^(x<<11);
  x = y;
  y = z;
  z = w;
  w = (w^(w>>19))^(t^(t>>8));
  return w;
}

class graph
{
  int V; // number of vertecies
  int E; // number of edges
  int P; // percentage of edge created between two vertecies
  int graph_type; // 0: use generate()   1: use genrate_random()
  std::vector<std::vector<std::pair<int,int>>> adjlist;
  void generate()
  {
    std::set<std::pair<int,int>> path;
    while ((int)path.size() < E) {
      int x = xor128() % V;
      int y = xor128() % V;
      if (x != y) {
	if (path.insert(std::make_pair(x,y)).second) {
	  int w = x > y ? x-y : y-x;
	  adjlist[x].push_back(std::make_pair(y,w));
	}	
      }
    }    
  }
  void generate_random()
  {
    int cnt = 0;
    for (int x = 0; x < V; ++x) {
      for (int y = 0; y < V; ++y) {
	if (x == y) {
	  continue;
	}
	if (P >= (int)(xor128() % 100)+1) {
	  int w = x > y ? x-y:y-x;
	  adjlist[x].push_back(std::make_pair(y,w));
	  cnt++;
	}
      }
    }
    E = cnt;
  }

public:
  graph(int _v, int _e, int _p, int _t) : V(_v), E(_e), P(_p), graph_type(_t)
  {
    if (E > V*(V-1)) {
      E = V*(V-1);
    }
    adjlist.resize(V);
    if (graph_type == 0) {
      generate();
    }
    else if (graph_type == 1) {
      generate_random();
    }
  }
  ~graph() {}
  void dot()
  {
    std::cout << "digraph graph {" << std::endl;
    std::cout << "overlap = false;" << std::endl;
    for (int x = 0; x < V; ++x) {
      for (int i = 0; i < (int)adjlist[x].size(); ++i) {
	int y = adjlist[x][i].first;
	int w = adjlist[x][i].second;
	std::cout << x << "->" << y << " [ len = " << w << " label = " << w << " ]" << std::endl;
      }
    }
    std::cout << "}" << std::endl;
  }
  int size() const
  {
    return V;
  }
  int edge_num() const
  {
    return E;
  }
};
