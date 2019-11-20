//
// Created by Andy on 19.11.2019.
//

#include "mLife.h"


mCell& operator++ (mCell& item, int)
{
    item.value++;
    return item;
}

int mCell::getvalue() const
{
    return value;
}

void mCell::SetValue(int val)
{
    value = val;
}

bool mGeneration::isAliveExist()
{
    return !space.empty();
}


vector<int> mGeneration::neighbours(int item)
{
    vector<int> ret;
    pair<int, int> indexs = convert_single_index_to_double(item);
    //cout << "neighbours el = " << item << " i = " << indexs.first << " j = " << indexs.second << endl;

    vector<int> steps_inline = {1, -1};
    vector<int> steps_downline = {m-1, m, m+1};
    vector<int> steps_upline = {-m-1, -m, -m+1};

    int element;

    for (int i=0; i<static_cast<int>(steps_inline.size()); i++)
    {
        element = item + steps_inline[i];
        if ( (element >= 0) & (element < msize) )
        {
            ret.push_back(element);
        }
    }

    for (int i=0; i<static_cast<int>(steps_upline.size()); i++)
    {
        element = item + steps_upline[i];
        if ( (element >= 0) & (element < msize) & ( element/m == (indexs.first - 1) ))
        {
            ret.push_back(element);
        }
    }

    for (int i=0; i<static_cast<int>(steps_downline.size()); i++)
    {
        element = item + steps_downline[i];
        if ( (element >= 0) & (element < msize) & (element/m == (indexs.first + 1) ))
        {
            ret.push_back(element);
        }
    }


    return ret;
}

void mGeneration::NextGen()
{
    mapCell potentialCell;
    mapCell::iterator itr;
    vector<int> item;
    const int countCellForAlive = 3;
    const int minCellForAlive = 2;
    const int maxCellForAlive = 3;


    for (auto it = space.begin(); it != space.end(); ++it)
    {
        //cout << " potentialCell element = " << it->first << " neighbours = ";
        item = neighbours(it->first);
        for (auto i: item)
        {
            itr = space.find(i);
            if (itr != space.end())
            {
                continue;
            }
            itr = potentialCell.find(i);
            if (itr == potentialCell.end())
            {
                potentialCell.emplace(pair<int, mCell>(i, mCell(1) ) );
            } else
            {
                potentialCell[i]++;
            }
            //cout << i << ' ';
        }
        //cout << endl;
    }

    for (auto it = space.begin(); it != space.end(); ++it)
    {
        //cout << " space element = " << it->first << " neighbours = ";
        item = neighbours(it->first);
        for (auto i: item)
        {
            itr = space.find(i);
            if (itr != space.end())
            {
                it->second++;
            }
            //cout << i << ' ';
        }
        //cout << " value = " << it->second.getvalue() << " isNextGen = " << it->second.GetIsNexGen() << endl;
    }


    for (auto it = space.begin(); it != space.end();)
    {
        if ((it->second.getvalue() < minCellForAlive) || (it->second.getvalue() > maxCellForAlive) )
        {
            it = space.erase(it);
        } else
        {
            space[it->first].SetValue(0);
            ++it;
        }
    }

    for (auto it = potentialCell.begin(); it != potentialCell.end();)
    {
        if (it->second.getvalue() != countCellForAlive)
        {
            it = potentialCell.erase(it);
        } else
        {
            //cout << " potentialCell index = " << it->first << " value = " << it->second.getvalue() << endl;
            space.emplace(pair<int, mCell>(it->first, mCell(0)));
            ++it;
        }
    }
}


void mGeneration::printSpace()
{


    /* for (std::pair<int, mCell> element : space)
     {
         cout << " index = " << element.first << " value = " << element.second.getvalue() << endl;
     }*/

    system("clear");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cor = {0,0};
    cor.X = 0;
    cor.Y = 0;
    SetConsoleCursorPosition(hConsole,cor);



    mapCell::iterator itr;


    string strout = "";

    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
        {
            int index = convert_double_index_to_single(i,j);
            itr = space.find(index);

            if (itr == space.end())
            {
                strout += mCell::DeadSymbol;
            } else
            {
                strout += mCell::AliveSymbol;
            }
        }
        //cout << endl;
        strout += '\n';
    }
    cout << strout;
}


mGeneration::mGeneration(string FileName)
{
    ifstream in(FileName);
    string str = "";
    int string_num = -1;
    if (in.is_open())
    {
        while (getline(in, str))
        {
            if (string_num == -1)
            {
                istringstream sstr(str);
                istream_iterator<string> it(sstr);
                n = stoi(*it);
                m = stoi(*(++it));
                msize = m*n;
                //cout << " n = " << n << " m = " << m << endl;
            }else
            {
                parseString(str, string_num);
            }
            string_num++;
            //cout << str << endl;
        }
        in.close();
    }
}

int mGeneration::convert_double_index_to_single(int a, int b)
{
    return a*m + b;
}

pair<int, int>  mGeneration::convert_single_index_to_double(int index)
{
    pair<int, int>  ret;
    ret.first =  index/m;
    ret.second = index - ret.first*m;

    return ret;
}


void mGeneration::parseString(string item, int line_number)
{
    for (int i=0; i < static_cast<int>(item.size()); i++)
    {
        if (item[i] == mCell::AliveSymbol)
        {
            //cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " alive" << endl;
            int index = convert_double_index_to_single(line_number, i);
            space.emplace (pair<int, mCell>(index, mCell() ) );
        }
        if (item[i] == mCell::DeadSymbol)
        {
            //cout << " item = " << item[i] << " line = " << line_number << " row = " << i << " dead" << endl;
        }
    }
}




//mLife::mLife(): mLife::mLife(1, 1) {};


mLife::mLife(const int32_t rows, const int32_t columns) {
    board = std::vector<std::vector<bool>>(rows, std::vector<bool>(columns, false));
}

void mLife::set(int32_t row, int32_t column, bool value) {
    board[row][column] = value;
}

int32_t mLife::numb_of_alive_neighboors(int32_t row, int32_t column)
{
    int32_t res = 0;
    int32_t di[] = {-1, -1, -1,  0, 0,  1, 1, 1};
    int32_t dj[] = {-1,  0,  1, -1, 1, -1, 0, 1};
    for (int32_t l = 0; l < 8; ++l) {
        if (isAlive(row + di[l], column + dj[l])) {
            res++;
        }
    }
    return res;
}

mLife::mLife(string FileName)
{
    ifstream in(FileName, std::ios::in);
    string str = "";
    //int string_num = -1;
    if (in.is_open())
    {
        in >> m >> n;
        //cout << " m = " << m << " n = " << n << endl;
        in.get();
        in.get();
        //cout << "ichar = " << (char)in.get() << endl;

        std::vector<std::vector<bool>> iboard(m);
        for (int32_t row = 0; row < m; ++row) {
            iboard[row].resize(n, false);
        }

        for (int32_t row = 0; row < m; ++row) {
            for (int32_t column = 0; column < n; ++column) {
                char point = in.get();
                //cout << " row = " << row <<  " column = " << column << " ichar = " << point << endl;
                if (point == '*') {
                    iboard[row][column] = 1;
                } else if (point == '_'){
                    iboard[row][column] = 0;
                }

            }
            in.get();
            in.get();
        }
        //board.assign(iboard.begin(), iboard.end());
       // std::copy(iboard.begin(), iboard.end(), board.begin());
        board = move(iboard);
        in.close();
    }
}

void mLife::NextGen()
{
    auto copy_board = board;
   // cout << " m = " << m << " n = " << n << endl;
    for (int32_t row = 0; row < m; ++row) {
        for (int32_t column = 0; column < n; ++column) {
            const auto alives = numb_of_alive_neighboors(row, column);
            if (board[row][column]) {
                if (alives == 3 || alives == 2) {
                    copy_board[row][column] = true;
                } else {
                    copy_board[row][column] = false;
                }
            } else {
                if (alives == 3) {
                    copy_board[row][column] = true;
                }
            }
        }
    }
    board = copy_board;
}

bool mLife::isAlive(int32_t row, int32_t column)
{
   if ((row >= m) || (column >= n))
    {
        return false;
    }

    if ((row < 0) || (column < 0))
    {
        return false;
    }


    return board[row][column];
}

bool mLife::isAliveExist()
{
    for(int32_t i=0; i < m; ++i)
    {
        for(int32_t j=0; j < n; ++j)
        {
            if (isAlive(i, j))
            {
                return true;
            }
        }
    }
    return true;
}

void mLife::printSpace()
{
    system("clear");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cor = {0,0};
    cor.X = 0;
    cor.Y = 0;
    SetConsoleCursorPosition(hConsole,cor);



    string strout = "";

    for (int32_t i = 0; i < m; ++i)
    {
        for (int32_t j = 0; j < n; ++j)
        {
          if (isAlive(i, j))
          {
              strout += "*";
          }else
          {
              strout += "_";
          }
        }
        //cout << endl;
        strout += '\n';
    }
    cout << strout;


}