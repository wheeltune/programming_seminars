#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> findLCP (string str, vector<int> suff) {
   str += "$";
   suff.insert(suff.begin(), str.length() - 1);

   int n = str.length();
   vector<int> lcp(n, 0);
   vector<int> pos(n);

   for (int i = 0; i < n; ++i) {
      pos[suff[i]] = i;
   }

   int k = 0;
   for (int i = 0; i < n - 1; ++i) {
      if (k > 0) {
         k--;
      }
      if (pos[i] == n - 1) {
         lcp[n - 1] = -1;
         k = 0;
      } else {
         int j = suff[pos[i] + 1];
         while (max(i + k, j + k) < n && str[i + k] == str[j + k]) {
            k++;
         }
         lcp[pos[i]] = k;
      }
   }
   return lcp;
}

int main() {
   int n = 0;
   string str;
   cin >> n >> str;
   vector<int> suff(n);
   
   for (int i = 0; i < n; ++i) {
      cin >> suff[i];
      suff[i]--;
   }

   vector<int> lcp = findLCP(str, suff);
   for (int i = 1; i < n; ++i) {
      cout << lcp[i] << " ";
   }
   cout << endl;
   return 0;
}