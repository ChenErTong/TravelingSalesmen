#include<iostream>
#include<fstream>
#include<string>
#include<random>
using namespace std;

#define MAX_CITY 50
#define ITERATION 1000

struct City{
	int id;
	string name;
	double la;
	double lo;
	City *before, *next;
};

City cities[MAX_CITY];
double distances[MAX_CITY][MAX_CITY];

void initDistance(int num) {
	for (int i = 0; i <= num; ++i){
		distances[i][i] = 0;
		for (int j = i + 1; j < num; ++j) {
			distances[i][j] = distances[j][i] = sqrt(pow(cities[i].la - cities[j].la, 2) + pow(cities[i].lo - cities[j].lo, 2));
		}
	}
}

void output(int n) {
	City* city = &cities[0];
	for (int i = 0; i <= n; ++i) {
		cout << city->name << " ";
		city = city->next;
	}
	cout << endl;
}

bool swap(int i, int j, int n) {
	if (cities[i].before != &cities[j]) {
		City *city = cities[i].next, *tmp;
		while (city != &cities[j]) {
			tmp = city->next;
			city->next = city->before;
			city->before = tmp;
			city = tmp;
		}
		city = cities[j].next;
		cities[j].next = cities[j].before;
		cities[j].before = cities[i].before;
		cities[j].before->next = &cities[j];
		cities[i].before = cities[i].next;
		cities[i].next = city;
		cities[i].next->before = &cities[i];
		return true;
	}
	return false;
}

double simulatedAnnealing(double distance, int n, double t) {
	int a, b;
	for (int i = 0; i < ITERATION; ++i){
		do {
			a = rand() % (n);
			b = rand() % (n);
		} while (a == b);
		double candidate = distance - distances[cities[a].id][cities[a].before->id] - distances[cities[b].id][cities[b].next->id] + distances[cities[a].before->id][cities[b].id] + distances[cities[a].id][cities[b].next->id];
		double p = (candidate <= distance ? 1 : exp((distance - candidate) / t));
		if (((double)rand()) / RAND_MAX <= p && swap(a, b, n)) {
			distance = candidate;
		}
	}
	
	return distance;
}

int main() {
	string path = "TSP20.txt", line;
	ifstream in(path);
	if (in) {
		int n;
		in >> n;
		for (int i = 0; i < n; ++i) {
			cities[i] = City();
			in >> cities[i].name >> cities[i].la >> cities[i].lo;
			cities[i].id = i;
		}
		initDistance(n);
		double distance = 0;
		for (int i = 0; i < n; ++i) {
			cities[i].next = &cities[(i + 1) % n];
			cities[i].before = &cities[(n + i - 1) % n];
			distance += distances[i][(i + 1) % n];
		}
		
		int count = 1;
		double cache;
		for (double t = 280; t > 0.0001; t *= 0.98) {
			cache = simulatedAnnealing(distance, n, t);
			if (cache == distance) {
				if (++count == 10) {
					break;
				}
			} else {
				count = 1;
				distance = cache;
			}
		}
		cout <<  distance << ": ";
		output(n);
	}
}