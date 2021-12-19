#include "include/parser.h"
details::argsizestr details::GetSizeArgument(const char *text, int size, int start)
{
    argsizestr arg;
    bool wr = false;
    int d = 0;
    for (int i = start; i < size; i++)
    {

        if (text[i] == '=')
        {
            wr = true;
        }
        else if (text[i] == '\"')
        {
            d++;
            if (d == 1 && text[i - 1] != '=')
                throw 1;
            else if (d == 2 && text[i - 1] == '=')
                throw 1;
            else if (d == 2)
                break;
        }

        else if (wr == false)
        {
            arg.sizearg++;
        }
        else if (wr == true)
        {
            arg.sizedata++;
        }
    }
    if (d != 2)
        throw 1;
    return arg;
}

std::vector<parse::argument> parse::GetArgumnets(const char *text, int start, int end)
{
    int numarg = 0;
    for (int i = start; i < end; i++)
    {
        if (text[i] == '=')
        {
            numarg++;
        }
    }
    std::vector<argument> array(numarg);

    int n = -1;
    bool write = false;
    int k = 0;
    details::argsizestr arg;
    try
    {
        for (int i = start; i < end; i++)
        {

            if (text[i] == '=')
            {
                write = true;
                k = 0;
            }
            else if (text[i] == ' ' && text[i + 1] != ' ')
            {
                arg = details::GetSizeArgument(text, end, i + 1);

                n++;
                write = false;
                k = 0;
                array[n].name.resize(arg.sizearg);
                array[n].data.resize(arg.sizedata);
            }
            else if (text[i] == '\"')
            {
                continue;
            }
            else if (text[i] != ' ')
            {
                if (write == false)
                {
                    array[n].name[k] = text[i];
                    k++;
                }
                else
                {
                    array[n].data[k] = text[i];
                    k++;
                }
            }
        }
        return std::move(array);
    }
    catch (int a)
    {
        std::cout << "ERROR PARSE ARGUMENT: " << a << "\n";
    }
}
parse::tag parse::ParseTag(const char *text, int start, int size)
{

    int indexend = 0;
    int indexstartat = 0;

    for (int i = start; i < size; i++)
    {

        if (text[i] == '>')
        {
            indexend = i;
            indexstartat = i;
            break;
        }
    }
    for (int i = start; i < indexend; i++)
    {
        if (text[i] == ' ')
        {
            indexstartat = i;
            break;
        }
    }
    std::string nametag = "";
    int sizenametag = indexstartat - start - 1;
    nametag.resize(sizenametag);
    memcpy(&nametag[0], &text[start + 1], sizenametag);
    std::vector<argument> arr;
    if (indexend != indexstartat)
        arr = GetArgumnets(text, indexstartat, indexend);
    tag tg;
    tg.start = indexend + 1;
    tg.nametag = nametag;
    tg.arguments = std::move(arr);
    tg.childs.push_back(tg);
    return tg;
}
std::string parse::str::GetWord(const char *text, int size, char stop)
{
    int indexend = 0;
    for (int i = 0; i < size; i++)
    {
        if (text[i] == stop)
        {
            indexend = i;
            break;
        }
    }
    std::string str = "";
    str.resize(indexend);
    for (int i = 0; i < indexend; i++)
    {
        str[i] = text[i];
    }
    return std::move(str);
}
parse::tag parse::ParseContentTag(const char *text, int start, int size)
{
    tag tg;
    int sizetag = 0;
    tag *tt = &tg;
    for (int i = start; i < size; i++)
    {
        if (text[i] == '<' && text[i + 1] == '/')
        {

            std::string end = str::GetWord(&text[i + 2], size - i, '>');
            int sizej = tt->childs.size();
            int index = -1;

            for (int j = sizej - 1; j >= 0; j--)
            {
                if (tt->childs[j].typetag == false)
                {
                    if (tt->childs[j].nametag != end)
                    {
                        throw 5;
                    }
                    index = j;
                    if(index==0)
                        tt->typetag=true;
                    tt->childs[index].typetag = true;
                    break;
                }
            }
            if (index == -1)
                return tg;
            std::string content = "";
            int sizecontent = (i - 1) - tt->childs[index].start + 1;
            content.resize(sizecontent);
            int start = tt->childs[index].start;
            for (int j = start; j < i; j++)
            {
                content[j - start] = text[j];
            }
            std::cout << "CONTENT: " << content << "\n";
            if(index==0)
                tt->content=content;
            tt->childs[index].content = std::move(content);
            bool endf = true;
            for (int j = sizej - 1; j >= 0; j--)
            {
                if (tt->childs[j].typetag == false)
                {
                    endf = false;
                }
            }
            if (endf == true)
            {
                // tag* temp;
                tt = tt->father;
            }
        }
        else if (text[i] == '<')
        {
            tag te;

            te = ParseTag(text, i, size);
            te.arguments.Show();
            for (int j = i; j < size; j++)
            {
                if (text[j] == '>')
                {
                    te.start = j + 1;
                    break;
                }
            }

            tg.childs.push_back(tag(te));

            sizetag++;
            if (sizetag == 1)
            {
                tg.nametag = te.nametag;
                tg.arguments = te.arguments;
                tg.start = te.start;
                tg.typetag = te.typetag;
            }
            else
            {
                tag *ft = tt;
                tt = &tg.childs[sizetag-1];
                tt->father = ft;
            }
        }
    }
    for (int i = 0; i < tg.childs.size(); i++)
    {
        if (tg.childs[i].typetag == false)
            throw 6;
    }
    return tg;
}
parse::tag parse::SearchTag(const char *text, int size, std::string nametag, argument filter)
{

    int index = str::SearchWord(text, size, (filter.name + "=\"" + filter.data + "\"").c_str());
    int indextag = str::SearchWordRevers(text, index, "<" + nametag);
    std::cout << &text[indextag] << "\n";

    tag tg = ParseContentTag(text, indextag, size);

    return tg;
}
int parse::str::SearchWord(const char *text, int size, const char *word)
{
    int lenword = strlen(word);
    int n = 0;
    for (int i = 0; i < size; i++)
    {

        if (text[i] == word[n])
        {

            n++;

            if (n == lenword)
            {
                return i - n + 1;
            }
        }
        else
        {
            n = 0;
        }
    }
    throw 2;
}
void parse::str::ReversStr(std::string &str)
{

    for (int i = 0; i < str.length() / 2; i++)
    {
        char temp = str[i];
        str[i] = str[str.length() - i - 1];
        str[str.length() - i - 1] = temp;
    }
}
int parse::str::SearchWordRevers(const char *text, int start, std::string word)
{
    int n = 0;
    ReversStr(word);
    std::cout << "WORD: " << word << "\n";
    for (int i = start; i >= 0; i--)
    {
        if (text[i] == word[n])
        {

            n++;

            if (n == word.length())
            {
                return i;
            }
        }
        else
        {
            n = 0;
        }
    }
    throw 2;
}