#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <math.h>

using namespace std;

int profitTotal=0;

struct problem{
	int numelements;
	int cap;
	vector<int> weights;
	vector<int> profits;
};

struct element{
	int weight;
	int profit;
	double ratio;
};
struct answer{
	int tprofit;
	int size;
	double time;
};
bool comp(const element& a, const element& b){
	if(a.weight==0){
		return false;
	}
	if(b.weight==0){
		return false;
	}
	return (a.profit/a.weight) > (b.profit/b.weight);
}
int greedy1(problem p){

	int capacity = p.cap;
	vector<int> ratios;
	vector<element> elements;
	for(int i = 0 ; i < p.numelements ; i++){
		ratios.push_back((p.profits[i]/p.weights[i]));
		element e;
		e.profit = p.profits[i];
		e.weight = p.weights[i];
		elements.push_back(e);
	}
	int n = sizeof(ratios) / sizeof(ratios[0]);
	sort(begin(ratios), end(ratios) , greater<int>());
	for(int i = 0 ; i < ratios.size() ; i++){
		for(int j = 0 ; j < elements.size();j++)
		if(ratios[i]==(elements[j].profit/elements[j].weight)){
			element temp = elements[i];
			elements[i]=elements[j];
			elements[j]=temp;
		}
	}
	vector<element> knapsack;
	int totalprofit = 0;
	int currweight = 0;
	for(int i = 0 ; i < elements.size() ; i++){
		if(elements[i].weight<=capacity){
			if((currweight+elements[i].weight)<=capacity){
				totalprofit+=elements[i].profit;
				currweight+=elements[i].weight;
				//cout << "currweight: " << currweight << endl;
				//cout << "totalprofit: " << totalprofit << endl;
			}
		}
	}

	return totalprofit;
};

int greedy2(problem p){

	int maxprofit = 0;
	for(int i = 0 ; i < p.numelements ; i++){
		if(p.profits[i]>maxprofit){
			if(p.weights[i] <= p.cap){
				maxprofit = p.profits[i];
			}
		}
	}
	int b = greedy1(p);
	//int bprof = b.tprofit;

	return max(b, maxprofit);
}

double bound(problem p, int j, int weight, int profit){
	//cout << "computing bound" << endl;
	//cout << "numelements: " << p.numelements << endl;
	vector<element> elements;
	for(int i = 0 ; i < p.numelements ; i++){
		element e;
		e.profit = p.profits[i];
		e.weight = p.weights[i];
		e.ratio = (double)p.profits[i]/(double)p.weights[i];
		elements.push_back(e);
	}
	sort(elements.begin(), elements.end() , comp);
	//cout << "finished sorting" << endl;
	int currweight = 0;
	int index = 0;
	double upperbound = 0;
	while(currweight < p.cap && index < p.numelements){
		//cout << p.numelements << endl;
		/*cout << "index: " << index << endl;
		cout << "weight: " << elements[index].weight << endl;
		cout << "profit: " << elements[index].profit << endl;
		cout << "upperbound: " << upperbound << endl;*/
		if(currweight+elements[index].weight <= p.cap){
			currweight+=elements[index].weight;
			upperbound+=elements[index].profit;
		}
		else{
			int sub = p.cap - currweight;
			double frac = (double)elements[index].profit/(double)elements[index].weight;
			//cout << "frac: " << frac << endl;
			double ret = sub * frac;
			//cout << "ret: " << ret << endl;
			currweight = p.cap;
			upperbound+=ret;
		}
		index++;
	}
	//cout << "finished while loop" << endl;
	//cout << "final upperbound: " << upperbound << endl;
	return upperbound;
}

bool isPromising(problem p, int weight, int profit, int maxprofit, int i){
//	int w =weight;
//	int p=profit;
	double upperbound = bound(p, i+1, weight, profit);
	/*cout << "bound: " << upperbound << endl;
	cout << "weight: " << weight << endl;
	cout<<"max profit: " <<maxprofit <<endl;
	*/
	//cout << p.cap << endl;
	//cout << upperbound << endl;
	//cout << maxprofit << endl;
	if(weight >= p.cap ) return false;
	return(upperbound > profitTotal);
//	if(weight < p.cap && upperbound>maxprofit){
//		cout << "true " << endl;
//		return true;
//	}
//	return false;
}

int knapsackHelper(problem p, int level , int weight , int value, int currentmax){
	//cout<<"level: " << level <<endl;
	//int level1=level;
//	if(level >= p.numelements){
//		return currentmax;
//	}
	/*cout << "level: " << level << endl;
	cout << "capacity: " << p.cap << endl;
	cout << "weight: " << weight << endl;
	cout << "profit: " << value << endl;
	cout << "currentmax: " << profitTotal << endl
	*/
	if(weight <= p.cap && value>profitTotal){
		//currentmax = value;
		profitTotal = value;
	}
	bool temp = isPromising(p, weight, value, currentmax, level);
	if(temp==true){
		if(level+1!=p.numelements){
			knapsackHelper(p, level+1, weight + p.weights[level+1] , value + p.profits[level+1], currentmax);
/*
			problem p2;
			p2.numelements = p.numelements;
			p2.cap = p.cap;
			for(int i = 0 ; i < p.numelements ; i++ ){
				if(i==level){
					p2.weights.push_back(0);
					p2.profits.push_back(0);
				}
				else{
					p2.weights.push_back(p.weights[i]);
					p2.profits.push_back(p.profits[i]);
				}
			}
			if(value>currentmax){
				currentmax = value;
			}
			if(leftmax > currentmax){
				currentmax = leftmax;
			}
	*/
			knapsackHelper(p, level+1, weight, value, currentmax);
			//return max(leftmax, rightmax);
	}
}
	//return currentmax;
}


int knapsack(problem p){
	profitTotal=greedy2(p);
	int maxprofit=greedy2(p);
	vector<element> elements;
	for(int i = 0 ; i < p.numelements ; i++){
		element e;
		e.profit = p.profits[i];
		e.weight = p.weights[i];
		e.ratio = (double)p.profits[i]/(double)p.weights[i];
		elements.push_back(e);
	}
	sort(elements.begin(), elements.end() , comp);
	for(int i = 0 ;i < elements.size() ; i++){
		p.profits[i]=elements[i].profit;
		//cout << "profit: " << p.profits[i] << endl;
		p.weights[i]=elements[i].weight;
		//cout << p.weights[i] << endl;
	}
  knapsackHelper(p, -1, 0, 0, maxprofit);
	return profitTotal;
};
//**** DYNAMIC PROGRAMMING STARTS HERE ****
int dp(problem p){
	int rows = p.numelements+1;
	vector<int> weights;
	vector<int> profits;
	for(int i = 0 ; i < p.numelements+1 ; i++){
		if(i==0){
			weights.push_back(0);
			profits.push_back(0);
		}
		else{
			weights.push_back(p.weights[i-1]);
			profits.push_back(p.profits[i-1]);
		}
	}
	int cols = p.cap+1;
	int matrix[2][cols];
	//cout << "hello" << endl;
	for(int i = 0 ; i < cols ; i++){
		//cout << "column zeroer" << endl;
		matrix[0][i]=0;
	}
	int mp = 0;
	matrix[0][0]=0;
	matrix[1][0]=0;
			int matrixcounter = 0;

	for(int i = 0 ; i < rows; i++){
	//	cout << "matrixcounter: " << matrixcounter << endl;
		//cout << "i: " << i << endl;
		//matrix[i][0]=0;
		for(int c = 1 ; c < cols ; c++){
			//matrix[0][c]=0;
			if(i!=0){
				if(weights[i]<=c&&(matrix[0][c-weights[i]]+profits[i])>matrix[0][c]){
					matrix[1][c] = matrix[0][c-weights[i]]+profits[i];
				}
				else{
					matrix[1][c] = matrix[0][c];
				}
			}
			//cout << "matrix[" << i << "][" << c << "]: " << matrix[matrixcounter][c] << " vs. mp: " << mp << endl;
			if(matrix[0][c]>mp&&matrixcounter==0){
				mp = matrix[0][c];
			}
			if(matrix[1][c]>mp&&matrixcounter==1){
				mp = matrix[1][c];
			}
		}
		if(i!=0){
			for(int c = 0 ; c < cols ; c++){
				matrix[0][c] = matrix[1][c];
			}
		}
		if(i==0){
			matrixcounter++;
		}
	}
	cout << mp << endl;
	return mp;
}


int main(int argc, char** argv){
	ifstream infile(argv[1]);
	ofstream output;
	output.open(argv[2]);
	string dec = argv[3];
	int decider = stoi(dec);
	vector<problem> problems;
	int number = 0;
	int capacity = 0;
	while(infile >> number >> capacity){
		problem p;
		p.numelements = number;
		p.cap = capacity;
		for(int i = 0 ; i < number ; i++){
			int weight;
			int profit;
			infile >> weight >> profit;
			p.weights.push_back(weight);
			p.profits.push_back(profit);
		}
		problems.push_back(p);
	}
	for(int i = 0 ; i < problems.size() ; i++){
		if(decider == 0){
			std::chrono::time_point<std::chrono::system_clock> s, e;//timer variable s = start , e = end
		  s = std::chrono::system_clock::now();
			int max = greedy1(problems[i]);
			e = std::chrono::system_clock::now();
		  std::chrono::duration<double> els = e - s;
			output << problems[i].numelements << " " << max << " " << els.count() << endl;
		}
		else if(decider == 1){
			std::chrono::time_point<std::chrono::system_clock> s, e;//timer variable s = start , e = end
		  s = std::chrono::system_clock::now();
			int max = greedy2(problems[i]);
			e = std::chrono::system_clock::now();
		  std::chrono::duration<double> els = e - s;
			output << problems[i].numelements << " " << max << " " << els.count() << endl;
		}
		else if(decider == 2){
			std::chrono::time_point<std::chrono::system_clock> s, e;//timer variable s = start , e = end
		  s = std::chrono::system_clock::now();
			int max = knapsack(problems[i]);
			e = std::chrono::system_clock::now();
		  std::chrono::duration<double> els = e - s;
			output << problems[i].numelements << " " << max << " " << els.count() << endl;
		}
		else if(decider == 3){
			std::chrono::time_point<std::chrono::system_clock> s, e;//timer variable s = start , e = end
		  s = std::chrono::system_clock::now();
			int max = dp(problems[i]);
			e = std::chrono::system_clock::now();
		  std::chrono::duration<double> els = e - s;
			output << problems[i].numelements << " " << max << " " << els.count() << endl;
		}
	}
	infile.close();
	output.close();
}
