#include <iostream>
#include <vector>
#include <algorithm>

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

int main() {
    string s;
    
    cin >> s;
    int n = s.length();
    vector<int> lastLen(n, 0);
    int cnt = 0;

    for (int i = 0; i < n; ++i) {
        string cS = s.substr(0, i + 1);
        reverse(cS.begin(), cS.end());
        vector<int> pi = prefix_function(cS);
        vector<bool> used(n, 0);
        used[0] = true;

        for (int j = i; j > 0; --j) {

            int pos = j;

            while (pos > 0 && !used[pos]) {
                lastLen[i - j + pi[pos] - 1] = max(lastLen[i - j + pi[pos] - 1], min(j - pi[pos] + 1, pi[pos]));
                used[pos] = true;
                pos = pi[pos] - 1;
            }   

            lastLen[i] = max(lastLen[i], pi[j]);
        }
        cnt += i - lastLen[i] + 1;
    }
    int ans = 0;
    for (int i = 0; i < lastLen.size(); ++i) {
        ans += i - lastLen[i] + 1;
    }
    cout << cnt - ans << endl;
    return 0;
}


// #include <iostream>
// #include <vector>
// #include <algorithm>

// using namespace std;

// vector<int> prefix_function (string s) {
//     int n = (int) s.length();
//     vector<int> pi (n);
//     for (int i=1; i<n; ++i) {
//         int j = pi[i-1];
//         while (j > 0 && s[i] != s[j])
//             j = pi[j-1];
//         if (s[i] == s[j])  ++j;
//         pi[i] = j;
//     }
//     return pi;
// }

// int main() {
//     string s;
    
//     cin >> s;
//     int n = s.length();
//     vector<int> to(n, 0);
//     int cnt = 0;

//     for (int i = 0; i < n; ++i) {
//         string cS = s.substr(0, i + 1);
//         reverse(cS.begin(), cS.end());
//         vector<int> pi = prefix_function(cS);

//         int from = 0;
//         int m = 0;
//         for (int j = 1; j < pi.size(); ++j) {
//             from = max(from, to[j - pi[j] + 1]);
//             int pos = j;
//             while (min(pi[pos], j - pi[pos] + 1) > m) {
//                 m = min(pi[pos], j - pi[pos] + 1);
//                 pos = pi[pos] - 1;
//             }
//         }
//         cnt += max(0, m - from);
//         to[i] = m;
//     }
//     cout << cnt << endl;
//     return 0;
// }