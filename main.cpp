#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

using namespace std;

bool read(string & s1, char & op, string & s2) {
	bool f = false;
	string line;
	getline(cin, line);
	istringstream stream(line);
	if (stream >> s1 && stream >> op) {
		if (op == 'T' || op == 'R') {
			f = !f;
			return f;
		}
		else {
			if (stream >> s2) {
				f = !f;
			}
		}
	}
	return f;
}

bool input(float ** & matrix, unsigned int & rows, unsigned int & columns, string s) {
	char op;
	ifstream fin;
	fin.open(s.c_str());
	if (!fin.is_open()) {
		cout << "file isn't opened" << endl;
		return false;
	}
	string line;
	getline(fin, line);
	istringstream stream(line);
	if (stream >> rows && stream >> op && op == ',' && stream >> columns) {
		for (unsigned int i = 0; i<rows; i++) {
			for (unsigned int j = 0; j<columns; j++) {
				fin >> matrix[i][j];
			}
		}
	}
	fin.close();
	return true;
}

bool multiply(float **lhs_elements, unsigned int lhs_rows, unsigned int lhs_columns, float **rhs_elements, unsigned int rhs_rows, unsigned int rhs_columns, float ** & result_elements, unsigned int & result_rows, unsigned int & result_columns) {
	if (lhs_columns != rhs_rows) {
		return false;
	}
	result_rows = lhs_rows; result_columns = rhs_columns;
	result_elements = new float *[result_rows];
	for (unsigned int i = 0; i<result_rows; ++i) {
		result_elements[i] = new float[result_columns];
		for (unsigned int j = 0; j<result_columns; ++j) {
			float result_op = 0;
			for (unsigned int k = 0; k<lhs_columns; ++k) {
				result_op += lhs_elements[i][k] * rhs_elements[k][j];
			}
			result_elements[i][j] = result_op;
		}
	}
	return true;
}

bool add(float **lhs_elements, unsigned int lhs_rows, unsigned int lhs_columns, float **rhs_elements, unsigned int rhs_rows, unsigned int rhs_columns, float ** & result_elements, unsigned int & result_rows, unsigned int & result_columns) {
	if (lhs_rows != rhs_rows || lhs_columns != rhs_columns) {
		return false;
	}
	result_rows = lhs_rows; result_columns = lhs_columns;
	result_elements = new float *[result_rows];
	for (unsigned int i = 0; i<result_rows; ++i) {
		result_elements[i] = new float[result_columns];
		for (unsigned int j = 0; j<result_columns; ++j) {
			result_elements[i][j] = lhs_elements[i][j] + rhs_elements[i][j];
		}
	}
	return true;
}

bool sub(float **lhs_elements, unsigned int lhs_rows, unsigned int lhs_columns, float **rhs_elements, unsigned int rhs_rows, unsigned int rhs_columns, float ** & result_elements, unsigned int & result_rows, unsigned int & result_columns) {
	if (lhs_rows != rhs_rows || lhs_columns != rhs_columns) {
		return false;
	}
	result_rows = lhs_rows; result_columns = lhs_columns;
	result_elements = new float *[result_rows];
	for (unsigned int i = 0; i<result_rows; ++i) {
		result_elements[i] = new float[result_columns];
		for (unsigned int j = 0; j<result_columns; ++j) {
			result_elements[i][j] = lhs_elements[i][j] - rhs_elements[i][j];
		}
	}
	return true;
}

bool transpose(float **A, unsigned int rows, unsigned int columns, float ** & result_elements, unsigned int & result_rows, unsigned int & result_columns) {
	result_rows = columns; result_columns = rows;
	result_elements = new float *[result_rows];
	for (unsigned int i = 0; i<result_rows; ++i) {
		result_elements[i] = new float[result_columns];
		for (unsigned int j = 0; j<result_columns; ++j) {
			result_elements[i][j] = A[j][i];
		}
	}
	return true;
}

bool reverse(float **A, unsigned int rows, unsigned int columns, float ** & result_elements, unsigned int & result_rows, unsigned int & result_columns, bool & f) {
	if (rows != columns) {
		return false;
	}
	result_rows = rows; result_columns = columns;
	result_elements = new float *[result_rows];
	for (unsigned int i = 0; i<result_rows; ++i) {
		result_elements[i] = new float[result_columns];
	}
	float temp;
	float **E = new float *[rows];
	for (unsigned int i = 0; i < rows; i++) {
		E[i] = new float[rows];
	}
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < rows; j++) {
			E[i][j] = 0;
			if (i == j) {
				E[i][j] = 1;
			}
		}
	}
	for (unsigned int k = 0; k < rows; k++) {
		temp = A[k][k];
		if (temp == 0) {
			f = 1;
			return true;
		}
		for (unsigned int j = 0; j < rows; j++) {
			A[k][j] /= temp;
			E[k][j] /= temp;
		}
		for (unsigned int i = k + 1; i < rows; i++) {
			temp = A[i][k];
			for (unsigned int j = 0; j < rows; j++) {
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}
	for (int k = rows - 1; k > 0; k--) {
		for (int i = k - 1; i >= 0; i--) {
			temp = A[i][k];
			for (unsigned int j = 0; j < rows; j++) {
				A[i][j] -= A[k][j] * temp;
				E[i][j] -= E[k][j] * temp;
			}
		}
	}
	for (unsigned int i = 0; i < rows; i++) {
		for (unsigned int j = 0; j < rows; j++) {
			A[i][j] = E[i][j];
			result_elements[i][j] = A[i][j];
		}
	}
	return true;
}

void destroy(float ** & matrix, unsigned int & rows) {
	for (unsigned int i = 0; i<rows; ++i) {
		delete[]matrix[i];
	}
	delete[]matrix;
}

int main() {
	string s1, s2;
	char op;
	if (!(read(s1, op, s2))) {
		cout << "An error has occured while reading input data" << endl;
		return 10;
	}
	float **lhs_elements;
	unsigned int lhs_rows, lhs_columns;
	if (!(input(lhs_elements, lhs_rows, lhs_columns, s1))) {
		cout << "An error has occured while reading input data" << endl;
		return -1;
	}
	if ((op != '+') && (op != '-') && (op != '*') && (op != 'R') && (op != 'T')) {
		cout << "An error has occured while reading input data" << endl;
		return -2;
	}
	float **result_elements;
	unsigned int result_rows, result_columns;
	switch (op) {
	case '+': {
		float **rhs_elements;
		unsigned int rhs_rows, rhs_columns;
		if (!(input(rhs_elements, rhs_rows, rhs_columns, s2))) {
			cout << "An error has occured while reading input data" << endl;
			return -3;
		}
		if (!(add(lhs_elements, lhs_rows, lhs_columns, rhs_elements, rhs_rows, rhs_columns, result_elements, result_rows, result_columns))) {
			cout << "An error has occured while reading input data" << endl;
			return -10;
		}
		break;
	}
	case '-': {
		float **rhs_elements;
		unsigned int rhs_rows, rhs_columns;
		if (!(input(rhs_elements, rhs_rows, rhs_columns, s2))) {
			cout << "An error has occured while reading input data" << endl;
			return -3;
		}
		if (!(sub(lhs_elements, lhs_rows, lhs_columns, rhs_elements, rhs_rows, rhs_columns, result_elements, result_rows, result_columns))) {
			cout << "An error has occured while reading input data" << endl;
			return -10;
		}
		break;
	}
	case '*': {
		float **rhs_elements;
		unsigned int rhs_rows, rhs_columns;
		if (!(input(rhs_elements, rhs_rows, rhs_columns, s2))) {
			cout << "An error has occured while reading input data" << endl;
			return -3;
		}
		if (!(multiply(lhs_elements, lhs_rows, lhs_columns, rhs_elements, rhs_rows, rhs_columns, result_elements, result_rows, result_columns))) {
			cout << "An error has occured while reading input data" << endl;
			return -10;
		}
		break;
	}
	case 'T': {
		if (!(transpose(lhs_elements, lhs_rows, lhs_columns, result_elements, result_rows, result_columns))) {
			cout << "An error has occured while reading input data" << endl;
			return -10;
		}
		break;
	}
	case 'R': {
		bool f = 0;
		if (!(reverse(lhs_elements, lhs_rows, lhs_columns, result_elements, result_rows, result_columns, f))) {
			cout << "An error has occured while reading input data" << endl;
			return -10;
		}
		if (f) {
			cout << "There is no reverse matrix" << endl;
			return -20;
		}
		break;
	}
	}
	destroy(lhs_elements, lhs_rows);
	for (unsigned int i = 0; i<result_rows; ++i) {
		for (unsigned int j = 0; j<result_columns; ++j) {
			cout << result_elements[i][j] << " ";
		}
		cout << endl;
	}
	destroy(result_elements, result_rows);
	return 0;
}
