#include <vector>

namespace vmc {


//Generic Combination Generator
template <class T>
class CombinationGenerator {
public:
    CombinationGenerator(const std::vector<T>& input) : _input{input}, _tempData(input.size()) {}
    CombinationGenerator() = delete;
    ~CombinationGenerator() = default;
    std::vector<std::vector<T>> generateAllCombos() {
        //Reserve some memory for the combinations
        int rSize = (_input.size() < 31) ?  1 << _input.size() : 2147483648;
        std::vector<std::vector<T>> c;
        c.reserve(rSize);
        
        for(int i = 1; i <= _input.size();++i) {
            generateCombos(c, i);
        }
        
        return c;
    }
    std::vector<std::vector<T>> generateCombosChoose(int nCr) {
        //Reserve some memory for the combinations
        int rSize = (_input.size() < 31) ?  1 << _input.size() : 2147483648;
        std::vector<std::vector<T>> c;
        c.reserve(rSize);
        
        generateCombos(c, nCr);
        
        return c;
    }
    
private:
    void generateCombos(std::vector<std::vector<T>>& combos, int nCr) {
        _tempData = std::vector<T>(nCr);
        
        combinationUtil(combos,0,_input.size()-1,0,nCr);
    }
    void combinationUtil(std::vector<std::vector<T>>& combos,int start, int end,int index, int r) {
        // Current combination is ready to be stored
        if (index == r)
        {
            combos.push_back(_tempData);
            return;
        }
        
        // replace index with all possible elements. The condition
        // "end-i+1 >= r-index" makes sure that including one element
        // at index will make a combination with remaining elements
        // at remaining positions
        for (int i=start; i<=end && end-i+1 >= r-index; i++)
        {
            _tempData[index] = _input[i];
            combinationUtil(combos, i+1, end, index+1, r);
        }
    }
    std::vector<T> _input;
    std::vector<T> _tempData;
};

}