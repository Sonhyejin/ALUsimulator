/*
������ �� ���:
������ ��ȯ
���� ��ȯ
����/����: ���� �������
������ Booth alg��
*/

#include <iostream>
#define SIZE 32 // �������� �ִ����: 32��Ʈ
using namespace std;

int *convert_binary(int num, int *arr) {
	//int arr[SIZE];
	int k = SIZE - 1; // ī���ÿ� ����

	for (int i = 0; i < SIZE; i++) // �迭 ��� 0���� ä��
		arr[i] = 0;

	while (1) {
		arr[k] = num % 2;
		//cout << arr[k];
		num = num / 2;
		if (num == 1) {
			arr[k - 1] = 1;
			break;
		}
		else k--;
	}
	//// ****��� �ߵǴ��� Ȯ�ο�****
	//for (int i = 0; i < SIZE; i++) {
	//	cout << arr[i];
	//	if (i % 4 == 3) cout << "|";
	//}
	//// ****************************
	return arr;
}


void main() {
	int a; 
	int b;
	int arr[SIZE];

	cout << "�� ���� �Է��ϼ���: ";
	cin >> a >> b;
	
	int *x = convert_binary(a, arr);
	int *y = convert_binary(b, arr);
	
	//cout << endl;

	//// ���Ȯ�ο�
	//for (int i = 0; i < SIZE; i++)
	//	cout << k[i];
	//cout << endl;
	//// ****

	return;
}