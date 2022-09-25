#pragma once
#include <iostream>
#include <exception>
#include <string>

class OutOfRangeException : public std::exception
{
private:
    std::string m_error;

public:
    OutOfRangeException(const std::string& error) : m_error(error)
    {
    }
    const char* what() const noexcept override
    {
        return m_error.c_str();
    }
};

class IteratorException : public std::exception
{
private:
    std::string m_error;
public:
    IteratorException(const std::string& error) : m_error(error)
    {
    }
    const char* what() const noexcept override
    {
        return m_error.c_str();
    }
};
