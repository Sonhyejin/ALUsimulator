#include <iostream>
#include <climits>

using namespace std;

void main() {
	int a; // length of stick
	int cnt = 0; // count #1

	while(1) {
		cout << "���ϴ� ���̸� �Է��ϼ���(0<����<65): ";
		cin >> a;

		if (a > 64 || a < 1)
			cout << "���̸� �ʰ��Ͽ����ϴ�." << endl;
		else if (cin.fail()) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "���ڸ� �Է°����մϴ�." << endl;
		}
			break;
	}
	//cout << a;

	while (1) {
		if (a == 1) break;

		//cout << a % 2;
		if (a % 2 == 1) {
			cnt++;
			a = a / 2;
			//cout << "[" << a << "]";
		}
		else if (a % 2 == 0) {
			a = a / 2;
			//cout << "[" << a << "]";
		}
	}
	cnt++;

	cout << "���� ���� ��:" << cnt << endl;

	return;
}