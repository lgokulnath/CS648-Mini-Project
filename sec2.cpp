#include <bits/stdc++.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Circle_2 Circle_2;
typedef Kernel::Bounded_side Bounded_side;

Circle_2 sec2(Point_2 p1, Point_2 p2, std::vector<Point_2> &points, int n)
{ 
  Circle_2 c(p1, p2);
  for(int i = 0; i < n; i++) {
    Point_2 p = points[i];
    Bounded_side s = c.bounded_side(p);
    if (! (s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY)) {
      // this point has to be on the circle
      c = Circle_2(p1, p2, p);
    }
  }
  return c;
  
}

Circle_2 sec1(std::vector<Point_2> &points, int n, Point_2 q)
{
  std::vector<Point_2> points_ (points.begin(), points.begin() + n);

  auto rng = std::default_random_engine {};
  std::shuffle(std::begin(points_), std::end(points_), rng);

  Circle_2 c(q, points_[0]);

  for(int i = 1; i < n; i++) {
    
    Point_2 p = points_[i];
    Bounded_side s = c.bounded_side(p);
    if(! (s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY)) {
      c = sec2(p, q, points_, i);
    }

  }
  return c;

}

Circle_2 sec(std::vector<Point_2> &points) 
{
  if(points.size() == 0) {
    return Circle_2(Point_2(0, 0), Point_2(1, 0));
  } 
  if(points.size() == 1) {
    return Circle_2(points[0], Point_2(0, 0));
  }
  // randomly shuffle the vector of points
  // ref: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
  
  auto rng = std::default_random_engine {};
  std::shuffle(std::begin(points), std::end(points), rng);


  Point_2 p1 = points[0];
  Point_2 p2 = points[1];
  Circle_2 c(p1, p2);
  for(int i = 2; i < points.size(); i++) {
    Point_2 p = points[i];
    Bounded_side s = c.bounded_side(p);
    if (! (s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY)) {
      c = sec1(points, i, p);
    }
  }
  return c;
}






int main()
{
  Point_2 p1(0, 0);
  Point_2 p2(4, 0);
  std::vector<Point_2> points = {Point_2(2, 0), Point_2(2, 2), Point_2(2, 10), p1, p2};
  Circle_2 c = sec(points);
  double radius = std::sqrt(CGAL::to_double(c.squared_radius()));
  std::cout << "SEC center = " << c.center() << "\n";
  std::cout << "SEC radius = " << radius << "\n";
}
