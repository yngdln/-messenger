#include "Matrix.h"
#include "Vector.h"
#include "Exceptions.h"
#include <iostream>
#include <iomanip>

CSRMatrix::CSRMatrix(double* p_valueArr, int p_valueSize, int* p_colsArr, int p_colsSize, int* p_rowsArr, int p_rowsSize)
{
	m_aelem = new double[p_valueSize];
	m_jptr= new int[p_colsSize];
	m_iptr = new int[p_rowsSize];
	m_valueSize = p_valueSize;
	m_colsSize = p_colsSize;
	m_rowsSize = p_rowsSize;

	for (int i = 0; i < m_valueSize; i++)
	{
		m_aelem[i] = p_valueArr[i];
	}
	for (int i = 0; i < m_colsSize; i++)
	{
		m_jptr[i] = p_colsArr[i];
	}
	for (int i = 0; i < m_rowsSize; i++)
	{
		m_iptr[i] = p_rowsArr[i];
	}
}


CSRMatrix::CSRMatrix(const CSRMatrix& other)
{
	m_aelem = new double[other.m_valueSize];
	m_jptr = new int[other.m_colsSize];
	m_iptr = new int[other.m_rowsSize];
	m_valueSize = other.m_valueSize;
	m_colsSize = other.m_colsSize;
	m_rowsSize = other.m_rowsSize;

	for (int i = 0; i < m_valueSize; i++)
	{
		m_aelem[i] = other.m_aelem[i];
	}
	for (int i = 0; i < m_colsSize; i++)
	{
		m_jptr[i] = other.m_jptr[i];
	}
	for (int i = 0; i < m_rowsSize; i++)
	{
		m_iptr[i] = other.m_iptr[i];
	}
}


CSRMatrix CSRMatrix::operator* (const double value) const
{
	CSRMatrix newMatrix(*this);
	for (int i = 0; i < newMatrix.m_valueSize; i++)
	{
		newMatrix.m_aelem[i] = newMatrix.m_aelem[i] * value;
	}
	return newMatrix;
}


CSRMatrix operator* (const double value, const CSRMatrix& another)
{
	return another * value;
}


Vector operator* (const CSRMatrix& matrix, const Vector& vector)
{
	if (matrix.RowsCount() != vector.Dimension())
	{
		throw IncompactibleDimException(matrix.RowsCount(), vector.Dimension());
	}
		Vector result(vector.Dimension(),0);
		double vecElem;
		for (int i = 0; i < vector.Dimension(); i++)
		{
			vecElem = 0;
			for (int j = matrix.m_iptr[i]; j < matrix.m_iptr[i + 1]-1; j++)
			{
				vecElem += matrix.m_aelem[j] * vector[matrix.m_jptr[j]];
			}
			result[i] = vecElem;
		}
		return result;
}


std::ostream& operator<< (std::ostream& os, const CSRMatrix& matrix)
{
	int index = 0;
	for (int i = 0; i < matrix.m_rowsSize - 1; i++)
	{
		for (int j = 0; j < matrix.m_rowsSize - 1; j++)
		{
			if (j == matrix.m_jptr[index] && index < matrix.m_iptr[i + 1] - 1)
			{
				os << std::fixed << std::setprecision(1) << std::setw(9) << matrix.m_aelem[index];
				index++;
			}
			else
			{
				os << std::fixed << std::setprecision(1) << std::setw(9) << 0;
			}
		}
		os << std::endl;
	}
	return os;
}


std::istream& operator>> (std::istream& is, CSRMatrix& matrix)
{
	std::cout << "Enter size of values array: ";
	int vs;
	is >> vs;
	if (vs != matrix.m_valueSize)
	{
		matrix.m_valueSize = vs;
		delete[] matrix.m_aelem;
		matrix.m_aelem = new double[vs];
	}
	std::cout << "Enter the values of matrix:\n";
	for (int i = 0; i < vs; i++)
	{
		is >> matrix.m_aelem[i];
	}

	std::cout << "Enter size of columns array: ";
	int cs;
	is >> cs;
	if (cs != matrix.m_colsSize)
	{
		matrix.m_colsSize = cs;
		delete[] matrix.m_aelem;
		matrix.m_jptr = new int[cs];
	}
	std::cout << "Enter the column indexes:\n";
	for (int i = 0; i < cs; i++)
	{
		is >> matrix.m_jptr[i];
	}

	std::cout << "Enter size of rows array: ";
	int rs;
	is >> rs;
	if (rs != matrix.m_rowsSize)
	{
		matrix.m_rowsSize = rs;
		delete[] matrix.m_iptr;
		matrix.m_iptr = new int[rs];
	}
	std::cout << "Enter the row indexes:\n";
	for (int i = 0; i < rs; i++)
	{
		is >> matrix.m_iptr[i];
	}

	return is;
}


int CSRMatrix::RowsCount() const
{
	return m_rowsSize - 1;
}


int CSRMatrix::ColsCount() const
{
	return m_colsSize;
}


int CSRMatrix::NonZeroCount() const
{
	return m_valueSize;
}


double* CSRMatrix::NonZeroArray() const
{
	return m_aelem;
}


int* CSRMatrix::ColsArray() const
{
	return m_jptr;
}


int* CSRMatrix::RowsArray() const
{
	return m_iptr;
}


CSRMatrix::~CSRMatrix()
{
	delete[] m_aelem;
	delete[] m_jptr;
	delete[] m_iptr;
}


