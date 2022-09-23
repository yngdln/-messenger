#pragma once
#include "Vector.h"
#include <iomanip> 

Vector::Vector(int p_size, double p_startvalue)
{
	m_size = p_size;
	m_vec = new double[p_size];
	for (int i = 0; i < m_size; i++)
	{
		m_vec[i] = p_startvalue;
	}
}


Vector::Vector(const Vector &other)
{
	m_vec = new double[other.m_size];
	m_size = other.m_size;
	for (int i = 0; i < m_size; i++)
	{
		this->m_vec[i] = other.m_vec[i];
	}

}


Vector::~Vector()
{
	delete[] m_vec;
}


Vector & Vector::operator=(const Vector & other)
{
	if (this == &other)  
	{
		return *this;
	}

	if (m_size != other.m_size)
	{
		delete[] m_vec;
		m_size = other.m_size;
		m_vec = new double[m_size];
	}

	for (int i = 0; i < m_size; i++)
	{
		m_vec[i] = other.m_vec[i];
	}

	return *this;

}


Vector & Vector::operator+=(const Vector & other)
{
	if (other.m_size != m_size)
	{
		throw IncompactibleDimException(m_size, other.m_size);
	}

	for (int i = 0; i <= other.m_size; i++)
	{
		m_vec[i] += other.m_vec[i];
	}
	return *this;
}


Vector & Vector::operator-=(const Vector & other)
{
	if (other.m_size != m_size)
	{
		throw IncompactibleDimException(m_size, other.m_size);
	}

	for (int i = 0; i <= other.m_size; i++)
	{
		m_vec[i] -= other.m_vec[i];
	}
	return *this;
}


Vector operator +(const Vector &cur, const Vector &other)
{
	Vector sum(cur);
	return sum+=other;
}


Vector operator -(const Vector &cur, const Vector &other)
{
	Vector sum(cur);
	return sum-=other;
}


Vector& Vector:: operator +()
{
	return this;
}


Vector Vector::operator- ()
{
	Vector negativeArray(*this);
	for (int i = 0; i < m_size; i++) {
		negativeArray.m_vec[i] = -negativeArray.m_vec[i];
	}
	return negativeArray;
}


Vector operator* (const Vector& cur, double scalar)
{
	Vector product(cur);
	for (int i = 0; i < cur.m_size; i++) {
		product.m_vec[i] = product.m_vec[i]*scalar;
	}
	return product;
}


double operator* (const Vector &cur,const Vector &other)
{
	if (cur.m_size != other.m_size)
	{
		throw IncompactibleDimException(cur.m_size, other.m_size);
	}
	double scalar=0;
	for (int i = 0; i < cur.m_size; i++)
	{
		scalar = scalar+cur.m_vec[i] * other.m_vec[i];
	}
	return scalar;
}


Vector operator* (double value, const Vector &other)
{
	return other*value;
}


Vector operator* (const Vector& vector,const CSRMatrix& matrix)
{
	if (matrix.RowsCount()  != vector.Dimension())
	{
		throw IncompactibleDimException(matrix.RowsCount(), vector.Dimension());
	}
	double* aelem = new double[matrix.NonZeroCount()];
	int* jptr = new int[matrix.ColsCount()];
	int* iptr = new int[matrix.RowsCount()+1];
	int valueSize = matrix.NonZeroCount();
	int colsSize = matrix.ColsCount();
	int rowsSize = matrix.RowsCount();

	for (int i = 0; i < valueSize; i++)
	{
		aelem[i] = matrix.NonZeroArray()[i];
	}
	for (int i = 0; i < colsSize; i++)
	{
		jptr[i] = matrix.ColsArray()[i];
	}
	for (int i = 0; i < rowsSize+1; i++)
	{
		iptr[i] = matrix.RowsArray()[i];
	}
	Vector result(rowsSize,0);
	double vecElem;
	for (int i = 0; i <rowsSize; i++)
	{
		vecElem = 0;
		for (int j = iptr[i]; j < iptr[i + 1] - 1; j++)
		{
			vecElem += aelem[j] * vector[jptr[j]];
		}
		result[i] = vecElem;
	}
	delete[] aelem;
	delete[] iptr;
	delete[] jptr;
	return result;
}


const double& Vector::operator [] (int index) const
{
	if (index < 0 || index > m_size - 1)
	{
		throw OutOfRangeException(0, m_size, index);
	}
	return m_vec[index];
}


double& Vector::operator [](const int index)
{
	if (index < 0 || index > m_size - 1)
	{
		throw OutOfRangeException(0, m_size, index);
	}
	return m_vec[index];
}


std::ostream& operator<< (std::ostream& os, const Vector& other)
{
	for (int i = 0; i < other.m_size; i++)
	{
		os << std::setw(10) << other.m_vec[i];
	}
	return os;
}


std::istream& operator>>(std::istream& is, Vector& other)
{
	int newSize;
	std::cout << "Enter size of vector: ";
	is >> newSize;
	if (other.m_size != newSize)
	{
		delete[] other.m_vec;
		other.m_size = newSize;
		other.m_vec = new double[newSize];
	}
	std::cout << "Enter values of your vector:\n";
	for (int i = 0; i < other.m_size; i++)
	{
		is >> other.m_vec[i];
	}
	return is;
}


Vector::operator double* ()
{
	return m_vec;
}


double Vector::VectorNorm(const Vector& vector)
{
	double norm = 0;
	for (int i = 0; i < m_size; i++)
	{
		norm += vector.m_vec[i] * vector.m_vec[i];
	}
	return sqrt(norm);
}


double Vector::LenCalc()
{
	double len=0;
	for (int i = 0; i < m_size; i++)
	{
		len = len + m_vec[i];
	}
	return len;
}


int Vector::Dimension() const
{
	return m_size;
}
