// rats.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////
const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_pelletsConsumed;
    bool    movedLastTime;
    
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonPellet();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(string msg) const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();
    
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;
    
    // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_pelletsConsumed = 0;
    movedLastTime = 1;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    if(m_pelletsConsumed > 1)
        return true;
    return false;
}

void Rat::move()
{
    int dir = randInt(0, 3);
    if(m_pelletsConsumed == 0)
    {
        //must move
        int status = m_arena -> getCellStatus(m_row, m_col);
        if(status == 2)
            m_arena -> setCellStatus(m_row, m_col, 0);
        else
            m_arena -> setCellStatus(m_row, m_col, status-1);
        
        attemptMove(*m_arena, dir, m_row, m_col);
        status = m_arena -> getCellStatus(m_row, m_col);
        if(status == HAS_POISON)
        {
            m_pelletsConsumed++;
            if(!isDead())
                m_arena -> setCellStatus(m_row, m_col, 2);
        }
        else if(status ==  EMPTY)
        {
            m_arena -> setCellStatus(m_row, m_col, 2);
        }
        else
            m_arena -> setCellStatus(m_row, m_col, status+1);
        
        //TIME TO CHECK IF RAT MOVED INTO PLAYER
        
        int prow = m_arena -> player() -> row();
        int pcol = m_arena -> player() -> col();
        
        if(m_row == prow && m_col == pcol)
        {
            m_arena -> player() -> setDead();
        }
        
    }
    else if(m_pelletsConsumed == 1)
    {
        if(movedLastTime)
        {
            movedLastTime = 0;
            return ;
        }
        movedLastTime = 1;
        //must move
        int status = m_arena -> getCellStatus(m_row, m_col);
        if(status == 2)
            m_arena -> setCellStatus(m_row, m_col, 0);
        else
            m_arena -> setCellStatus(m_row, m_col, status-1);
        
        attemptMove(*m_arena, dir, m_row, m_col);
        status = m_arena -> getCellStatus(m_row, m_col);
        if(status == HAS_POISON)
        {
            m_pelletsConsumed++;
            if(!isDead())
                m_arena -> setCellStatus(m_row, m_col, 2);
        }
        else if(status ==  EMPTY)
        {
            m_arena -> setCellStatus(m_row, m_col, 2);
        }
        else
            m_arena -> setCellStatus(m_row, m_col, status+1);
        
        //TIME TO CHECK IF RAT MOVED INTO PLAYER
        
        int prow = m_arena -> player() -> row();
        int pcol = m_arena -> player() -> col();
        
        if(m_row == prow && m_col == pcol)
        {
            m_arena -> player() -> setDead();
        }
        
    }
    
    //   Return without moving if the rat has eaten one poison pellet (so
    //   is supposed to move only every other turn) and this is a turn it
    //   does not move.
    
    //   Otherwise, attempt to move in a random direction; if it can't
    //   move, don't move.  If it lands on a poison pellet, eat the pellet,
    //   so it is no longer on that grid point.
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
    //  Attempt to move the player one step in the indicated
    //        direction.  If this fails,
    //        return "Player couldn't move; player stands."
    //        A player who moves onto a rat dies, and this
    //        returns "Player walked into a rat and died."
    //        Otherwise, return one of "Player moved north.",
    //        "Player moved east.", "Player moved south.", or
    //        "Player moved west."
    if(!attemptMove(*m_arena, dir, m_row, m_col))
        return "Player couldn't move; player stands.";  // This implementation compiles, but is incorrect.
    if(m_arena->getCellStatus(m_row, m_col) > 1)
    {
        setDead();
        return "Player walked into a rat and died.";
    }
    
    switch(dir)
    {
        case NORTH:
        {
            return "Player moved north.";
            break;
        }
            
        case SOUTH:
        {
            return "Player moved south.";
            break;
        }
            
        case EAST:
        {
            return "Player moved east.";
            break;
        }
            
        case WEST:
        {
            return "Player moved west.";
            break;
        }
            
        default: return "Player couldn't move; player stands.";
    }
    
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRats = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    //   Deallocate the player and all remaining dynamically allocated rats
    delete m_player;
    for(int i = 0; i < m_nRats; i++)
        delete m_rats[i];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::ratCount() const
{
    return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
    /*
     int status = getCellStatus(r, c);
     if(status > 1)
     return status-1;
     else
     return 0;
     */
    int count = 0;
    for(int i = 0; i < m_nRats; i++)
    {
        if(m_rats[i] -> row() == r && m_rats[i] -> col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r = 1, c = 1;
    
    // Fill displayGrid with dots (empty) and stars (poison pellets)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    
    for(int i = 1; i < r; i++)
    {
        for(int j = 1; j < c; j++)
        {
            int status = numberOfRatsAt(i, j);
            if(status > 0)
            {
                if(status == 1)
                    displayGrid[i-1][j-1] = 'R';
                else if(status > 1 && status < 9)
                    displayGrid[i-1][j-1] = char(status + '0');
                else if(status >= 9)
                    displayGrid[i-1][j-1] = '9';
            }
        }
    }
    
    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
    // Write message, rat, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << ratCount() << " rats remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRat(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a rat on a spot with a poison pellet
    if (getCellStatus(r, c) != EMPTY)
        return false;
    
    // Don't add a rat on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    // If there are MAXRATS existing rats, retirn false.  Otherwise,
    // dynamically allocate a new rat at coordinates (r,c).  Save the
    // pointer to the newly allocated rat and return true.
    
    if(m_nRats >= MAXRATS)
        return false;
    
    Rat *tat = new Rat(this, r, c);
    m_rats[m_nRats] = tat;
    m_nRats++;
    if(getCellStatus(r, c) == EMPTY)
        setCellStatus(r, c, 2);
    else
        setCellStatus(r, c, getCellStatus(r, c) + 1);
    
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a rat
    if (numberOfRatsAt(r, c) > 0)
        return false;
    
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRats()
{
    // Move all rats
    //   Move each rat.  Mark the player as dead if necessary.
    //        Deallocate any dead dynamically allocated rat.
    
    // Another turn has been taken
    for(int i = 0; i < m_nRats; i++)
    {
        m_rats[i] -> move();
        if(m_rats[i] -> isDead())
        {
            m_nRats--;
            m_rats[i] = m_rats[m_nRats];
            i--;
        }
    }
    
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
        << c << ")" << endl;
        exit(1);
    }
}


///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
        << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with rats
    while (nRats > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c);
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
        default:  return false;
        case 'n': dir = NORTH; break;
        case 'e': dir = EAST;  break;
        case 's': dir = SOUTH; break;
        case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    // Delete the following line and replace it with the correct code.
    switch(dir)
    {
        case NORTH:
        {
            if(!((r-1)>= 1 && c >= 1 && (r-1)<=a.rows() && c<=a.cols()))
                return false;
            r--;
            break;
        }
            
        case SOUTH:
        {
            if(!((r+1)>= 1 && c >= 1 && (r+1)<=a.rows() && c<=a.cols()))
                return false;
            r++;
            break;
        }
            
        case EAST:
        {
            if(!((r)>= 1 && (c+1)>= 1 && (r)<=a.rows() && (c+1)<=a.cols()))
                return false;
            c++;
            break;
        }
            
        case WEST:
        {
            if(!((r)>= 1 && (c-1)>= 1 && (r)<=a.rows() && (c-1)<=a.cols()))
                return false;
            c--;
            break;
        }
        default: return false;
            
    }
    
    return true;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
int canRatMoveToCell(const Arena& a, int r, int c, int ratLocation [])
{
    int rats = 0;
    int arow = a.rows(), acol = a.cols();
    for(int i = 0; i < 4; i++)
    {
        ratLocation[i] = 0;
    }
    
    if(r-1 >= 1)
    {
        if(a.numberOfRatsAt(r-1, c))
        {
            rats+= a.numberOfRatsAt(r-1, c);
            ratLocation[NORTH] = 1;
        }
        else
            ratLocation[NORTH] = 2;
    }
    
    if(c-1 >= 1)
    {
        if(a.numberOfRatsAt(r, c-1))
        {
            rats+= a.numberOfRatsAt(r, c-1);
            ratLocation[WEST] = 1;
        }
        else
            ratLocation[WEST] = 2;
    }
    
    if(r+1 <= arow)
    {
        if(a.numberOfRatsAt(r+1, c))
        {
            rats+= a.numberOfRatsAt(r+1, c);
            ratLocation[SOUTH] = 1;
        }
        else
            ratLocation[SOUTH] = 2;
    }
    
    if(c+1 <= acol)
    {
        if(a.numberOfRatsAt(r, c+1))
        {
            rats+= a.numberOfRatsAt(r, c+1);
            ratLocation[EAST] = 1;
        }
        else
            ratLocation[EAST] = 2;
    }
    
    return rats;
}
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // Delete the following line and replace it with your code.
    
    int ratCountAtPos;
    bool pelletPresent = a.getCellStatus(r, c) == HAS_POISON ? 1 : 0;
    
    int ratLocation[4], sratLocation[4], ratCount[4];
    
    for(int i = 0; i<4; i++)
    {
        ratCount[i] = MAXRATS;
    }
    
    //    if(canRatMoveToCell(a, r, c, ratLocation) == 0)
    //	    return false;
    //
    canRatMoveToCell(a, r, c, ratLocation);
    
    ratCountAtPos = canRatMoveToCell(a, r, c, ratLocation);
    
    if(ratLocation[NORTH] == 2)
    {
        //calculate rats which can move to that cell
        ratCount[NORTH] = canRatMoveToCell(a, r-1, c, sratLocation);
    }
    
    if(ratLocation[WEST] == 2)
    {
        ratCount[WEST] = canRatMoveToCell(a, r, c-1, sratLocation);
    }
    
    if(ratLocation[SOUTH] == 2)
    {
        ratCount[SOUTH] = canRatMoveToCell(a, r+1, c, sratLocation);
    }
    
    if(ratLocation[EAST] == 2)
    {
        ratCount[EAST] = canRatMoveToCell(a, r, c+1, sratLocation);
    }
    
    int min = ratCountAtPos; bool toMove = 0;
    
    bool randomizer[4] = {0,0,0,0}; int times = 0;
    
    while(times < 4)
    {
        int j = randInt(0,3);
        
        if(randomizer[j] == 0)
        {
            randomizer[j] = 1;
            times++;
            if(pelletPresent)
            {
                if(ratCount[j] <= min)
                {
                    min = ratCount[j];
                    bestDir = j;
                    toMove = 1;
                }
            }
            
            else if(ratCount[j] < min)
            {
                min = ratCount[j];
                bestDir = j;
                toMove = 1;
            }
            
        }
    }
    
    if(toMove)
        return true;
    
    return false;
    // Your replacement implementation should do something intelligent.
    // You don't have to be any smarter than the following, although
    // you can if you want to be:  If staying put runs the risk of a
    // rat possibly moving onto the player's location when the rats
    // move, yet moving in a particular direction puts the player in a
    // position that is safe when the rats move, then the chosen
    // action is to move to a safer location.  Similarly, if staying put
    // is safe, but moving in certain directions puts the player in danger,
    // then the chosen action should not be to move in one of the
    // dangerous directions; instead, the player should stay put or move to
    // another safe position.  In general, a position that may be moved to
    // by many rats is more dangerous than one that may be moved to by
    // few.
    //
    // Unless you want to, you do not have to take into account that a
    // rat might be poisoned and thus sometimes less dangerous than one
    // that is not.  That requires a more sophisticated analysis that
    // we're not asking you to do.
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////
#include<cassert>
/*
#include <type_traits>


#define CHECKTYPE(c, f, r, a)  \
static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
"FAILED: You changed the type of " #c "::" #f);  \
[[gnu::unused]] r (c::* xxx##c##_##f) a = &c::f

void thisFunctionWillNeverBeCalled()
{
    // If the student deleted or changed the interfaces to the public
    // functions, this won't compile.  (This uses magic beyond the scope
    // of CS 31.)
    
    Rat r(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Rat, row, int, () const);
    CHECKTYPE(Rat, col, int, () const);
    CHECKTYPE(Rat, isDead, bool, () const);
    CHECKTYPE(Rat, move, void, ());
    
    Player p(static_cast<Arena*>(0), 1, 1);
    CHECKTYPE(Player, row, int, () const);
    CHECKTYPE(Player, col, int, () const);
    CHECKTYPE(Player, isDead, bool, () const);
    CHECKTYPE(Player, dropPoisonPellet, string, ());
    CHECKTYPE(Player, move, string, (int));
    CHECKTYPE(Player, setDead, void, ());
    
    Arena a(1, 1);
    CHECKTYPE(Arena, rows, int, () const);
    CHECKTYPE(Arena, cols, int, () const);
    CHECKTYPE(Arena, player, Player*, () const);
    CHECKTYPE(Arena, ratCount, int, () const);
    CHECKTYPE(Arena, getCellStatus, int, (int,int) const);
    CHECKTYPE(Arena, numberOfRatsAt, int, (int,int) const);
    CHECKTYPE(Arena, display, void, (string) const);
    CHECKTYPE(Arena, setCellStatus, void, (int,int,int));
    CHECKTYPE(Arena, addRat, bool, (int,int));
    CHECKTYPE(Arena, addPlayer, bool, (int,int));
    CHECKTYPE(Arena, moveRats, void, ());
    
    Game g(1,1,1);
    CHECKTYPE(Game, play, void, ());
}
*/

void findTheRat(const Arena& a, int& r, int& c)
{
    if      (a.numberOfRatsAt(r-1, c) == 1) r--;
    else if (a.numberOfRatsAt(r+1, c) == 1) r++;
    else if (a.numberOfRatsAt(r, c-1) == 1) c--;
    else if (a.numberOfRatsAt(r, c+1) == 1) c++;
    else assert(false);
}

void doBasicTests()
{
    {
        Arena a(10, 20);
        a.addPlayer(2, 5);
        Player* pp = a.player();
        assert(pp->row() == 2  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player moved north.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player couldn't move; player stands.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        pp->setDead();
        assert(pp->row() == 1  &&  pp->col() == 5  && pp->isDead());
    }
    {
        Arena a(10, 20);
        int r = 4;
        int c = 4;
        
        a.setCellStatus(r-1, c, HAS_POISON);
        
        
        a.setCellStatus(r+1, c, HAS_POISON);
        a.setCellStatus(r, c-1, HAS_POISON);
        a.setCellStatus(r, c+1, HAS_POISON);
        a.addRat(r, c);
        
        a.addPlayer(8, 18);
        
        assert(a.ratCount() == 1  &&  a.numberOfRatsAt(r, c) == 1);
        
        a.moveRats();
        
        assert(a.ratCount() == 1  &&  a.numberOfRatsAt(r, c) == 0);
        
        findTheRat(a, r, c);
        
        assert(a.getCellStatus(r, c) != HAS_POISON);
        a.moveRats(); //moved left
        
        
        assert(a.ratCount() == 1  &&  a.numberOfRatsAt(r, c) == 1);
        a.moveRats();
        
        assert(a.ratCount() == 1  &&  a.numberOfRatsAt(r, c) == 0);
        findTheRat(a, r, c);
        a.moveRats();
        assert(a.ratCount() == 1  &&  a.numberOfRatsAt(r, c) == 1);
        a.setCellStatus(r-1, c, HAS_POISON);
        a.setCellStatus(r+1, c, HAS_POISON);
        a.setCellStatus(r, c-1, HAS_POISON);
        a.setCellStatus(r, c+1, HAS_POISON);
        a.moveRats();
        assert(a.ratCount() == 0  &&  a.numberOfRatsAt(r, c) == 0);
        assert(a.numberOfRatsAt(r-1, c) == 0);
        assert(a.numberOfRatsAt(r+1, c) == 0);
        assert(a.numberOfRatsAt(r, c-1) == 0);
        assert(a.numberOfRatsAt(r, c+1) == 0);
        
        for (int k = 0; k < MAXRATS/4; k++)
        {
            a.addRat(7, 18);
            a.addRat(9, 18);
            a.addRat(8, 17);
            a.addRat(8, 19);
        }
        assert(! a.player()->isDead());
        a.moveRats();
        assert(a.player()->isDead());
    }
    
}












#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <set>
#include <algorithm>
#include <cstdlib>
#include <cassert>
using namespace std;

class streambuf_switcher
{
public:
    streambuf_switcher(ios& dest, ios& src)
    : dest_stream(dest), saved_streambuf(dest.rdbuf(src.rdbuf()))
    {}
    ~streambuf_switcher()
    {
        dest_stream.rdbuf(saved_streambuf);
    }
private:
    ios& dest_stream;
    streambuf* saved_streambuf;
};

set<void*> addrs;
bool recordaddrs = false;

void* operator new(size_t n)
{
    void* p = malloc(n);
    fill_n(static_cast<char*>(p), n, 0xca);
    if (recordaddrs  &&  (n == sizeof(Rat)  ||  n == sizeof(Player)))
    {
        recordaddrs = false;
        addrs.insert(p);
        recordaddrs = true;
    }
    return p;
}

void operator delete(void* p) noexcept
{
    if (recordaddrs)
    {
        recordaddrs = false;
        set<void*>::iterator it = addrs.find(p);
        if (it != addrs.end())
            addrs.erase(it);
            recordaddrs = true;
            }
    free(p);
}

bool recommendMove(const Arena& a, int r, int c, int& bestDir);

void testone(int n)
{
    streambuf_switcher sso(cout, cerr);
    
    switch (n)
    {
        default: {
            cout << "Bad argument" << endl;
        } break; case  1: {
            int k;
            for (k = 0; k < 300; k++)
            {
                Arena a(1, 20);
                a.addPlayer(1, 3);
                Rat r(&a, 1, 18);
                assert(r.row() == 1  &&  r.col() == 18);
                r.move(); assert(r.row() == 1);
                if (r.col() != 19)
                {
                    assert(r.col() == 17  ||  r.col() == 18);
                    continue;
                }
                r.move(); assert(r.row() == 1);
                if (r.col() == 20)
                    break;
                assert(r.col() == 18  ||  r.col() == 19);
            }
            assert(k < 300);
        } break; case  2: {
            int k;
            for (k = 0; k < 600; k++)
            {
                Arena a(1, 20);
                a.addPlayer(1, 3);
                Rat r(&a, 1, 19);
                assert(r.row() == 1  &&  r.col() == 19);
                r.move(); assert(r.row() == 1);
                if (r.col() != 19)
                {
                    assert(r.col() == 18  ||  r.col() == 20);
                    continue;
                }
                r.move(); assert(r.row() == 1);
                if (r.col() != 19)
                {
                    assert(r.col() == 18  ||  r.col() == 20);
                    continue;
                }
                r.move(); assert(r.row() == 1);
                if (r.col() != 19)
                {
                    assert(r.col() == 18  ||  r.col() == 20);
                    continue;
                }
                r.move(); assert(r.row() == 1);
                if (r.col() != 19)
                {
                    assert(r.col() == 18  ||  r.col() == 20);
                    continue;
                }
                r.move(); assert(r.row() == 1);
                if (r.col() == 18  ||  r.col() == 20)
                    break;
                assert(r.col() == 19);
            }
            assert(k < 600);
        } break; case  3: {
            int k;
            for (k = 0; k < 600; k++)
            {
                Arena a(20, 1);
                a.addPlayer(3, 1);
                Rat r(&a, 19, 1);
                assert(r.row() == 19  &&  r.col() == 1);
                r.move(); assert(r.col() == 1);
                if (r.row() != 19)
                {
                    assert(r.row() == 18  ||  r.row() == 20);
                    continue;
                }
                r.move(); assert(r.col() == 1);
                if (r.row() != 19)
                {
                    assert(r.row() == 18  ||  r.row() == 20);
                    continue;
                }
                r.move(); assert(r.col() == 1);
                if (r.row() != 19)
                {
                    assert(r.row() == 18  ||  r.row() == 20);
                    continue;
                }
                r.move(); assert(r.col() == 1);
                if (r.row() != 19)
                {
                    assert(r.row() == 18  ||  r.row() == 20);
                    continue;
                }
                r.move(); assert(r.col() == 1);
                if (r.row() == 18  ||  r.row() == 20)
                    break;
                assert(r.row() == 19);
            }
            assert(k < 600);
        } break; case  4: {
            Arena a(10,20);
            a.addPlayer(9, 19);
            for (int r = 5-2; r <= 5+2; r++)
                for (int c = 10-2; c <= 10+2; c++)
                    a.setCellStatus(r, c, HAS_POISON);
            Rat r(&a, 5, 10);
            r.move();
            assert((r.row() == 5  &&  abs(r.col() - 10) == 1)  ||
                   (r.col() == 10  &&  abs(r.row() - 5) == 1));
            int oldr = r.row();
            int oldc = r.col();
            r.move();
            assert(r.row() == oldr  &&  r.col() == oldc);
            r.move();
            assert((r.row() == oldr  &&  abs(r.col() - oldc) == 1)  ||
                   (r.col() == oldc  &&  abs(r.row() - oldr) == 1));
        } break; case  5: {
            Arena a(10,20);
            a.addPlayer(9, 19);
            for (int r = 5-2; r <= 5+2; r++)
                for (int c = 10-2; c <= 10+2; c++)
                    if (r != 5  ||  c != 10)
                        a.setCellStatus(r, c, HAS_POISON);
            Rat r(&a, 5, 10);
            assert(!r.isDead());
            r.move();
            assert((r.row() == 5  &&  abs(r.col() - 10) == 1)  ||
                   (r.col() == 10  &&  abs(r.row() - 5) == 1));
            int oldr = r.row();
            int oldc = r.col();
            assert(!r.isDead());
            a.setCellStatus(5, 10, HAS_POISON);
            r.move();
            assert(r.row() == oldr  &&  r.col() == oldc);
            assert(!r.isDead());
            r.move();
            assert((r.row() == oldr  &&  abs(r.col() - oldc) == 1)  ||
                   (r.col() == oldc  &&  abs(r.row() - oldr) == 1));
            assert(r.isDead());
        } break; case  6: {
            Arena a(10,20);
            a.addPlayer(9, 19);
            a.setCellStatus(4, 10, HAS_POISON);
            a.setCellStatus(6, 10, HAS_POISON);
            a.setCellStatus(5, 9, HAS_POISON);
            a.setCellStatus(5, 11, HAS_POISON);
            Rat r(&a, 5, 10);
            r.move();
            assert((r.row() == 5  &&  abs(r.col() - 10) == 1)  ||
                   (r.col() == 10  &&  abs(r.row() - 5) == 1));
            int oldr = r.row();
            int oldc = r.col();
            r.move();
            assert(r.row() == oldr  &&  r.col() == oldc);
            r.move();
            assert((r.row() == oldr  &&  abs(r.col() - oldc) == 1)  ||
                   (r.col() == oldc  &&  abs(r.row() - oldr) == 1));
            oldr = r.row();
            oldc = r.col();
            r.move();
            assert(r.row() == oldr  &&  r.col() == oldc);
            a.setCellStatus(oldr-1, oldc, EMPTY);
            a.setCellStatus(oldr+1, oldc, EMPTY);
            a.setCellStatus(oldr, oldc-1, EMPTY);
            a.setCellStatus(oldr, oldc+1, EMPTY);
            r.move();
            assert((r.row() == oldr  &&  abs(r.col() - oldc) == 1)  ||
                   (r.col() == oldc  &&  abs(r.row() - oldr) == 1));
        } break; case  7: {
            Arena a(1, 20);
            a.addPlayer(1, 3);
            Player* p = a.player();
            assert(p->row() == 1  &&  p->col() == 3);
            p->move(WEST); assert(p->row() == 1  &&  p->col() == 2);
            p->move(WEST); assert(p->row() == 1  &&  p->col() == 1);
        } break; case  8: {
            Arena a(1, 20);
            a.addPlayer(1, 3);
            Player* p = a.player();
            p->move(WEST); assert(p->row() == 1  &&  p->col() == 2);
            p->move(WEST); assert(p->row() == 1  &&  p->col() == 1);
            p->move(WEST); assert(p->row() == 1  &&  p->col() == 1);
            p->move(NORTH); assert(p->row() == 1  &&  p->col() == 1);
            p->move(SOUTH); assert(p->row() == 1  &&  p->col() == 1);
        } break; case  9: {
            Arena a(10, 20);
            a.addPlayer(3, 6);
            Player* p = a.player();
            assert( ! p->isDead());
            p->setDead();
            assert(p->isDead());
        } break; case 10: {
            Arena a(20, 1);
            a.addPlayer(1, 1);
            Player* p = a.player();
            assert(p->move(WEST).find(" stands") != string::npos);
            assert(p->move(EAST).find(" stands") != string::npos);
            assert(p->move(NORTH).find(" stands") != string::npos);
            assert(p->move(SOUTH).find(" south") != string::npos);
        } break; case 11: {
            Arena a(20, 1);
            a.addPlayer(1, 1);
            Player* p = a.player();
            a.addRat(2, 1);
            assert(p->move(SOUTH).find(" died") != string::npos);
        } break; case 12: {
            Arena a(20, 1);
            a.addPlayer(1, 1);
            Player* p = a.player();
            a.addRat(2, 1);
            assert(!p->isDead());
            p->move(SOUTH);
            assert(p->isDead());
        } break; case 13: {
            Arena a(6, 15);
            assert(a.rows() == 6  &&  a.cols() == 15);
        } break; case 14: {
            Arena a(10, 20);
            a.addPlayer(3, 6);
            a.addRat(7, 5);
            assert(a.numberOfRatsAt(7, 5) == 1  &&  a.ratCount() == 1);
        } break; case 15: {
            Arena a(10, 20);
            a.addPlayer(3, 6);
            a.addRat(7, 5);
            a.addRat(4, 7);
            a.addRat(7, 5);
            assert(a.numberOfRatsAt(7, 5) == 2  &&  a.ratCount() == 3);
        } break; case 16: {
            Arena a(1, 20);
            a.addPlayer(1, 8);
            a.addRat(1, 1);
            a.setCellStatus(1, 2, HAS_POISON);
            a.setCellStatus(1, 3, HAS_POISON);
            a.addRat(1, 16);
            a.setCellStatus(1, 14, HAS_POISON);
            a.setCellStatus(1, 15, HAS_POISON);
            a.setCellStatus(1, 17, HAS_POISON);
            a.setCellStatus(1, 18, HAS_POISON);
            assert(a.ratCount() == 2);
            int k;
            for (k = 0; k < 100; k++)
            {
                a.moveRats();
                int nb1 = (a.getCellStatus(1, 2) == HAS_POISON) +
                (a.getCellStatus(1, 3) == HAS_POISON);
                int nb2 = (a.getCellStatus(1, 14) == HAS_POISON) +
                (a.getCellStatus(1, 15) == HAS_POISON) +
                (a.getCellStatus(1, 17) == HAS_POISON) +
                (a.getCellStatus(1, 18) == HAS_POISON);
                assert(a.ratCount() == (nb1 > 0) + (nb2 > 2));
                if (a.ratCount() == 0)
                    break;
            }
            assert(k < 100);
        } break; case 17: {
            Arena a(1, 3);
            a.addPlayer(1, 1);
            Player* p = a.player();
            for (int j = 0; j < 10; j++)
                a.addRat(1, 3);
            assert(!p->isDead());
            a.moveRats();
            int k;
            for (k = 0; k < 100; k++)
            {
                assert(!p->isDead());
                a.moveRats();
                if (a.numberOfRatsAt(1, 1) > 0)
                {
                    assert(p->isDead());
                    break;
                }
            }
            assert(k < 100);
        } break; case 18: {
            ostringstream oss;
            streambuf_switcher sso2(cout, oss);
            Arena a(2, 3);
            a.addPlayer(2, 3);
            a.addRat(2, 1);
            a.addRat(2, 1);
            a.addRat(2, 2);
            a.display("");
            assert(oss.str().find("2R@") != string::npos);
        } break; case 19: {
            ostringstream oss;
            streambuf_switcher sso2(cout, oss);
            Arena a(2, 3);
            a.addPlayer(2, 3);
            for (int k = 1; k <= 20; k++)
                a.addRat(2, 1);
            a.display("");
            assert(oss.str().find("9.@") != string::npos);
        } break; case 20: {
            recordaddrs = true;
            int n = addrs.size();
            {
                Arena a(20, 20);
                for (int r = 1; r <= 5; r++)
                    for (int c = 11; c <= 20; c++)
                        a.setCellStatus(r, c, HAS_POISON);
                for (int r = 16; r <= 20; r++)
                    for (int c = 15; c <= 20; c++)
                        a.setCellStatus(r, c, HAS_POISON);
                a.addPlayer(19, 19);
                int n2 = addrs.size();
                a.setCellStatus(3, 13, EMPTY);
                a.setCellStatus(3, 18, EMPTY);
                a.addRat(3, 13);
                a.addRat(3, 18);
                for (int k = 0; k < 4; k++)
                {
                    a.addRat(1, 1);
                    a.addRat(1, 4);
                    a.addRat(4, 1);
                    a.addRat(4, 4);
                }
                assert(addrs.size() >= n2 + 18);
                a.moveRats();
                a.setCellStatus(3, 13, HAS_POISON);
                a.setCellStatus(3, 18, HAS_POISON);
                a.moveRats();
                a.moveRats();
                assert(a.ratCount() == 18-2);
                for (int k = a.ratCount(); k < MAXRATS; k++)
                    assert(a.addRat(1, 1));
                int j;
                for (j = 0; j < 1000  &&  a.ratCount() > 20; j++)
                {
                    for (int r = 1; r <= 20; r++)
                        for (int c = 1; c <= 20; c++)
                            if (a.numberOfRatsAt(r, c) == 0  &&  !(r == 19 && c == 19))
                                a.setCellStatus(r, c, HAS_POISON);
                    a.moveRats();
                }
                assert(j < 1000);
                for (int k = a.ratCount(); k < MAXRATS; k++)
                    assert(a.addRat(1, 1));
                assert(addrs.size() >= n2 + MAXRATS);
            }
            assert(addrs.size() == n);
            recordaddrs = false;
        } break; case 21: {
            Arena a(20, 20);
            a.addPlayer(19, 19);
            for (int r = 1; r < 19; r++)
                for (int c = 1; c < 19; c++)
                    if (r != 10  |  c != 10)
                        a.setCellStatus(r, c, HAS_POISON);
            for (int k = 0; k < 100; k++)
                a.addRat(10, 10);
            assert(a.ratCount() == 100  &&  a.numberOfRatsAt(10, 10) == 100);
            int nr[1+20][1+20];
            a.moveRats();
            int tot = 0;
            for (int r = 1; r < 19; r++)
            {
                for (int c = 1; c < 19; c++)
                {
                    nr[r][c] = a.numberOfRatsAt(r, c);
                    tot += nr[r][c];
                    assert((r == 10  &&  c >= 9 && c <= 11) ||
                           (c == 10  &&  r >= 9 && r <= 11) ||
                           nr[r][c] == 0);
                }
            }
            assert(nr[10][10] == 0  &&  tot == a.ratCount());
            assert(nr[9][10] == 0  ||  a.getCellStatus(9, 10) == EMPTY);
            assert(nr[11][10] == 0  ||  a.getCellStatus(11, 10) == EMPTY);
            assert(nr[10][9] == 0  ||  a.getCellStatus(10, 9) == EMPTY);
            assert(nr[10][11] == 0  ||  a.getCellStatus(10, 11) == EMPTY);
            a.setCellStatus(10, 10, HAS_POISON);
            a.moveRats();
            assert(a.numberOfRatsAt(9, 10) == (nr[9][10] == 0 ? 0 : 1));
            assert(a.numberOfRatsAt(11, 10) == (nr[11][10] == 0 ? 0 : 1));
            assert(a.numberOfRatsAt(10, 9) == (nr[10][9] == 0 ? 0 : 1));
            assert(a.numberOfRatsAt(10, 11) == (nr[10][11] == 0 ? 0 : 1));
            for (int k = 0; k < 17; k++)
            {
                for (int r = 1; r < 19; r++)
                    for (int c = 1; c < 19; c++)
                        if (a.numberOfRatsAt(r, c) == 0)
                            a.setCellStatus(r, c, HAS_POISON);
                a.moveRats();
            }
            tot = 0;
            for (int r = 1; r < 19; r++)
                for (int c = 1; c < 19; c++)
                    tot += a.numberOfRatsAt(r, c);
            assert(a.ratCount() == tot  &&  tot < 100);
        } break; case 22: {
            Arena a(4, 2);
            a.addPlayer(1, 1);
            Player* p = a.player();
            a.addRat(4, 2);
            for (int k = 0; k < 10000 &&  ! a.player()->isDead()  &&
                 a.ratCount() != 0; k++)
            {
                int dir;
                if (recommendMove(a, p->row(), p->col(), dir))
                    p->move(dir);
                else
                    p->dropPoisonPellet();
                a.moveRats();
            }
            assert(! a.player()->isDead()  &&  a.ratCount() == 0);
        } break; case 23: {
            Arena a(10, 10);
            a.addPlayer(6, 6);
            a.addRat(5, 6);
            a.addRat(7, 6);
            a.addRat(6, 7);
            int dir;
            assert(recommendMove(a, 6, 6, dir)  &&  dir == WEST);
        } break; case 24: {
            Arena a(10, 10);
            a.addPlayer(6, 6);
            a.addRat(4, 6);
            a.addRat(5, 7);
            a.addRat(6, 8);
            a.addRat(7, 7);
            a.addRat(8, 6);
            a.addRat(7, 5);
            a.addRat(6, 4);
            a.addRat(5, 5);
            int dir;
            assert(!recommendMove(a, 6, 6, dir));
        } break; case 25: {
            Arena a(2, 3);
            a.addPlayer(1, 2);
            a.addRat(1, 1);
            for (int k = 0; k < 10; k++)
                a.addRat(2, 3);
            int dir;
            assert(!recommendMove(a, 1, 2, dir));
        } break; case 26: {
            Arena a(3, 2);
            a.addPlayer(3, 1);
            a.addRat(1, 1);
            for (int k = 0; k < 10; k++)
                a.addRat(3, 2);
            int dir;
            assert(recommendMove(a, 3, 1, dir)  &&  dir == NORTH);
        } break;
    }
}

int main()
{
    cout << "Enter test number (1-26): ";
    int n;
    cin >> n;
    testone(n);
    cout << "Passed!" << endl;
}



/*
int main()
{
    // doBasicTests(); // Remove this line after completing test.
    // return 0;       // Remove this line after completing test.
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);
    
    // Play the game
    g.play();
}
*/
///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
