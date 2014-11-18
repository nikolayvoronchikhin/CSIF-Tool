#include <iostream>
#include <fstream>
using namespace std;

const string CONFIG_FILE_NAME = "/Users/Shared/csifconfig.txt";
string username;                       // Username for CSIF
string username_verify;                // Verification purposes. 
const int STR_MATCH = 0;

void printAboutInfo() {
    cout << "\n==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==\n";
    cout << "|->  CSIF-Tool v1.0 UC Davis Computer Science \n";
    cout << "|->  Written by Joe Ferrucci, 2014\n";
    cout << "|->     @ (http://joeferrucci.info)\n";
    cout << "|->\n";
    cout << "|->  Help:\n";
    cout << "|->\n";
    cout << "|->     To connect to PC## type:\n";
    cout << "|->         $ csif ##\n";
    cout << "|->           ; where ## is a number 1-60\n";
    cout << "|->\n";
    cout << "|->     To change your username:\n";
    cout << "|->         $ csif -c\n";
    cout << "|->         $ csif -config\n";
    cout << "|->\n";
    cout << "|->     To access this help screen:\n";
    cout << "|->         $ csif -h\n";
    cout << "|->         $ csif -help\n";
    cout << "|->\n";
    cout << "|->   If you have any problems,\n";
    cout << "|->      reset your username!\n";
    cout << "|->\n";
    cout << "|-> https://github.com/JoeFerrucci/CSIF-Tool\n";
    cout << "==-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-==\n\n";
}

void setUsername(ofstream &fout) {
    fout.open(CONFIG_FILE_NAME);
    if ( !fout.is_open() )
    {
        // failed to open for writing.
        printf("ERROR: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    } 
    else 
    {
        // file is open. write new username to file.
        bool isValidUsername = false;
        while (!isValidUsername)
        {
            cout << "Enter your CSIF username: ";
            cin >> username;

            cout << "Verify by typing it again: ";
            cin >> username_verify;


            if (username == username_verify)
            {
                printf("\nUsername is updating...Done!\n\n");
                isValidUsername = true;
                fout << username;
                fout.close(); // save the file quickly.
            }
        }
    }
}

int main(int argc, char *argv[]) {

    ifstream fin;
    ofstream fout;

    // if config file does not exist yet.
    fin.open(CONFIG_FILE_NAME);
    if ( !fin.is_open() && errno == 2) // no such file or directory.
    {
        // then create the file.
        setUsername(fout);
    } 
    
    // the file is open for read, get username.
    fin >> username;
    fin.close();

    // too few or too many arguments. exit.
    if (argc <= 1 || argc >= 3) {
        printAboutInfo();
        exit(EXIT_FAILURE);
    }

    // GOOD # of arguments. let's go!
    if (strcmp(argv[1], "-config") == STR_MATCH || strcmp(argv[1], "-c") == STR_MATCH) 
    {
        printf("\nCurrent username is: %s\n", username.c_str());
        printf("Do you want to change it? [yes/no] ");
        char response[256];
        cin.getline(response, 256, '\n');
        if (strcmp(response, "yes") == STR_MATCH)
        {
            setUsername(fout);
        } 
        else 
        {
            printf("Quitting...Done!\n");
            exit(EXIT_FAILURE);
        }
    }

    // VALID PC #
    else if (strtoul(argv[1], NULL, 0) >= 1 && strtoul(argv[1], NULL, 0) <= 60) 
    {
        // string sUsername(username);

        // sUsername.pop_back(); // remove newline char at end
        string sArgv1 = argv[1];
        string sCall = "ssh " + username + "@pc" + sArgv1 + ".cs.ucdavis.edu";
        printf("Running %s...\n", sCall.c_str());
        int ret = system(sCall.c_str());
        return ret;
    }
    
    // HELP menu
    else if (strcmp(argv[1], "-help") == STR_MATCH || strcmp(argv[1], "-h") == STR_MATCH)
    {
        printAboutInfo();
    }

    // INVALID argument.
    else
    {
        printf("\nInvalid argument (%s). \nTry $ csif -help\nBye.\n", argv[1]);
    }

    return 0;
}