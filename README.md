# Inversion Counter & Simplex Solver  

## 📌 Overview  
This project implements two classic algorithms in C++:  

1. **Counting Inversions** – using a divide-and-conquer strategy based on merge sort.  
2. **Simplex Algorithm** – solving linear programming (LP) problems with slack variables to maximize an objective function.  
---

## 🔹 Counting Inversions  
**Problem:**  
Given an array of distinct integers, count the number of *inversions* (pairs `(i, j)` such that `i < j` and `A[i] > A[j]`).  

**Approach:**  
- Uses a modified merge sort.  
- While merging, if an element from the right half is smaller than one from the left, all remaining left elements count as inversions.  

**Complexity:**  
- Time: `O(n log n)`  
- Space: `O(n)`  

**Output:**  
- Total number of inversions.  
- Sorted version of the array.  

---

## 🔹 Simplex Method for Linear Programming  
**Problem:**  
Solve a linear program in standard form:  

\[
\text{maximize } Cx \quad \text{subject to } Ax \leq B, \; x \geq 0
\]  

**Approach:**  
- Builds a tableau with slack variables.  
- Iteratively selects an **entering variable** (most negative reduced cost) and a **leaving variable** (minimum ratio test).  
- Performs pivoting until optimality is reached.  

**Complexity:**  
- Efficient in practice (though worst-case exponential).  

**Output:**  
- Optimal solution vector `x`  
- Slack variables  
- Optimal objective value  

---

