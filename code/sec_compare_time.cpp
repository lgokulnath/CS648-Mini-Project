/*

Compare runtimes of all the algorithms, optimized versions use "Move to Start" heuristic

*/


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



#define NUM_RUNS 100


std::list<Point_2> recursive_points;
std::list<Point_2>::reverse_iterator itr;


void shuffle_list(std::list<Point_2>& l) {
  std::vector<std::reference_wrapper<const Point_2>> v(l.begin(), l.end());
    auto rng = std::default_random_engine {};
    std::shuffle(v.begin(), v.end(), rng);
    std::list<Point_2> shuffled;
    for (auto &ref : v) shuffled.push_back(std::move(ref.get()));
    l.swap(shuffled);
    //return shuffled;
}

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
    int idx = n-1;
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

Circle_2 recursive(std::vector<Point_2> &points, bool shuffle=true)
{
    std::vector<Point_2> P_copy = points;
    if(shuffle) {
      auto rng = std::default_random_engine {};
      std::shuffle(std::begin(points), std::end(points), rng);
    }
    return recursive_1(points, {}, points.size());
}

Circle_2 recursive_helper_list(std::list<Point_2> &P,
                    std::vector<Point_2> R, int n, std::list<Point_2>::reverse_iterator it)
{
    if (n== 0 || it == P.rend() || R.size() == 3)
    {
        return base_case(R);
    }
   
    Point_2 p = *it;
    std::list<Point_2>::reverse_iterator itr_ = it;
    it++;
    //Point_2 p1 = P.
    Circle_2 d = recursive_helper_list(P, R, n - 1, it);
    if (CGAL::squared_distance(d.center() , p) <= d.squared_radius())
    {
        return d;
        
    }
    R.push_back(p);
    d = recursive_helper_list(P, R, n - 1, it);
    P.push_front(p);
    P.erase(std::next(itr_).base() );
    
    return d;
}

Circle_2 recursive_list(std::list<Point_2> &P, bool shuffle=true)
{
    if(shuffle) {
      shuffle_list(P);
    }
    itr = P.rbegin();
    return recursive_helper_list(P, {}, P.size(), itr);
}


Circle_2 iterative_2_opt(Point_2 p1, Point_2 p2, std::list<Point_2> &points, int n)
{ 
  Circle_2 c(p1, p2);
  std::list<Point_2>::iterator it;
  it = points.begin();
  for(int i = 0; i < n; i++, it++ ){
    Point_2 p = *it;
    //Bounded_side s = c.bounded_side(p);
    if (CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
      // this point has to be on the circle
      c = Circle_2(p1, p2, p);
    }
  }
  return c;
  
}

Circle_2 iterative_1_opt(std::list<Point_2> &points, int n, Point_2 q)
{
  //std::vector<Point_2> points_ (points.begin(), points.begin() + n);

  //auto rng = std::default_random_engine {};
  //std::shuffle(std::begin(points_), std::end(points_), rng);

  Circle_2 c(q, points.front());
  std::list<Point_2>::iterator it;
  it = points.begin();
  it++;

  for(int i = 1; i < n; i++) {
    
    Point_2 p = *it;
    auto it1 = it;
    //Bounded_side s = c.bounded_side(p);
    if(CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
      c = iterative_2_opt(p, q, points, i);
      it++;

      //move to front
      points.push_front(p);
      points.erase(it1 );
      // // *it = (points.front());
      // *(points.begin()) = p;
      continue;
    }
    it++;

  }
  return c;

}

Circle_2 iterative_opt(std::list<Point_2> &points, bool shuffle=true) 
{
  if(points.size() <= 3) {
    std::vector<Point_2> v{ std::begin(points), std::end(points) };
    return base_case(v);
  }
  // randomly shuffle the vector of points
  // ref: https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector

  if(shuffle) {
    shuffle_list(points);
  }
  
//   auto rng = std::default_random_engine {};
//   std::shuffle(std::begin(points), std::end(points), rng);
   std::list<Point_2>::iterator it;
   it = points.begin();
   it++;



  Point_2 p1 = points.front();
  Point_2 p2 = *it;
  
  Circle_2 c(p1, p2); it++;
  for(int i=2; it != points.end() ;  i++) {
    Point_2 p = *it;
    auto it1 = it;
   // Bounded_side s = c.bounded_side(p);
    if (CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
      c = iterative_1_opt(points, i, p);
      it++;
      // move this point to front of list
      points.push_front(p);
      points.erase(it1 );
      continue;
    //   *it = p1;

    //   *(points.begin()) = p;

    }
    it++;
  }
  return c;
}







Circle_2 iterative_2(Point_2 p1, Point_2 p2, std::vector<Point_2> &points, int n)
{ 
  Circle_2 c(p1, p2);
  for(int i = 0; i < n; i++) {
    Point_2 p = points[i];
    //Bounded_side s = c.bounded_side(p);
    if (CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
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
    //Bounded_side s = c.bounded_side(p);
    if(CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
      c = iterative_2(p, q, points, i);
    }

  }
  return c;

}

Circle_2 iterative(std::vector<Point_2> &points, bool shuffle=true) 
{
  // if(points.size() == 0) {
  //   return Circle_2(Point_2(0, 0), 0);
  // } 
  // if(points.size() == 1) {
  //   return Circle_2(points[0], 0);
  // }

  if(points.size()<=3){
    return base_case(points);
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
    //Bounded_side s = c.bounded_side(p);
    if (CGAL::squared_distance(c.center() , p) > c.squared_radius()) {
      c = iterative_1(points, i, p);
    }
  }
  return c;
}


















int main(int argc, char *argv[])
{

    int n_points, n_samples;


    if(argc > 1) {
      n_points = std::stoi(argv[1]);
    }
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
        //std::cout << "Num points = " << n << "\n";
    }
    double t1=0, t2=0, t3=0, t4=0, t5 = 0;
    for(int ii = 0; ii < NUM_RUNS; ii++) {
      std::vector<Point_2> points;
      std::list<Point_2> list_points;
      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_real_distribution<> dis(-1000.0, 1000.0);

      for (int i = 0; i < n; ++i)
      {
          points.emplace_back(dis(gen), dis(gen));
      }
      auto rng = std::default_random_engine {};
      std::shuffle(std::begin(points), std::end(points), rng);

      std::vector<Point_2> points1 = points;
      std::vector<Point_2> points2 = points;
      recursive_points.clear();
      std::copy( points.begin(), points.end(), std::back_inserter( recursive_points) );
      std::copy( points.begin(), points.end(), std::back_inserter( list_points) );

      auto start = std::chrono::steady_clock::now();
      Circle_2 mec_recursive = recursive(points1);
      auto end = std::chrono::steady_clock::now();
      std::chrono::duration<double> recursive_sec_time = end - start;

      start = std::chrono::steady_clock::now();
      Circle_2 mec_recursive_list = recursive_list(recursive_points);
      end = std::chrono::steady_clock::now();
      std::chrono::duration<double> recursive_sec_list_time = end - start;


      start = std::chrono::steady_clock::now();
      Circle_2 mec_iterative = iterative(points2);
      end = std::chrono::steady_clock::now();
      std::chrono::duration<double> sec_time = end - start;

      start = std::chrono::steady_clock::now();
      Circle_2 mec_iterative_opt = iterative_opt(list_points);
      end = std::chrono::steady_clock::now();
      std::chrono::duration<double> sec_time_1 = end - start;

      start = std::chrono::steady_clock::now();
      Min_circle mc(points.begin(), points.end(), true);
      Traits::Circle mc_circle = mc.circle();
      Circle_2 mec_min_circle = Circle_2(mc_circle.center(),mc_circle.squared_radius());
      end = std::chrono::steady_clock::now();
      std::chrono::duration<double> min_circle_time = end - start;

      t1 += recursive_sec_time.count();
      t2 += recursive_sec_list_time.count();
      t3 += sec_time.count();
      t4 += min_circle_time.count();
      t5 += sec_time_1.count();

      if(ii % 50 == 0 && argc < 2) {
        std::cout << "Test case #" << ii << "\n";
      std::cout << "Recursive Algorithm: Center = {" << mec_recursive.center()
                << "} Radius = " << std::sqrt(CGAL::to_double(mec_recursive.squared_radius())) << " Time = " << recursive_sec_time.count() << " seconds" << std::endl;

                std::cout << "Recursive optimized Algorithm: Center = {" << mec_recursive_list.center()
                << "} Radius = " << std::sqrt(CGAL::to_double(mec_recursive_list.squared_radius())) << " Time = " << recursive_sec_list_time.count() << " seconds" << std::endl;

      std::cout << "Iterative Algorithm: Center = {" << mec_iterative.center()
                << "} Radius = " << std::sqrt(CGAL::to_double(mec_iterative.squared_radius())) << " Time = " << sec_time.count() << " seconds" << std::endl;

      std::cout << "Iterative optimized Algorithm: Center = {" << mec_iterative_opt.center()
                << "} Radius = " << std::sqrt(CGAL::to_double(mec_iterative_opt.squared_radius())) << " Time = " << sec_time_1.count() << " seconds" << std::endl;

      std::cout << " In-Built Algorithm: Center = {" << mec_min_circle.center()
                << "} Radius = " << std::sqrt(CGAL::to_double(mec_min_circle.squared_radius())) << " Time = " << min_circle_time.count() << " seconds" << std::endl;
      std::cout << "---------------------------------------------------------------------------------\n";
      }
    }
    //   if (argc == 3)
    //   {
    //       std::ofstream fout("data_report_tables/data_table_1.csv", std::ios::app);
    //       for (int i = 0; i < n_samples; i++)
    //       {

    //           std::vector<Point_2> points;
    //           std::random_device rd;
    //           std::mt19937 gen(rd());
    //           std::uniform_real_distribution<> dis(-1000.0, 1000.0);

    //           for (int i = 0; i < n; ++i)
    //           {
    //               points.emplace_back(dis(gen), dis(gen));
    //           }

    //           auto start = std::chrono::steady_clock::now();
    //           Circle_2 mec_recursive = recursive(points);
    //           auto end = std::chrono::steady_clock::now();
    //           std::chrono::duration<double> recursive_sec_time = end - start;
    //           fout << n << "," << recursive_sec_time.count() << std::endl;
    //       }
    //       fout.close();
    //   }
    // }

    t1 /= NUM_RUNS;
    t2 /= NUM_RUNS;
    t3 /= NUM_RUNS;
    t4 /= NUM_RUNS;
    t5 /= NUM_RUNS;
    if(argc < 2) {
      std::cout << "Number of points = " << n << "\n";
      std::cout << "Recursive algo time = " << t1 << "\n";
      std::cout << "Recursive optimized algo time = " << t2 << "\n";
      std::cout << "Iterative algo time = " << t3 << "\n";
      std::cout << "Iterative optimized algo time = " << t5 << "\n";
      std::cout << "CGAL algo time = " << t4 << "\n";
    }
    else if(argc == 2) {
      std::cout << n << "," << t3 << "," << t5 << "," << t4 << "\n";
    }
    

    return 0;
}