#pragma once

#include "Trit.h"
#include <unordered_map>
#include <vector>
#include <cmath>

#define cntOfTritsInBlock 16u
#define unknownTritMask 1431655765u //0101...010101

using namespace std;

typedef uint32_t uint;
typedef uint64_t uLL;

class TritSet{
private:
    vector<uint> uintVector;
    uLL cntTritsInSet = 0;
    inline uint8_t getTritMask(uLL blockIdx,  uint8_t tritInBlockIdx) const;
    inline void setTritMask(uLL blockIdx,  uint8_t tritInBlockIdx, uint tritMask);
    Trit getTrit(uLL tritIdx) const;
    void setTrit(uLL tritIdx, Trit trit);
    inline Trit bitMaskInterpreter(uint8_t value) const;
    struct tritHandler{
    private:
        TritSet& processedSet;
        uLL processedTritIdx;
    public:
        tritHandler(uLL tritIdx, TritSet& set) : processedTritIdx(tritIdx), processedSet(set){};
        operator Trit() const {
            return processedSet.getTrit(processedTritIdx);
        }
        const TritSet& getProcessedSet(){
            return processedSet;
        }
        uLL getProcessedTritIdx() const{
            return processedTritIdx;
        }
        Trit operator | (Trit trit) const{
            return trit | processedSet.getTrit(processedTritIdx);
        }
        tritHandler& operator |= (Trit trit){
            processedSet.setTrit(processedTritIdx, processedSet.getTrit(processedTritIdx) | trit);
            return *this;
        }
        Trit operator & (Trit trit) const{
            return trit & processedSet.getTrit(processedTritIdx);
        }
        tritHandler& operator &= (Trit trit){
            processedSet.setTrit(processedTritIdx, processedSet.getTrit(processedTritIdx) & trit);
            return *this;
        }
        Trit operator ! () const {
            return !processedSet.getTrit(processedTritIdx);
        }
        bool operator == (Trit trit) const {
            return trit == processedSet.getTrit(processedTritIdx);
        }
        bool operator != (Trit trit) const {
            return trit != processedSet.getTrit(processedTritIdx);
        }
        tritHandler& operator = (Trit trit){
            processedSet.setTrit(processedTritIdx, trit);
            return *this;
        }
    };
public:
    TritSet() = default;
    explicit TritSet(uLL size);
    TritSet(const TritSet& set);
    uLL capacity() const;
    uLL size() const;
    void shrink();
    size_t cardinality(Trit value);
    unordered_map<Trit, uLL, tritSignificator> cardinality();
    void trim(uLL lastIndex);
    uLL length();
    friend ostream& operator << (ostream& outputStream, tritHandler tritHandler) {
        return outputStream << tritHandler.getProcessedSet().getTrit(tritHandler.getProcessedTritIdx());
    }
    tritHandler operator [] (uLL tritIdx);
    const TritSet operator | (const TritSet& scndArg) const;
    TritSet& operator |= (const TritSet& scndArg);
    const TritSet operator & (const TritSet& scndArg) const;
    TritSet& operator &= (const TritSet& scndArg);
    const TritSet operator ! () const;
    TritSet& operator = (const TritSet& set);
};
