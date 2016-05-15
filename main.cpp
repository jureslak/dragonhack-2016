#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <tuple>
#include <queue>
#include <utility>
#include <stack>
#include <set>
#include <map>
#include <deque>
#include <limits>
#include <new>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <chrono>
#include <thread>

using namespace std;
typedef long long ll;

struct mv {
	int from, to, id;
};

int main(void) {
	cin.tie(nullptr);
	cin.sync_with_stdio(false);

	int nt, nb;
	cin >> nt >> nb;
	vector<vector<int>> b(nt);
	vector<vector<int>> f(nt);
	vector<int> t(nt);
	for (int i = 0; i < nt; i++) {
		t[i] = 2 + i;
		int temp;
		cin >> temp;
		if (temp == 0) continue;
		b[i].resize(temp);
		for (int j = 0; j < temp; j++) {
			cin >> b[i][j];
		}
	}
	for (int i = 0; i < nt; i++) {
		int temp;
		cin >> temp;
		if (temp == 0) continue;
		f[i].resize(temp);
		for (int j = 0; j < temp; j++) {
			cin >> f[i][j];
		}
	}
	mt19937_64 gen(clock());
	uniform_int_distribution<int> dist(500,1000);
	//Print: FROM TO WHO LENGTH
	mv mvs[] = { {1,2,1}, {2,3,1}, {3,2,1}, {1,2,4}, {3,1,3}, {2,3,4}, {2,3,1} };
	clock_t start = clock();
	for (int i = 0; i < 7; i++) {
		int temp = dist(gen);
//  		this_thread::sleep_for(chrono::milliseconds(temp));
		cout << mvs[i].from << ' ' << mvs[i].to << ' ' << mvs[i].id << ' ' << (double)(clock() - start) / CLOCKS_PER_SEC << '\n';
	}

	return 0;
}