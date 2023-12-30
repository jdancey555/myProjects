#include <iostream>
#include <array>
#include <sstream>
#include <string>
#include <list>
#include <iterator>
#include <vector>
#include <algorithm>
#include <time.h>
#include <random>
using namespace std;

// Node for each square on the grid
struct myNode {
    int num;        
    int options[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9}; // remaining possibile values for the square
    int length;                                   // length of options
    int id;
    int num_aft_var = 20;                         // Number of other squares this square would affect (doesn't affect already determined squares)
};

// Compare operator for Nodes based on ID
bool operator==(const myNode& lhs, const myNode& rhs) {         
    if (lhs.id == rhs.id) {
        return true;
    } else {
        return false;
    }
}

// Function to find the Least constraining Value to assign to the current node
int Find_LCV(struct myNode *grid, myNode currNode, vector <int> usedVals) {
    const int Col_num = currNode.id % 9;
    const int Row_num = currNode.id / 9;
    const int Col_gridnum = Col_num / 3;
    const int Row_gridnum = Row_num / 3;
    int grid_val = Row_gridnum * 27 + Col_gridnum * 3;
    int len = currNode.length;
    int curr_min = 100;
    int curr_val = 1;
    vector <int> min_vals;
    int values[9] = {0};
    bool value_used;

    // Sums throughout all grid squares that could be impacted by the current node, as well as all possible values
    for (int i = 0; i < 9; i++) {
        
        if (i % 3 == 0 && i != 0) {
            grid_val += 6;
        }

        for (int j = 0; j < 9; j++) {
            
            if (currNode.options[j] <= 0) {
                continue;
            }
            if (Col_num == i) {
                continue;
            } else {
                if (grid[currNode.id - Col_num + i].options[j] == j+1) {
                    values[j]++;
                }
            }
            if (Row_num == i) {
                continue;
            } else {
                if (grid[currNode.id - (Row_num * 9) + (i * 9)].options[j] == j+1) {
                    values[j]++;
                }
            }
            if (grid_val == i) {
                continue;
            } else {
                if (grid[grid_val].options[j] == j+1) {
                    values[j]++;
                }
            }
        }        
        grid_val += 1;
    }

    //Determines least constraining value
    for (int i = 0; i < 9; i++) {
        value_used = false;
        if ((values[i] != 0 || currNode.options[i] > 0) && values[i] <= curr_min) {
            for (int j = 0; j < usedVals.size(); j++) {
                if (usedVals[j] == i + 1) {
                    value_used = true;
                    break;
                }
            }
            if (!value_used) {
                if (values[i] == curr_min) {
                    min_vals.push_back(i+1);
                } else {
                    min_vals.clear();
                    min_vals.push_back(i+1);
                }
            }
        }
    }
    //If multiple LCV's, randomizes the list and selects the first. 
    unsigned seed = clock();
    shuffle(min_vals.begin(), min_vals.end(), default_random_engine(seed));
      return min_vals[0]; 
}

//Function to find Most Contraining Variable 
int Find_MCV(vector <myNode> list, struct myNode *grid) {
    int len = list.size();
    myNode tempNode = list[0];
    bool checked = false;
    vector <int> max_vals;

    // Iterates through list of all variables that have been determnied to be the most constrained
    for (int i = 0; i < len; i++) {

        if (list[i].num_aft_var >= tempNode.num_aft_var && grid[list[i].id].num == 0) {
            if (list[i].num_aft_var == tempNode.num_aft_var) {
                max_vals.push_back(list[i].id);
            } else {
                max_vals.clear();
                max_vals.push_back(list[i].id);
            }
            tempNode = list[i];
            checked = true;
        }
    }
    if (!checked) {
        return -1;
    }
    //If more than one MCV, shuffles list and selects the first variable
    unsigned seed = clock();
    shuffle(max_vals.begin(), max_vals.end(), default_random_engine(seed));
    //return nodeID
    return max_vals[0];
}

//Function that rearranges the C_Grid Array, which determines which variables are the most/least constrained
void Rearrage(vector <myNode> *C_Grid, myNode val, int location, int new_location) {
    int len = C_Grid[location].size();

    // Moves node to new location depending if they gained a new option or lost one
    for (int i = 0; i < len; i++) {
        if (C_Grid[location][i] == val) {
            C_Grid[new_location].push_back(C_Grid[location][i]);
            C_Grid[location].erase(C_Grid[location].begin() + i);
        }
    }
}

//Function used by C_Solve and B_Solve to update all the values options of affected variables 
struct myNode Update_Options(struct myNode *grid, int curr_square, int new_val, int old_val, int newlength, vector <myNode> *C_Grid) {
    const int Col_num = curr_square % 9;
    const int Row_num = curr_square / 9;
    const int Col_gridnum = Col_num / 3;
    const int Row_gridnum = Row_num / 3;
    int grid_val = Row_gridnum * 27 + Col_gridnum * 3;
    int temp_len = 0;

    //Iterates though all affected variables
    for (int i = 0; i < 9; i++) {

        if (i % 3 == 0 && i != 0) {
            grid_val += 6;
        }
        // Updates values that are losing options
        if (new_val == 0) {
            if (grid[curr_square - Col_num + i].options[old_val] > 0) {     //Updates Columns that are previously unaffected by this specific value
                grid[curr_square - Col_num + i].options[old_val] = new_val;
                grid[curr_square - Col_num + i].length += newlength;
                grid[curr_square - Col_num + i].num_aft_var--; 
                Rearrage(C_Grid, grid[curr_square - Col_num + i], grid[curr_square - Col_num + i].length + 1, grid[curr_square - Col_num + i].length);
            } else {    
                grid[curr_square - Col_num + i].options[old_val]--;     // Updates Columns previously affected by this specific value
                grid[curr_square - Col_num + i].num_aft_var--;
            }
            if (grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val] > 0) {     // Updates Rows previously unaffected by this specific value
                grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val] = new_val;
                grid[curr_square - (Row_num * 9) + (i * 9)].length += newlength;
                grid[curr_square - (Row_num * 9) + (i * 9)].num_aft_var--;
                Rearrage(C_Grid, grid[curr_square - (Row_num * 9) + (i * 9)], grid[curr_square - (Row_num * 9) + (i * 9)].length + 1, grid[curr_square - (Row_num * 9) + (i * 9)].length);
            } else {
                grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val]--;     // Updates Rows previously affected by this specific value
                grid[curr_square - (Row_num * 9) + (i * 9)].num_aft_var--;
            }
            if (grid[grid_val].options[old_val] > 0) {      // Updates 3x3 Grids previously unaffected by this specific value
            grid[grid_val].options[old_val] = new_val;
            grid[grid_val].length += newlength;
            grid[grid_val].num_aft_var--;
            Rearrage(C_Grid, grid[grid_val], grid[grid_val].length + 1, grid[grid_val].length);
            
            } else {
                grid[grid_val].options[old_val]--;      // Updates 3x3 grids previously affected by this specific value
                grid[grid_val].num_aft_var--;
            }

        } else {    // Updates options that are gaining options
            if (grid[curr_square - Col_num + i].options[old_val] >= 0) {        //Updates Columns that are previously unaffected by this specific value
                grid[curr_square - Col_num + i].options[old_val] = new_val;
                grid[curr_square - Col_num + i].length += newlength;
                grid[curr_square - Col_num + i].num_aft_var++;
                Rearrage(C_Grid, grid[curr_square - Col_num + i], grid[curr_square - Col_num + i].length - 1, grid[curr_square - Col_num + i].length);
            } else {
                grid[curr_square - Col_num + i].options[old_val]++;     //Updates Columns that are previously affected by this specific value
                grid[curr_square - Col_num + i].num_aft_var++;
            }
            if (grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val] >= 0) {        //Updates Rows that are previously unaffected by this specific value
                grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val] = new_val;
                grid[curr_square - (Row_num * 9) + (i * 9)].length += newlength;
                grid[curr_square - (Row_num * 9) + (i * 9)].num_aft_var++;
                Rearrage(C_Grid, grid[curr_square - (Row_num * 9) + (i * 9)], grid[curr_square - (Row_num * 9) + (i * 9)].length - 1, grid[curr_square - (Row_num * 9) + (i * 9)].length);
            } else {
                grid[curr_square - (Row_num * 9) + (i * 9)].options[old_val]++;     //Updates Rows that are previously affected by this specific value
                grid[curr_square - (Row_num * 9) + (i * 9)].num_aft_var++;
            }

           if (grid[grid_val].options[old_val] >= 0) {  //Updates 3x3 grid that are previously unffected by this specific value
            grid[grid_val].options[old_val] = new_val;
            grid[grid_val].length += newlength;
            grid[grid_val].num_aft_var++;
            Rearrage(C_Grid, grid[grid_val], grid[grid_val].length - 1, grid[grid_val].length);
            } else {
                grid[grid_val].options[old_val]++;      //Updates 3x3 grid that are previously affected by this specific value
                grid[grid_val].num_aft_var++;
            }
        }
        grid_val += 1;
    }
    return grid[curr_square];
}

// Used by A_Solve 
bool Check_Constraints(struct myNode *grid, int curr_square) {
    const int Col_num = curr_square % 9;
    const int Row_num = curr_square / 9;
    const int Col_gridnum = Col_num / 3;
    const int Row_gridnum = Row_num / 3;
    int grid_val = Row_gridnum * 27 + Col_gridnum * 3;

    // Iterates through all variables that affect the current node to determine if the chosen value is permissable 
    for (int i = 0; i < 9; i++) {
        
        if (i % 3 == 0 && i != 0) {
            grid_val += 6;
        }

        if (grid[curr_square - Col_num + i].num == grid[curr_square].num && Col_num != i) {         //Checks columns
            return false;
        }
        if (grid[curr_square - (Row_num * 9) + (i * 9)].num == grid[curr_square].num && Row_num != i) { // Checks Rows
            return false;
        }
        
        if (grid[grid_val].num == grid[curr_square].num && grid_val != curr_square) {   //Checks 3x3 Grid
            return false;
        }
        grid_val += 1;
    }
    return true; 
}

// Main Algorithm for C_Solve, utilize Backtracking, Forward checking, and MRV, MCV, and LCV Heuristics
bool C_Solve(struct myNode *grid, vector <myNode> *C_Grid, int count, int *node_count) {
    int currNum;
    int newVal;
    vector <int> usedVals;

    if (count == 81) {
        return true;
    }

    for (int i = 0; i < 10; i++) {  // Iterates through C_Grid to determine ideal variable with MRV
        int len = C_Grid[i].size();
        if (i == 0) {       // Determines if any unassigned variables have no options left, returns false if so, forces parent to reselect
            for (int j = 0; j < len; j++) {
                if (grid[C_Grid[i][j].id].num == 0) {
                    return false;
                }
            }
        }
        if (len == 1 && grid[C_Grid[i][0].id].num == 0) {   //If only 1 variable with i remaining options, immediatly selects
            currNum = C_Grid[i][0].id;
            break;
        } else if (len > 1) {
            currNum = Find_MCV(C_Grid[i], grid);        // If more than 1 variable with i remaining options, Runs Find_MCV to find ideal candidat
            if (currNum == -1) {
                continue;               // If currNum is -1, option is not usable, continue looking
            } else {
                break;
            }
        }
    }
    for (int i = 0; i < grid[currNum].length; i++) {        // Iterates through possible options to determine ideal value for the variable
        *node_count += 1;
        newVal = Find_LCV(grid, grid[currNum], usedVals);
        grid[currNum].num = newVal;
        Update_Options(grid, currNum, 0, newVal - 1, -1, C_Grid);       // Updates affected variables with this choice

        if (C_Solve(grid, C_Grid, count + 1, node_count)) {                 // Checks that Children Nodes aren't affected negativly before permanently assigning
            return true;
        }
        Update_Options(grid, currNum, newVal, newVal - 1, 1, C_Grid);   // If Children Nodes are left with no options, update affect variables back to normal
        usedVals.push_back(newVal);                                     // Agknowledge previously used value
        grid[currNum].num = 0;                                          // Set number back to zero, continue looping until ideal value is found
    }

    return false;                       // If no number is found, return false, let parent node change to allow for better options.
}

// Main Algorithm for B_Solve, using Backward search and Forward Checking
bool B_Solve(struct myNode *grid, int curr_square, vector <myNode> *C_Grid, int *count, vector <int> queue) {
    int val = 0;
    int currx = 0;
    vector <int> ran_var;
    for (int i = 1; i < 10; i++) {      
        ran_var.push_back(i);
    }
    unsigned seed = clock();
    shuffle(ran_var.begin(), ran_var.end(), default_random_engine(seed));  // Randomly decides order we search through variables

    if (curr_square == 81) {        // If searched through 81 squares, return solved
        return true;
    } else if (grid[queue[curr_square]].num != 0) {
        return B_Solve(grid, curr_square + 1, C_Grid, count, queue);        //If number already assigned, continue to next square. 
    }

    int length = grid[queue[curr_square]].length;

    for (int i = 0; i < length; i++) {  // Iterates through all options
        if (currx == 9) {
            break;
        }
        for (int x = currx; x < 9; x++) {
            if (grid[queue[curr_square]].options[ran_var[x]-1] > 0) {       // Randomly decides which value for the variable to select next
                val = grid[queue[curr_square]].options[ran_var[x]-1];
                currx = x;
                break;
            }
        }
        grid[queue[curr_square]].num = val;       
        Update_Options(grid, queue[curr_square], 0, val - 1, -1 , C_Grid);      // Updates affect variables
        *count += 1;
        if (B_Solve(grid, curr_square + 1, C_Grid, count, queue)) {     // Checks if Children nodes are negativly affected, returns true if not
            return true; 
        }
        Update_Options(grid, queue[curr_square], val, val - 1, 1, C_Grid);      // Adjusts affect variables, sets number to zero, if children are negativly affected
        grid[queue[curr_square]].num = 0;       
        currx++;        // increases counter to move on to next random value
    }
    return false;       // returns false if no reasonable options, shows a need to adjust parents nodes
}

// Main value for A_Solve
bool A_Solve(struct myNode *grid, int curr_square, int *count, vector <int> queue) {
    vector <int> ran_var;
    for (int i = 1; i < 10; i++) {
        ran_var.push_back(i);
    }
    unsigned seed = clock();
    shuffle(ran_var.begin(), ran_var.end(), default_random_engine(seed));       // Randomizes options we choose to check next

    if (curr_square == 81) {        // Checks if all values have been assigned
        return true;
    } else if (grid[queue[curr_square]].num != 0) {
        return A_Solve(grid, curr_square + 1, count, queue);        // If variable already assigned, move to next one
    }

    for (int i = 0; i < 9; i++) {
        grid[queue[curr_square]].num = ran_var[i];      // Assigned randomly chosen value option
        if (Check_Constraints(grid, queue[curr_square])) {      // Checks if it breaks any constraints
            *count += 1;
            if (A_Solve(grid, curr_square + 1, count, queue)) {     // Checks if Children Nodes are negativly affected
                return true;
            }
        }
        if (i == 8) {
            grid[queue[curr_square]].num = 0;       // If no options work, then reset variable, and change parent Nodes
            return false;
        }
    }
    return false;
}

int main() {
    
    string temp;
    myNode grid[81];
    int tempGrid[81];
    vector <myNode> C_Grid[10];         // Array of vectors for each constrained value of variable
    vector <myNode> tempCGrid[10];
    int count = 0;
    int node_count = 0;
    node_count++;
    vector <int> queue;
    int sumTotal = 0;
    double Time = 0;
    char function;
    int numNodes[50];
    double numTime[50];

    cin >> function;

    for (int i = 0; i < 81; i++) {
        queue.push_back(i);
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {       // Reads in sudoku square
            cin >> temp;
            istringstream str(temp);
            str >> tempGrid[j + (i * 9)];
        }
    }

    for (int z = 0; z < 10; z++) {
        cout << z << endl;
    clock_t start = clock();
    unsigned seed = clock();
    shuffle(queue.begin(), queue.end(), default_random_engine(seed));   // Random order of which variables to check next

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {       // Assigns values to Grid and C_Grid
            myNode tempNode;
            tempNode.num = tempGrid[j + (i * 9)];
            tempNode.length = 9;
            tempNode.id = j + (i * 9);
            grid[j + (i * 9)] = tempNode;
            C_Grid[9].push_back(tempNode);
        }
    }

    for (int i = 0; i < 81; i++) {      // Updates necessary nodes where values have been already assigned
        if (grid[i].num != 0) {
            grid[i] = Update_Options(grid, i, 0, grid[i].num - 1, -1, C_Grid);
            count++;
        }
    }

    if (function == 'A') {
        A_Solve(grid, 0, &node_count, queue);
    } else if (function == 'B') {
        B_Solve(grid, 0, C_Grid, &node_count, queue);
    } else if (function == 'C') {
        C_Solve(grid, C_Grid, count, &node_count);
    } else {
        cout << "Please input A, B, or C" << endl;
        return 0;
    }
   

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {       // prints result
            cout << grid[i*9 + j].num;
        }
        cout << endl;
    }

    cout << node_count << endl;
    sumTotal += node_count;
    numNodes[z] = node_count;
    node_count = 0;
    for (int i = 0; i < 10; i++) {
        C_Grid[i].clear();
    }
    count = 0;
    clock_t end = clock();

    double tempTime = double(end - start)/CLOCKS_PER_SEC;;
    Time += tempTime;
    numTime[z] = tempTime;
    cout << tempTime << endl;
    tempTime = 0;

    }
    
    int avgNode = sumTotal / 10;
    double avgTime = Time / 10;
    int stdDevNode = 0;
    double stdDevTime = 0;

    for (int z = 0; z < 10; z++) {
        numNodes[z] -= avgNode;
        numNodes[z] = pow(numNodes[z], 2);
        stdDevNode += numNodes[z];

        numTime[z] -= avgTime;
        numTime[z] = pow(numTime[z], 2);
        stdDevTime += numTime[z];
    }


    printf(" AverageTime Elapsed: %.5f seconds\n", avgTime);      // Print Node Count / Time to complete
    cout << "Node count average: " << avgNode << endl;
    cout << "Standard Deviation of Nodes: " << sqrt(stdDevNode/49) << endl;
    printf("Standard Deviation of time: %.5f seconds\n", sqrt(stdDevTime/49)); 
}
