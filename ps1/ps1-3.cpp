#include <iostream>
using namespace std;

int main() {
    float a, b, c, d, e, f, x, y;
    cout << "ax+by=c\n";
    cout << "dx+ey=f\n";
    cout << "Enter a b c d e f:";
    cin >> a >> b >> c >> d >> e >> f;
    if (a * e - b * d < 0.000001) {
        cout << "No solution";
        return 0;
    }
    x = (c * e - b * f) / (a * e - b * d);
    y = (a * f - c * d) / (a * e - b * d);
    cout << "x=" << x << " y=" << y;
    return 0;
}