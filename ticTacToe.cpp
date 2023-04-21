#include <iostream>
#include <unistd.h>

const char ai = 'x', player = 'o';
const int boardSize = 3;

struct Move
{
    int row;
    int column;
};

char** CreateBoard();
void ClearMemory(char** board);
void Game(char** board);
void InitializeBoard(char **board);
void InitializeGame(char **board);
void PrintBoard(char **board);
bool IsMoveLeft(char **board);
int Evaluate(char **board);
int MiniMax(char **board, int depth, bool isMaximizing);
void BestMove(char **board);
void PlayerMove(char **board);
void AiMove(char **board);

int main()
{
    char **board = CreateBoard();

    Game(board);

    ClearMemory(board);
}

bool IsMoveLeft(char **board) //checking if there are empty spots on the board
{
    for (int i = 0; i < boardSize; i++)
        for (int j = 0; i < boardSize; i++)
            if (board[i][j] == ' ')
                return true;
    return false;
}

int Evaluate(char **board) //assigning score to the board ("1"-x wins, "-1"-o wins, "0"-everything else)
{
    // checking rows
    for (int row = 0; row < boardSize; row++)
    {
        if (board[row][0] == board[row][1] && board[row][1] == board[row][2])
        {
            if (board[row][0] == ai)
                return 1;
            else if (board[row][0] == player)
                return -1;
        }
    }

    // checking columns
    for (int column = 0; column < boardSize; column++)
    {
        if (board[0][column] == board[1][column] && board[1][column] == board[2][column])
        {
            if (board[0][column] == ai)
                return 1;
            else if (board[0][column] == player)
                return -1;
        }
    }

    // checking diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
    {
        if (board[0][0] == ai)
            return 1;
        else if (board[0][0] == player)
            return -1;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
    {
        if (board[0][2] == ai)
            return 1;
        else if (board[0][2] == player)
            return -1;
    }

    return 0;
}

int MiniMax(char **board, int depth, bool isMaximizing)
{
    int score = Evaluate(board);

    if (score == 1)
        return score;
    else if (score == -1)
        return score;
    else if (IsMoveLeft(board) == false)
        return 0;

    // maximizer's move
    if (isMaximizing)
    {
        int best = -1000;

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = ai;

                    best = std::max(best, MiniMax(board, depth + 1, !isMaximizing));

                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
    else // minimizer's move
    {
        int best = 1000;

        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (board[i][j] == ' ')
                {
                    board[i][j] = player;

                    best = std::min(best, MiniMax(board, depth + 1, !isMaximizing));

                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void BestMove(char **board) //figuring out best move with minimax
{
    int best = -1000;
    Move bestMove;
    bestMove.row = -1;
    bestMove.column = -1;

    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (board[i][j] == ' ')
            {
                board[i][j] = ai;

                int score = MiniMax(board, 0, false);

                board[i][j] = ' ';

                if (score > best)
                {
                    best = score;
                    bestMove.row = i;
                    bestMove.column = j;
                }
            }
        }
    }

    board[bestMove.row][bestMove.column] = ai;
}

void PrintBoard(char **board)
{
    std::cout << "          --- --- ---" << std::endl;

    for (int i = 0; i < boardSize; i++)
    {
        std::cout << "       " << i + 1;

        for (int j = 0; j <= boardSize; j++)
        {
            if (j == boardSize)
            {
                std::cout << " |";
            }
            else
            {
                std::cout << " | " << board[i][j];
            }
        }

        std::cout << std::endl
                  << "          --- --- ---" << std::endl;
    }

    std::cout << "           1   2   3 " << std::endl;
}

void InitializeBoard(char **board)
{
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = ' ';
        }
    }
}

void InitializeGame(char **board)
{
    system("clear");
    std::cout << std::endl << "---WELCOME TO THE TIC-TAC-TOE---" << std::endl; // 32 signs
    InitializeBoard(board);
    PrintBoard(board);
}

void PlayerMove(char **board)
{
    int row, column;
    bool correctMove = false;

    while (correctMove == false)
    {
        std::cout << "Your move!" << std::endl;
        std::cout << "Row: ";
        std::cin >> row;

        std::cout << "Column: ";
        std::cin >> column;

        if (board[row - 1][column - 1] == ' ')
        {
            correctMove = true;
            board[row - 1][column - 1] = player;
            system("clear");
            PrintBoard(board);
        }
        else
        {
            std::cout << "Invalid move. Try again." << std::endl;
        }
    }
}

void AiMove(char **board)
{
    sleep(1);
    BestMove(board);
    system("clear");
    PrintBoard(board);
}

char** CreateBoard()
{
    char **board = new char *[boardSize];
    for (int i = 0; i < boardSize; i++)
        board[i] = new char[boardSize];
    return board;
}

void ClearMemory(char** board)
{
    for (int i = 0; i < boardSize; i++)
        delete[] board[i];
    delete[] board;
}

void Game(char** board)
{
    InitializeGame(board);
    
    int round = 1;

    AiMove(board);
    
    while (round < 5)
    {
        PlayerMove(board);
        if (Evaluate(board) == -1)
        {
            std::cout << player << " wins!" << std::endl;
            break;
        }   

        AiMove(board);

        if (Evaluate(board) == 1)
        {
            std::cout << ai << " wins!" << std::endl;
            break;
        }

        if (round == 4 && Evaluate(board) == 0)
        {
            std::cout << "Tie!" << std::endl << std::endl << std::endl << std::endl;
        }
            
        round++;
    }
}