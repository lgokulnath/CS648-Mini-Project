// data types in js
// Numbers:
// let length = 16;
// let weight = 7.5;

// // Strings:
// let color = "Yellow";
// let lastName = "Johnson";

// // Booleans
// let x = true;
// let y = false;

// // Object:
// const person = {firstName:"John", lastName:"Doe"};

// // Array object:
// const cars = ["Saab", "Volvo", "BMW"];

// // Date object:
// const date = new Date("2022-03-25");


const eps = 1e-6;


// credit for point class: https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Classes
class Point {
    constructor(x, y) {
      this.x = x;
      this.y = y;
    }
  
    static displayName = "point";
    static distance(a, b) {
      const dx = a.x - b.x;
      const dy = a.y - b.y;
  
      return Math.hypot(dx, dy);
    }
    static getRandomPoint(min_x, max_x, min_y, max_y) {
        let x = Math.random() * (max_x - min_x) + min_x;
        let y = Math.random() * (max_y - min_y) + min_y;
        return new Point(x, y);
    }
}

// function three_determinant(matrix) {
//     // input: 2d array of size 3x3
//     // out: determinant
//     const det = matrix[0][0] * (matrix[1][1]*matrix[2][2]-matrix[1][2]*matrix[2][1]) + matrix[0][1]*(matrix[1][2]*matrix[2][0] -  matrix[1][0]*matrix[2][2]) + matrix[0][2] * (matrix[1][0]*matrix[2][1] - matrix[1][1]*matrix[2][0]);
// }

class Circle {
    constructor(center, radius) {
        this.center = center;
        this.radius = radius;
    }

    // circle from diamterically opp points
    static diameterPoints(p1, p2) {
        const centerX = (p1.x + p2.x) / 2.0;
        const centerY = (p1.y + p2.y) / 2.0;
        const radius = Point.distance(p1, p2) / 2.0;
        const centerPoint = new Point(centerX, centerY);
        return new Circle(centerPoint, radius);
    }

    // circle passing thru 3 points
    static threePointCirle(p1, p2, p3) {
        let x1 = p1.x;
        let y1 = p1.y;
        let x2 = p2.x;
        let y2 = p2.y;
        let x3 = p3.x;
        let y3 = p3.y;
        x2 = x2 - x1;
        y2 = y2 - y1;
        x3 = x3 - x1;
        y3 = y3 - y1;

        let z1 = x2*x2 + y2*y2;
        let z2 = x3*x3 + y3*y3;
        let d = 2 * (x2 * y3 - x3 * y2);

        let xc = (z1 * y3 - z2 * y2) / d + x1;
        let yc = (x2 * z2 - x3 * z1) / d + y1;

        const centerPoint = new Point(xc, yc);
        const radius = Point.distance(centerPoint, p3);
        return new Circle(centerPoint, radius);
    }

    // static smallestEnclosingCircle(points) {
    //     if (points.length < 2) return new Circle(new Point(0,0),0);
    //     const p0 = points[0];
    //     const p1 = points[1];


    // }
}

// O(n^3) brute force algo
function bruteForceSEC(points) {
    //console.log(points.length);
    if (points.length < 2) return new Circle(new Point(0,0),0);
    // const p0 = points[0];
    // const p1 = points[1];

    // let circle = Circle.diameterPoints(p0, p1);

    // for(let i=2; i<points.length; i++) {
    //     const pi = points[i];
    //     if (Point.distance(circle.center, pi) < circle.radius) {
    //         circle = Circle.threePointCirle()
    //     }
    // }

    let sec = new Circle(new Point(0,0), maxX);

    for(let i=0; i<points.length; i++) {
        for(let j=i+1; j<points.length; j++) {
            const p0 = points[i];
            const p1 = points[j];

            let currentSec = Circle.diameterPoints(p0, p1);


            for(let k=j+1; k < points.length; k++) {
                const p2 = points[k];

                if(Point.distance(p2, currentSec.center) < currentSec.radius + eps) {
                    continue;
                }
                else {
                    currentSec = Circle.threePointCirle(p0, p1, p2);
                }
            }
            // console.log('Logging current sec:');
            // console.log(currentSec.center);
            // console.log(currentSec.radius);
            let flag = 1;
            for(let k=0; k < points.length; k++) {
                const p2 = points[k];

                if(Point.distance(p2, currentSec.center) < currentSec.radius + eps) {
                    continue;
                }
                else {
                    flag=0;
                    break;
                }
            }
            if (flag == 1 && sec.radius > currentSec.radius+eps) {
                // sec found
                sec = currentSec;
                //console.log('sec modified');
            }
            // else {
            //     // sec was not found
            // }


        }
        
    }
    return sec;

}


// https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
// equivalent to random permutation
function randomShuffle(a) {
    const n = a.length;
    for(let i=n-1; i >= 1; i--) {
        const j = Math.floor(Math.random() * (i+1));
        [a[i], a[j]] = [a[j], a[i]];
    }
    return a;

}

function checkPointOutsideCircle(circle, point) {
    const center = circle.center;
    const radius = circle.radius;

    if(Point.distance(point, center) >= radius+eps) {
        return true;
    }
    return false;
}

function _randomizedSEC2(points, p0, p1) {
    let circle = Circle.diameterPoints(p0, p1);
    for(let i=0; i<points.length; i++)  {
        if(checkPointOutsideCircle(circle, points[i])) {
            circle = Circle.threePointCirle(points[i], p0, p1);
        }
    }
    return circle;

    
}

function _randomizedSEC1(points, point) {
    let shuffledPoints = randomShuffle(points);

    let circle = Circle.diameterPoints(point, shuffledPoints[0]);

    for(let i=1; i < points.length; i++) {
        const pi = shuffledPoints[i];
        if (checkPointOutsideCircle(circle, pi)) {
            circle = _randomizedSEC2(shuffledPoints.slice(0, i) ,point, pi);
        }

    }
    return circle;

}

// randomized O(n) expected runtime algo
function randomizedSEC(points) {
    let shuffledPoints = randomShuffle(points);

    const p0 = shuffledPoints[0], p1 = shuffledPoints[1];

    let sec = Circle.diameterPoints(p0, p1);
    for(let i = 2; i < shuffledPoints.length; i++) {
        const p2 = shuffledPoints[i];
        if (checkPointOutsideCircle(sec, p2)) {
            // outside
            sec = _randomizedSEC1(shuffledPoints.slice(0, i), p2);
        }
        else {
            continue;
        }
    }
    return sec;
}

function clearCanvas() {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
}




// const p1 = new Point(0, 0);
// const p2 = new Point(4, 0);
// const p3 = new Point(2, 2);
// const c1 = Circle.threePointCirle(p1, p2, p3);

// Create an array of points
// const points = [
//     new Point(2, 2),
//     new Point(402, 2),
//     new Point(202, 202),
//     // Add more points as needed
// ];


var minX = Math.floor(Math.random()*500) + 105, minY = 105, maxX = minX + 300, maxY = 395;

var bruteForceSECRuntime = document.getElementById('bruteForceSECRuntime');bruteForceSECRuntime
var randmizedSECRuntime = document.getElementById('randomizedSECRuntime');

// Get canvas element and its context
const canvas = document.getElementById('canvas');
const rect = canvas.getBoundingClientRect();

var points = [];

console.log('width: ', rect.width);
console.log('height: ', rect.height);
const ctx = canvas.getContext('2d');


function getClickPosition(e) {
    var p = {
      x: e.clientX - rect.left,
      y: e.clientY - rect.top
    }
    console.log('Point: ');
    console.log(p.x, p.y);
    drawAt(p);
    return p;
}

function drawAt(point) {
    ctx.beginPath();
    ctx.arc(point.x, point.y, 3, 0, Math.PI * 2);
    ctx.fill();
  }
  




function genPointsAndSEC() {
    minX = Math.floor(Math.random()*500) + 105;
    maxX = minX + 300;
    //resetCanvas();
    var numPoints = parseInt(document.getElementById('numPoints').value);
    console.log('read numpoints as ');
    console.log(numPoints);
    //let points = [];

    for(let i=1; i <= numPoints; i++) {
        points.push(Point.getRandomPoint(minX, maxX, minY, maxY));
    }

    var startTime = performance.now();
    const c2 = randomizedSEC(points);
    var endTime = performance.now();
    var runtime = endTime - startTime;
    randmizedSECRuntime.value = runtime.toString() + ' ms';
    console.log(runtime);
    console.log('Runtime for randmized SEC: ', runtime);
   // console.log(runtime);
    startTime = performance.now();
    const c3 = bruteForceSEC(points);
    endTime = performance.now();
    runtime = endTime-startTime;
    bruteForceSECRuntime.value = runtime.toString() + ' ms';
    console.log('Runtime for brute force SEC: ', runtime);
    //console.log(runtime);
    // console.log(c1.center);
    // console.log(c1.radius);
    console.log('center and radius of circle as below');
    console.log(c2.center);
    console.log(c2.radius);

    

    // Draw points
    ctx.fillStyle = 'blue';
    points.forEach(point => {
    ctx.beginPath();
    ctx.arc(point.x, point.y, 3, 0, Math.PI * 2);
    ctx.fill();
    });

    // Draw enclosing circle
    ctx.strokeStyle = 'red';
    ctx.beginPath();
    ctx.arc(c2.center.x, c2.center.y, c2.radius, 0, Math.PI * 2);
    ctx.stroke();

}

function genSEC() {
    console.log('Printing points....');
    console.log(points);
    var startTime = performance.now();
    const c2 = randomizedSEC(points);
    var endTime = performance.now();
    var runtime = endTime - startTime;
    randmizedSECRuntime.value = runtime.toString() + ' ms';
    console.log(runtime);
    console.log('Runtime for randmized SEC: ', runtime);
   // console.log(runtime);
    startTime = performance.now();
    const c3 = bruteForceSEC(points);
    endTime = performance.now();
    runtime = endTime-startTime;
    bruteForceSECRuntime.value = runtime.toString() + ' ms';
    console.log('Runtime for brute force SEC: ', runtime);
    //console.log(runtime);
    // console.log(c1.center);
    // console.log(c1.radius);
    console.log('center and radius of circle as below');
    console.log(c2.center);
    console.log(c2.radius);

    

    

    // Draw enclosing circle
    ctx.strokeStyle = 'red';
    ctx.beginPath();
    ctx.arc(c2.center.x, c2.center.y, c2.radius, 0, Math.PI * 2);
    ctx.stroke();

}

function addManualPoints() {
    //points = [];
    document.getElementById("canvas").addEventListener('click', function(e) {
        console.log('click detected');
        var point = getClickPosition(e);
        drawAt(point);
        points.push(point);
    })
    

}

function resetCanvas() {
    points.length = 0;
    console.log('reset function');
    console.log(points);
    clearCanvas();
}







