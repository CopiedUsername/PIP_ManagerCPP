#define __WXMSW__
#define SDL_MAIN_HANDLED
#include <wx/setup.h>
#include <wx/wxprec.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <iostream>
#include <cstdio>
#include <string>
#include <string.h>
#include <thread>
#include <windows.h>
#include <stdexcept>
#include <future>
#include <pthread.h>
#include <vector>
#include <wx/listctrl.h>
#include <wx/dataview.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

using namespace std;

class PIP_Manager: public wxApp {
    public:
        virtual bool OnInit();
};



class MainFrame: public wxFrame {

    
    public:
       // wxTextCtrl *txtCtrl;
        
        MainFrame(const wxString& title);
        void searchListBox(){
            int changed = 0;
            totalLibrariesCount = count;
            const char* text = "";
            while (true){
                if (strcmp(searchText->GetValue().ToStdString().c_str(), text) != 0){
                    text = searchText->GetValue().ToStdString().c_str();
                    count = 0;
                    vector<string> searchedLibraries;
                    for (int i = 0; i < totalLibrariesCount; i++){
                        
                        string query = string(text);
                        transform(query.begin(), query.end(), query.begin(), ::tolower);
                        string potentialResult = installedLibraries[i];
                        transform(potentialResult.begin(), potentialResult.end(), potentialResult.begin(), ::tolower);
                        if (strstr(potentialResult.c_str(), query.c_str())){
                           // cout << text << endl;
                            searchedLibraries.push_back(installedLibraries[i]);
                            count++;
                            
                        }
                        
                    }
                    
                    //free(listCtrl);
                    listCtrl->ClearAll();
                    populateListBox(searchedLibraries, outdatedLibraries);
                    searchedLibraries.clear();
                }
            
            }
        }
        void populateListBox(vector<string> installedLibraries, vector<string> outdatedLibraries){
            wxListItem col0;
            col0.SetId(0);
            col0.SetWidth(450);
            col0.SetText("Installed Packages");
            listCtrl->InsertColumn(0, col0);
            wxListItem *item;
            for (int i = count - 1; i >= 0; i--){
               // cout << installedLibraries[i] << endl;
                //wxString newStr = new wxString(parsed)
                item = new wxListItem();
                item->SetText(installedLibraries[i]);
                item->SetId(0);
                item->SetWidth(200);
                item->SetColumn(0);
                for (int j = 0; j < outdatedCount; j++){
                    if (strcmp(installedLibraries[i].c_str(), outdatedLibraries[j].c_str()) == 0){
                        item->SetTextColour(*wxRED);
                        
                    }
                    
                }
                listCtrl->InsertItem(*item);
                //cout << "EINS" << endl;
            }
        }
        vector<string> getOutdatedLibraries(){
            cout << "executing call" << endl;
            string outdated = exec("pip list -o");

            char outdatedChar[strlen(outdated.c_str())];
            strcpy(outdatedChar, outdated.c_str());
            int startIndex = 0;
            int endIndex = 0;
            outdatedCount = 0;
            int newLineCount = 0;
            int waitForNextLine = 0;
            vector<string> outdatedPackages;
            int j = 0;
            for (int i = 0; i < strlen(outdatedChar); i++){
                if (newLineCount > 2){
                    if (outdatedChar[i] == '\n'){
                        newLineCount++;
                    }
                }else{
                    if (outdatedChar[i]==' ' && !waitForNextLine){
                        endIndex = i;
                        outdatedPackages.push_back(outdated.substr(startIndex, (endIndex-startIndex)));
                        j+=1;
                        outdatedCount++;
                        waitForNextLine = 1;
                    }else if (outdatedChar[i] == '\n'){
                        startIndex = i+1;
                        waitForNextLine = 0;
                    }
                }
            }
            cout << "HERE WE GOOO" << endl;
            for (int i = 0; i < outdatedCount; i++){
                cout << outdatedPackages[i] << endl;
            }

            return outdatedPackages;

        }
        vector<string> getInstalledLibraries(){
            string packages = exec("pip freeze");
            
            char packagesChar[strlen(packages.c_str())];
            strcpy(packagesChar, packages.c_str());
            int startIndex = 0;
            int endIndex = 0;
            count = 0;
            //cout<< "got herehgj" << endl;
            //const char* packagesCSTR = (*packages).c_str();
          //  cout << packages << endl;
            for (int i = 0; i < strlen(packages.c_str()); i++){

                if (packagesChar[i]=='=' && packagesChar[i+1] == '='){
                    count += 1;
                    i+=1;
                }
            }
           // cout << count << endl;
            string packagesStr = string(packages);
           // cout << "got here too" << endl;
            //string parsedPackages[count];
            vector<string> parsedPackages;
            //parsedPackages.insert()
            int j = 0;
            for (int i = 0; i < strlen(packages.c_str()); i++){
                if (packagesChar[i]=='=' && packagesChar[i+1]=='='){
                    endIndex = i;
                    parsedPackages.push_back(packagesStr.substr(startIndex, (endIndex-startIndex)));
                    j+=1;
                    i+=1;
                }else if (packages[i] == '\n'){
                    startIndex = i+1;
                }
            }

            return parsedPackages;
        }
        string exec(const char* cmd) {
            //string* command;
            //Command *command = (Command *) cmd;
            //command = (string*)cmd;
            std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
            if (!pipe) cout << "FAILED" << endl;
            char buffer[128];
            std::string result = "";
            while (!feof(pipe.get())) {
                if (fgets(buffer, 128, pipe.get()) != NULL)
                    result += buffer;
            }
            cout << result << endl;

            wxString wxOutput(result.c_str(), wxConvUTF8);
            //cout << "got here.." << endl;
            // command->txtCtrl->ChangeValue("wxOutput");
            // command->txtCtrl2->ChangeValue("wxOutput");
            // txtCtrl->SetValue(wxString(wxOutput));
            // txtCtrl->Hide();
            cout << result.c_str() << endl;
            return result;
        }
        void* thread_exec(const char* cmd){
            string output = exec(cmd);
           // txtCtrl->ChangeValue(output);
        }
        private:
            void OnInstall(wxCommandEvent& event);
            void OnUninstall(wxCommandEvent& event);
            void OnUpgrade(wxCommandEvent& event);
            //std::shared_ptr<wxTextCtrl> txtCtrl = std::make_shared<wxTextCtrl>();
            wxTextCtrl *txtCtrl;
            wxTextCtrl *txtctrl;
            wxTextCtrl *txtCtrl2;
            wxListBox *listBox;
            wxListCtrl *listCtrl;
            wxTextCtrl *searchText;
            wxListCtrl *tempCtrl;
            wxPanel *listPanel;
            vector<string> outdatedLibraries;
            vector<string> installedLibraries;
            int count = 0;
            int totalLibrariesCount = 0;
            int outdatedCount = 0;
            //void Command(char* command);
            //string installedPackages[];
            wxDECLARE_EVENT_TABLE();
    
};

enum {
    ID_INSTALL, ID_UNINSTALL, ID_UPGRADE
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(ID_INSTALL, MainFrame::OnInstall)
    EVT_BUTTON(ID_UNINSTALL, MainFrame::OnUninstall)
    EVT_BUTTON(ID_UPGRADE, MainFrame::OnUpgrade)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(PIP_Manager);

MainFrame *frame;
wxTextCtrl *txtCtrl;

void* searchThreadHelper(void* arg){
    frame->searchListBox();
}

bool PIP_Manager::OnInit(){
    frame = new MainFrame(wxT("PIP Manager"));
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title):wxFrame(NULL, -1, title, wxPoint(-1,-1), wxSize(600,600)){

    installedLibraries = getInstalledLibraries();

    outdatedLibraries = getOutdatedLibraries();

    wxPanel *panel = new wxPanel(this, -1);

    wxNotebook *noteBook = new wxNotebook(panel, wxID_ANY);

    wxPanel *managerPanel = new wxPanel(noteBook, 0);

    txtCtrl = new wxTextCtrl(managerPanel, wxID_ANY, wxT(""), wxPoint(-1,-1), wxSize(488,150), wxTE_MULTILINE);

    wxBoxSizer *vbox1 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *head = new wxStaticText(managerPanel, wxID_ANY, wxT("PIP Manager App"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE|wxBORDER_NONE);
    wxStaticText *subHead = new wxStaticText(managerPanel, wxID_ANY, wxT("Type in Module Name:"));
    head->SetFont(wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    subHead->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

    vbox1->Add(-1,15);
    hbox1->Add(head,0);

    vbox1->Add(hbox1, 0, wxALIGN_CENTER_HORIZONTAL);
    vbox1->Add(-1,15);
    wxBoxSizer *hbox12 = new wxBoxSizer(wxHORIZONTAL);
    hbox12->Add(subHead);
    vbox1->Add(hbox12, 0, wxALIGN_CENTER_HORIZONTAL);

    txtctrl = new wxTextCtrl(managerPanel, wxID_ANY); //Pip package text box
    txtctrl->SetBackgroundColour(*wxWHITE);
    wxBoxSizer *hbox13 = new wxBoxSizer(wxHORIZONTAL);
    vbox1->Add(30,15,0,20,20);
    hbox13->Add(txtctrl);
    vbox1->Add(hbox13, 0, wxALIGN_CENTER_HORIZONTAL);

    wxGridSizer *hbox14 = new wxGridSizer(1,4,0,20);
    wxButton *install = new wxButton(managerPanel, ID_INSTALL, wxT("Install"), {20,20},{90,30});
    install->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    //install->SetSize(wxRect(60,60,60,60), wxTOP|wxRIGHT|wxBOTTOM|wxLEFT);
    hbox14->Add(install,0);
    wxButton *uninstall = new wxButton(managerPanel, ID_UNINSTALL, wxT("Uninstall"), {20,20},{90,30});
    uninstall->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox14->Add(uninstall,0);
    wxButton *upgrade = new wxButton(managerPanel, ID_UPGRADE, wxT("Upgrade"), {20,20},{90,30});
    upgrade->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox14->Add(upgrade, 0);
    wxButton *import = new wxButton(managerPanel, wxID_ANY, wxT("Import"), {20,20},{90,30});
    import->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox14->Add(import, 0);

    vbox1->Add(-1,15);
    vbox1->Add(hbox14,0,wxALIGN_CENTER_HORIZONTAL);

    wxBoxSizer *hbox15 = new wxBoxSizer(wxHORIZONTAL);
    wxButton *upgradepip = new wxButton(managerPanel, wxID_ANY, wxT("Upgrade PIP"), {20,20},{90,30});
    upgradepip->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox15->Add(upgradepip);
    vbox1->Add(-1,10);
    vbox1->Add(hbox15, 0, wxALIGN_CENTER_HORIZONTAL);


    wxBoxSizer *hbox16 = new wxBoxSizer(wxHORIZONTAL);
    vbox1->Add(-1,70);
    
    txtCtrl->SetEditable(false);
   // txtCtrl->SetBackgroundColour(wxColour(0x00,0x00,0x00));
    txtCtrl->SetForegroundColour(wxColour(0x00,0xFF,0x00));
    txtCtrl->SetBackgroundColour(wxColour(0x00,0x00,0x00));
    hbox16->Add(txtCtrl);
    vbox1->Add(hbox16, wxALIGN_BOTTOM);
    
    managerPanel->SetSizer(vbox1);
    
    

    noteBook->AddPage(managerPanel, L"Manager");

    wxPanel *settingsPanel = new wxPanel(noteBook, 0);
    wxBoxSizer *vbox2 = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox2 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *hbox21 = new wxBoxSizer(wxHORIZONTAL);

    vbox2->Add(-1, 25);

    wxCheckBox *darkmode = new wxCheckBox(settingsPanel, wxID_ANY, wxT("Darkmode"));
    hbox2->Add(darkmode);
    wxCheckBox *CMDOff = new wxCheckBox(settingsPanel, wxID_ANY, wxT("CMD Off"));
    hbox21->Add(CMDOff);
    vbox2->Add(hbox2, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP); 
    vbox2->Add(-1, 5);
    vbox2->Add(hbox21, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_TOP);

    wxBoxSizer *hbox23 = new wxBoxSizer(wxHORIZONTAL);
    vbox2->Add(-1,205);
    txtCtrl2 = new wxTextCtrl(settingsPanel, wxID_ANY, wxT(""), wxPoint(-1,-1), wxSize(488,150), wxTE_MULTILINE);
    txtCtrl2->SetValue(txtCtrl->GetValue());
    txtCtrl2->SetEditable(false);
   // txtCtrl->SetBackgroundColour(wxColour(0x00,0x00,0x00));
    txtCtrl2->SetForegroundColour(wxColour(0x00,0xFF,0x00));
    txtCtrl2->SetBackgroundColour(wxColour(0x00,0x00,0x00));
    hbox23->Add(txtCtrl2);
    vbox2->Add(hbox23, wxALIGN_BOTTOM);

    settingsPanel->SetSizer(vbox2);

    noteBook->AddPage(settingsPanel, L"Settings");

    listPanel = new wxPanel(noteBook);

    wxStaticText *search = new wxStaticText(listPanel, wxID_ANY, wxString("Search"), wxPoint(-1,-1));
    searchText = new wxTextCtrl(listPanel, wxID_ANY);


    wxBoxSizer *text = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *searchBox = new wxBoxSizer(wxHORIZONTAL);

    text->Add(search);
    searchBox->Add(searchText);

    listCtrl = new wxListCtrl(listPanel, wxID_ANY, wxPoint(-1,-1), wxSize(488,150), wxLC_REPORT);
    

    populateListBox(installedLibraries, outdatedLibraries);

    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *hbox37 = new wxBoxSizer(wxHORIZONTAL);
    hbox37->Add(listCtrl);
    vbox->Add(text);
    vbox->Add(searchBox);
    vbox->Add(hbox37);


    listPanel->SetSizer(vbox);

    noteBook->AddPage(listPanel, L"LIST");

    wxBoxSizer *hbox32 = new wxBoxSizer(wxHORIZONTAL);
    hbox32->Add(noteBook, 1, wxEXPAND);
    panel->SetSizer(hbox32);
    

    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->SetMinSize(488,390);
    topSizer->Add(panel, 1, wxEXPAND);

    txtCtrl->ChangeValue("HI");

    SetSizerAndFit(topSizer);

    pthread_t n;
    pthread_create(&n, NULL, searchThreadHelper, NULL);
}





char* combine(char* str1, char* str2){
    char* command = (char*) calloc((strlen(str1) + strlen(str2)), sizeof(char));
    command = strcpy(command, str1);
    command = strcat(command, str2);
    // free(str1);
    // free(str2);
    return command;
}


void *RunCommand(void *arg){
    const char *command = (const char*) arg;
    cout << (char*)arg << endl;
    //cout << command << endl;
    frame->exec(command);
    pthread_exit(NULL);
    free(arg);
    //free(command);
    command = NULL;
    pthread_exit(NULL);
}

void MainFrame::OnInstall(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char pip[] = "pip install ";
    char* command = combine(pip, value);
    char terminate = '\0';
    char* fullCommand2 = combine(command, &terminate);
    cout << fullCommand2 << endl;
    if (value == NULL){
        printf("Please enter a valid package\n");
    }
    else {
        
        if (txtCtrl && txtCtrl2){
            pthread_t tid;
            int count = sizeof(fullCommand2);
            const char *fullCommand = fullCommand2;
            
            //strcpy(fullCommand, fullCommand2);
            cout << fullCommand << endl;
            void *cmd = (void*)(fullCommand);
            cout << (char*) (cmd) << endl;
            pthread_create(&tid, NULL, RunCommand, cref(cmd));
            pthread_detach(tid); //I don't want the program freezing while thread is running
        }
        
        
    }
    free(command);
    free(fullCommand2);
}

void MainFrame::OnUninstall(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char pip[] = "pip uninstall ";
    char yes[] = " -y\0";
    char* command = combine(pip, value);
    char* fullCommand = combine(command, yes);
    //free(command);
    if (value == NULL){
        printf("Please enter a valid name\n");
        free(fullCommand);
    }else{
        if (txtCtrl){
            pthread_t tid;
            cout << fullCommand << endl;
            void *cmd = static_cast<void*>(fullCommand);
            

            pthread_create(&tid, NULL, RunCommand, cmd);
            pthread_detach(tid); //I don't want the program freezing while thread is running
        }
        free(fullCommand);
    }
}

void MainFrame::OnUpgrade(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char pip[] = "pip install ";
    char upgrade[] = " --upgrade\0";
    char* command = combine(pip, value);
    char* fullCommand = combine(command, upgrade);
    free(command);
    if (value == NULL){
        printf("Please enter a valid name\n");
    }else{
        if (txtCtrl){
            pthread_t tid;
            void *cmd = static_cast<void*>(fullCommand);
            
            pthread_create(&tid, NULL, RunCommand, cmd);
            pthread_detach(tid); //I don't want the program freezing while thread is running
        }
    }
    free(command);
    command = NULL;
}
