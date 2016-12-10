#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

string str;


int main() {
    
    cin >> str;
    int n = str.length();
    vector<int> pref(n, 0);
    vector<int> filledBy(n, 0);
    vector<int> ans(n, 0);

    int pos = 0;
    for (int i = 1; i < n; ++i) {
        while (pos > 0 && str[pos] != str[i]) {
            pos = pref[pos - 1];
        }

        if (str[pos] == str[i]) {
            pos++;
        }
        pref[i] = pos;

        if (pos > 0 && filledBy[ans[pos - 1]] + ans[pos - 1] + 1 >= i) {
            ans[i] = ans[pos - 1];
            filledBy[ans[i]] = i;
        } else {
            ans[i] = i;
            filledBy[i] = i;
        }
        
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] + 1 << " ";
    }
    cout << endl;
    
    return 0;
}