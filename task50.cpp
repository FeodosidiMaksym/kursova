 /*  ----------------------------------------------------------------<Header>
Name:       task50.cpp
Title:      Numbers connector.
Group:      TV-41
Student:    Feodosidi M.K.
Written:    2025-06-22
Revised:    2025-06-22
Description: Write an algorithm for solving the problem and implement it as
a program in ANSI C++. Draw several separate segments, each of which connects
a pair of identical numbers. Each cell can contain only one line, and lines
can only pass horizontally or vertically between the centers of the cells.
-------------------------------------------------------------------<Header>  */
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <windows.h>



using namespace std;

const int EMPTY = -1;

struct Point {
    int x, y;
};

struct Path {
    vector<Point> points;
};

int rows, cols;
vector<vector<int>> grid;
vector<vector<char>> visualGrid;
set<pair<int, int>> occupied;

bool isValid(int x, int y) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

bool bfs(Point start, Point end, Path &path) {
    queue<vector<Point>> q;
    set<pair<int, int>> visited;
    q.push({start});
    visited.insert({start.x, start.y});

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while (!q.empty()) {
        auto currPath = q.front(); q.pop();
        Point last = currPath.back();

        if (last.x == end.x && last.y == end.y) {
            path.points = currPath;
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = last.x + dx[i], ny = last.y + dy[i];
            if (isValid(nx, ny) && !visited.count({nx, ny}) && (grid[nx][ny] == EMPTY || (nx == end.x && ny == end.y))) {
                if (occupied.count({nx, ny}) && !(nx == end.x && ny == end.y)) continue;

                visited.insert({nx, ny});
                auto nextPath = currPath;
                nextPath.push_back({nx, ny});
                q.push(nextPath);
            }
        }
    }
    return false;
}

/*  ---------------------------------------------------------------------[<]
   Function: drawPath
   Synopsis: Creates lines used to connect numbers.
   ---------------------------------------------------------------------[>]-*/
void drawPath(const Path& path) {
    for (size_t i = 1; i + 1 < path.points.size(); ++i) {
        Point p = path.points[i];
        occupied.insert({p.x, p.y});
        visualGrid[p.x][p.y] = '*';
    }
}

/*  ---------------------------------------------------------------------[<]
   Function: printVisualGrid
   Synopsis: Creates visualisation for solved problem.
   ---------------------------------------------------------------------[>]-*/
void printVisualGrid() {
    cout << "\nРезультат:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] != EMPTY)
                cout << grid[i][j] << ' ';
            else
                cout << visualGrid[i][j] << ' ';
        }
        cout << '\n';
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    cout << "Введіть розмірність поля (рядки і стовпці): ";
    cin >> rows >> cols;

    grid.assign(rows, vector<int>(cols, EMPTY));
    visualGrid.assign(rows, vector<char>(cols, '.'));

    int count;
    cout << "Скільки чисел ви хочете ввести? ";
    cin >> count;

    map<int, vector<Point>> numberPositions;

    for (int i = 0; i < count; ++i) {
        int x, y, val;
        cout << "Введіть координати (рядок, стовпець) і значення: ";
        cin >> x >> y >> val;
        if (!isValid(x, y)) {
            cout << "Невірні координати!\n";
            --i;
            continue;
        }
        grid[x][y] = val;
        visualGrid[x][y] = '0' + val;
        numberPositions[val].push_back({x, y});
    }

    for (auto &[val, points] : numberPositions) {
        if (points.size() < 2) continue;

        for (size_t i = 0; i + 1 < points.size(); i += 2) {
            Path path;
            if (bfs(points[i], points[i + 1], path)) {
                drawPath(path);
            } else {
                cout << "Не вдалося з'єднати " << val << " в точках (" << points[i].x << "," << points[i].y << ") і ("
                     << points[i + 1].x << "," << points[i + 1].y << ")\n";
            }
        }
    }

    printVisualGrid();
    return 0;
}
