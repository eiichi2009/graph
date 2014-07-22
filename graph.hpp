#include <iostream>
#include <vector>
#include <cstdlib>
#include <set>
#include <cmath>

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
  double P; // probability of edge created between two vertecies. Needs to be between 0 and 1
  int graph_type; // 0: use generate()   1: use genrate_random()

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
    std::pair<int,int> pq = double2cf(P); // P=0.25   pq.first=1  pq.second=4
    int cnt = 0;
    for (int x = 0; x < V; ++x) {
      for (int y = 0; y < V; ++y) {
	if (x == y) {
	  continue;
	}
	if (pq.first >= (int)(xor128() % pq.second)+1) {
	  int w = x > y ? x-y:y-x;
	  adjlist[x].push_back(std::make_pair(y,w));
	  cnt++;
	}
      }
    }
    E = cnt;
  }

  std::pair<int,int> cf2double(const std::vector<int> &a)
  {
    int N = a.size();
    std::vector<int> p(N,0);
    std::vector<int> q(N,0);
    p[0] = a[0];
    p[1] = a[0] * a[1] + 1;
    q[0] = 1;
    q[1] = a[1];
    for (int i = 2; i < N; ++i) {
      p[i] = p[i-1]*a[i] +p[i-2];
      q[i] = q[i-1]*a[i] +q[i-2];
    }
    return std::pair<int,int>(p[N-1],q[N-1]);
  }

  std::pair<int,int> double2cf(double orig_d)
  {
    double d = orig_d;
    int N = 100;
    double EPS = 1e-5;
    std::vector<int> a(N,0);
    std::pair<int,int> ret((int)d,1);
    for (int i = 0; i < N; ++i) {
      a[i] = (int)d;
      if (d - a[i] < EPS) {
	ret = cf2double(a);
	break;
      }
      d = 1.0 / (d-a[i]);
      if (i > 2) {
	ret = cf2double(a);
	if (std::abs((double)ret.first/ret.second - orig_d) < EPS) {
	  break;
	}
      }
    }
    return ret;
  }

public:
  typedef std::vector<std::vector<std::pair<int,int>>> vec;
  std::vector<std::vector<std::pair<int,int>>> adjlist;
  graph(int _v, int _e, double _p, int _t) : V(_v), E(_e), P(_p), graph_type(_t)
  {
    xor128(time(0));
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
  graph(graph::vec &v) : adjlist(v)
  {
    V = adjlist.size();
    int cnt = 0;
    for (int p = 0; p < V; ++p) {
      for (int i = 0; i < adjlist[p].size(); ++i) {
	cnt++;
      }
    }
    E = cnt;
    graph_type = 0;
    P = 0.0;    
  }
  ~graph() {}
  void dot()
  {
    std::cout << "digraph xxx {" << std::endl;
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

  const std::vector<std::pair<int,int>> &operator[](int idx) const
  {
    return adjlist[idx];
  }

  std::vector<std::pair<int,int>> &operator[](int idx)
  {
    return adjlist[idx];
  }
};
