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
	int m;
	int nnn;
	int position;
	vector<int> V;
	vector<int> W;
	vector<vector<int>> surrounding;

public:
	//Graph();
	void SetSize(int size) {
		// s0 -- -- s1 -- -- s2 
		// -- -- -- -- -- -- --
		// -- -- -- -- -- -- --
		// s3 -- -- s4 -- -- s5
		// -- -- -- -- -- -- --
		// -- -- -- -- -- -- --
		// s6 -- -- s7 -- -- s8

		// e0 e1 e2
		// e3 s? e5 
		// e6 e7 e8

		this->n = size;
		this->m = n - 1; 
		this->nnn = n * n * n;
		this->position = 0;
		int nn = n * n;
		int nnPn = nn + n;
		int nnMn = nn - n;

		// Surrounding Indexes 
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
		vector<int> sU0 = { eL4, eL5, eL7, eL8 };
		vector<int> sU1 = { eL3, eL4, eL5, eL6, eL7, eL8, eM3 };
		vector<int> sU2 = { eL3, eL4, eL6, eL7, eM3 };
		vector<int> sU3 = { eL1, eL2, eL4, eL5, eL7, eL8, eM1, eM2 };
		vector<int> sU4 = { eL0, eL1, eL2, eL3, eL4, eL5, eL6, eL7, eL8, eM0, eM1, eM2, eM3 };
		vector<int> sU5 = { eL0, eL1, eL3, eL4, eL6, eL7, eM0, eM1, eM3 };
		vector<int> sU6 = { eL1, eL2, eL4, eL5, eM1, eM2 };
		vector<int> sU7 = { eL0, eL1, eL2, eL3, eL4, eL5, eM0, eM1, eM2, eM3 };
		vector<int> sU8 = { eL0, eL1, eL3, eL4, eM0, eM1, eM3 };

		// Artificial Graph points for edges
		int edge1A = nnn;
		int edge1B = edge1A + 1;
		int edge2A = edge1B + 1;
		int edge2B = edge2A + 1;
		int edge3A = edge2B + 1;
		int edge3B = edge3A + 1;
		int edge4A = edge3B + 1;
		int edge4B = edge4A + 1;
		int edge5A = edge4B + 1;
		int edge5B = edge5A + 1;
		int edge6A = edge5B + 1;
		int edge6B = edge6A + 1;
		
		V.resize(edge6B + 1);
		W.resize(edge6B + 1);
		
		V[edge1A] = edge1A;
		V[edge1B] = edge1B;
		V[edge2A] = edge2A;
		V[edge2B] = edge2B;
		V[edge3A] = edge3A;
		V[edge3B] = edge3B;
		V[edge4A] = edge4A;
		V[edge4B] = edge4B;
		V[edge5A] = edge5A;
		V[edge5B] = edge5B;
		V[edge6A] = edge6A;
		V[edge6B] = edge6B;

		// First Pair
		sL0.push_back(edge1A);
		sL1.push_back(edge1A);
		sL2.push_back(edge1A);
		sU6.push_back(edge1B);
		sU7.push_back(edge1B);
		sU8.push_back(edge1B);

		// Second Pair
		sL2.push_back(edge2A);
		sL5.push_back(edge2A);
		sL8.push_back(edge2A);
		sU0.push_back(edge2B);
		sU3.push_back(edge2B);
		sU6.push_back(edge2B);

		// Third Pair
		sL6.push_back(edge3A);
		sL7.push_back(edge3A);
		sL8.push_back(edge3A);
		sU0.push_back(edge3B);
		sU1.push_back(edge3B);
		sU2.push_back(edge3B);

		// Fourth Pair
		sL0.push_back(edge4A);
		sL3.push_back(edge4A);
		sL6.push_back(edge4A);
		sU2.push_back(edge4B);
		sU5.push_back(edge4B);
		sU8.push_back(edge4B);

		// Fifth Pair
		sL0.push_back(edge5A);
		sM0.push_back(edge5A);
		sU0.push_back(edge5A);
		sL8.push_back(edge5B);
		sM8.push_back(edge5B);
		sU8.push_back(edge5B);

		// Sixth Pair
		sL2.push_back(edge6A);
		sM2.push_back(edge6A);
		sU2.push_back(edge6A);
		sL6.push_back(edge6B);
		sM6.push_back(edge6B);
		sU6.push_back(edge6B);

		this->surrounding = {	sL0, sL1, sL2, sL3, sL4, sL5, sL6, sL7, sL8, 
								sM0, sM1, sM2, sM3, sM4, sM5, sM6, sM7, sM8,	
								sU0, sU1, sU2, sU3, sU4, sU5, sU6, sU7, sU8 };
		
		return;
	}
	// 1's are positive connectable nodes, 0's are negative not connectable nodes
	void insert(int element) {
		W[position] = 1;
		if (element) {
			V[position] = position;
			int s = getSurrounding(position);
			for (auto i : surrounding[s]) {
				if (i < nnn)	{ join(position, position - i); }
				else		{ join(position, i); }
			}
		}
		else {
			V[position] = -1;
		}
		position++;  
		return;
	}
	// Find correct element in vector<int> surrounding
	int getSurrounding(int x) {
		int result{ 0 };
	
		int nn = n * n;
		int h = x / nn;
		int r = (x - (h * nn)) / n;
		int c = x % n;
		
		if (r == 0 && c == 0)		{ result = 0; }
		else if (r == 0 && c != m)	{ result = 1; }
		else if (r == 0 && c == m)	{ result = 2; }
		else if (r != m && c == 0)	{ result = 3; }
		else if (r != m && c != m)	{ result = 4; }
		else if (r != m && c == m)	{ result = 5; }
		else if (r == m && c == 0)	{ result = 6; }
		else if (r == m && c != m)	{ result = 7; }
		else if (r == m && c == m)	{ result = 8; }
		if (h) { result += 9; }
		if (h == n-1) { result += 9; }

		return result;
	}
	// Set two nodes to the same root
	void join(int a, int b) {
		//cout << b << "  ";  
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

};
// read data and filter it into Graph object
void readFile(string path) {
	Graph G;
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
	//G.vprint();
	int count = 0;
	size = size * size * size;
	for (int i = size; i < size + 12; i += 2) {
		if (G.find(i, i + 1)) {
			// cout << i << "  " << i + 1 << endl;  
			count++;
		}
	}
	cout << count << endl;  
	return;  
}
// For testing, create a file a various size and "1-doping" (p) for 3D cube
void createFile(string &path, int size, double p) {
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
	string path = "in.txt";
	int size = 20;  
	double p = 0.15; 
	
	createFile(path, size, p); 
    path = "in.txt"; 
	auto t0 = std::chrono::steady_clock::now();
    readFile(path); 
	double elapsed_time = (std::chrono::steady_clock::now() - t0).count() / 1e9;
	cout << "Total Elapsed Time:\t" << elapsed_time << endl << endl;

	return 0;
}




