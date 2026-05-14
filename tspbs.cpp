// 注意点: 
// 距離関数は二点間の距離を四捨五入して使用しているので、
// 　用途に応じてscoreとdist()をdoubleにするかしてください

#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>
#include <algorithm>
#include <string>
#include <climits>

using namespace std;
using ll = long long;

const int MAX_N = 200;

struct Point {
    int id;
    ll x, y;
};

struct State {
    ll score; // 暫定スコア
    int now; // 今回訪問した都市
    int LastPos; // Beam[i-1][どこ]から遷移したか
    bitset<MAX_N> visited; // 訪問済み都市の集合(今回含む)
};

bool operator<(const State &a, const State &b) {
    return a.score < b.score;
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

    string nameLine;
    getline(cin, nameLine);

    string line;
    while (getline(cin, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) break;
    }

    int id;
    ll x, y;

    while (cin >> id >> x >> y) {
        p.push_back({id, x, y});
    }

    int CITIES = p.size();

    // dist, city_id
    vector<vector<pair<ll, int>>> near(CITIES);

    for (int i = 0; i < CITIES; i++) {
        vector<pair<ll, int>> tmp;

        for (int j = 0; j < CITIES; j++) {
            if (i == j) continue;

            tmp.push_back({dist(p[i], p[j]), j});
        }
        sort(tmp.begin(), tmp.end());

        near[i] = tmp;
    }

    // showNearer(near);

    const int WIDTH = 10;
    vector<int> NumState(CITIES+1);
    vector<vector<State>> Beam(CITIES, vector<State>(WIDTH));

    auto beamSearch = [&](int start) -> void {
        NumState[0] = 1;
        Beam[0][0].score = 0;
        Beam[0][0].now = start;
        Beam[0][0].visited.reset();
        Beam[0][0].visited.set(start);

        // i個めの都市に訪れる
        for (int i = 1; i < CITIES; ++i) {
            vector<State> candidate;

            // 各状態について
            for (int j = 0; j < NumState[i-1]; ++j) {
                const State &cur = Beam[i-1][j];
                // 次に行く都市を探索
                for (auto [d, k] : near[cur.now]) {
                    // 訪問済みならスキップ
                    if (cur.visited.test(k)) continue;

                    State nowState = cur; 
                    nowState.now = k;
                    nowState.LastPos = j;
                    nowState.visited.set(k);
                    nowState.score += d;

                    candidate.emplace_back(nowState);
                }
            }

            // 候補のtop WIDTH個をとってくる
            sort(candidate.begin(), candidate.end());
            NumState[i] = min(WIDTH, (int)candidate.size());
            for(int j = 0; j < NumState[i]; j++) {
                Beam[i][j] = candidate[j];
            }
        }
    };

    auto restorePath = [&]() -> vector<int> {
        vector<int> path(CITIES);
        int curr = 0;
        for (int i = CITIES-1; i >= 0; --i) {
            path[i] = Beam[i][curr].now;
            if (i > 0) {
                curr = Beam[i][curr].LastPos;
            }
        }
        return path;
    };

    auto showPath = [&](vector<int> path) -> void {
        for (auto i : path) {
            cout << p[i].id << " " << p[i].x << " " << p[i].y << endl;
        }
    };

    ll bestScore = LLONG_MAX;
    vector<int> bestSchedule;
    // 各頂点スタートを試す
    for (int start = 0; start < CITIES; ++start) {
        fill(NumState.begin(), NumState.end(), 0);
        beamSearch(start);
        ll totalScore = Beam[CITIES-1][0].score + dist(p[Beam[CITIES-1][0].now], p[start]);
        if (bestScore > totalScore) {
            bestScore = totalScore;
            bestSchedule = restorePath();
        }
    }

    cout << bestScore << endl;
    showPath(bestSchedule);
    cout << endl;
}
