#pragma once
#include <iostream>
#include "Container.h"
#include "Algorithms.h"
#include "Exception.h"
using namespace std;
struct Goods
{
    uint32_t m_id;
    std::string m_name;
    std::string m_manufacturer;
    std::string m_warehouse_address;
    double m_weight;

    Goods(uint32_t id = 0,
        const std::string& name = " ",
        const std::string& manufacturer = " ",
        const std::string& address = " ",
        double weight = 0.) : m_id(id),
        m_name(name),
        m_manufacturer(manufacturer),
        m_warehouse_address(address),
        m_weight(weight)
    {}
    bool operator<(const Goods& other) {
        return m_name < other.m_name;
    }
    bool operator>(const Goods& other) {
        return m_name > other.m_name;
    }
};

class functor
{
public:
    bool operator() (const Goods& goods)
    {
        return goods.m_manufacturer == "Manuf_1";
    };
};

int main()
{
    try
    {
        auto printInfo = [](const auto& good)
        {
            std::cout << "ID is " << good.m_id << std::endl;
            std::cout << "Name is " << good.m_name << std::endl;
            std::cout << "Manufacturer is " << good.m_manufacturer << std::endl;
            std::cout << "Adress is " << good.m_warehouse_address << std::endl;
            std::cout << "Weight is " << good.m_weight << std::endl << std::endl;
        };


        Vector<Goods> cont_1{ {1,"Name_2","Manuf_1","Adress_1",31},
                              {2,"Name_3","Manuf_2","Adress_2",100},
                              {3,"Name_1","Manuf_3","Adress_3",51} };
        std::cout << "\nFirst container content:\n\n";
        forEach(cont_1.begin(), cont_1.end(), printInfo);


        Goods good1{ 4,"Name_4","Manuf_4","Adress_4", 600 };
        Goods good2{ 5,"Name_6","Manuf_1","Adress_1", 100 };
        Goods good3{ 6,"Name_5","Manuf_5","Adress_6", 513 };
        Vector<Goods> cont_2;

        cont_2.pushBack(good1);
        cont_2.pushBack(good2);
        cont_2.insert(cont_2.end() - 1, good3);
        std::cout << "\nEmpty second container before insertion of 3 elements:\n\n";
        forEach(cont_2.begin(), cont_2.end(), printInfo);


        cont_1.insert(cont_1.end(), cont_2.begin(), cont_2.end());
        std::cout << "\nFirst container before insertion of elements of the second container at the end of them:\n\n";
        forEach(cont_1.begin(), cont_1.end(), printInfo);


        Sort(cont_1.begin(), cont_1.end());
        std::cout << "\nThe first container sorted by good name\n\n";
        forEach(cont_1.begin(), cont_1.end(), printInfo);


        std::cout << "\nThe first good in the first container with a weight of at least 200\n\n";
        Vector<Goods>::Iterator res = findIf(cont_1.begin(), cont_1.end(), [](const auto& good) {return good.m_weight >= 200; });
        printInfo(*res);


        res->m_warehouse_address = "New_Adress";
        std::cout << "\nThe first container with new adress of found good:\n\n";
        forEach(cont_1.begin(), cont_1.end(), printInfo);


        Vector<Goods> cont_3(3, Goods());
        copyIf(cont_1.begin(), cont_1.end(), cont_3.begin(), functor());
        std::cout << "\nThe new container with goods (from first container) from the same manufacturer (Manuf_1):\n\n";
        forEach(cont_3.begin(), cont_3.end(), printInfo);
    }

    catch (OutOfRangeException& exception)
    {
        std::cerr << "Out of range error: " << exception.what() << ".\n";
    }

    catch (IteratorException& exception)
    {
        std::cerr << "Iterator error: " << exception.what() << ".\n";
    }

    catch (std::exception& exception)
    {
        std::cerr << "Error:" << exception.what() << ").\n";
    }

    catch (...)
    {
        std::cout << "Error!";
    }
    return 0;
}
