#include "stdafx.h"
#include "Reader.h"

Reader::Reader(const std::string & filePath)
{
    open(filePath);
}

Reader::~Reader()
{
    close();
}

void Reader::open(const std::string & filePath)
{
    m_filePath = filePath;
    m_file.open(filePath.c_str(), std::ios_base::in /*| std::ios_base::binary*/);
    if (m_file.fail())
    {
        std::cerr<<"READER : Error load file " << m_filePath << std::endl;
    }

    nextChar();
}

bool Reader::endFile()
{
    bool isEnd = m_file.peek()==EOF;
    if (m_file.eof())
    {
        m_file.clear();
    }
    return isEnd;
}

void Reader::nextChar()
{
    if (m_file.peek()==EOF)
        m_curChar=EOF;
    else
    {
        m_file.get(m_curChar);
    }
}

std::string Reader::nextWord()
{
    static std::string s;
    s="";
    skipSeparator();
    if (isdigit(m_curChar))
    {
        do {
            s=s+m_curChar;
            nextChar();
        } while (isdigit(m_curChar));
    }
    else if (isalpha(m_curChar))
    {
        do {
            s=s+m_curChar;
            nextChar();
        } while (isalpha(m_curChar) || isdigit(m_curChar) || m_curChar == '_' || m_curChar =='.' || m_curChar == '/');
    }
    else if (m_curChar != EOF)
    {
        s=s+m_curChar;
        nextChar();
    }
    return s;
}

void Reader::skipSeparator()
{
    while (m_curChar == ' '  || m_curChar == '\t' ||
           m_curChar == '\r' || m_curChar == '\n' ||
           m_curChar == ':')
            nextChar();
}
