#pragma once
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;
string unite[5] = { "","十","百","千","万" }; //单位
string num[10] = { "零","一","二","三","四","五","六","七","八","九" }; //一个汉字占两个字节,另外再加一个'\0'字符.


static string convertInt2Chinese(int a)
{
    auto func = [](int a)-> string {
        int flag = 0, tmp;
        string strtmp;
        string result;
        int atemp = a;     //设定a的临时存储值，防止每次进入循环都进行末尾零的判断
        while (a != 0)
        {

            while (atemp % 10 == 0)
            {
                flag++;
                atemp /= 10;
                a /= 10;
            }

            tmp = a % 10;
            if (tmp != 0)
                strtmp = num[tmp] + unite[flag];
            else if (tmp == 0)
                strtmp = "零";
            result = strtmp + result;
            a /= 10;
            flag++;
        }
        return result;
    };

    string result, temp;
    if (a < 100000)
    {
        result = func(a);
    }
    else
    {
        temp = func(a / 10000);
        result = temp + "万" + func(a - a / 10000 * 10000);
    }
    return result;
}

static string  convertFloat2Chinese(double a)
{
    //string s = to_string(a);
    char buf[100] = { 0 };
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%.1f", a);
    string s(buf);
    string lhd = s.substr(0, s.find('.'));
    string rhd = s.substr(s.find('.') + 1, 1);

    string output;

    stringstream s1;
    s1 << lhd;
    int nLhd;
    s1 >> nLhd;

    output += convertInt2Chinese(nLhd);

    if (!rhd.empty() && rhd[0] != '0')
    {
        output += "点";

        stringstream s2;
        s2 << rhd;
        int nRhd;
        s2 >> nRhd;
        output += convertInt2Chinese(nRhd);
    }  

    return output;
}



LPCWSTR StringToLPCWSTR(std::string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}



string WCharToMByte(LPCWSTR lpcwszStr)
{
    string str;
    DWORD dwMinSize = 0;
    LPSTR lpszStr = NULL;
    dwMinSize = WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, NULL, 0, NULL, FALSE);
    if (0 == dwMinSize)
    {
        return FALSE;
    }
    lpszStr = new char[dwMinSize];
    WideCharToMultiByte(CP_OEMCP, NULL, lpcwszStr, -1, lpszStr, dwMinSize, NULL, FALSE);
    str = lpszStr;
    delete[] lpszStr;
    return str;
}


void split(const string& s, vector<string>& tokens, char delim = ' ') {
    tokens.clear();
    auto string_find_first_not = [s, delim](size_t pos = 0) -> size_t {
        for (size_t i = pos; i < s.size(); i++) {
            if (s[i] != delim) return i;
        }
        return string::npos;
    };
    size_t lastPos = string_find_first_not(0);
    size_t pos = s.find(delim, lastPos);
    while (lastPos != string::npos) {
        tokens.emplace_back(s.substr(lastPos, pos - lastPos));
        lastPos = string_find_first_not(pos);
        pos = s.find(delim, lastPos);
    }
}
