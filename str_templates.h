/*
MIT License

Copyright (c) 2020 Daniel Gutson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef STR_TEMPLATES_H
#define STR_TEMPLATES_H

#include <string>
#include <list>

inline std::string rtrim(const std::string& str)
{
    auto rightLen = str.length();
    while(rightLen >= 1 && str[rightLen - 1] == ' ')
    {
        --rightLen;
    }
    return str.substr(0, rightLen);
}

class StrTemplate
{
    using Lines = std::list<std::string>;
    Lines lines;

    bool replaceOne(const std::string& target, const StrTemplate& other)
    {
        auto it = lines.begin();
        auto pos = std::string::npos;
        
        while (it != lines.end() && pos == std::string::npos)
        {
            pos = it->find(target);
            ++it;
        }
        
        auto found = pos != std::string::npos;
        if (found)
        {
            --it;
            it->erase(pos, target.length());
            if (!other.lines.empty())
            {
                it->insert(pos, other.lines.front());
                auto otherSecondRow = other.lines.begin();
                // insert since the second row
                for (otherSecondRow++, it++; otherSecondRow != other.lines.end(); ++otherSecondRow)
                {
                    if (!otherSecondRow->empty())
                    {
                        lines.insert(it, std::string(pos, ' ') + *otherSecondRow);
                    }
                    else
                    {
                        lines.insert(it, std::string());
                    }
                }
            }
        }
        return found;
    }

    bool replaceAll(const std::string& target, const StrTemplate& other)
    {
        auto replaced = false;
        while (replaceOne(target, other))
        {
            replaced = true;
        }
        return replaced;
    }

public:
    class LineMaker
    {
        Lines lines;
        StrTemplate* parent;
    public:
        LineMaker& operator+ (const std::string& line)
        {
            lines.emplace_back(rtrim(line));

            return *this;
        }
        
        LineMaker(const LineMaker& other) = delete;
        LineMaker(StrTemplate* parent) : parent(parent) {}
        LineMaker(LineMaker&& other) : parent(other.parent)
        {
            lines = std::move(other.lines);
            other.parent = nullptr;
        }
        
        ~LineMaker()
        {
            if (parent != nullptr)
            {
                parent->lines.insert(parent->lines.end(), lines.begin(), lines.end());
            }
        }
    };
    
    StrTemplate() = default;
    StrTemplate(const StrTemplate& other) = delete;
    StrTemplate(StrTemplate&& other)
    {
        lines = std::move(other.lines);
    }
    
    LineMaker operator()()
    {
        return LineMaker(this);
    }
    
    StrTemplate& operator+=(const StrTemplate& other)
    {
        lines.insert(lines.end(), other.lines.begin(), other.lines.end());
        return *this;
    }
    
    bool replace(unsigned int placeholder, const StrTemplate& other)
    {
        return replaceAll("$" + std::to_string(placeholder), other);
    }

    bool replace(unsigned int placeholder, const std::string& str)
    {
        StrTemplate temp;
        temp() + str;
        return replace(placeholder, temp);
    }

    bool replace(const std::string& key, const StrTemplate& other)
    {
        return replaceAll("$" + key + "$", other);
    }

    bool replace(const std::string& key, const std::string& str)
    {
        StrTemplate temp;
        temp() + str;
        return replace(key, temp);
    }
    

    std::string text() const
    {
        std::string output;
        for (const auto& line : lines)
        {
            output += line + "\n";
        }
        return output;
    }
};

#endif

