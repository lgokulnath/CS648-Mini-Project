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
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

struct Circle
{
    Point_2 C;
    double R;
};

Circle convert_to_circle(const Point_2 &center, double radius)
{
    Circle c;
    c.C = center;
    c.R = radius;
    return c;
}

Circle min_circle_trivial(const std::vector<Point_2> &P)
{
    assert(P.size() <= 3);
    if (P.empty())
    {
        return convert_to_circle(Point_2(0, 0), 0);
    }
    else if (P.size() == 1)
    {
        return convert_to_circle(P[0], 0);
    }
    else if (P.size() == 2)
    {
        return convert_to_circle(Point_2((P[0].x() + P[1].x()) / 2, (P[0].y() + P[1].y()) / 2), std::sqrt(CGAL::squared_distance(P[0], P[1])) / 2);
    }
    std::vector<Point_2>::const_iterator it = P.begin();
    Min_circle mc(it, it + 3, true);
    Traits::Circle c = mc.circle();
    return convert_to_circle(c.center(), std::sqrt(c.squared_radius()));
}

Circle welzl_helper(std::vector<Point_2> &P,
                    std::vector<Point_2> R, int n)
{
    if (n == 0 || R.size() == 3)
    {
        return min_circle_trivial(R);
    }
    int idx = rand() % n;
    Point_2 p = P[idx];
    std::swap(P[idx], P[n - 1]);
    Circle d = welzl_helper(P, R, n - 1);
    if (CGAL::squared_distance(d.C, p) <= d.R * d.R)
    {
        return d;
    }
    R.push_back(p);
    return welzl_helper(P, R, n - 1);
}

Circle welzl(const std::vector<Point_2> &P)
{
    std::vector<Point_2> P_copy = P;
    std::random_shuffle(P_copy.begin(), P_copy.end());
    return welzl_helper(P_copy, {}, P_copy.size());
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

    std::cout << "Welzl Algorithm: Center = { " << mec_welzl.C.x() << ", " << mec_welzl.C.y()
              << " } Radius = " << mec_welzl.R << " Time = " << welzl_time.count() << " seconds" << std::endl;

    std::cout << "Min Circle Algorithm: Center = { " << mec_min_circle.C.x() << ", " << mec_min_circle.C.y()
              << " } Radius = " << mec_min_circle.R << " Time = " << min_circle_time.count() << " seconds" << std::endl;

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
            Circle mec_welzl = welzl(points);
            auto end = std::chrono::steady_clock::now();
            std::chrono::duration<double> welzl_time = end - start;
            fout << n << "," << welzl_time.count() << std::endl;
        }
        fout.close();
    }

    return 0;
}
