int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int result;
int a;
int b;
print("Enter first number");
scan(a);
print("Enter second number");
scan(b);
result = add(a, b);
print("Sum is");
print(result);
result = multiply(a, b);
print("Product is");
print(result);
