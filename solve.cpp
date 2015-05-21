#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <array>
#define rep(i, n) for (int (i) = 0; (i) < (int)(n); (i)++)
//#define DEBUG

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const int BEAM_SEARCH_WIDTH = 3000;
using namespace std;
typedef long long ll;

pair<int, int> shouldBeThere[16];
ll p16[16];
int p8[8];

struct moveCell {
    int ny;
    int nx;
    int dir;
};

class Board {
public:
    /*----- メンバ関数 -----*/
    // コンストラクタ
    Board(const array<array<int, 4>, 4>& init);
    // y, xにある数字をoriginY, originXに動かす
    void move(int y, int x);
    // 今の盤面を記述する
    void print() const;

    /* -----メンバ変数----- */
    // ボード
    array<array<int, 4>, 4> board;
    // それぞれの数字のある場所
    array<pair<int, int>, 16> pos;
    // 0のいる点
    int originX;
    int originY;
};

Board::Board(const array<array<int, 4>, 4>& init) : board(init) {
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        if (board[i][j] == 0) {
            originY = i;
            originX = j;
        }
        pos[board[i][j]] = make_pair(i, j);
    }
}

void Board::move(int y, int x) {
    int tmp = board[y][x];
    // ボードの変化を記録
    board[originY][originX] = tmp;
    board[y][x] = 0;
    pos[tmp] = make_pair(originY, originX);
    pos[0] = make_pair(y, x);
    originX = x;
    originY = y;
}
void Board::print() const {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            cout << board[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "origin(y, x) is " << originY << "  " << originX << endl;
}

void init() {
    // shouldbethereのinit
    for (int i = 0; i < 16; i++) {
        if (i == 0) {
            shouldBeThere[i] = make_pair(3, 3);
        } else {
            shouldBeThere[i] = make_pair((i-1)/4, (i-1)%4);
        }
    }
    // p16のinit
    p16[0] = 1;
    for (int i = 1; i < 16; i++) {
        p16[i] = 16 * p16[i-1];
    }
    // p8のinit
    p8[0] = 1;
    for (int i = 1; i < 8; i++) {
        p8[i] = 8*p8[i-1];
    }
}

// nowが目標値lからrの間にあるか
bool isGoal(int l, int r, int now) {
    return l <= now && now <= r;
}

// stateからny, nxにあるセルを動かした時どのような状態になるかを返す
int nextState(int state, int ny, int nx) {
    int y[5], x[5];
    for (int i = 0; i <= 4; i++) {
        int p = state & (16-1);
        y[i] = p/4; x[i] = p%4;
        state /= 16;
    }
    int next = 4*ny+nx; // 次の状態
    for (int i = 1; i <= 4; i++) {
        next += p16[i]*(4*y[i]+x[i]);
    }
    for (int i = 1; i <= 4; i++) {
        if (ny == y[i] && nx == x[i]) {
            next += p16[i] * ((4*y[0]+x[0]) - (4*y[i]+x[i]));
        }
    }
    return next;
}

class sevenPuzzle {
public:
    sevenPuzzle(const array<array<int, 4>, 2>& init) : board(init) {
        for (int i = 0; i < 2; i++) for (int j = 0; j < 4; j++) {
            if (board[i][j] == 0) {
                originY = i;
                originX = j;
            }
        }
    }
    // y, xにある数字をoriginY, originXに動かす
    void move(int y, int x) {
        int tmp = board[y][x];
        // ボードの変化を記録
        board[originY][originX] = tmp;
        board[y][x] = 0;
        originX = x;
        originY = y;
    }
    // 今の盤面を記述する
    void print() const {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 4; j++) {
                cout << board[i][j] << "\t";
            }
            cout << endl;
        }
        cout << "origin(y, x) is " << originY << "  " << originX << endl;
    }
    // 比較関数
    bool operator<(const sevenPuzzle& rhs) const {
        for (int i = 0; i < 2; i++) for (int j = 0; j < 4; j++) {
            if (board[i][j] < rhs.board[i][j]) return true;
            else if (board[i][j] > rhs.board[i][j]) return false;
        }
        return false;
    }
    bool operator!=(const sevenPuzzle& rhs) const {
        for (int i = 0; i < 2; i++) for (int j = 0; j < 4; j++) {
            if (board[i][j] != rhs.board[i][j]) return true;
        }
        return false;
    }

    array<array<int, 4>, 2> board;
    // 0のいる点
    int originX;
    int originY;
};

bool ok(const sevenPuzzle& sp) {
    if (sp.board[1][3] != 0) return false;
    if (sp.board[0][3] != 12) return false;
    for (int i = 0; i < 2; i++) for (int j = 0; j < 3; j++) {
        if (sp.board[i][j] != 9+i*4+j) return false;
    }
    return true;
}

int main() {
    // 答えを入れておく
    vector<int> ans;
    init();
    // データの入力
    array<array<int, 4>, 4> init;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) {
        string s;
        int a;
        cin >> s;
        if (s == "*") a = 0;
        else a = stoi(s);
        init[i][j] = a;
    }
    Board board(init);
    // 問題を解く
    /* 計画としては
     * 上1行を揃える(全探索)
     * 上2行を揃える(全探索)
     * 下2行を揃える(全探索)
     */
    // 動かした座標を覚えておく(数字ではない)
    vector<pair<int, int> > path;

    // まずは上1行を揃える
    {
        map<int, moveCell> m1; // 0,1,2,3,4の座標の情報がfirst,その直前の動きがsecond
        // 幅優先探索
        queue<int> que;
        int first = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int tmp = init[i][j];
                if (tmp > 4) continue;
                first += p16[tmp] * (4*i+j);
            }
        }
        m1.insert(make_pair(first, moveCell{-1, -1, 0}));
        que.push(first);
        const int goal1 = 0*p16[1] + 1*p16[2] + 2*p16[3] + 3*p16[4];
        int goalmemo = 0;
        while (!que.empty()) {
            int now = que.front(); que.pop();
            if (isGoal(goal1, goal1+15, now)) {
                goalmemo = now;
                break;
            }
            int origin = now & (16-1);
            int y0 = origin / 4, x0 = origin % 4;
            for (int k = 0; k < 4; k++) {
                int ny = y0 + dy[k];
                int nx = x0 + dx[k];
                if (ny < 0 || ny >= 4 || nx < 0 || nx >= 4) continue;
                int next = nextState(now, ny, nx); // 次の状態
                if (m1.find(next) != m1.end()) continue;
                m1.insert(make_pair(next, moveCell{ny, nx, k}));
                que.push(next);
            }
        }
        int cur = goalmemo;
        while (cur != first) {
            auto p = m1[cur];
            path.push_back(make_pair(p.ny, p.nx));
            int tmp = (p.dir+2) % 4;
            cur = nextState(cur, p.ny+dy[tmp], p.nx+dx[tmp]);
        }
        reverse(path.begin(), path.end());
#ifdef DEBUG
        for (auto el : path) {
            cout << el.first << "  " << el.second << endl;
        }
#endif
    }
    // 次に上2行を揃えることを考える
    {
        // まずpathの言うとおり動かして今のボードがどうなっているのかを確認する
        for (auto el : path) {
            ans.push_back(board.board[el.first][el.second]);
            board.move(el.first, el.second);
        }
#ifdef DEBUG
        board.print();
#endif
        vector<pair<int, int> > path2;
        int first = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                int tmp = board.board[i][j];
                if (tmp == 0 || (5 <= tmp && tmp <= 8)) {
                    if (tmp == 0) first += 4*i+j;
                    else first += p16[tmp-4] * (4*i+j);
                }
            }
        }
        map<int, moveCell> m1; // 0,5,6,7,8の座標の情報がfirst,その直前の動きがsecond
        queue<int> que;
        m1.insert(make_pair(first, moveCell{-1, -1, 0}));
        que.push(first);
        const int goal2 = 4*p16[1] + 5*p16[2] + 6*p16[3] + 7*p16[4];
        int goalmemo = 0;
        // 幅優先探索
        while (!que.empty()) {
            int now = que.front(); que.pop();
            if (isGoal(goal2+8, goal2+15, now)) {
                goalmemo = now;
                break;
            }
            int origin = now & (16-1);
            int y0 = origin / 4, x0 = origin % 4;
            for (int k = 0; k < 4; k++) {
                int ny = y0 + dy[k];
                int nx = x0 + dx[k];
                if (ny <= 0 || ny >= 4 || nx < 0 || nx >= 4) continue;
                int next = nextState(now, ny, nx); // 次の状態
                if (m1.find(next) != m1.end()) continue;
                m1.insert(make_pair(next, moveCell{ny, nx, k}));
                que.push(next);
            }
        }
        // path2を復元
        int cur = goalmemo;
        while (cur != first) {
            auto p = m1[cur];
            path2.push_back(make_pair(p.ny, p.nx));
            int tmp = (p.dir+2) % 4;
            cur = nextState(cur, p.ny+dy[tmp], p.nx+dx[tmp]);
        }
        reverse(path2.begin(), path2.end());
        // pathにpath2を挿入,boardをpath2まで進めた状態にする
        for (auto el : path2) {
            ans.push_back(board.board[el.first][el.second]);
            board.move(el.first, el.second);
        }
        path.insert(path.end(), path2.begin(), path2.end());
#ifdef DEBUG
        for (auto el : path) {
            cout << el.first << "  " << el.second << endl;
        }
        board.print();
#endif
    }
    // 最後に7パズルの要領で全探索する
    {
        array<array<int, 4>, 2> init, target;
        for (int i = 2; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                init[i-2][j] = board.board[i][j];
                target[i-2][j] = i*4+j+1;
            }
        }
        target[1][3] = 0;
        sevenPuzzle sp(init);
        map<sevenPuzzle, moveCell> m1;
        queue<sevenPuzzle> que;
        que.push(sp);
        m1.insert(make_pair(sp, moveCell{-1, -1, 0}));
        while (!que.empty()) {
            sevenPuzzle now = que.front(); que.pop();
            if (ok(now)) break;
            for (int k = 0; k < 4; k++) {
                int ny = now.originY+dy[k];
                int nx = now.originX+dx[k];
                if (ny < 0 || ny >= 2 || nx < 0 || nx >= 4) continue;
                now.move(ny, nx);
                if (m1.find(now) == m1.end()) {
                    m1.insert(make_pair(now, moveCell{ny, nx, k}));
                    que.push(now);
                }
                int tmp = (k+2)%4;
                now.move(ny+dy[tmp], nx+dx[tmp]);
            }
        }
        vector<int> path3;
        sevenPuzzle cur(target);
        int cnt = 0;
        while (cur != sp) {
            auto p = m1[cur];
            int tmp = (p.dir+2)%4;
            path3.push_back(cur.board[p.ny+dy[tmp]][p.nx+dx[tmp]]);
            cur.move(p.ny+dy[tmp], p.nx+dx[tmp]);
        }
        reverse(path3.begin(), path3.end());
        ans.insert(ans.end(), path3.begin(), path3.end());
    }
    // 答えを記述
    for (int el : ans) {
        cout << el << endl;
    }
}
