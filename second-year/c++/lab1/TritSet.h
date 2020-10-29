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
    void setTritByFullIdx(uLL blockIdx, uLL tritInBlockIdx, Trit tritValue);
    void setTritByIdxInSet(uLL tritInSetIdx, Trit tritValue);
    Trit getTritByIdxInSet(uLL index) const;
//    Trit getTritByFullIdx(uLL beginIdx, uLL endIdx) const;
public:
    explicit TritSet(uLL size);
    TritSet(TritSet &&sourceSet) noexcept;
    uLL capacity();
    uLL size() const;
    void shrink();
    void trim(uLL lastIndex);
    uLL length();
    struct tritHandler{
    private:
        TritSet& processedSet;
        uLL processedTritIdx;
    public:
        tritHandler(uLL tritIdx, TritSet& set) : processedTritIdx(tritIdx), processedSet(set){};
        ~tritHandler() = default;
        operator Trit() const {
            if (processedTritIdx >= processedSet.cntTritsInSet)
                return Trit::Unknown;
            else
                return processedSet.getTritByIdxInSet(processedTritIdx);
        }
        const TritSet& getProcessedSet(){
            return processedSet;
        }
        uLL getProcessedTritIdx() const{
            return processedTritIdx;
        }
        Trit operator | (const Trit& trit) const{
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return Trit::Unknown;
            } else {
                return trit | processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        Trit operator | (const tritHandler& scndHandler) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return Trit::Unknown;
            } else {
                return scndHandler | processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        Trit operator & (const Trit& trit) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return Trit::Unknown;
            } else {
                return trit & processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        Trit operator & (const tritHandler& scndHandler) const{
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return Trit::Unknown;
            } else {
                return scndHandler & processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        Trit operator ! () const {
            if (processedTritIdx >= processedSet.cntTritsInSet) { //this->processedTritIdx
                return Trit::Unknown;
            } else {
                return !processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        bool operator == (const Trit& trit) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return Trit::Unknown == trit;
            } else {
                return trit == processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        bool operator == (const tritHandler& scndHandler) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return false;
            } else {
                return scndHandler == processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        bool operator != (const Trit& trit) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return false;
            } else {
                return trit != processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        bool operator != (const tritHandler& scndHandler) const {
            if (processedTritIdx >= processedSet.cntTritsInSet) {
                return false;
            } else {
                return scndHandler != processedSet.getTritByIdxInSet(processedTritIdx);
            }
        }
        tritHandler& operator = (const Trit& trit) {
            if (trit != Trit::Unknown && processedTritIdx >= processedSet.cntTritsInSet) {
                processedSet.uintVector.resize(ceil((1.0 + processedTritIdx) / 16.), unknownTritMask);
                processedSet.cntTritsInSet = processedTritIdx + 1;
                processedSet.setTritByIdxInSet(processedTritIdx, trit);
            } else if (processedTritIdx < processedSet.cntTritsInSet) {
                processedSet.setTritByIdxInSet(processedTritIdx, trit);
            }
            return *this;
        }
    };
    friend ostream& operator << (ostream& outputStream, TritSet::tritHandler tritHandler) {
        if (tritHandler.getProcessedTritIdx() >= tritHandler.getProcessedSet().cntTritsInSet) {
            return outputStream << Trit::Unknown;
        } else {
            return outputStream << tritHandler.getProcessedSet().getTritByIdxInSet(tritHandler.getProcessedTritIdx());
        }
    }
    tritHandler operator [] (uLL tritIdx);
    TritSet operator & (TritSet &scndArg);
    TritSet operator | (TritSet &scndArg);
    TritSet operator ! ();
};
