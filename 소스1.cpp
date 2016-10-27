/*
만들어야 할 기능:
이진수 변환(O)
2의 보수 변환
덧셈/뺄셈: 병렬 가감산기
곱셈은 Booth alg로
*/

#include <iostream>
#include <cmath>
#define SIZE 32 // 레지스터 최대길이: 32비트
using namespace std;

int *convert_ieee(int *arr, int count1, int count2, bool minus);
void Signbit(int num);
void Carrybit(int num);
void Zerobit(int *num);
void Overbit(int carry1, int carry2);
bool ieee_exception(int *arr);

int *convert_complement(int *arr, int size) { /* 2의 보수 변환 */

											  /* STEP 1: 비트반전(1의 보수) */
	for (int i = 0; i < size; i++) {
		if (arr[i] == 0)
			arr[i] = 1;
		else if (arr[i] == 1)
			arr[i] = 0;
		//cout << arr[i];
	}

	/* STEP 2: +1해서 2의 보수로 만듦 */
	arr[size - 1]++;

	for (int i = size - 1; i >= 0; i--) {
		if (arr[i] == 1)
			break;
		else if (arr[i] == 2) {
			arr[i] = 0;
			arr[i - 1]++;
		}
		else if (arr[i] == 3) {
			arr[i] = 1;
			arr[i - 1]++;
		}
	}

	return arr;
}

int *sum(int *arr1, int *arr2, int* result, int size, bool status) { /* 덧셈연산 */
																	 //int result[SIZE]; // 결과 저장용 배열
	int carry1 = 0;
	int carry2 = 0;

	// 결과값 담을 배열 초기화
	for (int i = 0; i < size; i++)
		result[i] = 0;

	cout << "원래 값: ";
	for (int i = 0; i < size; i++) cout << arr1[i];
	cout << "\n더할 애: ";
	for (int i = 0; i < size; i++) cout << arr2[i];
	cout << endl;

	for (int i = size - 1; i >= 0; i--) {
		int sum = arr1[i] + arr2[i]; // 자릿수 합 저장
		int temp = sum + result[i]; // 자리올림 발생할 수 있으므로 변수 하나 더 정의


		if (temp == 1 || temp == 0)
			result[i] = temp;
		else if (temp == 2) {
			result[i] = 0;
			if (i != 0) result[i - 1] = 1; // array index 가 -1 이 되는 것을 방지
			else break;
		}
		else if (temp == 3) {
			result[i] = 1;
			if (i != 0) result[i - 1] = 1;
			else break;
		}
		//cout << sum << " " << temp << " " << result[i] << endl;
	}

	//cout << "함수 안에서 덧셈 후" << endl;
	// for (int i = 0; i < SIZE; i++)
	//   cout << result[i];

	// cout << endl;
	//결과값 출력
	if (status) { //덧셈일 때
		cout << "덧셈 결과 : ";
		for (int i = 0; i < size; i++)
			cout << result[i];
		cout << endl;

		//비트값 출력
		Carrybit(carry2);
		Signbit(result[0]);
		Zerobit(result);
		Overbit(carry1, carry1); //carry1은 내부캐리, carry2는 외부캐리
	}
	//   cout << carry1 << endl;
	//   cout << carry2 << endl;

	return result;
}

int convert_decimal(int *arr, int size) { /* 십진수 변환 - 계산할 때 사용될 배열 */
	int result = 0; // 결과값 저장
	bool negative = false; // 양수 음수 판단 변수

						   /* STEP 1: 십진수 변환을 위해 양수로 바꿔줌 */
	if (arr[0] == 1) { // 먼저 보수로 바꿔줌
		negative = true;
		arr = convert_complement(arr, size);
	}

	/* STEP 2: 십진수 변환 */
	int k = 0; // 지수 카운팅용 변수
	for (int i = size - 1; i >= 0; i--) {
		if (arr[i] == 1)
			result = result + pow((double)2, k);
		k++;
	}

	//cout << "result = " << result << endl;

	if (negative) result = result * (-1); // 부호비트가 1(음수)였다면 음수로 취해줌

	return result;
}

void copy(int *original, int *object, int size) {
	for (int i = 0; i < size; i++)
		original[i] = object[i];
}

void print(int *A, int *Q, int prev) { // 매번 쓰기 번거로우므로 출력하는 함수 따로 선언
									   //cout << "수행 결과: ";
	for (int i = 0; i < SIZE / 2; i++) cout << A[i];
	cout << " ";
	for (int i = 0; i < SIZE / 2; i++) cout << Q[i];
	cout << " ";
	cout << prev << endl;
}

void sprint(int *A, int *Q, int prev) { // A ± M 수행 결과 출력
	char sign;
	if (prev == 0) sign = '-';
	else if (prev == 1) sign = '+';
	cout << "A" << sign << "M 수행 결과: ";
	for (int i = 0; i < SIZE / 2; i++) cout << A[i];
	cout << " ";
	for (int i = 0; i < SIZE / 2; i++) cout << Q[i];
	cout << " ";
	cout << prev << endl;
}

void ASR(int *A, int *Q, int &prev, int size) { /* Arithmatic Shift-Right - 적용될 배열, Q_-1 값, 배열 크기 */
												/*함수를 빠져나가니까 값 저장이 안됨. 값을 저장해서 밖으로 빼야됨*/

	cout << "전prev값: " << prev << endl;
	prev = Q[size - 1]; //  Q_-1에 배열 끝값 저장
	cout << "후prev값: " << prev << endl;

	cout << "Q 전출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << Q[i];
	cout << endl;

	for (int i = size - 1; i > 0; i--)
		Q[i] = Q[i - 1];
	Q[0] = A[size - 1];

	cout << "Q 후출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << Q[i];
	cout << endl;

	cout << "A 전출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << A[i];
	cout << endl;

	for (int i = size - 1; i > 0; i--)
		A[i] = A[i - 1];
	//A[0] = A[1];

	cout << "A 후출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << A[i];
	cout << endl;

	cout << "ASR 수행 결과: ";
	for (int i = 0; i < size; i++) cout << A[i];
	cout << " ";
	for (int i = 0; i < size; i++) cout << Q[i];
	cout << " ";
	cout << prev << endl;
}
void SL(int *A, int *Q, int size) { /* Shift Left */
	cout << "<Shift Left>" << endl;

	cout << "A 전출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << A[i];
	cout << endl;

	for (int i = 0; i < size - 1; i++)
		A[i] = A[i + 1];
	A[size - 1] = Q[0];

	cout << "A 후출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << A[i];
	cout << endl;

	cout << "Q 전출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << Q[i];
	cout << endl;

	for (int i = 0; i < size - 1; i++)
		Q[i] = Q[i + 1];
	Q[size - 1] = 0;

	cout << "Q 후출력 확인: ";
	for (int i = 0; i < size; i++)
		cout << Q[i];
	cout << endl;

	cout << "SL 수행 결과: ";
	for (int i = 0; i < size; i++) cout << A[i];
	cout << " ";
	for (int i = 0; i < size; i++) cout << Q[i];
	cout << " ";
}

int *mul(int *Q, int *M, int *A, int *result, int size) { /* 곱셈 by BOOTH Alg. - 계산할 이진수 배열 2개, A레지스터, Q레지스터 */
	int prev = 0; // Q_-1 값 저장할 변수
	int init_M = M[0]; // M의 처음 부호비트

					   /* STEP 1: A레지스터 값 설정, M레지스터 보수값 미리 구해놓기 */
	for (int i = 0; i < size; i++)
		A[i] = Q[0];

	int *M_cpl = convert_complement(M, size); // M의 2의 보수
	int init_Mcpl = M_cpl[0]; // M'의 처음 부호비트

	cout << "\n\n[곱셈]M의 2의 보수: ";
	for (int i = 0; i < size; i++)
		cout << M_cpl[i];

	cout << endl;

	cout << "\t\t A \t\t    Q \t    Q_-1" << endl;
	cout << "초기상태: ";
	print(A, Q, prev);

	/* STEP 2: Q레지스터 비트 크기(SIZE/2 = 16)만큼 cycle 반복 */

	int A_result[SIZE / 2] = { 0 }; // A레지스터 연산 결과 저장할 임시 변수
	int *R = { 0 };

	for (int i = 0; i < size; i++) {

		//cout << "\n\nA레지스터값: ";
		//for (int k = 0; k < size; k++)
		//   cout << A[k];

		//cout << "\n\nM레지스터값: ";
		//for (int k = 0; k < size; k++)
		//   cout << M[k];

		cout << "\ncycle " << i + 1 << endl;
		if (Q[size - 1] == 0 && prev == 0) { // 00일 경우 - just ASR(Arithmatic Shift - Right)
			cout << "<00일 경우>\n";
			ASR(A, Q, prev, size);
			//print(A, Q, prev);
		}
		else if (Q[size - 1] == 1 && prev == 1) { // 11일 경우 - just ASR
			cout << "<11일 경우>\n";
			ASR(A, Q, prev, size);
			//print(A, Q, prev);
		}
		else if (Q[size - 1] == 0 && prev == 1) { // 01일 경우 - A + M & ASR
			cout << "<01일 경우>\n";
			if (M[0] != init_M) M = convert_complement(M, size);
			R = sum(A, M, A_result, size, 0);
			copy(A, R, size); // A에 A_result값 덮어 씌움
			sprint(A, Q, prev);
			ASR(A, Q, prev, size);
			//print(A, Q, prev);
		}
		else if (Q[size - 1] == 1 && prev == 0) { // 10일 경우 - A + M'(M의 보수) & ASR
			cout << "<10일 경우>\n";
			if (M_cpl[0] != init_Mcpl) M_cpl = convert_complement(M_cpl, size);
			R = sum(A, M_cpl, A_result, size, 0);
			copy(A, R, size); // A에 A_result값 덮어 씌움
			sprint(A, Q, prev);
			ASR(A, Q, prev, size);
			//print(A, Q, prev);
		}
	}

	/* STEP 3: A와 Q레지스터 합쳐서 결과레지스터에 저장 */
	for (int i = 0; i < size; i++) {
		result[i] = A[i];
		//cout << result[i];
		//if (i % 4 == 3) cout << "_";
	}
	for (int j = 0; j < size; j++) {
		result[j + size] = Q[j];
		//cout << result[j + size];
		//if (j % 4 == 3) cout << "_";
	}

	////출력 확인용
	//cout << endl << "결과 레지스터: ";
	//for (int i = 0; i < SIZE; i++)
	//   cout << result[i];

	return result;
}

int *div(int *Q, int *M, int *A, int *result, int size) { /* 나눗셈 - 계산할 이진수 배열 2개, A레지스터, Q레지스터 */
														  /* STEP 1: A레지스터 값 설정, M레지스터 보수값 미리 구해놓기 */
	int compare[SIZE / 2] = { 0 }; // A, M 연산값이랑 비교할 배열

	for (int i = 0; i < size; i++) {
		A[i] = Q[0];
		compare[i] = M[0];
	}

	//cout << ""
	for (int i = 0; i < size; i++)
		cout << compare[i];

	int init_A = A[0];

	int *M_cpl = convert_complement(M, size); // M의 2의 보수
	int init_Mcpl = M_cpl[0]; // M'의 처음 부호비트

							  /* STEP 2: Q레지스터 비트 크기(SIZE/2 = 16)만큼 cycle 반복 */

	for (int i = 0; i < size; i++) {
		//   SL(A, Q, size);

		//      if ()
	}

	/* STEP 3: A와 Q레지스터 합쳐서 결과레지스터에 저장 */
	for (int i = 0; i < size; i++) {
		result[i] = A[i];
		//cout << result[i];
		//if (i % 4 == 3) cout << "_";
	}
	for (int j = 0; j < size; j++) {
		result[j + size] = Q[j];
		//cout << result[j + size];
		//if (j % 4 == 3) cout << "_";
	}
	return result;
}

int *convert_binary(double num, int *arr, int size) {
	int k = size - 1; // 카운팅용 변수
	bool minus = false;
	bool ieee = false;
	double R = 0;
	int count1 = 0; // 전체 개수
	int count2 = 0; // 정수 개수

	if (num < 0) { // 음수판별 --> 추후 2의 보수로 바꿀지 결정하는데 사용됨
		num = num * (-1);
		minus = true;
	}

	if ((int)num != num) { // 실수판별 --> 추후 IEEE754 표준형으로 바꿀지 결정하는 데 사용됨
		ieee = true;
		R = num - (int)num;
	}

	for (int i = 0; i < size; i++) // 배열 모두 0으로 채움
		arr[i] = 0;

	if (ieee) { //실수일 때는 실수부 먼저 계산한다
		do {

			R = R * 2;

			if (R >= 1) {
				R = R - 1;
				arr[k] = 1;
			}

			count1++;
			k--;

			//cout << "R값 : " << R << endl;

		} while (R != 0);

		if (count1) { //자리 바꾸기

			int swap = count1 - 1;
			int i = 1;
			int temp = 0;

			while (1) {
				cout << swap << endl;
				temp = arr[k + i];

				if (swap <= 0)
					break;

				//       cout << arr[k] << "arr[" << i + 1 << "]랑 arr[" << k + swap << "]" << arr[k + swap] << " 바꾸는 중ㅎㅎ" << endl;
				//     Sleep(60);

				arr[k + i] = arr[k + swap + i];
				arr[k + swap + i] = temp;
				swap = swap - 2;
				i++;

			}
		}
	}

	while (1) {
		arr[k] = (int)num % 2;
		//cout << arr[k];
		num = (int)num / 2;
		count1++;
		count2++; // 정수부분
		if (num == 1) {
			arr[k - 1] = 1;
			break;
		}
		else k--;
	}

	if (ieee) { // 위에서 실수라는 게 확인되었다면 ieee 표준으로 변환됨
		convert_ieee(arr, count1, count2, minus);
		minus = false; // 실수는 보수로 바꿀 필요x
	}

	if (minus) { // 위에서 음수라는게 확인되었다면 2의 보수로 변환됨
		convert_complement(arr, size);
	}

	return arr;
}


int *convert_ieee(int *arr, int count1, int count2, bool minus) {

	int exp1[8] = { 0 };
	int ieee1[SIZE] = { 0 };
	int k = 8;
	int j = 31;

	ieee1[0] = minus; // 부호비트

/*
	for (j; j - count1<32; j++) {

		ieee1[k] = arr[j - count1];

		cout << k << "가수부 넣는 중" << endl;
		cout << ieee1[k] << endl;

		k++;

	}*/

	count2 = count2 + 127; // 바이어스 값 더해주기

	int *exp = convert_binary((double)count2, exp1, 8);

	k = 8;
	j = 7;

	while (1) { // 지수부 집어넣기

		ieee1[k] = exp[j];

		j--;
		k--;

		if (k == 0)
			break;
	}
	cout << endl;
	//cout << "지수부 다 넣음" << endl;

	for (int i = 0; i<SIZE; i++) {
		arr[i] = ieee1[i];
	}

	ieee_exception(arr);

	if (!ieee_exception(arr)) {
		for (int i = 0; i < SIZE; i++)
			cout << arr[i];
		cout << endl;
	}

	return arr;
}

bool ieee_exception(int *arr) {
	int i = 0, j = 0;
	int sum_up = 0;
	int sum_down = 0;
	/*
	11111111 00101000101  -> NaN
	11111111 0000...0000  -> 오버플로우
	11111110 1111...11111 -> 최대정상수
	......
	00000001 000.....0000 -> 최소정상수
	00000000 111.....0010 -> 언더플로우
	00000000 0000....0000 -> zero
	*/

	for (i = 8; i > 0; i--, j++) { //지수부 10진수 변환해서 더하기 --> 예외처리 시각적으로 보기 편하도록
		sum_up += arr[i] * pow(10, j);
	}

	for (i = 0; i < 23; i++) { //가수부 배열 전체의 합 구하기 --> '1이 한개라도 있거나, 없거나'로 구분가능하기 때문
		sum_down += arr[9 + i];
	}

	if (sum_up == 11111111 && sum_down == 0) {
		cout << "오버플로우 입니다." << endl;
		return false;
	}
	else if (sum_up == 11111111 && sum_down >= 1) {
		cout << "NaN 입니다." << endl;
		return false;
	}
	else if (sum_up == 0 && sum_down == 0) {
		cout << "Zero 입니다." << endl;
		return false;
	}
	else if (sum_up == 0 && sum_down >= 1) {
		cout << "언더플로우 입니다." << endl;
		return false;
	}
	else {
		return true;
	}
}

double ieee_to_10(int* arr) {

	int sum_up = 0;
	int temp = 0;
	int arr_down[24] = { 0 };
	double a = 0, b = 0;
	int i = 0, j = 0;

	for (i = 8, j = 0; i > 0; i--, j++) { //지수부의 10진수 화
		sum_up += arr[i] * pow(2, j);
	}

	for (i = (SIZE - 1); i >= 0; i--) { //x의 가수부 길이 구하기
		if (arr[i] == 1) {

			break;
		}
		else {
			temp++;
		}
	}

	temp = SIZE - 9 - temp;

	for (i = 0; i < temp; i++) { //가수부 넣을 새로운 배열 추가
		arr_down[1 + i] = arr[9 + i];
	}
	arr_down[0] = 1; //생략되었던 1 추가

	for (i = sum_up, j = 0; i >= 0; i--, j++) {//정수부 합
		a += arr_down[i] * pow(2, j);
	}

	for (i = sum_up + 1, j = 0; i < 24; i++, j++) {//소수부 합
		b += arr_down[i] * pow(2, -1 * j);
	}

	return a + b;
}

void *ieee_sum(int *x, int *y) {
	int sum_up_X = 0, sum_up_Y = 0, length_down_X = 0, length_down_Y = 0;
	int sub_up_XY = 0, sub_up_YX = 0;
	int *sum_down = { 0 };
	int sum_final[32] = { 0 };
	int temp = 0;
	int A[8] = { 0 };
	int *sum_up_final = { 0 };
	int *x_3 = { 0 };
	int *y_3 = { 0 };
	int j = 0;
	int i = 0;

	for (i = 8, j = 0; i > 0; i--, j++) { //지수부의 10진수 화
		sum_up_X += x[i] * pow(2, j);
		sum_up_Y += y[i] * pow(2, j);
	}

	for (i = (SIZE - 1); i >= 0; i--) { //x의 가수부 길이 구하기
		if (x[i] == 1) {

			break;
		}
		else {
			length_down_X++;
		}
	}

	for (i = (SIZE - 1); i >= 0; i--) {
		if (y[i] == 1) {

			break;
		}
		else {
			length_down_Y++;
		}
	}

	length_down_X = SIZE - 9 - length_down_X; //가수부 길이 계산
	length_down_Y = SIZE - 9 - length_down_Y;

	sum_up_X = sum_up_X - 127 - length_down_X;
	sum_up_Y = sum_up_Y - 127 - length_down_Y; //소숫점 위치 맞춘 후의 지수 값 계산

	if (sum_up_X >= sum_up_Y) {//지수끼리의 차 구하기
		sub_up_XY = sum_up_X - sum_up_Y;

	}
	else {
		sub_up_YX = sum_up_Y - sum_up_X;
	}

	if (sub_up_XY == 0) {
		sum_up_X = sum_up_Y;
	}
	else {
		sum_up_X = sum_up_X;
	}

	sum_up_final = convert_binary(sum_up_X, A, 8);



	int x_2[32] = { 0 };
	int y_2[32] = { 0 }; //////////////////////////////////////////////////////////////동적할당 

	for (i = 0, j = 31; i < sub_up_XY; i++, j--) {
		x_2[j] = 0;
	}
	for (i = 0, j = length_down_X; j >= 0; i++, j--) {
		if (j = 0) {
			x_2[31 - sub_up_XY - i] = 1;//생략된 1 넣기
			x_2[30 - sub_up_XY - i] = x[0];//맨 앞에 부호비트 넣기
			break;
		}
		x_2[31 - sub_up_XY - i] = x[8 + length_down_X - i];
	}

	for (i = 0, j = 31; i < sub_up_YX; i++, j--) {
		y_2[j] = 0;
	}
	for (i = 0, j = length_down_Y; j>-0; i++, j--) {
		if (j = 0) {
			y_2[31 - sub_up_XY - i] = 1; //생략된 1 넣기
			y_2[30 - sub_up_XY - i] = y[0]; //맨 앞에 부호비트 넣기
			break;
		}
		y_2[31 - sub_up_YX - i] = y[8 + length_down_Y - i];
	}

	sum(x_2, y_2,sum_down,32,0);


	for (i = 0; i < 32; i++) {//가수부 합의 길이 구하기
		if (sum_down[i] == 1) {
			break;
		}
		temp++;
	}

	temp = 32 - temp;

	sum_up_X = temp - sum_up_X; //지수부 정규화

	for (i = 0; temp + i < 32; i++) {
		sum_final[9 + i] = sum_down[temp + 2 + i]; //첫 숫자 2개(부호비트, 1생략)를 빼야하므로 sum_down[temp+2+i]
	}

	for (i = 0; i < 8; i++) {
		sum_final[1 + i] = sum_up_final[i];
	}

	sum_final[0] = sum_down[temp]; //부호비트 넣기

								   /////////////////////////////////////////////////////////////////////예외처리하기

	for (i = 0; i < 32; i++) {
		cout << "계산 후 ieee754형식 : " << sum_final[i] << endl;
	}

	cout << "계산 후 10진수 형식 : " << ieee_to_10(sum_final);


	return 0;
}

void ieee_multipler(int* arr1, int* arr2) { //여기서 값 출력하고 끝낼 것임...

	int result[SIZE] = { 0 };
	int arr11[24] = { 0 };
	int arr22[24] = { 0 }; // 연산 위해 기존의 가수부를 저장할 배열들
	int count1 = 0;
	int   count2 = 0; //지수값
	int k1 = 0;
	int k2 = 0; // 가수부 자릿수
	int m = 0; //총 가수부 자릿수
	int n = 1;
	int num = 0; //총 지수승 값
	bool minus = false; // 총 연산 결과의 부호

	if (arr1[0] != arr2[0])
		minus = true;
	else
		minus = false;
	//이 연산의 부호 구하기

	for (int i = 8; i>0; i--) {
		count1 += arr1[i] * n;
		n *= 2;
	}

	count1 = count1 - 127;

	n = 1;

	for (int i = 8; i>0; i--) {
		count2 += arr2[i] * n;
		n *= 2;
	}

	count2 = count2 - 127;
	// 지수승 10진수 변환

	for (int i = SIZE - 1; i>9; i--) {
		k1 = i;
		if (arr1[i] == 0)
			continue;
		else
			break;
	}

	k1 = k1 - 8;
	//   cout << "arr1의 가수 자리: " <<k1<< endl;

	for (int i = SIZE - 1; i>9; i--) {
		k2 = i;
		if (arr2[i] == 0)
			continue;
		else
			break;
	}

	k2 = k2 - 8;
	//소수점 몇 자리인지 구하기

	//   cout << "arr2의 가수 자리: " <<k2<< endl;

	for (int i = 0; i <= k1; i++) { // 연산 위해 다른 배열에 집어넣기
		arr11[24 - 1 - i] = arr1[8 + k1 - i];
		if (i == k1)
			arr11[24 - 1 - i] = 1;
	}

	for (int i = 0; i <= k2; i++) { // 연산 위해 다른 배열에 집어넣기
		arr22[24 - 1 - i] = arr2[8 + k2 - i];
		if (i == k2)
			arr22[24 - 1 - i] = 1;
	}

	//cout << "arr11 : ";
	for (int i = 0; i<24; i++)
		cout << arr11[i]; // 결과 출력
	cout << endl;

	//cout << "arr22 : ";
	for (int i = 0; i<24; i++)
		cout << arr22[i]; // 결과 출력
	cout << endl;

	for (int j = 0; j<k1 + 1; j++) { //가수부 곱셈 수행
		for (int i = 0; i<k2 + 1; i++) {
			result[SIZE - 1 - j - i] += arr11[24 - 1 - i] * arr22[24 - 1 - j];

			//cout << result[SIZE - 1 - j - i] << " += " << arr11[24 - 1 - i] << " * " << arr22[24 - 1 - j] << endl;
			//cout << "result는 " << SIZE - 1 - j - i << "번째, arr11은 " << 24 - 1 - i << "번째, arr2는 " << 24 - 1 - j << "번째" << endl;

			if (result[SIZE - 1 - j - i]>1) { // 2나 3 같이 이진수로 표현할 수 있는 수가 초과되었을 때 바꿔주기
				result[SIZE - 2 - j - i] += result[SIZE - 1 - j - i] / 2;
				result[SIZE - 1 - j - i] = result[SIZE - 1 - j - i] % 2;
			}
			else
				continue;
		}
	}

	cout << "곱셈 수행 결과 : ";
	for (int i = 0; i<SIZE; i++)
		cout << result[i]; // 결과 출력
	cout << endl;

	for (int i = 0; i<SIZE; i++) { //구한 곱셈이 몇 자리인지 구하기
		m++;
		if (result[i] == 0)
			continue;
		else
			break;
	}

	m = 32 - m;
	//cout << "m값 : " << m << endl;
	num = count1 + count2;
	//   cout << count1 << " + " << count2 << endl;

	if (k1 + k2 != m) {
		num += m - k1 - k2;
		cout << "num값 : " << num << endl;
	}

	int *ieee = convert_ieee(result, m, num, minus);

	cout << "최종 곱셈 수행 결과 : ";
	for (int i = 0; i<SIZE - 1; i++)
		cout << ieee[i]; // 결과 출력
	cout << endl;

	return;
}

void ieee_divider(int* arr1, int* arr2) {

	int result[SIZE] = { 0 };
	int num = 0; // 지수승 값



				 //   int *ieee =   convert_ieee(result,m,num,minus);

	cout << "최종 나눗셈 수행 결과 : ";
	//   for(int i=0; i<SIZE-1; i++)
	//      cout << ieee[i]; // 결과 출력
	//   cout << endl;

	return;
}

void main() {
	double a;
	double b;
	int A[SIZE];
	int B[SIZE];
	int sumarr[SIZE] = { 0 }; // 덧셈결과 저장 배열
							  /* 곱셈연산용 변수 */
	int a_register[SIZE / 2] = { 0 };
	int q_register[SIZE / 2] = { 0 };
	int m_register[SIZE / 2] = { 0 };
	int result_mul[SIZE] = { 0 };
	bool ieeea = false;

	cout << "두 수를 입력하세요: ";
	cin >> a >> b;

	int *x = convert_binary(a, A, SIZE);
	int *y = convert_binary(b, B, SIZE);



	if ((int)a != a)
		ieeea = true;

	cout << "[" << a << "]의 2진수 변환 : ";
	if (a < 0) cout << "(2의보수로 변환됨)" << endl;
	for (int i = 0; i < SIZE; i++)
		cout << x[i];
	cout << endl;

	cout << "[" << b << "]의 2진수 변환 : ";
	if (b < 0) cout << "(2의보수로 변환됨)" << endl;
	for (int i = 0; i < SIZE; i++)
		cout << y[i];
	cout << endl;

	if (!ieeea) {
		int *result = sum(x, y, sumarr, SIZE, true);
		cout << "두 수의 덧셈: " << endl;
		for (int i = 0; i < SIZE; i++)
			cout << result[i];
		cout << endl;


		int test = convert_decimal(result, SIZE); // 32비트의 십진수 변환
		cout << "덧셈 결과값 십진수 변환 결과: " << test << endl;

		cout << "\n*\t*\t*\t*\t*\t*\t*";

		if (abs(a) >= pow((double)2, SIZE / 2) || abs(b) >= pow((double)2, SIZE / 2))
			cout << "수의 범위를 초과하여 곱셈 연산이 불가능합니다." << endl;
		else { // p, q는 16비트로 변환한 이진수
			int *p = convert_binary(a, q_register, SIZE / 2);
			int *q = convert_binary(b, m_register, SIZE / 2);

			int *result2 = mul(p, q, a_register, result_mul, SIZE / 2); // Q, M, A, A와 Q 합쳐서 출력하는 결과 레지스터

			cout << "\n곱셈 후: ";
			for (int i = 0; i < SIZE; i++)
				cout << result2[i];
			cout << endl;

			int test2 = convert_decimal(result2, SIZE); // 32비트의 십진수 변환
			cout << "\n십진수 변환 결과: " << test2 << endl;
		}
	}
	if (ieee_exception(x) && ieee_exception(y)) { //ieee 예외처리

		if (ieeea)
			ieee_multipler(x, y);
		if (ieeea)
			ieee_divider(x, y);
	}


	return;
}

void Signbit(int num) { // 계산한 값을 받음

	if (num<0)
		cout << "S bit : 1" << endl;
	else
		cout << "S bit : 0" << endl;

	return;

}


void Carrybit(int num) { //두 수의 맨 끝자리 수 하나만 받는다.

	if (num > 1)
		cout << "C bit : 1" << endl;
	else
		cout << "C bit : 0" << endl;

	return;

}


void Zerobit(int* num) { // 계산 완료된 값 받기

	for (int i = 0; i<SIZE - 1; i++) {
		if (num[i] == 0)
			continue;
		else {
			cout << "Z bit : 0" << endl;
			return;
		}
	}

	cout << "Z bit : 1" << endl;

	return;
}


void Overbit(int carry1, int carry2) { // 끝에서 두 번째판명된 C비트 받기

	if (carry1 + carry2>3) // 캐리값이 있다면 2 이상일 것임... 하나까진 괜찮으므로 3보다 클 경우엔 내부+외부 다 일어난 경우로 친다
		cout << "V bit : 1" << endl;
	else
		cout << "V bit : 0" << endl;

	return;

}
