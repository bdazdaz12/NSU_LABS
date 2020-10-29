#include "TritSet.h"

inline unsigned int getBit(uint block, unsigned char index) {
    return (block >> index) & 1u;
}

inline void setBit(uint& block, unsigned char bitVal, unsigned char index) {
    if (bitVal) {
        block |= 1u << index;
    } else {
        block &= ~(1u << index);
    }
}

TritSet::tritHandler TritSet::operator [] (uLL tritIdx) {
    tritHandler handler(tritIdx, *this);
    return handler;
}

TritSet TritSet::operator | (TritSet& scndArg){
    TritSet ans(scndArg.size() > size() ? scndArg.size() : this->size());
    ans.uintVector = scndArg.size() > this->size()? scndArg.uintVector : this->uintVector;
    ans.cntTritsInSet = scndArg.size() > this->size()? scndArg.cntTritsInSet : this->cntTritsInSet;
    uint minSetSize = scndArg.size() > this->size()? this->uintVector.size() : scndArg.uintVector.size();
    for (uint i = 0; i < minSetSize; ++i) {
        ans.uintVector[i] |= scndArg.size() > this->size()? this->uintVector[i] : scndArg.uintVector[i];
    }
    return move(ans);
}

TritSet TritSet::operator & (TritSet& scndArg){
    TritSet ans(scndArg.capacity() > this->size() ? scndArg.size() : this->size());
    ans.uintVector = scndArg.size() > this->size() ? scndArg.uintVector : this->uintVector;
    ans.cntTritsInSet = scndArg.size() > this->size() ? scndArg.cntTritsInSet : this->cntTritsInSet;
    uint minSetSize = scndArg.size() > this->size() ? this->uintVector.size() : scndArg.uintVector.size();
    for (uint i = 0; i < minSetSize; ++i){
        ans.uintVector[i] &= scndArg.size() > this->size() ? this->uintVector[i] : scndArg.uintVector[i];
    }
    return move(ans);
}

TritSet TritSet::operator ! (){
    TritSet reverseTritSet(this->size());
    reverseTritSet.uintVector = this->uintVector;
    reverseTritSet.cntTritsInSet = this->cntTritsInSet;
    for (uint& block : reverseTritSet.uintVector) {
        block = ~block;
        for (uint i = 0; i < cntOfTritsInBlock; ++i) {
            if (((block >> (i * 2u + 1u)) & 1u) && !((block >> (i * 2u)) & 1u)) {
                setBit(block, 0, i * 2u + 1u);
                setBit(block, 1, i * 2u);
            } ///переводим unknown в более привычный формат 10 -> 01
        }
    }
    return move(reverseTritSet);
}

TritSet::TritSet(uLL size){
    cntTritsInSet = size;
    uintVector.resize(ceil(size / 16.), unknownTritMask);
}

TritSet::TritSet(TritSet&& sourceSet) noexcept {
    uintVector = move(sourceSet.uintVector);
    cntTritsInSet = sourceSet.cntTritsInSet;
    sourceSet.cntTritsInSet = 0;
}

uLL TritSet::capacity(){ ///количество занятых блоков
    return ceil(cntTritsInSet / 16.);
}

//Trit TritSet::getTritByFullIdx(uLL beginIdx, uLL endIdx) const{
//    uint8_t value = 0;
//    value |= getBit(uintVector[beginIdx], endIdx * 2 + 1) << 1u;
//    value |= getBit(uintVector[beginIdx], endIdx * 2);
//    switch (value) {
//        case 0:
//            return Trit::False;
//        case 1:
//            return Trit::Unknown;
//        case 3:
//            return Trit::True;
//        default:
//            return Trit::Unknown;
//    }
//}

Trit TritSet::getTritByIdxInSet(uLL index) const {
    uLL blockIdx = index / 16;
    uLL tritInBlockIdx = index % cntOfTritsInBlock;
    uint8_t value = 0;
    value |= getBit(uintVector[blockIdx], tritInBlockIdx * 2 + 1) << 1u;
    value |= getBit(uintVector[blockIdx], tritInBlockIdx * 2);
    switch (value){
        case 0:
            return Trit::False;
        case 1:
            return Trit::Unknown;
        case 3:
            return Trit::True;
        default:
            return Trit::Unknown;
    }
}

void TritSet::setTritByIdxInSet(uLL tritInSetIdx, Trit tritValue){
    uLL blockIdx = tritInSetIdx / 16;
    uint tritInBlockIdx = tritInSetIdx % cntOfTritsInBlock;
    setBit(uintVector[blockIdx], tritValue & 1u, tritInBlockIdx * 2);
    setBit(uintVector[blockIdx], (tritValue >> 1) & 1u, tritInBlockIdx * 2 + 1);
}

void TritSet::setTritByFullIdx(uLL blockIdx, uLL tritInBlockIdx, Trit tritValue) {
    setBit(uintVector[blockIdx], tritValue & 1u, tritInBlockIdx * 2);
    setBit(uintVector[blockIdx], (tritValue >> 1) & 1u, tritInBlockIdx * 2 + 1);
}

void TritSet::shrink(){
    uint curAllocMemSize = uintVector.size();
    for(uint i = curAllocMemSize - 1;; --i){
        if(uintVector[i] ^ unknownTritMask){
            uintVector.resize(i + 1);
            cntTritsInSet -= cntTritsInSet % cntOfTritsInBlock + 1;
            cntTritsInSet -= (curAllocMemSize - i - 2) * cntOfTritsInBlock;
            break;
        }
        if(!i){
            uintVector.resize(0);
            cntTritsInSet = 0;
            return;
        }
    }
    uint lastDeterminedBlock = uintVector[uintVector.size() - 1];
    for(uint i = 0; i <= cntTritsInSet % cntOfTritsInBlock; ++i){ //идем с конца блока, т.е. справа налево
        if ((lastDeterminedBlock >> (i * 2u + 1u) & 1u) || !(lastDeterminedBlock >> (i * 2u) & 1u)) {
            cntTritsInSet -= cntTritsInSet % cntOfTritsInBlock - (i + 1);
            break;
        }
    }
}

void TritSet::trim(uLL lastIndex){
    if (lastIndex >= cntTritsInSet) {
        return;
    }
    uLL lastBlockIdx = lastIndex / 16;
    for (uint tritInBlockIdx = lastIndex % cntOfTritsInBlock; tritInBlockIdx < cntOfTritsInBlock; ++tritInBlockIdx){
        setTritByFullIdx(lastBlockIdx, tritInBlockIdx, Trit::Unknown);
    }
    for (uint j = lastBlockIdx + 1; j < uintVector.size(); ++j) {
        uintVector[j] = unknownTritMask;
    }
}

uLL TritSet::length(){ ///индекс послднего не unknown трита -- 00 или 11
    for (long long i = uintVector.size() - 1; i >= 0 ; --i){
        for (long long j = cntOfTritsInBlock - 1; j >= 0 ; --j) {
            if (((uintVector[i] >> (j * 2u + 1u)) & 1u) || !((uintVector[i] >> (j * 2u)) & 1u)) {
                return (i * cntOfTritsInBlock + j) + 1;
            }
        }
    }
    return 0;
}

inline uLL TritSet::size() const {
    return cntTritsInSet;
}
