#include <bits/stdc++.h>
#include <vector>
#include <string>
using namespace std;

struct SubstrRank
{
public:
    int leftRank;
    int rightRank;
    int index;

    SubstrRank(int leftRank = 0, int rightRank = 0, int index = 0)
    {
        this->leftRank = leftRank;
        this->rightRank = rightRank;
        this->index = index;
    }
};

vector<int> makeRanks(const vector<SubstrRank> &substr_rank, int n)
{
    int r = 1;
    vector<int> rank(n, -1);
    rank[substr_rank[0].index] = r;

    for (int i = 1; i < n; i++)
    {
        if (substr_rank[i].leftRank != substr_rank[i - 1].leftRank || substr_rank[i].rightRank != substr_rank[i - 1].rightRank)
        {
            r++;
        }
        rank[substr_rank[i].index] = r;
    }

    return rank;
}

int main()
{
    string T = "mississippi";
    vector<int> SA = suffixArray(T);

    for (int i : SA)
        cout << i << " ";
    cout << endl;

    return 0;
}
