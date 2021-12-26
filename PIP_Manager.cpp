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
#include <thread>
#include <windows.h>
#include <stdexcept>
#include <future>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

using namespace std;

class PIP_Manager: public wxApp {
    public:
        virtual bool OnInit();
};

struct CustomData final : public wxClientData {
    std::string myString;
};

class MainFrame: public wxFrame {
    public:
        MainFrame(const wxString& title);
    private:
        void OnInstall(wxCommandEvent& event);
        void OnUninstall(wxCommandEvent& event);
        void OnUpgrade(wxCommandEvent& event);
        //void Command(char* command);
        wxTextCtrl *txtCtrl;
        wxTextCtrl *txtctrl;
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

bool PIP_Manager::OnInit(){
    MainFrame *frame = new MainFrame(wxT("PIP Manager"));
    frame->Show(true);
    return true;
}

MainFrame::MainFrame(const wxString& title):wxFrame(NULL, -1, title, wxPoint(-1,-1), wxSize(600,600)){
    
    wxPanel *panel = new wxPanel(this, -1);

    wxNotebook *noteBook = new wxNotebook(panel, wxID_ANY);

    wxPanel *managerPanel = new wxPanel(noteBook, 0);

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
    wxBoxSizer *hbox13 = new wxBoxSizer(wxHORIZONTAL);
    vbox1->Add(488,15);
    hbox13->Add(txtctrl);
    vbox1->Add(hbox13, 0, wxALIGN_CENTER_HORIZONTAL);

    wxBoxSizer *hbox14 = new wxBoxSizer(wxHORIZONTAL);
    wxButton *install = new wxButton(managerPanel, ID_INSTALL, wxT("Install"), {20,20},{90,30});
    install->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    //install->SetSize(wxRect(60,60,60,60), wxTOP|wxRIGHT|wxBOTTOM|wxLEFT);

    hbox14->Add(-1,10);
    wxButton *uninstall = new wxButton(managerPanel, ID_UNINSTALL, wxT("Uninstall"), {20,20},{90,30});
    uninstall->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox14->Add(uninstall,0);
    hbox14->Add(-1,10);
    wxButton *upgrade = new wxButton(managerPanel, ID_UPGRADE, wxT("Upgrade"), {20,20},{90,30});
    upgrade->SetBackgroundColour(wxColour(0xFF,0xFF,0xFF));
    hbox14->Add(upgrade, 0);
    hbox14->Add(-1,10);
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
    vbox1->Add(-1,50);
    txtCtrl = new wxTextCtrl(managerPanel, wxID_ANY, wxT(""), wxPoint(-1,-1), wxSize(488,150), wxTE_MULTILINE);
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

    vbox2->Add(-1, 25);

    wxStaticText *greet = new wxStaticText(settingsPanel, wxID_ANY, wxT("Settings"));

    hbox2->Add(greet, 20);
    vbox2->Add(hbox2); 

    settingsPanel->SetSizer(vbox2);

    noteBook->AddPage(settingsPanel, L"Settings");


    wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);
    hbox->Add(noteBook, 1, wxEXPAND);
    panel->SetSizer(hbox);
    

    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->SetMinSize(488,390);
    topSizer->Add(panel, 1, wxEXPAND);

    SetSizerAndFit(topSizer);

    //Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MainFrame::Install, this, wxID_ANY);
}

std::string exec(const char* cmd, string *output) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    cout << result << endl;
    *output = result;
    return result;
}

void Command(char* command, string *output){
    //WinExec(command,1);
    string Scommand = command;
    //auto future = std::async(exec, Scommand);
   // string output = future.get();
    std::cout << "String" << output << endl;
    //exit(0);
}

char* combine(char* str1, char* str2){
    char* command = (char*) malloc((strlen(str1) + strlen(str2)));
    command = strcpy(command, str1);
    command = strcat(command, str2);
    return command;
}

void MainFrame::OnInstall(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char* pip = "pip install ";
    char* command = combine(pip, value);
    if (value == NULL)
        printf("Please enter a valid package\n");
    else {
        txtCtrl->Clear();
        txtCtrl->Refresh();
        if (txtCtrl){
            string output;
            thread th1(exec, command, &output);
            th1.join();
            wxString wxOutput(output.c_str(), wxConvUTF8);
            
            txtCtrl->ChangeValue(wxOutput);
        }
    }
}

void MainFrame::OnUninstall(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char* pip = "pip uninstall ";
    char* yes = " -y";
    char* command = combine(pip, value);
    command = strcat(command, yes);
    if (value == NULL){
        printf("Please enter a valid name\n");
    }else{
        txtCtrl->Clear();
        txtCtrl->Refresh();
        if (txtCtrl){
            string output;
            printf("HERE1");
            cout << command << endl;
            thread th1(exec, command, &output);
            printf("HERE2");
            th1.join();
            printf("HERE3");
            wxString wxOutput(output.c_str(), wxConvUTF8);
            
            txtCtrl->ChangeValue(wxOutput);
        }
    }
}

void MainFrame::OnUpgrade(wxCommandEvent& event){
    char* value = const_cast<char*>(MainFrame::txtctrl->GetValue().ToStdString().c_str());
    char* pip = "pip install ";
    char* upgrade = " --upgrade";
    char* command = combine(pip, value);
    command = (char*) realloc(command, 5*sizeof(command));
    command = strcat(command, upgrade);
 //   std::thread th1(Command, command);
 //   th1.join();
    free(command);
    command=NULL;
}