#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<long long> DP(100000, 0);
const int MOD = 1e9 + 7;

int main() {
	string str;
	cin >> str;
	int len = str.length();

    DP[0] = 1;

    switch (str[0]) {
    case '*': DP[1] = 9; break;
    case '0': DP[1] = 0; break;
    default: DP[1] = 1; break;
    }

	for (int i = 2; i <= len; i++) {
        if (str[i - 1] == '*') {
            DP[i] += 9 * DP[i - 1];
        }
        else {
            if (str[i - 1] != '0') {
                DP[i] += DP[i - 1];
            }
        }

        if (str[i - 1] == '*' && str[i - 2] == '*') {
            DP[i] += 15 * DP[i - 2];
        }
        else {
            if (str[i - 2] == '*') {
                if (str[i - 1] >= '0' && str[i - 1] <= '6') {
                    DP[i] += 2 * DP[i - 2];
                }
                else {
                    DP[i] += DP[i - 2];
                }
            }
            else {
                if (str[i - 1] == '*') {
                    if (str[i - 2] == '1') {
                        DP[i] += 9 * DP[i - 2];
                    }
                    else {
                        if (str[i - 2] == '2') {
                            DP[i] += 6 * DP[i - 2];
                        }
                    }
                }
                else {
                    int num = (str[i - 2] - '0') * 10 + (str[i - 1] - '0');
                    if (num >= 10 && num <= 26) {
                        DP[i] += DP[i - 2];
                    }
                }
            }
        }
        DP[i] %= MOD;
	}
	cout << DP[len];
}