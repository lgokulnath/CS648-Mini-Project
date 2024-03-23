#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/point_generators_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

int main() {
    int n;
    std::cout << "Enter the number of points: ";
    std::cin >> n;

    // Generate n random points in 2D
    // std::vector<Point> points;
    // CGAL::Random_points_in_square_2<Point> gen(1000);
    // for (int i = 0; i < n; ++i) {
    //     points.push_back(*gen++);
    // }

    std::vector<Point> points;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1000.0, 1000.0);

    for (int i = 0; i < n; ++i) {
        points.emplace_back(dis(gen), dis(gen));
    }


    // Randomly choose n/2 points
    std::random_shuffle(points.begin(), points.end());
    std::vector<Point> half_points(points.begin(), points.begin() + n/2);

    // Form a minimum enclosing circle of n/2 points
    Min_circle mc(half_points.begin(), half_points.end(), true);
    Traits::Circle circle = mc.circle();

    // Count points outside the minimum enclosing circle (strictly outside)
    int outside_count = 0;
    for (const Point& p : points) {
        K::FT dist_squared = CGAL::squared_distance(p, circle.center());
        if (dist_squared > circle.squared_radius()) {
            outside_count++;
        }
    }
    std::cout << "Number of points strictly outside the minimum enclosing circle with n/2 points: " << outside_count << std::endl;

    // Randomly choose sqrt(n) points
    int sqrt_n = std::sqrt(n);
    std::random_shuffle(points.begin(), points.end());
    std::vector<Point> sqrt_points(points.begin(), points.begin() + sqrt_n);

    // Form a minimum enclosing circle of sqrt(n) points
    Min_circle mc_sqrt(sqrt_points.begin(), sqrt_points.end(), true);
    Traits::Circle circle_sqrt = mc_sqrt.circle();

    // Count points outside the minimum enclosing circle with sqrt(n) points (strictly outside)
    outside_count = 0;
    for (const Point& p : points) {
        K::FT dist_squared = CGAL::squared_distance(p, circle_sqrt.center());
        if (dist_squared > circle_sqrt.squared_radius()) {
            outside_count++;
        }
    }
    std::cout << "Number of points strictly outside the minimum enclosing circle with sqrt(n) points: " << outside_count << std::endl;

    return 0;
}
