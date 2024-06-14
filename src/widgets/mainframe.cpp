#include "mainframe.h"
#include "../logic/logic.h"
#include <iostream>
#include <string>
#include <vector>

MainFrame::MainFrame(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 200)) {

  wxPanel *panel = new wxPanel(this, -1);
  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  wxStaticText *abc = new wxStaticText(
      panel, 11, "double click an element in the listbox to see its info");

  listBox = new FileListBox(panel);

  hbox->Add(listBox, 3, wxEXPAND | wxALL, 20);

  panel->SetSizer(hbox);
  Center();
}
