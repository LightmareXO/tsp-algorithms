#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;
};

struct State {
    Point p; // 現在位置
    ll score; // 暫定スコア
    int LastMove; // 最後に移動した都市
    int LastPos; // Beam[i-1][どこ]から遷移したか
};

bool operator>(const State &a, const State &b) {
    return a.score > b.score;
}

ll dist(Point a, Point b) {
    return round(sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)));
}

void showNearer(vector<vector<pair<ll, int>>> p) {
    for (int i = 0; i < p.size(); i++) {
        for (int j = 0; j < p[i].size(); j++) {
            cout << p[i][j].first  << " ";
    }
        cout << endl;
    }
}

int main() {
    vector<Point> p;

    int id, x, y;

    while (cin >> id >> x >> y) {
        p.push_back({x, y});
    }

    int size = p.size();

    // dist, city_id
    vector<vector<pair<ll, int>>> near(size);

    for (int i = 0; i < size; i++) {
        vector<pair<ll, int>> tmp;

        for (int j = 0; j < p.size(); j++) {
            if (i == j) continue;

            tmp.push_back({dist(p[i], p[j]), j});
        }
        sort(tmp.begin(), tmp.end());

        near[i] = tmp;
    }

    // showNearer(near);

    const int WIDTH = 10;
    int NumState[size];
    State Beam[size][WIDTH];
    Point schedule[size];

    auto beamSearch = [&](Point start) -> void {
        NumState[0] = 1;
        Beam[0][0].score = 0;
        Beam[0][0].p = start;

        for (int i = 1; i <= size; ++i) {
            vector<State> candidate;

            // 
            for (int j = 0; j < NumState[i-1]; ++j) {
                
            }


            sort(candidate.begin(), candidate.end());
            NumState[i] = min(WIDTH, (int)candidate.size());

            for(int j = 0; j < NumState[i]; j++) {
                Beam[i][j] = candidate[j];
            }
        }
    };


}