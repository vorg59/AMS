// file: calculator.js

function add(a, b) {
    return a + b;
}

function longFunction(x, y, z) {
    if (x > 0) {
        if (y > 0) {
            if (z > 0) {
                return x + y + z;
            } else {
                return x + y - z;
            }
        } else {
            if (z > 0) {
                return x - y + z;
            } else {
                return x - y - z;
            }
        }
    } else {
        if (y > 0) {
            if (z > 0) {
                return -x + y + z;
            } else {
                return -x + y - z;
            }
        } else {
            if (z > 0) {
                return -x - y + z;
            } else {
                return -x - y - z;
            }
        }
    }
}

function duplicateCode() {
    let a = 1, b = 2, c = 3;
    console.log(a + b + c);
    let d = 1, e = 2, f = 3;
    console.log(d + e + f);
}
