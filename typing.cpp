#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

wchar_t lat_to_cyl(wchar_t ch) {
    switch(ch) {
        case L'q': return L'й';
        case L'w': return L'ц';
        case L'e': return L'у';
        case L'r': return L'к';
        case L't': return L'е';
        case L'y': return L'н';
        case L'u': return L'г';
        case L'i': return L'ш';
        case L'o': return L'щ';
        case L'p': return L'з';
        case L'[': return L'х';
        case L']': return L'ъ';
        case L'a': return L'ф';
        case L's': return L'ы';
        case L'd': return L'в';
        case L'f': return L'а';
        case L'g': return L'п';
        case L'h': return L'р';
        case L'j': return L'о';
        case L'k': return L'л';
        case L'l': return L'д';
        case L';': return L'ж';
        case L'\'': return L'э';
        case L'z': return L'я';
        case L'x': return L'ч';
        case L'c': return L'с';
        case L'v': return L'м';
        case L'b': return L'и';
        case L'n': return L'т';
        case L'm': return L'ь';
        case L',': return L'б';
        case L'.': return L'ю';
        default: {
            throw invalid_argument("No such latin letter.");
        }
    }
}

wchar_t cyl_to_lat(wchar_t ch) {
    switch(ch) {
        case L'й': return L'q';
        case L'ц': return L'w';
        case L'у': return L'e';
        case L'к': return L'r';
        case L'е': return L't';
        case L'н': return L'y';
        case L'г': return L'u';
        case L'ш': return L'i';
        case L'щ': return L'o';
        case L'з': return L'p';
        case L'х': return L'[';
        case L'ъ': return L']';
        case L'ф': return L'a';
        case L'ы': return L's';
        case L'в': return L'd';
        case L'а': return L'f';
        case L'п': return L'g';
        case L'р': return L'h';
        case L'о': return L'j';
        case L'л': return L'k';
        case L'д': return L'l';
        case L'ж': return L';';
        case L'э': return L'\'';
        case L'я': return L'z';
        case L'ч': return L'x';
        case L'с': return L'c';
        case L'м': return L'v';
        case L'и': return L'b';
        case L'т': return L'n';
        case L'ь': return L'm';
        case L'б': return L',';
        case L'ю': return L'.';
        default: {
            throw invalid_argument("No such cyrillic letter.");
        }
    }
}

// global number generator
random_device seed;
default_random_engine generator(seed());
int random_between(int a, int b) {
    // random number [a,b] (from a to b INCLUDED) 
    uniform_int_distribution<int> distrib(a, b);
    return distrib(generator);
}

class QuizWord {
    int len;
    wstring lat_word;
    wstring cyl_word;
public:
    QuizWord(int len) : len(len) {
        for(int i=0; i<len; i++) {
            int n = random_between(0,31);
            wchar_t ch;
            switch(n) {
                case 26: ch = L'['; break;
                case 27: ch = L']'; break;
                case 28: ch = L';'; break;
                case 29: ch = L'\''; break;
                case 30: ch = L','; break;
                case 31: ch = L'.'; break;
                default: ch = L'a' + n; // between 'a' ~ 'z'
            }
            lat_word.push_back(ch);
            cyl_word.push_back(lat_to_cyl(ch));
        }
    }
    const wstring& get_lat_word() {
        return lat_word;
    }
    const wstring& get_cyl_word() {
        return cyl_word;
    }
};

#define MAX_X 64
#define MAX_Y 32

class Board {
    wchar_t board[MAX_X][MAX_Y];
public:
    Board() {
        empty_board();
    }
    wchar_t get(int x, int y) {
        if(x >= 0 && x < MAX_X && y >=0 && y < MAX_Y) {
            return board[x][y];
        } else {
            return L' ';
        }
    }
    void set(int x, int y, wchar_t ch) {
        if(x >= 0 && x < MAX_X && y >=0 && y < MAX_Y) {
            board[x][y]= ch;
        }
    }
    void draw(int x, int y, int width, int height, const wchar_t* str) {
        const wchar_t* ptr = str;
        for(int j=0; j<height; j++) {
            for(int i=0; i<width; i++) {
                wcout << *ptr << endl;
                set(x+i,y+j,*ptr++);
            }
        }
    }
    void empty_board() {
        for(int i=0; i<MAX_X; i++) {
            for(int j=0; j<MAX_Y; j++) {
                board[i][j] = L' ';
            }
        }
    }
};

class GameObj {
protected:
    QuizWord quiz;
    int cord_x;
    int cord_y;
    double accurate_y;
    int width;
    int height;
public:
    GameObj(int width, int height, int wordlen) :
        quiz(QuizWord(wordlen)),
        width(width),
        height(height)
    {
        cord_x = random_between(0, MAX_X - width);
        cord_y = 0;
        accurate_y = 0;
    }
    virtual ~GameObj() {};
    virtual double speed() = 0;
    virtual void draw_obj(Board& board) = 0;
    void move_step() {
        accurate_y += speed();
        cord_y = floor(accurate_y);
    }
    bool match_with(const wstring& word) {
        return quiz.get_lat_word() == word;
    }
    bool out_of_board() {
        return cord_y >= MAX_Y;
    }
};

class HashTag : public GameObj {
public:
    HashTag() : GameObj(3,3,1) {}
    double speed() {
        return 0.5;
    }
    void draw_obj(Board& board) {
        const wchar_t str[] =
L"\
###\
# #\
###\
";
        board.draw(cord_x,cord_y,width,height,str);
        board.draw(cord_x+1,cord_y+1,1,1,quiz.get_cyl_word().c_str());
    }
};

class AtMark : public GameObj {
public:
    AtMark() : GameObj(4,3,2) {}
    double speed() {
        return 0.4;
    }
    void draw_obj(Board& board) {
        const wchar_t str[] =
L"\
@@@@\
@  @\
@@@@\
";
        board.draw(cord_x,cord_y,width,height,str);
        board.draw(cord_x+1,cord_y+1,2,1,quiz.get_cyl_word().c_str());
    }
};

class Dollar : public GameObj {
public:
    Dollar() : GameObj(5,3,3) {}
    double speed() {
        return 0.3;
    }
    void draw_obj(Board& board) {
        const wchar_t str[] =
L"\
$$$$$\
$   $\
$$$$$\
";
        board.draw(cord_x,cord_y,width,height,str);
        board.draw(cord_x+1,cord_y+1,3,1,quiz.get_cyl_word().c_str());
    }
};

class Game {
    int level;
    int heart;
    Board board;
    vector<GameObj*> objects;
    void word_hit() {
        wstring word;
        wcin >> word;
        vector<GameObj*> vec;
        for(auto iter=objects.begin(); iter!=objects.end(); iter++) {
            if((*iter)->match_with(word)) {
                delete (GameObj*)*iter;
            } else {
                vec.push_back(*iter);
            }
        }
        objects = vec;
    }
    bool over_check() {
        vector<GameObj*> vec;
        for(auto iter=objects.begin(); iter!=objects.end(); iter++) {
            if((*iter)->out_of_board()) {
                heart -= 1;
                delete (GameObj*)*iter;
            } else {
                vec.push_back(*iter);
            }
        }
        objects = vec;
        return heart <= 0;
    }
    void generate() {
        int n = random_between(0, 1000);
        if(n <= 50 + level) {
            GameObj* obj;
            switch(random_between(0,2)) {
                case 0: obj = new HashTag(); break;
                case 1: obj = new AtMark(); break;
                case 2: obj = new Dollar(); break;
            }
            objects.push_back(obj);
        }
    }
    void refresh() {
        board.empty_board();
        for(auto iter=objects.begin(); iter!=objects.end(); iter++) {
            (*iter)->move_step();
            (*iter)->draw_obj(board);
        }
    }
    void display() {
        wcout << L"level = " << level << endl;
        wcout << L"total = " << objects.size() << endl;
        wcout << L"YOUR HEARTS:";
        for(int i=0; i<heart; i++) {
            wcout << L" ♥";
        }
        wcout << L'\n';
        for(int x=0; x<MAX_X+2; x++) {
            wcout << L'■';
        }
        wcout << L'\n';
        for(int y=0; y<MAX_Y; y++) {
            wcout << L'■';
            for(int x=0; x<MAX_X; x++) {
                wcout << board.get(x,y);
            }
            wcout << L"■\n";
        }
        for(int x=0; x<MAX_X+2; x++) {
            wcout << L'■';
        }
        wcout << L'\n';
        wcout.flush();
    }
public:
    Game() {
        level = 0;
        heart = 5;
    }
    void run_game() {
        int timer = 0;
        wcout << L"GAME START!" << endl;
        display();
        while(!over_check()) {
            auto start_clock = steady_clock::now();
            word_hit();
            auto end_clock = steady_clock::now();
            auto duration = duration_cast<milliseconds>(end_clock - start_clock);
            timer += duration.count();
            while(timer >= 200) {
                refresh();
                generate();
                timer -= 200;
            }
            wcout << L"duration = " << duration.count() << endl;
            display();
            start_clock = end_clock;
        }
        wcout << L"GAME OVER!" << endl;
    }
};

int main() {
    setlocale(LC_ALL, "en_US.UTF8");
    Game game;
    game.run_game();
    return 0;
}
