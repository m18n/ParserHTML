#include <iostream>
#include <unistd.h>
#include <ctime>
#include "include/parser.h"
using namespace std;
int main()
{
    std::cout << "START\n";

    const char hay[1000] = "<title id=\"23\" vent=\"df\">Document<p vsd=\"sdsf\">NIGAR</p></title>";
    parse::argument ar;
    ar.data = "23";
    ar.name = "id";
    
    try
    {
        parse::tag tg=parse::SearchTag(hay,strlen(hay),"title",ar);
        std::cout<<"\n\n";
        
    }
    catch (int e)
    {
        std::cerr <<"ERROR: "<< e << '\n';
    }

    // unsigned int start=clock();
    // parse::tag tag=parse::SearchTag(hay,strlen(hay),"div",ar);
    // unsigned int end=clock();
    // unsigned int search_time = (end - start);
    // std::cout<<"TIME: "<<search_time<<"\n";
    // std::vector<parse::argument>pars=tag.arguments.SearchArgumentOfVal("men");
    // for(int i=0;i<pars.size();i++){
    //     std::cout<<"ARG NAME: "<<pars[i].name<<" VAL: "<<pars[i].data<<"\n";
    // }
    return 0;
}