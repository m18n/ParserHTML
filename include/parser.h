#pragma once
#include <iostream>
#include <cstring>
#include <vector>
namespace details
{
    struct argsizestr
    {
        int sizearg = 0;
        int sizedata = 0;
    };
    argsizestr GetSizeArgument(const char *text, int size, int start);
   
}
namespace parse
{
    typedef char bool3;
    struct argument
    {
        std::string name = "";
        std::string data = "";
    };
    class Arguments
    {
    public:
        Arguments(std::vector<argument> argumente)
        {
            this->arguments = argumente;
           
        }
        Arguments()
        {
        }
        void SetArgument(std::vector<argument> argum)
        {
            this->arguments = argum;
        }

    public:
        argument SearchArgument(std::string name)
        {
            for (int i = 0; i < arguments.size(); i++)
            {
                if (arguments[i].name == name)
                {
                    return arguments[i];
                }
            }
        }
        std::vector<argument> SearchArgumentOfVal(std::string data)
        {
            int size = 0;
            for (int i = 0; i < arguments.size(); i++)
            {
                if (arguments[i].data == data)
                {
                    size++;
                }
            }
            std::vector<argument> arr(size);
            for (int i = 0; i < arguments.size(); i++)
            {
                if (arguments[i].data == data)
                {
                    arr[i] = arguments[i];
                }
            }
            return std::move(arr);
        }
        void Show(){
            for(int i=0;i<arguments.size();i++){
                std::cout<<"ARGUM: "<<arguments[i].name<<" VALUE: "<<arguments[i].data<<"\n";
            }
        }
        void ShowArgum(){
            std::cout<<"ARGUM: "<<&arguments[0]<<"\n";
        }
    private:
        std::vector<argument> arguments;
    };
    struct tag
    {
        std::string nametag;
        std::vector<tag*> childs;
        Arguments arguments;
        std::string content;
        bool3 typetag=false;
        int start=-1;
    };

    namespace str
    {
        int SearchWord(const char *text, int size, const char *word);
        int SearchWordRevers(const char *text, int start, std::string word);
        void ReversStr(std::string &str);
       std::string GetWord(const char *text, int size, char stop);
    }
    tag* ParseTag(const char* text,int start,int size);
    void ParseContentTag(const char* text,tag* tg,int start,int size);
    std::vector<parse::argument> GetArgumnets(const char *text, int start, int end);
    tag* SearchTag(const char *text, int size, std::string nametag, argument filter);
}