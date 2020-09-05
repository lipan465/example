#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

int GetMaxLength(const char* str, std::string& tmpstr) {
	int size = 0;
	const char* p = str;
	const char* p2 = str;
	bool bitmap[128] = {false};
 
	memset(&bitmap,0,sizeof(bitmap));
	for(;*p != 0;p++) {
		if(!bitmap[*p]) {
			bitmap[*p] = true;
		}
		else {
			memset(&bitmap,0,sizeof(bitmap));
			if((p - p2) > size) {
				size = p - p2;
				tmpstr.assign(p2,size);
			}
			p = strchr(p2,*p);
			p2 = p+1;
		}
	}
	if((p - p2) > size) {
		size = p - p2;
		tmpstr.assign(p2,size);
	}
	return size;
}
 
int GetMaxLength2(const char* str, std::string& tmpstr) {
	int size = 0;
	const char* p = str;
	const char* p2 = str;
	const char* bitmap[128] = {0};
 
	memset(&bitmap,0,sizeof(bitmap));
	for(;*p != 0;p++) {
		if(bitmap[*p] == NULL) {
			bitmap[*p] = p;
		}
		else {
			if((p - p2) > size) {
				size = p - p2;
				tmpstr.assign(p2,size);
			}
			p = bitmap[*p];
			p2 = p+1;
			memset(&bitmap,0,sizeof(bitmap));
		}
	}
	if((p - p2) > size) {
		size = p - p2;
		tmpstr.assign(p2,size);
	}
	return size;
}
 
int main(int argc, char* argv[])
{
	const char* str[] = {"1234562789asd",
		"12345678",
		"arabcacfr",
		"aaa12345666"};
 
	for(int i=0;i<sizeof(str)/sizeof(char*);i++) {
		std::string tmpstr;
		printf("%s %d ", str[i], GetMaxLength(str[i], tmpstr));
        printf("%s\n",tmpstr.c_str());
		printf("%s %d ", str[i], GetMaxLength2(str[i], tmpstr));
        printf("%s\n",tmpstr.c_str());
	}
 
	return 0;
}


class Solution {
public:
    int findDuplicate(std::vector<int>& nums) {
        int count = nums.size();
        bool* bits = new bool[count];
        for(int i=0;i<count;++i) {
            int num = nums[i];
            if(bits[num]) return num;
            else bits[num] = true;
        }
        return 0;
    }
};