int n;
int i;
int sum;
int isEven;
char grade;

sum = 0;
i = 1;
n = 10;

while (i <= n) {
    isEven = i - (i / 2) * 2;

    if (isEven == 0) {
        sum = sum + i;
    }

    i = i + 1;
}

print("sum of even numbers:");
print(sum);

if (sum >= 30) {
    grade = 'A';
} else if (sum >= 20) {
    grade = 'B';
} else {
    grade = 'C';
}

print("grade:");
print(grade);
