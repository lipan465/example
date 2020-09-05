#include <vector>
#include <string>
#include <iostream>

int main() 
{
    std::string tmpStr;
    while(std::cin>>tmpStr) {
        int length = tmpStr.length();
        std::vector<int> dp(length, 0);

        int max = 0;
        for(int i=1;i<length;i++) {
            if(tmpStr[i] == ')') {
                int j = i - dp[i-1] - 1;
                if(j >= 0 && tmpStr[j] == '(') {
                    dp[i] = (i - j + 1);
                    if((j -1) >= 0) dp[i] += dp[j-1];
                }
                max = std::max(max, dp[i]);
            }
        }

        std::cout<<max<<std::endl;
        for(int a : dp) {
            std::cout<<a<<" ";
        }
        std::cout<<std::endl;
    }
    return 0;
}