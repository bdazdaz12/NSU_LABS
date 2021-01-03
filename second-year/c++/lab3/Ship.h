#pragma once

enum Ships: uint8_t {
    battleship = 4,
    cruiser = 3,
    destroyer = 2,
    boat = 1
};

class Ship {
private:
    char x;
    char y;
    bool horizontal;
    char HP;
public:
    explicit Ship(char x, char y, bool horizontal, char HP){
        this->x = x;
        this->y = y;
        this->horizontal = horizontal;
        this->HP = HP;
    };
    ~Ship() = default;
    inline char getX() const{
        return x;
    }
    inline char getY() const{
        return y;
    }
    inline bool getHorizontal() const{
        return horizontal;
    }
    inline char getHP() const {
        return HP;
    }
    bool takeHit(){
        HP--;
        return HP == 0; ///уничтожен ли корабль
    }
};
