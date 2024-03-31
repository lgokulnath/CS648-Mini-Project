#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::Segment_2 Segment_2;
typedef Kernel::Circle_2 Circle_2;
typedef Kernel::Bounded_side Bounded_side;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;


struct Circle
{
    Point C;
    double R;
};

Circle convert_to_circle(const Point &center, double radius)
{
    Circle c;
    c.C = center;
    c.R = radius;
    return c;
}

Circle min_circle_trivial(const std::vector<Point> &P)
{
    assert(P.size() <= 3);
    if (P.empty())
    {
        return convert_to_circle(Point(0, 0), 0);
    }
    else if (P.size() == 1)
    {
        return convert_to_circle(P[0], 0);
    }
    else if (P.size() == 2)
    {
        return convert_to_circle(Point((P[0].x() + P[1].x()) / 2, (P[0].y() + P[1].y()) / 2), std::sqrt(CGAL::squared_distance(P[0], P[1])) / 2);
    }
    std::vector<Point>::const_iterator it = P.begin();
    Min_circle mc(it, it + 3, true);
    Traits::Circle c = mc.circle();
    return convert_to_circle(c.center(), std::sqrt(c.squared_radius()));
}

Circle welzl_helper(std::vector<Point> &P,
                    std::vector<Point> R, int n)
{
    if (n == 0 || R.size() == 3)
    {
        return min_circle_trivial(R);
    }
    int idx = rand() % n;
    Point p = P[idx];
    std::swap(P[idx], P[n - 1]);
    Circle d = welzl_helper(P, R, n - 1);
    if (CGAL::squared_distance(d.C, p) <= d.R * d.R)
    {
        return d;
    }
    R.push_back(p);
    return welzl_helper(P, R, n - 1);
}

Circle welzl(const std::vector<Point> &P)
{
    std::vector<Point> P_copy = P;
    std::random_shuffle(P_copy.begin(), P_copy.end());
    return welzl_helper(P_copy, {}, P_copy.size());
}


bool isAcuteAngleTriangle(const Point& p1, const Point& p2, const Point& p3) {
    K::Vector_2 v1(p1, p2);
    K::Vector_2 v2(p1, p3);
    K::Vector_2 v3(p2, p3);

    K::FT dot1 = v1 * v2;
    K::FT dot2 = v1 * v3;
    K::FT dot3 = v2 * v3;

    return dot1 > 0 && dot2 < 0 && dot3 > 0;
}

// Algorithm 1: Find the circle with maximum radius among pairs of points and circumcircles of acute triangles
std::pair<Point, double> algorithm1(const std::vector<Point>& points) {
    double max_radius_squared = 0.0;
    Point center;

    // Find circle with maximum radius among pairs of points
    for (auto it1 = points.begin(); it1 != points.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != points.end(); ++it2) {
            Point midpoint((it1->x() + it2->x()) / 2, (it1->y() + it2->y()) / 2);
            double radius_squared = CGAL::squared_distance(*it1, *it2) / 4;
            if (radius_squared > max_radius_squared) {
                max_radius_squared = radius_squared;
                center = midpoint;
            }
        }
    }

    // Find circumcircle of each triplet of points with all acute angles
    for (auto it1 = points.begin(); it1 != points.end(); ++it1) {
        for (auto it2 = std::next(it1); it2 != points.end(); ++it2) {
            for (auto it3 = std::next(it2); it3 != points.end(); ++it3) {
                if (isAcuteAngleTriangle(*it1, *it2, *it3)) {
                    Point circumcenter = CGAL::circumcenter(*it1, *it2, *it3);
                    double radius_squared = CGAL::squared_distance(*it1, circumcenter);
                    if (radius_squared > max_radius_squared) {
                        max_radius_squared = radius_squared;
                        center = circumcenter;
                    }
                }
            }
        }
    }

    double max_radius = std::sqrt(max_radius_squared);
    return std::make_pair(center, max_radius);
}

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

Circle_2 sec(std::vector<Point_2> &points) 
{
  if(points.size() == 0) {
    return Circle_2(Point_2(0, 0), 0);
  } 
  if(points.size() == 1) {
    return Circle_2(points[0], 0);
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

int main(int argc, char *argv[])
{

    int n_points, n_samples;
    if (argc == 3)
    {
        n_points = std::stoi(argv[1]);
        n_samples = std::stoi(argv[2]);

        // std::cout << "N: " << n_points << ", n_samples: " << n_samples << std::endl;
        // return 0;
    }
    else
    {
        std::cout<<"Usage: <executable> n_points n_samples\n";
    }

    int n = n_points;

    if (argc == 3)
    {
        std::ofstream fout("../data_report_tables/data_table_sec.csv", std::ios::app);
        for (int i = 0; i < n_samples; i++)
        {
            std::vector<Point> points;
            std::vector<Point_2> sec_points;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-1000.0, 1000.0);

            for (int i = 0; i < n; ++i)
            {
                auto x = dis(gen), y = dis(gen);
                points.emplace_back(x, y);
                sec_points.push_back(Point_2(x, y));
            }

            auto start = std::chrono::steady_clock::now();
            Circle mec_welzl = welzl(points);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> welzl_time = end - start;

            start = std::chrono::steady_clock::now();
            Min_circle mc(points.begin(), points.end(), true);
            Traits::Circle mc_circle = mc.circle();
            Circle mec_min_circle = convert_to_circle(mc_circle.center(), std::sqrt(mc_circle.squared_radius()));
            end = std::chrono::steady_clock::now();
            std::chrono::duration<double> min_circle_time = end - start;

            start = std::chrono::steady_clock::now();
            auto mec_deterministic = algorithm1(points);
            end = std::chrono::steady_clock::now();
            std::chrono::duration<double> deterministic_time = end - start;

            start = std::chrono::steady_clock::now();
            Circle_2 _sec = sec(sec_points);
            end = std::chrono::steady_clock::now();
            std::chrono::duration<double> sec_time = end - start;

            fout<<n<<","<<sec_time.count()<<","<<welzl_time.count()<<","<<min_circle_time.count()<<","<<deterministic_time.count()<<std::endl;
            // fout<<n<<","<<sec_time.count()<<","<<min_circle_time.count()<<std::endl;
        }
        fout.close();
    }


    return 0;
}
