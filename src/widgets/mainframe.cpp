#include "mainframe.h"
#include "../logic/logic.h"
#include <iostream>
#include <string>
#include <vector>

MainFrame::MainFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 200))
{

  wxPanel * panel = new wxPanel(this, -1);

  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  listbox = new wxListBox(panel, ID_LISTBOX, 
      wxPoint(-1, -1), wxSize(-1, -1));

    std::vector<std::string> files = GetFiles();
    for (int i = 0; i < files.size(); i++) {
        listbox->Append(files[i]);
    }

  hbox->Add(listbox, 3, wxEXPAND | wxALL, 20);

  panel->SetSizer(hbox);
  Center();  
}
