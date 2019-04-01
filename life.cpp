/**
 * Simulates John Conway's Game of Life.
 *
 * This is a relatively naive implementation. Grid is treated as a torus (i.e. the edges of the grid
 * wrap around to the opposite edge). A configuration file is required to instantiate the world.
 * The configuration file is provided as the first command-line argument, and should be formatted
 * as follows:
 *
 *      The first line specifies the grid dimensions: <rows> <cols>
 *      The second line lists the number of coordinate pairs that will follow: <n>
 *      The remaining n lines specify which grid cells should be initialized alive: <row> <col>
 *
 * @author Ryan Strauss
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>

using namespace std;

/**
 * Counts the number of living neighbors of a cell.
 *
 * @param board the array representing the board
 * @param r the row of the cell to check
 * @param c the column of the cell to check
 * @param num_rows the number of rows on the board
 * @param num_cols the number of columns on the board
 * @return the number of living neighbors of the cell at (r, c)
 */
int get_alive_neighbors(const bool *board, int r, int c, int num_rows, int num_cols) {
    int alive = 0;
    for (int i = num_rows + r - 1; i <= num_rows + r + 1; ++i) {
        for (int j = num_cols + c - 1; j <= num_cols + c + 1; ++j) {
            if (i % num_rows == r && j % num_cols == c)
                continue;

            alive += board[(i % num_rows) * num_cols + (j % num_cols)];
        }
    }
    return alive;
}

/**
 * Updates the board according to the rules of the game.
 *
 * The rules are as follows:
 *      1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
 *      2. Any live cell with two or three live neighbours lives on to the next generation.
 *      3. Any live cell with more than three live neighbours dies, as if by overpopulation.
 *      4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
 *
 * All cells in the board are updated simultaneously.
 *
 * @param board the array representing the board
 * @param num_rows the number of rows on the board
 * @param num_cols the number of columns on the board
 */
void update_grid(bool *board, int num_rows, int num_cols) {
    vector<pair<int, int>> to_kill, to_spark;
    for (int r = 0; r < num_rows; ++r) {
        for (int c = 0; c < num_cols; ++c) {
            int alive_neighbors = get_alive_neighbors(board, r, c, num_rows, num_cols);

            if (board[r * num_cols + c]) {
                // Check rules 1 and 3
                if (alive_neighbors < 2 || alive_neighbors > 3)
                    to_kill.emplace_back(r, c);
            } else {
                // Check rule 4
                if (alive_neighbors == 3)
                    to_spark.emplace_back(r, c);
            }
        }
    }

    // Make necessary updates to the board
    for (pair<int, int> p : to_kill) {
        board[p.first * num_cols + p.second] = false;
    }
    for (pair<int, int> p : to_spark) {
        board[p.first * num_cols + p.second] = true;
    }
}

/**
 * Prints the game board to the console.
 *
 * @param board the array representing the board
 * @param num_rows the number of rows on the board
 * @param num_cols the number of columns on the board
 */
void print_board(const bool *board, int num_rows, int num_cols) {
    for (int r = 0; r < num_rows; ++r) {
        for (int c = 0; c < num_cols; ++c) {
            if (board[r * num_cols + c])
                cout << "@ ";
            else
                cout << "- ";
        }
        cout << endl;
    }
}

/**
 * Simulates the game for a given number of iterations.
 *
 * @param board the array representing the board
 * @param num_rows the number of rows on the board
 * @param num_cols the number of columns on the board
 * @param iterations the number of iterations to run the simulation
 * @param verbosity the verbosity level of the output
 */
void run_simulation(bool *board, int num_rows, int num_cols, int iterations, int verbosity) {
    for (int i = 0; i < iterations; ++i) {
        update_grid(board, num_rows, num_cols);
        if (verbosity == 2) {
            system("clear");
            print_board(board, num_rows, num_cols);
            usleep(100000);
        } else if (verbosity == 1 && i == iterations - 1) {
            system("clear");
            print_board(board, num_rows, num_cols);
        }
    }
}

int main(int argc, char *argv[]) {
    // Require the proper number of command-line arguments
    if (argc != 4) {
        cout << "Error: three arguments must be provided.\n" << endl;
        cout << "Usage: ./life <filename> <iterations> <verbosity>\n" << endl;
        cout << "       <filename> = path to the configuration file\n"
                "       <iterations> = the number of steps to run the simulation\n"
                "       <verbosity> = 0 (no output),\n"
                "                     1 (final output), or\n"
                "                     2 (animated output)\n" << endl;

        return EXIT_FAILURE;
    }

    // Open the configuration file
    ifstream config(argv[1]);
    if (!config.is_open()) {
        cout << "Could not open file: " << argv[1] << endl;
        return EXIT_FAILURE;
    }
    int num_rows, num_cols;
    int n;
    config >> num_rows >> num_cols >> n;

    // Parse other command line arguments
    int iterations = stoi(argv[2]);
    int verbosity = stoi(argv[3]);
    if (verbosity < 0 || verbosity > 2) {
        cout << "Error: verbosity must be 0, 1, or 2." << endl;
        return EXIT_FAILURE;
    }

    // Initialize the board
    bool *board = new bool[num_rows * num_cols]{false};
    int r, c;
    for (int i = 0; i < n; ++i) {
        config >> r >> c;
        board[r * num_cols + c] = true;
    }
    config.close();

    // Run the simulation
    run_simulation(board, num_rows, num_cols, iterations, verbosity);

    delete[] board;

    return EXIT_SUCCESS;
}