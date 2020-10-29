#include "Trit.h"

std::ostream& operator << (std::ostream& outStream, Trit a){
    switch(a){
        case Trit::False:
            return outStream << "False";
        case Trit::Unknown:
            return outStream << "Unknown";
        case Trit::True:
            return outStream << "True";
    }
    return outStream << "Unknown"; // в случае неопределенности трита
}

Trit operator ! (Trit a){
    switch(a){
        case Trit::False:
            return Trit::True;
        case Trit::Unknown:
            return Trit::Unknown;
        case Trit::True:
            return Trit::False;
    }
    return Trit::Unknown;
}

Trit operator | (Trit a, Trit b){
    switch(a){
        case Trit::False:
            switch(b){
                case Trit::False:
                    return Trit::False;
                case Trit::Unknown:
                    return Trit::Unknown;
                case Trit::True:
                    return Trit::True;
            }
        case Trit::Unknown:
            switch(b){
                case Trit::False:
                    return Trit::Unknown;
                case Trit::Unknown:
                    return Trit::Unknown;
                case Trit::True:
                    return Trit::True;
            }
        case Trit::True:
            switch(b){
                case Trit::False:
                    return Trit::True;
                case Trit::Unknown:
                    return Trit::True;
                case Trit::True:
                    return Trit::True;
            }
    }
    return Trit::Unknown;
}

Trit operator & (Trit a, Trit b){
    switch(a){
        case Trit::False:
            switch(b){
                case Trit::False:
                    return Trit::False;
                case Trit::Unknown:
                    return Trit::False;
                case Trit::True:
                    return Trit::False;
            }
        case Trit::Unknown:
            switch(b){
                case Trit::False:
                    return Trit::False;
                case Trit::Unknown:
                    return Trit::Unknown;
                case Trit::True:
                    return Trit::Unknown;
            }
        case Trit::True:
            switch(b){
                case Trit::False:
                    return Trit::False;
                case Trit::Unknown:
                    return Trit::Unknown;
                case Trit::True:
                    return Trit::True;
            }
    }
    return Trit::Unknown;
}