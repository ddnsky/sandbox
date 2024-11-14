// not best solution
class Solution {
public:
    bool isPalindrome(string const& s, int start, int len)
    {
        while(len>1)
        {
          if (s[start]!=s[start+len-1])
            return false;
          start++;
          len-=2;  
        }
        return true;
    }
    string longestPalindrome(string s) {
        if (s.empty())
            return {};
        int resultStart = 0;
        int resultLength = 1;
        auto len = s.length();
        for(int i=0; i<len-resultLength; ++i)
        {
            for (int l=len-i; l>resultLength; --l)
            {
                if (isPalindrome(s, i, l))
                {
                    resultStart = i;
                    resultLength = l;
                }
            }
        }
        return s.substr(resultStart, resultLength);
    }
};