#include <iostream>
#include <time.h>
#include <cstdlib>
using namespace std;

void swap(string &a, string &b) {
	string c;
	c = a;
	a = b;
	b = c;
}

int main(void) {
	string f1 = "Pizza from Mineo's";
	string f2 = "Tofu Tikka Masala from Prince of India";
	string f3 = "Crispy Salmon Roll from Sushi Fuku";
	string f4 = "Sub from Uncle Sam's";
	string f5 = "Fried rice from How Lee";
	string f6 = "Sandwiches from La Prima";
	string f7 = "Find free food on campus";
	std::string food[7] = { f1, f2, f3, f4, f5, f6, f7 };

	std::string days[7] = {  "Sun ", "Mon ", "Tue ", "Wed ", "Thr ", "Fri ", "Sat "};

	//std::cout << food[2] << std::endl;
	srand(time(NULL));
	for (int i = 0; i <= 6; i++) {
		int n = rand() % 7;
		swap(food[i], food[n]);
		
	}
	for (int j = 0; j <= 6; j++) {
		std::cout << days[j];
		std::cout << food[j] << std::endl;
	}

	return 0;
}