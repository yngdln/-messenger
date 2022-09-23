#pragma once
#include <iostream>
class Vector;
class CSRMatrix
{
public:
	CSRMatrix(double* valueArray, int valuesize, int* colsArray, int colsSize, int* rowsArray, int rowssize);
	CSRMatrix(const CSRMatrix& other);
	~CSRMatrix();
	CSRMatrix operator* (const double value) const;
	friend Vector operator*(const CSRMatrix& matrix,const Vector& vector);
	friend std::ostream& operator<< (std::ostream& os, const CSRMatrix& matrix);
	friend std::istream& operator>> (std::istream& is, CSRMatrix& matrix);
	int NonZeroCount() const;
	int ColsCount() const;
	int RowsCount() const;
	double* NonZeroArray() const;
	int* ColsArray() const;
	int* RowsArray() const;
private:
	double* m_aelem;
	int* m_iptr;
	int* m_jptr;
	int m_valueSize;
	int m_colsSize;
	int m_rowsSize;

};

CSRMatrix operator* (const double value, const CSRMatrix& other);