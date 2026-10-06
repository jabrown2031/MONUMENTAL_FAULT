#include <iostream>
#include <string>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>

using namespace std;
using namespace boost::multiprecision;

// Initialize using a string to support massively large semiprimes
const cpp_int TARGET("45113"); 
bool solution_found = false;

// Helper to calculate arbitrary-precision powers of 10
cpp_int pow10_big(int exp) {
    cpp_int res = 1;
    for (int i = 0; i < exp; ++i) res *= 10;
    return res;
}

// DFS Backtracking Algorithm using cpp_int
void find_factors(int depth, cpp_int partial_A, cpp_int partial_B) {
    if (solution_found) return;

    // Base condition: If factors multiply to the exact target
    if (partial_A * partial_B == TARGET && partial_A != 1 && partial_B != 1) {
        cout << "\n[+] Prime Factors Found: " << partial_A << " and " << partial_B << endl;
        solution_found = true;
        return;
    }

    // Dynamic Depth Bounding (Square Root Bound principle)
    int target_length = TARGET.str().length();
    int max_depth = target_length / 2; 
    
    if (depth > max_depth) return;

    cpp_int current_mod = pow10_big(depth + 1);
    cpp_int position_multiplier = pow10_big(depth);

    // 10-ary tree branching for depths 1 and beyond
    for (int a = 0; a <= 9; ++a) {
        for (int b = 0; b <= 9; ++b) {
            
            // Construct candidate factors in arbitrary precision
            cpp_int candidate_A = partial_A + (a * position_multiplier);
            cpp_int candidate_B = partial_B + (b * position_multiplier);

            // Modulus Pruning
            if ((candidate_A * candidate_B) % current_mod == TARGET % current_mod) {
                find_factors(depth + 1, candidate_A, candidate_B);
            }
        }
    }
}

int main() {
    cout << "Initiating Reverse Multiplication Factorization for: " << TARGET << endl;
    cout << "Target length: " << TARGET.str().length() << " digits." << endl;
    cout << "Applying Square Root Bound depth limit: " << TARGET.str().length() / 2 << endl;
    
    // Terminal Digit Determinism (Depth 0 Amputation)
    int terminal_digit = (int)(TARGET % 10);
    vector<pair<int, int>> seed_pairs;

    if (terminal_digit == 1) {
        seed_pairs = {{1, 1}, {3, 7}, {9, 9}};
    } else if (terminal_digit == 3) {
        seed_pairs = {{1, 3}, {7, 9}};
    } else if (terminal_digit == 7) {
        seed_pairs = {{1, 7}, {3, 9}};
    } else if (terminal_digit == 9) {
        seed_pairs = {{1, 9}, {3, 3}, {7, 7}};
    } else {
        cout << "Target must end in 1, 3, 7, or 9 for valid RSA semiprimes." << endl;
        return 1;
    }

    cout << "\nTarget ends in " << terminal_digit << ". Bypassing depth 0." << endl;

    // Launch recursive searches starting directly at depth 1
    for (auto& p : seed_pairs) {
        if (solution_found) break;
        cout << "Evaluating branch for seed pair (" << p.first << ", " << p.second << ")..." << endl;
        find_factors(1, p.first, p.second);
    }

    if (!solution_found) {
        cout << "[-] No factors found. Target is likely prime." << endl;
    }
    return 0;
}