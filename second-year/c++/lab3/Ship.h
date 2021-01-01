#pragma once

class Ship {
private:
    char HP = 0;
public:
    explicit Ship(char HP){
        this->HP = HP;
    };
    ~Ship() = default;
    [[nodiscard]] char getHP() const {
        return HP;
    }
    bool takeHit(){
        HP--;
        return HP == 0; ///уничтожен ли корабль
    }
};
