
void functionA()
{
    int sum = 0;
    while (sum < 10e5)
        sum++;
}

void functionB()
{
    int sum = 0;
    while (sum < 30e5)
        sum++;
}

int main()
{
    int iterations = 1500;

    while (iterations--) {
        functionB();
        functionA();
    }

    return 0;
}
