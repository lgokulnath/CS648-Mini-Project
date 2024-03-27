#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/point_generators_2.h>
#include <chrono>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;

// Function to check if a triangle formed by three points is acute
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

// Algorithm 2: Find the minimum enclosing circle using CGAL's inbuilt function
std::pair<Point, double> algorithm2(const std::vector<Point>& points) {
    Min_circle mc(points.begin(), points.end(), true);
    Traits::Circle circle = mc.circle();
    double min_radius = std::sqrt(CGAL::to_double(circle.squared_radius()));
    return std::make_pair(circle.center(), min_radius);
}

int compare_time(int n, std::ofstream& outfile){

    std::vector<Point> points;
    CGAL::Random_points_in_square_2<Point> gen(1000);
    for (int i = 0; i < n; ++i) {
        points.push_back(*gen++);
    }

    auto start1 = std::chrono::high_resolution_clock::now();
    
    // Algorithm 1: Find circumcircle with maximum radius among pairs of points and acute triangles
    auto result1 = algorithm1(points);
    // std::cout << "Algorithm 1: Circumcircle with maximum radius among pairs and acute triangles:\n";
    // std::cout << "Center: " << result1.first << ", Radius: " << result1.second << std::endl;

    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;

    auto start2 = std::chrono::high_resolution_clock::now();
    
    // Algorithm 2: Find minimum enclosing circle using CGAL's inbuilt function
    auto result2 = algorithm2(points);
    // std::cout << "Algorithm 2: Minimum enclosing circle:\n";
    // std::cout << "Center: " << result2.first << ", Radius: " << result2.second << std::endl;
    
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;


    // std::cout << "Algorithm 1 runtime: " << duration1.count() << " seconds" << std::endl;
    // std::cout << "Algorithm 2 runtime: " << duration2.count() << " seconds" << std::endl;


    outfile << n << "," << duration1.count() << "," << duration2.count() << std::endl;

    return 0;

}

int main(){

    std::ofstream outputFile("../data/deterministic_vs_inbuilt_times.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }

    outputFile << "n,deterministic_time,inbuilt_time" << std::endl;

    int n; // Initial value of n
    std::cout << "Enter maximum number of points: ";
    std::cin >> n;
    if(n>400){
        std::cout << "Too many points added. Keep them below 400" << std::endl;
        return 0;
    }
    
    while (n > 3) {
        compare_time(n, outputFile);
        n--;
    }

    outputFile.close();
    return 0;
}
