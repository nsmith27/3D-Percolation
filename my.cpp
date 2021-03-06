// At its core, the following code solves the 3D percolation challenge using the Union-Find algorithm.
// Created by Nathanael Smith, June 2021

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

class Graph {
private:
	int n;
	int nn;
	int nnn;
	int m;
	int position;
	vector<int> V;
	vector<int> W;
	vector<vector<int>> surrounding;

public:
	// Edges are added to class just for convenience... maybe the don't belong!
	vector<int> E1;
	vector<int> E2;
	vector<int> E3;
	vector<int> E4;
	vector<int> E5;
	vector<int> E6;
	vector<int> E7;
	vector<int> E8;
	vector<int> E9;
	vector<int> E10;
	vector<int> E11;
	vector<int> E12;
	//Graph();
	void SetSize(int size) {
		this->n = size;
		this->nn = n * n;
		this->nnn = nn * n;
		this->m = n - 1;
		this->position = 0;
		int nnPn = nn + n;
		int nnMn = nn - n;
		// Set V and W to size
		V.resize(nnn);
		W.resize(nnn);
		// Surrounding_Indexes == Current Index - e??
		// ...See join() within insert()
		// Indexing can be really confusing.  Below is a diagram 
		// representing the pattern of sets of elements surrouding 
		// the current element being read in.  In a 7x7x7 matrix,
		// element 0 corresponds with sL0.  Elements 1-5 corresponds 
		// with sL1, and element 6 corresponds with sL2.  Element 49
		// corresponds with sM0 as does element 98. Elements not on 
		// the lowest level that are on the left edge and not corners
		// correspond with sM3.  Hopefully this is enough to see the
		// pattern.  When reading in a 1, GetSurrounding() will 
		// identify which set s?? corresponds with current index and 
		// will later be used to call join for as many connections 
		// are needed.  
		// sL0 --- --- sL1 --- --- sL2 
		// --- --- --- --- --- --- ---
		// --- --- --- --- --- --- ---
		// sL3 --- --- sL4 --- --- sL5
		// --- --- --- --- --- --- ---
		// --- --- --- --- --- --- ---
		// sL6 --- --- sL7 --- --- sL8
		// #
		// sM0 --- --- sM1 --- --- sM2 
		// --- --- --- --- --- --- ---
		// --- --- --- --- --- --- ---
		// sM3 --- --- sM4 --- --- sM5
		// --- --- --- --- --- --- ---
		// --- --- --- --- --- --- ---
		// sM6 --- --- sM7 --- --- sM8
		// #
		// .
		// .
		// .
		// Consider sM4:
		// eL0  eL1  eL2
		// eL3  eL4  eL5 
		// eL6  eL7  eL8
		// #
		// eM0  eM1  eM2
		// eM3  sM4  --- 
		// ---  ---  ---
		// Therefore, sM4 contains eL0-eL8 + eM0-eM3
		// sL4 only contains eM0-eM3
		int eL0 = nnPn + 1;
		int eL1 = nnPn;
		int eL2 = nnPn - 1;
		int eL3 = nn + 1;
		int eL4 = nn;
		int eL5 = nn - 1;
		int eL6 = nnMn + 1;
		int eL7 = nnMn;
		int eL8 = nnMn - 1;
		int eM0 = n + 1;
		int eM1 = n;
		int eM2 = n - 1;
		int eM3 = 1;
		// Sets based on current position
		vector<int> sL0 = {};
		vector<int> sL1 = { eM3 };
		vector<int> sL2 = { eM3 };
		vector<int> sL3 = { eM1, eM2 };
		vector<int> sL4 = { eM0, eM1, eM2, eM3 };
		vector<int> sL5 = { eM0, eM1, eM3 };
		vector<int> sL6 = { eM1, eM2 };
		vector<int> sL7 = { eM0, eM1, eM2, eM3 };
		vector<int> sL8 = { eM0, eM1, eM3 };
		vector<int> sM0 = { eL4, eL5, eL7, eL8 };
		vector<int> sM1 = { eL3, eL4, eL5, eL6, eL7, eL8, eM3 };
		vector<int> sM2 = { eL3, eL4, eL6, eL7, eM3 };
		vector<int> sM3 = { eL1, eL2, eL4, eL5, eL7, eL8, eM1, eM2 };
		vector<int> sM4 = { eL0, eL1, eL2, eL3, eL4, eL5, eL6, eL7, eL8, eM0, eM1, eM2, eM3 };
		vector<int> sM5 = { eL0, eL1, eL3, eL4, eL6, eL7, eM0, eM1, eM3 };
		vector<int> sM6 = { eL1, eL2, eL4, eL5, eM1, eM2 };
		vector<int> sM7 = { eL0, eL1, eL2, eL3, eL4, eL5, eM0, eM1, eM2, eM3 };
		vector<int> sM8 = { eL0, eL1, eL3, eL4, eM0, eM1, eM3 };
		// Set the surrouding vector
		this->surrounding = { 	sL0, sL1, sL2, sL3, sL4, sL5, sL6, sL7, sL8,
								sM0, sM1, sM2, sM3, sM4, sM5, sM6, sM7, sM8, };
		// Create groups of indexes to edges
		for (int i = 0; i < n; i++) {
			// Bottom 4
			E1.push_back(i);
			E2.push_back(m + n * i);
			E3.push_back(nn - 1 - i);
			E4.push_back(n * i);
			// Middle 4
			E5.push_back(nn * i);
			E6.push_back(m + nn * i);
			E7.push_back(nn - 1 + nn * i);
			E8.push_back(nn - n + nn * i);
			// Top 4
			E9.push_back(nnn - nn + i);
			E10.push_back(nnn - nn + m + n * i);
			E11.push_back(nnn - 1 - i);
			E12.push_back(nnn - nn + n * i);
		}
		return;
	}
	// 1's are positive connectable nodes, 0's are negative not connectable nodes
	void insert(int element) {
		W[position] = 1;
		if (element) {
			V[position] = position;
			int s = GetSurrounding(position);
			for (auto i : surrounding[s]) {
				if (i < nnn) { join(position, position - i); }
			}
		}
		else {
			V[position] = -1;
		}
		position++;
		return;
	}
	// Find correct element in vector<int> surrounding
	int GetSurrounding(int x) {
		int result{ 0 };
		int h = x / nn;
		int r = (x - (h * nn)) / n;
		int c = x % n;

		if (r == 0 && c == 0) { result = 0; }
		else if (r == 0 && c != m) { result = 1; }
		else if (r == 0 && c == m) { result = 2; }
		else if (r != m && c == 0) { result = 3; }
		else if (r != m && c != m) { result = 4; }
		else if (r != m && c == m) { result = 5; }
		else if (r == m && c == 0) { result = 6; }
		else if (r == m && c != m) { result = 7; }
		else if (r == m && c == m) { result = 8; }
		if (h) { result += 9; }

		return result;
	}
	// Set two nodes to the same root
	void join(int a, int b) {
		// cout << a << ", " << b << endl;
		if (V[b] < 0) {
			return;
		}
		int aroot = GetRoot(a);
		int broot = GetRoot(b);
		if (aroot != broot) {
			if (W[aroot] < W[broot]) {
				V[aroot] = broot;
				W[broot] += W[aroot];
			}
			else {
				V[broot] = aroot;
				W[aroot] += W[broot];
			}
		}
		return;
	}
	// Check if two elements are connected
	bool find(int a, int b) {
		if (a < 0 || b < 0 || V[a] < 0 || V[b] < 0) {
			return false;
		}
		return GetRoot(a) == GetRoot(b);
	}
	// Group Find
	int GroupFind(vector<int> &A, vector<int> &B) {
		// Bug prone!  If the roots are ever for some reason changed 
		// while this is running, it may not return 1 as it should... 
		vector<int> urA;
		vector<int> urB;
		GetUnique(A, urA);
		GetUnique(B, urB);
		for (int a : urA) {
			if (std::find(urB.begin(), urB.end(), a) != urB.end()) {
				return 1;
			}
		}
		return 0;
	}
	// Get Unique Roots
	void GetUnique(vector<int> &X, vector<int> &UR) {
		int temp;
		for (int i : X) {
			if (V[i] < 0) {
				continue;
			}
			temp = GetRoot(i);
			if (std::find(UR.begin(), UR.end(), temp) == UR.end()) {
				UR.push_back(temp);
			}
		}
		return;
	}
	// Find root in tree
	int GetRoot(int root) {
		while (V[root] != root) {
			V[root] = V[V[root]];
			root = V[root];
		}
		return root;
	}
	// Print out V
	void vprint() {
		cout << n << endl;
		for (int i = 0; i < V.size(); i++) {
			cout << i << ":\t";
			if (V[i] >= 0) {
				cout << V[i];
			}
			cout << endl;
		}
		return;
	}
	// Print edges
	void eprint() {
		for (int i = 0; i < n; i++) {
			cout << E1[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E2[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E3[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E4[i] << " - ";
		}
		cout << endl;
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E5[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E6[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E7[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E8[i] << " - ";
		}
		cout << endl;
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E9[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E10[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E11[i] << " - ";
		}
		cout << endl;
		for (int i = 0; i < n; i++) {
			cout << E12[i] << " - ";
		}
		cout << endl;
	}

}; // End of class Graph


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void readFile(string path, Graph &G);
void createFile(string &path, int size, double p);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// read data and filter it into Graph object
void readFile(string path, Graph &G) {
	cout << "Reading from:\t" << path << endl;
	string line;
	ifstream myfile(path);
	int size = 0;
	if (myfile.is_open()) {
		int w;
		myfile >> w;
		G.SetSize(w);
		size = w;
		while (getline(myfile, line)) {
			std::istringstream ss(line);
			while (ss >> w) {
				if (w == 0 || w == 1) {
					G.insert(w);
				}
			}
		}
	}
	else {
		cout << "Failure to open file!" << endl;
	}
	myfile.close();
	return;
}
// For testing, create a file a various size and "1-doping" (p) for 3D cube
void createFile(string &path, int size, double p) {
	// Used different path names and didn't want to have to type it out again and again...
	// Although the problem description asked for reading 'data.txt', Thadeus chose 'in.txt'
	// ...so you can thank him for this extra path naming confusion!  jkjk
	path = "created_" + to_string(size) + "_" + to_string(p) + ".txt";
	path = "in.txt";
	std::random_device rd;  // Required to obtain seed from random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	int count = size * size * size;
	int i;
	ofstream myfile(path);
	if (myfile.is_open()) {
		myfile << size << "\n";
		while (count) {
			count--;
			std::bernoulli_distribution rand_elem(p);
			i = rand_elem(gen);
			if (i) {
				myfile << 1 << " ";
			}
			else {
				myfile << 0 << " ";
			}
			if (count % size == 0) {
				myfile << "\n";
			}
			if (count % (size * size) == 0 && count) {
				myfile << "#\n";
			}
		}
	}
	myfile.close();
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	// This instance of size is needed to create the file...represents linear dimension
	// variable p represents the approximate concentration of 1's in created file
	int size = 100;
	double p = 0.12;
	string path = "in.txt";
	createFile(path, size, p);
	
	// Begin timing the solution
	auto t0 = std::chrono::steady_clock::now();
	Graph G;
	readFile(path, G);
	// G.vprint();
	// G.eprint();
	int count = 0;
	if (G.GroupFind(G.E1, G.E11)) {
		cout << "E1 + E11" << endl;
		count++;
	}
	if (G.GroupFind(G.E2, G.E12)) {
		cout << "E2 + E12" << endl;
		count++;
	}
	if (G.GroupFind(G.E3, G.E9)) {
		cout << "E3 + E9" << endl;
		count++;
	}
	if (G.GroupFind(G.E4, G.E10)) {
		cout << "E4 + E10" << endl;
		count++;
	}
	if (G.GroupFind(G.E5, G.E7)) {
		cout << "E5 + E7" << endl;
		count++;
	}
	if (G.GroupFind(G.E6, G.E8)) {
		cout << "E6 + E8" << endl;
		count++;
	}
	cout << count << endl;
	// End time
	double elapsed_time = (std::chrono::steady_clock::now() - t0).count() / 1e9;
	cout << "Total Elapsed Time:\t" << elapsed_time << endl << endl;

	return 0;
}




