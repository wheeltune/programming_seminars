#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> prefix_function (string s) {
    int n = (int) s.length();
    vector<int> pi (n);
    for (int i=1; i<n; ++i) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j])  ++j;
        pi[i] = j;
    }
    return pi;
}

vector<int> z_function (string s) {
    int n = (int) s.length();
    vector<int> z (n);
    for (int i=1, l=0, r=0; i<n; ++i) {
        if (i <= r)
            z[i] = min (r-i+1, z[i-l]);
        while (i+z[i] < n && s[z[i]] == s[i+z[i]])
            ++z[i];
        if (i+z[i]-1 > r)
            l = i,  r = i+z[i]-1;
    }
    return z;
}

int main() {
    string s;
    cin >> s;
    if (s[0] == '1') {
        cout << "1.(0)\n";
        return 0;
    }
    s = s.substr(2, s.length() - 2);

    reverse(s.begin(), s.end());
    int n = s.length();

    std::vector<int> pi = prefix_function(s);
    std::vector<int> z = z_function(s);

    int bestI = 0;
    int bestLen = n;

    for (int i = 1; i < n; ++i) {
        int k = (i + 1) - pi[i];
        if ((i + 1) % k == 0) {
            int j = i;
            if (i < n - 1) {
                j += z[i + 1];
            }
            int len = k + n - (j + 1);
            if (len < bestLen) {
                bestLen = len;
                bestI = j;
            }
        } else {
            int j = i;
            if (i < n - 1) {
                j += z[i + 1];
            }
            int len = (i + 1) + n - (j + 1);
            if (len < bestLen) {
                bestLen = len;
                bestI = j;
            }
        }
    }
    cout << "0.";
    for (int i = n - 1; i > bestI; --i) {
        cout << s[i];
        bestLen--;
    }
    cout << "(";
    for (int i = bestI; i > bestI - bestLen; --i) {
        cout << s[i];
    }
    cout << ")\n";

    return 0;
}