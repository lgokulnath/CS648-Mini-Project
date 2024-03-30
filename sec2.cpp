#include <bits/stdc++.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Circle_2.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Circle_2 Circle_2;
typedef Kernel::Bounded_side Bounded_side;

# define PI           3.14159265358979323846  /* pi */

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
  //std::vector<Point_2> points_ (points.begin(), points.begin() + n);

  //auto rng = std::default_random_engine {};
  //std::shuffle(std::begin(points_), std::end(points_), rng);

  Circle_2 c(q, points[0]);

  for(int i = 1; i < n; i++) {
    
    Point_2 p = points[i];
    Bounded_side s = c.bounded_side(p);
    if(! (s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY)) {
      c = sec2(p, q, points, i);
    }

  }
  return c;

}

Circle_2 sec(std::vector<Point_2> &points, bool shuffle=true) 
{
  if(points.size() == 0) {
    return Circle_2(Point_2(0, 0), Point_2(1, 0));
  } 
  if(points.size() == 1) {
    return Circle_2(points[0], Point_2(0, 0));
  }
  // randomly shuffle the vector of points
  // ref: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
  if(shuffle) {
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(points), std::end(points), rng);
  }


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


std::vector<Point_2> generate_spiral(int n, double max_radius=1000) {
    double theta = PI * 2 / ((double)(n));
    std::vector<Point_2> ps;
    for(int i = 0; i < n; i++) {
      double r = max_radius * (double(i)) / n;
      double x = r*std::cos(theta*i);
      double y = r*std::sin(theta*i);
      Point_2 p(x, y);
      ps.push_back(p);
    }
    return ps;

}






int main(int argc, char* argv[])
{
  // Point_2 p1(0, 0);
  // Point_2 p2(4, 0);
  // std::vector<Point_2> points = {Point_2(2, 0), Point_2(2, 2), Point_2(2, 10), p1, p2};
  // Circle_2 c = sec(points);
  // double radius = std::sqrt(CGAL::to_double(c.squared_radius()));
  // std::cout << "SEC center = " << c.center() << "\n";
  // std::cout << "SEC radius = " << radius << "\n";
  int n;
  std::cout << "Enter number of points: ";
  std::cin >> n;
  double max_radius = 1000;
  std::vector<Point_2> ps = generate_spiral(n, max_radius);

  auto start = std::chrono::steady_clock::now();
  Circle_2 mec_iterative = sec(ps, false);
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> sec_time = end - start;

  start = std::chrono::steady_clock::now();
  Circle_2 mec_iterative1 = sec(ps, true);
  end = std::chrono::steady_clock::now();
  std::chrono::duration<double> sec_time1 = end - start;

  std::cout << "Time taken without shuffling = " << sec_time.count() << "\n";
  std::cout << "Time taken after shuffling = " << sec_time1.count() << "\n";
    
}
