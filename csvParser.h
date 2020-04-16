//
// Created by andy on 15.03.2020.
//

#ifndef LAB4_CSVPARSER_H
#define LAB4_CSVPARSER_H


#include <tuple>
#include <iostream>
#include <string>
#include <fstream>
#include <utility>
#include <type_traits>
#include <string>
#include <sstream>




using namespace std;


namespace tuple_print {
    template <size_t index, typename ... Args>
    void tuple_printer(std::ostream& stream, const std::tuple<Args...>& my_tuple) {
        stream << std::get<index>(my_tuple);
        if constexpr (index < sizeof...(Args) - 1) {
            stream << ' ';
            tuple_printer < index + 1 > (stream, my_tuple);
        }
    }
}

template <typename ... Args>
std::ostream& operator<<(std::ostream& stream, const std::tuple<Args...>& my_tuple) {
    tuple_print::tuple_printer<0>(stream, my_tuple);
    return stream;
}



template<typename... Args>
class csvParser {

    public:
        csvParser(ifstream& file, uint32_t cntSkipLine, char sepLineChar = '\n', char sepColChar = ',', char shieldchar = '"'):
                                        csvFile(file),
                                        curLineIndex(cntSkipLine),
                                        colCounts(sizeof...(Args)),
                                        mEOF(false),
                                        mi(0),
                                        EOL_char(sepLineChar),
                                        separatorCol_char(sepColChar),
                                        shield_char(shieldchar)
        {
           for (uint32_t i=0;i< cntSkipLine; i++)
           {
               getline(csvFile, curLine, EOL_char);
               if (csvFile.eof()) break;
           }
        };

        class mIterator{
        public:
            explicit mIterator(csvParser<Args...> *parser = nullptr): item(parser) {};
            mIterator& operator++()
            {
                if(item)
                {
                    item->getLine();
                }
                return *this;
            }
            const tuple<Args...>& operator*()
            {
                return item->parseLine();
            }
            bool operator != (const mIterator& other)
            {
                return !(item->mEOF &&  other.item == nullptr);
            }
        private:
            csvParser<Args...>* item;
        };

        mIterator begin()
        {
            mEOF = false;
            getLine();
            return mIterator(this);
        }

        mIterator end()
        {
            return mIterator();
        }

        ~csvParser() {}

private:
    void getLine()
    {
        mi++;
        if (!csvFile.eof())
        {
            getline(csvFile, curLine, EOL_char);
        } else
        {
            mEOF = true;
        }

    }

    tuple<Args...>& parseLine()
    {
        fillTuple(std::make_index_sequence<sizeof...(Args)> {}); // size_t { 1, 2}
        return mConteiner;
    }

    //tuple<string, int>
    template <size_t... items>
    void fillTuple(std::index_sequence<items...> sequence) {
        ( (cout << items << endl), ... );
        leftIndex = 0;
        rightIndex = 0;
        ( ParseLineToTupleItem(std::get<items>(mConteiner), items) , ... );
        //ParseLineToTupleItem(string &item, 0);
        //ParseLineToTupleItem(int &item, 1);
    }


    template<typename T>
    void ParseLineToTupleItem(T &val, const size_t n)
    {

/*
        T output{};
        //string &output();
        std::stringstream ss;


        if (typeid(T).name() == typeid(std::string).name())
        {

            ss << curLine << 'a';

            if ( !(ss >> output) )
            {
                cout << "WTF1" << endl;
            }


            //output = curLine;
//          val = make_tuple(curLine);
//          cout << "curline = " << curLine << " ss.str() = " << ss.str() << endl;
            cout << "i = " << mi << " line = " << output << endl;
        } else if ( typeid(T).name() == typeid(int).name() )
        {
            ss << mi;

            if ( !(ss >> output) )
            {
                cout << "WTF2" << endl;
            }


            //val = mi;
        }

        val = output;

*/

        if (n != colCounts - 1) {

            if (curLine[leftIndex] == shield_char) {
                rightIndex = curLine.find(shield_char, leftIndex + 1) + 1;
            } else {
                rightIndex = curLine.find(separatorCol_char, leftIndex);
            }

            if (rightIndex == -1 && n != colCounts - 1) {
                throw std::runtime_error(std::string("separator error \"" + curLine + "\""));
            }
        } else
        {

            if (curLine.find(separatorCol_char, leftIndex) != -1)
            {
                throw std::runtime_error(std::string("no item for element of tuple \"" + curLine + "\" pos : "));
            }
            rightIndex = curLine.size();
        }

        std::string current_str = curLine.substr(leftIndex, rightIndex - leftIndex);
        std::stringstream ss(current_str);
        T output{};
        if (!(ss >> output)) {
            throw std::runtime_error(std::string("parse error \"" + curLine + "\" pos : ") + std::to_string(rightIndex));
        }
        leftIndex = rightIndex + 1;
        val = output;

    }


/*
    void ParseLineToTupleItem(string &val, const size_t n)
    {
        val = curLine;
    }

    void ParseLineToTupleItem(int &val, const size_t n)
    {
        val = mi;
    }
*/

    size_t colCounts;
    tuple<Args...> mConteiner;
    uint32_t mi = 0;
    uint32_t curLineIndex, leftIndex = 0, rightIndex = 0;
    bool mEOF;

    string curLine;

    char separatorCol_char, shield_char, EOL_char;
    ifstream& csvFile;
};




#endif //LAB4_CSVPARSER_H
