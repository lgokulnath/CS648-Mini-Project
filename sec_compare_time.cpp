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

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef K::Segment_2 Segment_2;
typedef K::Circle_2 Circle_2;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Bounded_side Bounded_side;

Circle_2 base_case(const std::vector<Point_2> &points)
{
    assert(points.size() <= 3);
    if (points.empty())
    {
        return Circle_2(Point_2(0, 0), 0);
    }
    else if (points.size() == 1)
    {
        return Circle_2(points[0], 0);
    }
    else if (points.size() == 2)
    {
        return Circle_2(Point_2((points[0].x() + points[1].x()) / 2, (points[0].y() + points[1].y()) / 2), CGAL::squared_distance(points[0], points[1]) / 4);
    }
    std::vector<Point_2>::const_iterator it = points.begin();
    Min_circle mc(it, it + 3, true);
    Traits::Circle c = mc.circle();
    return Circle_2(c.center(), c.squared_radius());
}


Circle_2 recursive_1(std::vector<Point_2> &points,
                    std::vector<Point_2> sec_points, int n)
{
    if (n == 0 || sec_points.size() == 3)
    {
        return base_case(sec_points);
    }
    int idx = rand() % n;
    Point_2 p = points[idx];
    std::swap(points[idx], points[n - 1]);
    Circle_2 smaller_sec = recursive_1(points, sec_points, n - 1);
    if (CGAL::squared_distance(smaller_sec.center() , p) <= smaller_sec.squared_radius())
    {
        return smaller_sec;
    }
    sec_points.push_back(p);
    return recursive_1(points, sec_points, n - 1);
}

Circle_2 recursive(std::vector<Point_2> &points)
{
    // std::vector<Point_2> P_copy = points;
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(points), std::end(points), rng);
    return recursive_1(points, {}, points.size());
}






Circle_2 iterative_2(Point_2 p1, Point_2 p2, std::vector<Point_2> &points, int n)
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

Circle_2 iterative_1(std::vector<Point_2> &points, int n, Point_2 q)
{
  // std::vector<Point_2> points_ (points.begin(), points.begin() + n);

  // auto rng = std::default_random_engine {};
  // std::shuffle(std::begin(points_), std::end(points_), rng);

 Circle_2 c(q, points[0]);

  for(int i = 1; i < n; i++) {
    
    Point_2 p = points[i];
    Bounded_side s = c.bounded_side(p);
    if(! (s == CGAL::ON_BOUNDED_SIDE || s == CGAL::ON_BOUNDARY)) {
      c = iterative_2(p, q, points, i);
    }

  }
  return c;

}

Circle_2 iterative(std::vector<Point_2> &points) 
{
  if(points.size() == 0) {
    return Circle_2(Point_2(0, 0), 0);
  } 
  if(points.size() == 1) {
    return Circle_2(points[0], 0);
  }

  // if(points.size()<=3){
  //   return base_case(points);
  // }

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
      c = iterative_1(points, i, p);
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

        std::cout << "N: " << n_points << ", n_samples: " << n_samples << std::endl;
        // return 0;
    }

    int n;

    if(argc==1)
    {
        std::cout << "Enter the number of points: ";
        std::cin >> n;
    }
    else
    {
        n = n_points;
    }

    std::vector<Point_2> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1000.0, 1000.0);

    for (int i = 0; i < n; ++i)
    {
        points.emplace_back(dis(gen), dis(gen));
    }
    std::vector<Point_2> points1 = points;
    std::vector<Point_2> points2 = points;

    auto start = std::chrono::steady_clock::now();
    Circle_2 mec_recursive_sec = recursive(points1);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> recursive_sec_time = end - start;


    start = std::chrono::steady_clock::now();
    Circle_2 mec_sec = iterative(points2);
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> sec_time = end - start;

    start = std::chrono::steady_clock::now();
    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle mc_circle = mc.circle();
    Circle_2 mec_min_circle = Circle_2(mc_circle.center(),mc_circle.squared_radius());
    end = std::chrono::steady_clock::now();
    std::chrono::duration<double> min_circle_time = end - start;

    std::cout << "recursive_sec Algorithm: Center = { " << mec_recursive_sec.center()
              << " } Radius = " << std::sqrt(CGAL::to_double(mec_recursive_sec.squared_radius())) << " Time = " << recursive_sec_time.count() << " seconds" << std::endl;

    std::cout << "Sec Algorithm: Center = { " << mec_sec.center()
              << " } Radius = " << std::sqrt(CGAL::to_double(mec_sec.squared_radius())) << " Time = " << sec_time.count() << " seconds" << std::endl;

    std::cout << "Min Circle Algorithm: Center = { " << mec_min_circle.center()
              << " } Radius = " << std::sqrt(CGAL::to_double(mec_min_circle.squared_radius())) << " Time = " << min_circle_time.count() << " seconds" << std::endl;

    if (argc == 3)
    {
        std::ofstream fout("data_report_tables/data_table_1.csv", std::ios::app);
        for (int i = 0; i < n_samples; i++)
        {

            std::vector<Point_2> points;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-1000.0, 1000.0);

            for (int i = 0; i < n; ++i)
            {
                points.emplace_back(dis(gen), dis(gen));
            }

            auto start = std::chrono::steady_clock::now();
            Circle_2 mec_recursive_sec = recursive(points);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> recursive_sec_time = end - start;
            fout << n << "," << recursive_sec_time.count() << std::endl;
        }
        fout.close();
    }

    return 0;
}