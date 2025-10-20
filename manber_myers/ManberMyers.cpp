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

vector<int> suffixArray(const string &T)
{
    int n = T.size();
    vector<SubstrRank> substr_rank;

    for (int i = 0; i < n; i++)
    {
        int left = (int)T[i];
        int right = (i < n - 1) ? (int)T[i + 1] : 0;
        substr_rank.push_back(SubstrRank(left, right, i));
    }

    sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank &a, const SubstrRank &b)
         {
        if (a.leftRank == b.leftRank)
            return a.rightRank < b.rightRank;
        return a.leftRank < b.leftRank; });

    int l = 2;
    while (l < n)
    {
        vector<int> rank = makeRanks(substr_rank, n);

        for (int i = 0; i < n; i++)
        {
            int nextIndex = substr_rank[i].index + l;
            int right = (nextIndex < n) ? rank[nextIndex] : 0;
            substr_rank[i] = SubstrRank(rank[substr_rank[i].index], right, substr_rank[i].index);
        }

        sort(substr_rank.begin(), substr_rank.end(), [](const SubstrRank &a, const SubstrRank &b)
             {
            if (a.leftRank == b.leftRank)
                return a.rightRank < b.rightRank;
            return a.leftRank < b.leftRank; });

        l *= 2;
    }

    vector<int> SA(n);
    for (int i = 0; i < n; i++)
        SA[i] = substr_rank[i].index;

    return SA;
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
