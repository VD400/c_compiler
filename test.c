int a;
int b;
int c;
int i;
int result;
char grade;
char ch;

a = 15;
b = 4;
c = 0;

print("=== ARITHMETIC ===");
c = a + b;
print(c);
c = a - b;
print(c);
c = a * b;
print(c);
c = a / b;
print(c);

print("=== RELATIONAL ===");
c = a > b;
print(c);
c = a < b;
print(c);
c = a >= 15;
print(c);
c = a <= 14;
print(c);
c = a == 15;
print(c);
c = a != 15;
print(c);

print("=== LOGICAL ===");
c = a > 10 && b > 2;
print(c);
c = a < 10 || b > 2;
print(c);
c = !(a == 15);
print(c);

print("=== PRECEDENCE ===");
c = 2 + 3 * 4;
print(c);
c = (2 + 3) * 4;
print(c);
c = 10 - 4 - 2;
print(c);

print("=== CONSTANT FOLDING ===");
c = 6 + 9;
print(c);
c = 100 / 5;
print(c);
c = 7 > 3;
print(c);

print("=== WHILE LOOP ===");
i = 1;
result = 0;
while (i <= 5) {
    result = result + i;
    i = i + 1;
}
print(result);

print("=== BREAK AND CONTINUE ===");
i = 1;
while (i <= 10) {
    if (i == 3) {
        i = i + 1;
        continue;
    }
    if (i == 6) {
        break;
    }
    print(i);
    i = i + 1;
}

print("=== IF ELSE CHAIN ===");
result = 75;
if (result >= 90) {
    print(1);
} else if (result >= 75) {
    print(2);
} else if (result >= 60) {
    print(3);
} else {
    print(4);
}

print("=== CHAR TYPE ===");
ch = 'A';
print(ch);
ch = 'z';
print(ch);
ch = 'A' + 1;
print(ch);
c = 'Z' - 'A';
print(c);

print("=== SCOPING ===");
a = 100;
{
    int a;
    a = 999;
    print(a);
}
print(a);

print("=== NESTED LOOPS ===");
i = 1;
while (i <= 3) {
    b = 1;
    while (b <= 3) {
        result = i * b;
        print(result);
        b = b + 1;
    }
    i = i + 1;
}

print("=== SCAN ===");
scan(a);
print(a);

print("=== DONE ===");
