#ifndef H_EDGE
#define H_EDGE

#include "vector2.h"

template <class T>
class Edge
{
 public:
  using VertexType = Vector2<T>;
  
  Edge(const VertexType& _p1, const VertexType& _p2)
    : p1(_p1),
      p2(_p2),
      isBad(false),
      weight(_p1.dist(_p2)) {
  };

  Edge(const Edge& e)
    : p1(e.p1),
      p2(e.p2),
      isBad(false),
      weight(e.weight) {
  };
  
  VertexType p1;
  VertexType p2;

  double weight;
  
  bool isBad;
};

template <class T>
inline std::ostream& operator<<(std::ostream &str, Edge<T> const& e) {
	return str << "Edge " << e.p1 << ", " << e.p2;
}

template <class T>
inline bool
operator==(const Edge<T>& e1, const Edge<T>& e2) {
	return (e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
         (e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

template <class T>
inline bool
almost_equal (const Edge<T>& e1, const Edge<T>& e2) {
	return (almost_equal(e1.p1, e2.p1) && almost_equal(e1.p2, e2.p2)) ||
         (almost_equal(e1.p1, e2.p2) && almost_equal(e1.p2, e2.p1));
}

#endif

