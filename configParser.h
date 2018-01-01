using namespace std;

int parseConfigFile(const char *configPath, map<string, string> &configMap) {
    string buffer;
    string key;
    string value;
    ifstream configFile(configPath);
    if(configFile.is_open()) {
        while(!configFile.eof()) {
            getline(configFile, buffer);
            if(!buffer.empty()) {
                if(buffer.find("[")) {
                    key = buffer.substr(0, buffer.find("="));
                    value = buffer.substr(buffer.find("=") + 1, buffer.find(";") - buffer.find("=") - 1);
                    configMap[key] = value;
                }
            }
        }
    }
    else {
        cerr << "Cant open Config File" << endl;
        return 1;
    }
    
    configFile.close();
    return 0;
}
