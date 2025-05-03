# file: calculator.py

def add(a, b):
    return a + b

def subtract(a, b):
    return a - b

def multiply(a, b):
    return a * b

def divide(a, b):
    if b == 0:
        return "Error: Division by zero"
    return a / b

def complex_function(x, y, z):
    if x > 0:
        if y > 0:
            if z > 0:
                return x + y + z
            else:
                return x + y - z
        else:
            if z > 0:
                return x - y + z
            else:
                return x - y - z
    else:
        if y > 0:
            if z > 0:
                return -x + y + z
            else:
                return -x + y - z
        else:
            if z > 0:
                return -x - y + z
            else:
                return -x - y - z

def duplicate_code():
    a = 1
    b = 2
    c = 3
    print(a + b + c)
    a = 1
    b = 2
    c = 3
    print(a + b + c)
