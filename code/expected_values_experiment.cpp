#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <fstream>
#include <numeric> // for std::accumulate
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/point_generators_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

// Function to calculate the expected value for a given number of points
double calculate_expected_value(int n, int repetitions, std::ofstream& outfile) {
    double total_outside_count1 = 0.0;
    double total_outside_count2 = 0.0;

    for (int i = 0; i < repetitions; ++i) {
        std::vector<Point> points;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-1000.0, 1000.0);

        for (int j = 0; j < n; ++j) {
            points.emplace_back(dis(gen), dis(gen));
        }

        std::random_shuffle(points.begin(), points.end());
        std::vector<Point> half_points(points.begin(), points.begin() + n/2);

        Min_circle mc(half_points.begin(), half_points.end(), true);
        Traits::Circle circle = mc.circle();

        int outside_count = 0;
        for (const Point& p : points) {
            K::FT dist_squared = CGAL::squared_distance(p, circle.center());
            if (dist_squared > circle.squared_radius()) {
                outside_count++;
            }
        }

        total_outside_count1 += outside_count;

        std::random_shuffle(points.begin(), points.end());
        std::vector<Point> sqrt_points(points.begin(), points.begin() + std::sqrt(n));

        Min_circle mc_sqrt(sqrt_points.begin(), sqrt_points.end(), true);
        Traits::Circle circle_sqrt = mc_sqrt.circle();

        outside_count = 0;
        for (const Point& p : points) {
            K::FT dist_squared = CGAL::squared_distance(p, circle_sqrt.center());
            if (dist_squared > circle_sqrt.squared_radius()) {
                outside_count++;
            }
        }

        total_outside_count2 += outside_count;
    }

    outfile << n << "," << total_outside_count1/repetitions << "," << total_outside_count2/repetitions << std::endl;

    return 0;
}

int main(){
    int repetitions = 1000;
    std::ofstream outfile("../data/expected_values_experiment.csv");
    if (!outfile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    outfile << "n,Expected_n/2,Expected_sqrt(n)" << std::endl;

    for (int n = 4; n <= 1000; ++n) {
        calculate_expected_value(n, repetitions,outfile);
    }

    outfile.close();

    return 0;
}
