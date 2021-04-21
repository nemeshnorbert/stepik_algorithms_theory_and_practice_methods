#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include <algorithm>

struct Segment {
    int begin;
    int end;
};

struct Point {
    int x;
    bool is_left;
    int segment_id;
};


std::vector<int> get_covering_set(const std::vector<Segment>& segments) {
    auto covering_points = std::vector<int>{};
    auto points = std::vector<Point>{};
    for (auto idx = 0; idx < segments.size(); ++idx) {
        const auto& segment = segments[idx];
        points.emplace_back(Point{segment.begin, true, idx});
        points.emplace_back(Point{segment.end, false, idx});
    }
    std::sort(
        points.begin(), points.end(),
        [](const Point& lhs, const Point& rhs) -> bool {
            return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.is_left);
        }
    );
    auto visited = std::vector<bool>(segments.size(), false);
    auto stack = std::stack<int>{};
    for (const auto& point : points) {
        if (point.is_left) {
            stack.push(point.segment_id);
        } else {
            if (!visited[point.segment_id]) {
                covering_points.emplace_back(point.x);
                while (!stack.empty()) {
                    auto segment_id = stack.top();
                    visited[segment_id] = true;
                    stack.pop();
                }
            }
        }
    }
    return covering_points;
}

int main() {
    auto segments_count = 0;
    std::cin >> segments_count;
    auto segments = std::vector<Segment>(segments_count);
    for (int i = 0; i < segments_count; i++) {
        std::cin >> segments[i].begin >> segments[i].end;
    }

    auto points = get_covering_set(segments);
    std::cout << points.size() << std::endl;
    for (std::size_t i = 0; i < points.size(); i++) {
        std::cout << points[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}