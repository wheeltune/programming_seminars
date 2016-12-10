#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int len;
vector<int> pi;
vector<int> z;

void buildZ () {
    z[0] = len;
    for (int i = 1, l = 0, r = 0; i < len; ++i) {
        if (i <= r) {
            z[i] = min(r - i + 1, z[i - l]);
        }
        while (i + z[i] < len && pi[i + z[i]] == z[i] + 1) {
            z[i]++;
        }
        if (i + z[i] - 1 > r) {
            l = i;
            r = i + z[i] - 1;
        }
    }
}

int main() {
    cin >> len;
    
    pi.resize(len);
    z.resize(len, 0);

    for (int i = 0; i < len; ++i) {
        cin >> pi[i];
    }

    buildZ();

    for (int i = 0; i < len; ++i) {
        cout << z[i] << " ";
    }
    cout << endl;


    return 0;
}