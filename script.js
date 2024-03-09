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


function bruteForceSEC(points) {
    console.log(points.length);
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
                console.log('sec modified');
            }
            // else {
            //     // sec was not found
            // }


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

const minX = 205, minY = 205, maxX = 395, maxY = 295;

// Get canvas element and its context
const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

function genPointsAndSEC() {



    let points = [];

    for(let i=1; i <= 10; i++) {
        points.push(Point.getRandomPoint(minX, maxX, minY, maxY));
    }


    const c2 = bruteForceSEC(points);

    // console.log(c1.center);
    // console.log(c1.radius);

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

function resetCanvas() {
    clearCanvas();
}







