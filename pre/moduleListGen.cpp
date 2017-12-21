#include <iostream>

using namespace std;

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <fstream>

vector<string> listDir(const char *path)
{
    vector<string> output;
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        return output;
    }
    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;
        if (name.length() > 2 && name.substr(name.length()-2, 2) == ".h"){
            output.push_back(name);
            //cout << name << '\n';
        }
    }
    closedir(dir);
    return output;
}

vector<string> listNames(vector<string> files){
    vector<string> output;
    for (int x = 0; x < files.size(); x++){
        string filePath = "Modules/" + files[x];
        ifstream in(filePath.c_str());
        string line;
        getline(in, line);
        //cout << line << '\n';
        output.push_back(line.substr(3, line.length()-4));
        //cout << output[x] << '\n';
        in.close();
    }
    return output;
}

int main(){
    vector<string> files = listDir("Modules");
    vector<string> names = listNames(files);
    ofstream out("modulesList.h");
    out << "#pragma once\n\n#include <vector>\n#include \"dataStructures.h\"\n\n";
    for (int x = 0; x < files.size(); x++){
      out << "#include \"Modules/" << files[x] << "\"\n";
    }
    out << "\nusing namespace std;\n\n";
    out << "vector<string> getGames(){\n\tvector<string> output;\n";
    for (int x = 0; x < names.size(); x++){
      out << "\toutput.push_back(\"" << names[x] << "\");\n";
    }
    out << "\treturn output;\n}\n\n";
    out << "Game* getGame(string name){\n\tGame * g;\n";
    for (int x = 0; x < names.size(); x++){
      out << "\tif (name == \"" << names[x] << "\"){\n\t\tg = new " << files[x].substr(0, files[x].size() - 2) << "();\n\t}\n";
    }
    out << "\treturn g;\n}\n";
    out.close();
    return 0;
}
