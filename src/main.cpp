#ifdef _WIN32
#include <Windows.h>
#define CLEAR "cls"
#else
#include <unistd.h>
#define CLEAR "clear"
#endif

#include <bits/stdc++.h>

// BUG: Only supports odd numbers
#define X 15
#define Y 15

struct Node {
    int pos;            // Position on the maze
    int parent_pos;     // Position on the parent node
    int path_cost;
    int total_cost;     // Path Cost + Heuristic Cost

    // Comparator
    bool operator()(Node const& a, Node const& b) {
        return a.total_cost > b.total_cost;
    }
};

// Prints the current state of the maze
void print_maze(int* maze) {
    printf("+ ");
    for (int i = 1; i < X; i++) {
        printf("-");
    }
    printf("+\n");

    for (int r = 0; r < Y; r++) {
        printf("|");
        for (int c = 0; c < X; c++) {
            switch (maze[r * X + c]) {
            case 1:
                printf("x");
                break;
            case 2:
                printf(".");
                break;
            default:
                printf(" ");
                break;
            }
        }
        printf("|\n");
    }

    printf("+");
    for (int i = 1; i < X; i++) {
        printf("-");
    }
    printf(" +\n");
}

int main() {
    // Change the current seed of the randomizer
    srand(time(NULL));

    // Get the number of tiles
    int size = X * Y;
    // This is the maze array
    int maze[size];

    // Maze generation
    for (int i = 0; i < size; i++) {
        maze[i] = i & 1 || i / X & 1;
    }

    std::set<int> visited;
    std::stack<int> stack;

    visited.insert(0);
    stack.push(0);

    // DFS algorithm
    int curr, start_direction, direction, temp;
    while (!stack.empty()) {
        curr = stack.top();

        start_direction = rand() % 4; 
        direction = start_direction;

        while (true) {
            // UP
            if (direction == 0 && curr > X && visited.find(temp = curr - X - X) == visited.end()) {
                maze[curr - X] = 0;
                stack.push(temp);
                visited.insert(temp);
                break;
            }
            
            // RIGHT
            if (direction == 1 && curr % X != X - 1 && visited.find(temp = curr + 2) == visited.end()) {
                maze[curr + 1] = 0;
                stack.push(temp);
                visited.insert(temp);
                break;
            }
            
            // DOWN
            if (direction == 2 && curr < X * (Y - 1) && visited.find(temp = curr + X + X) == visited.end()) {
                maze[curr + X] = 0;
                stack.push(temp);
                visited.insert(temp);
                break;
            }
            
            // LEFT
            if (curr % X && visited.find(temp = curr - 2) == visited.end()) {
                maze[curr - 1] = 0;
                stack.push(temp);
                visited.insert(temp);
                break;
            }
            
            direction = (direction + 1) % 4;
            if (direction == start_direction) {
                stack.pop();
                break;
            }
        }
    }

    // Remove X + Y / 4 walls so that there should
    // would be at least multiple paths to the end
    int removed = (X + Y) >> 2;
    int x = X / 2 + 1;
    int y = Y / 2 + 1;
    while (removed) {
        temp = rand() % x * (X + X) + rand() % y + 1;

        while (!maze[temp])
            temp = (temp + 2) % size; // remove the next one instead

        maze[temp] = 0;
        removed--;
    }

    // A* Algorithm
    int start = 0;
    int end = X * Y - 1;

    std::priority_queue<Node, std::vector<Node>, Node> queue;
    std::map<int, Node> map;

    Node node, new_node;
    node.pos = start;
    node.parent_pos = -1;
    node.path_cost = 0;
    node.total_cost = 0;

    visited.clear();
    queue.push(node);
    bool is_path = false;
    int path_cost;

    while (!queue.empty()) {
        // Get the current node
        node = queue.top();
        queue.pop();

        curr = node.pos;
        visited.insert(curr);

        map[curr] = node;
        // get the path cost for the next nodes
        path_cost = node.path_cost + 1;

        // Check if current node is the answer
        if (curr == end) {
            is_path = true;
            break;
        }

        // Check all directions(Up, Right, Down, Left)
        // Check if whether the tile is within bounds of the maze
        // and if the tile is not a wall
        // and if the tile is not yet visited
        // UP
        if (curr > X && !maze[temp = curr - X] && visited.find(temp) == visited.end()) {
            new_node.pos = temp;
            new_node.parent_pos = curr;

            x = temp / X;
            y = temp % X;

            new_node.path_cost = path_cost;
            new_node.total_cost = X - x + Y - y + path_cost - 2;

            queue.push(new_node);
        }

        // RIGHT
        if (curr % X != X - 1 && !maze[temp = curr + 1] && visited.find(temp) == visited.end()) {
            new_node.pos = temp;
            new_node.parent_pos = curr;

            x = temp / X;
            y = temp % X;

            new_node.path_cost = path_cost;
            new_node.total_cost = X - x + Y - y + path_cost - 2;

            queue.push(new_node);
        }

        // DOWN
        if (curr < X * (Y - 1) && !maze[temp = curr + X] && visited.find(temp) == visited.end()) {
            new_node.pos = temp;
            new_node.parent_pos = curr;

            x = temp / X;
            y = temp % X;

            new_node.path_cost = path_cost;
            new_node.total_cost = X - x + Y - y + path_cost - 2;

            queue.push(new_node);
        }
        
        // LEFT
        if (curr % X && !maze[temp = curr - 1] && visited.find(temp) == visited.end()) {
            new_node.pos = temp;
            new_node.parent_pos = curr;

            x = temp / X;
            y = temp % X;

            new_node.path_cost = path_cost;
            new_node.total_cost = X - x + Y - y + path_cost - 2;

            queue.push(new_node);
        }
    }

    if (!is_path) {
        printf("No Path Found\n");
        return 0;
    }

    // Trace the path
    std::vector<int> path;

    curr = end;
    while (curr != -1) {
        path.push_back(map[curr].pos);
        curr = map[curr].parent_pos;
    }

    for (int i = path.size(); i > -1; i--) {
        system(CLEAR);      // Clears the screen
        maze[path[i]] = 2;
        print_maze(maze);
        usleep(500000);     // Delay of 0.5 ms
    }
}