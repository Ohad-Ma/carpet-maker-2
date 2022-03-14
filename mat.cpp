#include "mat.hpp"

/**
 * @brief 
 * The algorithm is based on the J language solution shown in https://codegolf.stackexchange.com/questions/241219/mat-printing-matrix
 * a) Let be 3 matrices - mat_a, mat_b and minimal of both (let's call it minimal_mat)
 * b) fill mat_a with 0's and make cell[i][j] be the minimum of the cell behind, above and vertical plus 1 so we create a minimum function table (look a bit like the integer knapsack problem).
 *    # for example we have 7 5 matrix sized then the minimum function table would look like:
 * 
 *          0 0 0 0 0
 *          0 1 1 1 1
 *          0 1 2 2 2
 *          0 1 2 3 3
 *          0 1 2 3 4
 *          0 1 2 3 4
 *          0 1 2 3 4
 * 
 * c) mat_b - now we have to do multiple manipulations so we have a transponse kinda matrix which should look like:
 *               
 *               4 3 2 1 0
 *               4 3 2 1 0
 *               4 3 2 1 0
 *               3 3 2 1 0
 *               2 2 2 1 0
 *               1 1 1 1 0
 *               0 0 0 0 0
 *         
 * d) minimal_mat - we compare both matrices and put in every cell of minimal_mat the minimum value of each cell of mat_a & mat_b so we get something like:
 * 
 *              0 0 0 0 0
 *              0 1 1 1 0
 *              0 1 2 1 0
 *              0 1 2 1 0
 *              0 1 2 1 0
 *              0 1 1 1 0
 *              0 0 0 0 0
 * 
 * e) Then we mod 2 every cell and all the middle numbers should turn 0's
 * 
 * f) All left is to concatenate and recieve a matrix alike (or a beautiful carpet).
 *    So let us 'walk over' minimal_matrix and now:
 *      wherever we face 0 :
 *              we concatenate first given input char (symb_a in my case)
 *      whenever we face 1 :
 *              we concatenate second given input char (symb_b in my case)
 * 
 * g) returns string (or "carpet" in my case) 
 */
namespace ariel {
   std::string mat(int COLS, int ROWS, char symb_a, char symb_b) {
      int found = findSymb(symb_a); // Finds bad char '\r', '\n' etc.. 
      int found2 = findSymb(symb_b); // Finds bad char '\r', '\n' etc.. 
      std::string carpet; // The returned string

      // Base cases :
      if ((found == 1 || found2 == 1) || (found == 1 && found2 == 1)) { // symb_a/symb_b contains forbidden chars / magic numbers (the ascii ones below 33 and above 126)
         throw std::invalid_argument("invalid special character");
      }
      if ((ROWS < 1 || COLS < 1) || (ROWS < 1 && COLS < 1)) { // ROWS/COLS are 0 or negative 
         throw std::invalid_argument("negative values");
      }
      if ((ROWS % 2 == 0 || COLS % 2 == 0) || (ROWS % 2 == 0 && COLS % 2 == 0)) { // ROWS/COLS are even numbers
         throw std::invalid_argument("ROLS & COLS has to be odd numbers");
      }

      // Init 3 matrices (vector type integer) 
      std::vector < std::vector <int>> mat_a(ROWS, std::vector <int> (COLS, 0)); 
      std::vector < std::vector <int>> mat_b(ROWS, std::vector <int> (COLS, 0));
      std::vector < std::vector <int>> minimal_mat(ROWS, std::vector <int> (COLS, 0)); // Minimum cell values of both matrices above

      // Similar to knapsack 1-0 problem we take the minimum of the cell behind, above and vertical and add 1 
      for (int i = 1; i < ROWS; ++i) {
         for (int j = 1; j < COLS; ++j) {
            mat_a[i][j] = std::min(std::min(mat_a[i - 1][j - 1], mat_a[i][j - 1]), mat_a[i - 1][j]) + 1;
         }
      }

      // Reverse every vector in mat_a and place it in mat_b
      reverse_copy(mat_a.begin(), mat_a.end(), mat_b.begin());

      // Swap the vectors (swap(0,n),swap(1,n-1),swap(2,n-2)..... and so on)
      for (int i = COLS - 1, j = 0; i > j; --i, j++) {
         for (auto & v: mat_b) {
            std::swap(v[i], v[j]);
         }
      }
      // Every cell i,j in minimal_mat would be the minimal cell i,j in mat_a,mat_b (both matrices above)   
      for (int i = 0; i < ROWS; ++i) {
         for (int j = 0; j < COLS; ++j) {
            minimal_mat[i][j] = std::min(mat_a[i][j], mat_b[i][j]);
         }
      }
     
      // %2 every cell in the minimal_mat so it's easy to know where to place first symbol and second symbol 
      for (int i = 0; i < ROWS; ++i) {
         for (int j = 0; j < COLS; ++j) {
            minimal_mat[i][j] %= 2;
         }
      }
      // If at i,j in the minimal_mat 0 is found concatenate first char (or symbol_a) to carpet or second char to carpet if 1 is found 
      for (int i = 0; i < ROWS; ++i) {
         for (int j = 0; j < COLS; ++j) {
            minimal_mat[i][j] == 0 ? carpet += symb_a : carpet += symb_b;

         }
         carpet += '\n'; // To look like a matrix
      }
        // Clearing leftover memory
        mat_a.clear();
        mat_b.clear();
        minimal_mat.clear();
      return carpet;

   }
   /**
    * @brief finds if the input of the char contains forbidden character or any character which is not special aka magic numbers
    * 
    * @param c 
    * @return 1 - found, 0 - haven't found 
    */
   int findSymb(char c) {
      // both ch_1, ch_2 has no usage besides this function so no need to put it outside
      const char ch_1 = 127; 
      const char ch_2 = 33;
      std::vector < char > forbidden_chars = {
         '\n',
         '\r',
         '\t',
         '\0',
         ' '
      };
      std::vector < char > ::iterator it; // for searching in 
      it = std::find(forbidden_chars.begin(), forbidden_chars.end(), c);
      if (c >= ch_1 || c < ch_2) { // magic numbers found
         return 1;
      }

      return it != forbidden_chars.end() ? 1 : 0; // if any of the forbidden chars are found returns 1 else 0
   }
}