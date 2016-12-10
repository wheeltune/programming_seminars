#include <vector>
#include <iostream>

using namespace std;

const long mod1 = 1e9 + 7;
const long mod2 = 1e9 + 13;
const long p1 = 13;
const long p2 = 7;

long hash1[100001];
long hash2[100001];
long pow1 [100001];
long pow2 [100001];

string s;

void calcPow(long *po, int p, int mod, int n) {
    po[0] = 1;
    for (int i = 1; i < n; ++i) {
        po[i] = (po[i - 1] * p) % mod;
    }
}

void calcHash(long *hash, const long *po, long mod) {
    hash[0] = 0;
    for (int i = 1; i < s.length() + 1; ++i) {
        hash[i] = (po[i - 1] * s[i - 1] + hash[i - 1]) % mod;
    }
}

bool checkEqual(const long *hash, const long *po, long mod, int s1, int l1, int s2, int l2) {
    if (l1 != l2) return false;
    if (s1 > s2) {
        swap(s1, s2);
        swap(l1, l2);
    }

    long h1 = (hash[s1 + l1] - hash[s1]) % mod;
    if (h1 < 0) {
        h1 += mod;
    }
    h1 *= po[s2 - s1];
    h1 %= mod;

    long h2 = (hash[s2 + l2] - hash[s2]) % mod;
    if (h2 < 0) {
        h2 += mod;
    }

    return h1 == h2;
}

bool checkEqual(int s1, int l1, int s2, int l2) {
    return checkEqual(hash1, pow1, mod1, s1, l1, s2, l2) && 
        checkEqual(hash2, pow2, mod2, s1, l1, s2, l2);

}

int main() {
    cin >> s;
    int n = s.length();

    calcPow(pow1, p1, mod1, n);
    calcPow(pow2, p2, mod2, n);

    calcHash(hash1, pow1, mod1);
    calcHash(hash2, pow2, mod2);

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int s1, f1, s2, f2;
        scanf("%d %d %d %d", &s1, &f1, &s2, &f2);
        s1--; s2--;
        if (checkEqual(s1, f1 - s1, s2, f2 - s2)) {
            printf("Yes\n");
        } else {
            printf("No\n");
        }
    }

    return 0;
}