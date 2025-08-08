#include <iostream>
#include <string>
#include <vector>
#include <random>
using namespace std;

class Cell
{
public:
    Cell() : is_mine(false), is_open(false), is_flag(false), neibor_mines(0)
    {

    }

    bool get_is_mine()
    { 
        return is_mine;
    }
    void set_mine()
    {
        is_mine = true;
    }
    bool get_is_open()
    { 
        return is_open;
    }
    void open_cell()
    {
        is_open = true;
    }
    bool get_is_flagged()
    {
        return is_flag;
    }
    void switch_flag()
    {
        if(is_flag == true)
        {
            is_flag = false;
        }
        else
        {
            is_flag = true;
        }
    }
    int get_neighbor_mines()
    { 
        return neibor_mines;
    }
    void set_neibor_mines(int num)
    {
        neibor_mines = num; 
    }


private:
    bool is_mine;
    bool is_open;
    bool is_flag;
    int neibor_mines;
};

class Board
{
public:
    Board(int row_constracter, int col_constracter, int num_mine_constracter)
        : row(row_constracter), col(col_constracter), num_mines(num_mine_constracter), grid(row_constracter, vector<Cell>(col_constracter))
    {

    }
    void display()
    {
        cout << "　　";
        for(int i = 0; i < col; i++)
        {
            if(i < 9)
            {
                cout << i + 1 << " ";
            }
            else
            {
                cout << i + 1;
            }
        }
        cout << endl << "　　";
        for(int i = 0; i < 2 * col; i++)
        {
            cout << "―";
        }
        cout << endl;

        for (int i = 0; i < row; i++)
        {
            if (i < 9)
            {
                cout << i + 1 << " ｜";
            }
            else
            {
                cout << i + 1 << "｜";
            }

            for (int j = 0; j < col; j++)
            {
                if (grid[i][j].get_is_open() == true)
                {
                    if (grid[i][j].get_is_mine() == true)
                    {
                        cout << "〇";
                    }
                    else
                    {
                        vector<string> conversion = { "　", "１", "２", "３", "４", "５", "６", "７", "８" };
                        cout << conversion[grid[i][j].get_neighbor_mines()];
                    }
                }
                else
                {
                    if (grid[i][j].get_is_flagged() == true)
                    {
                        cout << "！";
                    }
                    else
                    {
                        cout << "－";
                    }
                }         
            }
            cout << endl;
        }
    }

    void generate(int x, int y)
    {
        if (is_valid(x, y) == false)
        {
            cout << "無効なマスです" << endl;
        }
        else
        {
            random_device engine;
            uniform_int_distribution<unsigned> dist_raw(0, row - 1);
            uniform_int_distribution<unsigned> dist_col(0, col - 1);

            int mine = 0;
            while (mine < num_mines)
            {
                int rand_row = dist_raw(engine);
                int rand_col = dist_col(engine);
                if ((grid[rand_row][rand_col].get_is_mine() == false) && !((x - 1 <= rand_row) && (rand_row <= x + 1) && (y - 1 <= rand_col) && (rand_col <= y + 1)))
                {
                    grid[rand_row][rand_col].set_mine();
                    mine++;
                }
            }

            for (int i = 0; i < row; i++)
            {
                for (int j = 0; j < col; j++)
                {
                    int mine_count = 0;
                    for (int dr = -1; dr <= 1; dr++)
                    {
                        for (int dc = -1; dc <= 1; dc++)
                        {
                            if(is_valid(i + dr, j + dc))
                            {
                                if (grid[i + dr][j + dc].get_is_mine() == true)
                                {
                                    mine_count++;
                                }
                            }
                        }
                    }
                    grid[i][j].set_neibor_mines(mine_count);
                }
            }
        }
    }

    void flag(int x, int y)
    {
        if (is_valid(x, y))
        {
            grid[x][y].switch_flag();
        }
        else
        {
            cout << "無効なマスです" << endl;
        }
    }

    bool open(int x, int y)
    {
        if (is_valid(x, y) == false)
        {
            cout << "無効なマスです" << endl;
            return false;
        }
        if (grid[x][y].get_is_flagged() == true)
        {
            cout << "旗(！)が立っているマスです" << endl;
            return false;
        }
        if (grid[x][y].get_is_open() == true)
        {
            cout << "このマスはすでに開いています" << endl;
            return false;
        }
        if (grid[x][y].get_is_mine() == true)
        {
            grid[x][y].open_cell();
            cout << "Game Over!!" << endl;
            return true;
        }

        vector<vector<int>> open_cell;
        open_cell = { {x, y} };
        while (open_cell.size() > 0)
        {
            int current_x = open_cell[0][0];
            int current_y = open_cell[0][1];
            grid[current_x][current_y].open_cell();
            if (grid[current_x][current_y].get_neighbor_mines() == 0)
            {
                for (int dr = -1; dr <= 1; dr++)
                {
                    for (int dc = -1; dc <= 1; dc++)
                    {
                        if (is_valid(current_x + dr, current_y + dc))
                        {
                            if ((grid[current_x + dr][current_y + dc].get_is_open() == false) && (grid[current_x + dr][current_y + dc].get_neighbor_mines() == 0))
                            {
                                open_cell.push_back({current_x + dr, current_y + dc});
                            }
                            grid[current_x + dr][current_y + dc].open_cell();
                        }
                    }
                }

            }
            open_cell.erase(open_cell.begin());

        }
        return false;
    }

    bool clear_check()
    {
        int close_count = 0;
        for(int i= 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if(grid[i][j].get_is_open() == false)
                {
                    close_count++;
                }
            }
        }
        if(close_count == num_mines)
        {
            cout << "Game Clear" << endl;
            return true;
        }
        return false;
    }


private:
    int row;
    int col;
    int num_mines;
    vector<vector<Cell>> grid;
    bool is_valid(int x, int y)
    {
        return (x >= 0) && (x < row) && (y >= 0) && (y < col);
    }
    
};






void main()
{
    int row;
    int col;
    int x;
    int y;
    int num_mines;
    bool gameover = false;
    bool gameclear = false;
    int mode;
    int input =false;
    while (!input) 
    {
        cout << "盤面サイズ（縦）を入力　>" << flush;
        cin >> row;
        if((4 <= row) || (row <= 100))
        {
            input = true;
        }
        else
        {
            cout << "４～１００の間で入力してください" << endl;
        }
    }
    input = false;
    while (!input)
    {
        cout << "盤面サイズ（横）を入力　>" << flush;
        cin >> col;
        if ((4 <= col) || (col <= 100))
        {
            input = true;
        }
        else
        {
            cout << "４～１００の間で入力してください" << endl;
        }
    }
    input = false;
    while (!input)
    {
        cout << "爆弾の数を入力　>" << flush;
        cin >> num_mines;
        if ((0 < num_mines) || (num_mines <= row * col - 10))
        {
            input = true;
        }
        else
        {
            cout << "この数字は無効です" << endl;
        }
    }

    Board board(row, col, num_mines);
    board.display();

    cout << "マスを選択（縦）>" << flush;
    cin >> x;
    cout << "マスを選択（横）>" << flush;
    cin >> y;

    board.generate(x -1, y - 1);
    board.open(x - 1, y - 1);
    board.display();

    while ((gameover == false) && (gameclear == false))
    {
        cout << "操作を選択してください (1:開ける, 2:旗を立てる/外す)　> " << flush;
        cin >> mode;
        if (mode == 1)
        {
            cout << "マスを選択（縦）>" << flush;
            cin >> x;
            cout << "マスを選択（横）>" << flush;
            cin >> y;
            gameover = board.open(x - 1, y - 1);
            gameclear = board.clear_check();
            board.display();
        }
        else if (mode == 2)
        {
            cout << "マスを選択（縦）>" << flush;
            cin >> x;
            cout << "マスを選択（横）>" << flush;
            cin >> y;
            board.flag(x - 1, y - 1);
            board.display();
        }
        else
        {
            cout << "1か2を入力してください" << endl;
        }
    }

}




    