# Smallest Enclosing Cirlce Algorithm

## sec_compare_time.cpp

Has implementations of both recursive and iterative versions of SEC algorithms and their optimized versions (with move to start heuristic)

It contains various functions:
### Circle_2 recursive(std::vector<Point_2> &points, bool shuffle=true)
This is the standard recursive method of computing SEC.
### Circle_2 recursive_list(std::list<Point_2> &P, bool shuffle=true)
This function makes use of the move to front heuristic to compute the SEC and works faster.
### Circle_2 iterative(std::vector<Point_2> &points, bool shuffle=true) 
Standard iterative implementation of finding SEC.
### Circle_2 iterative_opt(std::list<Point_2> &points, bool shuffle=true) 
This iterative version makes use of move to start heuristic.
### Min_circle mc(<arguments>)
This is CGAL's inbuilt function to compute SEC.

Command to execute:
```
cd code
g++ sec_compare_time.cpp -o sec_compare_time -lgmp
./sec_compare_time
```

## deterministic_On3.cpp

Implements the deterministic algorithm for computing the SEC that runs in $O(n^3)$ time, and then compares it with the in-built CGAL algorithm for computing SEC.

Command to execute:
```
cd code
g++ deterministic_On3.cpp -o deterministic_On3
./deterministic_On3
```

### bool isAcuteAngleTriangle(const Point& p1, const Point& p2, const Point& p3)

* Input: Three points p1, p2, p3
* Returns: True if the triangle formed by these three points is acute, else false

### pair<Point, double> algorithm1(vector<Point> points)

* Input: a vector of points
* Returns: Circle (as a pair of center point and radius) with maximum radius among all circles formed by pairs of points as diametric ends or circumcircles of acute-angled triangles

### pair<Point, double> algorithm2(vector<Point> points)

* Input: a vector of points
* Returns: Smallest enclosing circle using CGAL's inbuilt algorithm

### int compare_time(int n, std::ofstream& outfile)

* Input: number of points n, string representing the output file name where the runtime values will be logged
* Function: generates n points randomly and stores in a vector of points. Calls algorithm1 and algorithm2 on this vector and then logs the corresponding runtimes into outfile


## expected_values_experiment.cpp

Given a set of points P in 2D plane, generates a random subset S of P containing n/2 points. Then, computes the SEC for set S, and computes the number of points lying outside this SEC. Finally, logs the empirical average values for further analysis.

Command to execute:
```
cd code
g++ expected_values_experiment.cpp -o expected_values_experiment 
./expected_values_experiment
```

### calculate_expected_value(int n, int repetitions, std::ofstream& outfile)

* Input: Number of points (n), Number of repetitions for the experiment, string representing the output file name for logs
* Function: Generates n random points and stores into a vector, randomly shuffles the vector and takes a subset of size n/2. Finally, computes the SEC and then calculates the number of points lying outside this SEC. Logs the empirical average values (dividing by repetitions) into the outfile


## points_outside_sec.cpp

Given a set of points P in 2D plane, generates a random subset S of P containing i points (i = n/2 and sqrt(n)). Then, computes the SEC for set S, and computes the number of points lying outside this SEC. Finally, logs the empirical average values for further analysis.

Command to execute:
```
cd code
g++ points_outside_sec.cpp -o points_outside_sec 
./points_outside_sec
```


## compare_all_four.cpp

Given a sample of set of points P in 2D plane and n_samples denoting the number of samples to generate, generates SEC for the sample using four different algorithms: Randomized Expected O(n) algorithm, Welzl's Algorithm, CGAL's inbuilt algorithm, and deterministic $O(n^3)$ algorithm. Logs their runtimes into a csv file which is used for further analysis

Command to execute:
```
cd code
g++ compare_all_four.cpp -o compare_all_four -lgmp
./compare_all_four <numPoints> <numSamples>
```


### Circle convert_to_circle(const Point &center, double radius)

* Input: Point denoting the center, radius
* Returns: Circle with given center and radius

### Circle min_circle_trivial(const std::vector<Point> &P)

* Input: Vector of points 
* Require: Size of the input vector should be less than or equal to 3
* Returns: SEC corresponding to the input set of points

### Circle welzl(const std::vector<Point> &P)

* Input: Vector of points
* Function: Calls welzl_helper() function to compute the SEC using the standard Recursive implementation of Welzl's algorithm for the given set of points
* Returns: SEC corresponding to the input set of points

### Circle_2 sec(std::vector<Point_2> &points) 

* Input: Vector of points
* Returns: SEC for the given set of points using the Randomized Incremental algorithm with Expected O(n) time

### Circle_2 sec1(std::vector<Point_2> &points, int n, Point_2 q) 

* Input: Vector of points, number of points n, a given point q
* Returns: SEC for the given set of points given that q is one of the defining points for the required SEC

### Circle_2 sec2(Point_2 p1, Point_2 p2, std::vector<Point_2> &points, int n)

* Input: Vector of points, number of points n, two given points p1 and p2
* Returns: SEC for the given set of points given that p1 and p2 are two defining points for the required SEC


### bool isAcuteAngleTriangle(const Point& p1, const Point& p2, const Point& p3)

* Input: Three points p1, p2, p3
* Returns: True if the triangle formed by these three points is acute, else false

### pair<Point, double> algorithm1(vector<Point> points)

* Input: a vector of points
* Returns: Circle (as a pair of center point and radius) with maximum radius among all circles formed by pairs of points as diametric ends or circumcircles of acute-angled triangles

### pair<Point, double> algorithm2(vector<Point> points)

* Input: a vector of points
* Returns: Smallest enclosing circle using CGAL's inbuilt algorithm


## spiral_test_case.cpp

Generates and analyzes the test case where the points are arranged in a spiral arrangement. This test case proves to be the worst possible test case for the randomized SEC algorithm (proved in the report)

Command to execute:
```
cd code
g++ spiral_test_case.cpp -o spiral_test_case
./spiral_test_case
```


### std::vector<Point_2> generate_spiral(int n, double max_radius)

* Input: Number of points (n), maximum radius required for the spiral 
* Returns: A vector of points in a spiral arrangement

