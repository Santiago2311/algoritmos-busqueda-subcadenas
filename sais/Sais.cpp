#include <bits/stdc++.h>
using namespace std;

map<int, pair<int,int>> getBuckets(vector<int> T) {
    map<int,int> count;
    map<int,pair<int,int>> buckets;

    for (int c : T)
        count[c] = count[c] + 1;

    int start = 0;
    for (auto &p : count) {
        int c = p.first;
        buckets[c] = {start, start + count[c]};
        start += count[c];
    }
    return buckets;
}

vector<int> sais(vector<int> T) {
    int n = T.size();
    vector<char> t(n, '_');

    t[n-1] = 'S';
    for (int i = n - 1; i > 0; i--) {
        if (T[i-1] == T[i])
            t[i-1] = t[i];
        else
            t[i-1] = (T[i-1] < T[i]) ? 'S' : 'L';
    }

    map<int,pair<int,int>> buckets = getBuckets(T);
    map<int,int> count;
    vector<int> SA(n, -1);
    map<int,int> LMS;
    int end = -1;

    for (int i = n - 1; i > 0; i--) {
        if (t[i] == 'S' && t[i-1] == 'L') {
            int revoffset = ++count[T[i]];
            SA[buckets[T[i]].second - revoffset] = i;
            if (end != -1)
                LMS[i] = end;
            end = i;
        }
    }

    LMS[n - 1] = n - 1;
    count.clear();

    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && SA[i] - 1 >= 0 && t[SA[i] - 1] == 'L') {
            int symbol = T[SA[i] - 1];
            int offset = count[symbol];
            SA[buckets[symbol].first + offset] = SA[i] - 1;
            count[symbol] = offset + 1;
        }
    }

    count.clear();
    for (int i = n - 1; i > 0; i--) {
        if (SA[i] > 0 && t[SA[i] - 1] == 'S') {
            int symbol = T[SA[i] - 1];
            int revoffset = ++count[symbol];
            SA[buckets[symbol].second - revoffset] = SA[i] - 1;
        }
    }

    vector<int> namesp(n, -1);
    int name = 0;
    int prev = -1;
    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && t[SA[i]] == 'S' && SA[i] - 1 >= 0 && t[SA[i] - 1] == 'L') {
            if (prev != -1) {
                vector<int> sub1(T.begin() + SA[prev], T.begin() + LMS[SA[prev]]);
                vector<int> sub2(T.begin() + SA[i], T.begin() + LMS[SA[i]]);
                if (sub1 != sub2)
                    name++;
            }
            prev = i;
            namesp[SA[i]] = name;
        }
    }

    vector<int> names;
    vector<int> SApIdx;
    for (int i = 0; i < n; i++) {
        if (namesp[i] != -1) {
            names.push_back(namesp[i]);
            SApIdx.push_back(i);
        }
    }

    if (name < (int)names.size() - 1)
        names = sais(names);

    reverse(names.begin(), names.end());

    SA.assign(n, -1);
    count.clear();
    for (int i = 0; i < (int)names.size(); i++) {
        int pos = SApIdx[names[i]];
        int revoffset = ++count[T[pos]];
        SA[buckets[T[pos]].second - revoffset] = pos;
    }

    count.clear();
    for (int i = 0; i < n; i++) {
        if (SA[i] >= 0 && SA[i] - 1 >= 0 && t[SA[i] - 1] == 'L') {
            int symbol = T[SA[i] - 1];
            int offset = count[symbol];
            SA[buckets[symbol].first + offset] = SA[i] - 1;
            count[symbol] = offset + 1;
        }
    }

    count.clear();
    for (int i = n - 1; i > 0; i--) {
        if (SA[i] > 0 && t[SA[i] - 1] == 'S') {
            int symbol = T[SA[i] - 1];
            int revoffset = ++count[symbol];
            SA[buckets[symbol].second - revoffset] = SA[i] - 1;
        }
    }

    return SA;
}

int main() {
    string s = "GTCCCGATGTCATGTCAGGA$";
    vector<int> T;
    for (char c : s)
        T.push_back((int)c);

    vector<int> SA = sais(T);

    for (int x : SA)
        cout << x << " ";
    cout << endl;

    return 0;
}
