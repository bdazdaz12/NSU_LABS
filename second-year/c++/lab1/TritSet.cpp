#include "TritSet.h"

inline uint8_t TritSet::getTritMask(uLL blockIdx,  uint8_t tritInBlockIdx) const{
    return (uintVector[blockIdx] >> (tritInBlockIdx * 2u)) & 3u;
}

inline void TritSet::setTritMask(uLL blockIdx,  uint8_t tritInBlockIdx, uint tritMask){
    uintVector[blockIdx] &= ~(3u << (tritInBlockIdx * 2u));
    uintVector[blockIdx] |= (tritMask << (tritInBlockIdx * 2u));
}

inline Trit TritSet::bitMaskInterpreter(uint8_t value) const{
    return static_cast<Trit>(value);
}

TritSet::tritHandler TritSet::operator [] (uLL tritIdx) {
    tritHandler handler(tritIdx, *this);
    return handler;
}

const TritSet TritSet::operator | (const TritSet& scndArg) const{
    TritSet ans(scndArg.size() > this->size()? scndArg.size() : this->size());
    ans.uintVector = scndArg.size() > this->size()? scndArg.uintVector : this->uintVector;
    uint minSetSize = scndArg.size() > this->size()? this->uintVector.size() : scndArg.uintVector.size();
    for (uint i = 0; i < minSetSize; ++i) {
        ans.uintVector[i] |= scndArg.size() > this->size()? this->uintVector[i] : scndArg.uintVector[i];
    }
    return move(ans);
}

TritSet& TritSet::operator |= (const TritSet& scndArg){
    *this = *this | scndArg;
    return *this;
}

const TritSet TritSet::operator & (const TritSet& scndArg) const{
    TritSet ans(scndArg.size() > this->size()? scndArg.size() : this->size());
    ans.uintVector = scndArg.size() > this->size()? scndArg.uintVector : this->uintVector;
    uint minSetSize = scndArg.size() > this->size()? this->uintVector.size() : scndArg.uintVector.size();
    for (uint i = 0; i < minSetSize; ++i){
        ans.uintVector[i] &= scndArg.size() > this->size()? this->uintVector[i] : scndArg.uintVector[i];
    }
    return move(ans);
}

TritSet& TritSet::operator &= (const TritSet& scndArg){
    *this = *this & scndArg;
    return *this;
}

const TritSet TritSet::operator ! () const{
    TritSet reverseTritSet(this->size());
    reverseTritSet.uintVector = this->uintVector;
    reverseTritSet.cntTritsInSet = this->cntTritsInSet;
    for (uint& block : reverseTritSet.uintVector) {
        block = ~block;
        for (uint i = 0; i < cntOfTritsInBlock; ++i) {
            if (((block >> (i * 2u + 1u)) & 1u) && !((block >> (i * 2u)) & 1u)) {
                block &= ~(3u << (i * 2u));
                block |= (1u << (i * 2u));
            } ///переводим unknown в более привычный формат 10 -> 01
        }
    }
    return move(reverseTritSet);
}

TritSet& TritSet::operator = (const TritSet& set) {
    if (this == &set) {
        return *this;
    }
    this->cntTritsInSet = set.cntTritsInSet;
    this->uintVector = set.uintVector;
    return *this;
}

TritSet::TritSet(uLL size){
    cntTritsInSet = size;
    uintVector.resize(ceil(size / 16.), unknownTritMask);
}

TritSet::TritSet(const TritSet& set) {
    cntTritsInSet = set.cntTritsInSet;
    uintVector = set.uintVector;
}

uLL TritSet::capacity() const { ///количество занятых блоков
    return ceil(cntTritsInSet / 16.);
}

uLL TritSet::size() const { ///размер множества
    return cntTritsInSet;
}

Trit TritSet::getTrit(uLL tritIdx) const { //TODO:индексация тритов в блоке идет справа налево <--
    if (tritIdx >= cntTritsInSet){        //TODO:индексируем с нуля  ... 3 2 1 <-- 0
        return Trit::Unknown;
    }
    uLL blockIdx = tritIdx / 16u;
    uint8_t tritInBlockIdx = tritIdx % cntOfTritsInBlock;
    return bitMaskInterpreter(getTritMask(blockIdx, tritInBlockIdx));
}

void TritSet::setTrit(uLL tritIdx, Trit trit){
    if (trit == Trit::Unknown && tritIdx >= cntTritsInSet){
        return;
    }
    if (tritIdx >= cntTritsInSet){
        uintVector.resize(ceil((tritIdx + 1.) / 16.), unknownTritMask);
        cntTritsInSet = tritIdx + 1u;
    }
    uLL blockIdx = tritIdx / 16u;
    uint8_t tritInBlockIdx = tritIdx % cntOfTritsInBlock;
    setTritMask(blockIdx, tritInBlockIdx, tritSign(trit));
}

void TritSet::shrink(){
    uLL curAllocMemSize = uintVector.size();
    for(uLL i = curAllocMemSize - 1; i >= 0; --i){
        if(uintVector[i] ^ unknownTritMask){
            uintVector.resize(i + 1);
            cntTritsInSet = (i + 1) * cntOfTritsInBlock;
            break;
        }
        if(!i){
            uintVector.resize(0);
            cntTritsInSet = 0;
            return;
        }
    }
    // работаем с последним определенным блоком (вычтем кол-во тритов, которого не хватает до полного последнего блока)
    uint lastDeterminedBlock = uintVector[uintVector.size() - 1];
    for(int i = cntOfTritsInBlock - 1; i >= 0; --i){ //идем с конца блока, т.е. слева направо
        if ((lastDeterminedBlock >> (i * 2u + 1u) & 1u) || !(lastDeterminedBlock >> (i * 2u) & 1u)) {
            cntTritsInSet -= cntOfTritsInBlock - (i + 1);
            break;
        }
    }
}

size_t TritSet::cardinality(Trit value){
    uLL thisSetLength = this->length();
    if (thisSetLength == 0){
        return 0;
    }
    thisSetLength--;
    uLL ans = 0;
    uLL lastBlockIdx = thisSetLength / 16;
    uint8_t lastTritInLastBlockIdx = thisSetLength % 16;
    for (uLL i = 0; i <= lastBlockIdx; ++i){
        uint8_t lastTritInCurBlockIdx = 15;
        if (i == lastBlockIdx){
            lastTritInCurBlockIdx = lastTritInLastBlockIdx;
        }
        for (uint8_t j = 0; j <= lastTritInCurBlockIdx; ++j){
            uint8_t curMask = 0;
            curMask |= ((uintVector[i] >> (j * 2u + 1u)) & 1u) << 1u;
            curMask |= (uintVector[i] >> (j * 2u)) & 1u;
            if (bitMaskInterpreter(curMask) == value){
                ans++;
            }
        }
    }
    return ans;
}

unordered_map<Trit, uLL, tritSignificator> TritSet::cardinality(){
    unordered_map<Trit, uLL, tritSignificator> mapOfTritsInSet;
    mapOfTritsInSet.insert({Trit::False, cardinality(Trit::False)});
    mapOfTritsInSet.insert({Trit::Unknown, cardinality(Trit::Unknown)});
    mapOfTritsInSet.insert({Trit::True, cardinality(Trit::True)});
    return mapOfTritsInSet;
}

void TritSet::trim(uLL lastIndex){
    if (lastIndex >= cntTritsInSet){
        return;
    }
    uLL lastBlockIdx = lastIndex / 16;
    for (uint tritInBlockIdx = lastIndex % cntOfTritsInBlock; tritInBlockIdx < cntOfTritsInBlock; ++tritInBlockIdx){
        setTritMask(lastBlockIdx, tritInBlockIdx, tritSign(Trit::Unknown));
    }
    for (uint j = lastBlockIdx + 1; j < uintVector.size(); ++j) {
        uintVector[j] = unknownTritMask;
    }
}

uLL TritSet::length(){ ///индекс послднего не unknown трита + 1  (00 или 11)
    for (long long i = uintVector.size() - 1; i >= 0 ; --i){
        for (char j = cntOfTritsInBlock - 1; j >= 0 ; --j){
            if (((uintVector[i] >> (j * 2u + 1u)) & 1u) || !((uintVector[i] >> (j * 2u)) & 1u)){
                return (i * cntOfTritsInBlock + j) + 1;
            }
        }
    }
    return 0;
}
